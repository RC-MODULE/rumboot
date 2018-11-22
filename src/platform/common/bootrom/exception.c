#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <platform/devices.h>
#include <rumboot/boot.h>

int main()
{
        rumboot_printf("Hello, I will check exception handling.\n");

        struct rumboot_config conf;
        rumboot_platform_read_config(&conf);
        rumboot_platform_init_loader(&conf);

#ifdef __PPC__
        asm ("sc");
#endif
#ifdef __ARM__
        asm volatile (".word 0xf7f0a000\n");
#endif

        return 1;
}
