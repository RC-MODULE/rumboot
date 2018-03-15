#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>

int main()
{
	rumboot_print_logo();
	rumboot_printf("Dumping available heaps\n");
	int i;
	for (i=0; i < rumboot_malloc_num_heaps(); i++) {
		rumboot_printf("%d. %s start 0x%x -- %x \n",
		i, rumboot_malloc_heap_name(i),
		rumboot_platform_runtime_info.heaps[i].start,
		rumboot_platform_runtime_info.heaps[i].end
		);
	}

	return 0;
}
