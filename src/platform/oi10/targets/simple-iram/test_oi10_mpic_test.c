/*
 * test_oi10_mpic_test.c
 *
 *  Created on: Feb 20, 2019
 *      Author: v.fufaev
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/rumboot.h>
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
    TEC_SET_EXT_INT3,
    TEC_SET_EXT_INT4,
    TEC_SET_EXT_INT5,
    TEC_SET_EXT_INT01,
    TEC_SET_EXT_INT23,
    TEC_SET_EXT_INT45,
    TEC_SET_EXT_INT13,
    TEC_SET_EXT_INT15,
    TEC_SET_EXT_INT35,
    TEC_SET_EXT_INT012345,
    TEC_CLR_EXT_INT0,
    TEC_CLR_EXT_INT1,
    TEC_CLR_EXT_INT2,
    TEC_CLR_EXT_INT3,
    TEC_CLR_EXT_INT4,
    TEC_CLR_EXT_INT5,
    TEC_CLR_EXT_INT_ALL,
    TEC_SET_EXT_INT4_AFTER_WR_IAR,
    TEC_SET_EXT_INT5_AFTER_WR_IAR,
    TEC_SET_EXT_INT45_AFTER_WR_IAR
} test_event_code;

static struct rumboot_irq_entry *tbl = NULL;

uint32_t arg0 = TEC_CLR_EXT_INT0;
uint32_t arg1 = TEC_CLR_EXT_INT1;
uint32_t arg2 = TEC_CLR_EXT_INT2;
uint32_t arg3 = TEC_CLR_EXT_INT3;
uint32_t arg4 = TEC_CLR_EXT_INT4;
uint32_t arg5 = TEC_CLR_EXT_INT5;

static void exception_handler(int const id, char const * const name ) {
    rumboot_printf( "Exception: %s\n", name );
    spr_write( SPR_MCSR_C, spr_read(SPR_MCSR_RW) );
}

void irq_handler( int irq_num, void *arg )
{
    rumboot_printf("IRQ #%d received.\n", irq_num-EXT_INT0);
    test_event(*((uint32_t*)arg));
//    test_event(TEC_CLR_EXT_INT_ALL);
}

int main ()
{
    test_event_send_test_id("test_oi10_mpic_test");
    rumboot_memfill8_modelling((void*)SRAM0_BASE, 0x1000, 0x00, 0x00); //workaround (init 4KB)

    rumboot_printf("Set our own irq handlers... ");
    rumboot_irq_set_exception_handler(exception_handler);
    rumboot_irq_cli();
    tbl = rumboot_irq_create( NULL );
    rumboot_irq_set_handler( tbl, EXT_INT0, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH | RUMBOOT_IRQ_PRIOR_3, irq_handler, &arg0 );
    rumboot_irq_set_handler( tbl, EXT_INT1, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH | RUMBOOT_IRQ_PRIOR_4, irq_handler, &arg1 );
    rumboot_irq_set_handler( tbl, EXT_INT2, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH | RUMBOOT_IRQ_PRIOR_6, irq_handler, &arg2 );
    rumboot_irq_set_handler( tbl, EXT_INT3, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH | RUMBOOT_IRQ_PRIOR_7, irq_handler, &arg3 );
    rumboot_irq_set_handler( tbl, EXT_INT4, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH | RUMBOOT_IRQ_PRIOR_12, irq_handler, &arg4 );
    rumboot_irq_set_handler( tbl, EXT_INT5, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH | RUMBOOT_IRQ_PRIOR_14, irq_handler, &arg5 );
    rumboot_irq_table_activate( tbl );
    mpic128_set_interrupt_borders( DCR_MPIC128_BASE, MPIC128_PRIOR_11, MPIC128_PRIOR_5 );

    rumboot_irq_sei();
    rumboot_printf("done\n");

    rumboot_printf("---------------- Info ------------------------\n");
    rumboot_printf("IRQ #0 is Non critical Interrupt, priority 3\n"
                   "IRQ #1 is Non critical Interrupt, priority 4\n");
    rumboot_printf("IRQ #2 is Critical Interrupt, priority 6\n"
                   "IRQ #3 is Critical Interrupt, priority 7\n");
    rumboot_printf("IRQ #4 is Machine Check Interrupt, priority 12\n"
                   "IRQ #5 is Machine Check Interrupt, priority 14\n");
    rumboot_printf("----------------------------------------------\n");

#define EXT_INT_TEST_COUNT 2
#define EXT_INT_TIMEOUT 32

    for (int k = 0; k < EXT_INT_TEST_COUNT; ++k)
    {
        test_event(TEC_SET_EXT_INT0);
        for (int i = 0; i < EXT_INT_TIMEOUT; ++i) { nop(); }
        test_event(TEC_SET_EXT_INT1);
        for (int i = 0; i < EXT_INT_TIMEOUT; ++i) { nop(); }
        test_event(TEC_SET_EXT_INT2);
        for (int i = 0; i < EXT_INT_TIMEOUT; ++i) { nop(); }
        test_event(TEC_SET_EXT_INT3);
        for (int i = 0; i < EXT_INT_TIMEOUT; ++i) { nop(); }
        test_event(TEC_SET_EXT_INT4);
        for (int i = 0; i < EXT_INT_TIMEOUT; ++i) { nop(); }
        test_event(TEC_SET_EXT_INT5);
        for (int i = 0; i < EXT_INT_TIMEOUT; ++i) { nop(); }
        test_event(TEC_SET_EXT_INT01);
        for (int i = 0; i < EXT_INT_TIMEOUT; ++i) { nop(); }
        test_event(TEC_SET_EXT_INT23);
        for (int i = 0; i < EXT_INT_TIMEOUT; ++i) { nop(); }
        test_event(TEC_SET_EXT_INT45);
        for (int i = 0; i < EXT_INT_TIMEOUT; ++i) { nop(); }
        test_event(TEC_SET_EXT_INT13);
        for (int i = 0; i < EXT_INT_TIMEOUT; ++i) { nop(); }
        test_event(TEC_SET_EXT_INT15);
        for (int i = 0; i < EXT_INT_TIMEOUT; ++i) { nop(); }
        test_event(TEC_SET_EXT_INT35);
        for (int i = 0; i < EXT_INT_TIMEOUT; ++i) { nop(); }
        test_event(TEC_SET_EXT_INT012345);
        for (int i = 0; i < EXT_INT_TIMEOUT; ++i) { nop(); }
        test_event(TEC_SET_EXT_INT4_AFTER_WR_IAR);
        test_event(TEC_SET_EXT_INT3);
        for (int i = 0; i < EXT_INT_TIMEOUT; ++i) { nop(); }
        test_event(TEC_SET_EXT_INT45_AFTER_WR_IAR);
        test_event(TEC_SET_EXT_INT3);
        for (int i = 0; i < EXT_INT_TIMEOUT; ++i) { nop(); }
    }

    return 0;
}

