#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <devices/sdio_spi.h>
#include <rumboot/irq.h>
#include <platform/devices.h>


#define SPI_DMAWSTART		  GSPI_BASE+0x90
#define SPI_DMAWEND 		  GSPI_BASE+0x94
#define SPI_DMAWCNTRL 		  GSPI_BASE+0x98
#define SPI_DMAWCUR 		  GSPI_BASE+0x9c
#define SPI_AXIW_BRESP 		  GSPI_BASE+0xa0
#define SPI_DMARSTART 		  GSPI_BASE+0xa4
#define SPI_DMAREND 		  GSPI_BASE+0xa8
#define SPI_DMARCNTRL 	 	  GSPI_BASE+0xac
#define SPI_DMARCUR  		  GSPI_BASE+0xb0

#define Size                      33





unsigned int etalonarray[Size*2] __attribute__ ((section("data.esram1"))) = {
  0x00000000, // Dummy Pulled Down 32 Bits
  0xE6BF2F32,
  0xE222200D,
  0x1AFFFFF9,
  0xE92D0003,
  0xE59F1094,
  0xE8BD0003,
  0xE59F0080,
  0xEA000004,
  0x54205952,
  0x73736170,
  0xE4912004,
  0xE5802000,
  0xE1B02C02,
  0xE8BD0007,
  0xE59F0040,
  0xEA000002,
  0x65206C61,
  0xE4912004,
  0xE5802000,
  0xE1B02C02,
  0xE8BD0007,
  0xE59F0008,
  0xE5801000,
  0xFFF8F000,
  0xE3A00000,
  0xE3A02000,
  0xE3A04000,
  0xE3A06000,
  0xE3A08000,
  0xE3A0A000,
  0xE3A0C000,
  0xE3A0E000,
  0xE3A08000,
  0xE4912004,
  0xE5802000,
  0xE1B02C02,
  0xE8BD0007,
  0xE59F0094,
  0xE5801000,
  0xE92D0007,
  0xE28F1000,
  0x4F4D454D,
  0x20545345,
  0x0D0D6465,
  0xE6BF2F32,
  0xE222200D,
  0x1AFFFFF9,
  0xE92D0007,
  0xE28F1000,
  0x6D726F4E,
  0x0D746978,
  0xE6BF2F32,
  0xE222200D,
  0x1AFFFFF9,
  0xE92D0003,
  0xE59F1008,
  0xE8BD0003,
  0x20202004,
  0xE3A01000,
  0xE3A03000,
  0xE3A05000,
  0xE3A07000,
  0xE3A09000,
  0xE3A0B321
};

unsigned int srcarray[Size*2]  __attribute__ ((section("data.esram1"))) = {
  0x00000002, // Page Program Instruction
  0xE6BF2F32,
  0xE222200D,
  0x1AFFFFF9,
  0xE92D0003,
  0xE59F1094,
  0xE8BD0003,
  0xE59F0080,
  0xEA000004,
  0x54205952,
  0x73736170,
  0xE4912004,
  0xE5802000,
  0xE1B02C02,
  0xE8BD0007,
  0xE59F0040,
  0xEA000002,
  0x65206C61,
  0xE4912004,
  0xE5802000,
  0xE1B02C02,
  0xE8BD0007,
  0xE59F0008,
  0xE5801000,
  0xFFF8F000,
  0xE3A00000,
  0xE3A02000,
  0xE3A04000,
  0xE3A06000,
  0xE3A08000,
  0xE3A0A000,
  0xE3A0C000,
  0xE3A0E000,
  0xE3A08000,
  0xE4912004,
  0xE5802000,
  0xE1B02C02,
  0xE8BD0007,
  0xE59F0094,
  0xE5801000,
  0xE92D0007,
  0xE28F1000,
  0x4F4D454D,
  0x20545345,
  0x0D0D6465,
  0xE6BF2F32,
  0xE222200D,
  0x1AFFFFF9,
  0xE92D0007,
  0xE28F1000,
  0x6D726F4E,
  0x0D746978,
  0xE6BF2F32,
  0xE222200D,
  0x1AFFFFF9,
  0xE92D0003,
  0xE59F1008,
  0xE8BD0003,
  0x20202004,
  0xE3A01000,
  0xE3A03000,
  0xE3A05000,
  0xE3A07000,
  0xE3A09000,
  0xE3A0B321
};
unsigned int dstarray[Size*2] 	__attribute__ ((section("data.esram1"))) = {0} ;// Receive Here When Read
unsigned int dummyarray[Size*2]	__attribute__ ((section("data.esram1"))) = {0};// Receive Here When Write
     // Send This When Read // Read Instruction
     // Read Instruction Is Chosen Here (Instead Of Fast Read) Because It Aligns Valid Bytes On 32-bit Word
unsigned int readcmd[Size*2] __attribute__ ((section("data.esram1"))) = { 0x3,[ 1 ... (Size-1)] =0};

