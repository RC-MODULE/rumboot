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
#include <platform/interrupts.h>
#include <arch/common_macros.h>

#include <arch/ppc_476fp_config.h>
#include <arch/ppc_476fp_lib_c.h>
#include <platform/arch/ppc/ppc_476fp_mmu_fields.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>
#include <platform/arch/ppc/ppc_476fp_power_modes.h>
#include <arch/ppc_476fp_timer_fields.h>
#include <arch/ppc_476fp_itrpt_fields.h>

#include <platform/devices/l2c.h>
#include <platform/devices.h>
#include <platform/devices/plb6mcif2.h>
#include <platform/devices/emi.h>
#include <devices/mpic128.h>

#include <platform/regs/regs_l2c_l2.h>
#include <platform/regs/regs_l2c.h>
#include <platform/regs/sctl.h>
#include <platform/regs/fields/emi.h>

#include <devices/sp804.h>

#define EVENT_TIME_1                        (TEST_EVENT_CODE_MIN + 0)
#define EVENT_TIME_2                        (TEST_EVENT_CODE_MIN + 1)
#define EVENT_START_MONITORS                (TEST_EVENT_CODE_MIN + 2)

#define DELAY                               (0x2100)   // DIT, MPIC (25MHz - 40 ns)
#define DELAY2                              (0x10000)  // DEC, FIT, Watchdog (200MHz - 5 ns)

static void msr_ext_int_enable (void){
    msr_write (msr_read() | (1 << ITRPT_XSR_EE_i ));
}

static void msr_crit_int_enable (void){
    msr_write (msr_read() | (1 << ITRPT_XSR_CE_i ));
}

//***************************************************************************************************************
/*
 *  DECREMENT BLOCK
 */

static void dec_reset(void) {
    spr_write (SPR_DEC, 0x00);
}

static void dec_clr_exception (void){
    spr_write (SPR_TSR_RC, 1 << TIMER_TSR_DIS_i); //TSR[DIS]=0 - clear DEC exceptions
}

static void dec_interrupt_enable (void){
    spr_write (SPR_TCR, spr_read (SPR_TCR) | (1 << TIMER_TCR_DIE_i)  );
}

static void dec_interrupt_disable (void){
    spr_write (SPR_TCR, spr_read (SPR_TCR) & ~(1 << TIMER_TCR_DIE_i)  );
}

static void dec_set_value (uint32_t delay) {
    spr_write (SPR_DEC, delay);
}

static void dec_generate_interrupt (uint32_t delay)
{
    msr_ext_int_enable ();
    dec_reset();
    dec_clr_exception();
    dec_interrupt_enable ();
    dec_set_value (delay);
}

static inline void decrementer_handler (void)
{
    dec_interrupt_disable ();
    dec_clr_exception ();
}
/*
 *  DECREMENT BLOCK END
 */
//**************************************************************************************************************

//**************************************************************************************************************
/*
 *  FIT BLOCK
 */

static void fit_set_period (uint8_t field_value){ //see TCR[FP]
    uint32_t tmp =  spr_read(SPR_TCR);
    tmp &= ~(0b11 << TIMER_TCR_FP_i );
    spr_write(SPR_TCR, tmp | (field_value << TIMER_TCR_FP_i));
}

static void fit_clr_exception (void){
    spr_write (SPR_TSR_RC, 1 << TIMER_TSR_FIS_i); //TSR[FIS]=0 - clear FIT exceptions
}

static void fit_interrupt_enable (void){
    spr_write (SPR_TCR, spr_read (SPR_TCR) | (1 << TIMER_TCR_FIE_i)  );
}

static void fit_interrupt_disable (void){
    spr_write (SPR_TCR, spr_read (SPR_TCR) & ~(1 << TIMER_TCR_FIE_i)  );
}

static void fit_generate_interrupt (uint32_t delay)
{
    msr_ext_int_enable ();
    fit_set_period (0b11);
    spr_write(SPR_TBL_W, 0); //reset TBL
    fit_clr_exception();
    spr_write(SPR_TBL_W, 0x1000000 - delay); //set TBL
    fit_interrupt_enable();
}

static inline void fit_handler (void)
{
    fit_interrupt_disable ();
    fit_clr_exception ();
}

/*
 *  FIT BLOCK END
 */
//**************************************************************************************************************

//**************************************************************************************************************
/*
 *  WATCH DOG BLOCK
 */

