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

int (*rom_bootsource_try_single)(const struct rumboot_bootsource *src, void *pdata, struct rumboot_bootheader *dst, size_t maxsize, size_t *offset);
const struct rumboot_bootsource *(*rom_rumboot_platform_get_bootsources)();
void *(*rom_rumboot_platform_get_spl_area)(size_t *size);


int boot_redirect()
{
	char pdata[128];
        char buf[512];
        char redirname[64];
        strcpy(redirname, "SPI1 (CS: internal) [Redirection hack]");
        size_t splsize;
        size_t offset = 0;



        rom_bootsource_try_single = 0x2ca8;
        rom_rumboot_platform_get_bootsources = 0xc08;        
        rom_rumboot_platform_get_spl_area = 0xbe8;

        void *dst = rom_rumboot_platform_get_spl_area(&splsize);
	rumboot_printf("SPL from ROM: %x size %d bytes\n", dst, splsize);
        struct rumboot_bootsource *romsrc = rom_rumboot_platform_get_bootsources();
        struct rumboot_bootsource redir = romsrc[1];
        redir.name = redirname;
        redir.base = GSPI1_BASE;
        return rom_bootsource_try_single(&redir, (void *) pdata, dst, splsize, &offset);       

}

int main()
{
        return boot_redirect();        
}
