/*
 * test_oi10_ctrl_004.c
 *
 *  Created on: Aug 24, 2018
 *      Author: m.dubinkin
 */

/*
#include <mivem_test.h>
#include <ppc_c.h>
#include <sys_timer.h>
#include <mpic128.h>
#include <test_event_codes.h>
#include <mivem_regs_access.h>
#include "test_oi10_ctrl_004.h"
#include <trace.h>
#include <ppc_c.h>
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <rumboot/printf.h>

#include <ppc_476fp_lib_c.h>
#include <platform/common_macros/common_macros.h>
#include <platform/test_event_codes.h>
#include <platform/test_assert.h>
#include <rumboot/regpoker.h>
#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <irq_macros.h>
#include <platform/devices.h>
#include <macros.h> //dcr_write/dcr_read
#include <test_macro.h>
#include <devices.h>
#include <test_oi10_ctrl_004.h>

#include <regs/regs_mpic128.h>
#include <mpic128.h>


static uint32_t volatile TIMINT1_HANDLED = 0;
static uint32_t volatile TIMINT2_HANDLED = 0;
static int32_t check_array32[] = {
        0xFFFFFFFF,
        0x00000000,
        0xFFFF0000,
        0x0000FFFF,
        0xFF00FF00,
        0x00FF00FF,
        0xF0F0F0F0,
        0x0F0F0F0F,
        0xCCCCCCCC,
        0x33333333,
        0xAAAAAAAA,
        0x55555555
};


void NON_CR_INTERRUPT_HANDLER(void)
{
//    trace_msg("NON_CR_INTERRUPT_HANDLER\n");
    //get interrupt source
    uint32_t source_vector = mpic128_get_ncr_interrupt_vector( DCR_MPIC128_BASE, Processor0);// - clear interrupt by read NCIAR
    if(source_vector == TIM1_INTERRUPT_NUMBER)
    {
        timer_stop(1);
        TIMINT1_HANDLED = 1;
//        trace_msg("source_vector == TIM1_INTERRUPT_NUMBER\n");
//        trace_hex(TIMINT1_HANDLED);
    }
    else if(source_vector == TIM2_INTERRUPT_NUMBER)
    {
        timer_stop(2);
        TIMINT2_HANDLED = 1;
//        trace_msg("source_vector == TIM2_INTERRUPT_NUMBER\n");
//        trace_hex(TIMINT2_HANDLED);
    }

    //if test control (itc) is enabled
    if(timer_read_TIMERITCR(DCR_TIMERS_BASE) == 0x1)
    {   //clear handled output
        timer_itc_set_output(!TIMINT1_HANDLED,!TIMINT2_HANDLED);
        //if all interrupts handled then disable itc
        if(TIMINT1_HANDLED && TIMINT2_HANDLED)
            timer_itc_disable();
    }
    MPIC128_dcr_write_MPIC128x_NCEOI0( DCR_MPIC128_BASE, 0);
}

uint32_t wait_timint1_handled(){
    int t = 0;
    while(!TIMINT1_HANDLED && (t++<TIMER_INT_TIMEOUT)){;}
//    trace_msg("while(!TIMINT1_HANDLED && (t++<TIMER_INT_TIMEOUT))\n");
//    trace_hex(TIMINT1_HANDLED);
    return TIMINT1_HANDLED;
}

uint32_t wait_timint2_handled(){
    int t = 0;
    while(!TIMINT2_HANDLED && ( t++ < TIMER_INT_TIMEOUT)){;}
//    trace_msg("while(!TIMINT2_HANDLED && (t++<TIMER_INT_TIMEOUT))\n");
//    trace_hex(TIMINT2_HANDLED);
    return TIMINT2_HANDLED;
}

uint32_t check_timer_regs()
{
    uint32_t i;
    trace_msg("check_timer_regs\n");
    COMPARE_VALUES( (timer_read_TIMER1LOAD(DCR_TIMERS_BASE)      &    TIMER1LOAD_MASK),      TIMER1LOAD_VALUE,       "check TIMER1LOAD failed");
    COMPARE_VALUES( (timer_read_TIMER1VALUE(DCR_TIMERS_BASE)     &    TIMER1VALUE_MASK),     TIMER1VALUE_VALUE,      "check TIMER1VALUE failed");
    COMPARE_VALUES( (timer_read_TIMER1CONTROL(DCR_TIMERS_BASE)   &    TIMER1CONTROL_MASK),   TIMER1CONTROL_VALUE,    "check TIMER1CONTROL failed");
    COMPARE_VALUES( (timer_read_TIMER1RIS(DCR_TIMERS_BASE)       &    TIMER1RIS_MASK),       TIMER1RIS_VALUE,        "check TIMER1RIS failed");
    COMPARE_VALUES( (timer_read_TIMER1MIS(DCR_TIMERS_BASE)       &    TIMER1MIS_MASK),       TIMER1MIS_VALUE,        "check TIMER1MIS failed");
    COMPARE_VALUES( (timer_read_TIMER1BGLOAD(DCR_TIMERS_BASE)    &    TIMER1BGLOAD_MASK),    TIMER1BGLOAD_VALUE,     "check TIMER1BGLOAD failed");
    COMPARE_VALUES( (timer_read_TIMER2LOAD(DCR_TIMERS_BASE)      &    TIMER2LOAD_MASK),      TIMER2LOAD_VALUE,       "check TIMER2LOAD failed");
    COMPARE_VALUES( (timer_read_TIMER2VALUE(DCR_TIMERS_BASE)     &    TIMER2VALUE_MASK),     TIMER2VALUE_VALUE,      "check TIMER2VALUE failed");
    COMPARE_VALUES( (timer_read_TIMER2CONTROL(DCR_TIMERS_BASE)   &    TIMER2CONTROL_MASK),   TIMER2CONTROL_VALUE,    "check TIMER2CONTROL failed");
    COMPARE_VALUES( (timer_read_TIMER2RIS(DCR_TIMERS_BASE)       &    TIMER2RIS_MASK),       TIMER2RIS_VALUE,        "check TIMER2RIS failed");
    COMPARE_VALUES( (timer_read_TIMER2MIS(DCR_TIMERS_BASE)       &    TIMER2MIS_MASK),       TIMER2MIS_VALUE,        "check TIMER2MIS failed");
    COMPARE_VALUES( (timer_read_TIMER2BGLOAD(DCR_TIMERS_BASE)    &    TIMER2BGLOAD_MASK),    TIMER2BGLOAD_VALUE,     "check TIMER2BGLOAD failed");
    COMPARE_VALUES( (timer_read_TIMERITCR(DCR_TIMERS_BASE)       &    TIMERITCR_MASK),       TIMERITCR_VALUE,        "check TIMERITCR failed");
    COMPARE_VALUES( (timer_read_TIMERPERIPHID0(DCR_TIMERS_BASE)  &    TIMERPERIPHID0_MASK),  TIMERPERIPHID0_VALUE,   "check TIMERPERIPHID0 failed");
    COMPARE_VALUES( (timer_read_TIMERPERIPHID1(DCR_TIMERS_BASE)  &    TIMERPERIPHID1_MASK),  TIMERPERIPHID1_VALUE,   "check TIMERPERIPHID1 failed");
    COMPARE_VALUES( (timer_read_TIMERPERIPHID2(DCR_TIMERS_BASE)  &    TIMERPERIPHID2_MASK),  TIMERPERIPHID2_VALUE,   "check TIMERPERIPHID2 failed");
    COMPARE_VALUES( (timer_read_TIMERPERIPHID3(DCR_TIMERS_BASE)  &    TIMERPERIPHID3_MASK),  TIMERPERIPHID3_VALUE,   "check TIMERPCELLID0 failed");
    COMPARE_VALUES( (timer_read_TIMERPCELLID0(DCR_TIMERS_BASE)   &    TIMERPCELLID0_MASK),   TIMERPCELLID0_VALUE,    "check TIMERPCELLID1 failed");
    COMPARE_VALUES( (timer_read_TIMERPCELLID1(DCR_TIMERS_BASE)   &    TIMERPCELLID1_MASK),   TIMERPCELLID1_VALUE,    "check TIMERPCELLID2 failed");
    COMPARE_VALUES( (timer_read_TIMERPCELLID2(DCR_TIMERS_BASE)   &    TIMERPCELLID2_MASK),   TIMERPCELLID2_VALUE,    "check TIMERPCELLID2 failed");
    COMPARE_VALUES( (timer_read_TIMERPCELLID3(DCR_TIMERS_BASE)   &    TIMERPCELLID3_MASK),   TIMERPCELLID3_VALUE,    "check TIMERPCELLID3 failed");

    for (i = 0; i< 12; i++){
        //timer_write_TIMER1BGLOAD(DCR_TIMERS_BASE, check_array32[i]);
        dcr_write(DCR_TIMERS_BASE, check_array32[i]);
        COMPARE_VALUES(timer_read_TIMER1BGLOAD(DCR_TIMERS_BASE), check_array32[i],"SysTimer DCR data bus check failed");
    }

    return 1;
}

uint32_t check_tcr()
{
    /*
     * set tim1 interrupt using ITC mode.
     * clear interrupt using ITC mode
     */
    trace_msg("check_tcr\n");
    mpic128_reset( DCR_MPIC128_BASE );
    mpic128_pass_through_disable( DCR_MPIC128_BASE );
    //setup MPIC128 interrupt 36
    //mpic128_setup_ext_interrupt( DCR_MPIC128_BASE, TIM1_INTERRUPT_NUMBER,MPIC128_PRIOR_1,int_sense_level,int_pol_pos,Processor0);
    //mpic128_setup_ext_interrupt( DCR_MPIC128_BASE, TIM2_INTERRUPT_NUMBER,MPIC128_PRIOR_2,int_sense_level,int_pol_pos,Processor0);
    mpic128_setup_ext_interrupt( DCR_MPIC128_BASE, TIM1_INTERRUPT_NUMBER, int_sense_level);
    mpic128_setup_ext_interrupt( DCR_MPIC128_BASE, TIM2_INTERRUPT_NUMBER, int_sense_level);

    SET_NONCR_INT_HANDLER(non_cr_interrupt_handler);
    ppc_noncr_int_enable();//MSR setup
    mpic128_set_interrupt_borders( DCR_MPIC128_BASE, 10, 5);//MCK, CR borders
    //mpic128_set_minimal_interrupt_priority_mask( DCR_MPIC128_BASE, Processor0, MPIC128_PRIOR_0);
    TIMINT1_HANDLED = 0;
    TIMINT2_HANDLED = 0;
    //mpic128 is ready for interrupt

    //timer going to integration test control mode
    timer_itc_enable();
    timer_itc_set_output(1,1);
    //wait handler
    if(wait_timint1_handled() && wait_timint2_handled())
        return 1;
    else{
        trace_msg("Timint1 and/or Timint2 are not handled\n");
        timer_itc_disable();
        timer_itc_set_output(0,0);
        return 0;
    }
}

