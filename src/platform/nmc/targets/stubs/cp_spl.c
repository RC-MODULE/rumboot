#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <platform/devices.h>
#include <devices/rcm_cp.h>
#include <rumboot/macros.h>
#include <rumboot/timer.h>
#include <rumboot/io.h>

#ifndef COM_BASE
    #error Please provide COM_BASE
#endif

#ifndef TARGET_FREQ
    #define TARGET_FREQ 10000
#endif

#ifndef BASE_FREQ
    #define BASE_FREQ 100000
#endif

#ifdef CMAKE_BUILD_TYPE_DEBUG
#define debug_printf(fmt, ...) rumboot_printf(fmt, ##__VA_ARGS__)
#else
#define debug_printf(fmt, ...) 
#endif

uint64_t __attribute__ ((aligned (8))) txbuf[8];
uint64_t __attribute__ ((aligned (8))) rxbuf[8];

int main()
{
    debug_printf("SPL: Started, cp 0x%x txbuf %x rxbuf %x\n", COM_BASE, rxbuf, txbuf);
    struct rcm_cp_instance cp; 

    txbuf[0] = 'H';
    txbuf[1] = 'e';
    txbuf[2] = 'l';
    txbuf[3] = 'l';
    txbuf[4] = 'o';
    txbuf[5] = '\n';
    txbuf[6] = 1ULL << 63;

    cp_instance_init(&cp, COM_BASE, BASE_FREQ);
    cp_set_speed(&cp, TARGET_FREQ);
    cp_start_tx(&cp, txbuf, 7 * 8);
    debug_printf("SPL: Sending message\n");
    cp_wait_tx(&cp, ARRAY_SIZE(txbuf) * 8 * 50);
    debug_printf("SPL: Tx done\n");

#if 0
    #ifndef CMAKE_BUILD_TYPE_DEBUG
        cp_start_rx(&cp, rxbuf, 3 * 8); /* Fire up rx */
        /* Actually do something with RX */ 
        debug_printf("SPL: Message sent, receiving\n");        
        cp_wait_rx(&cp, 3 * 8 * 50);
        debug_printf("SPL: Done, sending back\n");
        cp_start_tx(&cp, rxbuf, 3 * 8);
        cp_wait_tx(&cp, 3 * 8 * 50);
    #endif // !CMAKE_BUILD_TYPE_DEBUG

    debug_printf("SPL: Done, requesting termination\n");
    txbuf[0] = 1ULL << 63;
    cp_start_tx(&cp, txbuf, 1 * 8);
    cp_wait_tx(&cp, 1 * 8 * 50);
#endif
    while(1);
}