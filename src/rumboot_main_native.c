#include <stdint.h>
#include <string.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <stdlib.h>

struct rumboot_runtime_info
__attribute__((section(".rumboot_platform_runtime_info")))
rumboot_platform_runtime_info;

void __attribute__((constructor)) rumboot_main()
{
    /* Clean up everything beyound marker */
    memset(&rumboot_platform_runtime_info, 0x0, sizeof(rumboot_platform_runtime_info));
}
