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
    iowrite32(0xdeadc0de, 0x4);
    iowrite32(0xdeadf00d, 0x8);
    asm("msync");
    rumboot_printf("Reading stuffs\n");
    /* ---- */
    rumboot_printf("%x %x\n", ioread32(0x4), ioread32(0x4));
    /* ---- */
    rumboot_printf("%x %x\n", ioread32(0x8), ioread32(0x8));
    return 0;
}
