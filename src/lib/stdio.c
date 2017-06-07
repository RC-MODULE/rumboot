#include <stdlib.h>
#include <stdint.h>
#include <rumboot/platform.h>

void rumboot_putstring(const char *string)
{
    while(*string)
        rumboot_platform_putchar(*string++);
}
