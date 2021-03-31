#include <rumboot/boot.h>
#include <rumboot/platform.h>
__attribute__((weak)) const struct rumboot_secondary_cpu *rumboot_platform_get_secondary_cpus(int *cpu_count)
{
    *cpu_count = 0;
    return NULL;
}

int __attribute__((weak)) rumboot_platform_selftest(struct rumboot_config *conf)
{
        return 0;
}