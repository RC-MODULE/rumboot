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

#define TIMER_CYCLES 5

#ifdef CHECK_REGS
static uint32_t check_watchdog_default_ro_val(uint32_t base_addr)
{
    rumboot_printf("Check the default values of the registers:");
    struct regpoker_checker check_default_array[] = {
          {   "WD_REG_PERIPHID0",   REGPOKER_READ_DCR,    WD_REG_PERIPHID0,         0x05,           0xff   },
          {   "WD_REG_PERIPHID1",   REGPOKER_READ_DCR,    WD_REG_PERIPHID1,         0x18,           0xff   },
          {   "WD_REG_PERIPHID2",   REGPOKER_READ_DCR,    WD_REG_PERIPHID2,         0x14,           0xff   },
          {   "WD_REG_PERIPHID3",   REGPOKER_READ_DCR,    WD_REG_PERIPHID3,         0x00,           0xff   },
          {   "WD_REG_PCELLID0",    REGPOKER_READ_DCR,    WD_REG_PCELLID0,          0x0D,           0xff   },
          {   "WD_REG_PCELLID1",    REGPOKER_READ_DCR,    WD_REG_PCELLID1,          0xF0,           0xff   },
          {   "WD_REG_PCELLID2",    REGPOKER_READ_DCR,    WD_REG_PCELLID2,          0x05,           0xff   },
          {   "WD_REG_PCELLID3",    REGPOKER_READ_DCR,    WD_REG_PCELLID3,          0xB1,           0xff   },
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

static uint32_t check_watchdog_default_rw_val( uint32_t base_addr, uint32_t reg_lock )
{
    rumboot_printf("Check the default values of the registers:");
    dcr_write(base_addr + reg_lock, 0x1ACCE551);
    struct regpoker_checker check_default_array[] = {
    {   "WdogLoad",      REGPOKER_READ_DCR,    WD_REG_LOAD,           0x00000000,     0xffffffff },
    {   "WdogLoad",      REGPOKER_WRITE_DCR,   WD_REG_LOAD,           0x00000000,     0xffffffff },

    {   "WdogValue",     REGPOKER_READ_DCR,    WD_REG_VALUE,          0x00000000,     0xffffffff },

    {   "WdogControl",   REGPOKER_READ_DCR,    WD_REG_CONTROL,        0b00,           0b11 },
    {   "WdogControl",   REGPOKER_WRITE_DCR,   WD_REG_CONTROL,        0b00,           0b11 },

    {   "WdogMIS",       REGPOKER_READ_DCR,    WD_REG_MIS,            0b0,            0b1 },

    {   "WdogLock",      REGPOKER_READ_DCR,    WD_REG_LOAD,           0x00000000, 0xffffffff },
    {   "WdogLock",      REGPOKER_WRITE_DCR,   WD_REG_LOAD,           0x00000000, 0xffffffff },

    {   "WdogITCR",      REGPOKER_READ_DCR,    WD_REG_ITCR,           0b0,           0b1 },

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

struct s805_instance {
    int wd_irq;
    uint32_t base_addr;
};

static void handler0(int irq, void *arg)
{
    struct s805_instance *a = (struct s805_instance *) arg;
    a->wd_irq = a->wd_irq;
    rumboot_printf("IRQ arrived  \n");
    rumboot_printf("sp805 watchdog INT # %d  \n", a->wd_irq);
    sp805_clrint( a->base_addr);
}

static bool wd_test( uint32_t structure )
{
    int c = 0;
    struct s805_instance *stru = (struct s805_instance *)structure;
    uint32_t base_addr = stru->base_addr;
    stru->wd_irq = 0;

    struct sp805_conf config_FREE_RUN =
    {
           .mode = FREERUN,
           .interrupt_enable = 1,
           .clock_division = 1,
           .width = 32,
           .load = 100,
    };
    dcr_write(base_addr + WD_REG_ITCR, 0b1);
    for(int i = 0; i < TIMER_CYCLES; i++)
    {
        rumboot_printf("Integration normal mode enabled %d: OK\n", i);
        sp805_config(base_addr, &config_FREE_RUN);
        sp805_enable(base_addr);
        while(sp805_get_value(base_addr))
        {};
        c++;
        stru->wd_irq++;
    }

    if(stru->wd_irq == TIMER_CYCLES)
    {
        rumboot_printf("Watchdog test OK \n");
        rumboot_printf("Counted interrupts is: %d\n", stru->wd_irq);
    }
    else
    {
        rumboot_printf("ERROR in Watchdog test \n");
        rumboot_printf("Interrupts came == %d, should be %d \n", stru->wd_irq, TIMER_CYCLES);
        return false;
    }
    return true;
}

static bool wd_test2(uint32_t structure)
{
    struct s805_instance *stru = ( struct s805_instance * )structure;
    uint32_t base_addr = stru->base_addr;
    int i,d; d = 0;
    stru->wd_irq = 0;

    dcr_write(base_addr + WD_REG_ITCR, 0b1);
    for(i = 0; i < TIMER_CYCLES; i++)
    {
        rumboot_printf("Integration test mode enabled %d: OK\n", i);
        while(sp805_get_value(base_addr))
        {};d++;
        stru->wd_irq++;
    }
    if(dcr_read(base_addr + WD_REG_ITCR))
    {

        rumboot_printf("Watchdog test OK \n");
        rumboot_printf("WD_REG_ITOP is: %d\n", dcr_read(base_addr + WD_REG_ITOP));
        rumboot_printf("Counted interrupts is: %d\n", stru->wd_irq);
    }
    else
    {
        rumboot_printf("ERROR in Watchdog test \n");
        rumboot_printf("WD_REG_ITOR is: %d\n", dcr_read(base_addr + WD_REG_ITOP));
        rumboot_printf("Interrupts came == %d, should be %d \n", stru->wd_irq, TIMER_CYCLES);
        return false;
    }
    return true;
}

static struct s805_instance in[] =
{
    {
        .base_addr = DCR_WATCHDOG_BASE,
    },
};

TEST_SUITE_BEGIN(wd_testlist, "SP805 IRQ TEST")
TEST_ENTRY("SP805_0", wd_test, (uint32_t) &in[0]),
TEST_ENTRY("SP805_0", wd_test2, (uint32_t) &in[0]),
#ifdef CHECK_REGS
TEST_ENTRY("SP805_0", check_default_ro_val, (uint32_t, &in[0]));
TEST_ENTRY("SP805_0", check_default_rw_val, (uint32_t, &in[0]));
#endif
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
    return result;
}
