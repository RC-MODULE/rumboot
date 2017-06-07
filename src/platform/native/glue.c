#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <rumboot/platform.h>

/* Platform-specific glue */
uint32_t rumboot_platform_get_uptime()
{
    return 0;
}

void rumboot_platform_raise_event(enum rumboot_simulation_event event)
{
    printf("[EVENT] %d \n", event);
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
