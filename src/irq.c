#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <rumboot/rumboot.h>
#include <rumboot/platform.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <platform/interrupts.h>

const char *const exception_names[] = {
	[RUMBOOT_IRQ_UNDEFINED_INSTRUCTION] = "Undefined Instruction",
	[RUMBOOT_IRQ_PREFETCH_ABORT] = "Prefetch Abort",
	[RUMBOOT_IRQ_DATA_ABORT] = "Data Abort",
	[RUMBOOT_IRQ_SOFTWARE_EXCEPTION] = "Software Exception"
};

struct rumboot_irq_entry {
	void		(*handler)(int irq, void *arg);
	void *		arg;
	uint32_t	flags;
};


const struct rumboot_irq_controller *rumboot_irq_controller_by_irq(int irq)
{
	int i = 0;
	do {
		const struct rumboot_irq_controller *ctl = rumboot_platform_runtime_info.irq_ctl_dev[i];

		if (!ctl)
			break;

		if ((irq <= ctl->last) && (irq >= ctl->first)) {
			return ctl;
		}
		i++;
	} while (1);
	rumboot_platform_panic("irq: Failed to look up controller for IRQ: %d\n", irq);
	return NULL;
}

const struct rumboot_irq_controller *rumboot_irq_controller_by_id(int id)
{
	if ((id > RUMBOOT_PLATFORM_NUM_IRQ_CONTROLLERS) || (id < 0)) {
			return NULL;
	}
	return rumboot_platform_runtime_info.irq_ctl_dev[id];
}

void rumboot_irq_register_controller(const struct rumboot_irq_controller *ctl)
{
	if (rumboot_platform_runtime_info.num_irq_controllers >= RUMBOOT_PLATFORM_NUM_IRQ_CONTROLLERS) {
		rumboot_platform_panic("irq: Failed to register irq controller %s: Please increase RUMBOOT_PLATFORM_NUM_IRQ_CONTROLLERS\n");
	}
	rumboot_platform_runtime_info.irq_ctl_dev[rumboot_platform_runtime_info.num_irq_controllers++] = ctl;
	ctl->init(ctl);
}


void rumboot_irq_free(struct rumboot_irq_entry *tbl)
{
	rumboot_free(tbl);
}

struct rumboot_irq_entry *rumboot_irq_create(struct rumboot_irq_entry *copyfrom)
{
	if (!copyfrom && rumboot_irq_table_get()) {
		copyfrom = rumboot_irq_table_get();
	}

	struct rumboot_irq_entry *tbl = rumboot_malloc(sizeof(*tbl) * (RUMBOOT_PLATFORM_NUM_IRQS + 1));
	if (!tbl) {
		rumboot_platform_panic("IRQ Table alloc failed!\n");
	}

	if (copyfrom) {
		memcpy(tbl, copyfrom, sizeof(*tbl) + (RUMBOOT_PLATFORM_NUM_IRQS + 1));
	}

	return tbl;
}

void rumboot_irq_set_default_handler(void (*handler)(int irq))
{
	RUMBOOT_ATOMIC_BLOCK() {
		rumboot_platform_runtime_info.irq_def_hndlr = handler;
	}
}

void rumboot_irq_set_handler(struct rumboot_irq_entry *tbl, int irq, uint32_t flags,
			     void (*handler)(int irq, void *args), void *arg)
{
	RUMBOOT_ATOMIC_BLOCK() {
		if (irq > (RUMBOOT_PLATFORM_NUM_IRQS - 1)) {
			rumboot_platform_panic("IRQ %d is too big\n", irq);
		}

		if (!tbl) {
			tbl = rumboot_irq_table_get();
		}

		if (!tbl) {
			rumboot_platform_panic("FATAL: Attempt to set handler on NULL table with no active table\n");
		}

		tbl[irq].handler = handler;
		tbl[irq].arg = arg;
		tbl[irq].flags = flags;
	}
}

void rumboot_irq_table_activate(struct rumboot_irq_entry *tbl)
{
	RUMBOOT_ATOMIC_BLOCK() {
		rumboot_platform_runtime_info.irq_handler_table = tbl;
		if (tbl) {
			int i = 0;
			for (i = 0; i < RUMBOOT_PLATFORM_NUM_IRQS; i++)
				if (tbl[i].handler) {
					rumboot_irq_enable(i);
				}
		}
	}
}

void rumboot_irq_swint(uint32_t irq)
{
	const struct rumboot_irq_controller *ctl = rumboot_irq_controller_by_irq(irq);
	ctl->generate_swint(ctl, irq);
}

void *rumboot_irq_table_get()
{
	return rumboot_platform_runtime_info.irq_handler_table;
}

void rumboot_irq_enable(int irq)
{
	struct rumboot_irq_entry *tbl = rumboot_irq_table_get();
	const struct rumboot_irq_controller *ctl = rumboot_irq_controller_by_irq(irq);
	ctl->configure(ctl, irq, tbl[irq].flags, 1);
}

void rumboot_irq_enable_all()
{
	int i;

	/* TODO: Iterating over interrupt controllers and their irqs, directly calling
	 * ctl->configure() will be waaay faster
	 */
	for (i = 0; i < RUMBOOT_PLATFORM_NUM_IRQS; i++) {
		rumboot_irq_enable(i);
	}
}

void rumboot_irq_disable(int irq)
{
	const struct rumboot_irq_controller *ctl = rumboot_irq_controller_by_irq(irq);
	struct rumboot_irq_entry *tbl = rumboot_irq_table_get();
	int flags = 0;

	if (tbl) {
		flags = tbl[irq].flags;
	}
	ctl->configure(ctl, irq, flags, 0);
}


static void process_irq(int id)
{
	struct rumboot_irq_entry *tbl = rumboot_irq_table_get();

	if (tbl && tbl[id].handler) {
		tbl[id].handler(id, tbl[id].arg);
	} else if (rumboot_platform_runtime_info.irq_def_hndlr) {
		rumboot_platform_runtime_info.irq_def_hndlr(id);
	} else {
		if (!tbl) {
			rumboot_platform_panic("FATAL: Unhandled IRQ %d when no active irq table present\n",
					       id);
		} else {
			rumboot_platform_panic("FATAL: Unhandled IRQ %d\n", id);
		}
	}
}

void rumboot_irq_core_dispatch(uint32_t ctrl, uint32_t type, uint32_t id)
{
	const struct rumboot_irq_controller *ctl = rumboot_irq_controller_by_id(ctrl);
	switch (type) {
	case RUMBOOT_IRQ_TYPE_NORMAL:
		id = ctl->begin(ctl);
		process_irq(id);
		ctl->end(ctl,id);
		break;
	case RUMBOOT_IRQ_TYPE_SOFT:
		process_irq(id);
		break;
	case RUMBOOT_IRQ_TYPE_EXCEPTION:
		rumboot_platform_panic("EXCEPTION: %s \n", exception_names[id]);
		break;
	}
}
