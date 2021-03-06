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

#define TIMER0_CYCLES           10
#define TIMER1_CYCLES           15

uint32_t count_of_cycles[]  = {TIMER0_CYCLES, TIMER1_CYCLES};
#define CHECK_REGS
#ifdef CHECK_REGS

#define DIT0_REG_LOAD0_DFLT     0
#define DIT0_REG_VALUE0_DFLT    0xFFFFFFFF
#define DIT0_REG_CONTROL0_DFLT  0x20
#define DIT0_REG_RIS0_DFLT      0
#define DIT0_REG_MIS0_DFLT      0
#define DIT0_REG_BGLOAD0_DFLT   0

#define DIT0_REG_LOAD1_DFLT     0
#define DIT0_REG_VALUE1_DFLT    0xFFFFFFFF
#define DIT0_REG_CONTROL1_DFLT  0x20
#define DIT0_REG_RIS1_DFLT      0
#define DIT0_REG_MIS1_DFLT      0
#define DIT0_REG_BGLOAD1_DFLT   0

#define DIT_REG_ITCR_DFLT       0
#define DIT_REG_ITOP_DFLT       0
#define DIT_REG_PERIPHID0_DFLT  0x4
#define DIT_REG_PERIPHID1_DFLT  0x18
#define DIT_REG_PERIPHID2_DFLT  0x14
#define DIT_REG_PERIPHID3_DFLT  0
#define DIT_REG_PCELLID0_DFLT   0xD
#define DIT_REG_PCELLID1_DFLT   0xF0
#define DIT_REG_PCELLID2_DFLT   0x5
#define DIT_REG_PCELLID3_DFLT   0xB1

#define DIT0_REG_LOAD0_mask     0xFFFFFFFF
#define DIT0_REG_VALUE0_mask    0xFFFFFFFF
#define DIT0_REG_CONTROL0_mask  0xFF
#define DIT0_REG_RIS0_mask      1
#define DIT0_REG_MIS0_mask      1
#define DIT0_REG_BGLOAD0_mask   0xFFFFFFFF

#define DIT0_REG_LOAD1_mask     0xFFFFFFFF
#define DIT0_REG_VALUE1_mask    0xFFFFFFFF
#define DIT0_REG_CONTROL1_mask  0xFF
#define DIT0_REG_RIS1_mask      1
#define DIT0_REG_MIS1_mask      1
#define DIT0_REG_BGLOAD1_mask   0xFFFFFFFF

#define DIT_REG_ITCR_mask       1
#define DIT_REG_ITOP_mask       3
#define DIT_REG_PERIPHID0_mask  0xFF
#define DIT_REG_PERIPHID1_mask  0xFF
#define DIT_REG_PERIPHID2_mask  0xFF
#define DIT_REG_PERIPHID3_mask  0xFF
#define DIT_REG_PCELLID0_mask   0xFF
#define DIT_REG_PCELLID1_mask   0xFF
#define DIT_REG_PCELLID2_mask   0xFF
#define DIT_REG_PCELLID3_mask   0xFF

