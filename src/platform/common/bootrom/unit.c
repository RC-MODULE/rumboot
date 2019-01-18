#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <rumboot/boot.h>
#include <stdbool.h>

#ifndef SIZE
	#define SIZE 4096 * 1024
#endif
int main()
{
	char pdata[512];
#ifndef RUMBOOT_NATIVE
    	struct rumboot_bootheader *hdr = (struct rumboot_bootheader *)&rumboot_platform_spl_start;
#else
    struct rumboot_bootheader *hdr = malloc(SIZE);
#endif

	struct rumboot_config conf;
    rumboot_platform_runtime_info->persistent[0] = 0;
	rumboot_platform_read_config(&conf);
	rumboot_platform_init_loader(&conf);
	rumboot_printf("Hello, I'm bootsource unit test for %d\n", SOURCE);
	rumboot_printf("I'm expecting boot to complete with error code %d\n", EXPECTED);
        int ret = bootsource_try_by_id(SOURCE, pdata, hdr, SIZE);
	rumboot_printf("bootsource_try_by_id returned %d, expecting %d\n", ret, EXPECTED);
	if (ret == EXPECTED) {
		return 0;
	}
	return 1;
}
