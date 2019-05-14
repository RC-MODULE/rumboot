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
                .base_freq_khz = 100000,
                .iface_freq_khz = 400,
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
