#include <stdint.h>
#include <rumboot/platform.h>
#include <rumboot/boot.h>

#define BOOTHEADER_MAGIC__BOOT_IMAGE_VALID    0xbeefc0de
#define BOOTHEADER_MAGIC__HOST_IMAGE_VALID    0xdeadc0de
#define BOOTHEADER_MAGIC__RESUME_IMAGE_VALID  0xdeadbabe
#define BOOTHEADER_MAGIC__CORERESET_IMAGE_VALID  0xdefec8ed


struct legacy_bootheader
{
    uint32_t magic; /* Image type ID. Affects image processing.*/
    uint32_t length; /* imagedata length in bytes. Zero is valid value. */
    uint32_t entry0; /* Entry point address for primary CPU that executes ROM boot code.
                        Address must be within the range
                        from BOOTHEADER_ENTRY__MIN
                        to BOOTHEADER_ENTRY__MAX */

    uint32_t entry1; /* Entry point address for secondary CPU.
                        Address must be within the range
                        from BOOTHEADER_ENTRY__MIN
                        to BOOTHEADER_ENTRY__MAX.
                        0 if secondary CPU is not used. */

    uint32_t sum;  /* CRC32 of imagedata. If length == 0 or sum == 0 then CRC32 is not checked. */

    uint32_t hdrsum; /* Checksum of all previous fields. */

    uint8_t data[]; /* Image data. */
} __attribute__((packed));


static void rumboot_main_wrapper()
{
    int ret = rumboot_main();
    dbg_boot(NULL, "Back in rom, code %d", ret);
    while(1);;
}

static const __attribute__((used)) __attribute__((section(".header")))
struct legacy_bootheader hdr = {
	.magic		= BOOTHEADER_MAGIC__HOST_IMAGE_VALID,
	.entry0	= (uint32_t) &rumboot_main_wrapper,
};
