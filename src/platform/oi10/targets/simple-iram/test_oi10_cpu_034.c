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

#include <arch/common_macros.h>

#include <arch/ppc_476fp_config.h>
#include <arch/ppc_476fp_lib_c.h>
#include <arch/ppc_476fp_mmu_fields.h>
#include <arch/ppc_476fp_mmu.h>

#include <platform/devices.h>
#include <devices/ugly/emi.h>
#include <devices/mpic128.h>
#include <regs/fields/emi.h>
#include <platform/interrupts.h>

#define PERMISSIBLE_DIFS 0x250

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
    msync();
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

uint32_t get_timers_dif (uint32_t mpic_base, mpic128_timer_freq mpic_clk, mpic128_tim_num const timer_num, uint32_t const mpic_counts)
{
    #define DEC_START_VALUE  0x100000

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
    msync(); isync();
    dec_set_value (DEC_START_VALUE);
    mpic128_start_timer(mpic_base, timer_num, mpic_counts);

    //wait interrupt
    while (INT_FLAG == 0);

    return (DEC_START_VALUE - DEC_CONTROL_VALUE);
}

uint32_t get_compare_dif (uint32_t const mpic_counts)
{
    rumboot_printf("START CHECK\n", mpic_counts);
    rumboot_printf("MPIC count value = 0x%x\n\n", mpic_counts);

    rumboot_printf("SOURCE = SYS_CLK (25 MHz)\n");
    uint32_t DIF_SYS_CLK = get_timers_dif(DCR_MPIC128_BASE, mpic128_timer_freq_SYS_CLK, Mpic128Timer0, mpic_counts);
    rumboot_printf("Counts (DIF_SYS_CLK) = %x\n\n", DIF_SYS_CLK);

    rumboot_printf("SOURCE = SYS_TMRCLK (12.5 MHz)\n");
    uint32_t DIF_TMR_CLK = get_timers_dif(DCR_MPIC128_BASE, mpic128_timer_freq_SYS_TMRCLK, Mpic128Timer0, mpic_counts);
    rumboot_printf("Counts (DIF_TMR_CLK) = %x\n\n", DIF_TMR_CLK);

    rumboot_printf("Compare\n");
    if (2 * DIF_SYS_CLK > DIF_TMR_CLK)
        return  (2 * DIF_SYS_CLK - DIF_TMR_CLK);

    return 0;
}

int main ()
{
    rumboot_printf("Init handlers\n\n");
    init_handlers();

    //low counts
    uint32_t first_compare_dif = 0;
    first_compare_dif = get_compare_dif (0x1000);
    rumboot_printf("Compare_dif (2 * DIF_SYS_CLK - DIF_TMR_CLK) = 0x%x\n\n", first_compare_dif);

    //high counts
    uint32_t second_compare_dif = 0;
    second_compare_dif = get_compare_dif (0x8000);
    rumboot_printf("Compare_dif (2 * DIF_SYS_CLK - DIF_TMR_CLK) = 0x%x\n\n", second_compare_dif);

    //check
    rumboot_printf("Check compare_difs\n");
    uint32_t compare_difs = (uint32_t) (abs( (int)(first_compare_dif-second_compare_dif) ));
    rumboot_printf("compare difs = 0x%x\n", compare_difs);
    TEST_ASSERT( compare_difs < PERMISSIBLE_DIFS, "TEST ERROR: the values are too different");

    rumboot_printf ("TEST OK\n\n");
    return 0;
}
