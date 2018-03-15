#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <rumboot/bootheader.h>
#include <rumboot/irq.h>

#include <devices/mdma.h>

#include <platform/interrupts.h>
#include <platform/devices.h>

int main()
{
	size_t data_size = 4;
	char *src = malloc(data_size);
	char *dst = malloc(data_size);

	memset(src, 0x55, data_size);
	memset(dst, 0x0, data_size);

	uint32_t base = MDMA0_BASE;

	rumboot_printf("Hello world from IRAM!: %d\n");

	//Create IRQ
	struct rumboot_irq_entry *tbl = rumboot_irq_create(NULL);
	rumboot_irq_set_handler(tbl, MDMA0_IRQ, 0, mdma_irq_handler, (void *)&base);
	rumboot_irq_enable(MDMA0_IRQ);
	rumboot_irq_table_activate(tbl);

	if (!mdma_transmit_data(base, dst, src, data_size)) {
		return -1;
	}

  //Remove IRQ
	rumboot_irq_table_activate(NULL);
	rumboot_irq_free(tbl);

	rumboot_printf("Compare arrays.\n");
	if (memcmp(src, dst, data_size) != 0) {
		return -1;
	}

	return 0;
}
