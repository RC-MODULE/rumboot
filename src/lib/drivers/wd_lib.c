 
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <platform/devices.h>
#include <regs/regs_sp805.h>
#include <devices/sp805.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>

void sp805_enable( uint32_t base_addr )
{
    int cntrl;
    int control_reg;
    control_reg = WD_CTRL_RESEN;
    cntrl = dcr_read( base_addr + control_reg );
    cntrl |= WD_CTRL_RESEN;
    cntrl = dcr_read( base_addr + control_reg );
    dcr_write( base_addr + control_reg, cntrl);
}

void sp805_stop( uint32_t base_addr)
{
    int cntrl;
    int control_reg;
    control_reg = WD_REG_CONTROL;
    cntrl = dcr_read( base_addr + control_reg );
    cntrl = cntrl & ( ~( WD_CTRL_RESEN ) );
    dcr_write( base_addr + control_reg, cntrl );
}

int sp805_get_value( uint32_t base_addr)
{
    int value_reg;
    value_reg = WD_REG_VALUE;    
    return dcr_read(base_addr + value_reg);
}

void sp805_clrint( uint32_t base_addr)
{
    int int_clr_reg;
    int_clr_reg = WD_REG_INTCLR;
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
    dcr_write( base_addr + WD_REG_CONTROL, cntrl );
        // LOAD
    if( config->load )
    {
        dcr_write( base_addr + WD_REG_LOAD, config->load );
    }
}
