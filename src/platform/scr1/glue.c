#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <rumboot/platform.h>
#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <arch/irq_macros.h>
#include <platform/devices.h>
#include <devices/sp804.h>
#include <rumboot/rumboot.h>
#include <string.h>
#include <rumboot/pcie_test_lib.h>
#include <rumboot/ddr_test_lib.h>
#include <devices/gic.h>
#include <devices/irq-proxy-gic-cdnpcie.h>
#include <rumboot/irq.h>
#include <stdbool.h>

int64_t rumboot_virt_to_phys(volatile void *addr)
{
        int64_t ret = (uint32_t)addr;

        return ret;
}

uint32_t rumboot_virt_to_dma(volatile const void *addr)
{
        return (uint32_t)addr;
}

/* Platform-specific glue */
#define TIMER_BASE 0x00490000
uint32_t rumboot_platform_get_uptime()
{
        //TODO: Figure out proper clock
        //return (0xFFFFFFFF - sp804_get_value(DIT_Base, 1)) / 8.32;
        return ioread32(TIMER_BASE + 0x8);
}

extern char rumboot_IRAM_heap_start;
extern char rumboot_IRAM_heap_end;

void rumboot_platform_init_loader(struct rumboot_config *conf)
{
}

void rumboot_platform_read_config(struct rumboot_config *conf)
{}

extern char rumboot_emem0_heap_start;
extern char rumboot_emem0_heap_end;
extern char rumboot_emem1_heap_start;
extern char rumboot_emem1_heap_end;

void rumboot_platform_setup()
{
        iowrite32(0, TIMER_BASE);
        iowrite32(99 , TIMER_BASE + 4);
        iowrite32(0, TIMER_BASE + 8);
        iowrite32(0, TIMER_BASE + 0xc);      
        iowrite32(1, TIMER_BASE);

#ifndef RUMBOOT_ONLY_STACK
        rumboot_malloc_register_heap("IRAM",
                                     &rumboot_IRAM_heap_start, &rumboot_IRAM_heap_end);
        rumboot_malloc_register_heap("EMEM0",
                                     &rumboot_emem0_heap_start, &rumboot_emem0_heap_end);
        rumboot_malloc_register_heap("EMEM1",
                                     &rumboot_emem1_heap_start, &rumboot_emem1_heap_end);

#endif
}

#ifndef CMAKE_BUILD_TYPE_DEBUG
#define SC_SIM_OUTPORT (0xf0000000)

void __attribute__((no_instrument_function)) rumboot_platform_putchar(uint8_t c)
{
	volatile char *out_ptr = (volatile char*)SC_SIM_OUTPORT;
    *out_ptr = c;
}


int rumboot_platform_getchar(uint32_t timeout_us)
{

}
#endif