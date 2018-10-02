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
#include <platform/regs/sctl.h>
#include <rumboot/boot.h>
#include <rumboot/bootsrc/spiflash.h>
#include <rumboot/bootsrc/physmap.h>
#include <rumboot/bootsrc/sdio.h>
#include <rumboot/bootsrc/boilerplate.h>

#define BOOTM_SELFTEST     (1 << 1)
#define BOOTM_HOST         (1 << 2)
#define BOOTM_FASTUART     (1 << 3)
#define BOOTM_SDIO_CD      (1 << 4)
#define BOOTM_CPU_ECC      (1 << 5)
#define BOOTM_EMI_ECC      (1 << 6)
#define BOOTM_NOR_BOOT     (1 << 7)

void rumboot_platform_read_config(struct rumboot_config *conf)
{
        uint32_t bootm = dcr_read(DCR_SCTL_BASE + SCTL_BOOTM);

        /* TODO: 19200 & 9600 */
        if (bootm & BOOTM_FASTUART) {
                conf->baudrate = 6250000;
        } else {
                conf->baudrate = 115200;
        }
        conf->hostmode = (bootm & BOOTM_HOST);
        conf->selftest = (bootm & BOOTM_SELFTEST);
}

void rumboot_platform_print_summary(struct rumboot_config *conf)
{
        uint32_t bootm = dcr_read(DCR_SCTL_BASE + SCTL_BOOTM);

        rumboot_printf("CPU ECC:         %s\n",
                bootm & BOOTM_CPU_ECC ? "enabled" : "disabled");

        rumboot_printf("NOR/SRAM ECC:    %s\n",
                bootm & BOOTM_EMI_ECC ? "disabled" : "enabled");

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
#include <platform/devices/emi.h>
#include <platform/regs/fields/emi.h>
#include <platform/regs/regs_emi.h>
#include <platform/devices/plb6mcif2.h>

static bool emi_enable(const struct rumboot_bootsource *src, void *pdata)
{
        plb6mcif2_simple_init( DCR_EM2_PLB6MCIF2_BASE,  0x00);


        emi_bank_cfg b5_cfg =
    {
        //SS5
        {
            BTYP_NOR,
            PTYP_NO_PAGES,
            SRDY_EXT_RDY_NOT_USE,
            TWR_0,
            SST_Flow_Through,
            TSSOE_1,
            TSOE_1,
            TCYC_8,
            0, //T_RDY
            TDEL_0
        },
        //SD5
        {
            CSP_256,
            SDS_2M,
            CL_3,
            TRDL_1,
            SI_EXT_INIT,
            TRCD_5,
            TRAS_9
        }
    };
    emi_set_bank_cfg(DCR_EM2_PLB6MCIF2_BASE, emi_b5_nor, &b5_cfg);
    dcr_write(DCR_EM2_EMI_BASE + EMI_FLCNTRL, 0x17);
    emi_set_ecc (DCR_EM2_PLB6MCIF2_BASE, emi_bank_all, emi_ecc_off);
    dcr_write(DCR_EM2_PLB6MCIF2_BASE + EMI_BUSEN, 0x01);

    return true;
}


static const struct rumboot_bootsource arr[] = {
        {
                .name = "SDIO (CD: X)",
                .base = SDIO_0_BASE,
                .freq_khz = 100000,
                .plugin = &g_bootmodule_boilerplate,
        },
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
        {
                .name = "NOR (CS: 5)",
                .base = NOR_BASE,
                .enable = emi_enable,
                .plugin = &g_bootmodule_physmap,
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
