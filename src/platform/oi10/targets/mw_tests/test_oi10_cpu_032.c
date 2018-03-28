#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>

int main()
{
#if 0
    rumboot_print_logo();
    rumboot_printf("rumboot: spl start: 0x%x end: 0x%x\n",
               &rumboot_platform_spl_start,
               &rumboot_platform_spl_end);
    rumboot_printf("test_oi10_cpu_032\n\n");
#endif
    return 0;
}
