#include <stdio.h>
#include <stdlib.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>

#include <rumboot/rumboot.h>
#include <rumboot/io.h>
#include <platform/devices.h>
#include <platform/regs/sctl.h>

int main()
{
    uint32_t i;
    
    rumboot_printf("================================================================================\n");
    rumboot_printf("=== Start pull_up_disabled ===\n");
    rumboot_printf("Write SCTL reg\n");
    
    for (i = 0; i < 22; i++){
        iowrite32(0x0, SCTL_BASE + IOCFG0 + 4*i); 
    }
    rumboot_printf("=== Finish pull_up_disabled ===\n");
    return 0;
}
