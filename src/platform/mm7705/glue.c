#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <rumboot/platform.h>
#include <rumboot/io.h>
#include <rumboot/boot.h>
#include <platform/devices.h>
#include <rumboot/bootsrc/sdio.h>
#include <rumboot/bootsrc/spiflash.h>
#include <devices/uart_pl011.h>
#include <regs/regs_gpio_pl061.h>
#include <rumboot/printf.h>
#include <arch/ppc_476fp_config.h>
#include <arch/ppc_476fp_lib_c.h>
#include <arch/ppc_476fp_itrpt_fields.h>

#define BOOTM_PRIMARY_CPU_ID                             (1<<0)
#define BOOTM_DISABLE_SECONDARY_CPU                      (1<<1)
#define BOOTM_DISABLE_SELFTEST                           (1<<2)
#define BOOTM_ENABLE_HOST_MODE                           (1<<3)
#define BOOTM_ENABLE_EDCL                                (1<<4)
#define BOOTM_RUN_MEM_SELFRECOVERY                       (1<<5)
#define BOOTM_ENABLE_USB_DEVICE                          (1<<6)
#define BOOTM_USE_EXTERNAL_REF_CLK_FOR_PCIE_PHY_BIST     (1<<7)
#define BOOTM_ENABLE_MODE_A                              (1<<8)
#define BOOTM_SKIP_LOADING_FROM_SD_CARD                  (1<<9)

#define IBM_BIT_INDEX(size, index)    (((size) - 1) - ((index) % (size)))
#define ITRPT_XSR_FP_e  50
#define ITRPT_XSR_FP_i IBM_BIT_INDEX(64, ITRPT_XSR_FP_e)


int64_t rumboot_virt_to_phys(volatile void *addr)
{
    return (uint32_t) addr; /* FixMe: ... */
}

uint32_t rumboot_virt_to_dma(volatile void *addr)
{
    //FixMe: Use TLB stuff here
    uint64_t ret = (uint64_t) addr; //get_physical_addr( (uint32_t)  addr, 0);
    return (uint32_t) (ret & 0xFFFFFFFF);
}


static void enable_fpu()
{
        msr_write(msr_read() | (1 << ITRPT_XSR_FP_i));
}


uint32_t rumboot_platform_get_uptime() {
    #define TIMER_TICKS_PER_US  800 /* TODO check if PPC_TMR_CLK is 5ns */

    uint32_t h = spr_read( SPR_TBU_R );
    uint32_t l = spr_read( SPR_TBL_R );    
    if (h != spr_read( SPR_TBU_R ))
        return rumboot_platform_get_uptime();

    uint64_t v = h; 
    v = (v << 32) | l;
    return v / TIMER_TICKS_PER_US;
}

#define PL011_UARTDR  0x0
#define PL011_UARTFR  0x018
#define PL011_UARTRIS 0x03C
#define PL011_TXFF_i  5
#define PL011_TXFE_i  7
#define PL011_TXRIS_i 5

static int  __attribute__((no_instrument_function)) tx_fifo_ready(uint32_t base_addr)
{
        if (ioread32(base_addr + PL011_UARTFR) & (1 << PL011_TXFE_i)) {
                return 0;
        }

        if (ioread32(base_addr + PL011_UARTRIS) & (1 << PL011_TXRIS_i)) {
                return 0;
        }

        do {
                if (!(ioread32(base_addr + PL011_UARTFR) & (1 << PL011_TXFF_i))) {
                        return 0;
                }
        } while (1);

        return -1;
}


void  __attribute__((no_instrument_function)) rumboot_platform_putchar(uint8_t c)
{
        while (tx_fifo_ready(UART0_BASE));;
        iowrite32(c, UART0_BASE + PL011_UARTDR);
}


int rumboot_platform_getchar(uint32_t timeout_us)
{
        uint32_t start = rumboot_platform_get_uptime();

        while (rumboot_platform_get_uptime() - start < timeout_us) {
                if (!uart_check_rfifo_empty(UART0_BASE)) {
                        return ioread32(UART0_BASE + PL011_UARTDR) & 0xFF;
                }
        }
        return -1;
}


void rumboot_platform_irq_init()
{
}

uint32_t rumboot_arch_irq_disable()
{
        return 0;
}

uint32_t rumboot_arch_irq_enable()
{
        return 0;
}

void rumboot_platform_init_loader(struct rumboot_config *conf)
{
        uart_rx_enable(UART0_BASE, 1);
        rumboot_printf("INIT MUX\n");
        iowrite32(0xff, LSIF1_MGPIO2_BASE + 0x420);
}

