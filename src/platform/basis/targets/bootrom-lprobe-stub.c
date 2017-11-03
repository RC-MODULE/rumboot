#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/bootheader.h>

int main()
{
	rumboot_print_logo();
	rumboot_printf("STUB: Entering lprobe event handling loop\n");

	//rumboot_platform_event_raise()
	while(1) {
        uint32_t *data;
		enum rumboot_simulation_event evt = rumboot_platform_event_get(&data);
		rumboot_printf("Inbound event %d arg 0x%x\n", evt, data[0]);
		rumboot_platform_event_clear();
	}
}
