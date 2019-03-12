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
    int *context_in_irq = arg;
    rumboot_printf("IRQ default handler called, arg %x (%d), context: %d\n", 
        arg, 
        *context_in_irq,
        rumboot_irq_get_context()
    );    
    *context_in_irq = rumboot_irq_get_context();
}

int main()
{
    volatile uint32_t context_in_irq = 0;
	rumboot_irq_sei();
    rumboot_printf("Creating a table\n");
	struct rumboot_irq_entry *tbl = rumboot_irq_create(NULL);
	rumboot_irq_table_activate(tbl);    
	rumboot_irq_enable(USE_SWINT);

    rumboot_printf("Setting default handler\n");
    rumboot_irq_set_default_handler(handler, (void *) &context_in_irq);

    rumboot_printf("Firing IRQ\n");
	rumboot_irq_swint(USE_SWINT);
    rumboot_printf("Current context %d, context during irq %d\n",
     rumboot_irq_get_context(),
     context_in_irq
     );

    if (rumboot_irq_get_context() != 0)
        return 1;

    if (context_in_irq != 1)
        return 1;

    return 0;
}
