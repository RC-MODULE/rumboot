#include <rumboot/boot.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <algo/crc32.h>
#include <platform/bootheader.h>
#include <rumboot/bitswapper.h>
#include <string.h>

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

/* TODO: Move to platform glue */
const char *rumboot_platform_get_cluster_name(int cluster_id) {
	if (cluster_id == 0) {
		return "PPC";
	} else {
		return "NMC";
	}
}

ssize_t rumboot_bootimage_check_header(struct rumboot_bootheader *hdr, void **dataptr)
{
	int swap = rumboot_bootimage_check_magic(hdr->magic);
	if (swap == -1)
		return -EBADMAGIC;

    dbg_boot(hdr->device, "--- Boot Image Header ---");
    dbg_boot(hdr->device, "Magic:            0x%x", rumboot_bootimage_header_item32(hdr->magic, swap));

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
		hdr->flags & RUMBOOT_FLAG_KILL     ? " KILL"    : ""
		);
	uint32_t cluster_id = rumboot_bootimage_header_item32(hdr->target_cpu_cluster, swap);
    dbg_boot(hdr->device, "Target CPU:       %d (%s)", cluster_id, rumboot_platform_get_cluster_name(cluster_id));	
    dbg_boot(hdr->device, "Data Length:      %d", rumboot_bootimage_header_item32(hdr->datalen, swap));
    dbg_boot(hdr->device, "Header CRC32:     0x%x", rumboot_bootimage_header_item32(hdr->header_crc32, swap));
    dbg_boot(hdr->device, "Data CRC32:       0x%x", rumboot_bootimage_header_item32(hdr->data_crc32, swap));
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

	if (checksum != rumboot_bootimage_header_item32(hdr->header_crc32, swap)) {
		dbg_boot(hdr->device, "Bad Header CRC32: expected: %x", checksum);
		return -EBADHDRCRC;
	}

	if (!rumboot_platform_check_entry_points(hdr))
		return -EBADENTRY;

	*dataptr = hdr->data;
	return rumboot_bootimage_header_item32(hdr->datalen, swap);
}

int32_t rumboot_bootimage_check_data(struct rumboot_bootheader *hdr)
{
	int swap = rumboot_bootimage_check_magic(hdr->magic);	
	if (rumboot_bootimage_header_item32(hdr->datalen, swap) == 0)
		return -EBADDATACRC;

	uint32_t checksum = crc32(0, hdr->data, rumboot_bootimage_header_item32(hdr->datalen, swap));
	if (checksum != rumboot_bootimage_header_item32(hdr->data_crc32, swap)) {
		dbg_boot(hdr->device, "Bad Data CRC32: expected: 0x%x",
			checksum
		);
        return -EBADDATACRC;
	}

	return 0;
}

int rumboot_bootimage_execute(struct rumboot_bootheader *hdr, const struct rumboot_bootsource *src)
{
	int swap = rumboot_bootimage_check_magic(hdr->magic);
	hdr->magic = 0x0; /* Wipe out magic */
	hdr->device = src; /* Set the src pointer */
	uint8_t flags = hdr->flags;

	int cluster_count = 0;
	const struct rumboot_secondary_cpu * cpus = rumboot_platform_get_secondary_cpus(&cluster_count);
	int cluster = rumboot_bootimage_header_item32(hdr->target_cpu_cluster, swap);
	const struct rumboot_secondary_cpu * cpu = NULL; 

	if (cluster > 0 && cluster <= cluster_count) {
		cpu = &cpus[cluster -1];
		if ((flags & RUMBOOT_FLAG_KILL) && (cpu->kill))
			cpu->kill(cpu); /* Kill any running tasks on this CPU */
	}

	/* The following is only for V3 images */
	if (flags & (RUMBOOT_FLAG_RELOCATE | RUMBOOT_FLAG_DECAPS)) {
		void *dest;
		/* If we only have a 'decaps' image, we'll have to do an in-place memmove/decompress */
		uint64_t dest_ptr = (flags & RUMBOOT_FLAG_RELOCATE) ? rumboot_bootimage_header_item64(hdr->relocation, swap) : (uintptr_t) hdr;
		if (sizeof(dest) == sizeof(uint32_t)) {
			dest = (void *) (uintptr_t) (dest_ptr & 0xffffffff);
		} else if (sizeof(dest) == sizeof(uint64_t)) {
			dest = (void*) (uintptr_t) dest_ptr;
		};

		void *src = hdr;
		size_t len = rumboot_bootimage_header_item32(hdr->datalen, swap) + sizeof(*hdr);

		if (flags & RUMBOOT_FLAG_DECAPS) {
			src = hdr->data;
			len -= sizeof(*hdr);
		}
		//dbg_boot(src, "Relocating data 0x%x -> 0x%x\n", src, dest);
		memmove(dest, src, len);
        #ifdef __PPC__
        /* FixMe: Use cross-platform barrier sync functions here */
        asm("msync");
        #endif
	}

	bailout:
		if (cluster == 0) {
    		return rumboot_platform_exec(hdr, swap);
		} else if (cluster <= cluster_count) {
			dbg_boot(src, "Starting %ssynchronous code execution on cluster %d (%s)", 
				(flags & RUMBOOT_FLAG_SYNC) ? "" : "a", 
				cluster,
				cpu->name);
			if (cpu->start) 
				cpu->start(cpu, hdr, swap);
			if (flags & RUMBOOT_FLAG_SYNC) {
				if (cpu->poll) {
					cpu->poll(cpu);
				} else { 
					dbg_boot(src, "WARN: %s doesn't support synchronous mode");
				}
			} else {
				return 0;
			}
		}
}

int rumboot_bootimage_execute_ep(void *ep)
{
	int (*ram_main)();
	ram_main = ep;
	return ram_main();
}
