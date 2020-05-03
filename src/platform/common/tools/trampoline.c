#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/boot.h>
#include <platform/devices.h>

#include <platform/rom_api_r1.h>
extern char rumboot_self;
int main()
{
	char pdata[128];

	rom_rumboot_putstring("rumboot: micro-trampoline to IM0\n");
	struct rumboot_bootheader *hdr = (struct rumboot_bootheader *) &(rumboot_self);
	if (!hdr->device) {
		rom_rumboot_putstring("rumboot: micro-trampoline can't run from host");
		return 1;
	}
	rom_bootsource_init(hdr->device, pdata);
	struct rumboot_bootheader *dst = (void *) IM0_BASE;
	size_t offset = 1 * 1024;
	rom_bootsource_try_single(hdr->device, pdata, dst, 64 * 1024, &offset);
	rom_bootsource_deinit(hdr->device, pdata);
	
	return 0;
}
