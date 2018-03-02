#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rumboot/platform.h>
#include <devices/sdio_spi.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <platform/devices.h>
#include <devices/go_dmac.h>
#include <devices/gpio.h>

void go_dmac(unsigned int gspibase,unsigned int* send, unsigned int* rcv, unsigned int amount) //
{   



unsigned int SSPSR;
unsigned int sup;
unsigned int read_data;
//unsigned int x;  

iowrite32(0xDF,GPIO0_BASE+0x20);//SET0
//iowrite32(0x20,GPIO0_BASE+0x08);//DIRECTION 

if (amount==1)  {  
 iowrite32(SSPDMACR_PROGRAM,(gspibase + SSPDMACR_ADDR)); 
 sup=ioread32((int)send);
 iowrite32(sup,(gspibase + SSPDR_ADDR));
 SSPSR=ioread32(gspibase + SSPSR_ADDR);
  while (SSPSR!=0x7)
  { SSPSR=ioread32(gspibase + SSPSR_ADDR);}
  read_data=ioread32(gspibase + SSPDR_ADDR);
  iowrite32(SSPDMACR,gspibase + SSPDMACR_ADDR); 
                 
             }
else         { 

rumboot_printf("GPIIIOO\n");
 sup=(int)send;
iowrite32(sup,gspibase + SPI_DMARSTART);      
  sup=(int)send + amount-1;
iowrite32(sup,gspibase + SPI_DMAREND);
iowrite32(0x80000000|0x1FFFFFE0 ,(gspibase + SPI_DMARCNTRL));//DMAR set descriptor 
iowrite32(0x1,(gspibase + SPI_AXIR_BUFTYPE));//set read axi buffer tipe - single (not circled)
  sup=(int)rcv;
iowrite32(sup,gspibase + SPI_DMAWSTART);
  sup=(int)rcv + amount-1;      
iowrite32(sup,gspibase + SPI_DMAWEND);
iowrite32( 0x80000000|0x1FFFFFE0,(gspibase + SPI_DMAWCNTRL ));//DMAW set descriptor 
iowrite32( SPI_AXI_PARAMS,(gspibase + SPI_AXI_PARAMS_ADDR));//set axi params AWLEN=ARLEN=F, all LSB (0 - LSB, 1 - MSB) ??
iowrite32( SPI_AXIR_BUFENA,(gspibase + SPI_AXIR_BUFENA_ADDR));//start AXIR DMA
  read_data=ioread32(gspibase +  SPI_STATUS);
  rumboot_printf("example_ok\n");
  while (!(read_data&0x008))
    {read_data=ioread32(gspibase + SPI_STATUS);}
  while (!(read_data&0x004))
    {read_data=ioread32(gspibase + SPI_STATUS);}
        
}
do 
  {SSPSR=ioread32(gspibase + 0x0c);}
  while  (SSPSR&0x10);

iowrite32(0x20,GPIO0_BASE+0x1c);//SET1             
};