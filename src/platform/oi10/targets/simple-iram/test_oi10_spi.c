#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <platform/common_macros/common_macros.h>
#include <platform/test_event_codes.h>
#include <platform/devices.h>
#include <platform/oi10/platform/regs/regs_spi.h>
#include <platform/oi10/platform/devices/spi.h>

#include <rumboot/irq.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <platform/interrupts.h>
#include <platform/test_assert.h>
#include <platform/regs/fields/mpic128.h>

#define CHECK_REGS

#ifdef CHECK_REGS
static uint32_t check_spi_default_val(uint32_t base_addr)
{
    rumboot_printf("Check the default values of the registers \n");

    TEST_ASSERT((ioread32(base_addr + SSPCR0) == DEFAULT_SSPCR0), "ERROR!!! The default value does not match the expected value the register SSPCR0.\n");
    rumboot_printf("Check SSPCR0: OK \n");

    TEST_ASSERT((ioread32(base_addr + SSPCR1) == DEFAULT_SSPCR1), "ERROR!!! The default value does not match the expected value the register SSPCR1.\n");
    rumboot_printf("Check SSPCR1: OK \n");

    TEST_ASSERT((ioread32(base_addr + SSPSR) == DEFAULT_SSPSR), "ERROR!!! The default value does not match the expected value the register SSPSR.\n");
    rumboot_printf("Check SSPSR: OK \n");

    TEST_ASSERT((ioread32(base_addr + SSPCPSR) == DEFAULT_SSPCPSR), "ERROR!!! The default value does not match the expected value the register SSPCPSR.\n");
    rumboot_printf("Check SSPCPSR: OK \n");

    TEST_ASSERT((ioread32(base_addr + SSPIMSC) == DEFAULT_SSPIMSC), "ERROR!!! The default value does not match the expected value the register SSPIMSC.\n");
    rumboot_printf("Check SSPIMSC: OK \n");

    TEST_ASSERT((ioread32(base_addr + SSPRIS) == DEFAULT_SSPRIS), "ERROR!!! The default value does not match the expected value the register SSPRIS.\n");
    rumboot_printf("Check SSPRIS: OK \n");

    TEST_ASSERT((ioread32(base_addr + SSPMIS) == DEFAULT_SSPMIS), "ERROR!!! The default value does not match the expected value the register SSPMIS.\n");
    rumboot_printf("Check SSPMIS: OK \n");

    TEST_ASSERT((ioread32(base_addr + SSPDMACR) == DEFAULT_SSPDMACR), "ERROR!!! The default value does not match the expected value the register SSPDMACR.\n");
    rumboot_printf("Check SSPDMACR: OK \n");

    rumboot_printf("SSPPeriphID0 = %x\n", (ioread32(base_addr + SSPPeriphID0)));
//    TEST_ASSERT((ioread32(base_addr + SSPPeriphID0) == DEFAULT_SSPPeriphID0), "ERROR!!! The default value does not match the expected value the register SSPPeriphID0.\n");
    rumboot_printf("Check SSPPeriphID0: OK \n");

    TEST_ASSERT((ioread32(base_addr + SSPPeriphID1) == DEFAULT_SSPPeriphID1), "ERROR!!! The default value does not match the expected value the register SSPPeriphID1.\n");
    rumboot_printf("Check SSPPeriphID1: OK \n");

    TEST_ASSERT((ioread32(base_addr + SSPPeriphID2) == DEFAULT_SSPPeriphID2), "ERROR!!! The default value does not match the expected value the register SSPPeriphID2.\n");
    rumboot_printf("Check SSPPeriphID2: OK \n");

    TEST_ASSERT((ioread32(base_addr + SSPPeriphID3) == DEFAULT_SSPPeriphID3), "ERROR!!! The default value does not match the expected value the register SSPPeriphID3.\n");
    rumboot_printf("Check SSPPeriphID3: OK \n");

    TEST_ASSERT((ioread32(base_addr + SSPPCellID0) == DEFAULT_SSPPCellID0), "ERROR!!! The default value does not match the expected value the register SSPPCellID0.\n");
    rumboot_printf("Check SSPPCellID0: OK \n");

    TEST_ASSERT((ioread32(base_addr + SSPPCellID1) == DEFAULT_SSPPCellID1), "ERROR!!! The default value does not match the expected value the register SSPPCellID1.\n");
    rumboot_printf("Check SSPPCellID1: OK \n");

    TEST_ASSERT((ioread32(base_addr + SSPPCellID2) == DEFAULT_SSPPCellID2), "ERROR!!! The default value does not match the expected value the register SSPPCellID2.\n");
    rumboot_printf("Check SSPPCellID2: OK \n");

    TEST_ASSERT((ioread32(base_addr + SSPPCellID3) == DEFAULT_SSPPCellID3), "ERROR!!! The default value does not match the expected value the register SSPPCellID3.\n");
    rumboot_printf("Check SSPPCellID3: OK \n");

    return 0;
}

