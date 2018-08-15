/**
 * Implemented test which described in 2.8 PPC_SRAM_SDRAM_slave0_testplan.docx
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/testsuite.h>

#include <rumboot/printf.h>
#include <rumboot/io.h>

#include <platform/devices.h>
#include <rumboot/platform.h>
#include <platform/test_assert.h>
#include <platform/devices/emi.h>
#include <platform/devices/greth.h>
#include <devices/gpio_pl061.h>

#define TEST_EVENT_CHECK_EMI_ACTIVE     0x00001000
#define TEST_EVENT_CHECK_EMI_HIZ        0x00001001
#define TEST_EVENT_CHECK_GPIO_ACTIVE    0x00001002
#define TEST_EVENT_CHECK_GPIO_HIZ       0x00001003
#define TEST_EVENT_CHECK_MDIO           0x00001004

void check_emi_ports()
{
    const uint32_t test_data = 0xDEADBA11;
    emi_init();

    test_event(TEST_EVENT_CHECK_EMI_ACTIVE);
    iowrite32(test_data, SRAM0_BASE);
    TEST_ASSERT(test_data==ioread32(SRAM0_BASE), "Data error");

    test_event(TEST_EVENT_CHECK_EMI_HIZ);
    emi_hiz_mode_on(DCR_EM2_EMI_BASE);

    test_event(TEST_EVENT_CHECK_EMI_ACTIVE);
    emi_hiz_mode_off(DCR_EM2_EMI_BASE);
    iowrite32(test_data, SRAM0_BASE);
    TEST_ASSERT(test_data==ioread32(SRAM0_BASE), "Data error");
}

void check_gpio()
{
    test_event(TEST_EVENT_CHECK_GPIO_HIZ);

    gpio_set_port_direction(GPIO_0_BASE, GIO_PIN_ALL_OUT);
    gpio_set_port_direction(GPIO_1_BASE, GIO_PIN_ALL_OUT);
    for (int i=0; i<8; i++)
    {
        gpio_set_pin(GPIO_0_BASE, i, 0);
        gpio_set_pin(GPIO_1_BASE, i, 0);
    }
    test_event(TEST_EVENT_CHECK_GPIO_ACTIVE);

    gpio_set_port_direction(GPIO_0_BASE, GIO_PIN_ALL_IN);
    gpio_set_port_direction(GPIO_1_BASE, GIO_PIN_ALL_IN);
    test_event(TEST_EVENT_CHECK_GPIO_HIZ);
}

void check_mdio()
{
    test_event(TEST_EVENT_CHECK_MDIO);
    TEST_ASSERT(greth_mdio_read(GRETH_0_BASE, ETH_PHY_ADDR, ETH_PHY_ID0 )==ETH_PHY_ID0_DEFAULT, "Error at mdio reading ETH0_PHY_ID0 register\n");
    TEST_ASSERT(greth_mdio_read(GRETH_1_BASE, ETH_PHY_ADDR, ETH_PHY_ID0 )==ETH_PHY_ID0_DEFAULT, "Error at mdio reading ETH1_PHY_ID0 register\n");
}


int main()
{
    rumboot_printf("Start test_oi10_em2_208\n");
    test_event_send_test_id("test_oi10_em2_208");

    check_emi_ports();
    check_gpio();
    check_mdio();

    return 0;
}


