#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#ifndef GPIO__
#define GPIO__

#define GPIO_PAD_DIR       0x008
#define GPIO_RD_DATA       0x010
#define GPIO_SWITCH_SOURCE 0x024



#endif


typedef enum {
    level,
    rising_edge,
    falling_edge,
    both_edge
}gpio_int_type;

typedef enum {
    direction_in = 0,
    direction_out = 1
}gpio_pin_direction;

typedef enum {
    software_mode = 0,
    hardware_mode = 1
}gpio_ctrl_mode;

typedef enum
{
    HW_MODE_PIN_0   = 1,
    HW_MODE_PIN_1   = (1 << 1),
    HW_MODE_PIN_2   = (1 << 2),
    HW_MODE_PIN_3   = (1 << 3),
    HW_MODE_PIN_4   = (1 << 4),
    HW_MODE_PIN_5   = (1 << 5),
    HW_MODE_PIN_6   = (1 << 6),
    HW_MODE_PIN_7   = (1 << 7),
    HW_MODE_PIN_ALL = 0xFF
} gpio_ctrl_port_mode;

typedef enum
{
    GPIO_PIN_0  = (1 << 0),
    GPIO_PIN_1  = (1 << 1),
    GPIO_PIN_2  = (1 << 2),
    GPIO_PIN_3  = (1 << 3),
    GPIO_PIN_4  = (1 << 4),
    GPIO_PIN_5  = (1 << 5),
    GPIO_PIN_6  = (1 << 6),
    GPIO_PIN_7  = (1 << 7)
} gpio_pin_t;

typedef enum
{
    GIO_PIN_0_OUT       = 1,
    GIO_PIN_1_OUT       = (1 << 1),
    GIO_PIN_2_OUT       = (1 << 2),
    GIO_PIN_3_OUT       = (1 << 3),
    GIO_PIN_4_OUT       = (1 << 4),
    GIO_PIN_5_OUT       = (1 << 5),
    GIO_PIN_6_OUT       = (1 << 6),
    GIO_PIN_7_OUT       = (1 << 7),
    GIO_PIN_ALL_OUT     = 0xFF,
    GIO_PIN_ALL_IN      = 0x00

} gpio_pin_dir;

//global functions
void gpio_interrupt_setup(uint32_t base_address, uint8_t value, bool enabled, gpio_int_type int_type);

uint8_t gpio_get_ris(uint32_t base_address);

uint8_t gpio_get_mis(uint32_t base_address);

void gpio_clear_edge_int(uint32_t base_address, uint8_t value);

void gpio_set_direction(uint32_t base_address, uint8_t value, gpio_pin_direction dir);

void gpio_set_port_direction(uint32_t base_address, gpio_pin_dir dir);

void gpio_set_control_mode(uint32_t base_address, uint8_t value, gpio_ctrl_mode mode);

void gpio_set_port_hw_control_mode(uint32_t base_address);

void gpio_set_pin(uint32_t base_address, uint8_t pin_number, uint8_t value);

uint8_t gpio_get_data(uint32_t base_address);

uint8_t gpio_get_pin(uint32_t base_address,uint8_t pin_number);
