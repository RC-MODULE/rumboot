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


#ifdef RUMBOOT_PRINTF_ACCEL

static __attribute__((no_instrument_function)) void deliver(enum rumboot_simulation_event event, uint32_t data)
{
   rumboot_platform_event_raise(event, &data, 1);
}

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

 __attribute__((no_instrument_function)) __attribute__((optimize("-O0"))) void do_memcmp(const void *ptr0,  ...)
{
    deliver(EVENT_MEMCMP, (uint32_t) __builtin_frame_address(0));
}

__attribute__((no_instrument_function)) int memcmp(const void *src, const void *dst, size_t sz)
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

__attribute__((no_instrument_function)) void *memmove(void *d, const void *s, size_t n)
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

 __attribute__((no_instrument_function)) __attribute__((optimize("-O0"))) void do_memfillseq8(void *ptr, ...)
{
    deliver(EVENT_MEMFILL8, (uint32_t)__builtin_frame_address(0));
}

__attribute__((no_instrument_function)) __attribute__((optimize("-O0"))) void* rumboot_memfill8(void *addr, size_t sz, uint8_t val, int8_t incr)
{
    do_memfillseq8(addr,sz,val,incr);
    return addr;
}

 __attribute__((no_instrument_function)) void rumboot_sim_get_realtime(uint32_t *rt)
{
    deliver(EVENT_REALTIME, (uint32_t) rt);
}

__attribute__((no_instrument_function)) __attribute__((optimize("-O0"))) void do_crc32(uint32_t crc, ...)
{
   deliver(EVENT_CRC32, (uint32_t) __builtin_frame_address(0));
}
uint32_t crc32(uint32_t crc, const void *buf, size_t size)
{
    do_crc32(crc, buf, size);
    volatile uint32_t *data; 
    int opcode = rumboot_platform_event_get(&data);
    if (opcode != EVENT_CRC32) {
        rumboot_platform_panic("SW<->TB Synchronization failed!\n");
    }
    return data[0];
}

__attribute__((no_instrument_function)) __attribute__((optimize("-O0"))) void do_crc8(uint32_t start_addr, ...)
{
    deliver(EVENT_CRC8, (uint32_t) __builtin_frame_address(0));
}
uint8_t crc8(uint32_t start_addr, uint32_t len)
{
    do_crc8(start_addr, len);
    volatile uint32_t *data;
    int opcode = rumboot_platform_event_get(&data);
    if (opcode != EVENT_CRC8) {
        rumboot_platform_panic("SW<->TB Synchronization failed!\n");
    }
    return data[0];
}

#else

__attribute__((no_instrument_function)) void rumboot_sim_get_realtime(uint32_t *rt)
{
   *rt = rumboot_platform_get_uptime();
}

__attribute__((no_instrument_function)) int rumboot_memcheck32(void *src, void *dst, size_t sz)
{
  int i;
  uint32_t *p_src = src;
  uint32_t *p_dst = dst;
  for (i=0; i<sz; i++) {
      if (p_src[i] != p_dst[i]) {
          rumboot_platform_panic("rumboot_memcheck32: data check failed");
      }
  }
  return 0;
}

__attribute__((no_instrument_function)) int memcmp(const void *src, const void *dst, size_t sz)
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

__attribute__((no_instrument_function))
void* rumboot_memfill8(void *addr, size_t sz, uint8_t val, int8_t incr)
{
    for(uint32_t current_addr = 0; current_addr < sz; ++current_addr, val+=incr)
        iowrite8(val,current_addr);
    return addr;
}

__attribute__((no_instrument_function)) int rumboot_memfill32(void *addr, size_t sz, int val, int incr)
{
    int i;
    uint32_t *dst = addr;
    for(i=0; i < sz; i++) {
      dst[i] = val; 
      val += incr;
    }
    return 0;
}

#endif
