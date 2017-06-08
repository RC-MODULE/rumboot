#include <rumboot/printf.h>
#include <rumboot/platform.h>

int main()
{
    rumboot_print_logo();
    rumboot_printf("rumboot: spl start: 0x%x end:0x%x\n",
        rumboot_platform_spl_start,
        rumboot_platform_spl_end);
}
