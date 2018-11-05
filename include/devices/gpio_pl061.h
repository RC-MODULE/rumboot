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


#define GPIO_PIN_N  8


typedef enum {
    level,
    rising_edge,
    falling_edge,
    both_edge
} gpio_int_type;

typedef enum {
    direction_in = 0,
    direction_out = 1
} gpio_pin_direction;

typedef enum {
    software_mode = 0,
    hardware_mode = 1
} gpio_ctrl_mode;


/**
 * Enable GPIO interrupt by mask
 * @param base_address device base address
 * @param pins_mask bits mask of pins
 * @param int_type
 */
void gpio_int_enable_by_mask( uint32_t base_address, uint8_t pins_mask, gpio_int_type int_type );

/**
 * Enable GPIO pin interrupt
 * @param base_address device base address
 * @param pin_offset
 * @param int_type
 */
void gpio_int_enable( uint32_t base_address, uint32_t pin_offset, gpio_int_type int_type );

/**
 * Disable GPIO interrupt by mask
 * @param base_address device base address
 * @param pins_mask bits mask of pins
 */
void gpio_int_disable_by_mask( uint32_t base_address, uint8_t pins_mask );

/**
 * Disable GPIO pin interrupt
 * @param base_address device base address
 * @param pin_offset
 */
void gpio_int_disable( uint32_t base_address, uint32_t pin_offset );

/**
 * Clears GPIO interrupt by mask
 * @param base_address device base address
 * @param pins_mask bits mask of pins
 */
void gpio_int_clear_by_mask( uint32_t base_address, uint8_t pins_mask );

/**
 * Clears GPIO pin interrupt
 * @param base_address device base address
 * @param pin_offset
 */
void gpio_int_clear( uint32_t base_address, uint32_t pin_offset );


/**
 * Setups direction by mask
 * @param base_address device base address
 * @param pins_mask bits mask of pins
 * @param dir
 */
void gpio_set_direction_by_mask( uint32_t base_address, uint8_t pins_mask, gpio_pin_direction dir );

/**
 * Setups pin direction
 * @param base_address device base address
 * @param pin_offset
 * @param dir
 */
void gpio_set_direction( uint32_t base_address, uint32_t pin_offset, gpio_pin_direction dir );

/**
 * Gets pin direction
 * @param base_address device base address
 * @param pin_offset
 * @return
 */
gpio_pin_direction gpio_get_direction( uint32_t base_address,  uint32_t pin_offset );


/**
 * Setups control mode by mask
 * @param base_address device base address
 * @param pins_mask bits mask of pins
 * @param ctrl_mode
 */
void gpio_set_ctrl_mode_by_mask( uint32_t base_address, uint8_t pins_mask, gpio_ctrl_mode ctrl_mode );

/**
 * Setups pin control mode
 * @param base_address device base address
 * @param pin_offset
 * @param ctrl_mode
 */
void gpio_set_ctrl_mode( uint32_t base_address, uint32_t pin_offset, gpio_ctrl_mode ctrl_mode );

/**
 * Gets pin control mode
 * @param base_address device base address
 * @param pin_offset
 * @return
 */
gpio_ctrl_mode gpio_get_ctrl_mode( uint32_t base_address,  uint32_t pin_offset );

/**
 * Setups pins value by mask
 * @param base_address device base address
 * @param pins_mask bits mask of pins
 * @param pins_value
 */
void gpio_set_value_by_mask( uint32_t const base_address, uint8_t const pins_mask, uint8_t const pins_value );

/**
 * Setups pin value
 * @param base_address device base address
 * @param pin_offset
 * @param value
 */
void gpio_set_value( uint32_t base_address, uint32_t pin_offset, bool value );

/**
 * Gets pin value
 * @param base_address device base address
 * @param pin_offset
 * @return
 */
bool gpio_get_value( uint32_t base_address, uint32_t pin_offset );

/**
 * Gets all pins data
 * @param base_address device base address
 * @return
 */
uint8_t gpio_get_data( uint32_t base_address );

/**
 * @}
 */


#endif /* DEVICES_GPIO_PL061_H */
