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

#include <platform/arch/ppc/ppc_476fp_config.h>
#include <platform/arch/ppc/ppc_476fp_lib_c.h>
#include <platform/arch/ppc/ppc_476fp_mmu_fields.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>

#include <platform/devices/l2c.h>
#include <platform/devices.h>
#include <platform/devices/plb6mcif2.h>
#include <platform/devices/emi.h>
#include <platform/regs/regs_emi.h>
#include <platform/regs/fields/emi.h>
#include <platform/interrupts.h>

#define EVENT_TIME_1                        (TEST_EVENT_CODE_MIN + 0)
#define EVENT_TIME_2                        (TEST_EVENT_CODE_MIN + 1)

#define DELAY                               (0x1000)  // external, MPIC
#define DELAY2                              (0x10000)  // DEC, FIT, Watchdog

uint8_t INT_FLAG = 0;
/*
//external
static void generate_external_interrupt(uint32_t delay)
{
    irq_unmask(irq_source_int_tim1);

    timer_info t_info1 = {
        .timer_number = 1,
        .load_value = delay,
        .run_mode = mode_free_run,
        .int_enable = true,
        .prescale = ps_div_by_1,
        .timer_size = timer_size_16bit,
        .one_shot_count = true //counter decrements once
    };
    timer_start(&t_info1);
}

//MPIC Timer
static void generate_mpic_timer_interrupt(uint32_t delay)
{
    mpic128_timer_init(MPICx_BASE, mpic128_timer_freq_SYS_CLK);
    irq_unmask(irq_source_MPIC_Timer0);
    mpic128_start_timer(MPICx_BASE, Mpic128Timer0, delay);
}

static void mpic_timer_handler()
{
    trace_msg("mpic_timer_handler\n");

    mpic128_stop_timer(MPICx_BASE, Mpic128Timer0);

    ppc470s_exit_doze_mode_on_noncritical_interrupt();
}

static void mpic_timer_handler()
{
    rumboot_printf("mpic_timer_handler\n");

    //mpic128_stop_timer(MPICx_BASE, Mpic128Timer0);

    ppc470s_exit_doze_mode_on_noncritical_interrupt();
}

static void external_timer_handler()
{
    rumboot_printf("external_timer_handler\n");

    //timer_stop(1);

    ppc470s_exit_doze_mode_on_noncritical_interrupt();

}
*/

/*
void ppc470s_exit_doze_mode_on_noncritical_interrupt()
{
    rumboot_printf("exit doze mode on nonecrit int\n");
    spr_write (SPR_SRR1, spr_read(SPR_SRR1) & ~(1 << ITRPT_XSR_WE_i) ); //xSRR[WE] = 0
}

void ppc470s_exit_doze_mode_on_critical_interrupt()
{
    rumboot_printf("exit doze mode on crit int\n");
    spr_write(SPR_CSRR1, spr_read(SPR_CSRR1) & ~(1 << IBM_BIT_INDEX(32, 45)) ); //xSRR[WE] = 0
}
*/

//DEC
static void generate_DEC_interrupt (uint32_t delay)
{
    spr_write (SPR_TCR, spr_read (SPR_TCR) & ~(1<<IBM_BIT_INDEX(32, 41)) ); //TCR[ARE]=0 - disable autoreload
    spr_write (SPR_DEC, 0x00); //reset DEC
    spr_write (SPR_TSR_RC, 1 << IBM_BIT_INDEX(64, 36)); //TSR[DIS]=0 - clear DEC exceptions
    spr_write (SPR_DEC, delay);
    INT_FLAG = 1;
    spr_write (SPR_TCR, spr_read (SPR_TCR) | (1 << IBM_BIT_INDEX(64, 37))     //Dec int en
                                           );
}

//FIT
static void generate_FIT_interrupt (uint32_t delay)
{
    spr_write (SPR_TCR, spr_read (SPR_TCR) | (0b11 << IBM_BIT_INDEX(32, 39)));//set TCR[FP] = 2pow24 = 0x1000000
    spr_write (SPR_TBL_W, 0x00); //reset TBL
    spr_write (SPR_TSR_RC, 1 << IBM_BIT_INDEX(64, 37) ); //TSR[FIS]=0 - clear FIT exceptions
    spr_write (SPR_TBL_W, 0x1000000 - delay); //set TBL
    INT_FLAG = 1;
    spr_write (SPR_TCR, spr_read (SPR_TCR) | (1 << IBM_BIT_INDEX(64, 40))     //Fit int en
                                           );
}

//Watchdog timer
static void generate_Watchdog_timer_interrupt(uint32_t delay)
{
    uint32_t tcr = spr_read (SPR_TCR);
    tcr &= ~(0b11 << IBM_BIT_INDEX(64, 33)); //TCR[WP]=0b00 - 2pow20 ticks = 0x100000
    tcr &= ~(0b11 << IBM_BIT_INDEX(64, 35)); //TCR[WRC]=0b00 - No Watchdog timer reset
    spr_write (SPR_TCR, tcr);
    spr_write (SPR_TBL_W, 0x00); //reset TBL
    spr_write(SPR_TSR_RC, (0b1 << IBM_BIT_INDEX(64, 32)) //TSR[ENW]=0 - clear WD next action
                     | (0b1 << IBM_BIT_INDEX(64, 33))    //TSR[WIS]=0 - clear WD exception
                     | (0b11 << IBM_BIT_INDEX(64, 35))); //TSR[WRS]=0 - clear WD reset status
    spr_write(SPR_TBL_W, 0x100000);  //set TBL value that triggers TSR[ENW] to 1
    spr_write(SPR_TBL_W, 0x100000 - delay); //set TBL
    INT_FLAG = 1;
    spr_write (SPR_TCR, spr_read (SPR_TCR) | (1 << IBM_BIT_INDEX(64, 36))     //Watchdog int en
                                           );
}