unsigned int writeenablecmd        =  0x06;
unsigned int readstatusregistercmd =  0x05;
unsigned int writedisablecmd       =  0x04;
unsigned int sectorerase           =  0xd8;
  
 void go_dmac(unsigned int* send, unsigned int* rcv, unsigned int amount) //
{   
unsigned int sup;
unsigned int read_data;
//unsigned int x;  
	

  
 
 sup=(int)send;
iowrite32(sup,SPI_DMARSTART);
  sup=(int)send + amount-1;
iowrite32(sup,SPI_DMAREND);
iowrite32(0x80000000|0x1FFFFFE0 ,(GSPI_BASE + 0x0ac));//DMAR set descriptor 
iowrite32(0x1,(GSPI_BASE + 0x0b4));//set read axi buffer tipe - single (not circled)
  sup=(int)rcv;
iowrite32(sup,SPI_DMAWSTART);
  sup=(int)rcv + amount-1;
iowrite32(sup,SPI_DMAWEND);
iowrite32( 0x80000000|0x1FFFFFE0,(GSPI_BASE + 0x098));;//DMAW set descriptor 
iowrite32( 0x1ef,(GSPI_BASE + 0x0c4));//set axi params AWLEN=ARLEN=F, all LSB (0 - LSB, 1 - MSB) ??
iowrite32( 0x1,(GSPI_BASE + 0x0b8));//start AXIR DMA
  read_data=ioread32(GSPI_BASE + 0x0d4);
  while (!(read_data&0x008))
    {read_data=ioread32(GSPI_BASE + 0x0d4);}
  while (!(read_data&0x004))
    {read_data=ioread32(GSPI_BASE + 0x0d4);}
    
};



 void init_ssp()
{   
unsigned int sup;

   sup=0xc7;
iowrite32(sup,GSPI_BASE + 0x000);//8-bit data
   sup=0x02;
iowrite32(sup,GSPI_BASE + 0x004); //turn on controller, loop operation
    sup=10;
iowrite32(sup,GSPI_BASE + 0x010); //clock prescale (10MHz)
    sup=0x00;
iowrite32(sup,GSPI_BASE + 0x014); //interrupt masks - mask all
    sup=0x03;
iowrite32(sup,GSPI_BASE + 0x024); //enable DMA	  
	  
};  



int main(void){

  unsigned int x;  

  init_ssp();	
  go_dmac(&writeenablecmd, dummyarray, 1);
  rumboot_printf("WREN Issued for SE\n");
  go_dmac(&sectorerase, dummyarray, 4);
  x=0x010000; // Test bit0 of STREG
  do {
  // Three Bytes (OPC|DUMMY|DUMMY)/(DUMMY|STREG|STREG)
    go_dmac(&readstatusregistercmd , dummyarray , 3);
    rumboot_printf("status_reg=%x\n",
    (*dummyarray));  
  }while ((x& (*dummyarray)) != 0);
  rumboot_printf("SECTOR ERASE Complete\n");
  for( int i = 0; i < 4; i++){
    rumboot_printf("loop=%x\n",(i));
    //rgSPI_CPSR = SSPDivisor25MHz;
    unsigned int x;  


    //--------- Write Flash (Page Program) -----
    // -- (1) Issue Write Enable Instruction To Flash    
    go_dmac(&writeenablecmd, dummyarray, 1);
    rumboot_printf("WREN Issued\n");

    // -- (2) Run KDMAC Both Channels For Simultaneous SPI Transfer
    go_dmac(srcarray , dummyarray , 260);
    rumboot_printf("PP Complete\n");

    // --- (6) Wait Until Page Program Instruction Complete (On Real Board takes About 1.5 - 5 ms
    rumboot_printf("Waiting Page Program complete\n");
    x=0x010000; // Test bit0 of STREG
    do{
    // Three Bytes (OPC|DUMMY|DUMMY)/(DUMMY|STREG|STREG)
      go_dmac(&readstatusregistercmd , dummyarray , 3);
      rumboot_printf("status_reg=%x\n",(*dummyarray)); 
    }while ((x & (*dummyarray)) != 0);
    rumboot_printf("Write Complete\n");
    // -- (6) Write Disable Instruction 
    go_dmac(&writedisablecmd , dummyarray , 1);
    rumboot_printf("WRDI Issued\n");
    //--------- Read Flash -----

    // -- (1) Run KDMAC Both Channels For Simultaneous SPI Transfer
    go_dmac(readcmd , dstarray , 260);
    rumboot_printf("Read Complete\n");
    //----------- Check Received Array -------- 
    for (int n =0; n < (256/4+1); n++){
      if (dstarray[n] != etalonarray[n]) {
        rumboot_printf("FAILED!etalonarray=%x,dstarray=%x\n",(etalonarray[n]),(dstarray[n]));
        return 1;
      };
    }
  }
  rumboot_printf( "test on spi has PASSED!!!\n");

  return 0;
}   







