#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rumboot/platform.h>
#include <devices/sdio_spi.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <platform/devices.h>
#include <devices/pl022_ssp.h>
#include <devices/gpio.h>
#include <devices/gpio.h>



void ssp_init( unsigned int gspibase)
{  

rumboot_printf("blabla_2\n");  
iowrite32(SSPCR0,gspibase + SSPCR0_ADDR);//8-bit data
rumboot_printf("blabla\n");
iowrite32(SSPCR1,gspibase + SSPCR1_ADDR ); //turn on controller, loop operation
iowrite32(SSPCPSR,gspibase + SSPCPSR_ADDR); //clock prescale (10MHz)
iowrite32(SSPIMSC,gspibase +  SSPIMSC_ADDR); //interrupt masks - mask all
iowrite32(SSPDMACR,gspibase + SSPDMACR_ADDR); //enable DMA

iowrite32(0x20,GPIO0_BASE+0x24);//SOURCE
iowrite32(0x20,GPIO0_BASE+0x08);//DIRECTION 
iowrite32(0x20,GPIO0_BASE+0x1c);//SET1
} ;
