#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <rumboot/bootheader.h>


int main()
{
	rumboot_printf("Hello world from IRAM!: %d\n");
	return 0;
}
