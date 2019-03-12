#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <platform/devices.h>
#include <rumboot/io.h>
#include <stdlib.h>
#include <rumboot/timer.h>

#ifndef USE_SWINT
#define USE_SWINT 0
#endif

void handler(int irq, void *arg)
{
    int *gotit = arg;
    rumboot_printf("IRQ default handler called, arg %x (%d)\n", arg, *gotit);    
    (*gotit)++; 
}

int main()
{
    volatile uint32_t gotit = 0;

    rumboot_printf("Creating a table\n");
	struct rumboot_irq_entry *tbl = rumboot_irq_create(NULL);
	rumboot_irq_table_activate(tbl);
	rumboot_irq_enable(USE_SWINT);
	rumboot_irq_sei();

    rumboot_printf("Setting default handler\n");
    rumboot_irq_set_default_handler(handler, (void *) &gotit);

	rumboot_irq_table_activate(NULL);
    rumboot_printf("Firing IRQ without a table\n");
	rumboot_irq_swint(USE_SWINT);

	rumboot_irq_table_activate(tbl);

    rumboot_printf("Firing IRQ\n");
	rumboot_irq_swint(USE_SWINT);
    udelay(1);
    rumboot_printf("Total IRQ Received: %d (must be 2)\n", gotit);
    rumboot_printf("IRQ Accouting count: %d (must be 1)\n", rumboot_irq_get_count(USE_SWINT));

    if (gotit != 2 )
        return 1;

    if (rumboot_irq_get_count(USE_SWINT) != 1)
        return 1;

    return 0;
}
