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

static const struct rumboot_bootsource src[] = {
        {
                .name = "I2C0 (EEPROM @ 0x50)",
                .base = I2C0_BASE,
                .slave_addr = 0x50,
                .freq_khz = 100000,
                .plugin = &g_bootmodule_eeprom,
        },
};

int main()
{
        rumboot_printf("Hello\n");

	char pdata[128];
        char buf[512];
	bootsource_init(src, (void *) pdata);
//        while(1) {
	        int ret = bootsource_read(src, (void *) pdata, buf, 0, 512);
	        rumboot_printf("Read %d bytes\n", ret);
	        hexdump(buf, 512);
//        }

	bootsource_deinit(src, (void *) pdata);        

        
	return 0;
}
