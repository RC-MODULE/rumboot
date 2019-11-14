#include <stdint.h>
#include <rumboot/platform.h>

struct nm6408_head {
    uint32_t magic;
    uint32_t length;
};

struct nm6408_tail {
    uint32_t crc;
};


static const __attribute__((used)) __attribute__((section(".header")))
struct nm6408_head the_hdr = {
	.magic		= 0x12345678,
};


static const __attribute__((used)) __attribute__((section(".trailer")))
struct nm6408_tail the_tlr = {
	.crc		= 0xb00bc0de,
};
