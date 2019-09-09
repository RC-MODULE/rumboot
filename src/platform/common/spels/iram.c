#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <platform/devices.h>
#include <rumboot/rumboot.h>
#include <rumboot/memtest.h>
#include <stdlib.h>


/* Defaults: Start at the end of spl area, end at the stack position */
#ifndef START
#define START (&rumboot_platform_spl_end)
#endif

#ifndef END
#define END (__builtin_frame_address(0) - 1024);
#endif

int main()
{
	rumboot_platform_runtime_info->silent = 0;

	uintptr_t start = (uintptr_t) START;
	start = start + (4 - (start % 4));

	uintptr_t end = (uintptr_t) END;
	end = end - (end % 8);

	rumboot_printf("Test area: %x -- %x [%x bytes]\n", start, end, end - start);
	uint64_t errors = memtest(start, end - start);
	return errors ? 1 : 0;
}
