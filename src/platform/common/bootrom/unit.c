#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <rumboot/boot.h>
#include <stdbool.h>

int main()
{
	char pdata[512];
 	const struct rumboot_bootsource *src = &rumboot_platform_get_bootsources()[SOURCE];
	rumboot_printf("Hello, I'm bootsource unit test for %s\n", src->name);
	rumboot_printf("I'm expecting boot to %s\n", EXPECTED ? "pass" : "fail");
	return (EXPECTED == bootsource_try_single(src, pdata)) ? 0 : 1;
}
