/**
 * Implemented test which described in 2.8 PPC_SRAM_SDRAM_slave0_testplan.docx
 *  inout     wire  UART0_TXD               ,check_others();
 *  inout     wire  UART0_RTS               ,check_others();
 *
 *  inout     wire  UART1_TXD               ,check_others();
 *  inout     wire  UART1_RTS               ,check_others();
 *
 *  inout     wire  MK0_TXA                 ,check_others()
 *  inout     wire  MK0_TXA_N               ,check_others()
 *  inout     wire  MK0_TXB                 ,check_others()
 *  inout     wire  MK0_TXB_N               ,check_others()
 *  inout     wire  MK0_TXINHA              ,check_others()
 *  inout     wire  MK0_TXINHB              ,check_others()
 *
 *  inout     wire  MK1_TXA                 ,check_others()
 *  inout     wire  MK1_TXA_N               ,check_others()
 *  inout     wire  MK1_TXB                 ,check_others()
 *  inout     wire  MK1_TXB_N               ,check_others()
 *  inout     wire  MK1_TXINHA              ,check_others()
 *  inout     wire  MK1_TXINHB              ,check_others()
 *
 *  inout     wire  SPI0_SCK                ,check_others()
 *  inout     wire  SPI0_MOSI               ,check_spi()
 *  inout     wire  SPI0_MISO               ,check_others()
 *  inout     wire  SPI0_IRQ                ,check_others()
 *  inout     wire  SPI0_DAT2               ,check_others()
 *  inout     wire  SPI0_SCS                ,check_spi()
 *
 *  inout     wire  SPI1_SCK                ,check_others()
 *  inout     wire  SPI1_MOSI               ,check_spi()
 *  inout     wire  SPI1_MISO               ,check_others()
 *  inout     wire  SPI1_IRQ                ,check_others()
 *  inout     wire  SPI1_DAT2               ,check_others()
 *  inout     wire  SPI1_SCS                ,check_spi()
 *
 *  inout     wire  GPIO0_0-GPIO0_7         , check_gpio
 *
 *  inout     wire  GPIO1_0-GPIO1_7         , check_gpio
 *
 *  inout     wire  ETH0_TX_EN              ,check_others();
 *  inout     wire  ETH0_TX_ER              ,check_others();
 *  inout     wire  ETH0_TXD0-ETH0_TXD3     ,check_others();
 *  inout     wire  ETH0_MDC                ,check_others();
 *  inout     wire  ETH0_MDIO               ,check_mdio
 *
 *  inout     wire  ETH1_TX_EN              ,check_others();
 *  inout     wire  ETH1_TX_ER              ,check_others();
 *  inout     wire  ETH1_TXD0-ETH1_TXD3     ,check_others();
 *  inout     wire  ETH1_MDC                ,check_others();
 *  inout     wire  ETH1_MDIO               ,check_mdio
 *
 *  inout     wire  TDO                     ,
 *
 *  inout     wire  MACHINE_CHECK0          ,
 *
 *  inout     wire  D0-D31                  ,check_emi_ports
 *  inout     wire  ED0-ED7                 ,check_emi_ports
 *  inout     wire  A0-A23                  ,check_emi_ports
 *  inout     wire  CSn0-CSn5               ,check_emi_ports
 *  inout     wire  OEn                     ,check_emi_ports
 *  inout     wire  WEn                     ,check_emi_ports
 *  inout     wire  WE_An0                  ,check_emi_ports
 *  inout     wire  RASn                    ,check_emi_ports
 *  inout     wire  CASn                    ,check_emi_ports
 *  inout     wire  DQMBn0-DQMBn4           ,check_emi_ports
 *  inout     wire  HOLDOn                  ,check_others();
 *  inout     wire  HOLDAn                  ,check_others();
 *
 *  inout     wire  NRST_SYS                ,check_others();
 *
 *  inout     wire  EXT_CLK                 ,check_others();
 *
 *  inout     wire  WDT_IRQ                  check_others();
 *
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
#define TEST_EVENT_CHECK_OTHERS         0x00001005
#define TEST_EVENT_CHECK_SPI            0x00001006

void check_emi_ports()
{
    const uint32_t test_data = 0xDEADBA11;
    emi_init(DCR_EM2_EMI_BASE);

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

void check_spi()
{
    test_event(TEST_EVENT_CHECK_SPI);
    iowrite32(0x1, 0xC002C000 + 0x300);//SDIO0_ENABLE to switch in HiZ
    iowrite32(0x1, 0xC003C000 + 0x300);//SDIO0_ENABLE to switch in HiZ
    iowrite32(0x0, 0xC002C000 + 0x300);//SDIO0_ENABLE to switch in act
    iowrite32(0x0, 0xC003C000 + 0x300);//SDIO0_ENABLE to switch in act
}

void check_others()
{
    test_event(TEST_EVENT_CHECK_OTHERS);
}

int main()
{
    rumboot_printf("Start test_oi10_em2_208\n");
    test_event_send_test_id("test_oi10_em2_208");

    check_emi_ports();
    check_gpio();
    check_mdio();
    check_spi();
    check_others();

    return 0;
}