static bool check_timer_default_ro_val(uint32_t base_addr)
{
    bool result = false;
    rumboot_printf("Check the default values of the registers:");

    struct regpoker_checker check_default_array[] = {
          {   "DIT0_REG_LOAD0",     REGPOKER_READ_DCR,    DIT0_REG_LOAD0,           DIT0_REG_LOAD0_DFLT,      DIT0_REG_LOAD0_mask },
          {   "DIT0_REG_VALUE0",    REGPOKER_READ_DCR,    DIT0_REG_VALUE0,          DIT0_REG_VALUE0_DFLT,     DIT0_REG_VALUE0_mask },
          {   "DIT0_REG_CONTROL0",  REGPOKER_READ_DCR,    DIT0_REG_CONTROL0,        DIT0_REG_CONTROL0_DFLT,   DIT0_REG_CONTROL0_mask },
          {   "DIT0_REG_RIS0",      REGPOKER_READ_DCR,    DIT0_REG_RIS0,            DIT0_REG_RIS0_DFLT,       DIT0_REG_RIS0_mask },
          {   "DIT0_REG_MIS0",      REGPOKER_READ_DCR,    DIT0_REG_MIS0,            DIT0_REG_MIS0_DFLT,       DIT0_REG_MIS0_mask },
          {   "DIT0_REG_BGLOAD0",   REGPOKER_READ_DCR,    DIT0_REG_BGLOAD0,         DIT0_REG_BGLOAD0_DFLT,    DIT0_REG_BGLOAD0_mask },

          {   "DIT0_REG_LOAD1",     REGPOKER_READ_DCR,    DIT0_REG_LOAD1,           DIT0_REG_LOAD1_DFLT,      DIT0_REG_LOAD1_mask },
          {   "DIT0_REG_VALUE1",    REGPOKER_READ_DCR,    DIT0_REG_VALUE1,          DIT0_REG_VALUE1_DFLT,     DIT0_REG_VALUE1_mask },
          {   "DIT0_REG_CONTROL1",  REGPOKER_READ_DCR,    DIT0_REG_CONTROL1,        DIT0_REG_CONTROL1_DFLT,   DIT0_REG_CONTROL1_mask },
          {   "DIT0_REG_RIS1",      REGPOKER_READ_DCR,    DIT0_REG_RIS1,            DIT0_REG_RIS1_DFLT,       DIT0_REG_RIS1_mask },
          {   "DIT0_REG_MIS1",      REGPOKER_READ_DCR,    DIT0_REG_MIS1,            DIT0_REG_MIS1_DFLT,       DIT0_REG_MIS1_mask },
          {   "DIT0_REG_BGLOAD1",   REGPOKER_READ_DCR,    DIT0_REG_BGLOAD1,         DIT0_REG_BGLOAD1_DFLT,    DIT0_REG_BGLOAD1_mask },

          {   "DIT_REG_ITCR",       REGPOKER_READ_DCR,    DIT_REG_ITCR,             DIT_REG_ITCR_DFLT,        DIT_REG_ITCR_mask },
          {   "DIT_REG_ITOP",       REGPOKER_READ_DCR,    DIT_REG_ITOP,             DIT_REG_ITOP_DFLT,        DIT_REG_ITOP_mask },
          {   "DIT_REG_PERIPHID0",  REGPOKER_READ_DCR,    DIT_REG_PERIPHID0,        DIT_REG_PERIPHID0_DFLT,   DIT_REG_PERIPHID0_mask },
          {   "DIT_REG_PERIPHID1",  REGPOKER_READ_DCR,    DIT_REG_PERIPHID1,        DIT_REG_PERIPHID1_DFLT,   DIT_REG_PERIPHID1_mask },
          {   "DIT_REG_PERIPHID2",  REGPOKER_READ_DCR,    DIT_REG_PERIPHID2,        DIT_REG_PERIPHID2_DFLT,   DIT_REG_PERIPHID2_mask },
          {   "DIT_REG_PERIPHID3",  REGPOKER_READ_DCR,    DIT_REG_PERIPHID3,        DIT_REG_PERIPHID3_DFLT,   DIT_REG_PERIPHID3_mask },
          {   "DIT_REG_PCELLID0",   REGPOKER_READ_DCR,    DIT_REG_PCELLID0,         DIT_REG_PCELLID0_DFLT,    DIT_REG_PCELLID0_mask  },
          {   "DIT_REG_PCELLID1",   REGPOKER_READ_DCR,    DIT_REG_PCELLID1,         DIT_REG_PCELLID1_DFLT,    DIT_REG_PCELLID1_mask  },
          {   "DIT_REG_PCELLID2",   REGPOKER_READ_DCR,    DIT_REG_PCELLID2,         DIT_REG_PCELLID2_DFLT,    DIT_REG_PCELLID2_mask  },
          {   "DIT_REG_PCELLID3",   REGPOKER_READ_DCR,    DIT_REG_PCELLID3,         DIT_REG_PCELLID3_DFLT,    DIT_REG_PCELLID3_mask  },
          { }
      };

    if( rumboot_regpoker_check_array( check_default_array, base_addr ) == 0 )
    {
        rumboot_printf( "OK\n" );
        result = true;
    }
    else
    {
        rumboot_printf( "ERROR\n" );
        result = false;
    }
    return result;
}

static bool check_timer_default_rw_val( uint32_t base_addr )
{
    bool result = false;
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
        result = true;
    }
    else
    {
        rumboot_printf( "ERROR\n" );
        result = false;
    }
    return result;
}
#endif

struct s804_instance
{
    volatile int timer0_irq;
    volatile int timer1_irq;
    uint32_t base_addr;
};

#define TIMER_INT_TIMEOUT   0x80

static void handler0(int irq, void *arg)
{
    struct s804_instance *a = (struct s804_instance *)arg;
    (a->timer0_irq)++;
    rumboot_printf("IRQ 0 arrived\n");
    rumboot_printf("sp804 timer 0 INT # %d \n", a->timer0_irq);
    if(sp804_get_itcr(a->base_addr))
        sp804_write_to_itop(a->base_addr, 0);
    else
        sp804_clrint(a->base_addr, 0);
}

static void handler1(int irq, void *arg)
{
    struct s804_instance *a = (struct s804_instance *)arg;
    (a->timer1_irq)++;
    rumboot_printf("IRQ 1 arrived\n");
    rumboot_printf("sp804 timer 1 INT # %d \n", a->timer1_irq);
    if(sp804_get_itcr(a->base_addr))
        sp804_write_to_itop(a->base_addr, 0);
    else
        sp804_clrint(a->base_addr, 1);
}


