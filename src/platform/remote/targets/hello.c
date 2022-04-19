#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>

int main()
{
	rumboot_printf("Hello world: %d !\n", 1);

	return 0;
}
