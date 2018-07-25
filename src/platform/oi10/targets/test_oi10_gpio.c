#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <platform/common_macros/common_macros.h>
#include <platform/test_event_codes.h>
#include <platform/devices.h>
#include <rumboot/irq.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <platform/interrupts.h>
#include <platform/test_assert.h>
#include <rumboot/regpoker.h>
#include <platform/regs/fields/mpic128.h>
#include <regs/regs_gpio-pl061.h>
#include <devices/gpio-pl061.h>


#ifdef CHECK_REGS
static uint32_t check_gpio_default_val( uint32_t base_addr ) {
    rumboot_printf("Check the default values of the registers:");

    struct regpoker_checker check_default_array[] = {
        {   "GPIO_DATA",        REGPOKER_READ32,    base_addr+GPIO_DATA,        GPIO_DATA_DEFAULT,      GPIO_REG_MASK   }, /* check only first of 256 */
        {   "GPIO_DIR",         REGPOKER_READ32,    base_addr+GPIO_DIR,         GPIO_DIR_DEFAULT,       GPIO_REG_MASK   },
        {   "GPIO_IS",          REGPOKER_READ32,    base_addr+GPIO_IS,          GPIO_IS_DEFAULT,        GPIO_REG_MASK   },
        {   "GPIO_IBE",         REGPOKER_READ32,    base_addr+GPIO_IBE,         GPIO_IBE_DEFAULT,       GPIO_REG_MASK   },
        {   "GPIO_IEV",         REGPOKER_READ32,    base_addr+GPIO_IEV,         GPIO_IEV_DEFAULT,       GPIO_REG_MASK   },
        {   "GPIO_IE",          REGPOKER_READ32,    base_addr+GPIO_IE,          GPIO_IE_DEFAULT,        GPIO_REG_MASK   },
        {   "GPIO_RIS",         REGPOKER_READ32,    base_addr+GPIO_RIS,         GPIO_RIS_DEFAULT,       GPIO_REG_MASK   },
        {   "GPIO_MIS",         REGPOKER_READ32,    base_addr+GPIO_MIS,         GPIO_MIS_DEFAULT,       GPIO_REG_MASK   },
        {   "GPIO_AFSEL",       REGPOKER_READ32,    base_addr+GPIO_AFSEL,       GPIO_AFSEL_DEFAULT,     GPIO_REG_MASK   },
        {   "GPIO_PeriphID0",   REGPOKER_READ32,    base_addr+GPIO_PeriphID0,   GPIO_PeriphID0_DEFAULT, GPIO_REG_MASK   },
        {   "GPIO_PeriphID1",   REGPOKER_READ32,    base_addr+GPIO_PeriphID1,   GPIO_PeriphID1_DEFAULT, GPIO_REG_MASK   },
        {   "GPIO_PeriphID2",   REGPOKER_READ32,    base_addr+GPIO_PeriphID2,   GPIO_PeriphID2_DEFAULT, GPIO_REG_MASK   },
        {   "GPIO_PeriphID3",   REGPOKER_READ32,    base_addr+GPIO_PeriphID3,   GPIO_PeriphID3_DEFAULT, GPIO_REG_MASK   },
        {   "GPIO_CellID0",     REGPOKER_READ32,    base_addr+GPIO_CellID0,     GPIO_CellID0_DEFAULT,   GPIO_REG_MASK   },
        {   "GPIO_CellID1",     REGPOKER_READ32,    base_addr+GPIO_CellID1,     GPIO_CellID1_DEFAULT,   GPIO_REG_MASK   },
        {   "GPIO_CellID2",     REGPOKER_READ32,    base_addr+GPIO_CellID2,     GPIO_CellID2_DEFAULT,   GPIO_REG_MASK   },
        {   "GPIO_CellID3",     REGPOKER_READ32,    base_addr+GPIO_CellID3,     GPIO_CellID3_DEFAULT,   GPIO_REG_MASK   },
        { /* Sentinel */ }
    };

    if( rumboot_regpoker_check_array( check_default_array, 0 ) == 0 ) {
        rumboot_printf( " OK\n" );
        return 0;
    }

    rumboot_printf( " ERROR\n" );
    return 1;
}

