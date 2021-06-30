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

uint64_t __attribute__ ((aligned (16))) txbuf[16];
uint64_t __attribute__ ((aligned (16))) rxbuf[16];

int main()
{
    rumboot_platform_runtime_info->magic = 0xb00bc0de;
    rumboot_platform_runtime_info->in.opcode  = 0;
    rumboot_platform_runtime_info->out.opcode = 0;
    /* Start event processing ! */
    rumboot_platform_runtime_info->in.magic  = RUMBOOT_SYNC_MAGIC_IN;
    rumboot_platform_runtime_info->out.magic = RUMBOOT_SYNC_MAGIC_OUT;
    /* TODO: Send a char via cp */
    debug_printf("SPL: Started, cp 0x%x txbuf %x rxbuf %x\n", COM_BASE, rxbuf, txbuf);
    struct rcm_cp_instance cp; 

    txbuf[0] = 'H';
    txbuf[1] = 'e';
    txbuf[2] = 'l';
    txbuf[3] = 'l';
    txbuf[4] = 'o';
    txbuf[5] = '\n';
    txbuf[6] = 1UL<<63;

    //iowrite32(0x0, TIMERS_BASE); 
    //iowrite32(0x3, TIMERS_BASE + 8);

    cp_instance_init(&cp, COM_BASE, BASE_FREQ);
    cp_set_speed(&cp, TARGET_FREQ);
    debug_printf("SPL: Sending message\n");
    cp_start_tx(&cp, txbuf, 8 * 8);
    debug_printf("SPL: Sending message\n");
    cp_wait_tx(&cp, ARRAY_SIZE(txbuf) * 8 * 50);
    debug_printf("SPL: Message sent\n");
    while(1);
    return 0;

/*
    cp_start_rx(&cp, tmp, 7 * 8);
    tmp[7] = 1UL << 63;
    cp_wait(&cp, 1, 1, ARRAY_SIZE(tmp) * 8 * 50);
    debug_printf("SPL: Message received\n");

    cp_start_tx(&cp, tmp, 8 * 8);
    cp_wait(&cp, 1, 1, ARRAY_SIZE(tmp) * 8 * 50);
    debug_printf("SPL: Message sent\n");
    udelay(1000);
    debug_printf("SPL: Still running? Doing emergency exit\n");
	uint32_t c = 8;
    rumboot_platform_event_raise(EVENT_TERM, &c, 1);
    return 1;
    */
}