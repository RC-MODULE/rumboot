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
       int i;
       RUMBOOT_ATOMIC_BLOCK() {
               for (i=0; i<len; i++)
                       rumboot_platform_runtime_info->out.data[i] = data[i];
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
