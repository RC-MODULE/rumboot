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


typedef enum {
    GPIO_PIN_0 = ( 1 << 0 ),
    GPIO_PIN_1 = ( 1 << 1 ),
    GPIO_PIN_2 = ( 1 << 2 ),
    GPIO_PIN_3 = ( 1 << 3 ),
    GPIO_PIN_4 = ( 1 << 4 ),
    GPIO_PIN_5 = ( 1 << 5 ),
    GPIO_PIN_6 = ( 1 << 6 ),
    GPIO_PIN_7 = ( 1 << 7 )
} gpio_pin_t;

typedef enum {
    GIO_PIN_0_OUT = 1,
    GIO_PIN_1_OUT = ( 1 << 1 ),
    GIO_PIN_2_OUT = ( 1 << 2 ),
    GIO_PIN_3_OUT = ( 1 << 3 ),
    GIO_PIN_4_OUT = ( 1 << 4 ),
    GIO_PIN_5_OUT = ( 1 << 5 ),
    GIO_PIN_6_OUT = ( 1 << 6 ),
    GIO_PIN_7_OUT = ( 1 << 7 ),
    GIO_PIN_ALL_OUT = 0xFF,
    GIO_PIN_ALL_IN = 0x00

} gpio_pin_dir;

/**
 * Setups GPIO interrupt
 * @param base_address device base address
 * @param value
 * @param enabled
 * @param int_type
 */
void gpio_interrupt_setup( uint32_t base_address, uint8_t value, bool enabled, gpio_int_type int_type );


/**
 * Clears edge interrupt
 * @param base_address device base address
 * @param value
 */
void gpio_clear_edge_int( uint32_t base_address, uint8_t value );

/**
 * Setups direction
 * @param base_address device base address
 * @param value
 * @param dir
 */
void gpio_set_direction( uint32_t base_address, uint8_t value, gpio_pin_direction dir );

/**
 * Setups predefined direction
 * @param base_address device base address
 * @param dir
 */
void gpio_set_port_direction( uint32_t base_address, gpio_pin_dir dir );

/**
 * Gets all pins data
 * @param base_address device base address
 * @return
 */
uint8_t gpio_get_data( uint32_t base_address );


#endif /* DEVICES_GPIO_PL061_H */
