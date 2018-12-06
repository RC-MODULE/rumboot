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
#include <platform/arch/ppc/ppc_476fp_lib_c.h>
#include <platform/common_macros/common_macros.h>
#include <rumboot/regpoker.h>
#include <arch/irq_macros.h>
#include <platform/devices.h>
#include <rumboot/macros.h>
#include <platform/arch/ppc/test_macro.h>
#include <devices/sp805.h>
#include <platform/interrupts.h>
#include <rumboot/testsuite.h>
#include <platform/regs/regs_mpic128.h>
#include <regs/regs_sp805.h>
#include <rumboot/testsuite.h>
#include <platform/devices.h>
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

    {   "WdogITCR",     REGPOKER_READ_DCR,    WD_REG_ITCR,            0b0,           0b1 },

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
/*
void sp805_enable( uint32_t base_addr )
{
    int cntrl;
    int control_reg;
    //if( index )
   // {
        control_reg = WD_CTRL_RESEN;
   // }
    cntrl = dcr_read( base_addr + control_reg );
    cntrl |= WD_CTRL_RESEN;

    cntrl = dcr_read( base_addr + control_reg );
    dcr_write( base_addr + control_reg, cntrl);
}

void sp805_stop( uint32_t base_addr)
{
    int cntrl;
    int control_reg;
    //if( index )
    //{
        control_reg = WD_REG_CONTROL;
   // }
    cntrl = dcr_read( base_addr + control_reg );
    //
    cntrl = cntrl & ( ~( WD_CTRL_RESEN ) );
    dcr_write( base_addr + control_reg, cntrl );
}

int sp805_get_value( uint32_t base_addr)
{
    int value_reg;
    //if( index )
    //{
        value_reg = WD_REG_VALUE;
   // }
    return dcr_read( base_addr + value_reg );
}

void sp805_clrint( uint32_t base_addr)
{
    int int_clr_reg;
    //if( index )
    //{
        int_clr_reg = WD_REG_INTCLR;
   // }
    dcr_write( base_addr + int_clr_reg, 1 );
}

void sp805_config( uint32_t base_addr, const struct sp805_conf * config)
{
    int cntrl = 0;
    // INT EN
    if( config->interrupt_enable )
    {
        cntrl |= WD_CTRL_INTEN;
    }
    else
    {
        cntrl &= ~WD_CTRL_INTEN;
    }
    // CLK DIV
    if( config->clock_division == 256 )
    {
        cntrl |= WD_CTRL_DIV1;
        cntrl &= ~WD_CTRL_DIV0;
    }
    else if( config->clock_division == 16 )
    {
        cntrl &= ~WD_CTRL_DIV1;
        cntrl |= WD_CTRL_DIV0;
    } else {
        cntrl &= ~WD_CTRL_DIV1;
        cntrl &= ~WD_CTRL_DIV0;
    }

    // SIZE 32
    if( config->width == 32 )
    {
        cntrl |= WD_CTRL_SIZE32;
    }
    else
    {
        cntrl &= ~WD_CTRL_SIZE32;
    }
    ///enable/disable watchdog
    //if( index ) //if RESEN == 1
    //{
        dcr_write( base_addr + WD_REG_CONTROL, cntrl );
        // LOAD
        if( config->load )
        {
            dcr_write( base_addr + WD_REG_LOAD, config->load );
        }
        // BG LOAD
    //}
        /*
    else
    {
        dcr_write( base_addr + WD_REG_CONTROL, cntrl );
        // LOAD
        if( config->load )
        {
            dcr_write( base_addr + WD_REG_LOAD, config->load );
        }
    }

}
*/
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
    dcr_write(DCR_WATCHDOG_BASE + WD_REG_LOCK, 0x1ACCE551);

    for(i = 0; i < TIMER_CYCLES + stru->wd_index; i++)
    {
        sp805_config( base_addr, &config_FREE_RUN);
        sp805_enable( base_addr );

        if(stru->wd_index == WD_CTRL_INTEN)
        {
            rumboot_printf("Watchdog load value %d\n", stru->wd_index);
        }
        sp805_enable(stru->base_addr);
    }
    return true;
}

static bool wd_test2( uint32_t structure)
{
    struct s805_instance *stru = ( struct s805_instance * )structure;
    int i;

    dcr_write(DCR_WATCHDOG_BASE + WD_REG_LOCK, 0x1ACCE551);
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
    i = dcr_read(DCR_WATCHDOG_BASE+WD_REG_ITCR);
    rumboot_printf("WD_REG_ITCR %d\n", i);
    sp805_enable(stru->base_addr, 0);
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

     if(!result)
     {
         rumboot_printf("Checked TEST_OK\n");
         return 0;
     }
     rumboot_printf("Checked TEST_ERROR\n");
     return result;
}
