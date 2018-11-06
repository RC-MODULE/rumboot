#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/platform.h>
#include <rumboot/printf.h>
#include <rumboot/memfill.h>
#include <rumboot/io.h>
#include <rumboot/macros.h>

/*
gtube::FlightControl: Received event 0x0a ARG1 0x8001fd48 0x00 0x00 0x00 ...
OpHandlerMemset: memset addr 0x8001fe74 chr 0x00000000 len 0x000000c8

gtube::FlightControl: Received event 0x0a ARG1 0x8001fd38 0x00 0x00 0x00 ...
OpHandlerMemset: memset addr 0x8001fd38 chr 0xba4e1074 len 0x8001fe74

 */

 static __attribute__((no_instrument_function)) void deliver(enum rumboot_simulation_event event, uint32_t data)
 {
 	rumboot_platform_event_raise(event, &data, 1);
 }

#ifdef RUMBOOT_PRINTF_ACCEL

#ifndef CMAKE_BUILD_TYPE_DEBUG
	#error "Simulation offloading should not be enabled for production builds"
#endif

 __attribute__((no_instrument_function)) __attribute__((optimize("-O0"))) void do_memset(void *ptr, ...)
{
	deliver(EVENT_MEMSET, (uint32_t)__builtin_frame_address(0));
}

__attribute__((no_instrument_function)) void *memset(void *s, int c, size_t n)
{
	do_memset(s, c, n);
	return s;
}

 __attribute__((no_instrument_function)) __attribute__((optimize("-O0"))) void do_memfillseq(void *ptr, ...)
{
	deliver(EVENT_MEMFILLSEQ, (uint32_t)__builtin_frame_address(0));
}

__attribute__((no_instrument_function)) int rumboot_memfill32(void *addr, size_t sz, int val, int incr)
{
	do_memfillseq(addr,sz,val,incr);
	return 0;
}

 __attribute__((no_instrument_function)) __attribute__((optimize("-O0"))) void do_memcheckseq(void *ptr0,  ...)
{
	deliver(EVENT_MEMCHECKSEQ, (uint32_t) __builtin_frame_address(0));
}

__attribute__((no_instrument_function)) int rumboot_memcheck32(void *src, void *dst, size_t sz)
{
	do_memcheckseq(src, dst, sz);
  return 0;
}

 __attribute__((no_instrument_function)) __attribute__((optimize("-O0"))) void do_memcmp(void *ptr0,  ...)
{
    deliver(EVENT_MEMCMP, (uint32_t) __builtin_frame_address(0));
}

__attribute__((no_instrument_function)) int rumboot_memcmp(void *src, void *dst, size_t sz)
{
    volatile uint32_t data;
    do_memcmp(src, dst, sz, &data);
    return data;
  return 1;
}

__attribute__((no_instrument_function)) __attribute__((optimize("-O0"))) void do_memcpy(void *ptr, ...)
{
   deliver(EVENT_MEMCPY, (uint32_t) __builtin_frame_address(0));
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
   	deliver(EVENT_PRINTF, (uint32_t) __builtin_frame_address(0));
}

 __attribute__((no_instrument_function))
 __attribute__((noreturn))
 __attribute__((optimize("-O0")))
 void rumboot_platform_panic(const char *why, ...)
{
	if (why) {
		deliver(EVENT_PRINTF, (uint32_t) __builtin_frame_address(0));
    }
	deliver(EVENT_STACKTRACE, (uint32_t) __builtin_frame_address(0));
	exit(1);
}


#else

__attribute__((no_instrument_function))
__attribute__((noreturn))
void rumboot_platform_panic(const char *why, ...)
{
   deliver(EVENT_STACKTRACE, (uint32_t) __builtin_frame_address(0));

   if (why) {
	   va_list ap;
	   rumboot_printf("PANIC: ");
	   va_start(ap, why);
	   rumboot_vprintf(why, ap);
	   va_end(ap);
   }
   exit(1);
}

__attribute__((no_instrument_function)) int rumboot_memcheck32(void *src, void *dst, size_t sz)
{
  rumboot_platform_panic("NOT IMPLEMENTED");
  return 123;
}

__attribute__((no_instrument_function)) int rumboot_memcmp(void *src, void *dst, size_t sz)
{
    uint8_t * rumboot_memcheck8_src_addr;
    uint8_t * rumboot_memcheck8_dst_addr;
    int data_ok = 0;
    rumboot_memcheck8_src_addr = (uint8_t *)src;
    rumboot_memcheck8_dst_addr = (uint8_t *)dst;
    for (uint32_t i=0; i<sz; ++rumboot_memcheck8_src_addr, ++rumboot_memcheck8_dst_addr, ++i )
    {
#ifdef CMAKE_BUILD_TYPE_DEBUG
        rumboot_printf("Check#%d\n", i);
        rumboot_printf("src[0x%X]: 0x%x\ndst[0x%X]: 0x%x\n",
                rumboot_memcheck8_src_addr,
                *rumboot_memcheck8_src_addr,
                rumboot_memcheck8_dst_addr,
                *rumboot_memcheck8_dst_addr);
#endif
        if (*rumboot_memcheck8_src_addr!=*rumboot_memcheck8_dst_addr)
        {
            rumboot_printf("Data error:\nsrc[0x%X]: 0x%x\ndst[0x%X]: 0x%x\n",
                    rumboot_memcheck8_src_addr,
                    *rumboot_memcheck8_src_addr,
                    rumboot_memcheck8_dst_addr,
                    *rumboot_memcheck8_dst_addr);
            if(!data_ok)
                data_ok = (*rumboot_memcheck8_src_addr) - (*rumboot_memcheck8_dst_addr);
        }
    }
    return data_ok;
}

__attribute__((no_instrument_function)) int rumboot_memfill32(void *addr, size_t sz, int val, int incr)
{
	rumboot_platform_panic("NOT IMPLEMENTED");
    return 123;
}

#endif
