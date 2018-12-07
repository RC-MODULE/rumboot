#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <platform/devices.h>
#include <regs/regs_sp804.h>
#include <devices/sp804.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>



void sp804_enable( uint32_t base_addr, int index)
{
    int cntrl ;
    int control_reg;
    if (index){
        control_reg = DIT0_REG_CONTROL1;
    }
    else{
        control_reg = DIT0_REG_CONTROL0;
    }
    cntrl =  dcr_read(base_addr+control_reg);
    cntrl |= DIT_CTRL_ENABLE;
    dcr_write(cntrl,base_addr+control_reg);

}


void sp804_stop( uint32_t base_addr, int index)
{
    int cntrl ;
    int control_reg;
    if (index){
        control_reg = DIT0_REG_CONTROL1;
    }
    else{
        control_reg = DIT0_REG_CONTROL0;
    }
    cntrl = dcr_read(base_addr+control_reg);
    cntrl = cntrl & (~(DIT_CTRL_ENABLE));
    dcr_write(cntrl,base_addr+control_reg);

}


int sp804_get_value( uint32_t base_addr, int index)
{
    int value_reg;
    if (index){
        value_reg = DIT0_REG_VALUE1;
    }
    else{
        value_reg = DIT0_REG_VALUE0;
    }
    return dcr_read(base_addr+value_reg);
}



void sp804_clrint( uint32_t base_addr, int index)
{
    int int_clr_reg;
    if (index){
        int_clr_reg = DIT0_REG_INTCLR1;
    }
    else{
        int_clr_reg = DIT0_REG_INTCLR0;
    }
    dcr_write( 1,base_addr+ int_clr_reg);
}



void sp804_config( uint32_t base_addr, const struct sp804_conf * config, int index)
{
    int cntrl = 0;
    // MODE
    if (config->mode == ONESHOT){
    cntrl |= DIT_CTRL_ONESHOT;
    cntrl &= ~DIT_CTRL_PERIODIC;
    }
    else if (config->mode == PERIODIC){
    cntrl &= ~ DIT_CTRL_ONESHOT;
    cntrl |=DIT_CTRL_PERIODIC;
    }
    else if (config->mode == FREERUN){
    cntrl &= ~ DIT_CTRL_ONESHOT;
    cntrl &= ~ DIT_CTRL_PERIODIC;
    }

    // INT EN
    if (config->interrupt_enable){
        cntrl |= DIT_CTRL_INTEN;
    }
    else{
        cntrl &= ~DIT_CTRL_INTEN;
    }

    // CLK DIV
    if (config->clock_division==256){
        cntrl |=  DIT_CTRL_DIV1;
        cntrl &= ~DIT_CTRL_DIV0;
    }
    else if (config->clock_division==16){
        cntrl &= ~DIT_CTRL_DIV1;
        cntrl |= DIT_CTRL_DIV0;
    }
    else{
        cntrl &= ~DIT_CTRL_DIV1;
        cntrl &= ~DIT_CTRL_DIV0;
    }

    // SIZE 32
    if (config->width == 32)
    {
        cntrl |= DIT_CTRL_SIZE32;
    }
    else
    {
        cntrl &=  ~DIT_CTRL_SIZE32;
    }


    if (index){
        dcr_write( cntrl, base_addr+DIT0_REG_CONTROL1);

        // LOAD
        if (config->load)
        {
            dcr_write(config->load,base_addr+DIT0_REG_LOAD1);
        }

        // BG LOAD
        if (config->bgload)
        {
            dcr_write(config->bgload,base_addr+DIT0_REG_BGLOAD1);
        }
    }
    else{
        dcr_write( cntrl, base_addr+DIT0_REG_CONTROL0);

        // LOAD
        if (config->load)
        {
            dcr_write(config->load,base_addr+DIT0_REG_LOAD0);
        }

        // BG LOAD
        if (config->bgload)
        {
            dcr_write(config->bgload,base_addr+DIT0_REG_BGLOAD0);
        }
    }

}
