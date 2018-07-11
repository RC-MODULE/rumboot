#include <stdint.h>
#include <stdlib.h>

#include <devices/gpio.h>

#include <regs/regs_gpio.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/timer.h>


static volatile uint32_t IRQ;
//GPIO0_IRQ_o
void NON_CR_INTERRUPT_HANDLER(void)
{
//  uint32_t gpio_status;
    trace_msg ("NON_CR_INTERRUPT_HANDLER \n");
    //get interrupt source
    uint32_t source_vector = mpic_get_ncr_interrupt_vector(Processor0);// - clear interrupt by read NCIAR
    trace_msg("NON_CRITICAL int handler message \n");
    trace_hex (source_vector);
    if (source_vector == LSIF0_MGPIO0_IRQ_o) {
        trace_msg("LSIF0_MGPIO0_IRQ_o \n");
        IRQ = 1;
        MEM32(LSIF0_MGPIO0_BASE + GPIOIC)   = 0xff; //clear interrupts      
    }
    if (source_vector == LSIF0_MGPIO1_IRQ_o) {
        trace_msg("LSIF0_MGPIO1_IRQ_o \n");
        IRQ = 1;
        MEM32(LSIF0_MGPIO1_BASE + GPIOIC)   = 0xff; //clear interrupts      
    }
    if (source_vector == LSIF0_MGPIO2_IRQ_o) {
        trace_msg("LSIF0_MGPIO2_IRQ_o \n");
        IRQ = 1;
        MEM32(LSIF0_MGPIO2_BASE + GPIOIC)   = 0xff; //clear interrupts      
    }
    if (source_vector == LSIF0_MGPIO3_IRQ_o) {
        trace_msg("LSIF0_MGPIO3_IRQ_o \n");
        IRQ = 1;
        MEM32(LSIF0_MGPIO3_BASE + GPIOIC)   = 0xff; //clear interrupts      
    }
    if (source_vector == LSIF0_MGPIO4_IRQ_o) {
        trace_msg("LSIF0_MGPIO4_IRQ_o \n");
        IRQ = 1;
        MEM32(LSIF0_MGPIO4_BASE + GPIOIC)   = 0xff; //clear interrupts      
    }
    if (source_vector == LSIF0_MGPIO5_IRQ_o) {
        trace_msg("LSIF0_MGPIO5_IRQ_o \n");
        IRQ = 1;
        MEM32(LSIF0_MGPIO5_BASE + GPIOIC)   = 0xff; //clear interrupts      
    }
    if (source_vector == LSIF0_MGPIO6_IRQ_o) {
        trace_msg("LSIF0_MGPIO6_IRQ_o \n");
        IRQ = 1;
        MEM32(LSIF0_MGPIO6_BASE + GPIOIC)   = 0xff; //clear interrupts      
    }
    if (source_vector == LSIF0_MGPIO7_IRQ_o) {
        trace_msg("LSIF0_MGPIO7_IRQ_o \n");
        IRQ = 1;
        MEM32(LSIF0_MGPIO7_BASE + GPIOIC)   = 0xff; //clear interrupts      
    }
    if (source_vector == LSIF0_MGPIO8_IRQ_o) {
        trace_msg("LSIF0_MGPIO8_IRQ_o \n");
        IRQ = 1;
        MEM32(LSIF0_MGPIO8_BASE + GPIOIC)   = 0xff; //clear interrupts      
    }
    if (source_vector == LSIF0_MGPIO9_IRQ_o) {
        trace_msg("LSIF0_MGPIO9_IRQ_o \n");
        IRQ = 1;
        MEM32(LSIF0_MGPIO9_BASE + GPIOIC)   = 0xff; //clear interrupts      
    }
    if (source_vector == LSIF0_MGPIO10_IRQ_o) {
        trace_msg("LSIF0_MGPIO10_IRQ_o \n");
        IRQ = 1;
        MEM32(LSIF0_MGPIO10_BASE + GPIOIC)  = 0xff; //clear interrupts      
    }
    
    if (source_vector == GPIO0_IRQ_o) {
        trace_msg("GPIO0_IRQ_o \n");
        IRQ = 1;
        MEM32(GPIO0_BASE + GPIOIC)  = 0xff; //clear interrupts
    }
    if (source_vector == GPIO1_IRQ_o) {
        trace_msg("GPIO1_IRQ_o \n");
        IRQ = 1;
        MEM32(GPIO1_BASE + GPIOIC)  = 0xff; //clear interrupts      
    }
    if (source_vector == LSIF1_MGPIO0_IRQ_o) {
        trace_msg("LSIF1_MGPIO0_IRQ_o \n");
        IRQ = 1;
        MEM32(LSIF1_MGPIO0_BASE + GPIOIC)   = 0xff; //clear interrupts      
    }
    if (source_vector == LSIF1_MGPIO1_IRQ_o) {
        trace_msg("LSIF1_MGPIO1_IRQ_o \n");
        IRQ = 1;
        MEM32(LSIF1_MGPIO1_BASE + GPIOIC)   = 0xff; //clear interrupts      
    }
    if (source_vector == LSIF1_MGPIO2_IRQ_o) {
        trace_msg("LSIF1_MGPIO2_IRQ_o \n");
        IRQ = 1;
        MEM32(LSIF1_MGPIO2_BASE + GPIOIC)   = 0xff; //clear interrupts      
    }
    if (source_vector == LSIF1_MGPIO3_IRQ_o) {
        trace_msg("LSIF1_MGPIO3_IRQ_o \n");
        IRQ = 1;
        MEM32(LSIF1_MGPIO3_BASE + GPIOIC)   = 0xff; //clear interrupts      
    }
    if (source_vector == LSIF1_MGPIO4_IRQ_o) {
        trace_msg("LSIF1_MGPIO4_IRQ_o \n");
        IRQ = 1;
        MEM32(LSIF1_MGPIO4_BASE + GPIOIC)   = 0xff; //clear interrupts      
    }
    END_NONCR_INT_P0;
}
//---------------------------------------
void gpio_irq_set(uint32_t irq_num) {
    // Initialization Inrerrupt controller
    IRQ = 0;
    trace_msg ("    Start MPIC initialization \n --irq_num:");
    trace_hex(irq_num);
    mpic_reset();
    mpic_pass_through_disable();
    mpic_setup_ext_interrupt(irq_num, MPIC_PRIOR_1,int_sense_level,int_pol_pos,Processor0);
    SET_NONCR_INT_HANDLER(non_cr_interrupt_handler);
    ppc_noncr_int_enable();
    mpic_set_interrupt_borders(10, 5);//MCK, CR borders
    mpic_set_minimal_interrupt_priority_mask(Processor0, MPIC_PRIOR_0);
    trace_msg ("    MPIC ave been initialized \n");
}
//---------------------------------------
uint32_t wait_gpio_int(){
    unsigned t;
    
    trace_msg ("wait_gpio_int \n");
    
    for (t=1;t<=GPIO_TIMEOUT;t++){
        if (IRQ==1){
            IRQ = 0;
            break;
        }
    }
    if (t>=GPIO_TIMEOUT) {
        trace_msg("Error! IRQ flag wait timeout! \n");
        return TEST_ERROR;
    }
    return TEST_OK;
}
//---------------------------------------
uint32_t test_gpio(uint32_t GPIO_ADDR, uint32_t GPIODIR_value, uint32_t irq_active, uint32_t irq_num){
//  trace_msg ("test_gpio");
    uint32_t test_result;
    uint8_t data_read_gpio, data_write_gpio, shift_data_read_gpio;
    
    if (irq_active == 1){   
        gpio_irq_set(irq_num);
    }
    
    data_write_gpio = 0xFF & GPIODIR_value;
    //init GPIO_W
    MEM32(GPIO_ADDR + GPIOAFSEL)    = 0x00          ; //gpio to gpio mode
    MEM32(GPIO_ADDR + GPIODIR)      = GPIODIR_value ; //set GPIO direction
    MEM32(GPIO_ADDR + 0x3fc)        = 0x00          ; //write data to output
    if (irq_active == 1){
        MEM32(GPIO_ADDR + GPIOIC)   = 0xff          ; //clear interrupts
        MEM32(GPIO_ADDR + GPIOIE)   = 0x00          ; //mask interrupts
        MEM32(GPIO_ADDR + GPIOIBE)  = 0xff          ; //enable interrupts both edges
        MEM32(GPIO_ADDR + GPIOIE)   = 0xff          ; //unmask interrupts
    }
    //write
    MEM32(GPIO_ADDR + 0x3fc) = data_write_gpio;
    if (irq_active == 1){
        if (wait_gpio_int()==TEST_ERROR){
            return TEST_ERROR;
        }
    }
    
    //read
    data_read_gpio = MEM32(GPIO_ADDR + 0x3fc);
    switch (GPIODIR_value){
        case 0x0f:{
                shift_data_read_gpio = data_read_gpio>>4;
                break;
            }
        case 0xf0:{
                shift_data_read_gpio = data_read_gpio<<4;
                break;
            }
        case 0x07:{
                shift_data_read_gpio = (data_read_gpio&0x3F)>>3;
                break;
            }
        case 0x38:{
                shift_data_read_gpio = ((data_read_gpio&0x3F)<<3)&0x3F;
                break;
            }
        case 0x03:{
                shift_data_read_gpio = (data_read_gpio&0x0F)>>2;
                break;
            }
        case 0x0c:{
                shift_data_read_gpio = ((data_read_gpio&0x0F)<<2)&0x0F;
                break;
            }
        default :{
            shift_data_read_gpio = data_read_gpio;
            test_result = TEST_ERROR;
            trace_msg( "Default shift!!!" );
            return TEST_ERROR;
            }
    }
        
    trace_msg( "shift_data_read_gpio : " );
    trace_hex(shift_data_read_gpio);
    
    if (shift_data_read_gpio!=data_write_gpio) {
        test_result = TEST_ERROR;
        trace_msg("Error! \n");
        trace_msg( "data_write_gpio : " );
        trace_hex(data_write_gpio);
    }else {
        test_result = TEST_OK;
    }
    
    return test_result;
}
