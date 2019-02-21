/*
 * test_oi10_mpic_test.c
 *
 *  Created on: Feb 20, 2019
 *      Author: v.fufaev
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/irq.h>

#include <platform/common_macros/common_macros.h>

#include <arch/ppc_476fp_config.h>
#include <arch/ppc_476fp_lib_c.h>

#include <platform/test_assert.h>
#include <platform/test_event_c.h>
#include <platform/test_event_codes.h>
#include <platform/test_assert.h>
#include <platform/trace.h>
#include <platform/devices.h>
#include <platform/interrupts.h>
#include <platform/devices/mpic128.h>

#include <platform/regs/fields/mpic128.h>

enum {
    TEC_SET_EXT_INT0 = TEST_EVENT_CODE_MIN,
    TEC_SET_EXT_INT1,
    TEC_SET_EXT_INT2,
    TEC_SET_EXT_INT01,
    TEC_SET_EXT_INT12,
    TEC_SET_EXT_INT02,
    TEC_SET_EXT_INT012,
    TEC_CLR_EXT_INT0,
    TEC_CLR_EXT_INT1,
    TEC_CLR_EXT_INT2,
    TEC_CLR_EXT_INT012
} test_event_code;

static struct rumboot_irq_entry *tbl = NULL;

uint32_t arg0 = TEC_CLR_EXT_INT0;
uint32_t arg1 = TEC_CLR_EXT_INT1;
uint32_t arg2 = TEC_CLR_EXT_INT2;

static void exception_handler(int const id, char const * const name ) {
    rumboot_printf( "Exception: %s\n", name );
    spr_write( SPR_MCSR_C, spr_read(SPR_MCSR_RW) );
}

void irq_handler( int irq_num, void *arg )
{
    rumboot_printf("IRQ #%d received.\n", irq_num-66);
//    test_event(*((uint32_t*)arg));
    test_event(TEC_CLR_EXT_INT012);
}

int main ()
{
    test_event_send_test_id("test_oi10_mpic_test");
    rumboot_memfill8_modelling((void*)SRAM0_BASE, 0x1000, 0x00, 0x00); //workaround (init 4KB)

    rumboot_printf("Set our own irq handlers... ");
    rumboot_irq_set_exception_handler(exception_handler);
    rumboot_irq_cli();
    tbl = rumboot_irq_create( NULL );
    rumboot_irq_set_handler( tbl, EXT_INT0, RUMBOOT_IRQ_EDGE | RUMBOOT_IRQ_POS | RUMBOOT_IRQ_PRIOR_3, irq_handler, &arg0 );
    rumboot_irq_set_handler( tbl, EXT_INT1, RUMBOOT_IRQ_EDGE | RUMBOOT_IRQ_POS | RUMBOOT_IRQ_PRIOR_8, irq_handler, &arg1 );
    rumboot_irq_set_handler( tbl, EXT_INT2, RUMBOOT_IRQ_EDGE | RUMBOOT_IRQ_POS | RUMBOOT_IRQ_PRIOR_12, irq_handler, &arg2 );
    rumboot_irq_table_activate( tbl );
    mpic128_set_interrupt_borders( DCR_MPIC128_BASE, MPIC128_PRIOR_11, MPIC128_PRIOR_5 );

    rumboot_irq_sei();
    rumboot_printf("done\n");

    rumboot_printf("Info: IRQ #0 is Non critical Interrupt\n");
    rumboot_printf("Info: IRQ #1 is Critical Interrupt\n");
    rumboot_printf("Info: IRQ #2 is Machine Check Interrupt\n");

#define EXT_INT_TEST_COUNT 1
#define EXT_INT_TIMEOUT 32

    for (int k = 0; k < EXT_INT_TEST_COUNT; ++k)
    {
        test_event(TEC_SET_EXT_INT0);
        for (int i = 0; i < EXT_INT_TIMEOUT; ++i) { nop(); }
        test_event(TEC_SET_EXT_INT1);
        for (int i = 0; i < EXT_INT_TIMEOUT; ++i) { nop(); }
        test_event(TEC_SET_EXT_INT2);
        for (int i = 0; i < EXT_INT_TIMEOUT; ++i) { nop(); }
        test_event(TEC_SET_EXT_INT01);
        for (int i = 0; i < EXT_INT_TIMEOUT; ++i) { nop(); }
        test_event(TEC_SET_EXT_INT12);
        for (int i = 0; i < EXT_INT_TIMEOUT; ++i) { nop(); }
        test_event(TEC_SET_EXT_INT02);
        for (int i = 0; i < EXT_INT_TIMEOUT; ++i) { nop(); }
        test_event(TEC_SET_EXT_INT012);
        for (int i = 0; i < EXT_INT_TIMEOUT; ++i) { nop(); }
    }

    return 0;
}

