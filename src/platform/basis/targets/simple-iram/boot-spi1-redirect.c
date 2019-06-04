#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>

#include <rumboot/rumboot.h>
#include <stdlib.h>
#include <rumboot/boot.h>
#include <rumboot/bootsrc/eeprom.h>
#include <platform/devices.h>
#include <regs/regs_sdio.h>
#include <rumboot/io.h>
#include <string.h>

 struct rom_rumboot_bootsource {
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

int (*rom_bootsource_try_single)(const struct rumboot_bootsource *src, void *pdata, struct rumboot_bootheader *dst, size_t maxsize, size_t *offset);
const struct rumboot_bootsource *(*rom_rumboot_platform_get_bootsources)();
void *(*rom_rumboot_platform_get_spl_area)(size_t *size);


int boot_redirect()
{
	char pdata[128];
        char redirname[64];
        strcpy(redirname, "SPI1 (CS: internal) [Redirection hack]");
        size_t splsize;
        size_t offset = 0;

        rom_bootsource_try_single = (void *) 0x2ca8;
        rom_rumboot_platform_get_bootsources = (void *) 0xc08;        
        rom_rumboot_platform_get_spl_area = (void *) 0xbe8; 

        void *dst = rom_rumboot_platform_get_spl_area(&splsize);
	rumboot_printf("SPL from ROM: %x size %d bytes\n", dst, splsize);
        struct rom_rumboot_bootsource *romsrc = (void *) rom_rumboot_platform_get_bootsources();
        struct rom_rumboot_bootsource redir = romsrc[1];
        redir.name = redirname;
        redir.base = GSPI1_BASE;
        return rom_bootsource_try_single((void *)&redir, (void *) pdata, dst, splsize, &offset);       


}

int main()
{
        rumboot_platform_runtime_info->silent = 0;
        return boot_redirect();        
}
