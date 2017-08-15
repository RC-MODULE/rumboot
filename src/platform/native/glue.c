#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <rumboot/platform.h>

/* Platform-specific glue */
uint32_t rumboot_platform_get_uptime()
{
    return 0;
}

void rumboot_platform_panic(const char *why, ...)
{
    va_list ap;
    va_start(ap, why);
    printf("PANIC: ");
    vprintf(why, ap);
    va_end(ap);
    exit(1);
}

void rumboot_platform_trace(void *pc)
{
    /* stack tracing code here */
}

void rumboot_platform_raise_event(enum rumboot_simulation_event event, uint8_t arg)
{
    printf("[EVENT] %d arg %d\n", event, arg);
    exit(event);
}

void rumboot_platform_putchar(uint8_t c)
{
    putc(c, stdout);
}

int rumboot_platform_getchar(uint32_t timeout_us)
{
    return (uint8_t) getc(stdin);
}
