#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <platform/devices.h>
#include <rumboot/boot.h>
#include <rumboot/irq.h>

int main()
{
        rumboot_printf("Hello, I will check exception handling.\n");
        rumboot_irq_set_exception_handler(rumboot_arch_exception);
        struct rumboot_config conf;
        rumboot_platform_read_config(&conf);
        rumboot_platform_init_loader(&conf);

#if defined(__PPC__)
        asm ("sc");
#elif defined(__arm__)
        asm volatile (".word 0xf7f0a000\n");
#elif defined(RUMBOOT_NATIVE)
        kill(getpid(), SIGILL);
#else
        #error "Not supported on this architecture"
#endif

        return 1;
}
