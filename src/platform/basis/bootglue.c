#include <stdint.h>
#include <platform/devices.h>
#include <rumboot/io.h>
#include <regs/regs_gpio_rcm.h>
#include <rumboot/boot.h>
#include <rumboot/platform.h>
#include <rumboot/bootsrc/spiflash.h>
#include <rumboot/bootsrc/eeprom.h>
#include <rumboot/bootsrc/boilerplate.h>
#include <rumboot/bootsrc/sdio.h>
#include <platform/regs/sctl.h>
#include <devices/muart.h>
#include <regs/regs_muart.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>

#define BOOTM_SELFTEST     (1 << 0)
#define BOOTM_HOST         (1 << 1)
#define BOOTM_FASTUART     (1 << 2)
#define BOOTM_SDIO0_CD     (1 << 3)
#define BOOTM_SDIO1_CD     (1 << 4)
#define BOOTM_SPI0_CS1     (1 << 5)

void rumboot_platform_read_config(struct rumboot_config *conf)
{
        uint32_t bootm = ioread32(SCTL_BASE + SCTL_BOOTM);

        if (bootm & BOOTM_FASTUART) {
                conf->baudrate = 6250000;
        } else {
                conf->baudrate = 115200;
        }

        conf->hostmode = (bootm & BOOTM_HOST);
        conf->selftest = (bootm & BOOTM_SELFTEST);
}

void rumboot_platform_init_loader(struct rumboot_config *conf)
{
        iowrite32(1, GLOBAL_TIMERS + 0x40); /* Fire up global timers */
        struct muart_conf uconf;
        uconf.wlen = WL_8;
        uconf.stp2 = STP1;
        uconf.is_even = false;
        uconf.is_parity = false;
        uconf.mode = RS_TTL;
        uconf.is_loopback = 0;
        uconf.baud_rate = conf->baudrate;  // bod/s (bdiv=4)
        uconf.is_dma = 0;
        muart_init(UART0_BASE, &uconf);
        muart_enable(UART0_BASE);
        muart_write_char(UART0_BASE, 0x55);
}

static bool sdio0_enable(const struct rumboot_bootsource *src, void *pdata)
{
        uint32_t v;

        v = ioread32(GPIO0_BASE + GPIO_PAD_DIR);
        v &= ~BOOTM_SDIO0_CD;
        iowrite32(v, GPIO0_BASE + GPIO_PAD_DIR);
        return !(ioread32(GPIO0_BASE + GPIO_RD_DATA) & BOOTM_SDIO0_CD);
}

static bool sdio1_enable(const struct rumboot_bootsource *src, void *pdata)
{
        uint32_t v;

        v = ioread32(GPIO0_BASE + GPIO_PAD_DIR);
        v &= ~BOOTM_SDIO1_CD;
        iowrite32(v, GPIO0_BASE + GPIO_PAD_DIR);

        return !(ioread32(GPIO0_BASE + GPIO_RD_DATA) & BOOTM_SDIO1_CD);
}


static bool spi0_1_enable(const struct rumboot_bootsource *src, void *pdata)
{
        uint32_t v;

        v = ioread32(GPIO0_BASE + GPIO_PAD_DIR);
        v |= 1 << 5;
        iowrite32(v, GPIO0_BASE + GPIO_PAD_DIR);
        iowrite32((1 << 5), GPIO0_BASE + GPIO_WR_DATA_SET1);
        return true;
}

static void spi0_1_disable(const struct rumboot_bootsource *src, void *pdata)
{
        uint32_t v;

        v = ioread32(GPIO0_BASE + GPIO_PAD_DIR);
        v &= ~(1 << 5);
        iowrite32(v, GPIO0_BASE + GPIO_PAD_DIR);
}

static void spi0_1_cs(const struct rumboot_bootsource *src, void *pdata, int select)
{
        if (!select) {
                iowrite32(~(1 << 5), GPIO0_BASE + GPIO_WR_DATA_SET0);
        } else {
                iowrite32((1 << 5), GPIO0_BASE + GPIO_WR_DATA_SET1);
        }
}

static const struct rumboot_bootsource arr[] = {
        {
                .name = "SDIO0 (CD: GPIO0_3)",
                .base = SDIO0_BASE,
                .freq_khz = 100000,
                .plugin = &g_bootmodule_sdio,
                .enable = sdio0_enable,
                .offset = 8192,
        },
        {
                .name = "SPI0 (CS: internal)",
                .base = GSPI0_BASE,
                .freq_khz = 100000,
                .plugin = &g_bootmodule_spiflash,
        },
        {
                .name = "SPI0 (CS: GPIO0_5)",
                .base = GSPI0_BASE,
                .freq_khz = 100000,
                .plugin = &g_bootmodule_spiflash,
                .enable = spi0_1_enable,
                .disable = spi0_1_disable,
                .chipselect = spi0_1_cs,
        },
        {
                .name = "I2C0 (EEPROM @ 0x50)",
                .base = I2C0_BASE,
                .slave_addr = 0x50,
                .freq_khz = 100000,
                .plugin = &g_bootmodule_eeprom,
        },
        {
                .name = "I2C0 (EEPROM @ 0x51)",
                .base = I2C0_BASE,
                .slave_addr = 0x51,
                .freq_khz = 100000,
                .plugin = &g_bootmodule_eeprom,
        },
        {
                .name = "I2C0 (EEPROM @ 0x52)",
                .base = I2C0_BASE,
                .slave_addr = 0x52,
                .freq_khz = 100000,
                .plugin = &g_bootmodule_eeprom,
        },
        {
                .name = "I2C0 (EEPROM @ 0x53)",
                .base = I2C0_BASE,
                .slave_addr = 0x53,
                .freq_khz = 100000,
                .plugin = &g_bootmodule_eeprom,
        },
        {
                .name = "SDIO1 (CD: GPIO0_4)",
                .base = SDIO1_BASE,
                .freq_khz = 100000,
                .plugin = &g_bootmodule_sdio,
                .enable = sdio1_enable,
                .offset = 8192
        },
        { /*Sentinel*/ }
};


void rumboot_platform_enter_host_mode()
{
        uint32_t v;

        v = ioread32(GPIO0_BASE + GPIO_PAD_DIR);
        v |= BOOTM_HOST;
        iowrite32(v, GPIO0_BASE + GPIO_PAD_DIR);
        iowrite32(BOOTM_HOST, GPIO0_BASE + GPIO_WR_DATA_SET1);
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


#ifndef CMAKE_BUILD_TYPE_DEBUG
void __attribute__((no_instrument_function)) rumboot_platform_putchar(uint8_t ch)
{
        while ((ioread32(UART0_BASE + MUART_FIFO_STATE) & 0x7ff0000) >= 0x3ff0000);
        iowrite32(ch, UART0_BASE + MUART_DTRANS);
}


int rumboot_platform_getchar(uint32_t timeout_us)
{
        uint32_t start = rumboot_platform_get_uptime();

        while (rumboot_platform_get_uptime() - start < timeout_us) {
                if ((ioread32(UART0_BASE + MUART_FIFO_STATE)) & 0xfff) {
                        return ioread32(UART0_BASE + MUART_DREC);
                }
        }
        return -1;
}
#endif
