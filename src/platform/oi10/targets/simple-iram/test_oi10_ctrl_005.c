/*
 * test_oi10_ctrl_005.c
 *
 *  Created on: Aug 24, 2018
 *      Author: m.dubinkin
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
#include <devices/sp805.h>
#include <platform/interrupts.h>

#include <platform/regs/regs_mpic128.h>
#include <platform/devices/mpic128.h>

#include <regs/regs_sp805.h>

//static struct sp804_conf conf1, conf2;

//#define TIMER_INT_TIMEOUT       60

struct s805_instance
{
    uint32_t base_addr;
    int dit_index;
};

struct s805_instance_i {
    int wd_irq;
    uint32_t base_addr;
    int dit_index;
} s805_i;

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

static uint32_t check_watchdog_default_rw_val( uint32_t base_addr )
{
    rumboot_printf("Check the default values of the registers:");

    struct regpoker_checker check_default_array[] = {
    {   "WdogLoad",      REGPOKER_READ_DCR,    WD_REG_LOAD,           0x00000000,     0x00000000 },
    {   "WdogLoad",      REGPOKER_WRITE_DCR,   WD_REG_LOAD,           0x00000000,     0x00000000 },

    {   "WdogValue",     REGPOKER_READ_DCR,    WD_REG_VALUE,          0x00000000,     0xffffffff },

    {   "WdogControl",   REGPOKER_READ_DCR,    WD_REG_CONTROL,        0b00,           0b11 },
    {   "WdogControl",   REGPOKER_WRITE_DCR,   WD_REG_CONTROL,        0b00,           0b11 },

    {   "WdogRIS",       REGPOKER_READ_DCR,    WD_REG_RIS,            0b0,            0b0 },

    {   "WdogMIS",       REGPOKER_READ_DCR,    WD_REG_MIS,            0b0,            0b0 },

    {   "WdogLock",      REGPOKER_READ_DCR,    WD_REG_LOAD,           0x00000000, 0xffffffff },
    {   "WdogLock",      REGPOKER_WRITE_DCR,   WD_REG_LOAD,           0x00000000, 0xffffffff },

    {   "WdogITCR",     REGPOKER_READ_DCR,    WD_REG_ITCR,            0b0,           0b0 },
    {   "WdogITCR",     REGPOKER_WRITE_DCR,   WD_REG_ITCR,            0b0,           0b0 },

    {   "WdogITOP",     REGPOKER_WRITE_DCR,   WD_REG_ITOP,            0b00,          0b00 },
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

void sp805_enable( uint32_t base_addr, int index )
{
    int cntrl;
    int control_reg;
    if( index )     /*if index == 1 */
    {
        control_reg = WD_CTRL_RESEN;
    }
    cntrl = dcr_read( base_addr + control_reg );
    cntrl |= WD_CTRL_RESEN;

    cntrl = dcr_read( base_addr + control_reg );
    dcr_write( base_addr + control_reg, cntrl);
}

void sp805_stop( uint32_t base_addr, int index )
{
    int cntrl;
    int control_reg;
    if( index )
    {
        control_reg = WD_REG_CONTROL;
    }
    cntrl = dcr_read( base_addr + control_reg );
    cntrl = cntrl & ( ~( WD_CTRL_RESEN ) );
    dcr_write( base_addr + control_reg, cntrl );
}

int sp805_get_value( uint32_t base_addr, int index )
{
    int value_reg;
    if( index )
    {
        value_reg = WD_REG_VALUE;
    }
    return dcr_read( base_addr + value_reg );
}

void sp805_clrint( uint32_t base_addr, int index )
{
    int int_clr_reg;
    if( index )
    {
        int_clr_reg = WD_REG_INTCLR;
    }
    dcr_write( base_addr + int_clr_reg, 1 );
}

static void handler0( int irq, void *arg )
{
    struct s805_instance_i *a = (struct s805_instance_i *) arg;
    a->wd_irq = a->wd_irq + 1;
    rumboot_printf( "IRQ 0 arrived  \n" );
    rumboot_printf( "sp805_%d timer 0 INT # %d  \n", a->dit_index, a->wd_irq );
    sp805_clrint( a->base_addr, 0 );
}
/*
void sp805_config( uint32_t base_addr, const struct sp805_conf * config, int index )
{
    int cntrl = 0;
    // MODE
    if( config->mode == ONESHOT )
    {
        cntrl |= DIT_CTRL_ONESHOT;
        cntrl &= ~DIT_CTRL_PERIODIC;
    } else if( config->mode == PERIODIC ) {
        cntrl &= ~ DIT_CTRL_ONESHOT;
        cntrl |= DIT_CTRL_PERIODIC;
    } else if( config->mode == FREERUN ) {
        cntrl &= ~ DIT_CTRL_ONESHOT;
        cntrl &= ~ DIT_CTRL_PERIODIC;
    }

/*
bool test_wd(struct s804_instance_i ins)
{

}
*/

uint32_t main(void)
{
    /*uint32_t result;
     * Test scenario:
     * - check APB connection
     * - setup TCR, wait TIMINT1,2
     * - setup timers, wait TIMINT1,2

     *  sp804_conf         : Structure contains configuration parameters
     *  sp804_mode         - Chose counting mode - Oneshot, Periodic or Freerun
     *  interrupt_enable   - Interrupts enabled
     *  width              - Width of the counter - 32 or 16
     *  load               - Load value to count from (won't be writen to corresponding reg if zero )
     *  bgload             - Background Load value to count from (won't be writen to corresponding reg if zero )
     */

    // Set up interrupt handlers
    uint32_t result;
    rumboot_printf( "SP805 test START\n" );
    struct rumboot_irq_entry *tbl = rumboot_irq_create( NULL );
    rumboot_printf( "SP805 int are clean up\n" );
        // ||
    result = check_watchdog_default_ro_val(DCR_WATCHDOG_BASE) ||
            check_watchdog_default_rw_val(DCR_WATCHDOG_BASE);
    if(!result)
    {
        rumboot_printf("Checked test Failed\n");
        return 1;
    }

    rumboot_printf("Checked TEST_OK\n");

    rumboot_printf( "SP805 test START\n" );
//  struct rumboot_irq_entry *tbl = rumboot_irq_create( NULL );
    rumboot_irq_cli();
    rumboot_irq_set_handler( tbl, WDT_INT, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler0, &s805_i );


    rumboot_printf("Checked TEST_OK\n");
//        sp804_config(DCR_TIMERS_BASE, c1, 0);
//        sp804_config(DCR_TIMERS_BASE, c2, 0);

        //sp804_clrint(DCR_TIMERS_BASE, 0);
        //sp804_clrint(DCR_TIMERS_BASE, 1);
     return result;
}
