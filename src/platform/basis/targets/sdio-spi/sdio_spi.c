#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <devices/sdio_spi.h>
#include <rumboot/irq.h>
#include <platform/devices.h>
#include <platform/interrupts.h>

volatile unsigned SDIO_HANDLED;
volatile unsigned timeout;
unsigned src_array[128];
unsigned dst_array[128];
int init_sd()
{
unsigned read_data; 

 //Switch to SDIO from GSPI
 // SDIO_SPI = true;
  iowrite32(0x1,SDIO_BASE + SDIO_ENABLE);  //sdio enable
  iowrite32(0x0,SDIO_BASE + SDIO_CLK_DIVIDE);  //sdio clock 
  iowrite32(0x7c,SDIO_BASE + SDIO_INT_MASKS ); //all sdio interrupts enable

  //Init SD card
  
  iowrite32( 0x16f,SDIO_BASE + 0x014);//enable the interrupt and the error flag  
  iowrite32( 0x0,SDIO_BASE + 0x008); //set CMD0 argument 
  iowrite32( 0x0,SDIO_BASE + 0x004);//set CMD0 index and send out the command
    read_data=ioread32 (SDIO_BASE+ 0x048);
  while (!(read_data&0x004))
      {read_data=ioread32 (SDIO_BASE+ 0x048);}
  iowrite32(0x004,SDIO_BASE+ 0x048);
  
    iowrite32( 0x16f,SDIO_BASE + 0x014);//enable the interrupt and the error flag  
  iowrite32( 0x0,SDIO_BASE + 0x008); //set CMD0 argument 
  iowrite32( 0x0,SDIO_BASE + 0x004);//set CMD0 index and send out the command
    read_data=ioread32 (SDIO_BASE+ 0x048);
  while (!(read_data&0x004))
      {read_data=ioread32 (SDIO_BASE+ 0x048);}
  iowrite32(0x004,SDIO_BASE+ 0x048);
  
  
  iowrite32( 0x000001aa,SDIO_BASE + 0x008);//set CMD8 argument  
  iowrite32( 0x00083811,SDIO_BASE + 0x004);//set CMD8 index and send out the command
  read_data=ioread32 (SDIO_BASE + 0x048);
  while (!(read_data&0x004))
      {read_data=ioread32 (SDIO_BASE+ 0x048);}
 iowrite32(0x004,SDIO_BASE + 0x048);
  //if (!wait_sdio_int_handled(1000)) {trace_msg("SDIO interrupt timeout\n"); return TEST_ERROR;}
  
  iowrite32( 0x00000000,SDIO_BASE + 0x008);// set CMD55 argument  
  iowrite32( 0x00373811,SDIO_BASE + 0x004);//set CMD55 index and send out the command
  //if (!wait_sdio_int_handled(1000)) {trace_msg("SDIO interrupt timeout\n"); return TEST_ERROR;}
    read_data=ioread32 (SDIO_BASE+ 0x048);
  while (!(read_data&0x004))
      {read_data=ioread32 (SDIO_BASE+ 0x048);}
  iowrite32(0x004,SDIO_BASE+ 0x048);
  
  iowrite32( 0x80ff8000,SDIO_BASE + 0x008);//set CMD41 argument   
  iowrite32( 0x00293811,SDIO_BASE + 0x004);//set CMD41 index and send out the command
  //if (!wait_sdio_int_handled(1000)) {trace_msg("SDIO interrupt timeout\n"); return TEST_ERROR;}
    read_data=ioread32 (SDIO_BASE+ 0x048);
  while (!(read_data&0x004))
      {read_data=ioread32 (SDIO_BASE+ 0x048);}
  iowrite32(0x004,SDIO_BASE+ 0x048);
  
  iowrite32( 0x00000000,SDIO_BASE + 0x008);//set CMD2 argument  
  iowrite32( 0x00023411,SDIO_BASE + 0x004);//set CMD2 index and send out the command
  //if (!wait_sdio_int_handled(1000)) {trace_msg("SDIO interrupt timeout\n"); return TEST_ERROR;}
    read_data=ioread32 (SDIO_BASE+ 0x048);
  while (!(read_data&0x004))
      {read_data=ioread32 (SDIO_BASE+ 0x048);}
  iowrite32(0x004,SDIO_BASE+ 0x048);
  
  iowrite32( 0x00000000,SDIO_BASE + 0x008); //set CMD3 argument 
  iowrite32( 0x00033811,SDIO_BASE + 0x004);//set CMD3 index and send out the command
  //if (!wait_sdio_int_handled(1000)) {trace_msg("SDIO interrupt timeout\n"); return TEST_ERROR;}
    read_data=ioread32 (SDIO_BASE+ 0x048);
  while (!(read_data&0x004))
      {read_data=ioread32 (SDIO_BASE+ 0x048);}
  iowrite32(0x004,SDIO_BASE+ 0x048);
  
  iowrite32( 0x12340000,SDIO_BASE + 0x008);//set CMD7 argument  
  iowrite32( 0x00073c11,SDIO_BASE + 0x004);//set CMD7index and send out the command
  //if (!wait_sdio_int_handled(1000)) {trace_msg("SDIO interrupt timeout\n"); return TEST_ERROR;}
    read_data=ioread32 (SDIO_BASE+ 0x048);
  while (!(read_data&0x004))
      {read_data=ioread32 (SDIO_BASE+ 0x048);}
  iowrite32(0x004,SDIO_BASE+ 0x048);
  
  iowrite32( 0x12340000,SDIO_BASE + 0x008); //set CMD55 argument 
  iowrite32( 0x00373811,SDIO_BASE + 0x004);//set CMD55 index and send out the command
  //if (!wait_sdio_int_handled(1000)) {trace_msg("SDIO interrupt timeout\n"); return TEST_ERROR;}
    read_data=ioread32 (SDIO_BASE+ 0x048);
  while (!(read_data&0x004))
      {read_data=ioread32 (SDIO_BASE+ 0x048);}
  iowrite32(0x004,SDIO_BASE+ 0x048);
  
  iowrite32( 0x00000002,SDIO_BASE + 0x008);//set CMD6 argument  
  iowrite32( 0x00063811,SDIO_BASE + 0x004);//set CMD6 index and send out the command
  //if (!wait_sdio_int_handled(1000)) {trace_msg("SDIO interrupt timeout\n"); return TEST_ERROR;}
   read_data=ioread32 (SDIO_BASE+ 0x048);
  while (!(read_data&0x004))
      {read_data=ioread32 (SDIO_BASE+ 0x048);}
  iowrite32(0x004,SDIO_BASE+ 0x048);
  
 return 0;
}
void prepare_array()
{
  unsigned i;
  for (i=0; i<128; i++) 
   src_array[i]= i + (i<<8) + (i<<16) + (i<<24); 
}  

