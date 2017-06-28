#include <rumboot/printf.h>
#include <rumboot/platform.h>

int main()
{
    rumboot_print_logo();
    rumboot_printf("rumboot: spl start: 0x%x end:0x%x\n",
        rumboot_platform_spl_start,
        rumboot_platform_spl_end);
    rumboot_printf("rumboot: stack @0x%x\n",
            rumboot_platform_stack_top);
    rumboot_printf("rumboot: Yarr! I need moar rum!");
    /* Return 0, test passed */
    return 0;
}
