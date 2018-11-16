#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rumboot/platform.h>
#include <devices/sdio_spi.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <platform/devices.h>
#include <regs/pl022_ssp.h>
#include <regs/regs_gpio_rcm.h>
#include <regs/regs_gpio_rcm.h>



void ssp_init( unsigned int base)
{

rumboot_printf("blabla_2\n");
iowrite32(SSPCR0,base + SSPCR0_ADDR);//8-bit data
rumboot_printf("blabla\n");
iowrite32(SSPCR1,base + SSPCR1_ADDR ); //turn on controller, loop operation
iowrite32(SSPCPSR,base + SSPCPSR_ADDR); //clock prescale (10MHz)
iowrite32(SSPIMSC,base +  SSPIMSC_ADDR); //interrupt masks - mask all
iowrite32(SSPDMACR,base + SSPDMACR_ADDR); //enable DMA

/*iowrite32(0x20,GPIO0_BASE+0x24);//SOURCE
iowrite32(0x20,GPIO0_BASE+0x08);//DIRECTION
iowrite32(0x20,GPIO0_BASE+0x1c);//SET1*/
} ;