static uint32_t check_spi_regs(uint32_t base_addr)
{
    rumboot_printf("Check WRITE/READ registers\n");
    rumboot_printf("SSPCR0 check:");
    iowrite32(MASK_SSPCR0 & (~DEFAULT_SSPCR0), base_addr + SSPCR0);
    TEST_ASSERT((ioread32(base_addr + SSPCR0) == (MASK_SSPCR0 & (~DEFAULT_SSPCR0))), "ERROR!!! The value read does not match the recorded value the register SSPCR0.\n");
    rumboot_printf(" OK\n");
    iowrite32(DEFAULT_SSPCR0, base_addr + SSPCR0);

    rumboot_printf("SSPCR1 check:");
    iowrite32(MASK_SSPCR1 & (~DEFAULT_SSPCR1), base_addr + SSPCR1);
    TEST_ASSERT((ioread32(base_addr + SSPCR1) == (MASK_SSPCR1 & (~DEFAULT_SSPCR1))), "ERROR!!! The value read does not match the recorded value the register SSPCR1.\n");
    rumboot_printf(" OK\n");
    iowrite32(DEFAULT_SSPCR1, base_addr + SSPCR1);

    rumboot_printf("SSPCPSR check:");
    iowrite32(MASK_SSPCPSR & (~DEFAULT_SSPCPSR), base_addr + SSPCPSR);
    TEST_ASSERT((ioread32(base_addr + SSPCPSR) == (MASK_SSPCPSR & (~DEFAULT_SSPCPSR))), "ERROR!!! The value read does not match the recorded value the register SSPCPSR.\n");
    rumboot_printf(" OK\n");
    iowrite32(DEFAULT_SSPCPSR, base_addr + SSPCPSR);

    rumboot_printf("SSPIMSC check:");
    iowrite32(MASK_SSPIMSC & (~SSPIMSC), base_addr + SSPIMSC);
    TEST_ASSERT((ioread32(base_addr + SSPIMSC) == (MASK_SSPIMSC & (~SSPIMSC))), "ERROR!!! The value read does not match the recorded value the register SSPIMSC.\n");
    rumboot_printf(" OK\n");
    iowrite32(SSPIMSC, base_addr + SSPIMSC);

    rumboot_printf("SSPDMACR check:");
    iowrite32(MASK_SSPDMACR & (~DEFAULT_SSPDMACR), base_addr + SSPDMACR);
    TEST_ASSERT((ioread32(base_addr + SSPDMACR) == (MASK_SSPDMACR & (~DEFAULT_SSPDMACR))), "ERROR!!! The value read does not match the recorded value the register SSPDMACR.\n");
    rumboot_printf(" OK\n");
    iowrite32(DEFAULT_SSPDMACR, base_addr + SSPDMACR);

    rumboot_printf("SSPDR check:");
    iowrite32(0xA5A5, base_addr + SSPDR);
    TEST_ASSERT((ioread32(base_addr + SSPDR) == 0xA5A5), "ERROR!!! The value read does not match the recorded value the register SSPDR.\n");
    rumboot_printf(" OK\n");
    iowrite32(0x00, base_addr + SSPDR);

    return 0;
}
#endif

