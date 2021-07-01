#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <rumboot/platform.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/rumboot.h>
#include <arch/ppc_476fp_config.h>
#include <arch/ppc_476fp_lib_c.h>
#include <arch/ppc_476fp_itrpt_fields.h>
#include <arch/mmu.h>
#include <devices/mpic128.h>
#include <devices/gpio_pl061.h>
#include <platform/devices.h>
#include <platform/regs/sctl.h>
#include <rumboot/boot.h>
#include <rumboot/bootsrc/spiflash.h>
#include <rumboot/bootsrc/physmap.h>
#include <rumboot/bootsrc/sdio.h>
#include <rumboot/bootsrc/boilerplate.h>
#include <platform/bootm.h>
#include <regs/regs_gpio_pl061.h>
#include <regs/regs_uart_pl011.h>

#include <devices/ugly/plb6mcif2.h>
#include <devices/uart_pl011.h>
#include <rumboot/irq.h>
#include <devices/greth_edcl.h>

void rumboot_platform_init_loader(struct rumboot_config *conf)
{
        /* Disable EDCL to avoid seecurity breaches */
        greth_edcl_configure(GRETH_0_BASE, 0);
        greth_edcl_configure(GRETH_1_BASE, 0);

        /* Set HOST pin to output */
        uint32_t v = ioread32(GPIO_0_BASE + GPIO_DIR);
        iowrite32(v | BOOTM_HOST, GPIO_0_BASE + GPIO_DIR);

        /* Initialize UART */
        struct uart_init_params sparams =
        {
            .wlen = UART_word_length_8bit,
            .uart_sys_freq_hz = UART_SYS_FREQ_HZ,
            .parity = UART_parity_no,
            .rx_fifo_level = UART_RX_FIFO_LEVEL_GT_7_8,
            .tx_fifo_level = UART_TX_FIFO_LEVEL_LT_1_8,
            .int_mask = 0x0,
            .use_rts_cts = false,
            .loopback = false
        };
        sparams.baud_rate = conf->baudrate;
        uart_init(UART0_BASE, &sparams);
        uart_tx_enable(UART0_BASE, true);
        uart_rx_enable(UART0_BASE, true);
        uart_enable(UART0_BASE, true);

        /* Send sync byte. This way it will work for debug builds */
        iowrite32(0x55, UART0_BASE + UARTDR);
}

