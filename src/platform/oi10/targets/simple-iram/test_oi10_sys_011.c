/*
 * test_oi10_sys_011.c
 *
 *  Created on: Oct 26, 2018
 *      Author: v.fufaev
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <rumboot/irq.h>

#include <platform/test_event_c.h>
#include <platform/trace.h>
#include <platform/test_assert.h>
#include <platform/devices.h>
#include <platform/interrupts.h>
#include <platform/arch/ppc/test_macro.h>
#include <platform/arch/ppc/ppc_476fp_config.h>
#include <platform/arch/ppc/ppc_476fp_lib_c.h>
#include <platform/arch/ppc/ppc_476fp_debug_fields.h>
#include <platform/arch/ppc/ppc_476fp_itrpt_fields.h>
#include <platform/arch/ppc/ppc_476fp_ctrl_fields.h>

enum {
       TEC_CHECK_DEBUG_MACHINECHECK = TEST_EVENT_CODE_MIN,
       TEC_CHECK_DEBUG_HALT,
       TEC_CHECK_DEBUG_UNCONDEVENT,
       TEC_CHECK_DEBUG_SYSTEMSTATUS
} test_event_code;

enum {
   	TEST_DATA_PROGG     = 0xFFFFFFFF,
   	TEST_DATA_OK        = 0x12345678,
	TEST_DATA_EVENT     = 0x00000002,
   	TEST_DATA_ERROR     = 0x8E5917E8
} test_data;

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

int check_systemstatus ()
{
	rumboot_printf("DEBUG_SYSTEMSTATUS check start!\n");

	rumboot_platform_event_clear();
	rumboot_printf("send TEC_CHECK_DEBUG_SYSTEMSTATUS\n");
	test_event(TEC_CHECK_DEBUG_SYSTEMSTATUS );
	if(event_get()) return 1;

	rumboot_printf("DEBUG_SYSTEMSTATUS check done!\n");
	return 0;
}

int check_uncondevent ()
{
	uint32_t dbsr;
	rumboot_printf("DEBUG_UNCONDEVENT check start!\n");

	spr_write(SPR_DBSR_RC,0xFFFFFFFF); //clear dbsr
	rumboot_printf("send TEC_CHECK_DEBUG_UNCONDEVENT\n");
	test_event(TEC_CHECK_DEBUG_UNCONDEVENT );

	dbsr = spr_read(SPR_DBSR_RC);
	spr_write(SPR_DBSR_RC,0xFFFFFFFF); //clear dbsr
	if (!(dbsr & (1 << DEBUG_DBSR_UDE_i)) ){
		rumboot_printf("DBSR UDE is not set!\n");
		return 1;
	}

	rumboot_printf("DEBUG_UNCONDEVENT check done!\n");
	return 0;
}

int check_halt ()
{
	uint32_t dbdr;

	rumboot_printf("DEBUG_HALT check start!\n");
	spr_write(SPR_DBDR, TEST_DATA_ERROR);

	rumboot_platform_event_clear();
	rumboot_printf("send TEC_CHECK_DEBUG_HALT\n");
	test_event(TEC_CHECK_DEBUG_HALT );

	dbdr = spr_read(SPR_DBDR);
	if (dbdr != TEST_DATA_OK ){
		rumboot_printf("DEBUG_HALT check failed!\n");
		return 1;
	}
	rumboot_printf("DEBUG_HALT check done!\n");
	return 0;
}

int check_machinecheck ()
{
	uint32_t const msr_old_value = msr_read();
	uint32_t const ccr1_old_value = spr_read(SPR_CCR1);;

	rumboot_printf("DEBUG_MACHINECHECK check start!\n");

	rumboot_printf("send TEC_CHECK_DEBUG_MACHINECHECK\n");
	test_event(TEC_CHECK_DEBUG_MACHINECHECK );

	//Check C470S_DBGMACHINECHECK via ESR[ISMC]
	spr_write(SPR_ESR,0x80000000);
	spr_write(SPR_ESR,0x00000000);

	//Check C470S_DBGMACHINECHECK via MCSR[MCS]
	spr_write(SPR_MCSR_C,0xFFFFFFFF);
	spr_write(SPR_MCSR_RW,0x80000000);
	spr_write(SPR_MCSR_C,0xFFFFFFFF);

	//Other check C470S_DBGMACHINECHECK
	msr_write( (0b1 << ITRPT_XSR_FP_i)   /* MSR[FP] - Floating point available. */
             | (0b0 << ITRPT_XSR_ME_i)); /* MSR[ME] - Machine check enable.     */

	asm volatile ("lfs  1, 0(13)\n\t");
	asm volatile ("lfs  2, 0(13)\n\t");
	msync();

	spr_write( SPR_CCR1, ccr1_old_value | (0b11 << CTRL_CCR1_FPRPEI_i)); //Floating-Point Register (FPR) parity error insert.
	isync();

	asm volatile ("lfs  1, 0(13)\n\t");
	asm volatile ("lfs  2, 0(13)\n\t");
	msync();

	spr_write( SPR_CCR1, ccr1_old_value);
	isync();

	asm volatile ("fcmpu 0, 1, 2\n\t");

	spr_write(SPR_MCSR_C,0xFFFFFFFF);
	msr_write(msr_old_value);

	rumboot_printf("DEBUG_MACHINECHECK check done!\n");

	return 0;
}

int main()
{
	test_event_send_test_id("test_oi10_sys_011");

	if(check_systemstatus()) {
		rumboot_printf("DEBUG_SYSTEMSTATUS test failed!");
		return 1;
	}
	if(check_uncondevent()) {
		rumboot_printf("DEBUG_UNCONDEVENT test failed!");
		return 1;
	}
	if(check_halt()) {
		rumboot_printf("DEBUG_HALT test failed!");
		return 1;
	}
	if(check_machinecheck()) {
		rumboot_printf("DEBUG_MACHINECHECK test failed!");
		return 1;
	}

	rumboot_printf("TEST OK\n");
    return 0;
}
