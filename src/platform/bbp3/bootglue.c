#include <stdint.h>
#include <platform/devices.h>
#include <rumboot/io.h>
#include <rumboot/boot.h>
#include <rumboot/platform.h>
#include <rumboot/bootsrc/spiflash.h>
#include <rumboot/bootsrc/eeprom.h>
#include <rumboot/bootsrc/boilerplate.h>
#include <rumboot/bootsrc/sdio.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <regs/regs_gpio_pl061.h>
#include <regs/regs_uart_pl011.h>
#include <devices/uart_pl011.h>
#include <rumboot/bootsrc/physmap.h>

//#define BOOTM_SELFTEST     (1 << 0)
//#define BOOTM_HOST         (1 << 1)
//#define BOOTM_FASTUART     (1 << 2)
//#define BOOTM_SDIO0_CD     (1 << 3)
//#define BOOTM_SDIO1_CD     (1 << 4)
//#define BOOTM_SPI1_CS1     (1 << 5)


static inline int is_silent()
{
        return false;
}

void rumboot_platform_read_config(struct rumboot_config *conf)
{
//        uint32_t bootm = ioread32(SCTL_BASE + SCTL_BOOTM);
//
//        if (bootm & BOOTM_FASTUART) {
        conf->baudrate = 6250000;
//        } else {
        //conf->baudrate = 115200;
//        }
//
//        conf->hostmode = (bootm & BOOTM_HOST);
//        conf->selftest = (bootm & BOOTM_SELFTEST);
}



void rumboot_platform_init_loader(struct rumboot_config *conf)
{
        /* Initialize UART */
        if (!is_silent()) {
                struct uart_init_params sparams =
                {
                        .wlen			= UART_word_length_8bit,
                        .uart_sys_freq_hz	= 128000000,
                        .parity			= UART_parity_no,
                        .rx_fifo_level		= UART_RX_FIFO_LEVEL_GT_7_8,
                        .tx_fifo_level		= UART_TX_FIFO_LEVEL_LT_1_8,
                        .int_mask		= 0x0,
                        .use_rts_cts		= false,
                        .loopback		= false
                };
                sparams.baud_rate = conf->baudrate;
                uart_init(UART0_Base, &sparams);
                uart_tx_enable(UART0_Base, true);
                uart_rx_enable(UART0_Base, true);
                uart_enable(UART0_Base, true);

                /* Send sync byte. This way it will work for debug builds */
                iowrite32(0x55, UART0_Base + UARTDR);
        }
}


#define SPI_CS0 (1 << 0)
static bool spi0_enable(const struct rumboot_bootsource *src, void *pdata)
{
        uint32_t v = ioread32(GPIOE_Base + GPIO_DIR);
        iowrite32(v | SPI_CS0, GPIOE_Base + GPIO_DIR);
        return true;
}

static void spi0_disable(const struct rumboot_bootsource *src, void *pdata)
{
        uint32_t v;

        v = ioread32(GPIOE_Base + GPIO_DIR);
        v &= ~SPI_CS0;
        iowrite32(v, GPIOE_Base + GPIO_DIR);
}

static void spi0_cs(const struct rumboot_bootsource *src, void *pdata, int select)
{
        if (!select) {
                iowrite32(0, GPIOE_Base + GPIO_DATA + (SPI_CS0 << 2));
        } else {
                iowrite32(SPI_CS0, GPIOE_Base + GPIO_DATA + (SPI_CS0 << 2));
        }
}

static const struct rumboot_bootsource spi_boot[] = {
        {
                .name = "SPI0 (CS: GPE0)",
                .base = SPI0_Base,
                .freq_khz = 128000,
                .plugin = &g_bootmodule_spiflash,
                .enable = spi0_enable,
                .disable = spi0_disable,
                .chipselect = spi0_cs,
        },
        { /*Sentinel*/ }
};

static const struct rumboot_bootsource emi_boot[] = {
        {
                .name = "EMI",
                .base = EMI_MEM_Base,
                .plugin = &g_bootmodule_physmap,
        },
        { /*Sentinel*/ }
};


void rumboot_platform_enter_host_mode()
{
        uint32_t v;
}

void *rumboot_platform_get_spl_area(size_t *size)
{
        *size = (&rumboot_platform_spl_end - &rumboot_platform_spl_start);
        return (void *)&rumboot_platform_spl_start;
}

const struct rumboot_bootsource *rumboot_platform_get_bootsources()
{
        return spi_boot;
}

bool rumboot_platform_check_entry_points(struct rumboot_bootheader *hdr)
{
        /* Any entry point is okay */
        return true;
}

int rumboot_platform_exec(struct rumboot_bootheader *hdr)
{
        return rumboot_bootimage_execute_ep((void *)hdr->entry_point[0]);
}

void rumboot_platform_print_summary(struct rumboot_config *conf)
{
}


int rumboot_platform_selftest(struct rumboot_config *conf)
{
        return 0;
}


#ifndef CMAKE_BUILD_TYPE_DEBUG
void __attribute__((no_instrument_function)) rumboot_platform_putchar(uint8_t c)
{
        if (is_silent()) {
                return;
        }

        if (c == '\n') {
                rumboot_platform_putchar('\r');
        }

        while (uart_check_tfifo_full(UART0_Base));;
        iowrite32(c, UART0_Base + UARTDR);
}


int rumboot_platform_getchar(uint32_t timeout_us)
{
        if (is_silent()) {
                delay_us(timeout_us);
                return -1;
        }

        uint32_t start = rumboot_platform_get_uptime();

        while (rumboot_platform_get_uptime() - start < timeout_us) {
                if (!uart_check_rfifo_empty(UART0_Base)) {
                        return ioread32(UART0_Base + UARTDR) & 0xFF;
                }
        }
        return -1;
}
#endif