uint32_t check_timint1_timint2()
{
    trace_msg("check_timint1_timint2\n");
   mpic128_reset( DCR_MPIC128_BASE );
   mpic128_pass_through_disable( DCR_MPIC128_BASE );
   //setup MPIC128 interrupt 36
   mpic128_setup_ext_interrupt( DCR_MPIC128_BASE, TIM1_INTERRUPT_NUMBER,MPIC128_PRIOR_1,int_sense_level,int_pol_pos,Processor0);
   mpic128_setup_ext_interrupt( DCR_MPIC128_BASE, TIM2_INTERRUPT_NUMBER,MPIC128_PRIOR_2,int_sense_level,int_pol_pos,Processor0);
   SET_NONCR_INT_HANDLER(non_cr_interrupt_handler);
   ppc_noncr_int_enable();//MSR setup
   mpic128_set_interrupt_borders( DCR_MPIC128_BASE, 10, 5);//MCK, CR borders
   mpic128_set_minimal_interrupt_priority_mask( DCR_MPIC128_BASE, Processor0, MPIC128_PRIOR_0);
   TIMINT1_HANDLED = 0;
   TIMINT2_HANDLED = 0;
   //mpic128 is ready for interrupt

    timer_info t_info1 = {
        .timer_number = 1,
        .load_value = 0x300,
        .run_mode = mode_free_run,
        .int_enable = 1,//TIMINT1 active
        .prescale = ps_div_by_1,
        .timer_size = timer_size_16bit,
        .one_shot_count = 1 //counter decrements once
    };
    timer_info t_info2 = {
        .timer_number = 2,
        .load_value = 0x300,
        .run_mode = mode_free_run,
        .int_enable = 1,//TIMINT2 active
        .prescale = ps_div_by_1,
        .timer_size = timer_size_16bit,
        .one_shot_count = 1 //counter decrements once
    };
    //One-shot
    //start first timer
    timer_start(&t_info1);
    //start 2nd timer
    timer_start(&t_info2);
    //wait for interrupt handling
    if(wait_timint1_handled() && wait_timint2_handled()){
        return 1;
    }else{
        timer_stop(1);
        timer_stop(2);
        return 0;
    }
    return 1;
}

uint32_t main(void){
    /*
     * Test scenario:
     * - check APB connection
     * - setup TCR, wait TIMINT1,2
     * - setup timers, wait TIMINT1,2
     */
    TEST_ASSERT(check_timer_regs() == 1,"check_timer_regs failed");
    TEST_ASSERT(check_tcr() == 1,"check TCR interrupt generation failed");
    TEST_ASSERT(check_timint1_timint2() == 1,"check_timint1_timint2 generation failed");
    return 0;
}
