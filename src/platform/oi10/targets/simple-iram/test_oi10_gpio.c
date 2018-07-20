#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <platform/common_macros/common_macros.h>
#include <platform/test_event_codes.h>
#include <platform/devices.h>
#include <platform/oi10/platform/regs/regs_gpio.h>
#include <platform/oi10/platform/devices/gpio.h>

#include <rumboot/irq.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <platform/interrupts.h>
#include <platform/test_assert.h>
#include <platform/regs/fields/mpic128.h>

#define CHECK_REGS

#ifdef CHECK_REGS
static uint32_t check_gpio_default_val(uint32_t base_addr)
{
    rumboot_printf("Check the default values of the registers \n");

    TEST_ASSERT((ioread32(base_addr + GPIO_DATA) == GPIO_DATA_DEFAULT), "ERROR!!! The default value does not match the expected value the register GPIO_DATA.\n");
    rumboot_printf("Check GPIO_DATA: OK \n");

    TEST_ASSERT((ioread32(base_addr + GPIO_DIR) == GPIO_DIR_DEFAULT), "ERROR!!! The default value does not match the expected value the register GPIO_DIR.\n");
    rumboot_printf("Check GPIO_DIR: OK \n");

    TEST_ASSERT((ioread32(base_addr + GPIO_IS) == GPIO_IS_DEFAULT), "ERROR!!! The default value does not match the expected value the register GPIO_IS.\n");
    rumboot_printf("Check GPIO_IS: OK \n");

    TEST_ASSERT((ioread32(base_addr + GPIO_IBE) == GPIO_IBE_DEFAULT), "ERROR!!! The default value does not match the expected value the register GPIO_IBE.\n");
    rumboot_printf("Check GPIO_IBE: OK \n");

    TEST_ASSERT((ioread32(base_addr + GPIO_IEV) == GPIO_IEV_DEFAULT), "ERROR!!! The default value does not match the expected value the register GPIO_IEV.\n");
    rumboot_printf("Check GPIO_IEV: OK \n");

    TEST_ASSERT((ioread32(base_addr + GPIO_IE) == GPIO_IE_DEFAULT), "ERROR!!! The default value does not match the expected value the register GPIO_IE.\n");
    rumboot_printf("Check GPIO_IE: OK \n");

    TEST_ASSERT((ioread32(base_addr + GPIO_RIS) == GPIO_RIS_DEFAULT), "ERROR!!! The default value does not match the expected value the register GPIO_RIS.\n");
    rumboot_printf("Check GPIO_RIS: OK \n");

    TEST_ASSERT((ioread32(base_addr + GPIO_MIS) == GPIO_MIS_DEFAULT), "ERROR!!! The default value does not match the expected value the register GPIO_MIS.\n");
    rumboot_printf("Check GPIO_MIS: OK \n");

    TEST_ASSERT((ioread32(base_addr + GPIO_AFSEL) == GPIO_AFSEL_DEFAULT), "ERROR!!! The default value does not match the expected value the register GPIO_AFSEL.\n");
    rumboot_printf("Check GPIO_AFSEL: OK \n");

    TEST_ASSERT((ioread32(base_addr + GPIO_PeriphID0) == GPIO_PeriphID0_DEFAULT), "ERROR!!! The default value does not match the expected value the register GPIO_PeriphID0.\n");
    rumboot_printf("Check GPIO_PeriphID0: OK \n");

    TEST_ASSERT((ioread32(base_addr + GPIO_PeriphID1) == GPIO_PeriphID1_DEFAULT), "ERROR!!! The default value does not match the expected value the register GPIO_PeriphID1.\n");
    rumboot_printf("Check GPIO_PeriphID1: OK \n");

    TEST_ASSERT((ioread32(base_addr + GPIO_PeriphID2) == GPIO_PeriphID2_DEFAULT), "ERROR!!! The default value does not match the expected value the register GPIO_PeriphID2.\n");
    rumboot_printf("Check GPIO_PeriphID2: OK \n");

    TEST_ASSERT((ioread32(base_addr + GPIO_PeriphID3) == GPIO_PeriphID3_DEFAULT), "ERROR!!! The default value does not match the expected value the register GPIO_PeriphID3.\n");
    rumboot_printf("Check GPIO_PeriphID3: OK \n");

    TEST_ASSERT((ioread32(base_addr + GPIO_CellID0) == GPIO_CellID0_DEFAULT), "ERROR!!! The default value does not match the expected value the register GPIO_CellID0.\n");
    rumboot_printf("Check GPIO_CellID0: OK \n");

    TEST_ASSERT((ioread32(base_addr + GPIO_CellID1) == GPIO_CellID1_DEFAULT), "ERROR!!! The default value does not match the expected value the register GPIO_CellID1.\n");
    rumboot_printf("Check GPIO_CellID1: OK \n");

    TEST_ASSERT((ioread32(base_addr + GPIO_CellID2) == GPIO_CellID2_DEFAULT), "ERROR!!! The default value does not match the expected value the register GPIO_CellID2.\n");
    rumboot_printf("Check GPIO_CellID2: OK \n");

    TEST_ASSERT((ioread32(base_addr + GPIO_CellID3) == GPIO_CellID3_DEFAULT), "ERROR!!! The default value does not match the expected value the register GPIO_CellID3.\n");
    rumboot_printf("Check GPIO_CellID3: OK \n");

    return 0;
}

