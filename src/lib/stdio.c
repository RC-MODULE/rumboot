#include <stdlib.h>
#include <stdint.h>
#include <rumboot/rumboot.h>
#include <rumboot/platform.h>

void rumboot_putstring(const char *string)
{
    while(*string)
        rumboot_platform_putchar(*string++);
}


void *rumboot_malloc(uint32_t length)
{
    #ifdef RUMBOOT_ONLY_STACK
        void *ret = (void *) rumboot_platform_runtime_info.current_heap_end;
        rumboot_platform_runtime_info.current_heap_end += length;
        if (rumboot_platform_runtime_info.current_heap_end > &rumboot_platform_heap_end) {
            rumboot_platform_panic("Heap memory exhausted!");
        }
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
