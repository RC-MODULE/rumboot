#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <devices/sdio_spi.h>
#include <rumboot/irq.h>
#include <platform/devices.h>



int main()
{
unsigned read_data; 
    rumboot_printf("rumboot: hello sdio!\n");
    /* Return 0, test passed */


   //GSPI loop test - configure SSP
  //SDIO_SPI = false;
/*  iowrite32(0xc7,GSPI0_BASE + 0x000); //8-bit data
  iowrite32(0x03,GSPI0_BASE + 0x004); //turn on controller, loop operation
  iowrite32(0x02,GSPI0_BASE + 0x010); //clock prescale
  iowrite32(0x00,GSPI0_BASE + 0x014); //interrupt masks - mask all
  iowrite32(0x03,GSPI0_BASE + 0x024); //enable DMA

  
  unsigned i;
  for (i=0; i<8; i++) iowrite32(i + (i<<8) + (i<<16) + (i<<24),DMA_SDIO_SRC_IM0 + i*4);

  
 //GSPI loop test - dma exchenge
  iowrite32(0xc,GSPI0_BASE + 0x0d8); //interrupt masks - unmask axiw
  iowrite32((DMA_SDIO_SRC_IM0),(GSPI0_BASE + 0x0a4));//DMAR start address 
  iowrite32((DMA_SDIO_SRC_IM0 + 31),(GSPI0_BASE + 0x0a8));//DMAR end address
  iowrite32(0x80000000|0x1FFFFFE0 ,(GSPI0_BASE + 0x0ac));//DMAR set descriptor 
  iowrite32(0x1,(GSPI0_BASE + 0x0b4));//set read axi buffer tipe - single (not circled)
  iowrite32((DMA_SDIO_SRC_IM0 + 0x100),(GSPI0_BASE + 0x090));//DMAW start address
  iowrite32((DMA_SDIO_SRC_IM0 + 0x100 + 31),(GSPI0_BASE + 0x094));//DMAW end address
  iowrite32( 0x80000000|0x1FFFFFE0,(GSPI0_BASE + 0x098));//DMAW set descriptor 
  iowrite32( 0x1ef,(GSPI0_BASE + 0x0c4));//set axi params AWLEN=ARLEN=F, all LSB (0 - LSB, 1 - MSB) ??
  iowrite32( 0x1,(GSPI0_BASE + 0x0b8));//start AXIR DMA
  
  
  read_data=ioread32(GSPI0_BASE + 0x0d4);
  while (!(read_data&0x008))
    {read_data=ioread32(GSPI0_BASE + 0x0d4);}
  while (!(read_data&0x004))
    {read_data=ioread32(GSPI0_BASE + 0x0d4);}
    
  
  //GSPI loop test - compare data
  for (i=0; i<8; i++)
  {
    if (ioread32(DMA_SDIO_SRC_IM0 + 0x100 + i*4) != i + (i<<8) + (i<<16) + (i<<24))
    {
      rumboot_printf
      (
	"Error DMA data check. Addr=%x, data_ref=%x, data_rd=%x \n",
	DMA_SDIO_SRC_IM0 + 0x100 + i*4,
	i + (i<<8) + (i<<16) + (i<<24),
	ioread32(DMA_SDIO_SRC_IM0 + 0x100 + i*4)
      );
      return 1;     
    }
  }

 
*/ 
 //Switch to SDIO from GSPI
 // SDIO_SPI = true;
  iowrite32(0x1,SDIO0_BASE + SDIO_ENABLE);  //sdio enable
  iowrite32(0x0,SDIO0_BASE + SDIO_CLK_DIVIDE);  //sdio clock 
  iowrite32(0x7c,SDIO0_BASE + SDIO_INT_MASKS ); //all sdio interrupts enable
  
 
  //Init SD card
  
  iowrite32( 0x16f,SDIO0_BASE + 0x014);//enable the interrupt and the error flag  
  iowrite32( 0x0,SDIO0_BASE + 0x008); //set CMD0 argument 
  iowrite32( 0x0,SDIO0_BASE + 0x004);//set CMD0 index and send out the command
    read_data=ioread32 (SDIO0_BASE+ 0x048);
  while (!(read_data&0x004))
      {read_data=ioread32 (SDIO0_BASE+ 0x048);}
  iowrite32(0x004,SDIO0_BASE+ 0x048);
  
  iowrite32( 0x000001aa,SDIO0_BASE + 0x008);//set CMD8 argument  
  iowrite32( 0x00083811,SDIO0_BASE + 0x004);//set CMD8 index and send out the command
  read_data=ioread32 (SDIO0_BASE+ 0x048);
  while (!(read_data&0x004))
      {read_data=ioread32 (SDIO0_BASE+ 0x048);}
 iowrite32(0x004,SDIO0_BASE + 0x048);
  //if (!wait_sdio_int_handled(1000)) {trace_msg("SDIO interrupt timeout\n"); return TEST_ERROR;}
  
  iowrite32( 0x00000000,SDIO0_BASE + 0x008);// set CMD55 argument  
  iowrite32( 0x00373811,SDIO0_BASE + 0x004);//set CMD55 index and send out the command
  //if (!wait_sdio_int_handled(1000)) {trace_msg("SDIO interrupt timeout\n"); return TEST_ERROR;}
    read_data=ioread32 (SDIO0_BASE+ 0x048);
  while (!(read_data&0x004))
      {read_data=ioread32 (SDIO0_BASE+ 0x048);}
  iowrite32(0x004,SDIO0_BASE+ 0x048);
  
  iowrite32( 0x80ff8000,SDIO0_BASE + 0x008);//set CMD41 argument   
  iowrite32( 0x00293811,SDIO0_BASE + 0x004);//set CMD41 index and send out the command
  //if (!wait_sdio_int_handled(1000)) {trace_msg("SDIO interrupt timeout\n"); return TEST_ERROR;}
    read_data=ioread32 (SDIO0_BASE+ 0x048);
  while (!(read_data&0x004))
      {read_data=ioread32 (SDIO0_BASE+ 0x048);}
  iowrite32(0x004,SDIO0_BASE+ 0x048);
  
  iowrite32( 0x00000000,SDIO0_BASE + 0x008);//set CMD2 argument  
  iowrite32( 0x00023411,SDIO0_BASE + 0x004);//set CMD2 index and send out the command
  //if (!wait_sdio_int_handled(1000)) {trace_msg("SDIO interrupt timeout\n"); return TEST_ERROR;}
    read_data=ioread32 (SDIO0_BASE+ 0x048);
  while (!(read_data&0x004))
      {read_data=ioread32 (SDIO0_BASE+ 0x048);}
  iowrite32(0x004,SDIO0_BASE+ 0x048);
  
  iowrite32( 0x00000000,SDIO0_BASE + 0x008); //set CMD3 argument 
  iowrite32( 0x00033811,SDIO0_BASE + 0x004);//set CMD3 index and send out the command
  //if (!wait_sdio_int_handled(1000)) {trace_msg("SDIO interrupt timeout\n"); return TEST_ERROR;}
    read_data=ioread32 (SDIO0_BASE+ 0x048);
  while (!(read_data&0x004))
      {read_data=ioread32 (SDIO0_BASE+ 0x048);}
  iowrite32(0x004,SDIO0_BASE+ 0x048);
  
  iowrite32( 0x12340000,SDIO0_BASE + 0x008);//set CMD7 argument  
  iowrite32( 0x00073c11,SDIO0_BASE + 0x004);//set CMD7index and send out the command
  //if (!wait_sdio_int_handled(1000)) {trace_msg("SDIO interrupt timeout\n"); return TEST_ERROR;}
    read_data=ioread32 (SDIO0_BASE+ 0x048);
  while (!(read_data&0x004))
      {read_data=ioread32 (SDIO0_BASE+ 0x048);}
  iowrite32(0x004,SDIO0_BASE+ 0x048);
  
  iowrite32( 0x12340000,SDIO0_BASE + 0x008); //set CMD55 argument 
  iowrite32( 0x00373811,SDIO0_BASE + 0x004);//set CMD55 index and send out the command
  //if (!wait_sdio_int_handled(1000)) {trace_msg("SDIO interrupt timeout\n"); return TEST_ERROR;}
    read_data=ioread32 (SDIO0_BASE+ 0x048);
  while (!(read_data&0x004))
      {read_data=ioread32 (SDIO0_BASE+ 0x048);}
  iowrite32(0x004,SDIO0_BASE+ 0x048);
  
  iowrite32( 0x00000002,SDIO0_BASE + 0x008);//set CMD6 argument  
  iowrite32( 0x00063811,SDIO0_BASE + 0x004);//set CMD6 index and send out the command
  //if (!wait_sdio_int_handled(1000)) {trace_msg("SDIO interrupt timeout\n"); return TEST_ERROR;}
   read_data=ioread32 (SDIO0_BASE+ 0x048);
  while (!(read_data&0x004))
      {read_data=ioread32 (SDIO0_BASE+ 0x048);}
  iowrite32(0x004,SDIO0_BASE+ 0x048);
  
 return 0;
}
  
