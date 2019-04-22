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
#include <rumboot/hexdump.h>


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
	        rumboot_hexdump(buf, 512);
//        }

	bootsource_deinit(src, (void *) pdata);        

        
	return 0;
}
