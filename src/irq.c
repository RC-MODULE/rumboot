#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <rumboot/rumboot.h>
#include <rumboot/platform.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <platform/interrupts.h>
#include <alloca.h>

const char *const exception_names[] = {
    [RUMBOOT_IRQ_UNDEFINED_INSTRUCTION] = "Undefined Instruction",
    [RUMBOOT_IRQ_PREFETCH_ABORT] = "Prefetch Abort",
    [RUMBOOT_IRQ_DATA_ABORT] = "Data Abort",
    [RUMBOOT_IRQ_SOFTWARE_EXCEPTION] = "Software Exception",
    [RUMBOOT_IRQ_MACHINE_CHECK] = "Machine Check",
    [RUMBOOT_IRQ_DATA_STORAGE] = "Data Storage",
    [RUMBOOT_IRQ_INST_STORAGE] = "Inst Storage",
    [RUMBOOT_IRQ_ALIGNMENT] = "Alignment",
    [RUMBOOT_IRQ_PROGRAM] = "Program",
    [RUMBOOT_IRQ_FP_UNAVAILABLE] = "Floating-Point Unavailable",
    [RUMBOOT_IRQ_SYSTEM_CALL] = "System Call",
    [RUMBOOT_IRQ_AP_UNAVAILABLE] = "Auxiliary Processor Unavailable",
    [RUMBOOT_IRQ_DECREMENTER] = "Decrementer",
    [RUMBOOT_IRQ_FIXED_INTERVAL_TIMER] = "Fixed-Interval Timer",
    [RUMBOOT_IRQ_WATCHDOG_TIMER] = "Watchdog Timer",
    [RUMBOOT_IRQ_DATA_TLB_ERROR] = "Data TLB Error",
    [RUMBOOT_IRQ_INST_TLB_ERROR] = "Inst TLB Error",
    [RUMBOOT_IRQ_DEBUG] = "Debug"
};

struct rumboot_irq_entry {
	void		(*handler)(int irq, void *arg);
	void *		arg;
	uint32_t	flags;
	int 		priority;
	uint32_t 	count;
};

static void check_irq_id(int id)
{
	if ((id < 0) || (id > RUMBOOT_PLATFORM_NUM_IRQS)) {
		rumboot_platform_panic("BUG: Invalid IRQ ID %d\n", id);
	}
}

const struct rumboot_irq_controller *rumboot_irq_controller_by_irq(int irq)
{
	int i = 0;
	do {
		const struct rumboot_irq_controller *ctl = rumboot_platform_runtime_info->irq_ctl_dev[i];

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
    	rumboot_platform_panic("irq: Invalid IRQ controller ID: %d (max %d)\n",
				id, RUMBOOT_PLATFORM_NUM_IRQ_CONTROLLERS);
	}
	return rumboot_platform_runtime_info->irq_ctl_dev[id];
}

void rumboot_irq_register_controller(const struct rumboot_irq_controller *ctl)
{
	if (rumboot_platform_runtime_info->num_irq_controllers >= RUMBOOT_PLATFORM_NUM_IRQ_CONTROLLERS) {
		rumboot_platform_panic("irq: Failed to register irq controller %s: Please increase RUMBOOT_PLATFORM_NUM_IRQ_CONTROLLERS\n");
	}
	rumboot_platform_runtime_info->irq_ctl_dev[rumboot_platform_runtime_info->num_irq_controllers++] = ctl;
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
		memcpy(tbl, copyfrom, sizeof(*tbl) * (RUMBOOT_PLATFORM_NUM_IRQS + 1));
	} else {
	    memset(tbl, 0, sizeof(*tbl) * (RUMBOOT_PLATFORM_NUM_IRQS + 1));
	}

	return tbl;
}

void rumboot_irq_set_default_handler(void (*handler)(int irq, void *arg), void *arg)
{
	RUMBOOT_ATOMIC_BLOCK() {
		rumboot_platform_runtime_info->irq_def_hndlr = handler;
		rumboot_platform_runtime_info->irq_def_arg = arg;		
	}
}

void rumboot_irq_set_handler(struct rumboot_irq_entry *tbl, int irq, uint32_t flags,
			     void (*handler)(int irq, void *args), void *arg)
{
	const struct rumboot_irq_controller *ctl = rumboot_irq_controller_by_irq(irq);
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
		tbl[irq].priority = ctl->priority_default;
	}
}

void rumboot_irq_table_activate(struct rumboot_irq_entry *tbl)
{
	RUMBOOT_ATOMIC_BLOCK() {
		rumboot_platform_runtime_info->irq_handler_table = tbl;
		if (tbl) {
			int i = 0;
			for (i = 0; i < RUMBOOT_PLATFORM_NUM_IRQS; i++) {
				if (tbl[i].handler) {
					rumboot_irq_enable(i);
				}
            }
		}
	}
}

void rumboot_irq_swint(uint32_t irq)
{
	const struct rumboot_irq_controller *ctl = rumboot_irq_controller_by_irq(irq);
	irq -= ctl->first;
	ctl->generate_swint(ctl, irq);
}

void *rumboot_irq_table_get()
{
	return rumboot_platform_runtime_info->irq_handler_table;
}

struct rumboot_irq_entry *rumboot_irq_table_fetch(struct rumboot_irq_entry *tbl)
{
       if (!tbl) {
               tbl = rumboot_irq_table_get();
       }

       if (!tbl) {
               rumboot_platform_panic("irq: Failed fetch irq table\n");
       }

       return tbl;
}


void rumboot_irq_enable(int irq)
{
	struct rumboot_irq_entry *tbl = rumboot_irq_table_fetch(NULL);
	const struct rumboot_irq_controller *ctl = rumboot_irq_controller_by_irq(irq);
	ctl->configure(ctl, irq - ctl->first, tbl[irq].flags, 1);
	rumboot_irq_priority_adjust(tbl, irq, tbl[irq].priority);
}

