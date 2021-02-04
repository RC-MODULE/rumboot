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

#ifndef CMAKE_BUILD_TYPE_DEBUG
void  rumboot_platform_putchar(uint8_t c)
{

}

int rumboot_platform_getchar(uint32_t timeout_us)
{

}
#endif

uint32_t rumboot_platform_get_uptime()
{
        return 0;
}

extern char rumboot_IRAM_heap_start;
extern char rumboot_IRAM_heap_end;

void rumboot_platform_setup()
{
        rumboot_irq_register_nmc_intc_controller();
        rumboot_malloc_register_heap("IM",
                                     &rumboot_IRAM_heap_start, &rumboot_IRAM_heap_end);

}
