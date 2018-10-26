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


__attribute__((no_instrument_function)) void rumboot_platform_event_raise(enum rumboot_simulation_event const event,
				  uint32_t const * const data, uint32_t const len)
{
    uint32_t const max_event_params_n = ARRAY_SIZE( rumboot_platform_runtime_info->out.data );
    if( len <= max_event_params_n ) rumboot_platform_panic( "Too many event params. Max: %d, set: %d", max_event_params_n, len );

    RUMBOOT_ATOMIC_BLOCK() {
        memcpy( (void *)(rumboot_platform_runtime_info->out.data), data, len*sizeof(data[0]));
        rumboot_platform_runtime_info->out.opcode = event;
    }
}

 __attribute__((no_instrument_function)) enum rumboot_simulation_event
	rumboot_platform_event_get(
				  volatile uint32_t **data)
{
	while(!rumboot_platform_runtime_info->in.opcode) {}

	*data = rumboot_platform_runtime_info->in.data;
	return rumboot_platform_runtime_info->in.opcode;
}

__attribute__((no_instrument_function)) void rumboot_platform_event_clear()
{
	rumboot_platform_runtime_info->in.opcode = 0;
}