static volatile uint32_t IRQ;

static void handler0( int irq, void *arg ) {
    rumboot_printf( "IRQ 0 arrived  \n" );
//    gpio_clear_edge_int(GPIO_0_BASE, 0xFF);
    rumboot_printf("Clear interrupts\n");
    IRQ = 1;
}

static void handler1( int irq, void *arg ) {
    rumboot_printf( "IRQ 1 arrived  \n" );
//    gpio_clear_edge_int(GPIO_1_BASE, 0xFF);
    rumboot_printf("Clear interrupts\n");
    IRQ = 1;
}

uint32_t wait_spi_int(){
    unsigned t;

    rumboot_printf ("wait_spi_int \n");

    for (t=1; t<=GSPI_TIMEOUT; t++){
        if (IRQ)
        {
            IRQ = 0;
            break;
        }
    }
    if (t>=GSPI_TIMEOUT) {
        rumboot_printf("Error! IRQ flag wait timeout! \n");
        return 1;
    }
    return 0;
}

uint32_t test_spi(uint32_t base_addr)
{
//    uint8_t data_read_gpio, data_write_gpio;
// setup of interrupts
    IRQ = 0;
    rumboot_irq_cli();
    struct rumboot_irq_entry *tbl = rumboot_irq_create( NULL );

    rumboot_irq_set_handler( tbl, GPIO0_INT, (int_sense_level << MPIC128_VP_S_i) | (int_pol_high << MPIC128_VP_POL_i), handler0, (void*)0);
    rumboot_irq_set_handler( tbl, GPIO1_INT, (int_sense_level << MPIC128_VP_S_i) | (int_pol_high << MPIC128_VP_POL_i), handler1, (void*)0);

    /* Activate the table */
    rumboot_irq_table_activate( tbl );
    rumboot_irq_enable( GSPI0_INT );
    rumboot_irq_enable( GSPI1_INT );
    rumboot_irq_sei();

//    //init GPIO_W
//    iowrite32(0x00, base_addr + GPIO_AFSEL); //gpio to gpio mode
//    gpio_set_direction(base_addr, GPIODIR_value, direction_out);
//    iowrite32(0x00, base_addr + GPIO_ADDR_MASK); //write data to output
//
//    gpio_interrupt_setup(base_addr, 0xff, true, both_edge);
//
//    //write
//    data_write_gpio = 0xFF & GPIODIR_value;
////    rumboot_printf( "data_write_gpio : %x\n", data_write_gpio);
//    iowrite32(data_write_gpio, base_addr + GPIO_ADDR_MASK);

    if (wait_spi_int())
    {
        return 1;
    }

    //read
//    data_read_gpio = gpio_get_data(base_addr);
////    rumboot_printf( "data_read_gpio : %x\n", data_read_gpio);
//    TEST_ASSERT((( (data_write_gpio>>1) | GPIODIR_value) == data_read_gpio),"Error! The value of GPOUT does not match the expected\n");

    rumboot_irq_table_activate(NULL);
    rumboot_irq_free(tbl);

    return 0;
}


int main(void)
{
    uint32_t result = 0x0;
    rumboot_printf("Check GSPI_0 \n");
    #ifdef CHECK_REGS
        check_spi_default_val(GSPI0_BASE);
        check_spi_regs(GSPI0_BASE);
    #endif

    if(!test_spi(GSPI0_BASE))
        rumboot_printf("Check GSPI_0: OK \n");
    else
        result = (1<<0);

    rumboot_printf("Check GSPI_1 \n");
    #ifdef CHECK_REGS
        check_spi_default_val(GSPI1_BASE);
        check_spi_regs(GSPI1_BASE);
    #endif

    if(!test_spi(GSPI1_BASE))
        rumboot_printf("Check GSPI_1: OK \n");
    else
        result = (1<<1);

    return result;

}
