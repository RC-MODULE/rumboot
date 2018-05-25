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
    iowrite32(0xdeadbeef, IM0_BASE);
    rumboot_print_logo();
#if 0
    rumboot_printf("rumboot: spl start: 0x%x end: 0x%x\n",
               &rumboot_platform_spl_start,
               &rumboot_platform_spl_end);
    rumboot_printf("rumboot: Yarr! I need moar rum!\n\n");
#endif
    return 0;
}
