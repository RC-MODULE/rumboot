#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <rumboot/platform.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>

void rumboot_arch_irq_disable()
{
    asm volatile ("cpsid i" : : : "memory");
}

void rumboot_arch_irq_enable()
{
    asm volatile ("cpsie i" : : : "memory");
}
