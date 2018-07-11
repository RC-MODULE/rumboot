#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#ifndef GPIO__
#define GPIO__

#define GPIO_PAD_DIR       0x008
#define GPIO_RD_DATA       0x010
#define GPIO_SWITCH_SOURCE 0x024



#endif


void gpio_interrupt_setup(uint32_t base_address, uint8_t pin_number, bool enabled, gpio_int_type int_type);

uint8_t gpio_get_ris(uint32_t base_address);

uint8_t gpio_get_mis(uint32_t base_address);

void gpio_clear_edge_int(uint32_t base_address, uint8_t pin_number);

void gpio_set_direction(uint32_t base_address, uint8_t pin_number, gpio_pin_direction dir);

void gpio_set_port_direction(uint32_t base_address, gpio_pin_dir dir);

void gpio_set_control_mode(uint32_t base_address, uint8_t pin_number, gpio_ctrl_mode mode);

void gpio_set_port_hw_control_mode(uint32_t base_address);

void gpio_set_pin(uint32_t base_address, uint8_t pin_number, uint8_t value);

uint8_t gpio_get_data(uint32_t base_address);

uint8_t gpio_get_pin(uint32_t base_address,uint8_t pin_number);