int check_array()
{
 unsigned i;
 for (i=0; i<128; i++)
 { 
  if ( !( src_array[i] == dst_array[i]))  
    return 1;
 }
  return 0;
}



static void handler1()
{
unsigned int SDIO_Status;
unsigned int SDIO_clear = 0;

       SDIO_Status=ioread32(SDIO_BASE + SDIO_INT_STATUS); //Read interrupt status

            if ((SDIO_Status>>5)&0x1) SDIO_clear = SDIO_clear|0x4; //Clear command done interrupt
            if ((SDIO_Status>>2)&0x1) SDIO_clear = SDIO_clear|0x1; //Clear buf transfer finish interrupt
            if ((SDIO_Status>>4)&0x1) SDIO_clear = SDIO_clear|0x2; //Clear card transfer done interrupt
            if ((SDIO_Status>>6)&0x1) 
              {
                rumboot_printf("Error in SDIO card !\n");
                SDIO_clear = SDIO_clear|0x10;
              }
            iowrite32(SDIO_clear,SDIO_BASE+ 0x048);
            SDIO_HANDLED=0x1;
}


uint32_t wait_sdio_int_handled(unsigned timeout)
{
    unsigned i = 0;
    while(!SDIO_HANDLED && (i++<timeout)){;}
    if (SDIO_HANDLED)
    {
      SDIO_HANDLED=0;
    return 1;
    }
    else 
    return 0;  
}

