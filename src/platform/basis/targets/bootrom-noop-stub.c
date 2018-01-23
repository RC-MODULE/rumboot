#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/bootheader.h>
#include <rumboot/pcie_test_lib.h>


struct __attribute__((packed)) check_data {
	uint64_t	u64;
	uint32_t	u32;
	uint16_t	u16;
	uint8_t	u8;
};

const  struct check_data arr = {
	.u64 = 0x1fffdeadc0be,
	.u32 = 0xb00bc0de,
	.u16 = 0xc0de,
	.u8  = 0xfe,
};


int main()
{
	rumboot_print_logo();

	rumboot_printf("rumboot-stub: Copying test data to 0x%x\n", &rumboot_platform_spl_start);
	memcpy(&rumboot_platform_spl_start, &arr, sizeof(arr));

	rumboot_printf("rumboot-stub: Configuring PCIe\n");
    if (pcie_turn_on_with_options_ep (1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) != 0) {
    	return -1;
	}

	rumboot_printf("rumboot-stub: Entering an everlasting loop\n");


	while (1);;
    return 1;
}
