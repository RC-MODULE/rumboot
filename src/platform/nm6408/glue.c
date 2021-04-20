#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <rumboot/platform.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/rumboot.h>
#include <platform/devices.h>
#include <rumboot/boot.h>
#include <rumboot/irq.h>
#include <devices/sp804.h>
#include <devices/pl022.h>
#include <rumboot/timer.h>

int64_t rumboot_virt_to_phys(volatile void *addr)
{
    return (int64_t) ((uintptr_t) addr);
}

uint32_t rumboot_virt_to_dma(volatile const void *addr)
{
    return (uint32_t) addr;
}

/* Platform-specific glue */
uint32_t rumboot_platform_get_uptime() {
        return (0xFFFFFFFF - sp804_get_value(SP804_BASE, 0)) / 12.5;
}


extern char rumboot_IRAM_heap_start;
extern char rumboot_IRAM_heap_end;

void pl022_context(uintptr_t base, int save);
void rumboot_platform_setup() {
#ifndef RUMBOOT_ONLY_STACK
        rumboot_malloc_register_heap("IRAM",
                                     &rumboot_IRAM_heap_start, &rumboot_IRAM_heap_end);
#endif
    rumboot_irq_set_exception_handler(rumboot_arch_exception);


    struct sp804_conf conf_str;
    conf_str.mode = FREERUN;
    conf_str.interrupt_enable = 0;
    conf_str.clock_division = 16;
    conf_str.width = 32;
    conf_str.load = 0xFFFFFFFF;
    conf_str.bgload = 0xFFFFFFFF;
    sp804_config(SP804_BASE, &conf_str, 0);
    sp804_enable(SP804_BASE, 0); 

    struct pl022_config conf;
    conf.ssp_clk = 200000000UL;
    conf.spi_clk = 115200;
    conf.data_size = 16;
    conf.soft_cs = 0;
    conf.variant = PL022_VARIANT_SHELUHIN;

    struct ssp_params prms;
    prms.cpol = 1;
    prms.cpha = 1;
    prms.mode = master_mode;
    prms.fr_format = ssp_motorola_fr_form;

    pl022_init(PL022_SSP_BASE, &conf);

    prms.loopback = true;
    pl022_set_param(PL022_SSP_BASE, &prms);

    pl022_clear_rx_buf(PL022_SSP_BASE);
    iowrite32(1<<2, PL022_SSP_BASE + 0x140);

    prms.loopback = false;
    pl022_set_param(PL022_SSP_BASE, &prms);

    pl022_context(PL022_SSP_BASE, 1);
    
    /* Send the start symbol */
    iowrite32(0xf55, PL022_SSP_BASE + 0x8);
    pl022_clear_rx_buf(PL022_SSP_BASE);
}
