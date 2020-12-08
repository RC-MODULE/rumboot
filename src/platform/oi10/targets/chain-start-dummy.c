#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <platform/devices.h>
#include <regs/regs_gpio_pl061.h>
#include <rumboot/timer.h>

int main() {
	rumboot_printf("This is the first test. Arming GPIOs!\n");
    iowrite32 (0xff,GPIO_1_BASE+GPIO_DIR);
    iowrite32 (0x80,GPIO_1_BASE+0x3fc);
	return 0;
}
