#include <stdint.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>


void rumboot_arch_stacktrace(void)
{
  /* TODO: .. */
}

void rumboot_arch_exception(int id, const char *name)
{
        rumboot_printf("\n\n\nWE GOT AN EXCEPTION: %d: %s\n", id, name);
        rumboot_printf("--- Guru Meditation ---\n");
        rumboot_printf("  -   Stack trace   -\n");
        rumboot_arch_stacktrace();
        rumboot_printf("---       ---       ---\n");
        rumboot_platform_sv_event("EXCEPTION");
        exit(1);
}