static void wd_clr_exception(void){
    spr_write (SPR_TSR_RC, (0b1 << TIMER_TSR_ENW_i )   //TSR[ENW]=0 - clear WD next action
                         | (0b1 << TIMER_TSR_WIS_i )   //TSR[WIS]=0 - clear WD exception
                         | (0b11 << TIMER_TSR_WRS_i ));//TSR[WRS]=0 - clear WD reset status
}

static void wd_set_period (uint8_t field_value){ //see TCR[WP]
    uint32_t tmp =  spr_read(SPR_TCR);
    tmp &= ~(0b11 << TIMER_TCR_WP_i );
    spr_write(SPR_TCR, tmp | (field_value << TIMER_TCR_WP_i ));
}

static void wd_set_reset_control (uint8_t field_value){ //see TCR[WRC]
    uint32_t tmp =  spr_read(SPR_TCR);
    tmp &= ~(0b11 << TIMER_TCR_WRC_i );
    spr_write(SPR_TCR, tmp | (field_value << TIMER_TCR_WRC_i));
}

static void wd_interrupt_enable (void){
    spr_write (SPR_TCR, spr_read (SPR_TCR) | (1 << TIMER_TCR_WIE_i)  );
}

static void wd_interrupt_disable (void){
    spr_write (SPR_TCR, spr_read (SPR_TCR) & ~(1 << TIMER_TCR_WIE_i)  );
}

static void wd_generate_interrupt(uint32_t delay)
{
    wd_set_period(0b00); //TCR[WP]=0b00 - 2pow20 ticks = 0x100000
    wd_set_reset_control(0b00);
    spr_write(SPR_TBL_W, 0x00); //reset TBL;
    wd_clr_exception();
    spr_write(SPR_TBL_W, 0x100000); //set TBL value that triggers TSR[ENW] to 1
    spr_write(SPR_TBL_W, 0x100000 - delay); //set TBL
    msr_crit_int_enable();
    wd_interrupt_enable();
}

static void wd_handler()
{
    wd_interrupt_disable();
    wd_clr_exception();
}

/*
 *  WATCH DOG BLOCK END
 */
//**************************************************************************************************************

//**************************************************************************************************************
/*
 *  DIT0 BLOCK
 */

static void dit0_generate_interrupt (uint32_t delay)
{
    struct sp804_conf config_0 = {
        .mode = ONESHOT,
        .interrupt_enable = 1,
        .clock_division = 1,
        .width = 32,
        .load = delay,
        .bgload = 0 };
    sp804_config( DCR_TIMERS_BASE, &config_0, 0 );
    sp804_enable( DCR_TIMERS_BASE, 0 );
}

static void dit0_handler ()
{
    rumboot_printf("Interrupt handler (DIT0)\n");
    sp804_stop( DCR_TIMERS_BASE, 0 );
    sp804_clrint( DCR_TIMERS_BASE, 0 );
    ppc470s_exit_sleep_mode_on_noncritical_interrupt();
}

/*
 *  DIT0 BLOCK END
 */
//**************************************************************************************************************

//**************************************************************************************************************
/*
 *  MPIC Timer
 */

static void  mpic_tim0_generate_interrupt (uint32_t delay)
{
    mpic128_timer_init(DCR_MPIC128_BASE, mpic128_timer_freq_SYS_CLK);
    mpic128_start_timer(DCR_MPIC128_BASE, Mpic128Timer0, delay);
}

static void mpic_tim0_handler()
{
    rumboot_printf("Interrupt handler (MPIC Timer0)\n");
    mpic128_stop_timer(DCR_MPIC128_BASE, Mpic128Timer0);
    ppc470s_exit_doze_mode_on_noncritical_interrupt();
}

/*
 *  MPIC Timer BLOCK END
 */
//**************************************************************************************************************

static void exception_handler( int id, const char *name )
{
    switch (id)
    {
    case RUMBOOT_IRQ_DECREMENTER:
        rumboot_printf("Exception handler (DEC)\n");
        decrementer_handler ();
        ppc470s_exit_sleep_mode_on_noncritical_interrupt();
        break;
    case RUMBOOT_IRQ_FIXED_INTERVAL_TIMER:
        rumboot_printf("Exception handler (FIT)\n");
        fit_handler();
        ppc470s_exit_sleep_mode_on_noncritical_interrupt();
        break;
    case RUMBOOT_IRQ_WATCHDOG_TIMER:
        rumboot_printf("Exception handler (WatchDog)\n");
        wd_handler();
        ppc470s_exit_sleep_mode_on_critical_interrupt();
        break;
    default:
        TEST_ASSERT(0, "TEST ERROR: NON EXPECTED INTERRUPT");
    }
}

