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
    DEC_CONTROL_VALUE = spr_read (SPR_DEC);
    rumboot_printf("Interrupt handler (MPIC Timer0)\n");
    mpic128_stop_timer(DCR_MPIC128_BASE, Mpic128Timer0);
    dec_set_value (0x00);
    dec_clr_exception();
    INT_FLAG = 1;
    msync();
}

static void exception_handler( int id, const char *name )
{
    switch (id)
    {
    case RUMBOOT_IRQ_DECREMENTER:
        rumboot_printf("Exception handler (DEC)\n");
        TEST_ASSERT(0, "TEST ERROR: NON EXPECTED INTERRUPT");
        break;
    default:
        TEST_ASSERT(0, "TEST ERROR: NON EXPECTED INTERRUPT");
    }
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

    rumboot_irq_set_exception_handler(exception_handler);
}

uint32_t get_timers_dif (uint32_t mpic_base, mpic128_timer_freq mpic_clk, mpic128_tim_num const timer_num)
{
    #define DEC_START_VALUE  0x100000
    #define MPIC_START_VALUE 0x1000

    //reset variables
    DEC_CONTROL_VALUE = 0;
    INT_FLAG = 0;

    //config timers
    //DEC
    spr_write (SPR_TCR, spr_read (SPR_TCR) & ~(1 << IBM_BIT_INDEX(64, 41)) ); //autoreload disable
    dec_set_value (0x00);
    dec_clr_exception();
    //MPIC
    mpic128_timer_init(mpic_base, mpic_clk);

    //start timers
    msync();
    mpic128_start_timer(mpic_base, timer_num, MPIC_START_VALUE);
    dec_set_value (DEC_START_VALUE);

    //wait interrupt
    while (INT_FLAG == 0) msync();

    return (DEC_START_VALUE - DEC_CONTROL_VALUE);
}

int main ()
{
    test_event_send_test_id("test_oi10_cpu_034");

    rumboot_printf("Init handlers\n\n");
    init_handlers();

    rumboot_printf("SOURCE = SYS_CLK (20 MHz)\n");
    uint32_t DIF_SYS_CLK = get_timers_dif(DCR_MPIC128_BASE, mpic128_timer_freq_SYS_CLK, Mpic128Timer0);
    rumboot_printf("Counts = %x\n\n", DIF_SYS_CLK);

    rumboot_printf("SOURCE = SYS_TMRCLK (12.5 MHz)\n");
    uint32_t DIF_TMR_CLK = get_timers_dif(DCR_MPIC128_BASE, mpic128_timer_freq_SYS_TMRCLK, Mpic128Timer0);
    rumboot_printf("Counts = %x\n\n", DIF_TMR_CLK);

    rumboot_printf("Compare...\n");
    TEST_ASSERT( (DIF_SYS_CLK * 2) < DIF_TMR_CLK,"TEST ERROR");

    rumboot_printf ("TEST OK\n\n");
    return 0;
}
