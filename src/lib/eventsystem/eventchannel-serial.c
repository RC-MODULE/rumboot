#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/platform.h>
#include <rumboot/printf.h>
#include <rumboot/memfill.h>
#include <rumboot/io.h>
#include <rumboot/macros.h>
#include <rumboot/xmodem.h>
#include <rumboot/hexdump.h>


void rumboot_platform_request_file(const char *plusarg, uint32_t addr)
{
        rumboot_printf("UPLOAD: %s to 0x%x\n", plusarg, addr);
        xmodem_get((void *) addr, 8*1024*1024);
}


__attribute__((no_instrument_function)) void rumboot_platform_event_raise(enum rumboot_simulation_event event,
                                                                          uint32_t const *data, uint32_t len)
{

        if (event == EVENT_TRACE) {
                rumboot_printf("TRACE: %d %x\n", data[1], data[0]);
        }
#if 0
        int i;
                rumboot_printf("EVENT %x: ", event);
                for (i = 0; i < len; i++)
                        rumboot_printf("%x ", data[i]);
                rumboot_printf("\n");
#endif
}

__attribute__((no_instrument_function)) enum rumboot_simulation_event
rumboot_platform_event_get(
        volatile uint32_t **data)
{
        while(1);;
        return 0;
}

__attribute__((no_instrument_function)) void rumboot_platform_event_clear()
{
}