static uint32_t check_gpio_regs(uint32_t base_addr)
{
    rumboot_printf("Check WRITE/READ registers\n");

    rumboot_printf("GPIO_DIR check:");
    iowrite32(GPIO_REG_MASK & (~GPIO_DIR_DEFAULT), base_addr + GPIO_DIR);
    TEST_ASSERT((ioread32(base_addr + GPIO_DIR) == (GPIO_REG_MASK & (~GPIO_DIR_DEFAULT))), "ERROR!!! The value read does not match the recorded value the register GPIO_DIR.\n");
    rumboot_printf(" OK\n");

    rumboot_printf("GPIO_DATA check:");
    iowrite32(GPIO_REG_MASK & (~GPIO_DATA_DEFAULT), base_addr + GPIO_DATA + GPIO_ADDR_MASK);
    TEST_ASSERT((ioread32(base_addr + GPIO_DATA + GPIO_ADDR_MASK) == (GPIO_REG_MASK & (~GPIO_DATA_DEFAULT))), "ERROR!!! The value read does not match the recorded value the register GPIO_DATA.\n");
    rumboot_printf(" OK\n");
    iowrite32(GPIO_DATA_DEFAULT, base_addr + GPIO_DATA);
    iowrite32(GPIO_DIR_DEFAULT, base_addr + GPIO_DIR);

    rumboot_printf("GPIO_IS check:");
    iowrite32(GPIO_REG_MASK & (~GPIO_IS_DEFAULT), base_addr + GPIO_IS);
    TEST_ASSERT((ioread32(base_addr + GPIO_IS) == (GPIO_REG_MASK & (~GPIO_IS_DEFAULT))), "ERROR!!! The value read does not match the recorded value the register GPIO_IS.\n");
    rumboot_printf(" OK\n");
    iowrite32(GPIO_IS_DEFAULT, base_addr + GPIO_IS);

    rumboot_printf("GPIO_IBE check:");
    iowrite32(GPIO_REG_MASK & (~GPIO_IBE_DEFAULT), base_addr + GPIO_IBE);
    TEST_ASSERT((ioread32(base_addr + GPIO_IBE) == (GPIO_REG_MASK & (~GPIO_IBE_DEFAULT))), "ERROR!!! The value read does not match the recorded value the register GPIO_IBE.\n");
    rumboot_printf(" OK\n");
    iowrite32(GPIO_IBE_DEFAULT, base_addr + GPIO_IBE);

    rumboot_printf("GPIO_IEV check:");
    iowrite32(GPIO_REG_MASK & (~GPIO_IEV_DEFAULT), base_addr + GPIO_IEV);
    TEST_ASSERT((ioread32(base_addr + GPIO_IEV) == (GPIO_REG_MASK & (~GPIO_IEV_DEFAULT))), "ERROR!!! The value read does not match the recorded value the register GPIO_IEV.\n");
    rumboot_printf(" OK\n");
    iowrite32(GPIO_IEV_DEFAULT, base_addr + GPIO_IEV);

    rumboot_printf("GPIO_IE check:");
    iowrite32(GPIO_REG_MASK & (~GPIO_IE_DEFAULT), base_addr + GPIO_IE);
    TEST_ASSERT((ioread32(base_addr + GPIO_IE) == (GPIO_REG_MASK & (~GPIO_IE_DEFAULT))), "ERROR!!! The value read does not match the recorded value the register GPIO_IE.\n");
    rumboot_printf(" OK\n");
    iowrite32(GPIO_IE_DEFAULT, base_addr + GPIO_IE);

    rumboot_printf("GPIO_AFSEL check:");
    iowrite32(GPIO_REG_MASK & (~GPIO_AFSEL_DEFAULT), base_addr + GPIO_AFSEL);
    TEST_ASSERT((ioread32(base_addr + GPIO_AFSEL) == (GPIO_REG_MASK & (~GPIO_AFSEL_DEFAULT))), "ERROR!!! The value read does not match the recorded value the register GPIO_AFSEL.\n");
    rumboot_printf(" OK\n");
    iowrite32(GPIO_AFSEL_DEFAULT, base_addr + GPIO_AFSEL);

    return 0;
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

uint32_t wait_gpio_int(){
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

uint32_t test_gpio(uint32_t base_addr, uint32_t GPIODIR_value)
{
    uint8_t data_read_gpio, data_write_gpio;
// setup of interrupts
    IRQ = 0;
    rumboot_irq_cli();
    struct rumboot_irq_entry *tbl = rumboot_irq_create( NULL );

    rumboot_irq_set_handler( tbl, GPIO0_INT, (int_sense_level << MPIC128_VP_S_i) | (int_pol_high << MPIC128_VP_POL_i), handler0, (void*)0);
    rumboot_irq_set_handler( tbl, GPIO1_INT, (int_sense_level << MPIC128_VP_S_i) | (int_pol_high << MPIC128_VP_POL_i), handler1, (void*)0);

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


int main(void)
{
    uint32_t result = 0x0;
    rumboot_printf("Check GPIO_0 \n");
    #ifdef CHECK_REGS
        check_gpio_default_val(GPIO_0_BASE);
        check_gpio_regs(GPIO_0_BASE);
    #endif

    if(!test_gpio(GPIO_0_BASE, 0x2A))
        rumboot_printf("Check GPIO_0: OK \n");
    else
        result = (1<<0);

    rumboot_printf("Check GPIO_1 \n");
    #ifdef CHECK_REGS
        check_gpio_default_val(GPIO_1_BASE);
        check_gpio_regs(GPIO_1_BASE);
    #endif

    if(!test_gpio(GPIO_1_BASE, 0xAA))
        rumboot_printf("Check GPIO_1: OK \n");
    else
        result = (1<<1);

    return result;

}
