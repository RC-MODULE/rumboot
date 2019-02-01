#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <rumboot/printf.h>
#include <rumboot/timer.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>

#include <platform/test_assert.h>
#include <platform/test_event_c.h>
#include <platform/test_event_codes.h>
#include <platform/test_assert.h>
#include <platform/trace.h>

#include <platform/common_macros/common_macros.h>

#include <arch/ppc_476fp_config.h>
#include <arch/ppc_476fp_lib_c.h>
#include <platform/arch/ppc/ppc_476fp_mmu_fields.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>

#include <platform/devices.h>
#include <platform/devices/emi.h>
#include <platform/devices/mpic128.h>
#include <platform/regs/fields/emi.h>
#include <platform/interrupts.h>

#include <devices/sp804.h>

#define DEC_START_VALUE  0x1000000
#define MPIC_START_VALUE 0x1000

volatile uint32_t DEC_CONTROL_VALUE = 0;
volatile uint32_t INT_FLAG = 0;

static void dec_clr_exception (void) {
    spr_write (SPR_TSR_RC, 1 << IBM_BIT_INDEX(64, 36)); //TSR[DIS]=0 - clear DEC exceptions
}

static void dec_set_value (uint32_t delay) {
    spr_write (SPR_DEC, delay);
}

static void mpic_tim0_handler()
{
    rumboot_printf("Interrupt handler (MPIC Timer0)\n");
    mpic128_stop_timer(DCR_MPIC128_BASE, Mpic128Timer0);
    DEC_CONTROL_VALUE = spr_read (SPR_DEC);
    dec_set_value (0x00);
    dec_clr_exception();
    INT_FLAG = 1;
}

static void init_handlers()
{
    rumboot_irq_cli();
    struct rumboot_irq_entry *tbl = rumboot_irq_create( NULL );

    rumboot_irq_set_handler( tbl, MPIC128_TIMER_0, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, mpic_tim0_handler, ( void* )0 );

    /* Activate the table */
    rumboot_irq_table_activate( tbl );
    rumboot_irq_enable( MPIC128_TIMER_0 );
    rumboot_irq_sei();
}
int main ()
{
    init_handlers();

    spr_write (SPR_TCR, spr_read (SPR_TCR) & ~(1 << IBM_BIT_INDEX(64, 41)) ); //autoreload disable

    DEC_CONTROL_VALUE = 0;
    INT_FLAG = 0;
    mpic128_timer_init(DCR_MPIC128_BASE, mpic128_timer_freq_SYS_CLK);
    //mpic128_timer_init(DCR_MPIC128_BASE, mpic128_timer_freq_SYS_TMRCLK);
    dec_clr_exception();

    mpic128_start_timer(DCR_MPIC128_BASE, Mpic128Timer0, MPIC_START_VALUE);
    dec_set_value (DEC_START_VALUE);

    while (INT_FLAG == 0) msync();
    rumboot_printf("DEC VALUE = %x\nDIFF = %x\n", DEC_CONTROL_VALUE, (DEC_START_VALUE - DEC_CONTROL_VALUE));

    rumboot_printf ("TEST OK\n");
    return 0;
}