int init_irq()
{
  rumboot_irq_cli();
  struct rumboot_irq_entry *tbl = rumboot_irq_create(NULL);
  rumboot_irq_set_handler(tbl,GSPI_SDIO_IRQ,0,handler1,NULL);
  
  rumboot_irq_table_activate(tbl);
  rumboot_irq_sei();
  return 0;
}

int sdio_dma_tr (unsigned int* src_addr, unsigned int* dst_addr)
{
  //write data
  iowrite32(0x00000101,SDIO_BASE + 0x000);//one block data transfer,512 byte
  iowrite32(0x00000006,SDIO_BASE + 0x050);//select buf0,write transfer start 
  iowrite32(0x00000200,SDIO_BASE + 0x034);//set dma total data 512 byte
  iowrite32((unsigned int)src_addr,SDIO_BASE + 0x02c);//dma source atart address
  iowrite32(0x00002001,SDIO_BASE + 0x028);//ARLEN=16,ARSIZE=2(4bytes)   
  rumboot_printf("check_1\n");
   if (!wait_sdio_int_handled(1000)) 
    {rumboot_printf("SDIO interrupt timeout_1\n");
    return 1;
    }
  
  iowrite32(0x00000000,SDIO_BASE + 0x00c);//set use buf0 and write data to SD bus
  iowrite32(0x00000101,SDIO_BASE + 0x000);//set data block 512 byte ans send one block 
  iowrite32(0x00000000,SDIO_BASE + 0x008);//set cmd24 argument 
  iowrite32(0x00187811,SDIO_BASE + 0x004);//set cmd24 index out the command 
  rumboot_printf("check_2\n");
   if (!wait_sdio_int_handled(1000)) 
    {rumboot_printf("SDIO interrupt timeout_2\n");
    return 1;
    }
   if (!wait_sdio_int_handled(1000)) 
    {rumboot_printf("SDIO interrupt timeout_3\n");
    return 1;
    }
  
  //Read data from SD card
  iowrite32(0x00000000,SDIO_BASE + 0x00c);
  iowrite32(0x00000101,SDIO_BASE + 0x000);
  iowrite32(0x00000000,SDIO_BASE + 0x008);
  iowrite32(0x00117911,SDIO_BASE + 0x004);
  rumboot_printf("check_3\n");
   if (!wait_sdio_int_handled(1000)) 
    {rumboot_printf("SDIO interrupt timeout_4\n");
    return 1;
    }
//    if (!wait_sdio_int_handled(1000)) 
//     {rumboot_printf("SDIO interrupt timeout_5\n");
//     return 1;
//     }

  iowrite32(0x00000004,SDIO_BASE + 0x050);
  iowrite32( 0x00000200,SDIO_BASE + 0x044);
  iowrite32((unsigned int)dst_addr,SDIO_BASE + 0x040);    
  iowrite32(0x00020001,SDIO_BASE + 0x038);
  rumboot_printf("check_4\n");
   if (!wait_sdio_int_handled(1000)) 
    {rumboot_printf("SDIO interrupt timeout_6\n");
    return 1;
    }
  return 0;
}

int main()
{
init_sd();
init_irq();
prepare_array();
  if (sdio_dma_tr(src_array, dst_array)!=0)
return 1;
  if ( check_array()!=0)
return 1; 

return 0; 
}  



  









