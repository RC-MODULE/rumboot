#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/platform.h>
#include <rumboot/printf.h>
#include <rumboot/memfill.h>
#include <rumboot/io.h>
#include <rumboot/macros.h>
#include <arch/irq_macros.h>


__attribute__((no_instrument_function)) void rumboot_platform_event_raise(enum rumboot_simulation_event event,
                                                                          uint32_t *data, uint32_t len)
{
        int i;
        RUMBOOT_ATOMIC_BLOCK() {
                rumboot_printf("EVENT %x: ", event);
                for (i = 0; i < len; i++)
                        rumboot_printf("%x ", data[i]);
                rumboot_printf("\n");
        }
}

__attribute__((no_instrument_function)) enum rumboot_simulation_event
rumboot_platform_event_get(
        volatile uint32_t **data)
{
        /* TODO: */
}

__attribute__((no_instrument_function)) void rumboot_platform_event_clear()
{
}
