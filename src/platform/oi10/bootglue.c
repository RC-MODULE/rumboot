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
#include <devices/gpio_pl061.h>
#include <platform/devices.h>
#include <platform/regs/sctl.h>
#include <rumboot/boot.h>
#include <rumboot/bootsrc/spiflash.h>
#include <rumboot/bootsrc/physmap.h>
#include <rumboot/bootsrc/sdio.h>
#include <rumboot/bootsrc/boilerplate.h>
#include <regs/regs_gpio_pl061.h>

#define BOOTM_SLOWUART     (1 << 0)
#define BOOTM_HOST         (1 << 1)
#define BOOTM_FASTUART     (1 << 2)
#define BOOTM_SDIO_CD      (1 << 3)
#define BOOTM_SPI_CS       (1 << 4)
#define BOOTM_CPU_ECC      (1 << 5)
#define BOOTM_EMI_ECC      (1 << 6)
#define BOOTM_NOR_BOOT     (1 << 7)

void rumboot_platform_read_config(struct rumboot_config *conf)
{
        uint32_t bootm = dcr_read(DCR_SCTL_BASE + SCTL_BOOTM);

        if (bootm & BOOTM_SLOWUART) {
                if (bootm & BOOTM_FASTUART) {
                        conf->baudrate = 6250000;
                } else {
                        conf->baudrate = 115200;
                }
        } else {
                if (bootm & BOOTM_FASTUART) {
                        conf->baudrate = 19200;
                } else {
                        conf->baudrate = 9600;
                }
        }
        conf->hostmode = (bootm & BOOTM_HOST);
        conf->legacyboot = 0;
        conf->selftest = -1;
        conf->edcl = 1;
}

void rumboot_platform_print_summary(struct rumboot_config *conf)
{
        uint32_t bootm = dcr_read(DCR_SCTL_BASE + SCTL_BOOTM);

        rumboot_printf("SD Card:         %s\n",
                       bootm & BOOTM_SDIO_CD ? "Not inserted" : "Inserted");

        rumboot_printf("CPU ECC:         %s\n",
                       bootm & BOOTM_CPU_ECC ? "enabled" : "disabled");

        rumboot_printf("NOR/SRAM ECC:    %s\n",
                       bootm & BOOTM_EMI_ECC ? "disabled" : "enabled");

        rumboot_printf("Direct NOR boot: %s\n",
                       bootm & BOOTM_NOR_BOOT ? "disabled" : "enabled");

        uint32_t v = ioread32(GPIO_0_BASE + GPIO_DIR);
        iowrite32(v | BOOTM_HOST, GPIO_0_BASE + GPIO_DIR);

}

void rumboot_platform_selftest(struct rumboot_config *conf)
{
        /* Execute selftest routines */
}




static bool spi0_gcs_enable(const struct rumboot_bootsource *src, void *pdata)
{
        uint32_t v = ioread32(GPIO_0_BASE + GPIO_DIR);
        iowrite32(v | 1<<4, GPIO_0_BASE + GPIO_DIR);
        iowrite32(1 << 4, GPIO_0_BASE + GPIO_DATA + (1 << (4 + 2)));
        return true;
}

static void spi0_gcs_disable(const struct rumboot_bootsource *src, void *pdata)
{
        uint32_t v = ioread32(GPIO_0_BASE + GPIO_DIR);
        v &= ~(1<<4);
        iowrite32(v, GPIO_0_BASE + GPIO_DIR);
        iowrite32(1 << 4, GPIO_0_BASE + GPIO_DATA + (1 << (4 + 2)));
}

static void spi0_gcs(const struct rumboot_bootsource *src, void *pdata, int select)
{
        iowrite32(select << 4, GPIO_0_BASE + (1 << (4 + 2)));
}

#include <platform/devices/emi.h>
#include <platform/regs/fields/emi.h>
#include <platform/regs/regs_emi.h>
#include <platform/devices/plb6mcif2.h>

static bool emi_enable(const struct rumboot_bootsource *src, void *pdata)
{
        plb6mcif2_simple_init(DCR_EM2_PLB6MCIF2_BASE, 0x00);
        #if 0
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
        emi_set_ecc(DCR_EM2_PLB6MCIF2_BASE, emi_bank_all, emi_ecc_off);
        dcr_write(DCR_EM2_PLB6MCIF2_BASE + EMI_BUSEN, 0x01);
        #endif
        return true;
}

static bool sdio_enable(const struct rumboot_bootsource *src, void *pdata)
{
        gpio_set_direction(GPIO_0_BASE, GPIO_PIN_3, direction_in);
        return !(gpio_get_data(GPIO_0_BASE) & BOOTM_SDIO_CD);
}

static const struct rumboot_bootsource arr[] = {
        {
                .name = "SDIO (CD: GPIO0_3)",
                .base = SDIO_0_BASE,
                .freq_khz = 100000,
                .plugin = &g_bootmodule_sdio,
                .enable = sdio_enable,
                .offset = 8192,
        },
        {
                .name = "NOR (CS: 5)",
                .base = NOR_BASE,
                .enable = emi_enable,
                .plugin = &g_bootmodule_physmap,
        },
        {
                .name = "SPI0 (CS: internal)",
                .base = GSPI_0_BASE,
                .freq_khz = 100000,
                .plugin = &g_bootmodule_spiflash,
        },
        {
                .name = "SPI0 (CS: GPIO0_4)",
                .base = GSPI_0_BASE,
                .freq_khz = 100000,
                .plugin = &g_bootmodule_spiflash,
                .enable = spi0_gcs_enable,
                .disable = spi0_gcs_disable,
                .chipselect = spi0_gcs,
        },
};



union u64 {
        uint8_t bytes[8];
        uint64_t dword;
};

union u32 {
    uint8_t bytes[4];
    uint32_t word;
};

#define EDCL_IP                             0x1C
#define EDCL_MAC_MSB                        0x28
#define EDCL_MAC_LSB                        0x2C

static void dump_greth_parameters(int i, uintptr_t base)
{
        union u64 macaddr;
        union u32 ip;
        macaddr.dword = ioread64(base + EDCL_MAC_MSB);
        ip.word = ioread32(base + EDCL_IP);
        dbg_boot(NULL, "GRETH%d EDCL MAC: %x:%x:%x:%x:%x:%x IP: %d.%d.%d.%d",
                i,
                macaddr.bytes[2],
                macaddr.bytes[3],
                macaddr.bytes[4],
                macaddr.bytes[5],
                macaddr.bytes[6],
                macaddr.bytes[7],
                ip.bytes[0],
                ip.bytes[1],
                ip.bytes[2],
                ip.bytes[3]
        );

}

void rumboot_platform_enter_host_mode()
{
        dump_greth_parameters(0, GRETH_0_BASE);
        dump_greth_parameters(1, GRETH_1_BASE);

        /* TODO: Set BOOT PIN */
        iowrite32(BOOTM_HOST, GPIO_0_BASE + GPIO_DATA + (BOOTM_HOST << 2));
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
        return rumboot_bootimage_execute_ep((void *) hdr->entry_point[0]);
}

void *rumboot_platform_get_spl_area(size_t *size)
{
        *size = (&rumboot_platform_spl_end - &rumboot_platform_spl_start);
        return (void *)&rumboot_platform_spl_start;
}
