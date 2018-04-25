#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <rumboot/bootheader.h>
#include <rumboot/rumboot.h>
#include <stdlib.h>

int main()
{
    rumboot_platform_perf("newlib malloc");
	char *tmp = malloc(2048);
	rumboot_platform_perf(NULL);

	rumboot_platform_perf("newlib malloc");
	tmp = rumboot_malloc(2048);
	rumboot_platform_perf(NULL);

	rumboot_printf("Hello world from IRAM!: %d\n", tmp);
	return 0;
}
