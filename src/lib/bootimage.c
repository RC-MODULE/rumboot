#include <rumboot/boot.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <algo/crc32.h>
#include <platform/bootheader.h>

/* TODO: Move to error.h */
static const char *errors[] =
{
	[0] = "Success",
	[EBADMAGIC] = "Bad Magic value",
	[EBADVERSION] = "Incompatible header version",
	[EBADHDRCRC] = "Header CRC32 mismatch",
	[EBADCHIPID] = "Incorrect ChipId",
	[EBADENTRY] = "Bad entry point(s)",
	[EBADDATACRC] = "Bad data CRC32",
	[ETOOBIG] = "Image too big",
    [EIO]   = "I/O Error while reading",
	[EMAXERROR] = "Unknown error",
};

const char *rumboot_strerror(int err)
{
	if (err < 0)
		err = -err;
	if (err > EMAXERROR)
		err = EMAXERROR;
	return errors[err];
}

ssize_t rumboot_bootimage_check_header(struct rumboot_bootheader *hdr, void **dataptr)
{
	if (hdr->magic != RUMBOOT_HEADER_MAGIC)
		return -EBADMAGIC;

    dbg_boot(hdr->device, "--- Boot header ---");
    dbg_boot(hdr->device, "Magic:            %x", hdr->magic);
    dbg_boot(hdr->device, "Header version:   %x", hdr->version);

	if (hdr->version != RUMBOOT_HEADER_VERSION)
		return -EBADVERSION;

    dbg_boot(hdr->device, "Chip Id:          %x", hdr->chip_id);
    dbg_boot(hdr->device, "Chip Revision:    %x", hdr->chip_rev);
    dbg_boot(hdr->device, "Data length:      %x", hdr->datalen);
    dbg_boot(hdr->device, "Header CRC32:     %x", hdr->header_crc32);
    dbg_boot(hdr->device, "Data CRC32:       %x", hdr->data_crc32);

	if (hdr->chip_id != RUMBOOT_PLATFORM_CHIPID)
		return -EBADCHIPID;

	/* Warn about possible incompatibilities */
	if (hdr->chip_rev != RUMBOOT_PLATFORM_CHIPREV) {
		dbg_boot(hdr->device, "WARNING: Chip Revision mismatch: Expecting %d, have %d",
		RUMBOOT_PLATFORM_CHIPREV, hdr->chip_rev)
	}

	int hdrcrclen = (int ) ((void *) &(hdr->header_crc32) - (void *)hdr);
	uint32_t checksum = crc32(0, hdr, hdrcrclen);

	if ( checksum != hdr->header_crc32) {
		dbg_boot(hdr->device, "Header CRC32 mismatch: expected: 0x%x calculated: %x",
			hdr->header_crc32, checksum);
		return -EBADHDRCRC;
	}

	if (!rumboot_platform_check_entry_points(hdr))
		return -EBADENTRY;

	*dataptr = hdr->data;
	return hdr->datalen;
}

int32_t rumboot_bootimage_check_data(struct rumboot_bootheader *hdr)
{
	if (hdr->datalen == 0)
		return -EBADDATACRC;

	uint32_t checksum = crc32(0, hdr->data, hdr->datalen);
	if (checksum != hdr->data_crc32) {
		dbg_boot(hdr->device, "Data CRC32 mismatch: expected: 0x%x calculated: %x",
			checksum, hdr->data_crc32
		);
	}

	return (checksum != hdr->data_crc32);
}

int rumboot_bootimage_execute_ep(void *ep)
{
	int (*ram_main)();
	ram_main = ep;
	return ram_main();
}
