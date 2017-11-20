#include <stdint.h>
#include <rumboot/bootheader.h>
#include <rumboot/platform.h>

static const __attribute__((used)) __attribute__((section(".header")))
	struct rumboot_bootheader hdr = {
	.magic     = RUMBOOT_HEADER_MAGIC,
	.version   = RUMBOOT_HEADER_VERSION,
	.num_cores = RUMBOOT_PLATFORM_NUMCORES,
    .chip_id   = RUMBOOT_PLATFORM_CHIPID,
    .chip_rev  = RUMBOOT_PLATFORM_CHIPREV,
	.entry_point = {
		(uint32_t) &rumboot_entry_point,
	}
};

int32_t rumboot_bootimage_check_header(struct rumboot_bootheader *hdr, void **dataptr) {

	return 0;
}

int rumboot_bootimage_check_data(struct rumboot_bootheader *hdr) {

	return 0;
}

int rumboot_bootimage_exec(struct rumboot_bootheader *hdr) {

	return 0;
}
