#include <stdint.h>
#include <rumboot/boot.h>
#include <rumboot/platform.h>

extern int main(int argc, const char *argv[]);

static const __attribute__((used)) __attribute__((section(".header")))
struct rumboot_bootheader hdr __attribute__((packed)) = {
	.magic		= RUMBOOT_HEADER_MAGIC,
	.version	= RUMBOOT_HEADER_VERSION,
	.chip_id	= RUMBOOT_PLATFORM_CHIPID,
	.chip_rev	= RUMBOOT_PLATFORM_CHIPREV,
	.entry_point	= {
#if defined(RUMBOOT_NOINIT)
		(uint32_t)&main,
#elif defined(RUMBOOT_NOENTRY)
		0x0,
#else
		(uint32_t)&rumboot_main,
#endif
	}
};
