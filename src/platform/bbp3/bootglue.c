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

//#define BOOTM_SELFTEST     (1 << 0)
//#define BOOTM_HOST         (1 << 1)
//#define BOOTM_FASTUART     (1 << 2)
//#define BOOTM_SDIO0_CD     (1 << 3)
//#define BOOTM_SDIO1_CD     (1 << 4)
//#define BOOTM_SPI1_CS1     (1 << 5)

void rumboot_platform_read_config(struct rumboot_config *conf)
{
//        uint32_t bootm = ioread32(SCTL_BASE + SCTL_BOOTM);
//
//        if (bootm & BOOTM_FASTUART) {
//                conf->baudrate = 6250000;
//        } else {
//                conf->baudrate = 115200;
//        }
//
//        conf->hostmode = (bootm & BOOTM_HOST);
//        conf->selftest = (bootm & BOOTM_SELFTEST);
}



void rumboot_platform_init_loader(struct rumboot_config *conf)
{
        rumboot_irq_set_exception_handler(rumboot_arch_exception);
}


static bool spi0_1_enable(const struct rumboot_bootsource *src, void *pdata)
{
        uint32_t v;

//        v = ioread32(GPIO0_BASE + GPIO_PAD_DIR);
//        v |= 1 << 5;
//        iowrite32(v, GPIO0_BASE + GPIO_PAD_DIR);
//        iowrite32((1 << 5), GPIO0_BASE + GPIO_WR_DATA_SET1);
        return true;
}

static void spi0_1_disable(const struct rumboot_bootsource *src, void *pdata)
{
        uint32_t v;

//        v = ioread32(GPIO0_BASE + GPIO_PAD_DIR);
//        v &= ~(1 << 5);
//        iowrite32(v, GPIO0_BASE + GPIO_PAD_DIR);
}

static void spi0_1_cs(const struct rumboot_bootsource *src, void *pdata, int select)
{
//        if (!select) {
//                iowrite32(~(1 << 5), GPIO0_BASE + GPIO_WR_DATA_SET0);
//        } else {
//                iowrite32((1 << 5), GPIO0_BASE + GPIO_WR_DATA_SET1);
//        }
}

static const struct rumboot_bootsource arr[] = {
        {
                .name = "SPI0 (CS: ???)",
                .base = SPI0_Base,
                .freq_khz = 100000,
                .plugin = &g_bootmodule_spiflash,
                .enable = spi0_1_enable,
                .disable = spi0_1_disable,
                .chipselect = spi0_1_cs,
        },
        { /*Sentinel*/ }
};


void rumboot_platform_enter_host_mode()
{
        uint32_t v;

//        v = ioread32(GPIO0_BASE + GPIO_PAD_DIR);
//        v |= BOOTM_HOST;
//        iowrite32(v, GPIO0_BASE + GPIO_PAD_DIR);
//        iowrite32(BOOTM_HOST, GPIO0_BASE + GPIO_WR_DATA_SET1);
}

void *rumboot_platform_get_spl_area(size_t *size)
{
        *size = (&rumboot_platform_spl_end - &rumboot_platform_spl_start);
        return (void *)&rumboot_platform_spl_start;
}

const struct rumboot_bootsource *rumboot_platform_get_bootsources()
{
        return arr;
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
void  __attribute__((no_instrument_function)) rumboot_platform_putchar(uint8_t c)
{
        if (c == '\n') {
                rumboot_platform_putchar('\r');
        }

        while (uart_check_tfifo_full(UART0_Base));;
        iowrite32(c, UART0_Base + UARTDR);
}


int rumboot_platform_getchar(uint32_t timeout_us)
{
        uint32_t start = rumboot_platform_get_uptime();

        while (rumboot_platform_get_uptime() - start < timeout_us) {
                if (!uart_check_rfifo_empty(UART0_Base)) {
                        return ioread32(UART0_Base + UARTDR) & 0xFF;
                }
        }
        return -1;
}
#endif
