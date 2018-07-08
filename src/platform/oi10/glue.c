#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <rumboot/platform.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/rumboot.h>

#include <platform/devices/mpic128.h>


/* Platform-specific glue */
uint32_t rumboot_platform_get_uptime() {
    #define TIMER_TICKS_PER_US  800
    #define SPR_TBL_R           0x10C

    uint32_t value = 0;

    __asm volatile (
        "mfspr %0, %1 \n\t"
        :"=r"(value)
        :"i"(SPR_TBL_R)
    );

    return value / TIMER_TICKS_PER_US;
}

void rumboot_platform_setup() {
    extern char rumboot_platform_heap_start;
    extern char rumboot_platform_heap_end;

    rumboot_irq_register_mpic128();

    rumboot_malloc_register_heap( "IM0", &rumboot_platform_heap_start, &rumboot_platform_heap_end );
}