void rumboot_platform_read_config(struct rumboot_config *conf)
{
        uint32_t bootm = dcr_read(DCR_SCTL_BASE + SCTL_SYS_BOOT_CFG);

        if (! (bootm & BOOTM_SLOWUART)) {
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
        conf->selftest = (bootm & BOOTM_SELFTEST);
        conf->edcl = 1;
        conf->secure_boot = 0;
}

void rumboot_platform_print_summary(struct rumboot_config *conf)
{
        uint32_t bootm = dcr_read(DCR_SCTL_BASE + SCTL_SYS_BOOT_CFG);

        rumboot_printf("SD Card:         %s\n",
                       (bootm & BOOTM_SDIO_CD) ? "Not inserted" : "Inserted");

        rumboot_printf("IM0 Scrubber:    %s\n",
                       (bootm & BOOTM_CPU_ECC) ? "enabled" : "disabled");

        rumboot_printf("NOR/SRAM ECC:    %s\n",
                       (bootm & BOOTM_EMI_ECC) ? "enabled" : "disabled");

        rumboot_printf("Direct NOR boot: %s\n",
                       (bootm & BOOTM_NOR_BOOT) ? "disabled" : "enabled");

        rumboot_printf("Reset cause:     SCTL: 0x%x SPR_DBCR0: 0x%x\n",
                        dcr_read(DCR_CRG_BASE + 0x40), spr_read(SPR_DBCR0));

}


static bool emi_enable(const struct rumboot_bootsource *src, void *pdata)
{
        plb6mcif2_simple_init(DCR_EM2_PLB6MCIF2_BASE, 0x00);
        return true;
}

static bool sdio_enable(const struct rumboot_bootsource *src, void *pdata)
{
        gpio_set_direction(GPIO_0_BASE, 3, gpio_pin_direction_in);
        return !(gpio_get_data(GPIO_0_BASE) & BOOTM_SDIO_CD);
}


static const struct rumboot_bootsource arr[] = {
        {
                .name = "SDIO (CD: GPIO0_3)",
                .base = SDIO_0_BASE,
                .base_freq_khz = 100000,
                .iface_freq_khz = 10000,
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
                .base_freq_khz = 100000,
                .iface_freq_khz = 12500,
                .plugin = &g_bootmodule_spiflash,
        },
        { /* sentinel */ }
};


void rumboot_platform_enter_host_mode(struct rumboot_config *conf)
{
        greth_dump_edcl_params(0, GRETH_0_BASE);
        greth_dump_edcl_params(1, GRETH_1_BASE);
        greth_edcl_configure(GRETH_0_BASE, 1);
        greth_edcl_configure(GRETH_1_BASE, 1);
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



static const tlb_entry little_endian_tlb[] =
{
 { MMU_TLB_ENTRY(  0x020,  0xC0000,    0x80020,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_LITTLE_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_3,     0b1 )},
 { MMU_TLB_ENTRY(  0x020,  0xC0010,    0x80030,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_LITTLE_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_4,     0b1 )},
 { MMU_TLB_ENTRY(  0x010,  0x80000,    0x80000,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_LITTLE_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_1,     0b1 )},
 { MMU_TLB_ENTRY(  0x010,  0x80010,    0x80010,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_LITTLE_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_2,     0b1 )},
};


static const tlb_entry big_endian_tlb[] =
{
 { MMU_TLB_ENTRY(  0x020,  0xC0000,    0x80020,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_3,     0b1 )},
 { MMU_TLB_ENTRY(  0x020,  0xC0010,    0x80030,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_4,     0b1 )},
 { MMU_TLB_ENTRY(  0x010,  0x80000,    0x80000,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_1,     0b1 )},
 { MMU_TLB_ENTRY(  0x010,  0x80010,    0x80010,    MMU_TLBE_DSIZ_64KB,     0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_0,    MMU_TLBWE_BE_2,     0b1 )},
};

#include <rumboot/hexdump.h>

int rumboot_platform_exec(struct rumboot_bootheader *hdr, int swap)
{
        /* Make sure PID is 0 */
        set_mem_window(MEM_WINDOW_SHARED);
        uint32_t ep = hdr->entry_point[0];

        if (swap) {
                write_tlb_entries(little_endian_tlb, ARRAY_SIZE(little_endian_tlb));
                ep = __swap32(ep);
        }

        int ret = rumboot_bootimage_execute_ep((void *) ep);

        if (swap) {
                write_tlb_entries(big_endian_tlb, ARRAY_SIZE(big_endian_tlb));
        }
        return ret;
}

void *rumboot_platform_get_spl_area(size_t *size)
{
        *size = (&rumboot_platform_spl_end - &rumboot_platform_spl_start);
        return (void *)&rumboot_platform_spl_start;
}

#ifndef CMAKE_BUILD_TYPE_DEBUG
void  __attribute__((no_instrument_function)) rumboot_platform_putchar(uint8_t c)
{
        while (uart_check_tfifo_full(UART0_BASE));;
        iowrite32(c, UART0_BASE + UARTDR);
}


int rumboot_platform_getchar(uint32_t timeout_us)
{
        uint32_t start = rumboot_platform_get_uptime();

        do {
                if (!uart_check_rfifo_empty(UART0_BASE)) {
                        return ioread32(UART0_BASE + UARTDR) & 0xFF;
                }
        } while (rumboot_platform_get_uptime() - start < timeout_us);
        return -1;
}
#endif