void rumboot_itrpt_hdr_base();
void rumboot_CI_hdr();
void rumboot_MC_hdr();
void rumboot_DS_hdr();
void rumboot_IS_hdr();
void rumboot_EI_hdr();
void rumboot_A_hdr();
void rumboot_P_hdr();
void rumboot_FPU_hdr();
void rumboot_SC_hdr();
void rumboot_APU_hdr();
void rumboot_DEC_hdr();
void rumboot_FIT_hdr();
void rumboot_WDT_hdr();
void rumboot_DTLBE_hdr();
void rumboot_ITLBE_hdr();
void rumboot_D_hdr();


void rumboot_platform_setup()
{
    rumboot_platform_runtime_info->silent = 0;
    /* Disable interrupts on the PPC core */
    uint32_t const msr_old_value = msr_read();
    msr_write( msr_old_value & ~((0b1 << ITRPT_XSR_CE_i)       /* MSR[CE] - Critical interrupt. */
                               | (0b1 << ITRPT_XSR_EE_i)       /* MSR[EE] - External interrupt. */
                               | (0b1 << ITRPT_XSR_ME_i)       /* MSR[ME] - Machine check. */
                               | (0b1 << ITRPT_XSR_DE_i)));    /* MSR[DE] - Debug interrupt. */

    spr_write( SPR_IVPR,    (uint32_t)&rumboot_itrpt_hdr_base & ITRPT_IVPR_ADDR_mask );    /* link irq handlers mirror */
    spr_write( SPR_IVOR0,   (uint32_t)&rumboot_CI_hdr       & ITRPT_IVORn_OFFSET_mask );
    spr_write( SPR_IVOR1,   (uint32_t)&rumboot_MC_hdr       & ITRPT_IVORn_OFFSET_mask );
    spr_write( SPR_IVOR2,   (uint32_t)&rumboot_DS_hdr       & ITRPT_IVORn_OFFSET_mask );
    spr_write( SPR_IVOR3,   (uint32_t)&rumboot_IS_hdr       & ITRPT_IVORn_OFFSET_mask );
    spr_write( SPR_IVOR4,   (uint32_t)&rumboot_EI_hdr       & ITRPT_IVORn_OFFSET_mask );
    spr_write( SPR_IVOR5,   (uint32_t)&rumboot_A_hdr        & ITRPT_IVORn_OFFSET_mask );
    spr_write( SPR_IVOR6,   (uint32_t)&rumboot_P_hdr        & ITRPT_IVORn_OFFSET_mask );
    spr_write( SPR_IVOR7,   (uint32_t)&rumboot_FPU_hdr      & ITRPT_IVORn_OFFSET_mask );
    spr_write( SPR_IVOR8,   (uint32_t)&rumboot_SC_hdr       & ITRPT_IVORn_OFFSET_mask );
    spr_write( SPR_IVOR9,   (uint32_t)&rumboot_APU_hdr      & ITRPT_IVORn_OFFSET_mask );
    spr_write( SPR_IVOR10,  (uint32_t)&rumboot_DEC_hdr      & ITRPT_IVORn_OFFSET_mask );
    spr_write( SPR_IVOR11,  (uint32_t)&rumboot_FIT_hdr      & ITRPT_IVORn_OFFSET_mask );
    spr_write( SPR_IVOR12,  (uint32_t)&rumboot_WDT_hdr      & ITRPT_IVORn_OFFSET_mask );
    spr_write( SPR_IVOR13,  (uint32_t)&rumboot_DTLBE_hdr    & ITRPT_IVORn_OFFSET_mask );
    spr_write( SPR_IVOR14,  (uint32_t)&rumboot_ITLBE_hdr    & ITRPT_IVORn_OFFSET_mask );
    spr_write( SPR_IVOR15,  (uint32_t)&rumboot_D_hdr        & ITRPT_IVORn_OFFSET_mask );

    rumboot_irq_register_mpic128();

    msr_write( msr_old_value );
    enable_fpu();
    extern char rumboot_platform_heap_start;
    extern char rumboot_platform_heap_end;
    rumboot_malloc_register_heap( "IM0", &rumboot_platform_heap_start, &rumboot_platform_heap_end );

}