int sdio_dma_tr (unsigned src_addr, unsigned dst_addr)
{
  unsigned read_data;  
  //Write data to SD card
  iowrite32(0x00000101,SDIO0_BASE + 0x000);//one block data transfer,512 byte
  iowrite32(0x00000006,SDIO0_BASE + 0x050);//select buf0,write transfer start 
  iowrite32(0x00000200,SDIO0_BASE + 0x034);//set dma total data 512 byte
  iowrite32(src_addr,SDIO0_BASE + 0x02c);//dma source atart address
  iowrite32(0x000020f1,SDIO0_BASE + 0x028);//ARLEN=16,ARSIZE=2(4bytes)
  //if (!wait_sdio_int_handled(1000)) {trace_msg("SDIO interrupt timeout\n"); return TEST_ERROR;}
read_data=ioread32 (SDIO0_BASE+ 0x28);
  while (!(read_data&0x00080000))
      {read_data=ioread32 (SDIO0_BASE+ 0x028);}
  iowrite32(0x00080000,SDIO0_BASE+ 0x028);
  read_data=ioread32 (SDIO0_BASE+ 0x28);
read_data=ioread32 (SDIO0_BASE+ 0x28);
  while (!(read_data&0x00000001))
      {read_data=ioread32 (SDIO0_BASE+ 0x048);}
  iowrite32(0x00000001,SDIO0_BASE+ 0x048);
  read_data=ioread32 (SDIO0_BASE+ 0x48);

  iowrite32(0x00000000,SDIO0_BASE + 0x00c);//set use buf0 and write data to SD bus
  iowrite32(0x00000101,SDIO0_BASE + 0x000);//set data block 512 byte ans send one block 
  iowrite32(0x00000000,SDIO0_BASE + 0x008);//set cmd24 argument 
  iowrite32(0x00187811,SDIO0_BASE + 0x004);//set cmd24 index out the command 
  //if (!wait_sdio_int_handled(1000)) {trace_msg("SDIO interrupt timeout\n"); return TEST_ERROR;}
read_data =ioread32 (SDIO0_BASE+ 0x048);
  while (!(read_data&0x004))
      {read_data=ioread32 (SDIO0_BASE+ 0x048);}
  iowrite32(0x004,SDIO0_BASE+ 0x048);
read_data=ioread32 (SDIO0_BASE+ 0x048);
  while (!(read_data&0x002))
      {read_data=ioread32 (SDIO0_BASE+ 0x048);}
  iowrite32(0x002,SDIO0_BASE+ 0x048);
  
 
  //Read data from SD card
  iowrite32(0x00000000,SDIO0_BASE + 0x00c);
  iowrite32(0x00000101,SDIO0_BASE + 0x000);
  iowrite32(0x00000000,SDIO0_BASE + 0x008);
  iowrite32(0x00117911,SDIO0_BASE + 0x004);
  //if (!wait_sdio_int_handled(1000)) {trace_msg("SDIO interrupt timeout\n"); return TEST_ERROR;}
  read_data=ioread32 (SDIO0_BASE+ 0x048);
  while (!(read_data&0x004))
      {read_data=ioread32 (SDIO0_BASE+ 0x048);}
  iowrite32(0x004,SDIO0_BASE+ 0x048);
read_data=ioread32 (SDIO0_BASE+ 0x048);
  while (!(read_data&0x002))
      {read_data=ioread32 (SDIO0_BASE+ 0x048);}
  iowrite32(0x002,SDIO0_BASE+ 0x048);
  
 iowrite32(0x00000004,SDIO0_BASE + 0x050);
 iowrite32( 0x00000200,SDIO0_BASE + 0x044);
 iowrite32(dst_addr,SDIO0_BASE + 0x040);
 iowrite32(0x00020f01,SDIO0_BASE + 0x038);
 // if (!wait_sdio_int_handled(1000)) {trace_msg("SDIO interrupt timeout\n"); return TEST_ERROR;}
 read_data=ioread32 (SDIO0_BASE+ 0x28);
  while (!(read_data&0x00080000))
      {read_data=ioread32 (SDIO0_BASE+ 0x028);}
  iowrite32(0x00080000,SDIO0_BASE+ 0x028);
  read_data=ioread32 (SDIO0_BASE+ 0x28);
read_data=ioread32 (SDIO0_BASE+ 0x28);
  while (!(read_data&0x00000001))
      {read_data=ioread32 (SDIO0_BASE+ 0x048);}
  iowrite32(0x00000001,SDIO0_BASE+ 0x048);
  read_data=ioread32 (SDIO0_BASE+ 0x48);

 return 0;
} 


  









