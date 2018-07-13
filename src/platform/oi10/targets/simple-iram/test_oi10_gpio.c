#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <platform/common_macros/common_macros.h>
#include <platform/test_event_codes.h>
#include <platform/devices.h>
#include <platform/oi10/platform/regs/regs_gpio.h>

#include <rumboot/irq.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <platform/interrupts.h>
#include <platform/regs/fields/mpic128.h>

static uint32_t check_gpio_default_val(uint32_t base_addr)
{
    uint32_t result = 0x0;
    rumboot_printf("Check the default values of the registers \n");

    if (ioread32(base_addr + GPIO_DIR) != GPIO_DIR_DEFAULT)
    {
        rumboot_printf("ERROR!!! The default value does not match the expected valuen the register GPIO_DIR.\n");
        rumboot_printf("expect = %x \n", GPIO_DIR_DEFAULT);
        rumboot_printf("actual = %x \n", ioread32(base_addr + GPIO_DIR));
        result |= (1<<1);
    } else
        rumboot_printf("Check GPIO_DIR: OK \n");

    iowrite32(GPIO_REG_MASK & (~GPIO_DIR_DEFAULT), base_addr + GPIO_DIR);
    if (ioread32(base_addr + GPIO_DATA) != GPIO_DATA_DEFAULT)
    {
        rumboot_printf("ERROR!!! The default value does not match the expected value in the register GPIO_DATA.\n");
        rumboot_printf("expect = %x \n", GPIO_DATA_DEFAULT);
        rumboot_printf("actual = %x \n", ioread32(base_addr + GPIO_DATA));
        result |= (1<<0);
    } else
        rumboot_printf("Check GPIO_DATA: OK \n");

    if (ioread32(base_addr + GPIO_RIS) != GPIO_RIS_DEFAULT)
    {
        rumboot_printf("ERROR!!! The default value does not match the expected valuen the register GPIO_RIS.\n");
        rumboot_printf("expect = %x \n", GPIO_RIS_DEFAULT);
        rumboot_printf("actual = %x \n", ioread32(base_addr + GPIO_RIS));
        result |= (1<<6);
    } else
        rumboot_printf("Check GPIO_RIS: OK \n");
    iowrite32(GPIO_DIR_DEFAULT, base_addr + GPIO_DIR);


    if (ioread32(base_addr + GPIO_IS) != GPIO_IS_DEFAULT)
    {
        rumboot_printf("ERROR!!! The default value does not match the expected valuen the register GPIO_IS.\n");
        rumboot_printf("expect = %x \n", GPIO_IS_DEFAULT);
        rumboot_printf("actual = %x \n", ioread32(base_addr + GPIO_IS));
        result |= (1<<2);
    } else
        rumboot_printf("Check GPIO_IS: OK \n");

    if (ioread32(base_addr + GPIO_IBE) != GPIO_IBE_DEFAULT)
    {
        rumboot_printf("ERROR!!! The default value does not match the expected valuen the register GPIO_IBE.\n");
        rumboot_printf("expect = %x \n", GPIO_IBE_DEFAULT);
        rumboot_printf("actual = %x \n", ioread32(base_addr + GPIO_IBE));
        result |= (1<<3);
    } else
        rumboot_printf("Check GPIO_IBE: OK \n");

    if (ioread32(base_addr + GPIO_IEV) != GPIO_IEV_DEFAULT)
    {
        rumboot_printf("ERROR!!! The default value does not match the expected valuen the register GPIO_IEV.\n");
        rumboot_printf("expect = %x \n", GPIO_IEV_DEFAULT);
        rumboot_printf("actual = %x \n", ioread32(base_addr + GPIO_IEV));
        result |= (1<<4);
    } else
        rumboot_printf("Check GPIO_IEV: OK \n");

    if (ioread32(base_addr + GPIO_IE) != GPIO_IE_DEFAULT)
    {
        rumboot_printf("ERROR!!! The default value does not match the expected valuen the register GPIO_IE.\n");
        rumboot_printf("expect = %x \n", GPIO_IE_DEFAULT);
        rumboot_printf("actual = %x \n", ioread32(base_addr + GPIO_IE));
        result |= (1<<5);
    } else
        rumboot_printf("Check GPIO_IE: OK \n");

    if (ioread32(base_addr + GPIO_MIS) != GPIO_MIS_DEFAULT)
    {
        rumboot_printf("ERROR!!! The default value does not match the expected valuen the register GPIO_MIS.\n");
        rumboot_printf("expect = %x \n", GPIO_MIS_DEFAULT);
        rumboot_printf("actual = %x \n", ioread32(base_addr + GPIO_MIS));
        result |= (1<<7);
    } else
        rumboot_printf("Check GPIO_MIS: OK \n");

    if (ioread32(base_addr + GPIO_AFSEL) != GPIO_AFSEL_DEFAULT)
    {
        rumboot_printf("ERROR!!! The default value does not match the expected valuen the register GPIO_AFSEL.\n");
        rumboot_printf("expect = %x \n", GPIO_AFSEL_DEFAULT);
        rumboot_printf("actual = %x \n", ioread32(base_addr + GPIO_AFSEL));
        result |= (1<<9);
    } else
        rumboot_printf("Check GPIO_AFSEL: OK \n");

    if (ioread32(base_addr + GPIO_PeriphID0) != GPIO_PeriphID0_DEFAULT)
    {
        rumboot_printf("ERROR!!! The default value does not match the expected valuen the register GPIO_PeriphID0.\n");
        rumboot_printf("expect = %x \n", GPIO_PeriphID0_DEFAULT);
        rumboot_printf("actual = %x \n", ioread32(base_addr + GPIO_PeriphID0));
        result |= (1<<10);
    } else
        rumboot_printf("Check GPIO_PeriphID0: OK \n");

    if (ioread32(base_addr + GPIO_PeriphID1) != GPIO_PeriphID1_DEFAULT)
    {
        rumboot_printf("ERROR!!! The default value does not match the expected valuen the register GPIO_PeriphID1.\n");
        rumboot_printf("expect = %x \n", GPIO_PeriphID1_DEFAULT);
        rumboot_printf("actual = %x \n", ioread32(base_addr + GPIO_PeriphID1));
        result |= (1<<11);
    } else
        rumboot_printf("Check GPIO_PeriphID1: OK \n");

    if (ioread32(base_addr + GPIO_PeriphID2) != GPIO_PeriphID2_DEFAULT)
    {
        rumboot_printf("ERROR!!! The default value does not match the expected valuen the register GPIO_PeriphID2.\n");
        rumboot_printf("expect = %x \n", GPIO_PeriphID2_DEFAULT);
        rumboot_printf("actual = %x \n", ioread32(base_addr + GPIO_PeriphID2));
        result |= (1<<12);
    } else
        rumboot_printf("Check GPIO_PeriphID2: OK \n");

    if (ioread32(base_addr + GPIO_PeriphID3) != GPIO_PeriphID3_DEFAULT)
    {
        rumboot_printf("ERROR!!! The default value does not match the expected valuen the register GPIO_PeriphID3.\n");
        rumboot_printf("expect = %x \n", GPIO_PeriphID3_DEFAULT);
        rumboot_printf("actual = %x \n", ioread32(base_addr + GPIO_PeriphID3));
        result |= (1<<13);
    } else
        rumboot_printf("Check GPIO_PeriphID3: OK \n");

    if (ioread32(base_addr + GPIO_CellID0) != GPIO_CellID0_DEFAULT)
    {
        rumboot_printf("ERROR!!! The default value does not match the expected valuen the register GPIO_CellID0.\n");
        rumboot_printf("expect = %x \n", GPIO_CellID0_DEFAULT);
        rumboot_printf("actual = %x \n", ioread32(base_addr + GPIO_CellID0));
        result |= (1<<14);
    } else
        rumboot_printf("Check GPIO_CellID0: OK \n");

    if (ioread32(base_addr + GPIO_CellID1) != GPIO_CellID1_DEFAULT)
    {
        rumboot_printf("ERROR!!! The default value does not match the expected valuen the register GPIO_CellID1.\n");
        rumboot_printf("expect = %x \n", GPIO_CellID1_DEFAULT);
        rumboot_printf("actual = %x \n", ioread32(base_addr + GPIO_CellID1));
        result |= (1<<15);
    } else
        rumboot_printf("Check GPIO_CellID1: OK \n");

    if (ioread32(base_addr + GPIO_CellID2) != GPIO_CellID2_DEFAULT)
    {
        rumboot_printf("ERROR!!! The default value does not match the expected valuen the register GPIO_CellID2.\n");
        rumboot_printf("expect = %x \n", GPIO_CellID2_DEFAULT);
        rumboot_printf("actual = %x \n", ioread32(base_addr + GPIO_CellID2));
        result |= (1<<16);
    } else
        rumboot_printf("Check GPIO_CellID2: OK \n");

    if (ioread32(base_addr + GPIO_CellID3) != GPIO_CellID3_DEFAULT)
    {
        rumboot_printf("ERROR!!! The default value does not match the expected valuen the register GPIO_CellID3.\n");
        rumboot_printf("expect = %x \n", GPIO_CellID3_DEFAULT);
        rumboot_printf("actual = %x \n", ioread32(base_addr + GPIO_CellID3));
        result |= (1<<17);
    } else
        rumboot_printf("Check GPIO_CellID3: OK \n");

    return result;
}

