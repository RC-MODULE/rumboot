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
#include <rumboot/timer.h>

#include <arch/common_macros.h>

#include <arch/ppc_476fp_config.h>
#include <arch/ppc_476fp_lib_c.h>

#include <platform/test_assert.h>
#include <platform/test_event_c.h>
#include <platform/test_event_codes.h>
#include <platform/test_assert.h>
#include <platform/trace.h>
#include <platform/devices.h>
#include <platform/interrupts.h>
#include <devices/mpic128.h>

#include <regs/fields/mpic128.h>
#include <platform/arch/ppc/ppc_476fp_timer_fields.h>
#include <platform/arch/ppc/ppc_476fp_ctrl_fields.h>

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
    TEC_SET_EXT_INT45_AFTER_WR_IAR,
    TEC_SET_EXT_INT0_AFTER_DEC_TIMER,
    TEC_SET_EXT_INT3_AFTER_DEC_TIMER,
    TEC_SET_EXT_INT0_BEFORE_DEC_TIMER,
    TEC_SET_EXT_INT3_BEFORE_DEC_TIMER,
    TEC_SET_EXT_INT3_AFTER_WD_TIMER,
    TEC_SET_EXT_INT5_AFTER_WD_TIMER,
    TEC_SET_EXT_INT3_BEFORE_WD_TIMER,
    TEC_SET_EXT_INT5_BEFORE_WD_TIMER,
    TEC_SET_EXT_INT2_AFTER_WR_CIAR,
    TEC_SET_EXT_INT0_WITHOUT_CHECK_TYPE
} test_event_code;

#define DEC_PERIOD  0x4000
#define WD_PERIOD   0x100000
#define WD_TIMEOUT  0x4000

static struct rumboot_irq_entry *tbl = NULL;

uint32_t arg0 = TEC_CLR_EXT_INT0;
uint32_t arg1 = TEC_CLR_EXT_INT1;
uint32_t arg2 = TEC_CLR_EXT_INT2;
uint32_t arg3 = TEC_CLR_EXT_INT3;
uint32_t arg4 = TEC_CLR_EXT_INT4;
uint32_t arg5 = TEC_CLR_EXT_INT5;

static int noncritical_int_cnt = 0;
static int critical_int_cnt    = 0;
static int mc_int_cnt          = 0;
static int sw_int_cnt          = 0;
static int ext_int_cnt         = 0;
static int tmr_int_cnt         = 0;

static int exp_noncritical_int_cnt = 0;
static int exp_critical_int_cnt    = 0;
static int exp_mc_int_cnt          = 0;

static void initcnt() {
    noncritical_int_cnt = 0;
    critical_int_cnt    = 0;
    mc_int_cnt          = 0;
    sw_int_cnt          = 0;
    ext_int_cnt         = 0;
    tmr_int_cnt         = 0;

    exp_noncritical_int_cnt = 0;
    exp_critical_int_cnt    = 0;
    exp_mc_int_cnt          = 0;
}

static void int_cnt_inc(int num)
{
    switch(num) {
    case 0:
    case 1:
        noncritical_int_cnt += 1;
        break;
    case 2:
    case 3:
        critical_int_cnt += 1;
        break;
    case 4:
    case 5:
        mc_int_cnt += 1;
        break;
    }
    msync();
}

static void exp_int_cnt_inc(int num)
{
    switch(num) {
    case 0:
    case 1:
        exp_noncritical_int_cnt += 1;
        break;
    case 2:
    case 3:
        exp_critical_int_cnt += 1;
        break;
    case 4:
    case 5:
        exp_mc_int_cnt += 1;
        break;
    }
    msync();
}

