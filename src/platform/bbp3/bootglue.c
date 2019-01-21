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
#include <rumboot/timer.h>
#include <devices/greth_edcl.h>

#define BOOTM_HOST         (1 << 0)
#define BOOTM_SILENT       (1 << 1)
#define BOOTM_EMI          (1 << 2)
#define BOOTM_NMC          (1 << 3)
#define BOOTM_EMI_BIS      (1 << 4)

#define BOOTM_NM0          (1 << 0)
#define BOOTM_NM1          (1 << 1)



static inline uint32_t bootm() {
        return ioread32(SCTL_Base);
}

static inline int is_silent()
{
        return bootm() & BOOTM_SILENT;
}

void rumboot_platform_read_config(struct rumboot_config *conf)
{
        conf->baudrate = 115200;
        conf->selftest = -1;
        conf->edcl = 1; /* EDCL Always on */
        conf->hostmode = (bootm() & BOOTM_HOST);
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

static bool emi_enable(const struct rumboot_bootsource *src, void *pdata)
{
        /* EMI BIS must be high! */
        bool ret = (bootm() & BOOTM_EMI_BIS);
        if (!ret) {
                dbg_boot(src, "EMI_BIS pin low, skipping NOR boot");
        }
        return ret;
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
                .name = "NOR",
                .base = EMI_MEM_Base,
                .plugin = &g_bootmodule_physmap,
                .enable = &emi_enable
        },
        { /*Sentinel*/ }
};


void rumboot_platform_enter_host_mode()
{
        greth_dump_edcl_params(0, GRETH_Base);
}

void *rumboot_platform_get_spl_area(size_t *size)
{
        *size = (&rumboot_platform_spl_end - &rumboot_platform_spl_start);
        return (void *)&rumboot_platform_spl_start;
}

const struct rumboot_bootsource *rumboot_platform_get_bootsources()
{
        return (bootm() & BOOTM_EMI) ? emi_boot : spi_boot;
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
        rumboot_printf("NMC Boot Mode:   %s\n",
                (bootm() & BOOTM_NMC) ? "Disabled" : "Enabled");
        rumboot_printf("NMC Cores:       %s%s\n",
                (bootm() & BOOTM_NM0) ? "[NM0]" : "",
                (bootm() & BOOTM_NM1) ? "[NM1]" : "");
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
                udelay(timeout_us);
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
