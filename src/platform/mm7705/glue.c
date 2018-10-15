#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <rumboot/platform.h>
#include <rumboot/io.h>
#include <rumboot/boot.h>
#include <platform/devices.h>
#include <rumboot/bootsrc/sdio.h>
#include <devices/uart_pl011.h>
#include <rumboot/printf.h>

#define IBM_BIT_INDEX(size, index)    (((size) - 1) - ((index) % (size)))
#define ITRPT_XSR_FP_e  50
#define ITRPT_XSR_FP_i IBM_BIT_INDEX(64, ITRPT_XSR_FP_e)

void msr_write(uint32_t const wval)
{
        __asm volatile
        (
                "mtmsr %0 \n\t"
                ::"r" (wval)
        );
}

uint32_t msr_read()
{
        uint32_t rval = 0;
        __asm volatile
        (
                "mfmsr %0 \n\t"
                : "=r" (rval)
        );

        return rval;
}

static void enable_fpu()
{
        msr_write(msr_read() | (1 << ITRPT_XSR_FP_i));
}


/* Platform-specific glue */
uint32_t rumboot_platform_get_uptime()
{
#define TIMER_TICKS_PER_US  800
#define SPR_TBL_R           0x10C

        uint32_t value = 0;

        __asm volatile
        (
                "mfspr %0, %1 \n\t"
                : "=r" (value)
                : "i" (SPR_TBL_R)
                :
        );

        return value / TIMER_TICKS_PER_US;
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
        if (c == '\n') {
                rumboot_platform_putchar('\r');
        }

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
}

void rumboot_platform_setup()
{
       enable_fpu();
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


static const struct rumboot_bootsource arr[] = {
        {
                .name = "SDIO (CD: GPIO1_X)",
                .base = SDIO_BASE,
                .freq_khz = 100000,
                .plugin = &g_bootmodule_sdio,
                .enable = sdio_enable,
                .disable = sdio_disable,
                .offset = 16384,
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

int rumboot_platform_exec(struct rumboot_bootheader *hdr)
{
        /* No-op, this chip has only one core */
        return rumboot_bootimage_execute_ep((void *) hdr->entry_point[0]);
}

void rumboot_platform_read_config(struct rumboot_config *conf)
{
        conf->baudrate = 115200;
        conf->hostmode = 0;
}


void rumboot_platform_selftest(struct rumboot_config *conf)
{
        /* Execute selftest routines */
}

void rumboot_platform_print_summary(struct rumboot_config *conf)
{
}

void rumboot_platform_enter_host_mode()
{

}
