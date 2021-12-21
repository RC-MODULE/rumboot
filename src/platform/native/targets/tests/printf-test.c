#include <stdio.h>
#include <stdint.h>
#include <rumboot/printf.h>


int main()
{
	rumboot_printf("Hello, this is our fancy built-in printf implementation\n");
	rumboot_printf("%d %x\n", 15, 15);
	return 0;
}
