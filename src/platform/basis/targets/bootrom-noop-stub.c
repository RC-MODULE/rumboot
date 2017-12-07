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
	rumboot_printf("rumboot-stub: Configuring PCIe\n");
    if (pcie_turn_on_with_options_ep (1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) != 0) {
    	return -1;
	}

	rumboot_printf("rumboot-stub: Entering an everlasting loop\n");


	while (1);;
    return 1;
}
