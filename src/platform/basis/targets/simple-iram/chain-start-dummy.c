#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>

#include <rumboot/rumboot.h>
#include <stdlib.h>
#include <regs/regs_gpio_rcm.h>
#include <rumboot/io.h>
#include <platform/devices.h>

int main()
{
	rumboot_printf("This is the first test. Arming GPIOs!\n");
	iowrite32(0xff, GPIO1_BASE + GPIO_PAD_DIR);
	iowrite32(0x0,  GPIO1_BASE + GPIO_WR_DATA_SET0);		
	iowrite32(1<<7, GPIO1_BASE + GPIO_WR_DATA_SET1);	
	return 0;
}
