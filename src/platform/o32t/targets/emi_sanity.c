#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <platform/devices.h>

int main()
{
    rumboot_printf("Writing stuffs!\n");
    iowrite32(0x4, 0xdeadc0de);
    iowrite32(0x8, 0xdeadf00d);
    asm("msync");
    rumboot_printf("Reading stuffs\n");
    rumboot_printf("%x %x\n", ioread32(0x4), ioread32(0x4));
    rumboot_printf("%x %x\n", ioread32(0x8), ioread32(0x8));
    return 0;
}
