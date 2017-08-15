#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <rumboot/platform.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>


#define RUMBOOT_PLATFORM_NUM_IRQS 32

const char *exception_names[] = {
	[RUMBOOT_IRQ_UNDEFINED_INSTRUCTION] = "Undefined Instruction",
	[RUMBOOT_IRQ_PREFETCH_ABORT] = "Prefetch Abort",
	[RUMBOOT_IRQ_DATA_ABORT] = "Data Abort",
	[RUMBOOT_IRQ_SOFTWARE_EXCEPTION] = "Software Exception"
};

void rumboot_irq_core_dispatch(uint32_t type, uint32_t id)
{
	switch (type) {
	case RUMBOOT_IRQ_TYPE_NORMAL:
		/* Route IRQ to controller driver */
		break;
	case RUMBOOT_IRQ_TYPE_EXCEPTION:
		rumboot_printf("FATAL: %s\n", exception_names[id]);
		rumboot_platform_panic("Tracing the stack...\n");
		break;
	}
}

struct rumboot_irq_entry
{
    void (*handler)(void *arg);
    void *arg;
};


void rumboot_irq_free(struct rumboot_irq_entry *tbl)
{
	free(tbl);
}

struct rumboot_irq_entry *rumboot_irq_create(struct rumboot_irq_entry *copyfrom)
{

    struct rumboot_irq_entry *tbl = calloc(sizeof(*tbl), RUMBOOT_PLATFORM_NUM_IRQS + 1);
    if (!tbl){
        rumboot_platform_panic("IRQ Table alloc failed!\n");
	}

	if (copyfrom)
		memcpy(tbl, copyfrom, sizeof(*tbl) + (RUMBOOT_PLATFORM_NUM_IRQS + 1));

	return tbl;
}

void rumboot_irq_set_handler(struct rumboot_irq_entry *tbl, int irq, void (*handler)(), void *arg)
{
	if (irq > (RUMBOOT_PLATFORM_NUM_IRQS - 1))
		rumboot_platform_panic("IRQ %d is too big\n", irq);
	tbl[irq].handler = handler;
	tbl[irq].arg=arg;
}

void rumboot_irq_activate_table(struct rumboot_irq_entry *tbl)
{
	/* TODO */
}


void rumboot_irq_enable(int irq)
{

}

void rumboot_irq_disable(int irq)
{

}
