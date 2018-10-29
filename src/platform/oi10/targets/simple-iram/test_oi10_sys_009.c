/*
 * test_oi10_sys_009.c
 *
 *  Created on: Sep 5, 2018
 *      Author: d.klepikov
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <platform/devices.h>

#include <platform/arch/ppc/test_macro.h>
#include <platform/test_event_c.h>
#include <platform/trace.h>
#include <platform/test_assert.h>
#include <platform/arch/ppc/ppc_476fp_timer_fields.h>
#include <platform/arch/ppc/ppc_476fp_debug_fields.h>
#include <platform/arch/ppc/ppc_476fp_lib_c.h>
#include <platform/arch/ppc/ppc_476fp_config.h>
#include <platform/arch/ppc/ppc_476fp_mmu_fields.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>
#include <platform/ppc470s/mmu/mem_window.h>

#include <platform/devices/emi.h>
#include <platform/regs/regs_emi.h>
#include <platform/regs/fields/emi.h>

#define PHY2RPN(ADDR)      	(((ADDR) & 0xFFFFF000) >> 12)
#define ADDR2EPN			PHY2RPN

#define PHY_BASE			(SSRAM_BASE)

#define ICACHEWR_TEST_DATA	(0x1234)
#define ICACHEWR_TEST_ADDR	(PHY_BASE)

#define FASTWR_DATA_ADDR  	(PHY_BASE + 0x100)
#define FASTWR_DATA0		(0x12345678)
#define FASTWR_DATA1 		(0xC0DEBEEF)
#define FASTWR_DATA2		(0x55555555)
#define FASTWR_DATA3		(0xAAAAAAAA)

/*                          MMU_TLB_ENTRY( ERPN, RPN,              EPN,               DSIZ,              IL1I,IL1D,W,I,M,G,E,                 UX,UW,UR,SX,SW,SR DULXE,IULXE,TS,TID,         WAY,              BID,             V )*/
#define TLB_ENTRY_CACHE_ON  MMU_TLB_ENTRY( 0x000,PHY2RPN(PHY_BASE),ADDR2EPN(PHY_BASE),MMU_TLBE_DSIZ_64KB,0,   0,   1,0,1,0,MMU_TLBE_E_BIG_END,0, 0, 0, 1, 1, 1, 0,    0,    0, MEM_WINDOW_0,MMU_TLBWE_WAY_UND,MMU_TLBWE_BE_UND,1 )

enum {
       TEC_CHECK_DEBUG_MASKOUT = TEST_EVENT_CODE_MIN,
       TEC_CHECK_DEBUG_SYSTEMCONTROL,
       TEC_CHECK_DEBUG_JDCR_STO,
       TEC_CHECK_DEBUG_JDCR_UDE,
       TEC_CHECK_DEBUG_JDCR_RSDBSR,
       TEC_CHECK_DEBUG_STUFF,
       TEC_CHECK_DEBUG_DBDR,
       TEC_CHECK_DEBUG_DBIMASK,
       TEC_CHECK_DEBUG_ICACHEWR,
       TEC_CHECK_DEBUG_FASTWR
} test_event_code;

enum {
   	TEST_DATA_PROGG     = 0xFFFFFFFF,
   	TEST_DATA_OK        = 0x12345678,
	TEST_DATA_EVENT     = 0x00000002,
   	TEST_DATA_ERROR     = 0x8E5917E8
} test_data;

static const tlb_entry tlb_entry_cache_on = {TLB_ENTRY_CACHE_ON};

int event_get()
{
	enum rumboot_simulation_event rumboot_event;
	volatile uint32_t *data;
	rumboot_event = rumboot_platform_event_get(&data);
	if (rumboot_event != EVENT_TESTEVENT ){
		rumboot_printf("Error event!\n");
		return 1;
	}
	if (data [0] != TEST_DATA_EVENT){
		rumboot_printf("Error event!\n");
		return 1;
	}
	return 0;
}

