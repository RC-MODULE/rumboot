#ifndef DEVICES_GPIO_PL061_H
#define DEVICES_GPIO_PL061_H


#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>


/**
 *
 * \defgroup gpio-pl061 GPIO PL061 functions
 * \ingroup devices
 *
 * \code{.c}
 * #include <devices/gpio-pl061.h>
 * \endcode
 *
 * @{
 */
#define GPIO_PAD_DIR       0x008
#define GPIO_RD_DATA       0x010
#define GPIO_SWITCH_SOURCE 0x024

typedef enum {
    level, rising_edge, falling_edge, both_edge
} gpio_int_type;

typedef enum {
    direction_in = 0, direction_out = 1
} gpio_pin_direction;

typedef enum {
    software_mode = 0, hardware_mode = 1
} gpio_ctrl_mode;


/**
 * Enable( GPIO interrupt
 * @param base_address device base address
 * @param pin_offset
 * @param int_type
 */
void gpio_int_enable( uint32_t base_address, uint32_t pin_offset, gpio_int_type int_type );

/**
 * Disable GPIO interrupt
 * @param base_address device base address
 * @param pin_offset
 */
void gpio_int_disable( uint32_t base_address, uint32_t pin_offset );

/**
 * Clears edge interrupt
 * @param base_address device base address
 * @param pin_offset
 */
void gpio_int_clear( uint32_t base_address, uint32_t pin_offset );

/**
 * Setups direction
 * @param base_address device base address
 * @param pin_offset
 * @param dir
 */
void gpio_set_direction( uint32_t base_address, uint32_t pin_offset, gpio_pin_direction dir );

/**
 * Gets direction
 * @param base_address device base address
 * @param pin_offset
 * @return
 */
gpio_pin_direction gpio_get_direction( uint32_t base_address,  uint32_t pin_offset );

/**
 * Gets value
 * @param base_address device base address
 * @param pin_offset
 * @return
 */
bool gpio_get_value( uint32_t base_address, uint32_t pin_offset );

/**
 * Setups value
 * @param base_address device base address
 * @param pin_offset
 * @param value
 */
void gpio_set_value( uint32_t base_address, uint32_t pin_offset, bool value );

/**
 * Gets all pins data
 * @param base_address device base address
 * @return
 */
uint8_t gpio_get_data( uint32_t base_address );

#endif /* DEVICES_GPIO_PL061_H */
