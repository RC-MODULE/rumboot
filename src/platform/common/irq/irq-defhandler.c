#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <platform/devices.h>
#include <rumboot/io.h>
#include <stdlib.h>

#ifndef USE_SWINT
#define USE_SWINT 0
#endif


void handler(int irq)
{
    rumboot_printf("IRQ default handler called\n");
    exit(0);
}

int main()
{
    rumboot_irq_sei();
    rumboot_printf("Setting default handler\n");
    rumboot_irq_set_default_handler(handler);

    struct rumboot_irq_entry *tbl = rumboot_irq_create(NULL);
    rumboot_irq_table_activate(tbl);
    rumboot_irq_enable(USE_SWINT);

    rumboot_printf("Firing IRQ\n");
	rumboot_irq_swint(USE_SWINT);;

    rumboot_irq_table_activate(NULL);
    rumboot_irq_free(tbl);

    return 1;
}
