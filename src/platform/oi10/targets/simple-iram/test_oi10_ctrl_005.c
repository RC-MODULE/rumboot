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

#define TIMER_CYCLES 60

struct s805_instance
{
    uint32_t base_addr;
    int wd_index;
};

struct s805_instance_i {
    int wd_irq;
    uint32_t base_addr;
    int wd_index;
} s805_i;

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

static void handler0( int irq, void *arg )
{
    struct s805_instance_i *a = (struct s805_instance_i *) arg;
    a->wd_irq = a->wd_irq + 1;
    rumboot_printf( "IRQ 0 arrived  \n" );
    rumboot_printf( "sp805_%d watchdog INT # %d  \n", a->wd_index, a->wd_irq );
    sp805_clrint( a->base_addr);
}

static bool wd_test( uint32_t structure)
{
    struct s805_instance *stru = ( struct s805_instance * )structure;
    uint32_t base_addr = stru->base_addr;
    int i;
    struct sp805_conf config_FREE_RUN =
    {
           .mode = FREERUN,
           .interrupt_enable = 1,
           .clock_division = 1,
           .width = 32,
           .load = 100,
    };
    dcr_write(base_addr + WD_REG_LOCK, 0x1ACCE551);
    for(i = 0; i < TIMER_CYCLES + stru->wd_index; i++)
    {
        sp805_config( base_addr, &config_FREE_RUN);
        sp805_enable( base_addr );

        if(stru->wd_index == WD_CTRL_INTEN)
        {
            rumboot_printf("Watchdog load value %d\n", stru->wd_index);
        }
        sp805_enable(base_addr);
    }
    return true;
}

static bool wd_test2( uint32_t structure)
{
    struct s805_instance *stru = ( struct s805_instance * )structure;
    uint32_t base_addr = stru->base_addr;
    int i;

    dcr_write(base_addr + WD_REG_LOCK, 0x1ACCE551);
    for(i = 0; i < TIMER_CYCLES + stru->wd_index; i++)
    {
        if(stru->wd_index == WD_REG_ITCR)
        {
             rumboot_printf("Watchdog WD_REG_ITCR %d\n", stru->wd_index);
        }
        else
        {
            if(stru->wd_index == WD_REG_ITOP)
            {
                rumboot_printf("Watchdog WD_REG_ITOR %d\n", stru->wd_index);
            }
        }
    }
    i = dcr_read(base_addr + WD_REG_ITCR);
    rumboot_printf("WD_REG_ITCR %d\n", i);
    sp805_enable(stru->base_addr);
    return true;
}

static struct s805_instance in[] =
{
    {
        .base_addr = DCR_WATCHDOG_BASE,
        .wd_index = 0
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
    // Set up interrupt handlers
    register uint32_t result;
    rumboot_printf( "SP805 test START\n" );
    struct rumboot_irq_entry *tbl = rumboot_irq_create( NULL );
    rumboot_irq_cli();
    rumboot_irq_set_handler( tbl, WDT_INT, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler0, &in[0]);
    /* Activate the table */
    rumboot_irq_table_activate( tbl );
    rumboot_irq_enable( WDT_INT);
    rumboot_irq_sei();

    result = test_suite_run(NULL, &wd_testlist);
    return (!result) ? rumboot_printf("CHECKED TEST_OK\n"), 1: rumboot_printf("CHECKED TEST_ERROR\n"), 0;
}