static uint32_t check_gpio_regs(uint32_t base_addr)
{
    rumboot_printf("Check WRITE/READ registers\n");
    uint32_t result = 0x0;



    rumboot_printf("GPIO_DIR check:");
    iowrite32(GPIO_REG_MASK & (~GPIO_DIR_DEFAULT), base_addr + GPIO_DIR);
    if (ioread32(base_addr + GPIO_DIR) != (GPIO_REG_MASK & (~GPIO_DIR_DEFAULT)))
    {
        rumboot_printf("ERROR!!! The value read does not match the recorded value the register GPIO_DIR.\n");
        rumboot_printf("expect = %x \n", GPIO_REG_MASK & (~GPIO_DIR_DEFAULT));
        rumboot_printf("actual = %x \n", ioread32(base_addr + GPIO_DIR));
        result |= (1<<1);
    } else
        rumboot_printf(" OK\n");

    rumboot_printf("GPIO_DATA check:");
    iowrite32(GPIO_REG_MASK & (~GPIO_DATA_DEFAULT), base_addr + GPIO_DATA + GPIO_ADDR_MASK);
    if (ioread32(base_addr + GPIO_DATA + GPIO_ADDR_MASK) != (GPIO_REG_MASK & (~GPIO_DATA_DEFAULT)))
    {
        rumboot_printf("ERROR!!! The value read does not match the recorded value the register GPIO_DATA.\n");
        rumboot_printf("expect = %x \n", GPIO_REG_MASK & (~GPIO_DATA_DEFAULT));
        rumboot_printf("actual = %x \n", ioread32(base_addr + GPIO_DATA + GPIO_ADDR_MASK));
        result |= (1<<0);
    } else
        rumboot_printf(" OK\n");
    iowrite32(GPIO_DATA_DEFAULT, base_addr + GPIO_DATA);
    iowrite32(GPIO_DIR_DEFAULT, base_addr + GPIO_DIR);

//    rumboot_printf("GPIO_IS check:");
//    iowrite32(GPIO_REG_MASK & (~GPIO_IS_DEFAULT), base_addr + GPIO_IS);
//    if (ioread32(base_addr + GPIO_IS) != (GPIO_REG_MASK & (~GPIO_IS_DEFAULT)))
//    {
//        rumboot_printf("ERROR!!! The value read does not match the recorded value the register GPIO_IS.\n");
//        rumboot_printf("expect = %x \n", GPIO_REG_MASK & (~GPIO_IS_DEFAULT));
//        rumboot_printf("actual = %x \n", ioread32(base_addr + GPIO_IS));
//        result |= (1<<2);
//    } else
//        rumboot_printf(" OK\n");
//    iowrite32(GPIO_IS_DEFAULT, base_addr + GPIO_IS);
//
//    rumboot_printf("GPIO_IBE check:");
//    iowrite32(GPIO_REG_MASK & (~GPIO_IBE_DEFAULT), base_addr + GPIO_IBE);
//    if (ioread32(base_addr + GPIO_IBE) != (GPIO_REG_MASK & (~GPIO_IBE_DEFAULT)))
//    {
//        rumboot_printf("ERROR!!! The value read does not match the recorded value the register GPIO_IBE.\n");
//        rumboot_printf("expect = %x \n", GPIO_REG_MASK & (~GPIO_IBE_DEFAULT));
//        rumboot_printf("actual = %x \n", ioread32(base_addr + GPIO_IBE));
//        result |= (1<<3);
//    } else
//        rumboot_printf(" OK\n");
//    iowrite32(GPIO_IBE_DEFAULT, base_addr + GPIO_IBE);
//
//
//    rumboot_printf("GPIO_IEV check:");
//    iowrite32(GPIO_REG_MASK & (~GPIO_IEV_DEFAULT), base_addr + GPIO_IEV);
//    if (ioread32(base_addr + GPIO_IEV) != (GPIO_REG_MASK & (~GPIO_IEV_DEFAULT)))
//    {
//        rumboot_printf("ERROR!!! The value read does not match the recorded value the register GPIO_IEV.\n");
//        rumboot_printf("expect = %x \n", GPIO_REG_MASK & (~GPIO_IEV_DEFAULT));
//        rumboot_printf("actual = %x \n", ioread32(base_addr + GPIO_IEV));
//        result |= (1<<4);
//    } else
//        rumboot_printf(" OK\n");
//    iowrite32(GPIO_IEV_DEFAULT, base_addr + GPIO_IEV);
//
//
//    rumboot_printf("GPIO_IE check:");
//    iowrite32(GPIO_REG_MASK & (~GPIO_IE_DEFAULT), base_addr + GPIO_IE);
//    if (ioread32(base_addr + GPIO_IE) != (GPIO_REG_MASK & (~GPIO_IE_DEFAULT)))
//    {
//        rumboot_printf("ERROR!!! The value read does not match the recorded value the register GPIO_IE.\n");
//        rumboot_printf("expect = %x \n", GPIO_REG_MASK & (~GPIO_IE_DEFAULT));
//        rumboot_printf("actual = %x \n", ioread32(base_addr + GPIO_IE));
//        result |= (1<<5);
//    } else
//        rumboot_printf(" OK\n");
//    iowrite32(GPIO_IE_DEFAULT, base_addr + GPIO_IE);
//
//    rumboot_printf("GPIO_AFSEL check:");
//    iowrite32(GPIO_REG_MASK & (~GPIO_AFSEL_DEFAULT), base_addr + GPIO_AFSEL);
//    if (ioread32(base_addr + GPIO_AFSEL) != (GPIO_REG_MASK & (~GPIO_AFSEL_DEFAULT)))
//    {
//        rumboot_printf("ERROR!!! The value read does not match the recorded value the register GPIO_AFSEL.\n");
//        rumboot_printf("expect = %x \n", GPIO_REG_MASK & (~GPIO_AFSEL_DEFAULT));
//        rumboot_printf("actual = %x \n", ioread32(base_addr + GPIO_AFSEL));
//        result |= (1<<5);
//    } else
//        rumboot_printf(" OK\n");
//    iowrite32(GPIO_AFSEL_DEFAULT, base_addr + GPIO_AFSEL);


    return result;
}
static void handler0( int irq, void *arg ) {
//    struct s804_instance *a = ( struct s804_instance * )arg;
//    a->timer0_irq = a->timer0_irq + 1;
    rumboot_printf( "IRQ 0 arrived  \n" );

    rumboot_printf( "sp804_%d timer 0 INT # %d  \n");
//    sp804_clrint( a->base_addr, 0 );
}

