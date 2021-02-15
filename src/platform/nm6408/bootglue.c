#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <rumboot/platform.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/rumboot.h>
#include <platform/devices.h>
#include <rumboot/boot.h>
#include <rumboot/bootsrc/spiflash.h>
#include <rumboot/bootsrc/physmap.h>
#include <devices/pl022.h>
#include <rumboot/bootsrc/sdio.h>
#include <rumboot/bootsrc/boilerplate.h>
#include <devices/greth_edcl.h>
#include <rumboot/timer.h>
#include <regs/regs_spi.h>

void rumboot_platform_init_loader(struct rumboot_config *conf)
{
  

}

int rumboot_platform_selftest(struct rumboot_config *conf)
{
        return 0;
}

void rumboot_platform_read_config(struct rumboot_config *conf)
{
        uint32_t bootm = ioread32(0x000CB004);
        conf->hostmode = bootm & (1<<2);
        conf->legacyboot = 0;
        conf->selftest = 0;
        conf->baudrate = 115200;
        conf->edcl = 1;
}

void rumboot_platform_print_summary(struct rumboot_config *conf)
{
        uint32_t bootm = ioread32(0x000CB004);        
        rumboot_printf("BOOTM:            0:%x 1:%x 2:%x\n", 
                (bootm & (1<<0)) ? 1 : 0,
                (bootm & (1<<1)) ? 1 : 0,
                (bootm & (1<<2)) ? 1 : 0                
                );
}


static const struct rumboot_bootsource arr[] = {
        {
                .name = "SPI0 (CS: 0)",
                .base = PL022_SSP_BASE,
                .base_freq_khz = 100000,
                .iface_freq_khz = 12500,
                .offset = 16384,
                .plugin = &g_bootmodule_spiflash,
        },
        { /* sentinel */ }
};


void rumboot_platform_enter_host_mode(struct rumboot_config *conf)
{
        greth_dump_edcl_params(0, GRETH_BASE);
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

int rumboot_platform_exec(struct rumboot_bootheader *hdr, int swap)
{
        return rumboot_bootimage_execute_ep((void *) hdr->entry_point32[0]);
}

void *rumboot_platform_get_spl_area(size_t *size)
{
        *size = (&rumboot_platform_spl_end - &rumboot_platform_spl_start);
        return (void *)&rumboot_platform_spl_start;
}

void pl022_context(uintptr_t base, int save) 
{
        static uint32_t cr0, cr1, cpsr, cs;
        if (save) {
                cr0 = ioread32(base + PL022_CR0);
                cr1 = ioread32(base + PL022_CR1);
                cpsr = ioread32(base + PL022_CPSR);
                cs = ioread32(PL022_SSP_BASE + 0x140);
        } else { 
                if (cs == ioread32(base + 0x140))
                        return; /* Nothing to restore */
                iowrite32(cr0, base + PL022_CR0);
                iowrite32(cr1, base + PL022_CR1);
                iowrite32(cpsr, base + PL022_CPSR);
                iowrite32(cs, base + 0x140);
        }
}

#ifndef CMAKE_BUILD_TYPE_DEBUG
void  __attribute__((no_instrument_function)) rumboot_platform_putchar(uint8_t c)
{

    while (!pl022_tx_avail(PL022_SSP_BASE));;
    iowrite32( 0x100 | c, PL022_SSP_BASE + 0x8);

    if (!pl022_rx_empty(PL022_SSP_BASE))
        pl022_clear_rx_buf(PL022_SSP_BASE);
        
    mdelay(1);
}

int rumboot_platform_getchar(uint32_t timeout_us)
{
        uint32_t start = rumboot_platform_get_uptime();
        /* Make sure no leftover data in fifo's */
        pl022_clear_rx_buf(PL022_SSP_BASE);

        while (rumboot_platform_get_uptime() - start < timeout_us) {
                mdelay(1);                
                iowrite32(0x0, PL022_SSP_BASE + 0x8);
                while(pl022_rx_empty(PL022_SSP_BASE));;
                uint32_t byte = ioread32(PL022_SSP_BASE + 0x8);
                if ((byte & (1<<8)))
                        return (int) byte & 0xff;                
        }
        mdelay(1);
        return -1;
}
#endif
