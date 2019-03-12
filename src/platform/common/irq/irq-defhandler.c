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
    *gotit=1;
    rumboot_printf("IRQ default handler called, arg %x\n", arg);    
}

int main()
{
    rumboot_irq_sei();
    int gotit = 0;
    rumboot_printf("Setting default handler\n");
    rumboot_irq_set_default_handler(handler, &gotit);

    struct rumboot_irq_entry *tbl = rumboot_irq_create(NULL);
    rumboot_irq_table_activate(tbl);
    rumboot_irq_enable(USE_SWINT);

    rumboot_printf("Firing IRQ\n");
	rumboot_irq_swint(USE_SWINT);;
    udelay(1);
    rumboot_printf("IRQ Received: %d", gotit);
    return (gotit > 0) ? 0 : 1;
}
