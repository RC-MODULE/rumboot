#include <rumboot/platform.h>
#include <string.h>
extern int main();

struct rumboot_runtime_info
     __attribute__((section (".rumboot_platform_runtime_info")))
     rumboot_platform_runtime_info;

void rumboot_main()
{
    /*
     * This is common initialization code needed to bring up proper C environment.
     * It does the following:
     * 1. Initialize the global runtime table with sane initial values
     * 2. Initialize bss section with zeroes (if we're using the BSS)
     * 3. Calls main() function
     */

     /* Initialize the runtime info */
     memset(&rumboot_platform_runtime_info, 0x0, sizeof(rumboot_platform_runtime_info));
     rumboot_platform_runtime_info.magic = 0xb00bc0de;
     rumboot_platform_runtime_info.current_heap_end = &rumboot_platform_heap_start;

     /* Zero-out BSS, if any */
     #ifndef RUMBOOT_ONLY_STACK
          memset(rumboot_platform_bss_start, 0x0, rumboot_platform_bss_end - rumboot_platform_bss_start);
     #endif

     /* Call Platform-specific setup code (e.g. init the event system) */
     rumboot_platform_setup();

     /* call main() */
     int ret = main();

     /* Now, let's handle the exit code from main */
     rumboot_platform_raise_event(EVENT_TERM, ret);

     /* Finally, if we're here - something didn't work out, loop forever */
     while(1) {}

}
