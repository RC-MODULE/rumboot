#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>


static void check_malloc(void *start, int heap)
{
	int i;
	void *prev, *ptr=NULL;
	int prevlen;
	for (i=0; i<10; i++) {
		int len = i+7;
		prev = ptr;
		ptr = rumboot_malloc_from_heap(heap, len);
		if (prev && ((ptr - prev) < prevlen)) {
			printf("Allocation too small: Wanted %d got %d (prev %x)!\n", prevlen, (ptr - prev), prev);
			exit(1);
		}
		printf("malloc: ptr 0x%x %d at offset %d \n", ptr, len, ptr-start);
		prevlen = len;
	}
}

static void check_malloc_align(void *start, int heap)
{
	int i;
	void *prev =NULL;
	for (i=1; i<10; i++) {
		int len = i+7;
		void *ptr = rumboot_malloc_from_heap_aligned(heap, len, i);
		printf("malloc_align: ptr 0x%x %d at offset %d (align %d)\n", ptr, len, ptr-start, i);
		if (i && ((uint64_t) ptr % i)) {
			printf("alignment failed!\n");
			exit(1);
		}
	}
}

static void check_malloc_misalign(void *start, int heap)
{
	int i;
	void *prev =NULL;
	for (i=1; i<10; i++) {
		int len = i+7;
		void *ptr = rumboot_malloc_from_heap_misaligned(heap, len, 128, i);
		printf("malloc_misalign: ptr 0x%x %d at offset %d (misalign %d)\n", ptr, len, ptr-start, i);
		if (((uint64_t) ptr % 128) != i) {
			printf("(mis)alignment failed %lld %ld!\n", ((uint64_t )ptr % 128), i );
			exit(1);
		}
	}
}


int main()
{
	char *buf = malloc(8192);
	rumboot_malloc_register_heap("NONE", buf, &buf[8192]);
	rumboot_malloc_register_heap("ALIGN", buf, &buf[8192]);
	rumboot_malloc_register_heap("MISALIGN", buf, &buf[8192]);

	int id_start = rumboot_malloc_heap_by_name("NONE");
	check_malloc(buf, id_start);
	check_malloc_align(buf, id_start + 1);
	check_malloc_misalign(buf, id_start + 2);
	return 0;
}
