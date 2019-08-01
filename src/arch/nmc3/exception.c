#include <rumboot/printf.h>
#include <rumboot/platform.h>
void rumboot_arch_exception(int id, const char *name)
{
    rumboot_platform_panic("Exception %d: %s\n", id, name);
}