int check_dbdr ()
{
	uint32_t dbdr;

	if(event_get()) return 1;

	/*read dbdr*/
	dbdr = spr_read(SPR_DBDR);
	if (dbdr != TEST_DATA_OK ){
		rumboot_printf("DBDR check failed!\n");
		return 1;
	}

	/*write dbdr for test*/
	spr_write(SPR_DBDR, TEST_DATA_PROGG);
	test_event(TEC_CHECK_DEBUG_DBDR );
	return 0;
}

int check_jdcr ()
{
	uint32_t dbdr, dbsr;

	rumboot_printf("JDSR check start!\n");
	spr_write(SPR_DBDR, TEST_DATA_ERROR);

	rumboot_platform_event_clear();
	rumboot_printf("send TEC_CHECK_DEBUG_JDCR_STO\n");
	test_event(TEC_CHECK_DEBUG_JDCR_STO );
	if(event_get()) return 1;
	rumboot_printf("got event\n");

	dbdr = spr_read(SPR_DBDR);
	if (dbdr != TEST_DATA_OK ){
		rumboot_printf("JDCR[STO] check failed!\n");
		return 1;
	}

	rumboot_platform_event_clear();
	rumboot_printf("send TEC_CHECK_DEBUG_JDCR_UDE\n");
	test_event(TEC_CHECK_DEBUG_JDCR_UDE );
	if(event_get()) return 1;
	rumboot_printf("got event\n");

	dbsr = spr_read(SPR_DBSR_RC);
	spr_write(SPR_DBSR_RC,0xFFFFFFFF); //clear dbsr
	if (!(dbsr & (1 << DEBUG_DBSR_UDE_i)) ){
		rumboot_printf("DBSR UDE is not set from JDCR!\n");
		return 1;
	}
	dbsr = spr_read(SPR_DBSR_RC);
	if (dbsr){
		rumboot_printf("DBSR is not clear (DBSR = 0x%X)!\n", dbsr);
		return 1;
	}

	spr_write(SPR_DBSR_W,1 << DEBUG_DBSR_UDE_i);
	dbsr = spr_read(SPR_DBSR_RC);
	if (!(dbsr & (1 << DEBUG_DBSR_UDE_i)) ){
		rumboot_printf("DBSR UDE is not writen!\n");
		return 1;
	}

	rumboot_platform_event_clear();
	rumboot_printf("send TEC_CHECK_DEBUG_JDCR_RSDBSR\n");
	test_event(TEC_CHECK_DEBUG_JDCR_RSDBSR );
	if(event_get()) return 1;
	rumboot_printf("got event\n");

	dbsr = spr_read(SPR_DBSR_RC);
	if (dbsr){
		rumboot_printf("DBSR reset check failed!\n");
		return 1;
	}

	rumboot_platform_event_clear();
	rumboot_printf("send TEC_CHECK_DEBUG_SYSTEMCONTROL\n");
	test_event(TEC_CHECK_DEBUG_SYSTEMCONTROL );
	if(event_get()) return 1;
	rumboot_printf("got event\n");

	rumboot_printf("JDSR check done!\n");
	return 0;
}

int check_dbimask ()
{
	uint32_t dbdr;

	rumboot_printf("DBIMASK check start!\n");
	spr_write(SPR_DBDR, TEST_DATA_ERROR);

	rumboot_platform_event_clear();
	rumboot_printf("send TEC_CHECK_DEBUG_DBIMASK\n");
	test_event(TEC_CHECK_DEBUG_DBIMASK );
//	if(event_get()) return 1;
//	rumboot_printf("got event\n");

	dbdr = spr_read(SPR_DBDR);
	if (dbdr != TEST_DATA_OK ){
		rumboot_printf("DBIMASK check failed!\n");
		return 1;
	}
	rumboot_printf("DBIMASK check done!\n");

	return 0;
}

