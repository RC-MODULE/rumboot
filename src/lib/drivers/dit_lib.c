#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <platform/devices.h>
#include <devices/dit.h>
#include <rumboot/dit_lib.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>



/**
 *  \file dit_lib.c
 *  \brief Double Timer block function libriary.
 *  Contains functions such as:
 *  - Start timer
 *  - Stop timer
 *  - Get timer value
 *  - Clear timer interrupt 
 *  - Config timer
 */


/**
 *  \brief Start timer
 *  
 *  \param [in] index Choose one of two identical timers 
 *  \param [in] base_addr Choose exact sp804 unit by setting its base address
 *  \return Return nothing
 *  
 *  \details Manualy sets ENABLE bit in CONTROL register of chosen timer.
 */
void sp804_enable(int index, int base_addr)
{
    int cntrl ;
    int control_reg;
    if (index){
        control_reg = DIT0_REG_CONTROL1;
    }
    else{
        control_reg = DIT0_REG_CONTROL0;
    }
    cntrl =  ioread8(base_addr+control_reg);
    cntrl |= DIT_CTRL_ENABLE;
    iowrite8(cntrl,base_addr+control_reg);

}


/**
 *  \brief Stop timer
 *  
 *  \param [in] index Choose one of two identical timers 
 *  \param [in] base_addr Choose exact sp804 unit by setting its base address
 *  \return Return nothing
 *  
 *  \details Manualy clears ENABLE bit in CONTROL register of chosen timer.
 */
void sp804_stop(int index, int base_addr)
{
    int cntrl ;
    int control_reg;
    if (index){
        control_reg = DIT0_REG_CONTROL1;
    }
    else{
        control_reg = DIT0_REG_CONTROL0;
    }
    cntrl = ioread8(base_addr+control_reg);
    cntrl = cntrl & (~(DIT_CTRL_ENABLE));
    iowrite8(cntrl,base_addr+control_reg);

}


/**
 *  \brief Get timer value
 *  
 *  \param [in] index Choose one of two identical timers 
 *  \param [in] base_addr Choose exact sp804 unit by setting its base address
 *  \return Return current counter value
 *  
 *  \details  
 */ 
int sp804_get_value(int index, int base_addr)
{
    int value_reg;
    if (index){
        value_reg = DIT0_REG_VALUE1;
    }
    else{
        value_reg = DIT0_REG_VALUE0;
    }
    return ioread32(base_addr+value_reg);
}

 
/**
 *  \brief Clear timer interrupt 
 *  
 *  \param [in] index Choose one of two identical timers 
 *  \param [in] base_addr Choose exact sp804 unit by setting its base address
 *  \return Return nothing
 *  
 *  \details Manualy writes 1 in CLEARINT register of the chosen timer.
 */ 
void sp804_clrint(int index, int base_addr)
{
    int int_clr_reg;
    if (index){
        int_clr_reg = DIT0_REG_INTCLR1;
    }
    else{
        int_clr_reg = DIT0_REG_INTCLR0;
    }
    iowrite32( 1,base_addr+ int_clr_reg);
}


///**
// *  \brief Config timer
// *  
// *  \param [in] load Description for load
// *  \param [in] optional_bgload Description for optional_bgload
// *  \param [in] cntrl: Input control register value
// *  \return Return nothing
// *  
// *  \details Manualy writes input value to CONTROL1, LOAD1 and BGLOAD1 registers. No need to use with oneshot/periodi/freerun funcs.
// */
//void sp804_config_1(int load, int optional_bgload, int cntrl, int base_addr)
//{
//    if (load)
//    {
//        iowrite32(load,base_addr+DIT0_REG_LOAD1);
//    }
//    if (optional_bgload)
//    {
//        iowrite32(optional_bgload,base_addr+DIT0_REG_BGLOAD1);
//    }
//
//    iowrite8( cntrl, base_addr+DIT0_REG_CONTROL1);
//}
//

/**
 *  \brief Config timer
 *  
 *  \param [in] config Input structure, contains config options
 *  \param [in] index Choose one of two identical timers 
 *  \param [in] base_addr Choose exact sp804 unit by setting its base address
 *  \return Return nothing
 *  
 *  \details Sets CONTROl register of the chosen timer due to input structure values,
 *   sets LOAD and BgLoad values if they are non-zero.
 */
void sp804_config( struct sp804_conf config, int index, int base_addr)
{
    int cntrl = 0;
    // MODE
    if (config.mode == ONESHOT){
    cntrl |= DIT_CTRL_ONESHOT;
    cntrl &= ~DIT_CTRL_PERIODIC;
    }
    else if (config.mode == PERIODIC){
    cntrl &= ~ DIT_CTRL_ONESHOT;
    cntrl |=DIT_CTRL_PERIODIC;
    }
    else if (config.mode == FREERUN){
    cntrl &= ~ DIT_CTRL_ONESHOT;
    cntrl &= ~ DIT_CTRL_PERIODIC;
    }
    
    // INT EN
    if (config.interrupt_enable){
        cntrl |= DIT_CTRL_INTEN;
    }
    else{
        cntrl &= ~DIT_CTRL_INTEN;
    }
    
    // CLK DIV
    if (config.clock_division==256){
        cntrl |=  DIT_CTRL_DIV1;
        cntrl &= ~DIT_CTRL_DIV0;
    }
    else if (config.clock_division==16){
        cntrl &= ~DIT_CTRL_DIV1;
        cntrl |= DIT_CTRL_DIV0;
    }
    else{
        cntrl &= ~DIT_CTRL_DIV1;
        cntrl &= ~DIT_CTRL_DIV0;
    }
    
    // SIZE 32
    if (config.width == 32)
    {
        cntrl |= DIT_CTRL_SIZE32;
    }
    else
    {
        cntrl &=  ~DIT_CTRL_SIZE32;
    }     
    
    
    if (index){
        iowrite8( cntrl, base_addr+DIT0_REG_CONTROL1);
        
        // LOAD
        if (config.load)
        {
            iowrite32(config.load,base_addr+DIT0_REG_LOAD1);
        }
        
        // BG LOAD
        if (config.bgload)
        {
            iowrite32(config.bgload,base_addr+DIT0_REG_BGLOAD1);
        }
    }
    else{
        iowrite8( cntrl, base_addr+DIT0_REG_CONTROL0);

        // LOAD
        if (config.load)
        {
            iowrite32(config.load,base_addr+DIT0_REG_LOAD0);
        }
        
        // BG LOAD
        if (config.bgload)
        {
            iowrite32(config.bgload,base_addr+DIT0_REG_BGLOAD0);
        }
    }

}



