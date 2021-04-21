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
    return ((uintptr_t) addr << 2);
}

extern char rumboot_IRAM_heap_start;
extern char rumboot_IRAM_heap_end;
static struct nfifo_instance g_nfifo;

void rumboot_platform_setup()
{
        iowrite32(0x0, TIMERS_BASE); 
        iowrite32(0x3, TIMERS_BASE + 8);
        nfifo_instance_init(&g_nfifo, FIFO_BASE);
        nfifo_enable(&g_nfifo, 1);
        rumboot_irq_register_nmc_intc_controller();
        rumboot_malloc_register_heap("IM",
                                     &rumboot_IRAM_heap_start, &rumboot_IRAM_heap_end);
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

