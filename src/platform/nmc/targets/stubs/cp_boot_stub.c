#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <platform/devices.h>
#include <devices/rcm_cp.h>
#include <platform/devices.h>

#ifndef COM_BASE
    #error Please provide COM_BASE
#endif

#ifndef BASE_FREQ
    #define BASE_FREQ 100000
#endif


static void (*spl_main)() = 0; 
int main()
{
    struct rcm_cp_instance cp; 
    cp_instance_init(&cp, COM_BASE, BASE_FREQ);
    rumboot_printf("nmstub: Initialized cp at 0x%x, receiving to 0x%x\n", 
        COM_BASE, rumboot_virt_to_dma(IM1_BASE));
    cp_start_rx(&cp, IM1_BASE, 256 * 8);
    cp_wait(&cp, 1, 1, 256 * 8 * 50);
    rumboot_printf("nmstub: Payload received, jumping to reset vector\n"), 
    spl_main();
}
