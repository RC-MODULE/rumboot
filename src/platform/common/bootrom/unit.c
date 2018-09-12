#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <rumboot/boot.h>
#include <stdbool.h>

#define SIZE 2 * 1024
int main()
{
	char pdata[512];
	#ifndef RUMBOOT_NATIVE
    	struct rumboot_bootheader *hdr = (struct rumboot_bootheader *)&rumboot_platform_spl_start;
	#else
    	struct rumboot_bootheader *hdr = malloc(SIZE);
	#endif
 	const struct rumboot_bootsource *src = &rumboot_platform_get_bootsources()[SOURCE];
	rumboot_printf("Hello, I'm bootsource unit test for %s\n", src->name);
	rumboot_printf("I'm expecting boot to %s\n", EXPECTED ? "pass" : "fail");
	return (EXPECTED == bootsource_try_single(src, pdata, hdr, SIZE)) ? 0 : 1;
}
