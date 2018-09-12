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
#include <devices/sp804.h>
#include <platform/interrupts.h>

#include <platform/regs/regs_mpic128.h>
#include <platform/devices/mpic128.h>

#include <platform/devices.h>

#include <regs/regs_sp804.h>

//#define TIMER0_CYCLES           1
//#define TIMER1_CYCLES           2

//#define TIM1_INTERRUPT_NUMBER   40
//#define TIM2_INTERRUPT_NUMBER   41
//#define TIMER_INT_TIMEOUT       60

/*
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
*/
//static struct sp804_config c1, c2;

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
/*
static uint32_t check_timer_default_rw_val( uint32_t base_addr )
{
    rumboot_printf("Check the default values of the registers:");

    struct regpoker_checker check_default_array[] = {
          {   "Timer1Load",     REGPOKER_READ32,    DIT0_REG_LOAD0,          0x00,           0xff },
          {   "Timer1Load",     REGPOKER_WRITE32,   DIT0_REG_LOAD0,          0x00,           0xff },

          {   "Timer1Value",    REGPOKER_READ32,    DIT0_REG_VALUE0,         0xffffffff,      0xffffffff },


          {   "Timer1Control",  REGPOKER_READ32,    DIT0_REG_CONTROL0,       0xff,           0xff },
          {   "Timer1Control",  REGPOKER_WRITE32,   DIT0_REG_CONTROL0,       0xff,           0xff },

          {   "Timer1IntClr",   REGPOKER_READ32,    DIT0_REG_INTCLR0,        0xff,           0xff },

          {   "Timer1RIS",      REGPOKER_READ32,    DIT0_REG_RIS0,           0xff,           0xff },
          {   "Timer1MIS",      REGPOKER_READ32,    DIT0_REG_MIS0,           0xff,           0xff },


          {   "Timer1BGLoad",   REGPOKER_READ32,    DIT0_REG_BGLOAD0,        0x18,           0xff },
          {   "Timer1BGLoad",   REGPOKER_WRITE32,   DIT0_REG_BGLOAD0,        0x18,           0xff },

          {   "Timer2Load",     REGPOKER_READ32,    DIT0_REG_LOAD1,          0x00,           0xff },
          {   "Timer2Load",     REGPOKER_WRITE32,   DIT0_REG_LOAD1,          0x00,           0xff },

          {   "Timer2Value",    REGPOKER_READ32,    DIT0_REG_VALUE1,         0x00,           0xff },


          {   "Timer2Control",  REGPOKER_READ32,    DIT0_REG_CONTROL1,       0x0,            0xf },
          {   "Timer2Control",  REGPOKER_WRITE32,   DIT0_REG_CONTROL1,       0x0,            0xff },

          {   "Timer2RIS",      REGPOKER_READ32,    DIT0_REG_RIS1,           0x0,           0xff },
          {   "Timer2MIS",      REGPOKER_READ32,    DIT0_REG_MIS1,           0x0,           0xff },

          {   "Timer2BGLoad",   REGPOKER_READ32,    DIT0_REG_BGLOAD1,        0x0,           0xff },
          {   "Timer2BGLoad",   REGPOKER_WRITE32,   DIT0_REG_BGLOAD1,        0x0,           0xff },


          {   "TimerITCR",      REGPOKER_READ32,    DIT_REG_ITCR,            0xf0,           0xff },
          {   "TimerITOP",      REGPOKER_READ32,    DIT_REG_ITOP,            0xf04,          0xff },
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
*/
/*
static uint32_t check_timer_default_w_val( uint32_t base_addr )
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
*/


