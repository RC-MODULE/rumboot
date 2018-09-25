#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <rumboot/platform.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/rumboot.h>
#include <platform/arch/ppc/ppc_476fp_config.h>
#include <platform/arch/ppc/ppc_476fp_lib_c.h>
#include <platform/arch/ppc/ppc_476fp_itrpt_fields.h>
#include <platform/ppc470s/mmu.h>
#include <platform/devices/mpic128.h>
#include <platform/devices.h>
#include <rumboot/boot.h>
#include <rumboot/bootsrc/spiflash.h>


void rumboot_platform_read_config(struct rumboot_config *conf)
{
}

void rumboot_platform_selftest(struct rumboot_config *conf)
{
        /* Execute selftest routines */
}


static bool spi0_gcs_enable(const struct rumboot_bootsource *src, void *pdata)
{
        return true;
//        uint32_t v;
//
//        v = ioread32(GPIO0_BASE + GPIO_PAD_DIR);
//        v &= ~(1 << 4);
//        iowrite32(v, GPIO0_BASE + GPIO_PAD_DIR);
}

static void spi0_gcs_disable(const struct rumboot_bootsource *src, void *pdata)
{
//        uint32_t v;
//
//        v = ioread32(GPIO0_BASE + GPIO_PAD_DIR);
//        v &= ~(1 << 4);
//        iowrite32(v, GPIO0_BASE + GPIO_PAD_DIR);
}

static void spi0_gcs(const struct rumboot_bootsource *src, void *pdata, int select)
{
//        if (!select) {
//                iowrite32(~(1 << 4), GPIO0_BASE + GPIO_WR_DATA_SET0);
//        } else {
//                iowrite32((1 << 4), GPIO0_BASE + GPIO_WR_DATA_SET1);
//        }
}


static const struct rumboot_bootsource arr[] = {
        {
                .name = "SPI0 (CS: internal)",
                .base = GSPI_0_BASE,
                .freq_khz = 100000,
                .plugin = &g_bootmodule_spiflash,
        },
        {
                .name = "SPI0 (CS: GPIO0_5)",
                .base = GSPI_0_BASE,
                .freq_khz = 100000,
                .plugin = &g_bootmodule_spiflash,
                .enable = spi0_gcs_enable,
                .disable = spi0_gcs_disable,
                .chipselect = spi0_gcs,
        },
};


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

void *rumboot_platform_get_spl_area(size_t *size)
{
        *size = (&rumboot_platform_spl_end - &rumboot_platform_spl_start);
        return (void *)&rumboot_platform_spl_start;
}
