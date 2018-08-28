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

#include <platform/arch/ppc/ppc_476fp_lib_c.h>
#include <platform/common_macros/common_macros.h>
#include <platform/test_event_codes.h>
#include <platform/test_assert.h>
#include <rumboot/regpoker.h>
#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <arch/irq_macros.h>
#include <platform/devices.h>
#include <rumboot/macros.h> //dcr_write/dcr_read
#include <platform/arch/ppc/test_macro.h>
//#include <devices.h>
#include <platform/interrupts.h>

#include <platform/regs/regs_mpic128.h>
#include <platform/devices/mpic128.h>

#include <regs/regs_sp804.h>

//#define TIM1_INTERRUPT_NUMBER   40
//#define TIM2_INTERRUPT_NUMBER   41
//#define TIMER_INT_TIMEOUT       60
static volatile uint32_t IRQ = 0;

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

static uint32_t check_timer_default_ro_val( uint32_t base_addr )
{
    rumboot_printf("Check the default values of the registers:");

    struct regpoker_checker check_default_array[] = {
          {   "TimerPeriphID0",   REGPOKER_READ32,    DIT_REG_PERIPHID0,      0xFE0, 0xff   }, /* check only first of 256 */
          {   "TimerPeriphID1",   REGPOKER_READ32,    DIT_REG_PERIPHID1,      0xFE4, 0xff   },
          {   "TimerPeriphID2",   REGPOKER_READ32,    DIT_REG_PERIPHID2,      0xFE8, 0xff   },
          {   "TimerPeriphID3",   REGPOKER_READ32,    DIT_REG_PERIPHID3,      0x00,  0xff   },
          {   "TimerPCellID0",    REGPOKER_READ32,    DIT_REG_PCELLID0,       0x0D,  0xff   },
          {   "TimerPCellID1",    REGPOKER_READ32,    DIT_REG_PCELLID1,       0xF0,  0xff   },
          {   "TimerPCellID2",    REGPOKER_READ32,    DIT_REG_PCELLID2,       0x05,  0xff   },
          {   "TimerPCellID3",    REGPOKER_READ32,    DIT_REG_PCELLID3,       0xB1,  0xff   },
          { /* Sentinel */ }
      };

    if( rumboot_regpoker_check_array( check_default_array, base_addr ) == 0 ) {
        rumboot_printf( " OK\n" );
        return 0;
    }

    rumboot_printf( " ERROR\n" );
    return 1;
}

static uint32_t check_timer_default_rw_val( uint32_t base_addr )
{
    rumboot_printf("Check the default values of the registers:");

    struct regpoker_checker check_default_array[] = {
          {   "Timer1Load",     REGPOKER_READ32,    DIT0_REG_LOAD0,          0x000,          0xff   },
          {   "Timer1Value",    REGPOKER_READ32,    DIT0_REG_VALUE0,         0x04,           0xff   },
          {   "Timer1Control",  REGPOKER_READ32,    DIT0_REG_CONTROL0,       0x08,           0xff   },
          {   "Timer1IntClr",   REGPOKER_READ32,    DIT0_REG_INTCLR0,        0x0C,           0xff   },
          {   "Timer1RIS",      REGPOKER_READ32,    DIT0_REG_RIS0,           0x10,           0xff   },
          {   "Timer1MIS",      REGPOKER_READ32,    DIT0_REG_MIS0,           0x14,           0xff   },
          {   "Timer1BGLoad",   REGPOKER_READ32,    DIT0_REG_BGLOAD0,        0x18,           0xff   },
          {   "Timer2Load",     REGPOKER_READ32,    DIT0_REG_LOAD1,          0x20,           0xff   },
          {   "Timer2Value",    REGPOKER_READ32,    DIT0_REG_VALUE1,         0x24,           0xff   },
          {   "Timer2Control",  REGPOKER_READ32,    DIT0_REG_CONTROL1,       0x28,           0xff   },
          {   "Timer2IntClr",   REGPOKER_READ32,    DIT0_REG_INTCLR1,        0x2C,           0xff   },
          {   "Timer2RIS",      REGPOKER_READ32,    DIT0_REG_RIS1,           0x30,           0xff   },
          {   "Timer2MIS",      REGPOKER_READ32,    DIT0_REG_MIS1,           0x34,           0xff   },
          {   "Timer2BGLoad",   REGPOKER_READ32,    DIT0_REG_BGLOAD1,        0x38,           0xff   },
          {   "TimerITCR",      REGPOKER_READ32,    DIT_REG_ITCR,            0xf0,           0xff   },
          {   "TimerITOP",      REGPOKER_READ32,    DIT_REG_ITOP,            0xf04,          0xff   },
          { }
      };


    if( rumboot_regpoker_check_array( check_default_array, base_addr ) == 0 )
    {
        rumboot_printf( "OK\n" );
        return 0;
    }

    rumboot_printf( "ERROR\n" );
    return 1;
}

