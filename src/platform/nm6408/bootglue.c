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

void rumboot_platform_init_loader(struct rumboot_config *conf)
{
  

}

int rumboot_platform_selftest(struct rumboot_config *conf)
{
        return 0;
}

void rumboot_platform_read_config(struct rumboot_config *conf)
{
        conf->hostmode = 1;
        conf->legacyboot = 0;
        conf->selftest = 0;
        conf->edcl = 1;
}

void rumboot_platform_print_summary(struct rumboot_config *conf)
{


}


static const struct rumboot_bootsource arr[] = {
        {
                .name = "SPI0 (CS: 0)",
                .base = PL022_SSP_BASE,
                .base_freq_khz = 100000,
                .iface_freq_khz = 12500,
                .offset = 8192,
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

int rumboot_platform_exec(struct rumboot_bootheader *hdr)
{
        return rumboot_bootimage_execute_ep((void *) hdr->entry_point[0]);
}

void *rumboot_platform_get_spl_area(size_t *size)
{
        *size = (&rumboot_platform_spl_end - &rumboot_platform_spl_start);
        return (void *)&rumboot_platform_spl_start;
}

#ifndef CMAKE_BUILD_TYPE_DEBUG
void  __attribute__((no_instrument_function)) rumboot_platform_putchar(uint8_t c)
{
    while (!pl022_tx_avail(PL022_SSP_BASE));;
    iowrite32( 0x100 | c, PL022_SSP_BASE + 0x8);
    while (!pl022_rx_empty(PL022_SSP_BASE)) {
        ioread32(PL022_SSP_BASE + 0x8);
    }
}

int rumboot_platform_getchar(uint32_t timeout_us)
{
        uint32_t start = rumboot_platform_get_uptime();
        /* Make sure no leftover data in fifo's */
        while (!pl022_tx_empty(PL022_SSP_BASE));;
        while (!pl022_rx_empty(PL022_SSP_BASE)) {
                ioread32(PL022_SSP_BASE + 0x8);
        }        

        while (rumboot_platform_get_uptime() - start < timeout_us) {
                iowrite32(0x0, PL022_SSP_BASE + 0x8);
                while(pl022_rx_empty(PL022_SSP_BASE));;
                uint32_t byte = ioread32(PL022_SSP_BASE + 0x8);
                if ((byte & (1<<8)) == 0)
                        return (int) byte & 0xff;
        }
        return -1;
}
#endif
