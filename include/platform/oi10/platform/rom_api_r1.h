#ifndef __RUMBOOT_ROM_API
#define __RUMBOOT_ROM_API

 struct rumboot_config;
 struct rumboot_bootsource;
 struct rumboot_bootheader;
 size_t (*rom_bootsource_read)(const struct rumboot_bootsource *src, void *pdata, void *dest, size_t offset, size_t len)= (void *) 0xffff3b54;
 void (*rom_rumboot_platform_init_loader)(struct rumboot_config *conf)= (void *) 0xffff0e18;
 bool (*rom_bootsource_init)(const struct rumboot_bootsource *src, void *pdata)= (void *) 0xffff3a74;
void (*rom_rumboot_putstring)(const char *string)= (void *) 0xffff1a18;
int (*rom_rumboot_vprintf)(const char *fmt, va_list args)= (void *) 0xffff1d60;
 bool (*rom_rumboot_platform_check_entry_points)(struct rumboot_bootheader *hdr)= (void *) 0xffff10b4;
 int (*rom_bootsource_try_by_id)(int bootid, void *pdata, struct rumboot_bootheader *hdr, size_t maxsize)= (void *) 0xffff3ffc;
 void (*rom_rumboot_platform_enter_host_mode)(struct rumboot_config *conf)= (void *) 0xffff1044;
 void (*rom_bootsource_deinit)(const struct rumboot_bootsource *src, void *pdata)= (void *) 0xffff3c24;
void (*rom_rumboot_print_logo)()= (void *) 0xffff19d8;
 void (*rom_bootsource_try_chain)(void *pdata, struct rumboot_bootheader *hdr, size_t maxsize)= (void *) 0xffff3f74;
 int (*rom_rumboot_platform_selftest)(struct rumboot_config *conf)= (void *) 0xffff1564;
 int (*rom_bootsource_try_single)(const struct rumboot_bootsource *src, void *pdata, struct rumboot_bootheader *dst, size_t maxsize, size_t *offset)= (void *) 0xffff3f24;
 void (*rom_rumboot_platform_print_summary)(struct rumboot_config *conf)= (void *) 0xffff0f40;
int32_t (*rom_rumboot_bootimage_check_data)(struct rumboot_bootheader *hdr)= (void *) 0xffff45b4;
 void (*rom_rumboot_platform_read_config)(struct rumboot_config *conf)= (void *) 0xffff0ebc;
size_t (*rom_xmodem_get)( char *to, size_t maxszs)= (void *) 0xffff4d98;
ssize_t (*rom_rumboot_bootimage_check_header)(struct rumboot_bootheader *hdr, void **dataptr)= (void *) 0xffff4098;
 int (*rom_rumboot_platform_exec)(struct rumboot_bootheader *hdr, int swap)= (void *) 0xffff10d4;
 int (*rom_rumboot_bootimage_execute_ep)(void *ep)= (void *) 0xffff469c;
 int (*rom_bootsource_try_source_once)(const struct rumboot_bootsource *src, void *pdata, struct rumboot_bootheader *dst, size_t maxsize, size_t *offset)= (void *) 0xffff3dd0;

#endif
