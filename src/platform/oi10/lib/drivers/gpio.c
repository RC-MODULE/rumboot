#include <stdint.h>
#include <stdlib.h>

#include <platform/oi10/platform/gpio.h>

#include <platform/oi10/platform/regs/regs_gpio.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/timer.h>



void gpio_interrupt_setup(uint32_t base_address, uint8_t pin_number, bool enabled, gpio_int_type int_type){
    if(enabled){
        GPIO_set_GPIO_IE(base_address, (1 << pin_number));
        TEST_ASSERT(GPIO_read_GPIO_IE(base_address) & (1 << pin_number), "gpio_interrupt_setup enable error");
    }else{
        GPIO_clear_GPIO_IE(base_address, (1 << pin_number));
        TEST_ASSERT((GPIO_read_GPIO_IE(base_address) & (1 << pin_number)) == false, "gpio_interrupt_setup disable error");
    }
    switch(int_type){
    case level:
        GPIO_set_GPIO_IS(base_address,1 << pin_number);
        break;
    case rising_edge:
        //set edge triggered
        GPIO_clear_GPIO_IS(base_address,1 << pin_number);
        //disable both edges
        GPIO_clear_GPIO_IBE(base_address,1 << pin_number);
        //setup rising edge
        GPIO_set_GPIO_IEV(base_address,1<<pin_number);
        break;
    case falling_edge:
        //set edge triggered
        GPIO_clear_GPIO_IS(base_address,1 << pin_number);
        //disable both edges
        GPIO_clear_GPIO_IBE(base_address,1 << pin_number);
        //setup falling edge
        GPIO_clear_GPIO_IEV(base_address,1<<pin_number);
        break;
    case both_edge:
        //set edge triggered
        GPIO_clear_GPIO_IS(base_address,1 << pin_number);
        //enable both edges
        GPIO_set_GPIO_IBE(base_address,1 << pin_number);
        break;
    default:
        TEST_ASSERT(false, "unsupported interrupt type\n");
        break;
    }
}

uint8_t gpio_get_ris(uint32_t base_address){
    return GPIO_read_GPIO_RIS(base_address);
}

uint8_t gpio_get_mis(uint32_t base_address){
    return GPIO_read_GPIO_MIS(base_address);
}

void gpio_clear_edge_int(uint32_t base_address, uint8_t pin_number){
    GPIO_write_GPIO_IC(base_address, (1 << pin_number));
}

void gpio_set_direction(uint32_t base_address, uint8_t pin_number, gpio_pin_direction dir){
    if(dir == direction_out){
        GPIO_set_GPIO_DIR(base_address, 1 << pin_number);
        TEST_ASSERT(GPIO_read_GPIO_DIR(base_address) & (1 << pin_number), "Error setting direction out");
    } else { //direction_in
        GPIO_clear_GPIO_DIR(base_address, 1 << pin_number);
        TEST_ASSERT(!(GPIO_read_GPIO_DIR(base_address) & (1 << pin_number)), "Error setting direction in");
    }
}

void gpio_set_port_direction(uint32_t base_address, gpio_pin_dir dir)
{
    GPIO_write_GPIO_DIR(base_address, dir);
}

void gpio_set_control_mode(uint32_t base_address, uint8_t pin_number, gpio_ctrl_mode mode){
    if(mode == hardware_mode){ //== 1
        GPIO_set_GPIO_AFSEL(base_address, 1 << pin_number);
        TEST_ASSERT(GPIO_read_GPIO_AFSEL(base_address) & (1 << pin_number), "Error setting hardware mode");
    } else { //software mode ==0
        GPIO_clear_GPIO_AFSEL(base_address, 1 << pin_number);
        TEST_ASSERT(!(GPIO_read_GPIO_AFSEL(base_address) & (1 << pin_number)), "Error setting software mode");
    }
}

void gpio_set_port_hw_control_mode(uint32_t base_address)
{
    GPIO_set_GPIO_AFSEL(base_address, HW_MODE_PIN_ALL);
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
ioread32();