static void exception_handler(int const id, char const * const name ) {
        switch(id){
        case RUMBOOT_IRQ_DECREMENTER:
            rumboot_printf("Decrementer timer IRQ received.\n");
            spr_write(SPR_TSR_RC, 1 << TIMER_TSR_DIS_i);
            //spr_write(SPR_TCR, spr_read(SPR_TCR) & (~(1 << TIMER_TCR_DIE_i)));
            break;
        case RUMBOOT_IRQ_FIXED_INTERVAL_TIMER:
            rumboot_printf("Fixed interval timer IRQ received.\n");
            spr_write(SPR_TSR_RC, 1 << TIMER_TSR_FIS_i);
            spr_write(SPR_TCR, spr_read(SPR_TCR) & (~(1 << TIMER_TCR_FIE_i)));
            break;
        case RUMBOOT_IRQ_WATCHDOG_TIMER:
            rumboot_printf("Watchdog timer IRQ received.\n");
            spr_write(SPR_TSR_RC, 1 << TIMER_TSR_WIS_i);
//            spr_write(SPR_TCR, spr_read(SPR_TCR) & (~(1 << TIMER_TCR_WIE_i)));
            break;
        default:
            rumboot_printf("\nWE GOT AN EXCEPTION: %d: %s\n", id, name);
            rumboot_printf("--- Guru Meditation ---\n");
            rumboot_printf("MSR:  0x%x\n", msr_read());
            rumboot_printf("SRR0: 0x%x\n", spr_read(SPR_SRR0));
            rumboot_printf("SRR1: 0x%x\n", spr_read(SPR_SRR1));
            rumboot_printf("CSRR0: 0x%x\n", spr_read(SPR_CSRR0));
            rumboot_printf("CSRR1: 0x%x\n", spr_read(SPR_CSRR1));
            rumboot_printf("MCSRR0: 0x%x\n", spr_read(SPR_MCSRR0));
            rumboot_printf("MCSRR1: 0x%x\n", spr_read(SPR_MCSRR1));
            rumboot_printf("---       ---       ---\n");
            rumboot_platform_panic("Please reset or power-cycle the board\n");
            break;
        }
}

void irq_handler( int irq_num, void *arg )
{
    int num = irq_num-EXT_INT0;
    int_cnt_inc(num);
    rumboot_printf("irq_handler: IRQ #%d type #%d received.\n", num, rumboot_irq_current_type());
    test_event(*((uint32_t*)arg));
}

void irq_handler2( int irq_num, void *arg )
{
    int num = irq_num-EXT_INT0;
    rumboot_printf("irq_handler2: IRQ #%d type #%d received.\n", num, rumboot_irq_current_type());
    ext_int_cnt += 1;
    msync();
    test_event(*((uint32_t*)arg));
}

void swirq_handler( int irq_num, void *arg )
{
    rumboot_printf("swirq_handler: IRQ #%d type #%d received.\n", irq_num, rumboot_irq_current_type());
    sw_int_cnt += 1;
    msync();
}

void tmr_handler( int irq_num, void *arg )
{
    rumboot_printf("tmr_handler: IRQ #%d type #%d received.\n", irq_num, rumboot_irq_current_type());
    mpic128_stop_timer(DCR_MPIC128_BASE, (uint32_t)arg);
    tmr_int_cnt += 1;
    msync();
}

void def_handler( int irq_num, void *arg)
{
    rumboot_printf("def_handler: IRQ #%d type #%d received.\n", irq_num, rumboot_irq_current_type());
    if (irq_num == MPIC128_IPI_0) sw_int_cnt += 1;
    if (irq_num == MPIC128_TIMER_0) tmr_int_cnt += 1;
    msync();
    mpic128_stop_timer(DCR_MPIC128_BASE, Mpic128Timer0);
}


void dec_timer_start()
{
    spr_write(SPR_CCR1,CTRL_CCR1_TSS_CPU_clock << CTRL_CCR1_TSS_i);

    spr_write(SPR_DEC, 0);
    spr_write(SPR_DECAR, 0);

    spr_write(SPR_TBL_W,0);
    spr_write(SPR_TBU_W,0);

    spr_write(SPR_TCR, 0);
    spr_write(SPR_TSR_RC, 0xFFFFFFFF);

    spr_write(SPR_DEC, DEC_PERIOD);
    spr_write(SPR_DECAR, DEC_PERIOD);

    msr_write(msr_read()
            | 1 << ITRPT_XSR_EE_i   /* MSR[EE] - Externel interrupt. */
            | 1 << ITRPT_XSR_CE_i); /* MSR[CE] - Critical interrupt. */

    spr_write(SPR_TCR, 0 << TIMER_TCR_WIE_i
                     | 1 << TIMER_TCR_DIE_i
                     | 0 << TIMER_TCR_FIE_i
                     | 1 << TIMER_TCR_ARE_i
                     | TIMER_TCR_WP_2pow21_clocks << TIMER_TCR_WP_i
                     | TIMER_TCR_FP_2pow21_clocks << TIMER_TCR_FP_i
                     | TIMER_TCR_WRC_No_WD_reset  << TIMER_TCR_WRC_i);
}

