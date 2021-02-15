#include <rumboot/boot.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <algo/crc32.h>
#include <platform/bootheader.h>
#include <rumboot/bitswapper.h>

/* HACK: Endian defines are missing on older gcc versions */
#if  !defined(__ORDER_LITTLE_ENDIAN__) && !defined(__ORDER_BIG_ENDIAN__)
#define __ORDER_LITTLE_ENDIAN__ 1234
#endif

/* TODO: Move to error.h */
static const char *errors[] =
{
	[0] = "Success",
	[EBADMAGIC] = "Bad Magic",
	[EBADVERSION] = "Bad header version",
	[EBADHDRCRC] = "Bad Header CRC32",
	[EBADCHIPID] = "Bad ChipId",
	[EBADENTRY] = "Bad entry point(s)",
	[EBADHEADER] = "Bad header",
	[EBADDATACRC] = "Bad data CRC32",
	[ETOOBIG] = "Image too big",
	[EBADSOURCE] = "Bad boot source",
    [EIO]   = "I/O Error",
	[EMAXERROR] = "Unknown",
};

const char *rumboot_strerror(int err)
{
	if (err < 0)
		err = -err;
	if (err > EMAXERROR)
		err = EMAXERROR;
	return errors[err];
}

int rumboot_bootimage_check_magic(uint32_t magic) 
{
	if (magic == RUMBOOT_HEADER_MAGIC) {
		return 0;
	}
#ifdef RUMBOOT_SUPPORTS_SPL_ENDIAN_SWAP
	if (__swap32(magic) == RUMBOOT_HEADER_MAGIC) {	
		return 1; 
	}
#endif	
	return -1;
}

#ifdef RUMBOOT_SUPPORTS_SPL_ENDIAN_SWAP
static uint32_t rumboot_bootimage_header_item(uint32_t v, int swap)
{
	return swap ? __swap32(v) : v;
}
#else 
#define rumboot_bootimage_header_item(v,s) (s ? v : v)
#endif

ssize_t rumboot_bootimage_check_header(struct rumboot_bootheader *hdr, void **dataptr)
{
	int swap = rumboot_bootimage_check_magic(hdr->magic);
	if (swap == -1)
		return -EBADMAGIC;

    dbg_boot(hdr->device, "--- Boot Image Header ---");
    dbg_boot(hdr->device, "Magic:            0x%x", rumboot_bootimage_header_item(hdr->magic, swap));

#ifdef RUMBOOT_SUPPORTS_SPL_ENDIAN_SWAP
	#if (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
	const char *endian = swap ? "big" : "little";
	#endif 

	#if (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
	const char *endian = swap ? "little" : "big";
	#endif 

	dbg_boot(hdr->device, "Byte order:       %s (%s)", endian, swap ? "reversed" : "native");
#endif

    dbg_boot(hdr->device, "Header version:   %d",   hdr->version);
	if ((hdr->version != RUMBOOT_HEADER_VERSION) && (hdr->version != 2))
		return -EBADVERSION;

    dbg_boot(hdr->device, "Chip Id:          %d", hdr->chip_id);
    dbg_boot(hdr->device, "Chip Revision:    %d", hdr->chip_rev);
    dbg_boot(hdr->device, "Image Flags:     %s%s%s%s%s%s%s%s", 
		hdr->flags & RUMBOOT_FLAG_COMPRESS ? " GZIP"    : "",
		hdr->flags & RUMBOOT_FLAG_ENCRYPT  ? " ENCRYPT" : "",
		hdr->flags & RUMBOOT_FLAG_SIGNED   ? " SIGNED"  : "",
		hdr->flags & RUMBOOT_FLAG_SMP      ? " SMP"     : "",
		hdr->flags & RUMBOOT_FLAG_DECAPS   ? " DECAPS"  : "",
		hdr->flags & RUMBOOT_FLAG_RELOCATE ? " RELOC"   : "",
		hdr->flags & RUMBOOT_FLAG_SYNC     ? " SYNC"    : "",
		hdr->flags & RUMBOOT_FLAG_RESERVED ? " RSVD"    : ""
		);
    dbg_boot(hdr->device, "Data Length:      %d", rumboot_bootimage_header_item(hdr->datalen, swap));
    dbg_boot(hdr->device, "Header CRC32:     0x%x", rumboot_bootimage_header_item(hdr->header_crc32, swap));
    dbg_boot(hdr->device, "Data CRC32:       0x%x", rumboot_bootimage_header_item(hdr->data_crc32, swap));
    dbg_boot(hdr->device, "---        ---        ---");
	if (hdr->chip_id != RUMBOOT_PLATFORM_CHIPID)
		return -EBADCHIPID;

	/* Warn about possible incompatibilities */
	if (hdr->chip_rev != RUMBOOT_PLATFORM_CHIPREV) {
		dbg_boot(hdr->device, "WARN: Bad Chip Revision: expected: %d",
		RUMBOOT_PLATFORM_CHIPREV);
	}

	int hdrcrclen = (int ) ((void *) &(hdr->header_crc32) - (void *)hdr);
	uint32_t checksum = crc32(0, hdr, hdrcrclen);

	if (checksum != rumboot_bootimage_header_item(hdr->header_crc32, swap)) {
		dbg_boot(hdr->device, "Bad Header CRC32: expected: %x", checksum);
		return -EBADHDRCRC;
	}

	if (!rumboot_platform_check_entry_points(hdr))
		return -EBADENTRY;

	*dataptr = hdr->data;
	return rumboot_bootimage_header_item(hdr->datalen, swap);
}

int32_t rumboot_bootimage_check_data(struct rumboot_bootheader *hdr)
{
	int swap = rumboot_bootimage_check_magic(hdr->magic);	
	if (rumboot_bootimage_header_item(hdr->datalen, swap) == 0)
		return -EBADDATACRC;

	uint32_t checksum = crc32(0, hdr->data, rumboot_bootimage_header_item(hdr->datalen, swap));
	if (checksum != rumboot_bootimage_header_item(hdr->data_crc32, swap)) {
		dbg_boot(hdr->device, "Bad Data CRC32: expected: 0x%x",
			checksum
		);
        return -EBADDATACRC;
	}

	return 0;
}

int rumboot_bootimage_execute(struct rumboot_bootheader *hdr, const struct rumboot_bootsource *src)
{
	/* TODO: Handle decapsulation, relocation and decompression here */
	int swap = rumboot_bootimage_check_magic(hdr->magic);
	hdr->magic = 0x0; /* Wipe out magic */
	hdr->device = src; /* Set the src pointer */
	if (hdr->version == 2) {
		hdr->entry_point == (hdr->entry_point >> 32); /* Properly handle 32-bit V2 images */
		goto bailout;
	}
	
	/* The following is only for V3 images */
	if (hdr->flags & RUMBOOT_FLAG_RELOCATE) {
		void *dest = (void *) hdr->relocation;
		void *src = hdr;
		size_t len = hdr->datalen + sizeof(*hdr);

		if (hdr->flags & RUMBOOT_FLAG_DECAPS) {
			src = hdr->data;
			len -= sizeof(*hdr);
		}
		dbg_boot(hdr->device, "Relocating image to 0x%x -> 0x%x", src, dest);
		memcpy(dest, src, len);
	}

	bailout:
    	return rumboot_platform_exec(hdr, swap);
}

int rumboot_bootimage_execute_ep(void *ep)
{
	int (*ram_main)();
	ram_main = ep;
	return ram_main();
}
