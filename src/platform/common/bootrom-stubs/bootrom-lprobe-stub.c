#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/boot.h>
#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <rumboot/rumboot.h>
#include <platform/interrupts.h>

enum lprobe_op_type {
	lprobe_op_terminate=0,
	lprobe_op_read8,
	lprobe_op_read16,
	lprobe_op_read32,
	lprobe_op_read64,
	lprobe_op_write8,
	lprobe_op_write16,
	lprobe_op_write32,
	lprobe_op_write64,
	lprobe_op_irq_wait,
	lprobe_op_irq_pending,
	lprobe_op_irq_ack,
	lprobe_op_get_uptime,
	lprobe_op_delay_clocks,
};

struct lprobe_event {
	uint32_t	opcode;
	uint32_t	addr_low;
	uint32_t	addr_high;
	uint32_t	data_low;
	uint32_t	data_high;
} __attribute__((packed));

static inline void parse_event(struct lprobe_event *evt)
{
    struct lprobe_event *out = (struct lprobe_event *) rumboot_platform_runtime_info->out.data;

	out->opcode = evt->opcode;

	switch (evt->opcode) {
	case lprobe_op_read8:
		out->data_low = ioread8(evt->addr_low);
		break;

	case lprobe_op_read16:
		out->data_low = ioread16(evt->addr_low);
		break;

	case lprobe_op_read32:
		out->data_low = ioread32(evt->addr_low);
		break;

	case lprobe_op_read64:
		out->data_low = ioread32(evt->addr_low);
		out->data_high = ioread32(evt->addr_low + 4);
		break;

	case lprobe_op_write8:
		iowrite8(evt->data_low, evt->addr_low);
		break;

	case lprobe_op_write16:
		iowrite16(evt->data_low, evt->addr_low);
		break;

	case lprobe_op_write32:
		iowrite32(evt->data_low, evt->addr_low);
		break;

	case lprobe_op_write64:
		iowrite32(
			(evt->data_low) | ((uint64_t)evt->data_high << 32),
			evt->addr_low);
		break;
	}

	rumboot_platform_runtime_info->out.opcode = EVENT_LPROBE;

}

void lprobe_proxy_irq(int irq)
{
}

int main()
{
	rumboot_print_logo();
	rumboot_irq_set_default_handler(lprobe_proxy_irq);
	rumboot_irq_sei();
	rumboot_printf("STUB: Entering lprobe event handling loop\n");

	/* Send out dummy event to signal that we've started up */
	rumboot_platform_event_raise(EVENT_LPROBE, NULL, 0);
	while (1) {
		volatile uint32_t *data;
		rumboot_platform_event_get(&data);
		//rumboot_printf("Inbound event %d arg 0x%x\n", evt, data[0]);
		parse_event((void *)data);
		rumboot_platform_event_clear();
	}
}
