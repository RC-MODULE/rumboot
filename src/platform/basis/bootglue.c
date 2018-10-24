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


#define BOOTM_SELFTEST     (1 << 0)
#define BOOTM_HOST         (1 << 1)
#define BOOTM_FASTUART     (1 << 2)
#define BOOTM_SDIO_CD      (1 << 3)

void rumboot_platform_read_config(struct rumboot_config *conf)
{
        uint32_t bootm = ioread32(SCTL_BASE + SCTL_BOOTM);

        if (bootm & BOOTM_FASTUART) {
                conf->baudrate = 6000000;
        } else {
                conf->baudrate = 115200;
        }

        conf->hostmode = (bootm & BOOTM_HOST);
        conf->selftest = (bootm & BOOTM_SELFTEST);
}


void rumboot_platform_selftest(struct rumboot_config *conf)
{
        /* Execute selftest routines */
}

void rumboot_platform_init_loader(struct rumboot_config *conf)
{
        
}

static bool sdio_enable(const struct rumboot_bootsource *src, void *pdata)
{
        uint32_t v;

        v = ioread32(GPIO0_BASE + GPIO_PAD_DIR);
        v &= ~ BOOTM_SDIO_CD;
        iowrite32(v, GPIO0_BASE + GPIO_PAD_DIR);

        return !(ioread32(GPIO0_BASE + GPIO_RD_DATA) & BOOTM_SDIO_CD);
}

static bool spi0_0_enable(const struct rumboot_bootsource *src, void *pdata)
{
        uint32_t v;

        v = ioread32(GPIO0_BASE + GPIO_PAD_DIR);
        v |= 1 << 4;
        iowrite32(v, GPIO0_BASE + GPIO_PAD_DIR);
        iowrite32((1 << 4), GPIO0_BASE + GPIO_WR_DATA_SET1);
        return true;
}

static void spi0_0_disable(const struct rumboot_bootsource *src, void *pdata)
{
        uint32_t v;

        v = ioread32(GPIO0_BASE + GPIO_PAD_DIR);
        v &= ~(1 << 4);
        iowrite32(v, GPIO0_BASE + GPIO_PAD_DIR);
}

static void spi0_0_cs(const struct rumboot_bootsource *src, void *pdata, int select)
{
        if (!select) {
                iowrite32(~(1 << 4), GPIO0_BASE + GPIO_WR_DATA_SET0);
        } else {
                iowrite32((1 << 4), GPIO0_BASE + GPIO_WR_DATA_SET1);
        }
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
                .enable = sdio_enable,
                .offset = 8192,
        },
        {
                .name = "SPI0 (CS: GPIO0_4)",
                .base = GSPI0_BASE,
                .freq_khz = 100000,
                .plugin = &g_bootmodule_spiflash,
                .enable = spi0_0_enable,
                .disable = spi0_0_disable,
                .chipselect = spi0_0_cs,
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
                .name = "SDIO1 (CD: GPIO0_X)",
                .base = SDIO0_BASE,
                .freq_khz = 100000,
                .plugin = &g_bootmodule_boilerplate,
        },
        {
                .name = "I2C1 (EEPROM @ 0x50)",
                .base = I2C1_BASE,
                .slave_addr = 0x50,
                .freq_khz = 100000,
                .plugin = &g_bootmodule_eeprom,
        },
        {
                .name = "I2C1 (EEPROM @ 0x51)",
                .base = I2C1_BASE,
                .slave_addr = 0x51,
                .freq_khz = 100000,
                .plugin = &g_bootmodule_eeprom,
        },
        {
                .name = "I2C1 (EEPROM @ 0x52)",
                .base = I2C1_BASE,
                .slave_addr = 0x52,
                .freq_khz = 100000,
                .plugin = &g_bootmodule_eeprom,
        },
        {
                .name = "I2C1 (EEPROM @ 0x53)",
                .base = I2C1_BASE,
                .slave_addr = 0x53,
                .freq_khz = 100000,
                .plugin = &g_bootmodule_eeprom,
        },
                /* TODO: SPI1 ? */

//g_bootmodule_boilerplate
#if 0
        {
                .name = "SDIO_0",
                .base = SDIO0_BASE,
                .freq_khz = SDIO_CLK_FREQ,
                .privdatalen = 128,
                .init = sd_init,
                .deinit = sd_deinit,
                .read = sd_read,

                .prepare = sdio_init_gpio_mux,
                .unprepare = sdio_deinit_gpio_mux,
                .retry = sd_load_again,
        },

        {
                .name = "EEPROM",
                .base = 0,
                .freq_khz = EEPROM_CLK_FREQ,
                .privdatalen = 128,
                .init = eeprom_init,
                .deinit = eeprom_deinit,
                .read = eeprom_read,
                .init_gpio_mux = eeprom_init_gpio_mux,
                .deinit_gpio_mux = eeprom_deinit_gpio_mux,
                .load_again = eeprom_load_again,
        },
#endif
        { /*Sentinel*/ }
};


/* TODO: Set pin and */
void rumboot_platform_enter_host_mode()
{

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
        /* No-op, this chip has only one core */
        return 0;
}

void rumboot_platform_print_summary(struct rumboot_config *conf)
{

}
