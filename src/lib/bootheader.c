#include <stdint.h>
#include <rumboot/bootheader.h>

extern int main();
static const __attribute__((used)) __attribute__((section(".header")))
	struct rumboot_bootheader hdr = {
	.magic = RUMBOOT_HEADER_MAGIC,
	.version = RUMBOOT_HEADER_VERSION,
	.num_cores = 1,
	.entry_point = {
		(uint32_t) &main,
	}
};
