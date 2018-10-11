#include <stdint.h>
#include <stdlib.h>

#include <platform/test_assert.h>
#include <platform/common_macros/common_macros.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/timer.h>
#include <devices/gpio_pl061.h>
#include <regs/regs_gpio_pl061.h>


static void reg_write( uint32_t base, uint32_t reg_offset, uint32_t value ) {
    iowrite32( value, base + reg_offset );
}

static void reg_set( uint32_t base, uint32_t reg_offset, uint32_t value ) {
    uint32_t reg = ioread32( base + reg_offset );

    reg |= ( value );
    iowrite32( reg, base + reg_offset );
}

static void reg_clear( uint32_t base, uint32_t reg_offset, uint32_t value ) {
    uint32_t reg = ioread32( base + reg_offset );

    reg &= ~( value );
    iowrite32( reg, base + reg_offset );
}

void gpio_int_enable( uint32_t base_address, uint32_t pin_offset, gpio_int_type int_type ){
    reg_set( base_address, GPIO_IE, pin_offset );

    switch( int_type ) {
    case level:
        reg_set( base_address, GPIO_IS, pin_offset );
        break;
    case rising_edge:
        //set edge triggered
        reg_clear( base_address, GPIO_IS, pin_offset );
        //disable both edges
        reg_clear( base_address, GPIO_IBE, pin_offset );
        //setup rising edge
        reg_set( base_address, GPIO_IEV, pin_offset );
        break;
    case falling_edge:
        //set edge triggered
        reg_clear( base_address, GPIO_IS, pin_offset );
        //disable both edges
        reg_clear( base_address, GPIO_IBE, pin_offset );
        //setup falling edge
        reg_clear( base_address, GPIO_IEV, pin_offset );
        break;
    case both_edge:
        //set edge triggered
        reg_clear( base_address, GPIO_IS, pin_offset );
        //enable both edges
        reg_set( base_address, GPIO_IBE, pin_offset );
        break;
    default:
        rumboot_putstring( "unsupported interrupt type\n" );
        break;
    }
}
void gpio_int_disable( uint32_t base_address, uint32_t pin_offset ){
    TEST_ASSERT(pin_offset < (1 << 8), "ERROR!!! No such GPIO port exists");
    reg_clear( base_address, GPIO_IE, pin_offset );
}


void gpio_int_clear( uint32_t base_address, uint32_t pin_offset ){
    TEST_ASSERT(pin_offset < (1 << 8), "ERROR!!! No such GPIO port exists");
    reg_write( base_address, GPIO_IC, pin_offset );
}

void gpio_set_direction( uint32_t base_address, uint32_t pin_offset, gpio_pin_direction dir ){
    TEST_ASSERT(pin_offset < (1 << 8), "ERROR!!! No such GPIO port exists");
    if( dir == direction_out )
        reg_set( base_address, GPIO_DIR, pin_offset );
    else //direction_in
    reg_clear( base_address, GPIO_DIR, pin_offset );
}

gpio_pin_direction gpio_get_direction( uint32_t base_address,  uint32_t pin_offset ){
     TEST_ASSERT(pin_offset < (1 << 8), "ERROR!!! No such GPIO port exists");
     gpio_pin_direction dir = pin_offset && ioread32( base_address + GPIO_DIR);
     return dir;
}

bool gpio_get_value( uint32_t base_address, uint32_t pin_offset ){
    bool raw_val;
    TEST_ASSERT(pin_offset < (1 << 8), "ERROR!!! No such GPIO port exists");
    raw_val = *( volatile bool* )( base_address + ( 1 << ( pin_offset + 2 ) ) );
    return ( raw_val >> pin_offset );
}

void gpio_set_value( uint32_t base_address, uint32_t pin_offset, bool value ){
    TEST_ASSERT(pin_offset < (1 << 8), "ERROR!!! No such GPIO port exists");
    *( volatile bool* )( base_address + ( 1 << ( pin_offset + 2 ) ) ) = ( value << pin_offset );

}

//return 8bit
uint8_t gpio_get_data( uint32_t base_address ) {
    return *( volatile uint8_t* )( base_address + GPIO_ADDR_MASK );
}
