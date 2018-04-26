#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <devices/sdio_spi.h>
#include <rumboot/irq.h>
#include <../src/lib/drivers/pl022_ssp_dma.c>
#include <devices/go_dmac.h>
#include <regs/pl022_ssp.h>
#include <../src/lib/drivers/go_dmac.c>
#include <devices/gpio.h>




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
unsigned int dstarray[Size*2]   __attribute__ ((section("data.esram1"))) = {0} ;// Receive Here When Read
unsigned int dummyarray[Size*2] __attribute__ ((section("data.esram1"))) = {0};// Receive Here When Write
     // Send This When Read // Read Instruction
     // Read Instruction Is Chosen Here (Instead Of Fast Read) Because It Aligns Valid Bytes On 32-bit Word
unsigned int readcmd[Size*2] __attribute__ ((section("data.esram1"))) = { 0x3,[ 1 ... (Size-1)] =0};

unsigned int writeenablecmd        =  0x06;
unsigned int readstatusregistercmd =  0x05;
unsigned int writedisablecmd       =  0x04;
unsigned int sectorerase           =  0xd8;
unsigned int gspibase              = GSPI0_BASE;  
unsigned int gpio5_1 = 0x10;
unsigned int gpio5_2 = 0x0;

int main(void){
  unsigned int x;  
 rumboot_printf("lala\n");
  iowrite32(0x1010,SCTL_BASE + 0x128);
  iowrite32(0x1000,SCTL_BASE + 0x12C); 
  ssp_init(gspibase); 
  go_dmac(gspibase,&writeenablecmd, dummyarray, 1);
  rumboot_printf("WREN Issued for SE\n");
  go_dmac(gspibase,&sectorerase, dummyarray, 4);
  x=0x010000; // Test bit0 of STREG
  do {
  // Three Bytes (OPC|DUMMY|DUMMY)/(DUMMY|STREG|STREG)
    go_dmac(gspibase,&readstatusregistercmd , dummyarray , 3);
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
    go_dmac(gspibase,&writeenablecmd, dummyarray, 1);
    rumboot_printf("WREN Issued\n");

    // -- (2) Run KDMAC Both Channels For Simultaneous SPI Transfer
    go_dmac(gspibase,srcarray , dummyarray , 260);
    rumboot_printf("PP Complete\n");

    // --- (6) Wait Until Page Program Instruction Complete (On Real Board takes About 1.5 - 5 ms
    rumboot_printf("Waiting Page Program complete\n");
    x=0x010000; // Test bit0 of STREG
    do{
    // Three Bytes (OPC|DUMMY|DUMMY)/(DUMMY|STREG|STREG)
      go_dmac(gspibase,&readstatusregistercmd , dummyarray , 3);
      rumboot_printf("status_reg=%x\n",(*dummyarray)); 
    }while ((x & (*dummyarray)) != 0);
    rumboot_printf("Write Complete\n");
    // -- (6) Write Disable Instruction 
    go_dmac(gspibase,&writedisablecmd , dummyarray , 1);
    rumboot_printf("WRDI Issued\n");
    //--------- Read Flash -----

    // -- (1) Run KDMAC Both Channels For Simultaneous SPI Transfer
    go_dmac(gspibase,readcmd , dstarray , 260);
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





