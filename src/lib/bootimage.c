#include <rumboot/boot.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <algo/crc32.h>
#include <algo/heatshrink_decoder.h>
#include <platform/bootheader.h>
#include <rumboot/bitswapper.h>
#include <string.h>

/* HACK: Endian defines are missing on older gcc versions */
#if  !defined(__ORDER_LITTLE_ENDIAN__) && !defined(__ORDER_BIG_ENDIAN__)
#define __ORDER_LITTLE_ENDIAN__ 1234
#endif

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
	[ETOHOST] = "To Host Mode",
	[EBADCLUSTERID] = "Bad Target Cluster",
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

static const struct rumboot_secondary_cpu *get_secondary_cpu(int cluster)
{
	int cluster_count;
	const struct rumboot_secondary_cpu * cpus = rumboot_platform_get_secondary_cpus(&cluster_count);
	const struct rumboot_secondary_cpu *ret;
	if (cluster > 0 && cluster <= cluster_count) {
		ret = &cpus[cluster -1];
		return ret;
	}
	return NULL;
}

int rumboot_decompress_buffer(const struct rumboot_bootsource *src, const uint8_t *compressed_data, uint8_t *decompressed_data, size_t input_length, size_t output_size)
{
    uint32_t sunk = 0;
    uint32_t polled = 0;
    heatshrink_decoder hsd;
    heatshrink_decoder_reset(&hsd);
    while (sunk < input_length)
    {
        size_t count;
        int ret = heatshrink_decoder_sink(&hsd, (void *) &compressed_data[sunk], input_length - sunk, &count);
        if (ret < 0)
        {
            dbg_boot(src, "heatshrink_decoder_sink() error: %d", ret);
            return -EBADDATACRC;
        }
        sunk += count;
        if (sunk == input_length)
        {
            if (HSDR_FINISH_MORE != heatshrink_decoder_finish(&hsd))
            {
                dbg_boot(src, "heatshrink_decoder_finish() error: %d", ret);
                return -EBADDATACRC;
            }
        }
        HSD_poll_res pres;

        do
        {
            pres = heatshrink_decoder_poll(&hsd, &decompressed_data[polled],
                                           output_size - polled, &count);
            if (pres < 0)
            {
                dbg_boot(src, "heatshrink_decoder_poll() error: %d", ret);
                return -EBADDATACRC;
            }
            polled += count;
        } while (pres == HSDR_POLL_MORE);
        if (sunk == input_length)
        {
            HSD_finish_res fres = heatshrink_decoder_finish(&hsd);
            if (fres == HSDR_FINISH_DONE)
            {
                dbg_boot(src, "Decompressed image: %d to %d bytes", sunk, polled);
                break;
            }
        }
    }
    return polled;
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

	/* Require hdr->flags to be zero for old images */
	if (hdr->version == 2) {
		if (hdr->flags) {
    		dbg_boot(hdr->device, "ERR: Non-zero 'reserved' field in header");
			return -EBADHEADER;
		}		
	}

	if (hdr->version > 2) {
    	dbg_boot(hdr->device, "Image Flags:     %s%s%s%s%s%s%s%s", 
			hdr->flags & RUMBOOT_FLAG_COMPRESS ? " GZIP"    : "",
			hdr->flags & RUMBOOT_FLAG_DATA     ? " DATA"    : "",
			hdr->flags & RUMBOOT_FLAG_RESERVED ? " RSVD"    : "",
			hdr->flags & RUMBOOT_FLAG_SMP      ? " SMP"     : "",
			hdr->flags & RUMBOOT_FLAG_DECAPS   ? " DECAPS"  : "",
			hdr->flags & RUMBOOT_FLAG_RELOCATE ? " RELOC"   : "",
			hdr->flags & RUMBOOT_FLAG_SYNC     ? " SYNC"    : "",
			hdr->flags & RUMBOOT_FLAG_KILL     ? " KILL"    : ""
			);
		uint32_t cluster_id = rumboot_bootimage_header_item32(hdr->target_cpu_cluster, swap);
		const struct rumboot_secondary_cpu *cpu = get_secondary_cpu(cluster_id);
		if (cluster_id && !cpu) {
			dbg_boot(hdr->device, "ERR: Invalid target cluster_id specified: %d\n", cluster_id);
			return -EBADCLUSTERID;
		}
    	dbg_boot(hdr->device, "Target CPU:       %d (%s)", cluster_id, cpu ? cpu->name : "boot");	
	}
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
	size_t datasize = rumboot_bootimage_header_item32(hdr->datalen, swap);
	uint8_t flags = hdr->flags;
	size_t spl_size;

	hdr->magic = 0x0; /* Wipe out magic */
	hdr->device = src; /* Set the src pointer */

	rumboot_platform_get_spl_area(&spl_size);
	int cluster = (hdr->version == 3) ? rumboot_bootimage_header_item32(hdr->target_cpu_cluster, swap) : 0;
	const struct rumboot_secondary_cpu * cpu = get_secondary_cpu(cluster); 

	if (cpu && (flags & RUMBOOT_FLAG_KILL) && (cpu->kill)) {
		cpu->kill(cpu); /* Kill any running tasks on secondary CPU, if needed */
	}

	/* Now, let's calculate the relocation. This will only affect V3 images, since V2 have flags zeroed */
	if (flags & (RUMBOOT_FLAG_RELOCATE | RUMBOOT_FLAG_DECAPS | RUMBOOT_FLAG_COMPRESS)) {
		uint8_t *image_destination_base;
		uint8_t *image_source_base;
		size_t image_destination_offset = 0; 
		size_t image_source_offset = 0;
		size_t effective_size = datasize + sizeof(*hdr);

		/* If we only have a 'decaps' image, we'll have to do an in-place memmove/decompress */
		uint64_t dest_ptr = (flags & RUMBOOT_FLAG_RELOCATE) ? rumboot_bootimage_header_item64(hdr->relocation, swap) : (uintptr_t) hdr;
		if (sizeof(image_destination_base) == sizeof(uint32_t)) {
			image_destination_base = (void *) (uintptr_t) (dest_ptr & 0xffffffff);
		} else if (sizeof(image_destination_base) == sizeof(uint64_t)) {
			image_destination_base = (void*) (uintptr_t) dest_ptr;
		};

		image_source_base = hdr;
		/* First, let's make sure the header is where it's needed */
		if (flags & RUMBOOT_FLAG_DECAPS) {
			/* Decapsulation: Skip header. Same for compressed images */
			image_source_offset = sizeof(*hdr);
			image_destination_offset = 0;
			effective_size = datasize; 
		}

		if (flags & RUMBOOT_FLAG_COMPRESS) {
			image_source_offset = sizeof(*hdr);
			image_destination_offset = (flags & RUMBOOT_FLAG_DECAPS) ? 0 : sizeof(*hdr);
			effective_size = datasize; 
		}

		if (image_source_base != image_destination_base) { /* We relocation */
			if ((flags & RUMBOOT_FLAG_DECAPS) == 0) { /* No decapsulation, so move header */
				dbg_boot(src, "Moving header: %x -> %x", image_source_base, image_destination_base);
				memmove(image_destination_base, image_source_base, sizeof(*hdr));
				image_source_offset = sizeof(*hdr);
				image_destination_offset = sizeof(*hdr);
				effective_size = datasize; 
			}
		}
		
		/* TODO: Split this into a chain of distinct 'data handlers'. 
		 * Perhaps use a pair of fd's to pipe the data
		 */ 
		if (flags & RUMBOOT_FLAG_COMPRESS) {
			ssize_t ret; 
			if (flags & RUMBOOT_FLAG_RELOCATE) {
					/* If we do relocation, we do not impose any limits on destination size. */
					ret = rumboot_decompress_buffer(src, 
						image_source_base + image_source_offset, 
						image_destination_base + image_destination_offset, 
						effective_size, 128000);
			} else {
					dbg_boot(src, "Doing in-place decompression, watch out for side effects!");
					/* Move compressed data to the end of the SPL area */
					void *temp_buf = (uint8_t *) image_source_base + spl_size - datasize;
					dbg_boot(src, "Moving: %x -> %x\n", 
						image_source_base + image_source_offset, 
						temp_buf);
					memmove(temp_buf, image_source_base + image_source_offset, datasize);				
					/* Actual decompression */
					ret = rumboot_decompress_buffer(src, temp_buf, image_destination_base + image_destination_offset, effective_size, 128000);
			}
			if (ret < 0) {
				return ret;
			}
		} else if ( /* Do we need any memmoves ? */
				image_source_base + image_source_offset != 
				image_destination_base + image_destination_offset ) {
			dbg_boot(src, "Relocating data 0x%x -> 0x%x", 
				image_source_base + image_source_offset, 
				image_destination_base + image_destination_offset);
			memmove(
				image_destination_base + image_destination_offset, 
				image_source_base + image_source_offset, effective_size);
		}

        #ifdef __PPC__
        /* FixMe: Use cross-platform barrier sync functions here */
        asm("msync");
        #endif
	}

	if (flags & RUMBOOT_FLAG_DATA) {
		/* This image contains only user data to be loaded */
		return 0;
	}

	if (!cpu) {
    	return rumboot_platform_exec(hdr, swap);
	}

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

int rumboot_bootimage_execute_ep(void *ep)
{
	int (*ram_main)();
	ram_main = ep;
	return ram_main();
}
