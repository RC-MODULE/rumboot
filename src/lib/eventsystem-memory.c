#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/platform.h>
#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <rumboot/macros.h>
#include <arch/irq_macros.h>

__attribute__((no_instrument_function)) void rumboot_platform_event_raise(enum rumboot_simulation_event event,
				  uint32_t *data, uint32_t len)
{
	int i;
	RUMBOOT_ATOMIC_BLOCK() {
		for (i=0; i<len; i++)
			rumboot_platform_runtime_info.out.data[i] = data[i];
		rumboot_platform_runtime_info.out.opcode = event;
	}
}

 __attribute__((no_instrument_function)) enum rumboot_simulation_event
	rumboot_platform_event_get(
				  volatile uint32_t **data)
{
	while(!rumboot_platform_runtime_info.in.opcode);;

	*data = rumboot_platform_runtime_info.in.data;
	return rumboot_platform_runtime_info.in.opcode;
}

__attribute__((no_instrument_function)) void rumboot_platform_event_clear()
{
	rumboot_platform_runtime_info.in.opcode = 0;
}

__attribute__((no_instrument_function)) static inline void raise_event_fast(enum rumboot_simulation_event event, uint32_t data)
{
	rumboot_platform_runtime_info.out.data[0] = data;
	rumboot_platform_runtime_info.out.opcode = event;
}

__attribute__((no_instrument_function)) void rumboot_platform_trace(void *pc)
{
//	RUMBOOT_ATOMIC_BLOCK() {
    	rumboot_platform_runtime_info.out.data[0] = (uint32_t) (pc);
    	rumboot_platform_runtime_info.out.data[1] = rumboot_platform_runtime_info.nestlevel;
		rumboot_platform_runtime_info.out.opcode = EVENT_TRACE;
//	}
}

__attribute__((no_instrument_function)) void rumboot_platform_perf(const char *tag)
{
	raise_event_fast(EVENT_PERF, (uint32_t) tag);
}

__attribute__((no_instrument_function)) void rumboot_platform_perf_func(void *addr)
{
	raise_event_fast(EVENT_PERF_FUNC, (uint32_t) addr);
}


void __attribute__((no_instrument_function)) rumboot_platform_putchar(uint8_t c)
{
	raise_event_fast(EVENT_STDOUT, c);
}


#ifdef RUMBOOT_PRINTF_ACCEL

 __attribute__((no_instrument_function)) __attribute__((optimize("-O0"))) void do_memset(void *ptr, ...)
{
	raise_event_fast(EVENT_MEMSET, (uint32_t) __builtin_frame_address(0));
}

__attribute__((no_instrument_function)) void *memset(void *s, int c, size_t n)
{
	do_memset(s, c, n);
	return s;
}

__attribute__((no_instrument_function)) __attribute__((optimize("-O0"))) void do_memcpy(void *ptr, ...)
{
   raise_event_fast(EVENT_MEMCPY, (uint32_t) __builtin_frame_address(0));
}

__attribute__((no_instrument_function)) void *memcpy(void *d, const void *s, size_t n)
{
   do_memcpy(d, s, n);
   return d;
}

 __attribute__((no_instrument_function)) __attribute__((optimize("-O0"))) void rumboot_printf(const char *fmt, ...)
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

void rumboot_platform_sim_save(const char *filename)
{
	uint32_t data[] = { (uint32_t) filename };
	rumboot_platform_event_raise(EVENT_SIM_SAVE, data, ARRAY_SIZE(data));
}

void rumboot_platform_sim_restore(const char *filename)
{
	uint32_t data[] = { (uint32_t) filename };
	rumboot_platform_event_raise(EVENT_SIM_RESTORE, data, ARRAY_SIZE(data));
}

void rumboot_platform_dump_region(const char *filename, uint32_t addr, uint32_t len)
{
	uint32_t data[] = { (uint32_t) filename, addr, len };
	rumboot_platform_event_raise(EVENT_DOWNLOAD, data, ARRAY_SIZE(data));
}

void rumboot_platform_store_gcda(const char *filename, uint32_t addr, uint32_t len)
{
	uint32_t data[] = { (uint32_t) filename, addr, len };
	rumboot_platform_event_raise(EVENT_GCDA, data, ARRAY_SIZE(data));
}

void __attribute__((noreturn)) rumboot_platform_panic(const char *why, ...)
{
	uint32_t *fp = __builtin_frame_address(0);

	if (why) {
        va_list ap;
        rumboot_printf("PANIC: ");
    	va_start(ap, why);
        rumboot_vprintf(why, ap);
    	va_end(ap);
    }

    raise_event_fast(EVENT_STACKTRACE, (uint32_t) fp);
	exit(1);
}
