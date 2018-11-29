#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <rumboot/bootsrc/sdio.h>
#include <rumboot/bootsrc/spiflash.h>
#include <platform/devices.h>
#include <rumboot/irq.h>
#include <rumboot/timer.h>


//setenv rumboot 'tftpboot 0x2080000 hello.bin; go 0x2080000'
//setenv rumboot 'icache off; dcache off; tftpboot 0x00000 hello.bin; go 0x00000'


int main()
{
	rumboot_print_logo();
	rumboot_printf("rumboot: Yarr! I need moar rum!\n\n");
	return 0;
}
