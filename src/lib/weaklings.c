#include <rumboot/boot.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>

/*
 *   Direct linux booting requires us to set up a few registers, that are populated via bootargs array.
 *   Cortex-A5:
 *   r0 = 0, r1 = machine nr, r2 = atags or dtb pointer.
 *   PPC440x:
 *   r3 - Board info structure pointer (DRAM, frequency, MAC address, etc.)
 *   r4 - Starting address of the init RAM disk
 *   r5 - Ending address of the init RAM disk
 *   r6 - Start of kernel command line string (e.g. "mem=128")
 *   r7 - End of kernel command line string
 * 
 */

static const struct rumboot_cpu_cluster own[] = {
    {
        #ifdef __PPC__
        .name = "PPC (boot)",
        #elif defined(__arm__)
        .name = "ARM (boot)",
        #elif defined(__NM__)        
        .name = "NMC (boot)",
        #elif defined(__riscv)        
        .name = "SCR1 (boot)",
        #else
        .name = "Unknown (boot)",
        #endif
        .start = rumboot_bootimage_jump_to_ep_with_args,
    }, 
};

__attribute__((weak)) const struct rumboot_cpu_cluster *rumboot_platform_get_cpus(int *cpu_count)
{
    *cpu_count = ARRAY_SIZE(own);
    return own;
}

int __attribute__((weak)) rumboot_platform_selftest(struct rumboot_config *conf)
{
        return 0;
}

/* TODO: Reference AES-CBC encryption */
int rumboot_platform_decrypt_buffer(const struct rumboot_bootsource *src, int encryption_slot, void *data, size_t datasize)
{
    return 0;
}

int rumboot_platform_verify_signature(const struct rumboot_bootsource *src, int certificate_slot, void *data, size_t datasize)
{
    return 0;
}