static void handler1( int irq, void *arg ) {
//    struct s804_instance *a = ( struct s804_instance * )arg;
//    a->timer1_irq = a->timer1_irq + 1;
    rumboot_printf( "IRQ 1 arrived  \n" );
    rumboot_printf( "sp804_%d timer 1 INT # %d  \n");
//    sp804_clrint( a->base_addr, 1 );

}

uint32_t check_int(uint32_t base_addr)
{
    uint32_t result = 0x0;
    rumboot_irq_cli();
    struct rumboot_irq_entry *tbl = rumboot_irq_create( NULL );

    rumboot_irq_set_handler( tbl, GPIO0_INT, (int_sense_level << MPIC128_VP_S_i) | (int_pol_high << MPIC128_VP_POL_i), handler0, (void*)0);
    rumboot_irq_set_handler( tbl, GPIO1_INT, (int_sense_level << MPIC128_VP_S_i) | (int_pol_high << MPIC128_VP_POL_i), handler1, (void*)0);

    /* Activate the table */
    rumboot_irq_table_activate( tbl );
    rumboot_irq_enable( GPIO0_INT );
    rumboot_irq_enable( GPIO1_INT );
    rumboot_irq_sei();

    iowrite32(0xFF, base_addr + GPIO_IS);
    iowrite32(0xFF, base_addr + GPIO_IEV);
    iowrite32(0xFF, base_addr + GPIO_IE);

    iowrite32(0xFF, base_addr + GPIO_DIR);
    iowrite32(0xAA, base_addr + GPIO_DATA + GPIO_ADDR_MASK);


    rumboot_irq_table_activate(NULL);
    rumboot_irq_free(tbl);
    return result;
}

int main(void)
{
    uint32_t result = 0x0;
    rumboot_printf("Check GPIO_0 \n");
    if(!check_gpio_default_val(GPIO_0_BASE) && !check_gpio_regs(GPIO_0_BASE) && !check_int(GPIO_0_BASE))
        rumboot_printf("Check GPIO_0: OK \n");
    else
        result = (1<<0);

    rumboot_printf("Check GPIO_1 \n");
    if(!check_gpio_default_val(GPIO_1_BASE) && !check_gpio_regs(GPIO_1_BASE) && !check_int(GPIO_1_BASE))
        rumboot_printf("Check GPIO_1: OK \n");
    else
        result = (1<<1);

    return result;

}
