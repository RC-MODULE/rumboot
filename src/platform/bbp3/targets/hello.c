#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>

#include <rumboot/rumboot.h>
#include <stdlib.h>

int main()
{
    rumboot_print_logo();
	rumboot_printf("Hello world from BBP3 ROM!\n");
	return 0;
}
