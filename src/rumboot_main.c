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

      // Remove this when correct finish done
     /* TODO */
     uint32_t volatile *ptr_gpio0_data = 0x010403FC;
     uint32_t volatile *ptr_gpio0_dir  = 0x01040400;
     uint32_t volatile *ptr_gpio1_data = 0x010413FC;
     uint32_t volatile *ptr_gpio1_dir  = 0x01041400;
       // Init GPIO
     *ptr_gpio1_dir  = 0x000000FF;
     *ptr_gpio1_data = 0x00000000;
     *ptr_gpio0_dir  = 0x000000FF;
     *ptr_gpio0_data = 0x00000000;
     
     *ptr_gpio1_data = 0x000000FF;   // Write Tube Command PRINTFINISH
     *ptr_gpio0_data = 0x00000007;   //   With Data 0xFF
     main();
}