static void exception_handler( int id, const char *name )
{
    switch (id)
    {
    case RUMBOOT_IRQ_DECREMENTER:
        rumboot_printf("Exception handler (DEC)\n");
        spr_write (SPR_TCR, spr_read (SPR_TCR) & ~(1 << IBM_BIT_INDEX(64, 37))     //Dec int dis
                                               );
        spr_write (SPR_SRR1, spr_read(SPR_SRR1) & ~(1 << ITRPT_XSR_WE_i) ); //xSRR[WE] = 0
        //ppc470s_exit_doze_mode_on_noncritical_interrupt();
        break;
    case RUMBOOT_IRQ_FIXED_INTERVAL_TIMER:
        rumboot_printf("Exception handler (FIT)\n");
        spr_write (SPR_TCR, spr_read (SPR_TCR) & ~(1 << IBM_BIT_INDEX(64, 40))     //Fit int dis
                                               );
        spr_write (SPR_SRR1, spr_read(SPR_SRR1) & ~(1 << ITRPT_XSR_WE_i) ); //xSRR[WE] = 0
        //ppc470s_exit_doze_mode_on_noncritical_interrupt();
        break;
    case RUMBOOT_IRQ_WATCHDOG_TIMER:
        rumboot_printf("Exception handler (WATCHDOG)\n");
        spr_write (SPR_TCR, spr_read (SPR_TCR) & ~(1 << IBM_BIT_INDEX(64, 36))     //Watchdog int en
                                               );
        spr_write ( SPR_TSR_RC,(0b1 << IBM_BIT_INDEX(32, 32))     //TSR[ENW]=0 - clear WD next action
                               | (0b1 << IBM_BIT_INDEX(32, 33))   //TSR[WIS]=0 - clear WD exception
                               | (0b11 << IBM_BIT_INDEX(32, 35)));//TSR[WRS]=0 - clear WD reset status

        spr_write(SPR_CSRR1, spr_read(SPR_CSRR1) & ~(1 << IBM_BIT_INDEX(32, 45)) ); //xSRR[WE] = 0
        //ppc470s_exit_doze_mode_on_critical_interrupt();
        break;
    default:
        TEST_ASSERT(0, "TEST ERROR: NON EXPECTED INTERRUPT");
    }
    INT_FLAG = 0;
}

static void init_handlers()
{
/*
    rumboot_irq_cli();
    struct rumboot_irq_entry *tbl = rumboot_irq_create( NULL );

    //MPIC Timer
    rumboot_irq_set_handler( tbl, MPIC128_TIMER_0, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, mpic_timer_handler, ( void* )0 );
    //external
    rumboot_irq_set_handler( tbl, DIT_INT0, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, external_timer_handler, ( void* )0 );

     // Activate the table
    rumboot_irq_table_activate( tbl );
    rumboot_irq_enable( MPIC128_TIMER_0 );
    rumboot_irq_enable( DIT_INT0 );
    rumboot_irq_sei();
*/

    //DEC, FIT, WD
    rumboot_irq_set_exception_handler(exception_handler);          //handler for fit, dec, watchdog
    msr_write( msr_read() | (1 << ITRPT_XSR_EE_i)); //fit, dec interrupts enable
}


int main ()
{
    rumboot_printf("Init handlers\n");
    init_handlers();

    //DEC
    rumboot_printf("Generate DEC interrupt\n");
    generate_DEC_interrupt(DELAY2);
    //test_event(EVENT_TIME_1);
    rumboot_printf("Enter doze mode\n");
    uint32_t i = 0;
    while (INT_FLAG != 0) i++;
    rumboot_printf("i = %x\n", i);
    //ppc470s_enter_doze_mode();
    //test_event(EVENT_TIME_2);

    //FIT
    rumboot_printf("Generate FIT interrupt\n");
    generate_FIT_interrupt(DELAY2);
    //test_event(EVENT_TIME_1);
    rumboot_printf("Enter doze mode\n");
    i = 0;
    while (INT_FLAG != 0) i++;
    rumboot_printf("i = %x\n", i);
    //ppc470s_enter_doze_mode();
    //test_event(EVENT_TIME_2);

    //Watchdog timer
    rumboot_printf("Watchdog timer\n");
    generate_Watchdog_timer_interrupt(DELAY2);
    //test_event(EVENT_TIME_1);
    rumboot_printf("Enter doze mode\n");
    i = 0;
    while (INT_FLAG != 0) i++;
    rumboot_printf("i = %x\n", i);
    //ppc470s_enter_doze_mode();
    //test_event(EVENT_TIME_2);
/*
    //external
    rumboot_printf("Generate external interrupt\n");
    generate_external_interrupt(DELAY); //delayed
    test_event(EVENT_TIME_1);
    rumboot_printf("Enter doze mode\n");
    ppc470s_enter_doze_mode();
    test_event(EVENT_TIME_2);

    //MPIC Timer
    rumboot_printf("Generate MPIC Timer interrupt\n");
    irq_mask(irq_source_int_tim1);
    generate_mpic_timer_interrupt(DELAY);
    test_event(EVENT_TIME_1);
    rumboot_printf("Enter doze mode\n");
    ppc470s_enter_doze_mode();
    test_event(EVENT_TIME_2);
*/
    rumboot_printf("TEST OK\n");
    return 0;
}

