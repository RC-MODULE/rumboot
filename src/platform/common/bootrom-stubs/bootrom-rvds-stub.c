#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/bootheader.h>
#include <rumboot/irq.h>


int main()
{
	rumboot_print_logo();
	volatile uint32_t *vtable = (volatile uint32_t *) 0x40000;

	rumboot_platform_request_file("IM0BIN", (uint32_t) vtable);

	int (*ram_main)();
	ram_main = (void *) vtable;
	arm_vbar_set((uint32_t) vtable);
	rumboot_printf("Now executing RAM reset vector: %x\n", vtable[0]);
	ram_main();
	rumboot_platform_panic("ROM STUB: We're back in ROM code, we shouldn't be\n");
}
