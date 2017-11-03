#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/platform.h>
#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <rumboot/macros.h>
#include <arch/arm/irq_macros.h>


void rumboot_platform_event_raise(enum rumboot_simulation_event event,
				  uint32_t *data, uint32_t len)
{
	RUMBOOT_ATOMIC_BLOCK() {
		memcpy(rumboot_platform_runtime_info.out.data, data, len * sizeof(*data));
		rumboot_platform_runtime_info.out.opcode = event;
	}
}

enum rumboot_simulation_event rumboot_platform_event_get(
				  uint32_t **data)
{
	while(!rumboot_platform_runtime_info.in.opcode);;

	*data = rumboot_platform_runtime_info.in.data;
	return rumboot_platform_runtime_info.in.opcode;
}

void rumboot_platform_event_clear()
{
	rumboot_platform_runtime_info.in.opcode = 0;
}

static inline void raise_event_fast(enum rumboot_simulation_event event, uint32_t data)
{
	rumboot_platform_runtime_info.out.data[0] = data;
	rumboot_platform_runtime_info.out.opcode = event;
}

void rumboot_platform_trace(void *pc)
{
	raise_event_fast(EVENT_TRACE, (uint32_t) pc);
}

void rumboot_platform_perf(const char *tag)
{
	raise_event_fast(EVENT_PERF, (uint32_t) tag);
}


void rumboot_platform_putchar(uint8_t c)
{
	raise_event_fast(EVENT_STDOUT, c);
}


#ifdef RUMBOOT_PRINTF_ACCEL
void __attribute__((optimize("-O0"))) rumboot_printf(const char *fmt, ...)
{
	/* va-arg func sends r0-r3 registers to stack in prologue
	 * 00000384 <rumboot_sim_printf>:
	 *  384:	e92d000f        push	{r0, r1, r2, r3}
	 *  388:	e92d4800        push	{fp, lr}
	 *  38c:	e28db004        add	fp, sp, #4
	 */
	raise_event_fast(EVENT_PRINTF, (uint32_t) __builtin_frame_address(0));
}
#endif

int rumboot_platform_getchar(uint32_t timeout_us)
{
	return 0;
}

void rumboot_platform_request_file(const char *plusarg, uint32_t addr)
{
	uint32_t data[] = { (uint32_t) plusarg, addr };
	rumboot_platform_event_raise(EVENT_UPLOAD, data, ARRAY_SIZE(data));
}

void rumboot_platform_dump_region(const char *filename, uint32_t addr, uint32_t len)
{
	uint32_t data[] = { (uint32_t) filename, addr, len };
	rumboot_platform_event_raise(EVENT_DOWNLOAD, data, ARRAY_SIZE(data));
}
