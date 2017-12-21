#include <rumboot/bootheader.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <algo/crc32.h>


int32_t rumboot_bootimage_check_header(struct rumboot_bootheader *hdr, void **dataptr)
{
	rumboot_printf("MAGIC: %x\n", hdr->magic);

	if (hdr->magic != RUMBOOT_HEADER_MAGIC)
		return -1;

	rumboot_printf("NUMBER OF CORES: %x\n", hdr->num_cores);

	if (hdr->num_cores == 0)
		return -2;

	rumboot_printf("CRC32: %x\n", hdr->header_crc32);

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
	for (i = 0; i < hdr->num_cores; i++)
		if (crc32(0, hdr->entry_point, hdr->datalen) != hdr->data_crc32)
			return -2;

	return 0;
}

int32_t rumboot_bootimage_exec(struct rumboot_bootheader *hdr)
{
	rumboot_printf("STUB: Executing SPL image. Magic: 0x%x Entry: 0x%x\n",
		       hdr->magic, hdr->entry_point[0]);

	int (*ram_main)();
	ram_main = (void *)hdr->entry_point[0];
	rumboot_platform_perf("IM0 startup");
	ram_main();

	return 0;
}