void wd_timer_start()
{
    spr_write(SPR_CCR1,CTRL_CCR1_TSS_CPU_clock << CTRL_CCR1_TSS_i);

    spr_write(SPR_DEC, 0);
    spr_write(SPR_DECAR, 0);

    spr_write(SPR_TBL_W,WD_PERIOD - WD_TIMEOUT);
    spr_write(SPR_TBU_W,0);

    spr_write(SPR_TCR, 0);
    spr_write(SPR_TSR_RC, 0xFFFFFFFF);

    msr_write(msr_read()
            | 1 << ITRPT_XSR_EE_i   /* MSR[EE] - Externel interrupt. */
            | 1 << ITRPT_XSR_CE_i); /* MSR[CE] - Critical interrupt. */

    spr_write(SPR_TCR, 1 << TIMER_TCR_WIE_i
                     | 0 << TIMER_TCR_DIE_i
                     | 0 << TIMER_TCR_FIE_i
                     | 0 << TIMER_TCR_ARE_i
                     | TIMER_TCR_WP_2pow21_clocks << TIMER_TCR_WP_i
                     | TIMER_TCR_FP_2pow21_clocks << TIMER_TCR_FP_i
                     | TIMER_TCR_WRC_No_WD_reset  << TIMER_TCR_WRC_i);

    spr_write(SPR_TSR_W, 1 << TIMER_TSR_ENW_i);
}

void timer_stop()
{
    spr_write(SPR_DECAR, 0);
    spr_write(SPR_TCR, 0);
    spr_write(SPR_TSR_RC, 0xFFFFFFFF);
}

void ext_int_set_level(struct rumboot_irq_entry *tbl)
{
    rumboot_printf("\n------------------------------------\n"
                     "Set level sense per interrupt source\n"
                     "------------------------------------\n");
    rumboot_irq_cli();
    rumboot_irq_set_handler( tbl, EXT_INT0, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, irq_handler, &arg0 );
    rumboot_irq_set_handler( tbl, EXT_INT1, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, irq_handler, &arg1 );
    rumboot_irq_set_handler( tbl, EXT_INT2, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, irq_handler, &arg2 );
    rumboot_irq_set_handler( tbl, EXT_INT3, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, irq_handler, &arg3 );
    rumboot_irq_set_handler( tbl, EXT_INT4, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, irq_handler, &arg4 );
    rumboot_irq_set_handler( tbl, EXT_INT5, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, irq_handler, &arg5 );
    rumboot_irq_priority_adjust(tbl, EXT_INT0, MPIC128_PRIOR_3);
    rumboot_irq_priority_adjust(tbl, EXT_INT1, MPIC128_PRIOR_4);
    rumboot_irq_priority_adjust(tbl, EXT_INT2, MPIC128_PRIOR_6);
    rumboot_irq_priority_adjust(tbl, EXT_INT3, MPIC128_PRIOR_7);
    rumboot_irq_priority_adjust(tbl, EXT_INT4, MPIC128_PRIOR_12);
    rumboot_irq_priority_adjust(tbl, EXT_INT5, MPIC128_PRIOR_14);
    rumboot_irq_table_activate( tbl );
    rumboot_irq_sei();
}

