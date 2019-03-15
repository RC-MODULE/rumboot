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
    uint8_t  reserved;
    uint8_t  chip_id;
    uint8_t  chip_rev;
    uint32_t data_crc32;
    uint32_t datalen;
    uint32_t entry_point[10];
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


/*
 * @}
 */

#define dbg_boot(src, msg, ...) rumboot_printf("boot: %s: " msg "\n", src ? ((struct rumboot_bootsource *) src)->name : "host", ## __VA_ARGS__)

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
   uintptr_t base;
   uint64_t offset;
   uint32_t freq_khz;
   uint32_t slave_addr;
   const struct rumboot_bootmodule *plugin;
   bool (*enable)   (const struct rumboot_bootsource* src, void* pdata);
   void (*disable) (const struct rumboot_bootsource* src, void* pdata);
   void (*chipselect)(const struct rumboot_bootsource* src, void* pdata, int select);
 };

 bool bootsource_init(const struct rumboot_bootsource *src, void *pdata);
 size_t bootsource_read(const struct rumboot_bootsource *src, void *pdata, void *dest, size_t offset, size_t len);
 void bootsource_deinit(const struct rumboot_bootsource *src, void *pdata);


 /**
  * Attempts to boot from specified boot source once.
  * Used for unit-testing
  *
  * @param  bootid  Boot device id
  * @param  pdata   pdata for the bootsource
  * @param  hdr     Pointer to the area where the image will be loaded
  * @param  maxsize Maximum size available in hdr
  * @return         Returns the code from executed image or -EBADSOURCE if a bad id was specified
  */
 int bootsource_try_by_id(int bootid, void *pdata, struct rumboot_bootheader *hdr, size_t maxsize);
 /**
  * Attempts to boot from a specified device once. The image is read from *offset parameter, which in turn
  * is incremented by the amount of valid image data read.
  * @param  src     Bootsource to boot from
  * @param  pdata   pdata for the bootsource
  * @param  dst     Where the image is stored
  * @param  maxsize Maximum size of image and header
  * @param  offset  Pointer to size_t with device offset
  * @return         TODO:...
  */
 int bootsource_try_source_once(const struct rumboot_bootsource *src, void *pdata, struct rumboot_bootheader *dst, size_t maxsize, size_t *offset);
 int bootsource_try_single(const struct rumboot_bootsource *src, void *pdata, struct rumboot_bootheader *dst, size_t maxsize, size_t *offset);
 void bootsource_try_chain(void *pdata, struct rumboot_bootheader *hdr, size_t maxsize);


 /* Header check errors must go first */
#define EBADMAGIC      1
#define EBADVERSION    2
#define EBADHDRCRC     3
#define EBADCHIPID     4
#define EBADENTRY      5
#define EBADHEADER     6
#define EBADDATACRC    7
#define ETOOBIG        8
#define EBADSOURCE     9
#define EIO            10
#define ETOHOST        11
#define EMAXERROR      12

 struct rumboot_config {
         bool	hostmode;
         int	selftest;
         int	edcl;
         bool	legacyboot;
         int	baudrate;
 };

 const char *rumboot_strerror(int err);
 void rumboot_platform_read_config(struct rumboot_config *conf);
 void rumboot_platform_print_summary(struct rumboot_config *conf);
 int rumboot_platform_selftest(struct rumboot_config *conf);
 const struct rumboot_bootsource *rumboot_platform_get_bootsources();
 bool rumboot_platform_check_entry_points(struct rumboot_bootheader *hdr);
 int rumboot_platform_exec(struct rumboot_bootheader *hdr);
 void *rumboot_platform_get_spl_area(size_t *size);
 int rumboot_bootimage_execute_ep(void *ep);
 void rumboot_platform_enter_host_mode(struct rumboot_config *conf);
 void rumboot_platform_init_loader(struct rumboot_config *conf);
#endif /* end of include guard: BOOTHEADER_H */
