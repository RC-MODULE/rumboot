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


static inline __attribute__((always_inline)) void reg_write( uint8_t const value, uint32_t const address ) {
    iowrite32( value, address );
}

static inline __attribute__((always_inline)) uint8_t reg_read( uint32_t const address ) {
    return (uint8_t)( ioread32(address) & GPIO_REG_MASK );
}

static inline __attribute__((always_inline)) void reg_set( uint8_t const value, uint32_t const address ) {
    reg_write( reg_read(address) | value, address );
}

static inline __attribute__((always_inline)) void reg_clear( uint8_t const value, uint32_t const address ) {
    reg_write( reg_read(address) & ~value, address );
}


void gpio_int_enable_by_mask( uint32_t const base_address, uint8_t const pins_mask, gpio_int_type const int_type ) {
    switch( int_type ) {
    case gpio_int_level:
        reg_set( pins_mask, base_address+GPIO_IS );     /* set level triggered */
        break;
    case gpio_int_rising_edge:
        reg_clear( pins_mask, base_address+GPIO_IS );   /* set edge triggered */
        reg_clear( pins_mask, base_address+GPIO_IBE );  /* disable both edges */
        reg_set( pins_mask, base_address+GPIO_IEV );    /* setup rising edge */
        break;
    case gpio_int_falling_edge:
        reg_clear( pins_mask, base_address+GPIO_IS );   /* set edge triggered */
        reg_clear( pins_mask, base_address+GPIO_IBE );  /* disable both edges */
        reg_clear( pins_mask, base_address+GPIO_IEV );  /* setup falling edge */
        break;
    case gpio_int_both_edge:
        reg_clear( pins_mask, base_address+GPIO_IS );   /* set edge triggered */
        reg_set( pins_mask, base_address+GPIO_IBE );    /* enable both edges */
        break;
    default:
        TEST_ASSERT( 0, "Unsupported interrupt type" );
        return;
    }

    reg_set( pins_mask, base_address+GPIO_IE );         /* enable interrupt */
}

void gpio_int_enable( uint32_t const base_address, uint32_t const pin_offset, gpio_int_type const int_type ) {
    TEST_ASSERT( pin_offset < GPIO_PIN_N, "ERROR!!! No such GPIO port exists" );

    gpio_int_enable_by_mask( base_address, (0b1 << pin_offset), int_type );
}


void gpio_int_disable_by_mask( uint32_t const base_address, uint8_t const pins_mask ) {
    reg_clear( pins_mask, base_address+GPIO_IE );
}

void gpio_int_disable( uint32_t const base_address, uint32_t const pin_offset ) {
    TEST_ASSERT( pin_offset < GPIO_PIN_N, "ERROR!!! No such GPIO port exists" );

    gpio_int_disable_by_mask( base_address, (0b1 << pin_offset) );
}


void gpio_int_clear_by_mask( uint32_t const base_address, uint8_t const pins_mask ) {
    reg_write( pins_mask, base_address+GPIO_IC );
}

void gpio_int_clear( uint32_t const base_address, uint32_t const pin_offset ) {
    TEST_ASSERT( pin_offset < GPIO_PIN_N, "ERROR!!! No such GPIO port exists" );

    gpio_int_clear_by_mask( base_address, (0b1 << pin_offset) );
}


void gpio_set_direction_by_mask( uint32_t const base_address, uint8_t const pins_mask, gpio_pin_direction const dir ) {
    if(      dir == gpio_pin_direction_0in_1out )   reg_write( pins_mask, base_address+GPIO_DIR );
    else if( dir == gpio_pin_direction_in )         reg_clear( pins_mask, base_address+GPIO_DIR );
    else /*  dir == gpio_pin_direction_out */       reg_set(   pins_mask, base_address+GPIO_DIR );
}

void gpio_set_direction( uint32_t const base_address, uint32_t const pin_offset, gpio_pin_direction const dir ) {
    TEST_ASSERT( pin_offset < GPIO_PIN_N, "ERROR!!! No such GPIO port exists" );

    gpio_set_direction_by_mask( base_address, (0b1 << pin_offset), dir );
}

gpio_pin_direction gpio_get_direction( uint32_t const base_address,  uint32_t const pin_offset ){
    TEST_ASSERT( pin_offset < GPIO_PIN_N, "ERROR!!! No such GPIO port exists" );

    return (gpio_pin_direction)( (0b1 << pin_offset) & (ioread32( base_address+GPIO_DIR ) & GPIO_REG_MASK) );
}


void gpio_set_ctrl_mode_by_mask( uint32_t const base_address, uint8_t const pins_mask, gpio_ctrl_mode const ctrl_mode ) {
    if(      ctrl_mode == gpio_ctrl_mode_0sw_1hw )  reg_write( pins_mask, base_address+GPIO_AFSEL );
    else if( ctrl_mode == gpio_ctrl_mode_sw )       reg_clear( pins_mask, base_address+GPIO_AFSEL );
    else /*  ctrl_mode == gpio_ctrl_mode_hw */      reg_set(   pins_mask, base_address+GPIO_AFSEL );
}

void gpio_set_ctrl_mode( uint32_t const base_address, uint32_t const pin_offset, gpio_ctrl_mode const ctrl_mode ) {
    TEST_ASSERT( pin_offset < GPIO_PIN_N, "ERROR!!! No such GPIO port exists" );

    gpio_set_ctrl_mode_by_mask( base_address, (0b1 << pin_offset), ctrl_mode );
}

gpio_ctrl_mode gpio_get_ctrl_mode( uint32_t const base_address,  uint32_t const pin_offset ) {
    TEST_ASSERT( pin_offset < GPIO_PIN_N, "ERROR!!! No such GPIO port exists" );

    return (gpio_ctrl_mode)( (0b1 << pin_offset) & reg_read(base_address+GPIO_AFSEL) );
}


void gpio_set_value_by_mask( uint32_t const base_address, uint8_t const pins_mask, uint8_t const pins_value ) {
    reg_write( pins_value, base_address+GPIO_DATA + GPIO_GET_PINS_OFFSET( pins_mask ) );
}

void gpio_set_value( uint32_t const base_address, uint32_t const pin_offset, bool const value ) {
    TEST_ASSERT( pin_offset < GPIO_PIN_N, "ERROR!!! No such GPIO port exists" );

    gpio_set_value_by_mask( base_address, (0b1 << pin_offset), (value << pin_offset) );
}

bool gpio_get_value( uint32_t const base_address, uint32_t const pin_offset ) {
    TEST_ASSERT( pin_offset < GPIO_PIN_N, "ERROR!!! No such GPIO port exists" );

    return (bool)( reg_read( base_address+GPIO_DATA + GPIO_GET_PINS_OFFSET(0b1 << pin_offset) ) >> pin_offset );
}

uint8_t gpio_get_data( uint32_t const base_address ) {
    return reg_read( base_address+GPIO_DATA + GPIO_ADDR_MASK );
}
