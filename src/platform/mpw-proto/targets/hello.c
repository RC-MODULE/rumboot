#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>

int main()
{
	rumboot_print_logo();
	rumboot_printf("rumboot: Yarr! I need moar rum!\n\n");
	/* Return 0, test passed */
	return 0;
}
