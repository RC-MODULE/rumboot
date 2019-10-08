#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <platform/devices.h>
#include <regs/regs_gpio_pl061.h>
#include <rumboot/timer.h>

int main() {
    rumboot_printf("This is the last test. Congratulations, traveler! You've travelled a long way\n");
    iowrite32 (0xff,GPIO_1_BASE+GPIO_DIR);
    iowrite32 (0x40,GPIO_1_BASE+0x3fc);
    while(1);;
    return 0;
}
