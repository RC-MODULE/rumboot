/*
 * test_oi10_ctrl_005.c
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
#include <devices/sp805.h>
#include <platform/interrupts.h>
#include <rumboot/testsuite.h>
#include <regs/regs_sp805.h>
#include <rumboot/platform.h>

#define TIMER_CYCLES 10

#define WD_INT_TIMEOUT   0x80

#define CHECK_REGS
#ifdef CHECK_REGS
#define WD_REG_LOAD_mask        0xffffffff
#define WD_REG_VALUE_mask       0xffffffff
#define WD_REG_CONTROL_mask     0x3
#define WD_REG_RIS_mask         0x1
#define WD_REG_MIS_mask         0x1
#define WD_REG_LOCK_mask        0xffffffff
#define WD_REG_ITCR_mask        0x1
#define WD_REG_PERIPHID0_mask   0xff
#define WD_REG_PERIPHID1_mask   0xff
#define WD_REG_PERIPHID2_mask   0xff
#define WD_REG_PERIPHID3_mask   0xff
#define WD_REG_PCELLID0_mask    0xff
#define WD_REG_PCELLID1_mask    0xff
#define WD_REG_PCELLID2_mask    0xff
#define WD_REG_PCELLID3_mask    0xff

#define WD_REG_LOAD_DFLT        0xffffffff
#define WD_REG_VALUE_DFLT       0xffffffff
#define WD_REG_CONTROL_DFLT     0
#define WD_REG_RIS_DFLT         0
#define WD_REG_MIS_DFLT         0
#define WD_REG_LOCK_DFLT        0x1ACCE551
#define WD_REG_ITCR_DFLT        0
#define WD_REG_PERIPHID0_DFLT   0x5
#define WD_REG_PERIPHID1_DFLT   0x18
#define WD_REG_PERIPHID2_DFLT   0x14
#define WD_REG_PERIPHID3_DFLT   0
#define WD_REG_PCELLID0_DFLT    0xD
#define WD_REG_PCELLID1_DFLT    0xF0
#define WD_REG_PCELLID2_DFLT    0x5
#define WD_REG_PCELLID3_DFLT    0xB1

static bool check_watchdog_default_ro_val(uint32_t base_addr)
{
    bool result = false;
    rumboot_printf("Check the default values of the registers:");
    struct regpoker_checker check_default_array[] = {
        {   "WD_REG_PERIPHID0",   REGPOKER_READ_DCR,    WD_REG_PERIPHID0,         WD_REG_PERIPHID0_DFLT,           WD_REG_PERIPHID0_mask   },
        {   "WD_REG_PERIPHID1",   REGPOKER_READ_DCR,    WD_REG_PERIPHID1,         WD_REG_PERIPHID1_DFLT,           WD_REG_PERIPHID1_mask   },
        {   "WD_REG_PERIPHID2",   REGPOKER_READ_DCR,    WD_REG_PERIPHID2,         WD_REG_PERIPHID2_DFLT,           WD_REG_PERIPHID2_mask   },
        {   "WD_REG_PERIPHID3",   REGPOKER_READ_DCR,    WD_REG_PERIPHID3,         WD_REG_PERIPHID3_DFLT,           WD_REG_PERIPHID3_mask   },
        {   "WD_REG_PCELLID0",    REGPOKER_READ_DCR,    WD_REG_PCELLID0,          WD_REG_PCELLID0_DFLT ,           WD_REG_PCELLID0_mask    },
        {   "WD_REG_PCELLID1",    REGPOKER_READ_DCR,    WD_REG_PCELLID1,          WD_REG_PCELLID1_DFLT ,           WD_REG_PCELLID1_mask    },
        {   "WD_REG_PCELLID2",    REGPOKER_READ_DCR,    WD_REG_PCELLID2,          WD_REG_PCELLID2_DFLT ,           WD_REG_PCELLID2_mask    },
        {   "WD_REG_PCELLID3",    REGPOKER_READ_DCR,    WD_REG_PCELLID3,          WD_REG_PCELLID3_DFLT ,           WD_REG_PCELLID3_mask    },
        {   "WdogLock",           REGPOKER_READ_DCR,    WD_REG_LOCK,              0                    ,           0                       },
        {   "WdogLoad",           REGPOKER_READ_DCR,    WD_REG_LOAD,              WD_REG_LOAD_DFLT,                WD_REG_LOAD_mask    },
        {   "WdogControl",        REGPOKER_READ_DCR,    WD_REG_CONTROL,           WD_REG_CONTROL_DFLT,             WD_REG_CONTROL_mask },
        {   "WdogValue",          REGPOKER_READ_DCR,    WD_REG_VALUE,             WD_REG_VALUE_DFLT,               WD_REG_VALUE_mask   },
        {   "WdogRIS",            REGPOKER_READ_DCR,    WD_REG_RIS,               WD_REG_RIS_DFLT,                 WD_REG_RIS_mask     },
        {   "WdogMIS",            REGPOKER_READ_DCR,    WD_REG_MIS,               WD_REG_MIS_DFLT,                 WD_REG_MIS_mask     },
        {   "WdogITCR",           REGPOKER_READ_DCR,    WD_REG_ITCR,              WD_REG_ITCR_DFLT,                WD_REG_ITCR_mask    },
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

static bool check_watchdog_default_rw_val( uint32_t base_addr)//, uint32_t reg_lock )
{
    bool result = false;
    rumboot_printf("Check the default values of the registers:");
    struct regpoker_checker check_default_array[] = {
    {   "WdogLoad",      REGPOKER_WRITE_DCR,   WD_REG_LOAD,           WD_REG_LOAD_DFLT,     WD_REG_LOAD_mask    },

    {   "WdogControl",   REGPOKER_WRITE_DCR,   WD_REG_CONTROL,        WD_REG_CONTROL_DFLT,  WD_REG_CONTROL_mask },

    {   "WdogITCR",      REGPOKER_WRITE_DCR,   WD_REG_ITCR,           WD_REG_ITCR_DFLT,     WD_REG_ITCR_mask    },
    {   "WdogLock",      REGPOKER_WRITE_DCR,   WD_REG_LOCK,                          0,     0                   },
    {   "WdogLock",      REGPOKER_READ_DCR,    WD_REG_LOCK,                          1,     1                   },

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

struct s805_instance {
    volatile int wd_irq;
    uint32_t base_addr;
};


static void handler0(int irq, void *arg)
{
    struct s805_instance *a = (struct s805_instance *) arg;
    sp805_unlock_access(a->base_addr);
    (a->wd_irq)++;
    rumboot_printf("IRQ arrived\n");
    rumboot_printf("sp805 watchdog INT # %d  \n", a->wd_irq);
    if(sp805_get_itcr(a->base_addr))//if integrated test control is enabled
    {
        rumboot_printf("handler0: itcr\n");
        sp805_write_to_itop(a->base_addr,0);
    }
    else{
        rumboot_printf("handler0: freerun\n");
        sp805_disable(a->base_addr);
        sp805_clrint(a->base_addr);
    }
}

static bool wd_test( uint32_t structure )
{
    struct s805_instance *stru = (struct s805_instance *)structure;
    uint32_t base_addr = stru->base_addr;
    uint32_t t;
    bool result = true;
    struct sp805_conf config_FREE_RUN =
    {
           .mode = FREERUN,
           .interrupt_enable = 1,
           .clock_division = 1,
           .width = 32,
           .load = 100,
    };

    sp805_unlock_access(base_addr);
    sp805_write_to_itcr(base_addr, 0b0); //set up normal mode
    rumboot_printf("Integration normal mode enabled: OK\n");
    for(int i = 0; i < TIMER_CYCLES; i++)
    {
        sp805_config(base_addr, &config_FREE_RUN);
        sp805_enable(base_addr);
        t = 0;
        while((stru->wd_irq == i) && (t<WD_INT_TIMEOUT)){t++;}

    }
    if(stru->wd_irq == TIMER_CYCLES)
    {
        rumboot_printf("Watchdog test OK\n");
        rumboot_printf("Counted interrupts is: %d\n", stru->wd_irq);
    }
    else
    {
        rumboot_printf("ERROR in Watchdog test\n");
        rumboot_printf("Interrupts came == %d, should be %d \n", stru->wd_irq, TIMER_CYCLES);
        result = false;
    }
    return result;
}

//TEST MODE
static bool wd_test2(uint32_t structure)
{
    struct s805_instance *stru = (struct s805_instance *)structure;
    uint32_t base_addr = stru->base_addr;
    stru->wd_irq = 0;
    int i, t;
    bool result = true;
    sp805_unlock_access(base_addr);
    sp805_write_to_itcr(base_addr, 0b1);
    if(sp805_get_itcr(base_addr))
    {
        rumboot_printf("Integration test mode enabled: OK\n");
    }
    for(i = 0; i < TIMER_CYCLES; i++)
    {
            sp805_write_to_itop(base_addr,2);
            t = 0;
            while((stru->wd_irq == i) && (t<WD_INT_TIMEOUT)){t++;}
    }
    if(stru->wd_irq == TIMER_CYCLES)
    {
        rumboot_printf("Watchdog test OK\n");
        rumboot_printf("WD_REG_ITCR is: %d\n", sp805_get_itcr(base_addr));
        rumboot_printf("Counted interrupts is: %d\n", stru->wd_irq);
    }
    else
    {
        rumboot_printf("ERROR in Watchdog test \n");
        rumboot_printf("Interrupts came == %d, should be %d \n", stru->wd_irq, TIMER_CYCLES);
        result = false;
    }
    return result;
}

static struct s805_instance in[] =
{
    {
        .base_addr = DCR_WATCHDOG_BASE,
    },
};

TEST_SUITE_BEGIN(wd_testlist, "SP805 IRQ TEST")
#ifdef CHECK_REGS
TEST_ENTRY("SP805_0 reg read default", check_watchdog_default_ro_val, DCR_WATCHDOG_BASE),
TEST_ENTRY("SP805_0 reg rw check", check_watchdog_default_rw_val, DCR_WATCHDOG_BASE),
#endif
TEST_ENTRY("SP805_0 real mode", wd_test, (uint32_t) &in[0]),
TEST_ENTRY("SP805_0 test mode", wd_test2, (uint32_t) &in[0]),
TEST_SUITE_END();

uint32_t main(void)
{
    register int result;
    rumboot_printf("SP805 test START\n");
    struct rumboot_irq_entry *tbl = rumboot_irq_create(NULL);
    rumboot_irq_cli();
    rumboot_irq_set_handler(tbl, WDT_INT, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler0, &in[0]);
    /* Activate the table */
    rumboot_irq_table_activate(tbl);
    /* Activate Watchdog Interrupt */
    rumboot_irq_enable(WDT_INT);
    rumboot_irq_sei();
    result = test_suite_run(NULL, &wd_testlist);
    rumboot_irq_table_activate(NULL);
    rumboot_irq_free(tbl);
    return result;
}
