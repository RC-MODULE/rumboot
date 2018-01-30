#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>


extern char rumboot_data_start;
extern char rumboot_data_end;
extern char rumboot_rom_data;

static int a = -1;
int main()
{
	rumboot_print_logo();
	rumboot_printf("rumboot: spl start: 0x%x end: 0x%x\n",
		       &rumboot_platform_spl_start,
		       &rumboot_platform_spl_end);
	rumboot_printf("rumboot: bss start: 0x%x end: 0x%x\n",
		       &rumboot_platform_bss_start,
		       &rumboot_platform_bss_end);

	rumboot_printf("rumboot: .data at: 0x%x -- 0x%x from %x\n",
		   		       &rumboot_data_start,
		   		       &rumboot_data_end,
				   		&rumboot_rom_data);

	rumboot_printf("rumboot: a: 0x%x value 0x%x\n",
		       &a,
		       a);


	a++;
	/* Return 0, test passed */
	return a;
}
