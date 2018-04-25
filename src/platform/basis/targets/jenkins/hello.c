#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>

int main()
{
	rumboot_print_logo();
    rumboot_platform_perf("printf");
	rumboot_printf("rumboot: spl start: 0x%x end: 0x%x\n",
		       &rumboot_platform_spl_start,
		       &rumboot_platform_spl_end);
	rumboot_printf("rumboot: Yarr! I need moar rum!\n\n");
    rumboot_platform_perf(NULL);
	/* Return 0, test passed */
	return 0;
}
