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

#include <platform/arch/ppc/test_macro_asm.S.h>
#include <platform/test_event_c.h>
#include <platform/trace.S.h>
#include <platform/test_assert.S.h>
#include <platform/arch/ppc/ppc_476fp_timer_fields.h>
#include <platform/arch/ppc/ppc_476fp_lib_c.h>
#include <platform/arch/ppc/ppc_476fp_config.h>



   typedef enum
       {
       TEC_CHECK_DEBUG_GRPHLTR_GRPHLTS = TEST_EVENT_CODE_MIN,
       TEC_CHECK_DEBUG_GRP0,
       TEC_CHECK_DEBUG_GRP1,
       TEC_CHECK_DEBUG_LDBO,
       TEC_CHECK_DEBUG_MACHINECHECK,
       TEC_CHECK_DEBUG_MASKOUT,
       TEC_CHECK_DEBUG_HALT,
       TEC_CHECK_DEBUG_UNCONDEVENT,
       TEC_CHECK_DEBUG_SYSTEMCONTROL,
       TEC_CHECK_DEBUG_JDCR_STO,
       TEC_CHECK_DEBUG_JDCR_UDE,
       TEC_CHECK_DEBUG_JDCR_RSDBSR,
       TEC_CHECK_DEBUG_SYSTEMSTATUS,
       TEC_CHECK_DEBUG_STUFF,
       TEC_CHECK_DEBUG_DBDR,
       TEC_CHECK_DEBUG_DBIMASK,
       TEC_CHECK_DEBUG_ICACHEWR,
       TEC_CHECK_DEBUG_FASTWR
   } test_event_code;


   typedef enum
   	{
   	TEST_DATA_PROGG     = 0xFFFFFFFF,
   	TEST_DATA_OK        = 0x00000001,
	TEST_DATA_EVENT     = 0x00000001,
   	TEST_DATA_ERROR     = 0x8E5917E8
   } test_data;

int main()
{
	test_event_send_test_id("test_oi10_sys_009");

    rumboot_printf("Hello world from IRAM!\n");

    volatile uint32_t *data;
    enum rumboot_simulation_event rumboot_event;
    rumboot_event = rumboot_platform_event_get(&data);
	if (rumboot_event != EVENT_TESTEVENT ){
		rumboot_printf("Error event!\n");
		return 1;
	}
	if (data [0] != TEST_DATA_EVENT){
		rumboot_printf("Error event!\n");
		return 1;
	}
	/*schitay dbdr*/
	uint32_t dbdr = spr_read(SPR_DBDR);
	if (dbdr != TEST_DATA_OK ){
		rumboot_printf("Error DBDR_TEST!\n");
		return 1;
	}

	/*prover' dbdr*/
	spr_write(SPR_DBDR, TEST_DATA_PROGG);

	test_event(TEC_CHECK_DEBUG_DBDR );

    return 0;
}
