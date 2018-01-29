#include <stdlib.h>
#include <stdint.h>
#include <rumboot/rumboot.h>
#include <rumboot/platform.h>
#include <rumboot/printf.h>
#include <string.h>
 
void rumboot_putstring(const char *string)
{
#ifndef RUMBOOT_PRINTF_ACCEL
	while (*string)
		rumboot_platform_putchar(*string++);
#else
	rumboot_printf(string);
#endif
}


void *rumboot_malloc(uint32_t length)
{
#ifdef RUMBOOT_ONLY_STACK
	void *ret = (void *)rumboot_platform_runtime_info.current_heap_end;
	rumboot_platform_runtime_info.current_heap_end += length;
	if (rumboot_platform_runtime_info.current_heap_end > &rumboot_platform_heap_end)
		rumboot_platform_panic("Heap memory exhausted!");
	return ret;
#else
	return malloc(length);
#endif
}

void rumboot_free(void *ptr)
{
#ifndef RUMBOOT_ONLY_STACK
	free(ptr);
#endif
}

void *rumboot_memdup(const void *src, uint32_t len)
{
	char *ret = rumboot_malloc(len);
	if (!ret)
		rumboot_platform_panic("Failed to allocate %d bytes\n", len);
	memcpy(ret, src, len);
	return ret;
}
