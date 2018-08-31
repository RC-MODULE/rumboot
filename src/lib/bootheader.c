#include <stdint.h>
#include <rumboot/boot.h>
#include <rumboot/platform.h>

extern void rumboot_main();
extern int main(int argc, const char *argv[]);

static const __attribute__((used)) __attribute__((section(".header")))
struct rumboot_bootheader hdr = {
	.magic		= RUMBOOT_HEADER_MAGIC,
	.version	= RUMBOOT_HEADER_VERSION,
	.num_cores	= RUMBOOT_PLATFORM_NUMCORES,
	.chip_id	= RUMBOOT_PLATFORM_CHIPID,
	.chip_rev	= RUMBOOT_PLATFORM_CHIPREV,
	.entry_point	= {
#ifndef RUMBOOT_NOINIT
#warning sfsfdvsdfv
		(uint32_t)&rumboot_main,
#else
		(uint32_t)&main,
#endif
	}
};
