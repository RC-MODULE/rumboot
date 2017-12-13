#include <rumboot/bootheader.h>
#include <rumboot/printf.h>
#include <algo/crc32.h>


int32_t rumboot_bootimage_check_header(struct rumboot_bootheader *hdr, void **dataptr)
{
	if (hdr->magic != RUMBOOT_HEADER_MAGIC)
		return -1;

	if (hdr->num_cores == 0)
		return -2;

	if (crc32(0, hdr, sizeof(*hdr) + sizeof(uint32_t) * hdr->num_cores) != hdr->header_crc32)
		return -3;

	return 0;
}

int32_t rumboot_bootimage_check_data(struct rumboot_bootheader *hdr)
{
	int i;

	if (hdr->datalen == 0)
		return -1;

	//Check CRC
	for (i = 0; i < hdr->num_cores; i++) {
		if (crc32(0, hdr->entry_point, hdr->datalen) != hdr->data_crc32)
			return -2;
	}

	return 0;
}

int32_t rumboot_bootimage_exec(struct rumboot_bootheader *hdr)
{
  while(1) {};

	return 0;
}
