#ifndef BOOTHEADER_H
#define BOOTHEADER_H

#define RUMBOOT_HEADER_VERSION 2
#define RUMBOOT_HEADER_MAGIC 0xb01dface

#include <platform/bootheader.h>

#include <stdint.h>
struct __attribute__((packed)) rumboot_bootheader {
    uint32_t magic;
    uint8_t  version;
    uint8_t  num_cores;
    uint8_t  chip_id;
    uint8_t  chip_rev;
    uint32_t data_crc32;
    uint32_t datalen;
    uint32_t entry_point[11];
    uint32_t header_crc32;
    char     data[];
};


/**
 * Validate a header pointed by hdr
 * @param  hdr     Bootimage header pointer
 * @param  dataptr Pointer to image data (for a valid header)
 * @return         Image data length
 */
int32_t rumboot_bootimage_check_header(struct rumboot_bootheader *hdr, void *dataptr);

/**
 * Validate data section of the bootable image
 * @param  hdr [description]
 * @return     [description]
 */
int rumboot_bootimage_check_data(struct rumboot_bootheader *hdr);


/**
 * Execute the loaded bootimage. The bootimage may return with
 * an exit code from the image's main().
 * @param [name] [description]
 * @return     exit code from image file's main()
 */
int rumboot_bootimage_exec(struct rumboot_bootheader *hdr);


#endif /* end of include guard: BOOTHEADER_H */
