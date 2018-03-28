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


int main()
{
	size_t data_size = 8;
	uint32_t base = MDMA3_BASE;

	rumboot_printf("Test MDMA with base: %x\n", base);

	size_t num_heaps = rumboot_malloc_num_heaps();
	rumboot_printf("We have %d heaps!\n", num_heaps);
	volatile char *src = NULL;
	volatile char *dst = NULL;

	if (num_heaps == 0) {
		rumboot_printf("We have no one heap!\n");
		return -1;
	}

	if (num_heaps == 1) {
		rumboot_printf("We have only one heap! It isn't enought for test!");
		return -1;
	}

	//Create IRQ
	struct rumboot_irq_entry *tbl = rumboot_irq_create(NULL);
	rumboot_irq_set_handler(tbl, MDMA0_IRQ, 0, mdma_irq_handler, (void *)&base);
	rumboot_irq_enable(MDMA0_IRQ);
	rumboot_irq_table_activate(tbl);

	size_t heap_ids[num_heaps];
	size_t i;
	for (i = 0; i < num_heaps; i++)
		heap_ids[i] = i;

	//Descriptor table we keep in IM0 table.
	size_t src_heap_id = heap_ids[1];
	size_t dst_heap_id = heap_ids[1];
	if (num_heaps > 3) {
		dst_heap_id = heap_ids[2];
	}

	bool begin = true;
	bool end = false;
	volatile char *begin_addr = NULL;
	//Ring IT!
	while (src_heap_id < dst_heap_id) {
		rumboot_printf("Source memory id: %d, destination memory id: %d\n", src_heap_id, dst_heap_id);

		if (begin) {
			src = rumboot_malloc_from_heap_aligned(src_heap_id, data_size, 8);
			begin_addr = src;

			size_t i;
			for (i = 0; i < data_size; i++)
				*(&src[i]) = 0xff;
		} else {
			src = dst;
		}

		if (end) {
			dst_heap_id = heap_ids[1];
			dst = begin_addr;
		} else {
			dst = rumboot_malloc_from_heap_aligned(dst_heap_id, data_size, 8);
		}

		for (i = 0; i < data_size; i++)
			*(&dst[i]) = 0x0;

		rumboot_printf("dst: %x, src: %x\n", &dst[0], &src[0]);
		if (mdma_transmit_data(base, &dst[0], &src[0], data_size) != 0) {
			rumboot_printf("Test failed!\n");
			return -1;
		}

		src_heap_id++;
		dst_heap_id++;
		if (dst_heap_id == num_heaps) {
			end = true;
		}
		begin = false;
	}

	rumboot_printf("Compare arrays.\n");
	if (memcmp((char *)&src[0], (char *)&dst[0], data_size) != 0) {
		rumboot_printf("Test failed!\n");
		return -1;
	}

	rumboot_free((char *)src);
	rumboot_free((char *)dst);

	//Remove IRQ
	rumboot_irq_table_activate(NULL);
	rumboot_irq_free(tbl);

	rumboot_printf("Test OK!");
	return 0;
}
