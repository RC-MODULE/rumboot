#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <rumboot/bootheader.h>
#include <rumboot/irq.h>
#include <rumboot/rumboot.h>
#include <rumboot/macros.h>

#include <devices/mdma.h>

#include <platform/interrupts.h>
#include <platform/devices.h>

enum memory_heap_ids {
	IM0 = 0,
	IM1 = 1,
	DDR0 = 2,
	DDR1 = 3,
	PCIe = 4
};

int main()
{
	size_t data_size = 8;
	uint32_t base = MDMA0_BASE;

	rumboot_printf("Test MDMA\n");

	//Create IRQ
	struct rumboot_irq_entry *tbl = rumboot_irq_create(NULL);
	rumboot_irq_set_handler(tbl, MDMA0_IRQ, 0, mdma_irq_handler, (void *)&base);
	rumboot_irq_enable(MDMA0_IRQ);
	rumboot_irq_table_activate(tbl);

	enum memory_heap_ids heap_ids[] = { IM1, DDR0, DDR1, PCIe };

	size_t src_heap_id = heap_ids[0];
	size_t dst_heap_id = heap_ids[1];

	//Ring IT!
	while (src_heap_id < dst_heap_id) {

		rumboot_printf("Source memory: %d, destination memory: %d\n", src_heap_id, dst_heap_id);

		if(dst_heap_id == ARRAY_SIZE(heap_ids))
			dst_heap_id = heap_ids[0];

		volatile char *src = rumboot_malloc_from_heap_aligned(src_heap_id, data_size, 8);
		volatile char *dst = rumboot_malloc_from_heap_aligned(dst_heap_id, data_size, 8);

		src_heap_id++;
		dst_heap_id++;

		memset((char *)src, 0xff, data_size);
		memset((char *)dst, 0x0, data_size);

		if (!mdma_transmit_data(base, dst, src, data_size)) {
			rumboot_printf("Test failed!\n");
			return -1;
		}

		rumboot_printf("Compare arrays.\n");
		if (memcmp((char *)src, (char *)dst, data_size) != 0) {
			rumboot_printf("Test failed!\n");
			return -1;
		}

		rumboot_free((char *) src);
		rumboot_free((char *) dst);
	}

	//Remove IRQ
	rumboot_irq_table_activate(NULL);
	rumboot_irq_free(tbl);

	rumboot_printf("Test OK!");
	return 0;
}
