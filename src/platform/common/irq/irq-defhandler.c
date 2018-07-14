#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <platform/devices.h>
#include <rumboot/io.h>
#include <stdlib.h>

void handler(int irq)
{
    rumboot_printf("IRQ default handler called\n");
    exit(0);
}

int main()
{
    rumboot_printf("Setting default handler\n");
    rumboot_irq_set_default_handler(handler);
    rumboot_printf("Firing IRQ\n");

    #if 0
    struct rumboot_irq_entry *tbl = rumboot_irq_create(NULL);
    rumboot_irq_set_handler(tbl, 0, 0, handler, (void *)&done);
    rumboot_irq_enable(0);
    rumboot_irq_table_activate(tbl);
    #endif

    asm volatile("swi #0");

    #if 0
    rumboot_irq_table_activate(NULL);
    rumboot_irq_free(tbl);
    #endif
    return 1;
}