void ext_int_set_edge(struct rumboot_irq_entry *tbl)
{
    rumboot_printf("\n------------------------------------\n"
                     "Set edge sense per interrupt source\n"
                     "------------------------------------\n");
    rumboot_irq_cli();
    rumboot_irq_set_handler( tbl, EXT_INT0, RUMBOOT_IRQ_EDGE | RUMBOOT_IRQ_POS, irq_handler, &arg0 );
    rumboot_irq_set_handler( tbl, EXT_INT1, RUMBOOT_IRQ_EDGE | RUMBOOT_IRQ_POS, irq_handler, &arg1 );
    rumboot_irq_set_handler( tbl, EXT_INT2, RUMBOOT_IRQ_EDGE | RUMBOOT_IRQ_POS, irq_handler, &arg2 );
    rumboot_irq_set_handler( tbl, EXT_INT3, RUMBOOT_IRQ_EDGE | RUMBOOT_IRQ_POS, irq_handler, &arg3 );
    rumboot_irq_set_handler( tbl, EXT_INT4, RUMBOOT_IRQ_EDGE | RUMBOOT_IRQ_POS, irq_handler, &arg4 );
    rumboot_irq_set_handler( tbl, EXT_INT5, RUMBOOT_IRQ_EDGE | RUMBOOT_IRQ_POS, irq_handler, &arg5 );
    rumboot_irq_priority_adjust(tbl, EXT_INT0, MPIC128_PRIOR_3);
    rumboot_irq_priority_adjust(tbl, EXT_INT1, MPIC128_PRIOR_4);
    rumboot_irq_priority_adjust(tbl, EXT_INT2, MPIC128_PRIOR_6);
    rumboot_irq_priority_adjust(tbl, EXT_INT3, MPIC128_PRIOR_7);
    rumboot_irq_priority_adjust(tbl, EXT_INT4, MPIC128_PRIOR_12);
    rumboot_irq_priority_adjust(tbl, EXT_INT5, MPIC128_PRIOR_14);
    rumboot_irq_table_activate( tbl );
    rumboot_irq_sei();
}

