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


void *rumboot_malloc_from_heap_misaligned(int heap_id, size_t length, int align, int misalign)
{
	struct rumboot_heap *hp = &rumboot_platform_runtime_info.heaps[heap_id];
	void *ret;

	size_t pad = align ? (((uint32_t)hp->pos) % align) : 0;

	if (pad) {
		hp->pos += (align - pad);
	}

	hp->pos += misalign;

	ret = hp->pos;
	hp->pos += length;
	if (hp->pos >= hp->end) {
		rumboot_platform_panic("Heap %s: out of memory!", hp->name);
	}

	return ret;
}

int rumboot_malloc_heap_by_name(const char *name)
{
	int i;

	for (i = 0; i < rumboot_platform_runtime_info.num_heaps; i++) {
		struct rumboot_heap *hp;
		hp = &rumboot_platform_runtime_info.heaps[i];
		if (strcmp(hp->name, name) == 0) {
			return i;
		}
	}
	return -1;
}

const char *rumboot_malloc_heap_name(int heap_id)
{
	if (heap_id < rumboot_platform_runtime_info.num_heaps) {
		return rumboot_platform_runtime_info.heaps[heap_id].name;
	}
	return NULL;
}

int rumboot_malloc_num_heaps()
{
	return rumboot_platform_runtime_info.num_heaps;
}

uint32_t rumboot_malloc_heap_length(int heap_id)
{
    if (heap_id < rumboot_platform_runtime_info.num_heaps) {
        return rumboot_platform_runtime_info.heaps[heap_id].end -
            rumboot_platform_runtime_info.heaps[heap_id].start;
    }
    return 0;
}

void *rumboot_malloc_from_heap(int heap_id, size_t length)
{
	return rumboot_malloc_from_heap_misaligned(heap_id, length, 0x0, 0x0);
}

void *rumboot_malloc_from_heap_aligned(int heap_id, size_t sz, int align)
{
	return rumboot_malloc_from_heap_misaligned(heap_id, sz, align, 0x0);
}


int rumboot_malloc_register_heap(const char *name, void *heap_start, void *heap_end)
{
	int i;

	/* Ignore empty heaps */
	if (heap_start == heap_end) {
		return -1;
	}

	for (i = 0; i < RUMBOOT_PLATFORM_NUM_HEAPS; i++) {
		struct rumboot_heap *hp;
		hp = &rumboot_platform_runtime_info.heaps[i];
		if (hp->start) {
			continue;
		}
		hp->name = name;
		hp->start = heap_start;
		hp->pos = heap_start;
		hp->end = heap_end;
		rumboot_platform_runtime_info.num_heaps++;
		break;
	}

	if (i == RUMBOOT_PLATFORM_NUM_HEAPS) {
		rumboot_platform_panic("Failed to register heap %s, please increase RUMBOOT_PLATFORM_NUM_HEAPS (current - %d)\n", name, RUMBOOT_PLATFORM_NUM_HEAPS);
	}
	return i;
}

void *rumboot_malloc(uint32_t length)
{
	return rumboot_malloc_from_heap(0, length);
}

void rumboot_free(void *ptr)
{
	/* No-op */
}

void *rumboot_memdup(const void *src, uint32_t len)
{
	char *ret = rumboot_malloc(len);

	if (!ret) {
		rumboot_platform_panic("Failed to allocate %d bytes\n", len);
	}
	memcpy(ret, src, len);
	return ret;
}
