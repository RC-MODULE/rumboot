#include <rumboot/printf.h>
#include <rumboot/platform.h>

int main()
{
    rumboot_print_logo();
    rumboot_platform_raise_event(EVENT_PERF, 0);

#if 0
    /* This shit breaks everything */
    rumboot_printf("rumboot: spl start @0x%x\n", rumboot_platform_spl_start);
    rumboot_printf("rumboot: spl end   @0x%x\n", rumboot_platform_spl_end);
    rumboot_printf("rumboot: spl start: 0x%x end:0x%x\n",
        rumboot_platform_spl_start,
        rumboot_platform_spl_end);
#endif

    return 0;
}
