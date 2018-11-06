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


static inline __attribute__((always_inline)) void ioset32( uint32_t const value, uint32_t const address ) {
    iowrite32( (ioread32( address ) & GPIO_REG_MASK) | value, address );
}

static inline __attribute__((always_inline)) void ioclear32( uint32_t const value, uint32_t const address ) {
    iowrite32( (ioread32( address ) & GPIO_REG_MASK) & ~value, address );
}


void gpio_int_enable_by_mask( uint32_t const base_address, uint8_t const pins_mask, gpio_int_type const int_type ) {
    switch( int_type ) {
    case level:
        ioset32( pins_mask, base_address+GPIO_IS );     /* set level triggered */
        break;
    case rising_edge:
        ioclear32( pins_mask, base_address+GPIO_IS );   /* set edge triggered */
        ioclear32( pins_mask, base_address+GPIO_IBE );  /* disable both edges */
        ioset32( pins_mask, base_address+GPIO_IEV );    /* setup rising edge */
        break;
    case falling_edge:
        ioclear32( pins_mask, base_address+GPIO_IS );   /* set edge triggered */
        ioclear32( pins_mask, base_address+GPIO_IBE );  /* disable both edges */
        ioclear32( pins_mask, base_address+GPIO_IEV );  /* setup falling edge */
        break;
    case both_edge:
        ioclear32( pins_mask, base_address+GPIO_IS );   /* set edge triggered */
        ioset32( pins_mask, base_address+GPIO_IBE );    /* enable both edges */
        break;
    default:
        TEST_ASSERT( 0, "Unsupported interrupt type" );
        return;
    }

    ioset32( pins_mask, base_address+GPIO_IE );         /* enable interrupt */
}

void gpio_int_enable( uint32_t const base_address, uint32_t const pin_offset, gpio_int_type const int_type ) {
    TEST_ASSERT( pin_offset < GPIO_PIN_N, "ERROR!!! No such GPIO port exists" );

    gpio_int_enable_by_mask( base_address, (0b1 << pin_offset), int_type );
}


void gpio_int_disable_by_mask( uint32_t const base_address, uint8_t const pins_mask ) {
    ioclear32( pins_mask, base_address+GPIO_IE );
}

void gpio_int_disable( uint32_t const base_address, uint32_t const pin_offset ) {
    TEST_ASSERT( pin_offset < GPIO_PIN_N, "ERROR!!! No such GPIO port exists" );

    gpio_int_disable_by_mask( base_address, (0b1 << pin_offset) );
}


void gpio_int_clear_by_mask( uint32_t const base_address, uint8_t const pins_mask ) {
    iowrite32( pins_mask, base_address+GPIO_IC );
}

void gpio_int_clear( uint32_t const base_address, uint32_t const pin_offset ) {
    TEST_ASSERT( pin_offset < GPIO_PIN_N, "ERROR!!! No such GPIO port exists" );

    gpio_int_clear_by_mask( base_address, (0b1 << pin_offset) );
}


void gpio_set_direction_by_mask( uint32_t const base_address, uint8_t const pins_mask, gpio_pin_direction const dir ) {
    if( dir == direction_out )  ioset32( pins_mask, base_address+GPIO_DIR );
    else /* direction_in */     ioclear32( pins_mask, base_address+GPIO_DIR );
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
    if( ctrl_mode == software_mode )    ioclear32( pins_mask, base_address+GPIO_AFSEL );
    else /* hardware_mode */            ioset32( pins_mask, base_address+GPIO_AFSEL );
}

void gpio_set_ctrl_mode( uint32_t const base_address, uint32_t const pin_offset, gpio_ctrl_mode const ctrl_mode ) {
    TEST_ASSERT( pin_offset < GPIO_PIN_N, "ERROR!!! No such GPIO port exists" );

    gpio_set_ctrl_mode_by_mask( base_address, (0b1 << pin_offset), ctrl_mode );
}

gpio_ctrl_mode gpio_get_ctrl_mode( uint32_t const base_address,  uint32_t const pin_offset ) {
    TEST_ASSERT( pin_offset < GPIO_PIN_N, "ERROR!!! No such GPIO port exists" );

    return (gpio_ctrl_mode)( (0b1 << pin_offset) & (ioread32( base_address+GPIO_AFSEL ) & GPIO_REG_MASK) );
}


void gpio_set_value_by_mask( uint32_t const base_address, uint8_t const pins_mask, uint8_t const pins_value ) {
    iowrite32( pins_value, base_address+GPIO_DATA + GPIO_GET_PINS_OFFSET( pins_mask ) );
}

void gpio_set_value( uint32_t const base_address, uint32_t const pin_offset, bool const value ) {
    TEST_ASSERT( pin_offset < GPIO_PIN_N, "ERROR!!! No such GPIO port exists" );

    gpio_set_value_by_mask( base_address, (0b1 << pin_offset), (value << pin_offset) );
}

bool gpio_get_value( uint32_t const base_address, uint32_t const pin_offset ) {
    TEST_ASSERT( pin_offset < GPIO_PIN_N, "ERROR!!! No such GPIO port exists" );

    return (bool)( (ioread32( base_address+GPIO_DATA + GPIO_GET_PINS_OFFSET(0b1 << pin_offset) ) & GPIO_REG_MASK) >> pin_offset );
}

uint8_t gpio_get_data( uint32_t const base_address ) {
    return (uint8_t)( ioread32( base_address+GPIO_DATA + GPIO_ADDR_MASK ) & GPIO_REG_MASK );
}