/*
struct s804_instance {
    int timer0_irq;
    int timer1_irq;
    uint32_t base_addr;
    int dit_index;
};

static void handler0( int irq, void *arg ) {
    struct s804_instance *a = ( struct s804_instance * )arg;
    a->timer0_irq = a->timer0_irq + 1;
    rumboot_printf( "IRQ 0 arrived  \n" );

    rumboot_printf( "sp804_%d timer 0 INT # %d  \n", a->dit_index, a->timer0_irq );
    sp804_clrint( a->base_addr, 0 );
}

static void handler1( int irq, void *arg ) {
    struct s804_instance *a = ( struct s804_instance * )arg;
    a->timer1_irq = a->timer1_irq + 1;
    rumboot_printf( "IRQ 1 arrived  \n" );
    rumboot_printf( "sp804_%d timer 1 INT # %d  \n", a->dit_index, a->timer1_irq );
    sp804_clrint( a->base_addr, 1 );
}

bool test_dit_timers( uint32_t structure ) {
    int c = 0;
    int d = 0;

    struct s804_instance *stru = ( struct s804_instance * )structure;
    uint32_t base_addr = stru->base_addr;

    struct sp804_conf config_0 = {
        .mode = ONESHOT,
        .interrupt_enable = 1,
        .clock_division = 1,
        .width = 32,
        .load = 100,
        .bgload = 0 };

    struct sp804_conf config_1 = {
        .mode = ONESHOT,
        .interrupt_enable = 1,
        .clock_division = 1,
        .width = 32,
        .load = 200,
        .bgload = 0 };

    for( int i = 0; i < TIMER0_CYCLES + stru->dit_index; i++ ) {
        sp804_config( base_addr, &config_0, 0 );
        sp804_enable( base_addr, 0 );
        while( sp804_get_value( base_addr, 0 ) ) {
        };
        c++;
    }regs

    for( int i = 0; i < TIMER1_CYCLES + stru->dit_index; i++ ) {
        sp804_config( base_addr, &config_1, 1 );
        sp804_enable( base_addr, 1 );
        while( sp804_get_value( base_addr, 1 ) ) {
        };
        d++;
    }

    if( stru->timer0_irq == TIMER0_CYCLES + stru->dit_index ) {
        rumboot_printf( "Timer 0 test OK \n" );
    } else {
        rumboot_printf( "ERROR in Timer 0 test \n" );
        rumboot_printf( "Interrupts came == %d, should be %d \n", stru->timer0_irq, TIMER0_CYCLES + stru->dit_index );
        return false;
    }

    if( stru->timer1_irq == TIMER1_CYCLES + stru->dit_index ) {
        rumboot_printf( "Timer 1 test OK \n" );
    } else {
        rumboot_printf( "ERROR in Timer 1 test \n" );
        rumboot_printf( "Interrupts came == %d, should be %d \n", stru->timer1_irq, TIMER1_CYCLES + stru->dit_index );
        return false;
    }

    return true;
}
*/
uint32_t main(void){
    /*
     * Test scenario:
     * - check APB connection
     * - setup TCR, wait TIMINT1,2
     * - setup timers, wait TIMINT1,2
     */
    /*
    struct sp804_conf c1 = {
        .mode = 0,
        .interrupt_enable = 1,
        .clock_division = 1,
        .width = 32,
        .load = 100,
        .bgload = 0 };

    struct sp804_conf c2 = {
        .mode = 0,
        .interrupt_enable = 1,
        .clock_division = 1,
        .width = 32,
        .load = 200,
        .bgload = 0 };
*/
/*
    c1.mode = 0;
    c1.interrupt_enable = 1;
    c1.clock_division = 1;
    c1.width = 32;
    c1.load = 100;
    c1.bgload = 0;

    c2.mode = 0;
    c2.interrupt_enable = 1;
    c2.clock_division = 1;
    c2.width = 32;
    c2.load = 200;
    c2.bgload = 0;
*/
    // Set up interrupt handlers
     uint32_t result;

        rumboot_printf( "SP804 test START\n" );

        rumboot_printf( "SP804 int are clean up\n" );
        result = check_timer_default_ro_val(DCR_TIMERS_BASE);// ||
//                check_timer_default_rw_val(DCR_TIMERS_BASE);
        if(!result)
        {
            rumboot_printf("Checked TEST_ERROR\n");
            return 1;
        }
        rumboot_printf("Checked TEST_OK\n");

      //  sp804_config(DCR_TIMERS_BASE, &c1, 0);
      //  sp804_config(DCR_TIMERS_BASE, &c2, 1);


        //sp804_clrint(DCR_TIMERS_BASE, 0);
        //sp804_clrint(DCR_TIMERS_BASE, 1);


        return result;

      //  struct rumboot_irq_entry *tbl = rumboot_irq_create( NULL );

}
