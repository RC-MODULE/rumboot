#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>

#include <rumboot/rumboot.h>
#include <rumboot/memtest.h>
#include <stdlib.h>

int main()
{
	rumboot_platform_runtime_info->silent = 0;
	uintptr_t start = &rumboot_platform_spl_end;
	start = start + (4 - (start % 4));
	uintptr_t end = __builtin_frame_address(0) - 1024;
	end = end - (end % 8);
	rumboot_printf("Test area: %x -- %x [%x bytes]\n", start, end, end - start);
	uint64_t errors = memtest(start, end - start);
	return errors ? 1 : 0;
}
