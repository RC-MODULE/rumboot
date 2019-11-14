#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <rumboot/io.h>
#include <rumboot/rumboot.h>
#include <stdlib.h>
#include <rumboot/timer.h>

int main()
{
	int i;
		iowrite32(0x8, (void *)0xCC010);
	while(1) {
		mdelay(1000);
		iowrite32(0x0, (void *)0xCC000);	
		mdelay(1000);
		iowrite32(0x8, (void *)0xCC000);	
	}
    //rumboot_print_logo();
	//rumboot_printf("Hello world from BBP3 ROM!\n");
	return 0;
}
