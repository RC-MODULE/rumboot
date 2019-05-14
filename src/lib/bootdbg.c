#include <rumboot/boot.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <algo/crc32.h>
#include <platform/bootheader.h>

void dbg_boot(const struct rumboot_bootsource * src, const char *fmt, ...)
{
	va_list args;
	rumboot_printf("boot: %s: \n", src ? src->name : "host");
	va_start(args, fmt);
	rumboot_vprintf(fmt, args);
	va_end(args);
}