static bool test_dit_timers(uint32_t structure)
{
    int c = 0;
    int d = 0;
    struct s804_instance *stru = (struct s804_instance *)structure;
    uint32_t base_addr = stru->base_addr;
    bool result = true;
    stru->timer0_irq = 0;
    stru->timer1_irq = 0;

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

        for(int i = 0; i < TIMER0_CYCLES; i++)
        {
            sp804_config(base_addr, &config_0, 0);
            sp804_enable(base_addr, 0);
            while(sp804_get_value(base_addr, 0))
            {
            };
            c++;
        }
        for(int i = 0; i < TIMER1_CYCLES; i++)
        {
            sp804_config(base_addr, &config_1, 1);
            sp804_enable(base_addr, 1);
            while(sp804_get_value(base_addr, 1))
            {
            };
            d++;
        }
        if(stru->timer0_irq == TIMER0_CYCLES)
        {
            rumboot_printf("Timer 0 test OK \n");
            rumboot_printf("Count of interrupts is: %d\n", stru->timer0_irq);
        }
        else
        {
            rumboot_printf("ERROR in Timer 0 test \n");
            rumboot_printf("Interrupts came == %d, should be %d \n", stru->timer0_irq, TIMER0_CYCLES);
            result = false;
        }
        if(stru->timer1_irq == TIMER1_CYCLES)
        {
            rumboot_printf("Timer 1 test OK \n");
            rumboot_printf("Count of interrupts is: %d\n", stru->timer1_irq);
        }
        else
        {
            rumboot_printf("ERROR in Timer 1 test \n");
            rumboot_printf("Interrupts came == %d, should be %d \n", stru->timer1_irq, TIMER1_CYCLES);
            result = false;
        }
    return result;
}

//TEST MODE
static bool test_dit_timers2( uint32_t structure)
{
    struct s804_instance *stru = (struct s804_instance *)structure;
    uint32_t base_addr = stru->base_addr;
    int i = 0;
    uint32_t t;
    bool result = true;
    stru->timer0_irq = 0;
    stru->timer1_irq = 0;

    sp804_write_to_itcr(base_addr, 0b1);
    rumboot_printf("Test mode enabled\n", i);

    for(i = 0; i < TIMER0_CYCLES; i++)
    {
        sp804_write_to_itop(base_addr, 1);
        t = 0;
        while((i == stru->timer0_irq) && (t<TIMER_INT_TIMEOUT))
        {
            t++;
        }
        if((i + 1) != stru->timer0_irq)
        {
            result = false;
            rumboot_printf("TIMER0 did not produce test event #%d, timer0_irq == %d, t == 0x%x\n", i, stru->timer0_irq, t);
            break;
        }
    }


    if(stru->timer0_irq == TIMER0_CYCLES)
        rumboot_printf("TIMER0 test mode: OK\n");
    else
    {
        result = false;
        rumboot_printf("TIMER0 test mode: FAIL\nexpected == %d, counted == %d\n",TIMER0_CYCLES,stru->timer0_irq);
    }

    for(i = 0; i < TIMER1_CYCLES; i++)
    {
        sp804_write_to_itop(base_addr, 2);
        t = 0;
        while((i == stru->timer1_irq) && (t<TIMER_INT_TIMEOUT))
        {
            t++;
        }
        if((i + 1) != stru->timer1_irq)
        {
            result = false;
            rumboot_printf("TIMER1 did not produce test event #%d, timer1_irq == %d, t == 0x%x\n", i, stru->timer1_irq, t);
            break;
        }
    }

    if(stru->timer1_irq == TIMER1_CYCLES)
        rumboot_printf("TIMER1 test mode: OK\n");
    else
    {
        result = false;
        rumboot_printf("TIMER1 test mode: FAIL\nexpected == %d, counted == %d\n",TIMER1_CYCLES,stru->timer1_irq);
    }
    sp804_write_to_itcr(base_addr, 0b0);

    return result;
}

static struct s804_instance in[] =
{
    {
        .base_addr = DCR_TIMERS_BASE,
    },
};

TEST_SUITE_BEGIN(dit_testlist, "SP804 IRQ TEST")
#ifdef CHECK_REGS
TEST_ENTRY("SP804_0_initials_reg_check", check_timer_default_ro_val, DCR_TIMERS_BASE),
TEST_ENTRY("SP804_0_rw_reg_check", check_timer_default_rw_val, DCR_TIMERS_BASE),
#endif
TEST_ENTRY("SP804_0 test mode", test_dit_timers2, (uint32_t) &in[0]),
TEST_ENTRY("SP804_0", test_dit_timers, (uint32_t) &in[0]),
TEST_SUITE_END();

int main(void)
{
    register int result;
    rumboot_printf("SP804 test START\n");
    rumboot_irq_cli();
    struct rumboot_irq_entry *tbl = rumboot_irq_create(NULL);
    rumboot_irq_set_handler(tbl, DIT_INT0, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler0, &in[0]);
    rumboot_irq_set_handler(tbl, DIT_INT1, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler1, &in[0]);
    /* Activate the table */
    rumboot_irq_table_activate(tbl);
    rumboot_irq_enable(DIT_INT0);
    rumboot_irq_enable(DIT_INT1);
    rumboot_irq_sei();
    result = test_suite_run(NULL, &dit_testlist);
    rumboot_irq_table_activate(NULL);
    rumboot_irq_free(tbl);
    return result;
}

