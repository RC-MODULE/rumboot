#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <rumboot/rumboot.h>
#include <rumboot/platform.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <platform/interrupts.h>

const char * const exception_names[] = {
	[RUMBOOT_IRQ_UNDEFINED_INSTRUCTION] = "Undefined Instruction",
	[RUMBOOT_IRQ_PREFETCH_ABORT] = "Prefetch Abort",
	[RUMBOOT_IRQ_DATA_ABORT] = "Data Abort",
	[RUMBOOT_IRQ_SOFTWARE_EXCEPTION] = "Software Exception"
};

struct rumboot_irq_entry
{
    void (*handler)(int irq, void *arg);
    void *arg;
	uint32_t flags;
};


void rumboot_irq_free(struct rumboot_irq_entry *tbl)
{
	rumboot_free(tbl);
}

struct rumboot_irq_entry *rumboot_irq_create(struct rumboot_irq_entry *copyfrom)
{

    struct rumboot_irq_entry *tbl = rumboot_malloc(sizeof(*tbl) * (RUMBOOT_PLATFORM_NUM_IRQS + 1));
    if (!tbl){
        rumboot_platform_panic("IRQ Table alloc failed!\n");
	}

	if (copyfrom)
		memcpy(tbl, copyfrom, sizeof(*tbl) + (RUMBOOT_PLATFORM_NUM_IRQS + 1));

	return tbl;
}

void rumboot_irq_set_handler(struct rumboot_irq_entry *tbl, int irq, uint32_t flags,
		void (*handler)(int irq, void *args), void *arg)
{
	if (irq > (RUMBOOT_PLATFORM_NUM_IRQS - 1))
		rumboot_platform_panic("IRQ %d is too big\n", irq);

	tbl[irq].handler = handler;
	tbl[irq].arg=arg;
	tbl[irq].flags=flags;
}

void rumboot_irq_table_activate(struct rumboot_irq_entry *tbl)
{
    rumboot_platform_runtime_info.irq_handler_table = tbl;
}

void *rumboot_irq_table_get()
{
    return rumboot_platform_runtime_info.irq_handler_table;
}

void rumboot_irq_enable(int irq)
{
	struct rumboot_irq_entry *tbl = rumboot_irq_table_get();
	rumboot_platform_irq_configure(irq, tbl[irq].flags, 1);
}

void rumboot_irq_disable(int irq)
{
	struct rumboot_irq_entry *tbl = rumboot_irq_table_get();
	rumboot_platform_irq_configure(irq, tbl[irq].flags, 0);
}

void rumboot_irq_cli()
{
	rumboot_arch_irq_disable();
}

void rumboot_irq_sei()
{
	rumboot_arch_irq_enable();
}

static void process_irq(int id)
{
		struct rumboot_irq_entry *tbl = rumboot_irq_table_get();
		if (!tbl)
			rumboot_platform_panic("INTERNAL BUG: IRQ %d arrived when no table active", id);
		if (!tbl[id].handler)
			rumboot_platform_panic("FATAL: Unhandled IRQ %d\n", id);

		tbl[id].handler(id, tbl[id].arg);
}

void rumboot_irq_core_dispatch(uint32_t type, uint32_t id)
{

	switch (type) {
	case RUMBOOT_IRQ_TYPE_NORMAL:
		id = rumboot_platform_irq_begin();
		process_irq(id);
		rumboot_platform_irq_end(id);
		break;
	case RUMBOOT_IRQ_TYPE_SOFT:
		process_irq(id);
		break;
	case RUMBOOT_IRQ_TYPE_EXCEPTION:
		rumboot_platform_panic("%s \n", exception_names[id]);
		break;
	}
}