int main ()
{
    int status = 0;

    test_event_send_test_id("test_oi10_mpic_test");
    rumboot_memfill8_modelling((void*)SRAM0_BASE, 0x1000, 0x00, 0x00); //workaround (init 4KB)

    rumboot_irq_cli();
    rumboot_printf("Set interrupt borders to 11 and 5 priority\n");
    mpic128_set_interrupt_borders( DCR_MPIC128_BASE, MPIC128_PRIOR_11, MPIC128_PRIOR_5 );
    rumboot_printf("Set our own irq exception handler\n");
    rumboot_irq_set_exception_handler(exception_handler);
    rumboot_printf("Set default interrupt handler\n");
    rumboot_irq_set_default_handler(def_handler, NULL);
    tbl = rumboot_irq_create( NULL );
    rumboot_irq_table_activate( tbl );
    rumboot_irq_sei();

    rumboot_printf("---------------- Info ------------------------\n");
    rumboot_printf("IRQ #0 is external interrupt #0               \n");
    rumboot_printf("----------------------------------------------\n");

    rumboot_printf("\n------------------------------------\n"
                     "Checking adjust priority and interrupt generation\n"
                     "------------------------------------\n");

    rumboot_printf("Set level sense for interrupt source\n"
                   "------------------------------------\n");
    rumboot_irq_set_handler( rumboot_irq_table_get(), EXT_INT0, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, irq_handler2, &arg0 );
    rumboot_irq_enable(EXT_INT0);

    for(int k=0; k <=1; k++) {
        if(k==1) {
            rumboot_printf("Set edge sense for interrupt source\n"
                           "------------------------------------\n");
            rumboot_irq_set_handler( rumboot_irq_table_get(), EXT_INT0, RUMBOOT_IRQ_EDGE | RUMBOOT_IRQ_POS, irq_handler2, &arg0 );
        }
        for (int i = 2; i <= 8; i += 2)
        {
            rumboot_printf("Adjust priority to %d\n", i);
            rumboot_irq_priority_adjust(rumboot_irq_table_get(), EXT_INT0, i);
            ext_int_cnt = 0;
            rumboot_printf("Firing IRQ #0\n");
            test_event(TEC_SET_EXT_INT0_WITHOUT_CHECK_TYPE);
            udelay(5);
            if (ext_int_cnt == 0)
            {
                rumboot_printf("IRQ #0 is not arrive!\n");
                rumboot_printf("TEST ERROR\n");
                return 1;
            }
        }
        for (int i = 8; i >= 2; i -= 2)
        {
            rumboot_printf("Adjust priority to %d\n", i);
            rumboot_irq_priority_adjust(rumboot_irq_table_get(), EXT_INT0, i);
            ext_int_cnt = 0;
            rumboot_printf("Firing IRQ #0\n");
            test_event(TEC_SET_EXT_INT0_WITHOUT_CHECK_TYPE);
            udelay(5);
            if (ext_int_cnt == 0)
            {
                rumboot_printf("IRQ #0 is not arrive!\n");
                rumboot_printf("TEST ERROR\n");
                return 1;
            }
        }
        for (int i = 8; i <= 14; i += 2)
        {
            rumboot_printf("Adjust priority to %d\n", i);
            rumboot_irq_priority_adjust(rumboot_irq_table_get(), EXT_INT0, i);
            ext_int_cnt = 0;
            rumboot_printf("Firing IRQ #0\n");
            test_event(TEC_SET_EXT_INT0_WITHOUT_CHECK_TYPE);
            udelay(5);
            if (ext_int_cnt == 0)
            {
                rumboot_printf("IRQ #0 is not arrive!\n");
                rumboot_printf("TEST ERROR\n");
                return 1;
            }
        }
        for (int i = 14; i >= 8; i -= 2)
        {
            rumboot_printf("Adjust priority to %d\n", i);
            rumboot_irq_priority_adjust(rumboot_irq_table_get(), EXT_INT0, i);
            ext_int_cnt = 0;
            rumboot_printf("Firing IRQ #0\n");
            test_event(TEC_SET_EXT_INT0_WITHOUT_CHECK_TYPE);
            udelay(5);
            if (ext_int_cnt == 0)
            {
                rumboot_printf("IRQ #0 is not arrive!\n");
                rumboot_printf("TEST ERROR\n");
                return 1;
            }
        }
    }

    rumboot_printf("---------------- Info ------------------------\n");
    rumboot_printf("IRQ #%d is MPIC128 timer #%d interrupt\n", MPIC128_TIMER_0, Mpic128Timer0);
    rumboot_printf("----------------------------------------------\n");

#define TMR_COUNT    (0x800)

    rumboot_printf("\n------------------------------------\n"
                     "Checking adjust priority and interrupt generation\n"
                     "------------------------------------\n");
    rumboot_irq_set_handler( rumboot_irq_table_get(), MPIC128_TIMER_0, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, tmr_handler, (void *)Mpic128Timer0 );
    rumboot_irq_enable(MPIC128_TIMER_0);
    mpic128_timer_init(DCR_MPIC128_BASE, mpic128_timer_freq_SYS_CLK);

    for(int k=0; k < 1; k++) {
        for (int i = 2; i <= 8; i += 2)
        {
            rumboot_printf("Adjust priority to %d\n", i);
            rumboot_irq_priority_adjust(rumboot_irq_table_get(), MPIC128_TIMER_0, i);
            tmr_int_cnt = 0;
            rumboot_printf("Firing IRQ #%d\n", MPIC128_TIMER_0);
            mpic128_start_timer(DCR_MPIC128_BASE, Mpic128Timer0, TMR_COUNT);
            udelay(100);
            if (tmr_int_cnt == 0)
            {
                rumboot_printf("IRQ #%d is not arrive!\n", MPIC128_TIMER_0);
                rumboot_printf("TEST ERROR\n");
                return 1;
            }
        }
        for (int i = 8; i >= 2; i -= 2)
        {
            rumboot_printf("Adjust priority to %d\n", i);
            rumboot_irq_priority_adjust(rumboot_irq_table_get(), MPIC128_TIMER_0, i);
            tmr_int_cnt = 0;
            rumboot_printf("Firing IRQ #%d\n", MPIC128_TIMER_0);
            mpic128_start_timer(DCR_MPIC128_BASE, Mpic128Timer0, TMR_COUNT);
            udelay(100);
            if (tmr_int_cnt == 0)
            {
                rumboot_printf("IRQ #%d is not arrive!\n", MPIC128_TIMER_0);
                rumboot_printf("TEST ERROR\n");
                return 1;
            }
        }
        for (int i = 8; i <= 14; i += 2)
        {
            rumboot_printf("Adjust priority to %d\n", i);
            rumboot_irq_priority_adjust(rumboot_irq_table_get(), MPIC128_TIMER_0, i);
            tmr_int_cnt = 0;
            rumboot_printf("Firing IRQ #%d\n", MPIC128_TIMER_0);
            mpic128_start_timer(DCR_MPIC128_BASE, Mpic128Timer0, TMR_COUNT);
            udelay(100);
            if (tmr_int_cnt == 0)
            {
                rumboot_printf("IRQ #%d is not arrive!\n", MPIC128_TIMER_0);
                rumboot_printf("TEST ERROR\n");
                return 1;
            }
        }
        for (int i = 14; i >= 8; i -= 2)
        {
            rumboot_printf("Adjust priority to %d\n", i);
            rumboot_irq_priority_adjust(rumboot_irq_table_get(), MPIC128_TIMER_0, i);
            tmr_int_cnt = 0;
            rumboot_printf("Firing IRQ #%d\n", MPIC128_TIMER_0);
            mpic128_start_timer(DCR_MPIC128_BASE, Mpic128Timer0, TMR_COUNT);
            udelay(100);
            if (tmr_int_cnt == 0)
            {
                rumboot_printf("IRQ #%d is not arrive!\n", MPIC128_TIMER_0);
                rumboot_printf("TEST ERROR\n");
                return 1;
            }
        }
    }

    rumboot_printf("---------------- Info ------------------------\n");
    rumboot_printf("IRQ #%d is SW Interrupt\n", MPIC128_IPI_0);
    rumboot_printf("----------------------------------------------\n");

    rumboot_printf("\n------------------------------------\n"
                     "Checking adjust priority and interrupt generation\n"
                     "------------------------------------\n");
    rumboot_printf("Enable SW interrupt\n");
    rumboot_irq_enable(MPIC128_IPI_0);

    for(int k=0; k <=1; k++) {
        if(k==1) {
            rumboot_printf("Set custom interrupt handler\n"
                           "------------------------------------\n");
            rumboot_irq_set_handler( rumboot_irq_table_get(), MPIC128_IPI_0, 0, swirq_handler, NULL );
        }
        for (int i = 2; i <= 8; i += 2)
        {
            rumboot_printf("Adjust priority to %d\n", i);
            rumboot_irq_priority_adjust(rumboot_irq_table_get(), MPIC128_IPI_0, i);
            sw_int_cnt = 0;
            rumboot_printf("Firing SW IRQ\n");
            rumboot_irq_swint(MPIC128_IPI_0);
            udelay(5);
            if (sw_int_cnt == 0)
            {
                rumboot_printf("SW interrupt is not arrive!\n");
                rumboot_printf("TEST ERROR\n");
                return 1;
            }
        }
        for (int i = 8; i >= 2; i -= 2)
        {
            rumboot_printf("Adjust priority to %d\n", i);
            rumboot_irq_priority_adjust(rumboot_irq_table_get(), MPIC128_IPI_0, i);
            sw_int_cnt = 0;
            rumboot_printf("Firing SW IRQ\n");
            rumboot_irq_swint(MPIC128_IPI_0);
            udelay(5);
            if (sw_int_cnt == 0)
            {
                rumboot_printf("SW interrupt is not arrive!\n");
                rumboot_printf("TEST ERROR\n");
                return 1;
            }
        }
        for (int i = 8; i <= 14; i += 2)
        {
            rumboot_printf("Adjust priority to %d\n", i);
            rumboot_irq_priority_adjust(rumboot_irq_table_get(), MPIC128_IPI_0, i);
            sw_int_cnt = 0;
            rumboot_printf("Firing SW IRQ\n");
            rumboot_irq_swint(MPIC128_IPI_0);
            udelay(5);
            if (sw_int_cnt == 0)
            {
                rumboot_printf("SW interrupt is not arrive!\n");
                rumboot_printf("TEST ERROR\n");
                return 1;
            }
        }
        for (int i = 14; i >= 8; i -= 2)
        {
            rumboot_printf("Adjust priority to %d\n", i);
            rumboot_irq_priority_adjust(rumboot_irq_table_get(), MPIC128_IPI_0, i);
            sw_int_cnt = 0;
            rumboot_printf("Firing SW IRQ\n");
            rumboot_irq_swint(MPIC128_IPI_0);
            udelay(5);
            if (sw_int_cnt == 0)
            {
                rumboot_printf("SW interrupt is not arrive!\n");
                rumboot_printf("TEST ERROR\n");
                return 1;
            }
        }
    }

    rumboot_printf("---------------- Info ------------------------\n");
    rumboot_printf("IRQ #0 is Non critical Interrupt, priority 3  \n"
                   "IRQ #1 is Non critical Interrupt, priority 4  \n");
    rumboot_printf("IRQ #2 is Critical Interrupt, priority 6      \n"
                   "IRQ #3 is Critical Interrupt, priority 7      \n");
    rumboot_printf("IRQ #4 is Machine Check Interrupt, priority 12\n"
                   "IRQ #5 is Machine Check Interrupt, priority 14\n");
    rumboot_printf("----------------------------------------------\n");

#define EXT_INT_TEST_COUNT 2
#define EXT_INT_TIMEOUT 48
#define EXT_INT_TIMEOUT2 320

    rumboot_printf("\n------------------------------------\n"
                     "Checking interrupt generation       \n"
                     "------------------------------------\n");
    initcnt();

    for (int k = 0; k < EXT_INT_TEST_COUNT; ++k)
    {
        if((k & 1) == 0) ext_int_set_level(tbl);
        else  ext_int_set_edge(tbl);

        exp_int_cnt_inc(0);
        test_event(TEC_SET_EXT_INT0);
        for (int i = 0; i < EXT_INT_TIMEOUT; ++i) { nop(); }
        exp_int_cnt_inc(1);
        test_event(TEC_SET_EXT_INT1);
        for (int i = 0; i < EXT_INT_TIMEOUT; ++i) { nop(); }
        exp_int_cnt_inc(2);
        test_event(TEC_SET_EXT_INT2);
        for (int i = 0; i < EXT_INT_TIMEOUT; ++i) { nop(); }
        exp_int_cnt_inc(2);
        test_event(TEC_SET_EXT_INT3);
        for (int i = 0; i < EXT_INT_TIMEOUT; ++i) { nop(); }
        exp_int_cnt_inc(4);
        test_event(TEC_SET_EXT_INT4);
        for (int i = 0; i < EXT_INT_TIMEOUT; ++i) { nop(); }
        exp_int_cnt_inc(5);
        test_event(TEC_SET_EXT_INT5);
        for (int i = 0; i < EXT_INT_TIMEOUT; ++i) { nop(); }
        exp_int_cnt_inc(0);
        exp_int_cnt_inc(1);
        test_event(TEC_SET_EXT_INT01);
        for (int i = 0; i < EXT_INT_TIMEOUT; ++i) { nop(); }
        exp_int_cnt_inc(2);
        exp_int_cnt_inc(3);
        test_event(TEC_SET_EXT_INT23);
        for (int i = 0; i < EXT_INT_TIMEOUT; ++i) { nop(); }
        exp_int_cnt_inc(4);
        exp_int_cnt_inc(5);
        test_event(TEC_SET_EXT_INT45);
        for (int i = 0; i < EXT_INT_TIMEOUT; ++i) { nop(); }
        exp_int_cnt_inc(1);
        exp_int_cnt_inc(3);
        test_event(TEC_SET_EXT_INT13);
        for (int i = 0; i < EXT_INT_TIMEOUT; ++i) { nop(); }
        exp_int_cnt_inc(1);
        exp_int_cnt_inc(5);
        test_event(TEC_SET_EXT_INT15);
        for (int i = 0; i < EXT_INT_TIMEOUT; ++i) { nop(); }
        exp_int_cnt_inc(3);
        exp_int_cnt_inc(5);
        test_event(TEC_SET_EXT_INT35);
        for (int i = 0; i < EXT_INT_TIMEOUT; ++i) { nop(); }
        exp_int_cnt_inc(0);
        exp_int_cnt_inc(1);
        exp_int_cnt_inc(2);
        exp_int_cnt_inc(3);
        exp_int_cnt_inc(4);
        exp_int_cnt_inc(5);
        test_event(TEC_SET_EXT_INT012345);

        for (int i = 0; i < EXT_INT_TIMEOUT; ++i) { nop(); }
        exp_int_cnt_inc(4);
        exp_int_cnt_inc(3);
        test_event(TEC_SET_EXT_INT4_AFTER_WR_IAR);
        test_event(TEC_SET_EXT_INT3);
        for (int i = 0; i < EXT_INT_TIMEOUT; ++i) { nop(); }
        exp_int_cnt_inc(4);
        exp_int_cnt_inc(5);
        exp_int_cnt_inc(3);
        test_event(TEC_SET_EXT_INT45_AFTER_WR_IAR);
        test_event(TEC_SET_EXT_INT3);
        for (int i = 0; i < EXT_INT_TIMEOUT; ++i) { nop(); }
        exp_int_cnt_inc(2);
        exp_int_cnt_inc(1);
        test_event(TEC_SET_EXT_INT2_AFTER_WR_CIAR);
        test_event(TEC_SET_EXT_INT1);
        for (int i = 0; i < EXT_INT_TIMEOUT; ++i) { nop(); }

        dec_timer_start();
        exp_int_cnt_inc(0);
        test_event(TEC_SET_EXT_INT0_AFTER_DEC_TIMER);
        for (int i = 0; i < EXT_INT_TIMEOUT2; ++i) { nop(); }
        exp_int_cnt_inc(3);
        test_event(TEC_SET_EXT_INT3_AFTER_DEC_TIMER);
        for (int i = 0; i < EXT_INT_TIMEOUT2; ++i) { nop(); }
        exp_int_cnt_inc(0);
        test_event(TEC_SET_EXT_INT0_BEFORE_DEC_TIMER);
        for (int i = 0; i < EXT_INT_TIMEOUT2; ++i) { nop(); }
        exp_int_cnt_inc(3);
        test_event(TEC_SET_EXT_INT3_BEFORE_DEC_TIMER);
        for (int i = 0; i < EXT_INT_TIMEOUT2; ++i) { nop(); }
        timer_stop();

        exp_int_cnt_inc(3);
        wd_timer_start();
        test_event(TEC_SET_EXT_INT3_AFTER_WD_TIMER);
        for (int i = 0; i < EXT_INT_TIMEOUT2; ++i) { nop(); }
        exp_int_cnt_inc(5);
        wd_timer_start();
        test_event(TEC_SET_EXT_INT5_AFTER_WD_TIMER);
        for (int i = 0; i < EXT_INT_TIMEOUT2; ++i) { nop(); }
        exp_int_cnt_inc(3);
        wd_timer_start();
        test_event(TEC_SET_EXT_INT3_BEFORE_WD_TIMER);
        for (int i = 0; i < EXT_INT_TIMEOUT2; ++i) { nop(); }
        exp_int_cnt_inc(5);
        wd_timer_start();
        test_event(TEC_SET_EXT_INT5_BEFORE_WD_TIMER);
        for (int i = 0; i < EXT_INT_TIMEOUT2; ++i) { nop(); }
        timer_stop();

        test_event(TEC_CLR_EXT_INT_ALL);
        for (int i = 0; i < EXT_INT_TIMEOUT2; ++i) { nop(); }
    }

    if(exp_noncritical_int_cnt != noncritical_int_cnt)
    {
        rumboot_printf("Error! Some noncritical interrupts were not recieved.\n");
        status += 1;
    }
    if(exp_critical_int_cnt != critical_int_cnt)
    {
        rumboot_printf("Error! Some critical interrupts were not recieved.\n");
        status += 1;
    }
    if(exp_mc_int_cnt != mc_int_cnt)
    {
        rumboot_printf("Error! Some machine check interrupts were not recieved.\n");
        status += 1;
    }

    if(status == 0)
        rumboot_printf("TEST OK\n");
    else
        rumboot_printf("TEST ERROR\n");

    return status;
}
