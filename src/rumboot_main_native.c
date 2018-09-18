#include <stdint.h>
#include <string.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>

struct rumboot_runtime_info *rumboot_platform_runtime_info;

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

void __attribute__((constructor)) rumboot_main()
{
        rumboot_platform_setup();
        /* Clean up everything beyound marker */
        memset(rumboot_platform_runtime_info, 0x0, sizeof(rumboot_platform_runtime_info));
}
