#ifndef BOOTHEADER_H
#define BOOTHEADER_H

/**
 *
 * \defgroup bootheader Boot image handling
 * \ingroup libraries
 *
 * These functions implement checking and executing boot images
 * Platforms should implement their own bootheader.h with
 *
 * RUMBOOT_PLATFORM_CHIPID
 * RUMBOOT_PLATFORM_CHIPREV
 * RUMBOOT_PLATFORM_NUMCORES
 *
 * \code{.c}
 *
 * \endcode
 *
 * \addtogroup bootheader
 * @{
 */

#define RUMBOOT_HEADER_VERSION 2
#define RUMBOOT_HEADER_MAGIC 0xb01dface

#include <platform/bootheader.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>

struct rumboot_bootsource;

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
    const struct rumboot_bootsource *device;
    char     data[];
};

/**
 * Validate a header pointed by hdr
 * @param  hdr     Bootimage header pointer
 * @param  dataptr Pointer to image data (for a valid header)
 * @return         Image data length
 */
ssize_t rumboot_bootimage_check_header(struct rumboot_bootheader *hdr, void **dataptr);

/**
 * Validate data section of the bootable image
 * @param  hdr [description]
 * @return     [description]
 */
int32_t rumboot_bootimage_check_data(struct rumboot_bootheader *hdr);


/**
 * Execute the loaded bootimage. The bootimage may return with
 * an exit code from the image's main().
 * @param [name] [description]
 * @return     exit code from image file's main()
 */
int32_t rumboot_bootimage_exec(struct rumboot_bootheader *hdr);

/*
 * @}
 */

#define dbg_boot(src, msg, ...) rumboot_printf("boot: %s: " msg "\n", src ? src->name : "host", ## __VA_ARGS__);

 #define DBG_ASSERT(statement, message) \
   { \
     if(statement) {\
       rumboot_printf("%s\n", message); \
       while(1); \
     } \
   }


 struct rumboot_bootmodule {
    int privdatalen;
    size_t align;
    bool (*init)      (const struct rumboot_bootsource* src, void* pdata);
    void (*deinit)    (const struct rumboot_bootsource* src, void* pdata);
    size_t  (*read)   (const struct rumboot_bootsource* src, void* pdata, void* to, size_t offset, size_t length);
 };

 struct rumboot_bootsource {
   const char *name;
   uint32_t base;
   uint32_t offset;
   uint32_t freq_khz;
   const struct rumboot_bootmodule *plugin;
   bool (*enable)   (const struct rumboot_bootsource* src, void* pdata);
   void (*disable) (const struct rumboot_bootsource* src, void* pdata);
   void (*chipselect)(const struct rumboot_bootsource* src, void* pdata, int select);
 };

 bool bootsource_try_single(const struct rumboot_bootsource *src, void* pdata);
 bool bootsource_try_chain(const struct rumboot_bootsource *src, void* pdata);

#define EBADMAGIC      1
#define EBADVERSION    2
#define EBADHDRCRC     3
#define EBADCHIPID     4
#define EBADENTRY      5
#define EBADNUMCORES   6
#define EBADDATACRC    7
#define EMAXERROR      8

 struct rumboot_config {
         bool	hostmode;
         bool	selftest;
         bool   has_edcl;
         bool	edcl;
         bool	legacyboot;
         int	baudrate;
 };

 const char *rumboot_strerror(int err);
 void rumboot_platform_read_config(struct rumboot_config *conf);
 void rumboot_platform_selftest(struct rumboot_config *conf);
 const struct rumboot_bootsource *rumboot_platform_get_bootsources();
 bool rumboot_platform_check_entry_points(struct rumboot_bootheader *hdr);
 void rumboot_platform_exec(struct rumboot_bootheader *hdr);
#endif /* end of include guard: BOOTHEADER_H */