static bool sdio_enable(const struct rumboot_bootsource *src, void *pdata)
{
        uint32_t afsel;

#ifdef MM7705_USE_MPW
        iowrite32(0xff, 0x3c067000 + 0x420);
        iowrite32(0xff, 0x3C040000 + 0x420);
        iowrite32(0xff, 0x3C060000 + 0x420);
        iowrite32(0xff, 0x3C061000 + 0x420);
        iowrite32(0xff, 0x3C062000 + 0x420);
        iowrite32(0xff, 0x3C063000 + 0x420);
        iowrite32(0xff, 0x3C064000 + 0x420);
        iowrite32(0xff, 0x3C065000 + 0x420);
        iowrite32(0xff, 0x3C066000 + 0x420);
        iowrite32(0xff, 0x3C067000 + 0x420);
#else
        iowrite32(0xff, LSIF1_MGPIO4_BASE + 0x420);
#endif
        return true;
}

static void sdio_disable(const struct rumboot_bootsource *src, void *pdata)
{
}

#define BOOT_SPI_BASE SPI_CTRL0__
#define BOOT_GPIO_FOR_SPI_BASE  GPIO1_BASE
#define BOOT_GPIO_FOR_SPI_PIN  2
#define SPI_CS0 (1 << BOOT_GPIO_FOR_SPI_PIN)

static void mm7705_cs(const struct rumboot_bootsource *src, void *pdata, int select)
{
    if (!select) {
            iowrite32(0, BOOT_GPIO_FOR_SPI_BASE + GPIO_DATA + (SPI_CS0 << 2));
    } else {
            iowrite32(SPI_CS0, BOOT_GPIO_FOR_SPI_BASE + GPIO_DATA + (SPI_CS0 << 2));
    }
}

static bool mm7705_spi_enable(const struct rumboot_bootsource *src, void *pdata)
{
    uint8_t afsel;
    iowrite32(0xff, LSIF1_MGPIO3_BASE + 0x420 );

    afsel = ioread32(BOOT_GPIO_FOR_SPI_BASE + 0x420);
    afsel &= ~(1 << BOOT_GPIO_FOR_SPI_PIN);
    iowrite32(afsel, BOOT_GPIO_FOR_SPI_BASE + 0x420 );

    uint8_t dir = ioread32(BOOT_GPIO_FOR_SPI_BASE + GPIO_DIR);
    dir |= (1 << BOOT_GPIO_FOR_SPI_PIN);
    iowrite32(dir, BOOT_GPIO_FOR_SPI_BASE + GPIO_DIR);

}

static bool mm7705_spi_disable(const struct rumboot_bootsource *src, void *pdata)
{

    uint8_t afsel;
    afsel = ioread32(LSIF1_MGPIO3_BASE + 0x420);
    afsel &= ~0b01110000;
    iowrite32(afsel, LSIF0_MGPIO3_BASE + 0x420);

    uint8_t dir = ioread32(BOOT_GPIO_FOR_SPI_BASE + GPIO_DIR);
    dir &= ~(1 << BOOT_GPIO_FOR_SPI_PIN);
    iowrite32(dir, BOOT_GPIO_FOR_SPI_BASE + GPIO_DIR);

}


static const struct rumboot_bootsource arr[] = {
        {
                .name = "SDIO (CD: GPIO1_X)",
                .base = SDIO_BASE,
                .base_freq_khz = 100000,
                .iface_freq_khz = 10000,
                .plugin = &g_bootmodule_sdio,
                .enable = sdio_enable,
                .disable = sdio_disable,
                .offset = 16384,
        },
        {
                .name = "SPI FLASH",
                .base = SPI_CTRL0_BASE,
                .base_freq_khz = 30000,
                .plugin = &g_bootmodule_spiflash,
		.enable = mm7705_spi_enable,
		.disable = mm7705_spi_disable,
		.chipselect = mm7705_cs
        },
        { /*Sentinel*/ }
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

void *rumboot_platform_get_spl_area(size_t *size)
{
        *size = (&rumboot_platform_spl_end - &rumboot_platform_spl_start);
        return (void *)&rumboot_platform_spl_start;
}

int rumboot_platform_exec(struct rumboot_bootheader *hdr, int swap)
{
        /* No-op, this chip has only one core */
        rumboot_printf("EP: %x\n", hdr->entry_point[0]);
        return rumboot_bootimage_execute_ep((void *) hdr->entry_point[0]);
}

void rumboot_platform_read_config(struct rumboot_config *conf)
{
        uint32_t bootm = ioread32(SCTL_BASE + 0x0);
        rumboot_printf("BOOTM:           0x%x\n", bootm);
        conf->baudrate = 1000000;
        conf->hostmode = bootm & BOOTM_ENABLE_EDCL;
}


int rumboot_platform_selftest(struct rumboot_config *conf)
{
        /* Execute selftest routines */
        return 0;
}

void rumboot_platform_print_summary(struct rumboot_config *conf)
{
}

void rumboot_platform_enter_host_mode(struct rumboot_config *conf)
{

}
