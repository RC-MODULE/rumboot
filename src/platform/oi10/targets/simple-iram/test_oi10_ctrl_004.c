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

//#define CHECK_REGS
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
    rumboot_printf("Check the default values of the registers:");

    struct regpoker_checker check_default_array[] = {
/*              name                 test_function         reg_offset             expected_val            mask;
    uint64_t (*readfunc)(uintptr_t base);
    void (*writefunc)(uint64_t value, uintptr_t addr);
};
*/
/*        {   "HSCB_ID",              REGPOKER_READ32,    HSCB_ID,                HSCB_ID_DFLT,               HSCB_ID_MASK                },
 * */
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
        return true;
    }

    rumboot_printf( "ERROR\n" );
    return false;
}

static bool check_timer_default_rw_val( uint32_t base_addr )
{
    rumboot_printf("Check the default values of the registers:");

    struct regpoker_checker check_default_array[] = {
//        {   "HSCB_TIMINGS",         REGPOKER_WRITE32,   HSCB_TIMINGS,           HSCB_TIMINGS_DFLT,          HSCB_TIMINGS_MASK           },
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
        return true;
    }

    rumboot_printf( "ERROR\n" );
    return false;
}
#endif

struct s804_instance
{
    int timer0_irq;
    int timer1_irq;
    uint32_t base_addr;
};

static void handler0(int irq, void *arg)
{
    struct s804_instance *a = (struct s804_instance *)arg;
    a->timer0_irq = a->timer0_irq + 1;
    rumboot_printf("IRQ 0 arrived\n");
    rumboot_printf("sp804 timer 0 INT # %d \n", a->timer0_irq);
    sp804_clrint(a->base_addr, 0);
}

static void handler1(int irq, void *arg)
{
    struct s804_instance *a = (struct s804_instance *)arg;
    a->timer1_irq = a->timer1_irq + 1;
    rumboot_printf("IRQ 1 arrived\n");
    rumboot_printf("sp804 timer 1 INT # %d \n", a->timer1_irq);
    sp804_clrint(a->base_addr, 1);
}
__attribute__((unused))
static bool test_dit_timers(uint32_t structure)
{
    int c = 0;
    int d = 0;
    struct s804_instance *stru = (struct s804_instance *)structure;
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
            rumboot_printf("Counted interrupts is: %d\n", stru->timer0_irq);
        }
        else
        {
            rumboot_printf("ERROR in Timer 0 test \n");
            rumboot_printf("Interrupts came == %d, should be %d \n", stru->timer0_irq, TIMER0_CYCLES);
            return false;
        }
        if(stru->timer1_irq == TIMER1_CYCLES)
        {
            rumboot_printf("Timer 1 test OK \n");
            rumboot_printf("Counted interrupts is: %d\n", stru->timer1_irq);
        }
        else
        {
            rumboot_printf("ERROR in Timer 1 test \n");
            rumboot_printf("Interrupts came == %d, should be %d \n", stru->timer1_irq, TIMER1_CYCLES);
            return false;
        }
    return true;
}

//TEST MODE
__attribute__((unused))
static bool test_dit_timers2( uint32_t structure)
{
    struct s804_instance *stru = (struct s804_instance *)structure;
    uint32_t base_addr = stru->base_addr;
    int i = 0;
    int c = 0;
    int d = 0;

    dcr_write(base_addr + DIT_REG_ITCR, 0b1);
    for(i = 0; i < TIMER0_CYCLES; i++)
    {
        rumboot_printf("Test mode enabled %d: OK\n", i);
        while(sp804_get_value(base_addr, 0))
        {};
        c++;
        if(dcr_read(base_addr + DIT_REG_ITCR))
        {
            rumboot_printf("Timer 0 DIT_REG_ITOP is %d\n", dcr_read(base_addr + DIT_REG_ITOP));
            rumboot_printf("Counted interrupts is: %d\n", stru->timer0_irq);
        }
        else
        {
            rumboot_printf("Timer 0 DIT_REG_ITOR %d\n", dcr_read(base_addr + DIT_REG_ITOP));
            return false;
        }
    }
    for(i = 0; i < TIMER1_CYCLES; i++)
    {
        rumboot_printf("Test mode enabled %d: OK\n", i);
        while(sp804_get_value(base_addr, 1))
        {};
        d++;
        if(dcr_read(base_addr + DIT_REG_ITCR))
        {
            rumboot_printf("Timer 1 DIT_REG_ITOP is %d\n", dcr_read(base_addr + DIT_REG_ITOP));
            rumboot_printf("Counted interrupts is: %d\n", stru->timer1_irq);
        }
        else
        {
            rumboot_printf("Timer 1 DIT_REG_ITOR %d\n", dcr_read(base_addr + DIT_REG_ITOP));
            return false;
        }
    }
    return true;
}

static struct s804_instance in[] =
{
    {
        .base_addr = DCR_TIMERS_BASE,
    },
};

TEST_SUITE_BEGIN(dit_testlist, "SP804 IRQ TEST")
TEST_ENTRY("SP804_0", test_dit_timers, DCR_TIMERS_BASE),
TEST_ENTRY("SP804_0", test_dit_timers2, DCR_TIMERS_BASE),
//#ifdef CHECK_REGS
#ifdef CHECK_REGS
TEST_ENTRY("SP804_0_initials_reg_check", check_timer_default_ro_val, DCR_TIMERS_BASE),
TEST_ENTRY("SP804_0_rw_reg_check", check_timer_default_rw_val, DCR_TIMERS_BASE),
#endif
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

