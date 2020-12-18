/*
 * test_oi10_cpu_ppc_timer.c
 *
 *  Created on: Dec 5, 2018
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

#include <arch/ppc_476fp_config.h>
#include <arch/ppc_476fp_lib_c.h>
#include <platform/devices.h>
#include <platform/arch/ppc/test_macro.h>
#include <platform/test_event_c.h>
#include <platform/trace.h>
#include <platform/test_assert.h>
#include <arch/ppc_476fp_ctrl_fields.h>
#include <arch/ppc_476fp_timer_fields.h>
#include <arch/ppc_476fp_debug_fields.h>

#define DEC_PERIOD  0x800

enum {
    TEC_START_CHECK_WD_MODE_CPU_CLK = TEST_EVENT_CODE_MIN,
    TEC_START_CHECK_WD_MODE_CPU_CLK_4,
    TEC_START_CHECK_FIX_MODE_CPU_CLK,
    TEC_START_CHECK_FIX_MODE_CPU_CLK_4,
    TEC_START_CHECK_DEC_MODE_CPU_CLK,
    TEC_START_CHECK_DEC_MODE_CPU_CLK_4,
    TEC_START_CHECK_FREQUENCY_CPU_CLK,
    TEC_START_CHECK_FREQUENCY_CPU_CLK_4
} test_event_code;

volatile static int DEC_cnt, FIX_cnt, WD_cnt;

static void my_exception_handler(int id, const char *name)
{
        switch(id){
        case RUMBOOT_IRQ_DECREMENTER:
            spr_write(SPR_TSR_RC, 1 << TIMER_TSR_DIS_i);
            DEC_cnt += 1;
            spr_write(SPR_TCR, spr_read(SPR_TCR) & (~(1 << TIMER_TCR_DIE_i)));
            break;
        case RUMBOOT_IRQ_FIXED_INTERVAL_TIMER:
            spr_write(SPR_TSR_RC, 1 << TIMER_TSR_FIS_i);
            FIX_cnt += 1;
            spr_write(SPR_TCR, spr_read(SPR_TCR) & (~(1 << TIMER_TCR_FIE_i)));
            break;
        case RUMBOOT_IRQ_WATCHDOG_TIMER:
            spr_write(SPR_TSR_RC, 1 << TIMER_TSR_WIS_i);
            WD_cnt += 1;
            spr_write(SPR_TCR, spr_read(SPR_TCR) & (~(1 << TIMER_TCR_WIE_i)));
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

int check_from_CPU_CLK()
{
    rumboot_printf("Start test. Source clock is the CPU clock (CPU_CLK).\n");

    DEC_cnt = 0;
    FIX_cnt = 0;
    WD_cnt  = 0;

    spr_write(SPR_CCR1,CTRL_CCR1_TSS_CPU_clock << CTRL_CCR1_TSS_i);

    spr_write(SPR_DEC, 0);
    spr_write(SPR_DECAR, 0);

    test_event(TEC_START_CHECK_FREQUENCY_CPU_CLK);
    test_event(TEC_START_CHECK_DEC_MODE_CPU_CLK);
    test_event(TEC_START_CHECK_FIX_MODE_CPU_CLK);
    test_event(TEC_START_CHECK_WD_MODE_CPU_CLK);

    spr_write(SPR_TBL_W,0);
    spr_write(SPR_TBU_W,0);

    spr_write(SPR_TCR, 0);
    spr_write(SPR_TSR_RC, 0xFFFFFFFF);

    spr_write(SPR_DEC, DEC_PERIOD);
    spr_write(SPR_DECAR, DEC_PERIOD);

    msr_write(msr_read()
            | 1 << ITRPT_XSR_EE_i   /* MSR[EE] - Externel interrupt. */
            | 1 << ITRPT_XSR_CE_i); /* MSR[CE] - Critical interrupt. */

    spr_write(SPR_TCR, 1 << TIMER_TCR_WIE_i
                     | 1 << TIMER_TCR_DIE_i
                     | 1 << TIMER_TCR_FIE_i
                     | 1 << TIMER_TCR_ARE_i
                     | TIMER_TCR_WP_2pow21_clocks << TIMER_TCR_WP_i
                     | TIMER_TCR_FP_2pow13_clocks << TIMER_TCR_FP_i
                     | TIMER_TCR_WRC_No_WD_reset  << TIMER_TCR_WRC_i);

    spr_write(SPR_TSR_W, 1 << TIMER_TSR_ENW_i);

    rumboot_printf("Waiting for interrupts.\n");
    while(1){
        if((DEC_cnt >= 1) && (FIX_cnt >= 1) && (WD_cnt >= 1)) break;
    };

    rumboot_printf("Test done.\n");
	return 0;
}

int check_from_EXT_CLK()
{
    rumboot_printf("Start test. Source clock is the external clock (CPU_CLK/4).\n");

    DEC_cnt = 0;
    FIX_cnt = 0;
    WD_cnt  = 0;

    spr_write(SPR_CCR1,CTRL_CCR1_TSS_alt_clock << CTRL_CCR1_TSS_i);

    spr_write(SPR_DEC, 0);
    spr_write(SPR_DECAR, 0);

    test_event(TEC_START_CHECK_FREQUENCY_CPU_CLK_4);
    test_event(TEC_START_CHECK_DEC_MODE_CPU_CLK_4);
    test_event(TEC_START_CHECK_FIX_MODE_CPU_CLK_4);
    test_event(TEC_START_CHECK_WD_MODE_CPU_CLK_4);

    spr_write(SPR_TBL_W,0);
    spr_write(SPR_TBU_W,0);

    spr_write(SPR_TCR, 0);
    spr_write(SPR_TSR_RC, 0xFFFFFFFF);

    spr_write(SPR_DEC, DEC_PERIOD);
    spr_write(SPR_DECAR, DEC_PERIOD);

    msr_write(msr_read()
            | 1 << ITRPT_XSR_EE_i   /* MSR[EE] - Externel interrupt. */
            | 1 << ITRPT_XSR_CE_i); /* MSR[CE] - Critical interrupt. */

    spr_write(SPR_TCR, 1 << TIMER_TCR_WIE_i
                     | 1 << TIMER_TCR_DIE_i
                     | 1 << TIMER_TCR_FIE_i
                     | 1 << TIMER_TCR_ARE_i
                     | TIMER_TCR_WP_2pow21_clocks << TIMER_TCR_WP_i
                     | TIMER_TCR_FP_2pow13_clocks << TIMER_TCR_FP_i
                     | TIMER_TCR_WRC_No_WD_reset  << TIMER_TCR_WRC_i);

    spr_write(SPR_TSR_W, 1 << TIMER_TSR_ENW_i);


    rumboot_printf("Waiting for interrupts.\n");
    while(1){
        if((DEC_cnt >= 1) && (FIX_cnt >= 1) && (WD_cnt >= 1)) break;
    };

    rumboot_printf("Test done.\n");
    return 0;
}

int main()
{
	test_event_send_test_id("test_oi10_cpu_ppc_timer");

    /* Set our own handler */
    rumboot_irq_set_exception_handler(my_exception_handler);

    uint32_t const msr_old_value = msr_read();

	if(check_from_CPU_CLK()) {
		rumboot_printf("Test failed!");
        msr_write(msr_old_value);
		return 1;
	}
    if(check_from_EXT_CLK()) {
        rumboot_printf("Test failed!");
        msr_write(msr_old_value);
        return 1;
    }

	msr_write(msr_old_value);
    rumboot_printf("TEST OK\n");

    return 0;
}
