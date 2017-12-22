#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>

int main()
{

	rumboot_print_logo();

	rumboot_printf("rumboot: HELLO FROM RAM!\n\n");

	while (1) ;

	return 0;
}
