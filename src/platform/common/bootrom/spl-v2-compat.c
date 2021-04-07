#include <stdint.h>
#include <rumboot/boot.h>
#include <rumboot/platform.h>

struct __attribute__((packed)) rumboot_bootheader_v2 {
    uint32_t magic;
    uint8_t  version;
    uint8_t  reserved;
    uint8_t  chip_id;
    uint8_t  chip_rev;
    uint32_t data_crc32;
    uint32_t datalen;
    uint32_t entry_point[10];
    uint32_t header_crc32;
    const struct rumboot_bootsource *device;
    char     data[];
};

extern char rumboot_self;
int main() {
    struct rumboot_bootheader_v2 *hdr = (void *) &rumboot_self;
    uint32_t c = (hdr->version == 2) ? 0 : 1;
    rumboot_platform_event_raise(EVENT_TERM, &c, 1);
    return (hdr->version == 2) ? 0 : 1;
}

static const __attribute__((used)) __attribute__((section(".header")))
struct rumboot_bootheader_v2 hdr __attribute__((packed)) = {
	.magic		= RUMBOOT_HEADER_MAGIC,
	.version	= 2,
	.chip_id	= RUMBOOT_PLATFORM_CHIPID,
	.chip_rev	= RUMBOOT_PLATFORM_CHIPREV,
	.entry_point	= {
		(uint32_t)&main,
	},
};