void rumboot_irq_enable_all()
{
	int i;
	for (i = 0; i < RUMBOOT_PLATFORM_NUM_IRQS; i++) {
		rumboot_irq_enable(i);
	}
}

void rumboot_irq_disable(int irq)
{
	const struct rumboot_irq_controller *ctl = rumboot_irq_controller_by_irq(irq);
	struct rumboot_irq_entry *tbl = rumboot_irq_table_fetch(NULL);
	int flags = 0;

	if (tbl) {
		flags = tbl[irq].flags;
	}
	irq -= ctl->first;
	ctl->configure(ctl, irq, flags, 0);
}


static void process_irq(int id)
{
	struct rumboot_irq_entry *tbl = rumboot_irq_table_get();

	check_irq_id(id); 
	if (tbl && tbl[id].handler) {
		tbl[id].handler(id, tbl[id].arg);
	} else if (rumboot_platform_runtime_info->irq_def_hndlr) {
		rumboot_platform_runtime_info->irq_def_hndlr(id, rumboot_platform_runtime_info->irq_def_arg);
	} else {
			rumboot_platform_panic("FATAL: Unhandled IRQ %d\n", id);
	}
	if (tbl) {
		tbl[id].count++;
	}
}

uint32_t rumboot_irq_get_count(int irq) 
{
	struct rumboot_irq_entry *tbl = rumboot_irq_table_get();
	check_irq_id(irq); 
	if (tbl) {
		return tbl[irq].count;
	}
	return 0;
}

void rumboot_irq_reset_count(int irq)
{
	check_irq_id(irq); 
	struct rumboot_irq_entry *tbl = rumboot_irq_table_get();
	if (tbl) {
		tbl[irq].count = 0;
	}
} 

void rumboot_irq_set_exception_handler(void (*handler)(int id, const char *name))
{
	RUMBOOT_ATOMIC_BLOCK() {
		rumboot_platform_runtime_info->irq_exception_hndlr = handler;
	}
}

void rumboot_irq_core_dispatch(uint32_t ctrl, uint32_t type, uint32_t id)
{
	const struct rumboot_irq_controller *ctl = rumboot_irq_controller_by_id(ctrl);
	int prevtype;
	void *scratch = NULL;

	rumboot_platform_runtime_info->irq_context_counter++;
	prevtype = rumboot_platform_runtime_info->irq_type;
	/* Controllers may later change IRQ type on their own */
	rumboot_platform_runtime_info->irq_type = type;

	if (ctl->scratch_size) {
		/* Only allocate a scratch buffer if controller requests it */
		scratch = alloca(ctl->scratch_size);
	}

	switch (type) {
	case RUMBOOT_IRQ_TYPE_NORMAL:
		id = ctl->begin(ctl, scratch);
		process_irq(id);
		ctl->end(ctl, scratch, id);
		break;
	case RUMBOOT_IRQ_TYPE_SOFT:
		process_irq(id);
		break;
	case RUMBOOT_IRQ_TYPE_EXCEPTION:
    if (rumboot_platform_runtime_info->irq_exception_hndlr) {
			rumboot_platform_runtime_info->irq_exception_hndlr(id, exception_names[id]);
		} else {
			rumboot_platform_panic("EXCEPTION: %s \n", exception_names[id]);
		}
		break;
	}
	/* Restore context counter and previous IRQ type */
	rumboot_platform_runtime_info->irq_type = prevtype;
	rumboot_platform_runtime_info->irq_context_counter--;
}

int rumboot_irq_priority_adjust(struct rumboot_irq_entry *tbl, int irq, int priority)
{
        int ret;
        const struct rumboot_irq_controller *ctl = rumboot_irq_controller_by_irq(irq);
        tbl = rumboot_irq_table_fetch(tbl);

		/* 0 is always considered default priority */
		if (priority == 0) {
			priority = ctl->priority_default;
		}

        if (priority > ctl->priority_max) {
                rumboot_printf("WARN: irq: priority %d is bigger than maximum (%d) for %s",
                               priority, ctl->priority_max, ctl->name);
                priority = ctl->priority_max;
        }

        if (priority < ctl->priority_min) {
                rumboot_printf("WARN: irq: priority %d is less than minimum (%d) for %s",
                               priority, ctl->priority_min, ctl->name);
                priority = ctl->priority_min;
        }

        ret = tbl[irq].priority;
        tbl[irq].priority = priority;

        /* If the table is active, adjust priority immediately */
        if (tbl == rumboot_irq_table_get()) {
                if (ctl->adjust_priority) {
                    ctl->adjust_priority(ctl, irq-ctl->first, priority);
                }
        }

        /* Return old priority */
        return ret;
}

int rumboot_irq_priority_get(struct rumboot_irq_entry *tbl, int irq)
{
        return tbl[irq].priority;
}

int rumboot_irq_prioity_get_max(int irq)
{
        const struct rumboot_irq_controller *ctl = rumboot_irq_controller_by_irq(irq);
        return ctl->priority_max;
}

int rumboot_irq_prioity_get_min(int irq)
{
        const struct rumboot_irq_controller *ctl = rumboot_irq_controller_by_irq(irq);
        return ctl->priority_min;
}

int rumboot_irq_get_context()
{
	int ret; 
	RUMBOOT_ATOMIC_BLOCK() {
		ret = rumboot_platform_runtime_info->irq_context_counter;
	}
	return ret;
}

int rumboot_irq_current_type()
{
	int ret; 
	RUMBOOT_ATOMIC_BLOCK() {
		ret = rumboot_platform_runtime_info->irq_type;
	}
	return ret;
}
