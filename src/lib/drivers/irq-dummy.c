#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rumboot/platform.h>
#include <devices/gic.h>
#include <rumboot/printf.h>

uint32_t rumboot_platform_irq_begin()
{
    return 0;
}

void rumboot_platform_irq_end()
{

}

void rumboot_platform_irq_configure(int irq, uint32_t flags, int enable)
{

}
