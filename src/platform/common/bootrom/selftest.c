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

	return 	rumboot_platform_selftest(&conf);
}
