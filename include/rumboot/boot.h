#ifndef BOOTHEADER_H
#define BOOTHEADER_H

#include <rumboot/bitswapper.h>
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

#ifndef RUMBOOT_HEADER_VERSION
  #define RUMBOOT_HEADER_VERSION 3
#endif

#define RUMBOOT_HEADER_MAGIC 0xb01dface

#include <platform/bootheader.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>

struct rumboot_bootsource;

enum rumboot_header_flags {
    RUMBOOT_FLAG_COMPRESS = (1 << 0), /** Image data is compressed with heatshrink */
    RUMBOOT_FLAG_DATA     = (1 << 1), /** Image file contains only data to be loaded, it shouldn't be executed */
    RUMBOOT_FLAG_RESERVED = (1 << 2), /** Reserved for further usage */ 
    RUMBOOT_FLAG_SMP      = (1 << 3), /** SMP Image. If supported by target CPU cluster, all cores will start execution */
    RUMBOOT_FLAG_DECAPS   = (1 << 4), /** Remove header before executing and move data to the beginning */
    RUMBOOT_FLAG_RELOCATE = (1 << 5), /** Relocate image before execution to the address in relocation field */
    RUMBOOT_FLAG_SYNC     = (1 << 6), /** Wait for the image to finish before exiting */
    RUMBOOT_FLAG_KILL     = (1 << 7), /** Issue a reset to the target CPU before operating */
};

struct rumboot_bootheader {
    uint32_t magic;
    uint8_t  version;
    uint8_t  flags;
    uint8_t  chip_id;
    uint8_t  chip_rev;
    uint32_t data_crc32;
    uint32_t datalen;
    /* We use two 32-bit numbers, since gcc can't assign statically a 64-bit one */ 
    uint32_t  entry_point[2];
    uint64_t  relocation;
    uint8_t   target_cpu_cluster;

    uint8_t   encryption_slot;
    uint8_t   certificate_slot;
    uint8_t   priority;
    uint32_t  bootargs[5];
    uint32_t  header_crc32;
    uint32_t  device;
    char     data[];
} __attribute__((packed));




/**
 * @brief Check 
 * 
 * @param magic 
 * @return int 
 */
int rumboot_bootimage_check_magic(uint32_t magic);

/**
 * Validate a header pointed by hdr
 * @param  hdr     Bootimage header pointer
 * @param  dataptr Pointer to image data (for a valid header)
 * @return         Image data length
 */
ssize_t rumboot_bootimage_check_header(const struct rumboot_bootsource *src, struct rumboot_bootheader *hdr, void **dataptr);

/**
 * Validate data section of the bootable image
 * @param  hdr [description]
 * @return     [description]
 */
int32_t rumboot_bootimage_check_data(const struct rumboot_bootsource *src, struct rumboot_bootheader *hdr);


/*
 * @}
 */

void dbg_boot(const struct rumboot_bootsource * src, const char *fmt, ...);

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
   uint32_t base_freq_khz;
   uint32_t iface_freq_khz;
   uint32_t slave_addr;
   const struct rumboot_bootmodule *plugin;
   bool (*enable)   (const struct rumboot_bootsource* src, void* pdata);
   void (*disable) (const struct rumboot_bootsource* src, void* pdata);
   void (*chipselect)(const struct rumboot_bootsource* src, void* pdata, int select);
 };

 bool bootsource_init(const struct rumboot_bootsource *src, void *pdata);
 size_t bootsource_read(const struct rumboot_bootsource *src, void *pdata, void *dest, size_t offset, size_t len);
 void bootsource_deinit(const struct rumboot_bootsource *src, void *pdata);
 int rumboot_bootimage_execute(struct rumboot_bootheader *hdr, const struct rumboot_bootsource *src);

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
#define EBADCLUSTERID  12
#define EMAXERROR      13

 struct rumboot_config {
         bool	hostmode;
         bool secure_boot;
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
 int rumboot_platform_exec(struct rumboot_bootheader *hdr, int swap);
 void *rumboot_platform_get_spl_area(size_t *size);
 void rumboot_platform_enter_host_mode(struct rumboot_config *conf);
 void rumboot_platform_init_loader(struct rumboot_config *conf);


#define rumboot_bootimage_header_item8(v,s) v
#ifdef RUMBOOT_SUPPORTS_SPL_ENDIAN_SWAP
static inline uint32_t rumboot_bootimage_header_item32(uint32_t v, int swap)
{
	return swap ? __swap32(v) : v;
}
#else 
#define rumboot_bootimage_header_item32(v,s) (s ? v : v)
#endif

#ifdef RUMBOOT_SUPPORTS_SPL_ENDIAN_SWAP
static inline uint64_t rumboot_bootimage_header_item64(uint64_t v, int swap)
{
	return swap ? __swap64(v) : v;
}
#else 
#define rumboot_bootimage_header_item64(v,s) (s ? v : v)
#endif


struct rumboot_cpu_cluster {
  const char *name;
  uintptr_t base;
  void (*kill)(const struct rumboot_cpu_cluster *cpu);
  int  (*poll)(const struct rumboot_cpu_cluster *cpu);
  int (*start)(const struct rumboot_cpu_cluster *cpu, struct rumboot_bootheader *hdr, void *data, int swap);
};

const struct rumboot_cpu_cluster *rumboot_platform_get_cpus(int *cpu_count);
int __attribute__((deprecated)) rumboot_bootimage_execute_ep(void *ep);
int rumboot_bootimage_jump_to_ep_with_args(const struct rumboot_cpu_cluster *cpu,  struct rumboot_bootheader *hdr, void *data, int swap);
int rumboot_platform_decrypt_buffer(const struct rumboot_bootsource *src, int encryption_slot, void *data, size_t datasize);
int rumboot_platform_verify_signature(const struct rumboot_bootsource *src, int certificate_slot, void *data, size_t datasize);

#endif /* end of include guard: BOOTHEADER_H */


