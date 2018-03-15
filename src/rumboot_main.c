#include <stdint.h>
#include <string.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <stdlib.h>

extern int main();

#include <rumboot/printf.h>
struct rumboot_runtime_info
__attribute__((section(".rumboot_platform_runtime_info")))
rumboot_platform_runtime_info;


extern void __libc_init_array (void);
void rumboot_main()
{
    /*
     * This is common initialization code needed to bring up proper C environment.
     * It does the following:
     * 1. Initialize the global runtime table with sane initial values
     * 2. Initialize bss section with zeroes (if we're using the BSS)
     * 3. Calls main() function
     */

     /* Initialize the runtime info, avoid memset since event system
        is not up yet
     */
     rumboot_platform_runtime_info.magic = 0xb00bc0de;
     rumboot_platform_runtime_info.in.opcode  = 0;
     rumboot_platform_runtime_info.out.opcode = 0;
     /* Start event processing ! */
     rumboot_platform_runtime_info.in.magic  = RUMBOOT_SYNC_MAGIC_IN;
     rumboot_platform_runtime_info.out.magic = RUMBOOT_SYNC_MAGIC_OUT;


     /* Clean up everything beyound marker */
     memset(&rumboot_platform_runtime_info.clean_me_marker, 0x0,
          /* Holy fuck this looks is weird */
          sizeof(rumboot_platform_runtime_info)
               + ((void *)&rumboot_platform_runtime_info)
               - ((void *)&rumboot_platform_runtime_info.clean_me_marker)
     );

     /* Zero-out BSS, if any */
     #ifndef RUMBOOT_ONLY_STACK
          memset(&rumboot_platform_bss_start, 0x0, &rumboot_platform_bss_end - &rumboot_platform_bss_start);
     #endif

     #ifdef RUMBOOT_DATA_FROM_ROM
          uint32_t len = (&rumboot_data_end - &rumboot_data_start);
          rumboot_printf("Copy %d bytes of .data\n", len);
          memcpy(&rumboot_data_start, &rumboot_rom_data, len);
     #endif

     /* Make sure IRQs are off */
     rumboot_irq_cli();

     /* Initialize interrupt controller */
     rumboot_platform_irq_init();

     /* Call Platform-specific setup code (e.g. init the event system) */
     rumboot_platform_setup();

     /* Run constructors */
     __libc_init_array();

     /* Tell environment that we're done with startup */
     rumboot_platform_perf(NULL);

     /* call main() */
     int ret = main();

     /*  Now, let's handle the exit code from main
      *  This will also call destructors
      */
     exit(ret);

     /* Finally, if we're here - something didn't work out, loop forever */
     while(1) {}
}
