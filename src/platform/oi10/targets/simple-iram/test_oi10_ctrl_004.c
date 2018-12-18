/*
 * test_oi10_ctrl_004.c
 *
 *  Created on: Aug 24, 2018
 *      Author: m.dubinkin
 */

#include <stdio.h>
#include <stdint.h>
#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <rumboot/printf.h>
#include <rumboot/regpoker.h>
#include <platform/devices.h>
#include <devices/sp804.h>
#include <platform/interrupts.h>
#include <rumboot/testsuite.h>
#include <regs/regs_sp804.h>
#include <rumboot/platform.h>

#define TIMER0_CYCLES           1
#define TIMER1_CYCLES           2
#define TIMER_CYCLES            60

#ifdef CHECK_REGS
static uint32_t check_timer_default_ro_val(uint32_t base_addr)
{
    rumboot_printf("Check the default values of the registers:");

    struct regpoker_checker check_default_array[] = {
          {   "DIT_REG_PERIPHID0",   REGPOKER_READ_DCR,    DIT_REG_PERIPHID0,         0x04,           0xff   },
          {   "DIT_REG_PERIPHID1",   REGPOKER_READ_DCR,    DIT_REG_PERIPHID1,         0x18,           0xff   },
          {   "DIT_REG_PERIPHID2",   REGPOKER_READ_DCR,    DIT_REG_PERIPHID2,         0x14,           0xff   },
          {   "DIT_REG_PERIPHID3",   REGPOKER_READ_DCR,    DIT_REG_PERIPHID3,         0x00,           0xff   },
          {   "DIT_REG_PCELLID0",    REGPOKER_READ_DCR,    DIT_REG_PCELLID0,          0x0D,           0xff   },
          {   "DIT_REG_PCELLID1",    REGPOKER_READ_DCR,    DIT_REG_PCELLID1,          0xF0,           0xff   },
          {   "DIT_REG_PCELLID2",    REGPOKER_READ_DCR,    DIT_REG_PCELLID2,          0x05,           0xff   },
          {   "DIT_REG_PCELLID3",    REGPOKER_READ_DCR,    DIT_REG_PCELLID3,          0xB1,           0xff   },
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

static uint32_t check_timer_default_rw_val( uint32_t base_addr )
{
    rumboot_printf("Check the default values of the registers:");

    struct regpoker_checker check_default_array[] = {
          {   "Timer1Load",     REGPOKER_READ_DCR,  DIT0_REG_LOAD0,          0x00000000, 0xffffffff },
          {   "Timer1Load",     REGPOKER_WRITE_DCR, DIT0_REG_LOAD0,          0x00000000, 0xffffffff },

          {   "Timer1Value",    REGPOKER_READ_DCR,  DIT0_REG_VALUE0,         0x00000000, 0xffffffff },

          {   "Timer1RIS",      REGPOKER_READ_DCR,  DIT0_REG_RIS0,           0b0,           0b1 },
          {   "Timer1MIS",      REGPOKER_READ_DCR,  DIT0_REG_MIS0,           0b0,           0b1 },

          {   "Timer1BGLoad",   REGPOKER_READ_DCR,    DIT0_REG_BGLOAD0,      0x00000000,    0xffffffff },
          {   "Timer1BGLoad",   REGPOKER_WRITE_DCR,   DIT0_REG_BGLOAD0,      0x00000000,    0xffffffff },

          {   "Timer2Load",     REGPOKER_WRITE_DCR,   DIT0_REG_LOAD1,        0x00000000,    0xffffffff },

          {   "Timer2Value",    REGPOKER_READ_DCR,    DIT0_REG_VALUE1,       0x00000000,    0x00000000 },

          {   "Timer2RIS",      REGPOKER_READ_DCR,    DIT0_REG_RIS1,           0b0,           0b1 },
          {   "Timer2MIS",      REGPOKER_READ_DCR,    DIT0_REG_MIS1,           0b0,           0b1 },

          {   "Timer2BGLoad",   REGPOKER_READ_DCR,    DIT0_REG_BGLOAD1,        0x0,           0xff },
          {   "Timer2BGLoad",   REGPOKER_WRITE_DCR,   DIT0_REG_BGLOAD1,        0x0,           0xff },

          {   "TimerITCR",      REGPOKER_READ_DCR,    DIT_REG_ITCR,            0b0,           0b1 },
          {   "TimerITCR",      REGPOKER_WRITE_DCR,   DIT_REG_ITCR,            0b0,           0b1 },

          {   "TimerITOP",      REGPOKER_READ_DCR,    DIT_REG_ITOP,            0b00,          0b11 },

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
#endif

struct s804_instance
{
    int timer0_irq;
    int timer1_irq;
    uint32_t base_addr;
    int dit_index;
};

static void handler0( int irq, void *arg )
{
    struct s804_instance *a = ( struct s804_instance * )arg;
    a->timer0_irq = a->timer0_irq + 1;
    rumboot_printf("IRQ 0 arrived\n");
    rumboot_printf("sp804_%d timer 0 INT # %d  \n", a->dit_index, a->timer0_irq);
    sp804_clrint( a->base_addr, 0);
}

static void handler1( int irq, void *arg )
{
    struct s804_instance *a = ( struct s804_instance * )arg;
    a->timer1_irq = a->timer1_irq + 1;
    rumboot_printf("IRQ 1 arrived\n");
    rumboot_printf("sp804_%d timer 1 INT # %d  \n", a->dit_index, a->timer1_irq);
    sp804_clrint( a->base_addr, 1);
}

bool test_dit_timers( uint32_t structure )
{
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

    for( int i = 0; i < TIMER0_CYCLES + stru->dit_index; i++ )
    {
        sp804_config(base_addr, &config_0, 0);
        sp804_enable(base_addr, 0);
        while(sp804_get_value(base_addr, 0))
        {
        };
        c++;
    }

    for( int i = 0; i < TIMER1_CYCLES + stru->dit_index; i++ ) {
        sp804_config( base_addr, &config_1, 1 );
        sp804_enable( base_addr, 1 );
        while( sp804_get_value( base_addr, 1 ) ) {
        };
        d++;
    }

    if( stru->timer0_irq == TIMER0_CYCLES + stru->dit_index )
    {
        rumboot_printf( "Timer 0 test OK \n" );
    }
    else
    {
        rumboot_printf( "ERROR in Timer 0 test \n" );
        rumboot_printf( "Interrupts came == %d, should be %d \n", stru->timer0_irq, TIMER0_CYCLES + stru->dit_index );
        return false;
    }
    if( stru->timer1_irq == TIMER1_CYCLES + stru->dit_index )
    {
        rumboot_printf( "Timer 1 test OK \n" );
    }
    else
    {
        rumboot_printf( "ERROR in Timer 1 test \n" );
        rumboot_printf( "Interrupts came == %d, should be %d \n", stru->timer1_irq, TIMER1_CYCLES + stru->dit_index );
        return false;
    }
    return true;
}

static bool test_dit_timers2( uint32_t structure)
{
    struct s804_instance *stru = ( struct s804_instance * )structure;
    uint32_t base_addr = stru->base_addr;
    int i;
    for(i = 0; i < TIMER_CYCLES + stru->dit_index; i++)
    {
        if(stru->dit_index == DIT_REG_ITCR)
        {
            rumboot_printf("Timers DIT_REG_ITCR %d\n", stru->dit_index);
        }
        else
        {
            if(stru->dit_index == DIT_REG_ITOP)
            {
                rumboot_printf("Timers DIT_REG_ITOR %d\n", stru->dit_index);
            }
        }
    }
    i = dcr_read(base_addr + DIT_REG_ITCR);
    rumboot_printf("DIT_REG_ITCR %d\n", i);
    sp804_enable(stru->base_addr, 0);
    return true;
}

static struct s804_instance in[] =
{
    {
        .base_addr = DCR_TIMERS_BASE,
        .dit_index = 0,
    },
};

TEST_SUITE_BEGIN(dit_testlist, "SP804 IRQ TEST")
TEST_ENTRY("SP804_0", test_dit_timers, (uint32_t) &in[0]),
TEST_ENTRY("SP804_0", test_dit_timers2, (uint32_t) &in[0]),
#ifdef CHECK_REGS
TEST_ENTRY("SP804_0", check_default_ro_val, (uint32_t, &in[0]));
TEST_ENTRY("SP804_0", check_default_rw_val, (uint32_t, &in[0]));
#endif
TEST_SUITE_END();

int main(void)
{
    register int result;
    rumboot_printf( "SP804 test START\n" );
    rumboot_irq_cli();
    struct rumboot_irq_entry *tbl = rumboot_irq_create( NULL );
    rumboot_irq_set_handler( tbl, DIT_INT0, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler0, &in[0]);
    rumboot_irq_set_handler( tbl, DIT_INT1, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler1, &in[0]);
    /* Activate the table */
    rumboot_irq_table_activate( tbl );
    rumboot_irq_enable( DIT_INT0 );
    rumboot_irq_enable( DIT_INT1 );
    rumboot_irq_sei();

    result = test_suite_run( NULL, &dit_testlist );
    if(!result)
    {
        rumboot_printf("Checked TEST_OK\n");
        return result;
    }
    rumboot_printf("Checked TEST_ERROR\n");
    return result;
}

