#ifndef BOOTHEADER_H
#define BOOTHEADER_H

#define RUMBOOT_HEADER_VERSION 2
#define RUMBOOT_HEADER_MAGIC 0xb01dface

struct __attribute__((packed)) rumboot_bootheader {
    uint32_t magic;
    uint8_t  version;
    uint8_t  num_cores;
    uint8_t  reserved[2];
    uint32_t header_crc32;
    uint32_t data_crc32;
    uint32_t datalen;
    uint32_t entry_point[];
};


/**
 * Validate a header pointed by hdr
 * @param  hdr Pointer to header data
 * @return     Size of data section or a negative error code
 */
int32_t rumboot_bootimage_check_header(struct rumboot_bootheader *hdr);

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
