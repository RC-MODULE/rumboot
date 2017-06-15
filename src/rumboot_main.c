#include <rumboot/platform.h>
#include <string.h>
extern int main();
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
     rumboot_platform_runtime_info.magic = 0xb00bc0de;
     rumboot_platform_runtime_info.current_max_heap = rumboot_platform_heap_start;

     #ifdef RUMBOOT_HAS_BSS
          memset(rumboot_platform_bss_start, 0x0, rumboot_platform_bss_end - rumboot_platform_bss_start);
     #endif

     main();
}
