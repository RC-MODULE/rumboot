/**
 * \defgroup test_hello_iram Hello World
 * \ingroup tests_basis
 *
 * Description
 * ===========
 *
 * Prints hello world via gtube and exits with 0 exit code
 *
 * Verification environment/plusargs
 * ==============================================
 * None
 *
 * CFLAGS
 * ======
 * None
 *
 * Expected output
 * ===============
 *
 *
 * How to compile
 * ==============
 *
 * \code{cmake}
 *    add_rumboot_target(
 *       CONFIGURATION IRAM
 *       FILES common/tools/drom-check.c
 *       NAME "hello-world"
 *   )
 * \endcode
 *
 */

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



#ifndef HEXDUMP_COLS
#define HEXDUMP_COLS 16
#endif
 
void hexdump(void *mem, unsigned int len)
{
        unsigned int i, j;
        
        for(i = 0; i < len + ((len % HEXDUMP_COLS) ? (HEXDUMP_COLS - len % HEXDUMP_COLS) : 0); i++)
        {
                /* print offset */
                if(i % HEXDUMP_COLS == 0)
                {
                        rumboot_printf("0x%06x: ", i);
                }
 
                /* print hex data */
                if(i < len)
                {
                        rumboot_printf("%02x ", 0xFF & ((char*)mem)[i]);
                }
                else /* end of block, just aligning for ASCII dump */
                {
                        rumboot_printf("   ");
                }
                
                /* print ASCII dump */
                if(i % HEXDUMP_COLS == (HEXDUMP_COLS - 1))
                {
                        for(j = i - (HEXDUMP_COLS - 1); j <= i; j++)
                        {
                                if(j >= len) /* end of block, not really printing */
                                {
                                        rumboot_platform_putchar(' ');
                                }
                                else if(isprint(((char*)mem)[j])) /* printable char */
                                {
                                        rumboot_platform_putchar(0xFF & ((char*)mem)[j]);        
                                }
                                else /* other char */
                                {
                                        rumboot_platform_putchar('.');
                                }
                        }
                        rumboot_platform_putchar('\n');
                }
        }
}





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
