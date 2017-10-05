#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/bootheader.h>

int main()
{
	rumboot_print_logo();
	struct rumboot_bootheader *hdr = (struct rumboot_bootheader *) 0x40000;

    rumboot_platform_request_file("IM0BIN", (uint32_t) hdr);

	rumboot_printf("ROM STUB: Executing image in IM0 without verification\n");
	rumboot_printf("ROM STUB: Magic: 0x%x Entry: 0x%x\n", hdr->magic, hdr->entry_point[0]);

	int (*ram_main)();
	ram_main = (void *) hdr->entry_point[0];
	ram_main();

	rumboot_platform_panic("ROM STUB: We're back in ROM code\n");
}
