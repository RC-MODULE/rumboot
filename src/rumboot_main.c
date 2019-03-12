#include <stdint.h>
#include <string.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <stdlib.h>
#include <rumboot/printf.h>
#include <stdio.h>


extern int main();

struct rumboot_runtime_info
__attribute__((section(".rumboot_platform_runtime_info")))
rumboot_platform_runtime;


extern void (*__preinit_array_start []) (void) __attribute__((weak));
extern void (*__preinit_array_end []) (void) __attribute__((weak));
extern void (*__init_array_start []) (void) __attribute__((weak));
extern void (*__init_array_end []) (void) __attribute__((weak));

extern void _init();
static void rumboot_init_array (void)
{
        size_t count;
        size_t i;

        count = __preinit_array_end - __preinit_array_start;
        for (i = 0; i < count; i++)
                __preinit_array_start[i] ();

        _init();

        count = __init_array_end - __init_array_start;
        for (i = 0; i < count; i++)
                __init_array_start[i] ();
}

int rumboot_main()
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
        rumboot_platform_runtime_info->magic = 0xb00bc0de;
        rumboot_platform_runtime_info->in.opcode  = 0;
        rumboot_platform_runtime_info->out.opcode = 0;
        /* Start event processing ! */
        rumboot_platform_runtime_info->in.magic  = RUMBOOT_SYNC_MAGIC_IN;
        rumboot_platform_runtime_info->out.magic = RUMBOOT_SYNC_MAGIC_OUT;


        /* Clean up everything beyound marker */
        memset(&rumboot_platform_runtime.clean_me_marker, 0x0,
               /* Holy fuck this looks is weird */
               sizeof(rumboot_platform_runtime)
               + ((void *)&rumboot_platform_runtime)
               - ((void *)&rumboot_platform_runtime.clean_me_marker)
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


        /* Call Platform-specific setup code (e.g. init the event system) */
        rumboot_platform_setup();

        /* Run constructors */
        rumboot_init_array();

        /* Tell environment that we're done with startup */
        rumboot_platform_perf(NULL);

        /*
         * This is quite hacky. We do a setjmp before calling main().
         * _exit() does a longjmp back here with a mangled exit code
         * on real hardware. We demangle return exit code from here.
         * This way we can chain post-production tests on one SPI flash
         * for all our production testing
         *
         * The return value is the test_exit_code + 256
         */
        int ret = setjmp(rumboot_platform_runtime.exit_trampoline);
        if (ret == 0) {
                /* That's how we do it the very first time */
                ret = main();
                exit(ret); /* Does a longjmp */
                /* Never reached */
        }

        /* If we are here, we're back from longjmp */
        rumboot_platform_perf("Trampoline");
        
      #ifdef RUMBOOT_MAIN_NORETURN
        uint32_t code = ret - 256; 
        rumboot_platform_event_raise(EVENT_TERM, &code, 1);
      #endif

        /* De-mangle exit code from trampoline */
        return ret - 256;
}
