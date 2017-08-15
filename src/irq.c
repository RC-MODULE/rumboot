#include <stdio.h>
#include <stdint.h>
#include <rumboot/platform.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>

void rumboot_panic()
{
	rumboot_printf("Running stack trace...\n");
	uint32_t *fp = __builtin_frame_address(0);
	int depth = 32;
	while (depth--) {
		uint32_t pc = *(((uint32_t*)fp) + 0);
		uint32_t lr = *(((uint32_t*)fp) + 1);
	 	fp = (uint32_t *) (lr);
        rumboot_platform_trace((void *) (pc - 4 ));
		if (fp == 0)
			break;
	}
	rumboot_platform_raise_event(EVENT_CRASH, 0);
}

const char *exception_names[] = {
	[RUMBOOT_IRQ_UNDEFINED_INSTRUCTION] = "Undefined Instruction",
	[RUMBOOT_IRQ_PREFETCH_ABORT] = "Prefetch Abort",
	[RUMBOOT_IRQ_DATA_ABORT] = "Data Abort"
};

void rumboot_irq_core_dispatch(uint32_t type, uint32_t id)
{
	switch (type) {
	case RUMBOOT_IRQ_TYPE_NORMAL:
		/* Route IRQ to controller driver */
		
		break;
	case RUMBOOT_IRQ_TYPE_EXCEPTION:
		rumboot_printf("FATAL: %s\n", exception_names[id]);
		rumboot_panic();
		break;
	}
}
