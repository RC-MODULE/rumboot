#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <rumboot/boot.h>
#include <stdbool.h>
#include <alloca.h>
#include <rumboot/timer.h>

int main()
{
	struct rumboot_config conf;
	rumboot_platform_read_config(&conf);
	rumboot_platform_init_loader(&conf);

    rumboot_platform_sv_event("HOST");
	int c = rumboot_getchar(10000);
	if (c == 'e') {
			/* This will terminate simulation */
			rumboot_platform_sv_event("EASTER_EGG");
	}
	if (c == -1) {
			return 1;
	}
	udelay(100);
	return 2;
}
