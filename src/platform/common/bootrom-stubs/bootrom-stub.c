#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/bootheader.h>
#include <rumboot/pcie_test_lib.h>

int main()
{
	rumboot_print_logo();
	struct rumboot_bootheader *hdr = (struct rumboot_bootheader *)0x40000;

	rumboot_platform_request_file("IM0BIN", (uint32_t)hdr);


    #ifdef RUMBOOT_MIRROR_STUB
        /* Set up mirror mode, cut half of memory for it */
		rumboot_printf("STUB: Configuring PCIe, this will take a while...\n");
        pcie_mirror_tests_setup();
    #endif

	rumboot_printf("STUB: Executing SPL image. Magic: 0x%x Entry: 0x%x\n",
		       hdr->magic, hdr->entry_point[0]);

	int (*ram_main)();
	ram_main = (void *)hdr->entry_point[0];
	rumboot_platform_perf("IM0 startup");
	ram_main();

	rumboot_platform_panic("ROM STUB: We're back in ROM code, we shouldn't be\n");
}
