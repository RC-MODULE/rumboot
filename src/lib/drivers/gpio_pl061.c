#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include <platform/test_assert.h>
#include <platform/common_macros/common_macros.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/timer.h>
#include <devices/gpio_pl061.h>
#include <regs/regs_gpio_pl061.h>


static inline __attribute__((always_inline)) void ioset8( uint8_t const value, uint32_t const address ) {
    iowrite8( ioread8( address ) | value , address );
}

static inline __attribute__((always_inline)) void ioclear8( uint8_t const value, uint32_t const address ) {
    iowrite8( ioread8( address ) & ~value, address );
}


void gpio_int_enable_by_mask( uint32_t const base_address, uint8_t const pins_mask, gpio_int_type const int_type ) {
    switch( int_type ) {
    case level:
        ioset8( pins_mask, base_address+GPIO_IS );
        break;
    case rising_edge:
        ioclear8( pins_mask, base_address+GPIO_IS );    /* set edge triggered */
        ioclear8( pins_mask, base_address+GPIO_IBE );   /* disable both edges */
        ioset8( pins_mask, base_address+GPIO_IEV );     /* setup rising edge */
        break;
    case falling_edge:
        ioclear8( pins_mask, base_address+GPIO_IS );    /* set edge triggered */
        ioclear8( pins_mask, base_address+GPIO_IBE );   /* disable both edges */
        ioclear8( pins_mask, base_address+GPIO_IEV );   /* setup falling edge */
        break;
    case both_edge:
        ioclear8( pins_mask, base_address+GPIO_IS );    /* set edge triggered */
        ioset8( pins_mask, base_address+GPIO_IBE );     /* enable both edges */
        break;
    default:
        TEST_ASSERT( 0, "Unsupported interrupt type" );
        return;
    }

    ioset8( pins_mask, base_address+GPIO_IE );
}

void gpio_int_enable( uint32_t const base_address, uint32_t const pin_offset, gpio_int_type const int_type ) {
    TEST_ASSERT( pin_offset < GPIO_PIN_N, "ERROR!!! No such GPIO port exists" );

    gpio_int_enable_by_mask( base_address, (0b1 << pin_offset), int_type );
}


void gpio_int_disable_by_mask( uint32_t const base_address, uint8_t const pins_mask ) {
    ioclear8( pins_mask, base_address+GPIO_IE );
}

void gpio_int_disable( uint32_t const base_address, uint32_t const pin_offset ) {
    TEST_ASSERT( pin_offset < GPIO_PIN_N, "ERROR!!! No such GPIO port exists" );

    gpio_int_disable_by_mask( base_address, (0b1 << pin_offset) );
}


void gpio_int_clear_by_mask( uint32_t const base_address, uint8_t const pins_mask ) {
    iowrite8( pins_mask, base_address+GPIO_IC );
}

void gpio_int_clear( uint32_t const base_address, uint32_t const pin_offset ) {
    TEST_ASSERT( pin_offset < GPIO_PIN_N, "ERROR!!! No such GPIO port exists" );

    gpio_int_clear_by_mask( base_address, (0b1 << pin_offset) );
}


void gpio_set_direction_by_mask( uint32_t const base_address, uint8_t const pins_mask, gpio_pin_direction const dir ) {
    if( dir == direction_out )  ioset8( pins_mask, base_address+GPIO_DIR );
    else /* direction_in */     ioclear8( pins_mask, base_address+GPIO_DIR );
}

void gpio_set_direction( uint32_t const base_address, uint32_t const pin_offset, gpio_pin_direction const dir ) {
    TEST_ASSERT( pin_offset < GPIO_PIN_N, "ERROR!!! No such GPIO port exists" );

    gpio_set_direction_by_mask( base_address, (0b1 << pin_offset), dir );
}

gpio_pin_direction gpio_get_direction( uint32_t const base_address,  uint32_t const pin_offset ){
    TEST_ASSERT( pin_offset < GPIO_PIN_N, "ERROR!!! No such GPIO port exists" );

    return (gpio_pin_direction)((0b1 << pin_offset) & ioread8( base_address+GPIO_DIR ));
}


void gpio_set_ctrl_mode_by_mask( uint32_t const base_address, uint8_t const pins_mask, gpio_ctrl_mode const ctrl_mode ) {
    if( ctrl_mode == software_mode )    ioclear8( pins_mask, base_address+GPIO_AFSEL );
    else /* hardware_mode */            ioset8( pins_mask, base_address+GPIO_AFSEL );
}

void gpio_set_ctrl_mode( uint32_t const base_address, uint32_t const pin_offset, gpio_ctrl_mode const ctrl_mode ) {
    TEST_ASSERT( pin_offset < GPIO_PIN_N, "ERROR!!! No such GPIO port exists" );

    gpio_set_ctrl_mode_by_mask( base_address, (0b1 << pin_offset), ctrl_mode );
}

gpio_ctrl_mode gpio_get_ctrl_mode( uint32_t const base_address,  uint32_t const pin_offset ) {
    TEST_ASSERT( pin_offset < GPIO_PIN_N, "ERROR!!! No such GPIO port exists" );

    return (gpio_ctrl_mode)((0b1 << pin_offset) & ioread8( base_address+GPIO_AFSEL ));
}


void gpio_set_value_by_mask( uint32_t const base_address, uint8_t const pins_mask, uint8_t const pins_value ) {
    iowrite8( pins_value, base_address+GPIO_DATA + GPIO_GET_PINS_OFFSET( pins_mask ) );
}

void gpio_set_value( uint32_t const base_address, uint32_t const pin_offset, bool const value ) {
    TEST_ASSERT( pin_offset < GPIO_PIN_N, "ERROR!!! No such GPIO port exists" );

    gpio_set_value_by_mask( base_address, (0b1 << pin_offset), (value << pin_offset) );
}

bool gpio_get_value( uint32_t const base_address, uint32_t const pin_offset ) {
    TEST_ASSERT( pin_offset < GPIO_PIN_N, "ERROR!!! No such GPIO port exists" );

    return (bool)( ioread8( base_address+GPIO_DATA + GPIO_GET_PINS_OFFSET(0b1 << pin_offset) ) >> pin_offset );
}

uint8_t gpio_get_data( uint32_t const base_address ) {
    return ioread8( base_address+GPIO_DATA + GPIO_ADDR_MASK );
}