static void handler( int irq, void *arg ) {
    rumboot_printf( "IRQ arrived  \n" );
    gpio_clear_edge_int( DIT_INT0, 0xFF );
    gpio_clear_edge_int( DIT_INT1, 0xFF );
    rumboot_printf( "Clear interrupts\n" );
    IRQ = 1;
}


void check_timers_irq(void)
{
//    rumboot_putstring("NON_CR_INTERRUPT_HANDLER\n");
    //get interrupt source
    //uint32_t source_vector = mpic128_get_ncr_interrupt_vector( DCR_MPIC128_BASE );// - clear interrupt by read NCIAR
    IRQ = 0;
    rumboot_irq_cli();
    struct rumboot_irq_entry *tbl1 = rumboot_irq_create( NULL );
    struct rumboot_irq_entry *tbl2 = rumboot_irq_create( NULL );

   rumboot_irq_set_handler( tbl1, DIT_INT0, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler, ( void* )0 );
   rumboot_irq_set_handler( tbl2, DIT_INT1, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler, ( void* )0 );


    if(source_vector == TIM1_INTERRUPT_NUMBER)
    {
        timer_stop(1);
        TIMINT1_HANDLED = 1;
//        rumboot_putstring("source_vector == TIM1_INTERRUPT_NUMBER\n");
//        trace_hex(TIMINT1_HANDLED);
    }
    else if(source_vector == TIM2_INTERRUPT_NUMBER)
    {
        timer_stop(2);
        TIMINT2_HANDLED = 1;
//        rumboot_putstring("source_vector == TIM2_INTERRUPT_NUMBER\n");
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
    while(!TIMINT1_HANDLED && (t++ < TIMER_INT_TIMEOUT)){;}
//    rumboot_putstring("while(!TIMINT1_HANDLED && (t++<TIMER_INT_TIMEOUT))\n");
//    trace_hex(TIMINT1_HANDLED);
    return TIMINT1_HANDLED;
}

uint32_t wait_timint2_handled(){
    int t = 0;
    while(!TIMINT2_HANDLED && ( t++ < TIMER_INT_TIMEOUT)){;}
//    rumboot_putstring("while(!TIMINT2_HANDLED && (t++<TIMER_INT_TIMEOUT))\n");
//    trace_hex(TIMINT2_HANDLED);
    return TIMINT2_HANDLED;
}

uint32_t check_timer_regs()
{
    uint32_t i;
    rumboot_printf("check_timer_regs\n");
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
    rumboot_putstring("check_tcr\n");
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
        rumboot_putstring("Timint1 and/or Timint2 are not handled\n");
        timer_itc_disable();
        timer_itc_set_output(0,0);
        return 0;
    }
}

uint32_t check_timint1_timint2()
{
   rumboot_putstring("check_timint1_timint2\n");
   mpic128_reset( DCR_MPIC128_BASE );
   mpic128_pass_through_disable( DCR_MPIC128_BASE );
   //setup MPIC128 interrupt 36
  // mpic128_setup_ext_interrupt( DCR_MPIC128_BASE, TIM1_INTERRUPT_NUMBER,MPIC128_PRIOR_1,int_sense_level,int_pol_pos,Processor0);
 //  mpic128_setup_ext_interrupt( DCR_MPIC128_BASE, TIM2_INTERRUPT_NUMBER,MPIC128_PRIOR_2,int_sense_level,int_pol_pos,Processor0);
   mpic128_setup_ext_interrupt( DCR_MPIC128_BASE, TIM1_INTERRUPT_NUMBER, int_sense_level);
   mpic128_setup_ext_interrupt( DCR_MPIC128_BASE, TIM2_INTERRUPT_NUMBER, int_sense_level);

   SET_NONCR_INT_HANDLER(non_cr_interrupt_handler);
   ppc_noncr_int_enable();//MSR setup
   mpic128_set_interrupt_borders( DCR_MPIC128_BASE, 10, 5);//MCK, CR borders
   //mpic128_set_minimal_interrupt_priority_mask( DCR_MPIC128_BASE, Processor0, MPIC128_PRIOR_0);
   mpic128_set_minimal_interrupt_priority_mask( DCR_MPIC128_BASE );
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
