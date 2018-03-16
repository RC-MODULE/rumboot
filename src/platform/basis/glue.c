#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <rumboot/platform.h>
#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <arch/arm/irq_macros.h>
#include <platform/devices.h>
#include <devices/sp804.h>
#include <rumboot/rumboot.h>
#include <string.h>
#include <rumboot/pcie_test_lib.h>

/* Platform-specific glue */
uint32_t rumboot_platform_get_uptime()
{
	return (0xFFFFFFFF - sp804_get_value(DIT3_BASE, 1)) / 6.25;
}


extern char rumboot_im0_heap_start;
extern char rumboot_im0_heap_end;
extern char rumboot_im1_heap_start;
extern char rumboot_im1_heap_end;

/* Comes from startup.S */
extern char rumboot_default_irq_vectors;

void setup_mirrored_heap(int id)
{
	uint32_t len = rumboot_malloc_heap_length(id);
	void *ptr = rumboot_malloc_from_heap(id, len / 2);
	char *name = rumboot_malloc_from_heap(0, 16);
	strcpy(name, "PCIE-");
	strcat(name, rumboot_malloc_heap_name(id));
	rumboot_malloc_register_heap(name, BASIS_PHYS(ptr), BASIS_PHYS(ptr + (len/2)));
};

void rumboot_platform_setup()
{
	/*
	 * Make sure VBAR is where we need it.
	 * Needed for handling IRQs in secondary image
	 */
	arm_vbar_set((uint32_t)&rumboot_default_irq_vectors);

	rumboot_malloc_register_heap("IM0",
				     &rumboot_im0_heap_start, &rumboot_im0_heap_end);
	rumboot_malloc_register_heap("IM1",
				     &rumboot_im1_heap_start, &rumboot_im1_heap_end);

	#ifdef RUMBOOT_BASIS_ENABLE_MIRROR
		int heaps = rumboot_malloc_num_heaps();
		int i;
		for (i=0; i < heaps; i++) {
			setup_mirrored_heap(i);
		}
	#endif

	/* Fire timer subsystem */
	struct sp804_conf conf_str;

	conf_str.mode = FREERUN;
	conf_str.interrupt_enable = 1;
	conf_str.clock_division = 16;
	conf_str.width = 32;
	conf_str.load = 0;
	conf_str.bgload = 0;

	sp804_config(DIT3_BASE, &conf_str, 1);
	sp804_enable(DIT3_BASE, 1);

}
