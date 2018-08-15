#include <rumboot/boot.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <algo/crc32.h>


ssize_t rumboot_bootimage_check_header(struct rumboot_bootheader *hdr, void **dataptr)
{
	rumboot_printf("MAGIC: %x\n", hdr->magic);

	if (hdr->magic != RUMBOOT_HEADER_MAGIC)
		return -1;

	rumboot_printf("NUMBER OF CORES: %x\n", hdr->num_cores);

	if (hdr->num_cores == 0)
		return -2;

	int hdrcrclen = (int ) ((void *) &(hdr->header_crc32) - (void *)hdr);
	uint32_t checksum = crc32(0, hdr, hdrcrclen);
	dbg_boot(hdr->device, "Header crc32: expected: 0x%x calculated: %x",
		hdr->header_crc32, checksum);

	rumboot_printf("CRC32: %x len %d/%d\n", hdr->header_crc32, hdrcrclen, sizeof(*hdr));

	if ( checksum != hdr->header_crc32)
		return -3;

	*dataptr = hdr->data;
	return hdr->datalen;
}

int32_t rumboot_bootimage_check_data(struct rumboot_bootheader *hdr)
{
	if (hdr->datalen == 0)
		return -1;

	uint32_t checksum = crc32(0, hdr->data, hdr->datalen);
	dbg_boot(hdr->device, "Data CRC32: expected: 0x%x calculated: %x",
		checksum, hdr->data_crc32
	);

	if (checksum != hdr->data_crc32)
			return -2;

	return 0;
}

int32_t rumboot_bootimage_exec(struct rumboot_bootheader *hdr)
{
//	int32_t ret;

	dbg_boot(hdr->device, "Executing image. Entry point: 0x%x", hdr->entry_point[0]);

	/* TODO: Start secondary cores */

	int (*ram_main)();
	ram_main = (void *)hdr->entry_point[0];
	ram_main();

	//It must not be!
	return -1;
}
