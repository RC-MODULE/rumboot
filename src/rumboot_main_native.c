#include <stdint.h>
#include <string.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <stdlib.h>
#include <stdio.h>
struct rumboot_runtime_info
__attribute__((section(".rumboot_platform_runtime_info")))
rumboot_platform_runtime_info;


int g_argc = 0;
char *g_argv[64];

void __attribute__((constructor)) rumboot_main()
{
    /* Clean up everything beyound marker */
    memset(&rumboot_platform_runtime_info, 0x0, sizeof(rumboot_platform_runtime_info));
    rumboot_platform_setup();
}
