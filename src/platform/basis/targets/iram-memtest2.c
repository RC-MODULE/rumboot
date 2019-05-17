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
    uintptr_t len = end - start;

    bool dose = 0;
    unsigned int iteration = 0;
    int errors = 0;
	rumboot_printf("Test area: %x -- %x [%x bytes]\n", start, end, end - start);    
    memtest_fill_pattern(start, len, 0xAAAAAAAA);
	while(1)
	{
		if(dose)
		{
        	memtest_fill_pattern(start, len, 0xAAAAAAAA);
            errors += memtest_check_pattern(0xAAAAAAAA, 0, start, len);
            rumboot_printf("Iteration: %d Errors: %u\n", iteration, errors);
        
            memtest_fill_pattern(start, len, 0x55555555);
            errors += memtest_check_pattern(0x55555555, 0, start, len);
            rumboot_printf("Iteration: %d Errors: %u\n", iteration, errors);
		} else {
            errors += memtest_check_pattern(0xAAAAAAAA, 1, start, len);
            rumboot_printf("Iteration: %d Errors: %u\n", iteration, errors);
		}
		iteration++;
	}
    /* Never gets here */    
	return 1;
}