int check_dbomask ()
{
//	uint32_t dbdr;

	rumboot_printf("DBOMASK check start!\n");
//	spr_write(SPR_DBDR, TEST_DATA_ERROR);

	rumboot_platform_event_clear();
	rumboot_printf("send TEC_CHECK_DEBUG_MASKOUT\n");
	test_event(TEC_CHECK_DEBUG_MASKOUT );
	if(event_get()) return 1;
	rumboot_printf("got event\n");

//	dbdr = spr_read(SPR_DBDR);
//	if (dbdr != TEST_DATA_OK ){
//		rumboot_printf("DBIMASK check failed!\n");
//		return 1;
//	}
	rumboot_printf("DBOMASK check done!\n");

	return 0;
}

int check_stuff ()
{
	uint32_t stuff;

	spr_write(SPR_DBDR, TEST_DATA_OK);

	rumboot_platform_event_clear();
	test_event(TEC_CHECK_DEBUG_STUFF );

	if(event_get()) return 1;

	//mfspr r4, SPR_DBDR
	//mtspr SPR_USPRG0, r4

	stuff = spr_read(SPR_USPRG0);
    if (stuff != TEST_DATA_OK ){
            rumboot_printf("The stuff instruction check failed!\n");
            return 1;
    }

	return 0;
}

int check_icachewr ()
{
    uint32_t test_data, i;
    volatile uint32_t *stuff = (uint32_t*)(ICACHEWR_TEST_ADDR);

    for (i=0; i < 8; i++)
    	stuff[i] = 0x60000000;  //nop
    msync();
    isync();

	spr_write(SPR_USPRG0, &stuff[0]);

	rumboot_platform_event_clear();
	test_event(TEC_CHECK_DEBUG_ICACHEWR );

	if(event_get()) return 1;
	rumboot_printf("ICACHEWR: got event!\n");

	test_data = spr_read(SPR_USPRG0);
	rumboot_printf("SPR_USPRG0 = 0x%X\n", test_data);

    if (test_data != ICACHEWR_TEST_DATA ){
            rumboot_printf("The icachewr instruction check failed!\n");
            return 1;
    }

    rumboot_printf("ICACHEWR: Test done.\n");
	return 0;
}

int check_fastwr ()
{
    uint32_t *test_data;
    rumboot_printf("FASTWR: Test started...\n");

	rumboot_platform_event_clear();
	test_event(TEC_CHECK_DEBUG_FASTWR );

	if(event_get()) return 1;
	rumboot_printf("FASTWR: got event!\n");

    test_data = (uint32_t *)(FASTWR_DATA_ADDR);
    TEST_ASSERT(test_data[0] == FASTWR_DATA0, "Invalid value in test_data[0]");
    TEST_ASSERT(test_data[1] == FASTWR_DATA1, "Invalid value in test_data[1]");
    TEST_ASSERT(test_data[2] == FASTWR_DATA2, "Invalid value in test_data[2]");
    TEST_ASSERT(test_data[3] == FASTWR_DATA3, "Invalid value in test_data[3]");

    rumboot_printf("FASTWR: Test done.\n");
	return 0;
}

int main()
{
	test_event_send_test_id("test_oi10_sys_009");

	rumboot_printf("Init EMI2...\n");
    emi_init(DCR_EM2_EMI_BASE);

    rumboot_printf("Cache on...\n");
    write_tlb_entries(&tlb_entry_cache_on, 1);

	if(check_dbdr()) {
		rumboot_printf("DBDR test failed!");
		return 1;
	}
	if(check_jdcr()) {
		rumboot_printf("JDCR test failed!");
		return 1;
	}
	if(check_dbimask()) {
		rumboot_printf("DBIMask test failed!");
		return 1;
	}
	if(check_dbomask()) {
		rumboot_printf("DBOMask test failed!");
		return 1;
	}
	if(check_stuff()) {
		rumboot_printf("STUFF test failed!");
		return 1;
	}
	if(check_icachewr()) {
		rumboot_printf("ICACHEWR test failed!");
		return 1;
	}
	if(check_fastwr()) {
		rumboot_printf("FASTWR test failed!");
		return 1;
	}

	rumboot_printf("TEST OK\n");

    return 0;
}
