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

#include <platform/devices.h>
#include <platform/interrupts.h>
#include <platform/arch/ppc/test_macro.h>
#include <platform/test_event_c.h>
#include <platform/trace.h>
#include <platform/test_assert.h>
#include <platform/arch/ppc/ppc_476fp_debug_fields.h>
#include <platform/arch/ppc/ppc_476fp_itrpt_fields.h>

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

static struct rumboot_irq_entry *tbl;

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

static void check_machinecheck_handler( int irq, void *args ) {
    rumboot_putstring( "check_machinecheck_handler\n" );

}

int check_machinecheck ()
{
	rumboot_printf("DEBUG_MACHINECHECK check start!\n");

    rumboot_irq_cli();
    tbl = rumboot_irq_create(NULL);
    rumboot_irq_set_handler(tbl, L2C0_MCHKOUT, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, check_machinecheck_handler, NULL);
    rumboot_irq_table_activate(tbl);
    rumboot_irq_enable(L2C0_MCHKOUT);
    rumboot_irq_sei();





	rumboot_platform_event_clear();
	rumboot_printf("send TEC_CHECK_DEBUG_MACHINECHECK\n");


	test_event(TEC_CHECK_DEBUG_MACHINECHECK );
	if(event_get()) return 1;

	rumboot_printf("DEBUG_MACHINECHECK check done!\n");
/*
    rumboot_irq_cli();
    rumboot_irq_table_activate(NULL);
    rumboot_irq_free(tbl);
    rumboot_irq_sei();
*/

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