static void init_handlers()
{
    //MPIC, DIT
    rumboot_irq_cli();
    struct rumboot_irq_entry *tbl = rumboot_irq_create( NULL );

    rumboot_irq_set_handler( tbl, DIT_INT0, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, dit0_handler, ( void* )0 );
    rumboot_irq_set_handler( tbl, MPIC128_TIMER_0, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, mpic_tim0_handler, ( void* )0 );

    /* Activate the table */
    rumboot_irq_table_activate( tbl );
    rumboot_irq_enable( DIT_INT0 );
    rumboot_irq_enable( MPIC128_TIMER_0 );
    rumboot_irq_sei();

    //DEC, FIT, WD
    rumboot_irq_set_exception_handler(exception_handler);
}

int main ()
{
    test_event_send_test_id("test_oi10_cpu_sleep_l2c");

    rumboot_printf("Init handlers\n");
    init_handlers();

    spr_write (SPR_TCR, spr_read (SPR_TCR) & ~(1 << IBM_BIT_INDEX(64, 41)) ); //autoreload disable

    rumboot_printf("Start hw monitors\n");
    test_event(EVENT_START_MONITORS);

    //MPIC Timer0
    rumboot_printf("Generate MPIC Timer0 interrupt...\n");
    mpic_tim0_generate_interrupt(DELAY);
    uint32_t start_mpic = rumboot_platform_get_uptime();    // in us

    test_event(EVENT_TIME_1);
    rumboot_printf("Enter sleep with L2C mode\n");
    ppc470s_enter_sleep_mode(SCTL_PPC_SLP_CPU_L2C_SLP);
    test_event(EVENT_TIME_2);
    TEST_ASSERT(rumboot_platform_get_uptime() - start_mpic > DELAY/25, "SLEEP MODE ERROR");

    //DIT0
    rumboot_printf("Generate DIT0 interrupt...\n");
    dit0_generate_interrupt(DELAY);
    uint32_t start_dit0 = rumboot_platform_get_uptime();    // in us

    test_event(EVENT_TIME_1);
    rumboot_printf("Enter sleep with L2C mode\n");
    ppc470s_enter_sleep_mode(SCTL_PPC_SLP_CPU_L2C_SLP);
    test_event(EVENT_TIME_2);
    TEST_ASSERT(rumboot_platform_get_uptime() - start_dit0 > DELAY/25, "SLEEP MODE ERROR");

    //DEC
    rumboot_printf("Generate DEC interrupt...\n");
    dec_generate_interrupt(DELAY2);
    uint32_t start_dec = rumboot_platform_get_uptime();     // in us

    test_event(EVENT_TIME_1);
    rumboot_printf("Enter sleep with L2C mode\n");
    ppc470s_enter_sleep_mode(SCTL_PPC_SLP_CPU_L2C_SLP);
    test_event(EVENT_TIME_2);
    TEST_ASSERT(rumboot_platform_get_uptime() - start_dec > DELAY2/200, "SLEEP MODE ERROR");

    //FIT
    rumboot_printf("Generate FIT interrupt...\n");
    fit_generate_interrupt(DELAY2);
    uint32_t start_fit = rumboot_platform_get_uptime();     // in us

    test_event(EVENT_TIME_1);
    rumboot_printf("Enter sleep with L2C mode\n");
    ppc470s_enter_sleep_mode(SCTL_PPC_SLP_CPU_L2C_SLP);
    test_event(EVENT_TIME_2);
    TEST_ASSERT(rumboot_platform_get_uptime() - start_fit > DELAY2/200, "SLEEP MODE ERROR");

    //WatchDog
    rumboot_printf("Generate WD interrupt...\n");
    wd_generate_interrupt(DELAY2);
    uint32_t start_wd = rumboot_platform_get_uptime();      // in us

    test_event(EVENT_TIME_1);
    rumboot_printf("Enter sleep with L2C mode\n");
    ppc470s_enter_sleep_mode(SCTL_PPC_SLP_CPU_L2C_SLP);
    test_event(EVENT_TIME_2);
    TEST_ASSERT(rumboot_platform_get_uptime() - start_wd > DELAY2/200, "SLEEP MODE ERROR");

    return 0;
}