static uint32_t check_gpio_regs( uint32_t base_addr ) {
    rumboot_printf("Check WRITE/READ registers:");

    struct regpoker_checker check_rw_array[] = {
        {   "GPIO_DIR",         REGPOKER_WRITE32,   base_addr+GPIO_DIR,                 ~GPIO_DIR_DEFAULT,      GPIO_REG_MASK   },
        {   "GPIO_DATAFF",      REGPOKER_WRITE32,   base_addr+GPIO_DATA+GPIO_ADDR_MASK, GPIO_DATA_DEFAULT,      GPIO_REG_MASK   },
//        {   "GPIO_DATA00",      REGPOKER_WRITE32,   base_addr+GPIO_DATA,                GPIO_DATA_DEFAULT,      0x00            },
        {   "GPIO_DIR",         REGPOKER_WRITE32,   base_addr+GPIO_DIR,                 GPIO_DIR_DEFAULT,       0x00            },
        {   "GPIO_IS",          REGPOKER_WRITE32,   base_addr+GPIO_IS,                  GPIO_IS_DEFAULT,        GPIO_REG_MASK   },
        {   "GPIO_IBE",         REGPOKER_WRITE32,   base_addr+GPIO_IBE,                 GPIO_IBE_DEFAULT,       GPIO_REG_MASK   },
        {   "GPIO_IEV",         REGPOKER_WRITE32,   base_addr+GPIO_IEV,                 GPIO_IEV_DEFAULT,       GPIO_REG_MASK   },
        {   "GPIO_IE",          REGPOKER_WRITE32,   base_addr+GPIO_IE,                  GPIO_IE_DEFAULT,        GPIO_REG_MASK   },
        {   "GPIO_AFSEL",       REGPOKER_WRITE32,   base_addr+GPIO_AFSEL,               GPIO_AFSEL_DEFAULT,     GPIO_REG_MASK   },
        { /* Sentinel */ }
    };

    if( rumboot_regpoker_check_array( check_rw_array, 0 ) == 0 ) {
        rumboot_printf( " OK\n" );
        return 0;
    }

    rumboot_printf( " ERROR\n" );
    return 1;
}
#endif

static volatile uint32_t IRQ;

static void handler0( int irq, void *arg ) {
    rumboot_printf( "IRQ 0 arrived  \n" );
    gpio_clear_edge_int(GPIO_0_BASE, 0xFF);
    rumboot_printf("Clear interrupts\n");
    IRQ = 1;
}

static void handler1( int irq, void *arg ) {
    rumboot_printf( "IRQ 1 arrived  \n" );
    gpio_clear_edge_int(GPIO_1_BASE, 0xFF);
    rumboot_printf("Clear interrupts\n");
    IRQ = 1;
}

static uint32_t wait_gpio_int() {
    unsigned t;

    rumboot_printf ("wait_gpio_int \n");

    for (t=1; t<=GPIO_TIMEOUT; t++){
        if (IRQ)
        {
            IRQ = 0;
            break;
        }
    }
    if (t>=GPIO_TIMEOUT) {
        rumboot_printf("Error! IRQ flag wait timeout! \n");
        return 1;
    }
    return 0;
}

static uint32_t test_gpio( uint32_t base_addr, uint32_t GPIODIR_value ) {
    uint8_t data_read_gpio, data_write_gpio;
// setup of interrupts
    IRQ = 0;
    rumboot_irq_cli();
    struct rumboot_irq_entry *tbl = rumboot_irq_create( NULL );

    rumboot_irq_set_handler( tbl, GPIO0_INT, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler0, (void*)0);
    rumboot_irq_set_handler( tbl, GPIO1_INT, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler1, (void*)0);

    /* Activate the table */
    rumboot_irq_table_activate( tbl );
    rumboot_irq_enable( GPIO0_INT );
    rumboot_irq_enable( GPIO1_INT );
    rumboot_irq_sei();

    //init GPIO_W
    iowrite32(0x00, base_addr + GPIO_AFSEL); //gpio to gpio mode
    gpio_set_direction(base_addr, GPIODIR_value, direction_out);
    iowrite32(0x00, base_addr + GPIO_ADDR_MASK); //write data to output

    gpio_interrupt_setup(base_addr, 0xff, true, both_edge);

    //write
    data_write_gpio = 0xFF & GPIODIR_value;
//    rumboot_printf( "data_write_gpio : %x\n", data_write_gpio);
    iowrite32(data_write_gpio, base_addr + GPIO_ADDR_MASK);

    if (wait_gpio_int())
    {
        return 1;
    }

    //read
    data_read_gpio = gpio_get_data(base_addr);
//    rumboot_printf( "data_read_gpio : %x\n", data_read_gpio);
    TEST_ASSERT((( (data_write_gpio>>1) | GPIODIR_value) == data_read_gpio),"Error! The value of GPOUT does not match the expected\n");

    rumboot_irq_table_activate(NULL);
    rumboot_irq_free(tbl);

    return 0;
}


int main() {
    uint32_t result = 0x0;

    rumboot_printf( "Check GPIO (0x%x) \n", GPIO_X_BASE );

#ifdef CHECK_REGS
    result += check_gpio_default_val(GPIO_X_BASE);
    result += check_gpio_regs(GPIO_X_BASE);
#endif
    result += test_gpio(GPIO_X_BASE, 0x2A);

    return result;
}
