#include <stdarg.h>
#include <stdint.h>
#include <platform/devices.h>
#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <devices/gic.h>
#include <unistd.h>
#include <rumboot/rumboot.h>
#include <rumboot/printf.h>
#include <devices/irq-nmc.h> 
#include <devices/nfifo.h>


uint32_t rumboot_platform_get_uptime()
{
        return (ioread32(TIMERS_BASE) / 100);
}

int64_t rumboot_virt_to_phys(volatile void *addr)
{
    return ((uintptr_t) addr << 2);
}

uint32_t rumboot_virt_to_dma(volatile const void *addr) 
{
    uintptr_t addr_v = ((uintptr_t) addr);
    if (addr_v < 0x8000) {
            return (IM1_BASE << 2) + (addr_v << 2);
    }
    if ((addr_v >= 0x8000) && (addr_v < 0x10000)) {
            return (IM2_BASE << 2) + (addr_v << 2);
    }
    if ((addr_v >= 0x10000) && (addr_v < 0x18000)) {
            return (IM3_BASE << 2) + (addr_v << 2);
    }
    if ((addr_v >= 0x10000) && (addr_v < 0x18000)) {
            return (IM3_BASE << 2) + (addr_v << 2);
    }
    if ((addr_v >= 0x40000000) && (addr_v < 0x5FFFFFFF)) {
            return (addr_v - 0x40000000) << 2;
    }
    rumboot_printf("failed to translate addr 0x%x to axi\n", addr);
    rumboot_platform_panic("nmc: Invalid soft-translation address");
}

extern char rumboot_IM1_heap_start;
extern char rumboot_IM1_heap_end;
extern char rumboot_IM2_heap_start;
extern char rumboot_IM2_heap_end;
extern char rumboot_IM3_heap_start;
extern char rumboot_IM3_heap_end;
extern char rumboot_SRAM_heap_start;
extern char rumboot_SRAM_heap_end;

static struct nfifo_instance g_nfifo;

void rumboot_platform_setup()
{
        iowrite32(0x0, TIMERS_BASE); 
        iowrite32(0x3, TIMERS_BASE + 8);
        nfifo_instance_init(&g_nfifo, FIFO_BASE);
        nfifo_enable(&g_nfifo, 1);
        rumboot_irq_register_nmc_intc_controller();
        rumboot_malloc_register_heap("IM1",
                                     &rumboot_IM1_heap_start, &rumboot_IM1_heap_end);
        rumboot_malloc_register_heap("IM2",
                                     &rumboot_IM2_heap_start, &rumboot_IM2_heap_end);
        rumboot_malloc_register_heap("IM3",
                                     &rumboot_IM3_heap_start, &rumboot_IM3_heap_end);
        rumboot_malloc_register_heap("SRAM",
                                     &rumboot_SRAM_heap_start, &rumboot_SRAM_heap_end);

}

void rumboot_platform_exit(int status) {
        nfifo_write(&g_nfifo, (1<<31) | status);
}

#ifndef CMAKE_BUILD_TYPE_DEBUG
void  rumboot_platform_putchar(uint8_t c)
{
        nfifo_write(&g_nfifo, c);
}

int rumboot_platform_getchar(uint32_t timeout_us)
{
        return nfifo_read(&g_nfifo, timeout_us);
}
#endif

