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
#include <rumboot/timer.h>

#ifndef COM_BASE
    #error Please provide COM_BASE
#endif

#ifndef TARGET_FREQ
#define TARGET_FREQ 10000
#endif

#ifndef BASE_FREQ
    #define BASE_FREQ 100000
#endif


#ifndef XFER_SIZE_WORDS
#define XFER_SIZE_WORDS 2048
#endif

/* No printf's in production, since bootrom won't be listening to the fifo */
#ifndef CMAKE_BUILD_TYPE_DEBUG
#define rumboot_printf(fmt, ...)
#endif

/* This shit receives data at the start of nmc memory and jumps there after getting 256 words */
static void (*spl_main)() = 0; 
char staging_buffer[XFER_SIZE_WORDS * 8] __attribute__ ((aligned (16)));

int main()
{
    struct rcm_cp_instance cp; 
    cp_instance_init(&cp, COM_BASE, BASE_FREQ);
    cp_set_speed(&cp, TARGET_FREQ);
    rumboot_printf("nmstub: Initialized cp at 0x%x, receiving to virt: 0x%x (dma: 0x%x) total len: %d bytes\n", 
        COM_BASE, staging_buffer, rumboot_virt_to_dma(staging_buffer), XFER_SIZE_WORDS * 8);
    udelay(10);
    cp_start_rx(&cp, staging_buffer, XFER_SIZE_WORDS * 8);
    cp_wait(&cp, 1, 1, XFER_SIZE_WORDS * 8 * 50);
    rumboot_printf("nmstub: Payload received, moving it to the start\n");
    rumboot_printf("staging: %x %x %x\n", staging_buffer[0], staging_buffer[1], staging_buffer[2]);
    memmove(0x0, staging_buffer, XFER_SIZE_WORDS * 8);
    rumboot_printf("nmstub: Executing it\n");
    spl_main();
    rumboot_printf("nmstub: Back from spl, WTF?\n");
    return 1;
}
