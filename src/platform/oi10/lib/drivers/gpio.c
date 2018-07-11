#include <stdint.h>
#include <stdlib.h>

#include <platform/oi10/platform/devices/gpio.h>

#include <platform/oi10/platform/regs/regs_gpio.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/timer.h>

 static void reg_write(uint32_t base, uint32_t reg_offset, uint32_t value)
 {
     iowrite32(value, base + reg_offset);
 }

 static uint32_t reg_read(uint32_t base, uint32_t reg_offset)
 {
     return ioread32(base + reg_offset);
 }

 static void reg_set(uint32_t base, uint32_t reg_offset, uint32_t value)
 {
     uint32_t reg = ioread32(base + reg_offset);

     reg |= (value);
     iowrite32(reg, base + reg_offset);
 }

 static void reg_clear(uint32_t base, uint32_t reg_offset, uint32_t value)
 {
     uint32_t reg = ioread32(base + reg_offset);

     reg &= ~(value);
     iowrite32(reg, base + reg_offset);
 }


void gpio_interrupt_setup(uint32_t base_address, uint8_t pin_number, bool enabled, gpio_int_type int_type){
    if(enabled)
        reg_set(base_address, GPIO_IE, (1 << pin_number));
    else
        reg_clear(base_address, GPIO_IE, (1 << pin_number));

    switch(int_type){
    case level:
        reg_set(base_address, GPIO_IS, (1 << pin_number));;
        break;
    case rising_edge:
        //set edge triggered
        reg_clear(base_address, GPIO_IS, (1 << pin_number));
        //disable both edges
        reg_clear(base_address, GPIO_IBE, (1 << pin_number));
        //setup rising edge
        reg_set(base_address, GPIO_IEV, (1 << pin_number));
        break;
    case falling_edge:
        //set edge triggered
        reg_clear(base_address, GPIO_IS, (1 << pin_number));
        //disable both edges
        reg_clear(base_address, GPIO_IBE, (1 << pin_number));
        //setup falling edge
        reg_clear(base_address, GPIO_IEV, (1 << pin_number));
        break;
    case both_edge:
        //set edge triggered
        reg_clear(base_address, GPIO_IS, (1 << pin_number));
        //enable both edges
        reg_set(base_address, GPIO_IBE, (1 << pin_number));
        break;
    default:
        rumboot_putstring("unsupported interrupt type\n");
        break;
    }
}

uint8_t gpio_get_ris(uint32_t base_address){
    return reg_read(base_address, GPIO_RIS);
}

uint8_t gpio_get_mis(uint32_t base_address){
    return reg_read(base_address, GPIO_MIS);
}

void gpio_clear_edge_int(uint32_t base_address, uint8_t pin_number){
    reg_write(base_address, GPIO_IC, (1 << pin_number));
}

void gpio_set_direction(uint32_t base_address, uint8_t pin_number, gpio_pin_direction dir){
    if(dir == direction_out)
        reg_set(base_address, GPIO_DIR, (1 << pin_number));
    else //direction_in
        reg_clear(base_address, GPIO_DIR, (1 << pin_number));
}

void gpio_set_port_direction(uint32_t base_address, gpio_pin_dir dir)
{
    reg_write(base_address, GPIO_DIR, dir);
}

void gpio_set_control_mode(uint32_t base_address, uint8_t pin_number, gpio_ctrl_mode mode){
    if(mode == hardware_mode) //== 1
        reg_set(base_address, GPIO_AFSEL, (1 << pin_number));
    else //software mode ==0
        reg_clear(base_address, GPIO_AFSEL, (1 << pin_number));
}

void gpio_set_port_hw_control_mode(uint32_t base_address)
{
    reg_set(base_address, GPIO_AFSEL, HW_MODE_PIN_ALL);
}

void gpio_set_pin(uint32_t base_address, uint8_t pin_number, uint8_t value){
    *(volatile uint8_t*)(base_address + (1 << (pin_number + 2))) = (value << pin_number);
}

//return 8bit
uint8_t gpio_get_data(uint32_t base_address){
    return *(volatile uint8_t*)(base_address + (0xFF << 2));
}
//return value of specified pin
uint8_t gpio_get_pin(uint32_t base_address,uint8_t pin_number){
    uint8_t raw_val;
    raw_val = *(volatile uint8_t*)(base_address + (1 << (pin_number + 2)));
    return (raw_val >> pin_number);
}
