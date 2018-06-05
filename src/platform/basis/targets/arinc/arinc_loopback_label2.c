// 
//  Test includes:
//  - 16 ARINC429 transmitters and receivers delivery through the external loopback
//	- 16  receivers accept short arrays awithout checking labels 
//  - configured  frequency =100KHz by default value
//	- use delay control for neighboring arrays switch for correct read status registers
//  - accept 2 32 bit words from all transmitted array separating it by label's using for every receiver
//	- received arrays compared with ethalon values	
//    Test duration (RTL): < 
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <regs/arinc.h>
#include <rumboot/irq.h>
#include <platform/devices.h>
static const int32_t tx_array32[] = {
        0x80818283,
        0x40414243,
        0x20212223,
		0x10111213,
        0x08090a08,
		0x04152608,
        0x02132434,
        0x01122356,
        0x00112233,
        0x7F8091AB,
		0x8F90A1B2,
		0xDF90A1B1,
        0xE090A1B0,
        0xEF90A1B7,
        0xF790A1B6,
        0xFB90A1B2,
		0xFF0EFEFC,
		0xFF1EFDFd,
		0xF01973FE,
		0xF11873FF,
		0x70718281,
        0x60413253,
        0x50212231,
		0x10111214,
        0x41045a06,
		0x04152637,
        0x02532456,
        0x05122334,
        0x50112230,
        0x7F8091A6,
		0x8F90A162,
		0xDF90A6B2
};
static const int32_t rx_array32[] = {
        0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
        0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0
};
void arinc_init (uint32_t arinc_base_addr){
	//uint32_t receiver_number;	
     int i =0;
	uint32_t  init_axi_mst;
	uint32_t size;
   int j;
	init_axi_mst =0x0;
	size   = 0x2;

	uint32_t  tx_mem = 0x40000;
	uint32_t  rx_mem = 0x50000;

//-------------------------------------------------------------------------------------
//  Set parameters for DMA exchange loopback  connected on chip pins in testbench
//-------------------------------------------------------------------------------------
	//iowrite32(receiver_number,(arinc_base_addr + SELF_RX)); //receiver ch number
	
		for (i = 0; i< 16; i++)	{    
	iowrite32(i,(arinc_base_addr + RNUM_RX +4*i));      			// even receiver buffer ch number
	//rumboot_printf("RCV_NUM=0x%x\n", i); 
	iowrite32((tx_mem + i*8),(arinc_base_addr + AG_E_TX  + i*4));   // dma rd channel memory address
	//rumboot_printf("AG_E_TX=0x%x\n", (tx_mem + i*8)); 
	iowrite32(size,(arinc_base_addr + SZ_E_TX + i*4));    			// dma rd channel size
	iowrite32((rx_mem + i*8),(arinc_base_addr + AG_E_RX + i*4));  	// dma wr channel memory address
	iowrite32(0x1,(arinc_base_addr + SZ_E_RX + i*4)); // dma wr channel size
	
	}
	iowrite32(init_axi_mst,(arinc_base_addr + AXI_CTRL)); 	//AXI parameters
	for (j = 0; j< 8; j++)	{ 								    // label fifo depth
	iowrite32(0x43, arinc_base_addr + LABEL_RX );
	iowrite32(0x55, arinc_base_addr + LABEL_RX +4);
	iowrite32(0x66, arinc_base_addr + LABEL_RX +8);
	iowrite32(0x77, arinc_base_addr + LABEL_RX +12);
	
	iowrite32(0x88, arinc_base_addr + LABEL_RX + 16);
	iowrite32(0x99, arinc_base_addr + LABEL_RX + 20);
	iowrite32(0xAA, arinc_base_addr + LABEL_RX + 24);
	iowrite32(0xBB, arinc_base_addr + LABEL_RX + 28);

	iowrite32(0xCC, arinc_base_addr + LABEL_RX + 32);
	iowrite32(0xDD, arinc_base_addr + LABEL_RX + 36);
	iowrite32(0xEE, arinc_base_addr + LABEL_RX + 40);
	iowrite32(0x31, arinc_base_addr + LABEL_RX + 44);
	
	iowrite32(0x11, arinc_base_addr + LABEL_RX + 48);
	iowrite32(0x22, arinc_base_addr + LABEL_RX + 52);
	iowrite32(0x55, arinc_base_addr + LABEL_RX + 56);
	iowrite32(0x88, arinc_base_addr + LABEL_RX + 60);	
		}	
	iowrite32(0xFFFF,(arinc_base_addr  + LABEL_EN_RX));		// receiver LABEL enable   
	iowrite32(0xffff,(arinc_base_addr + WAIT_TMR_TX));   			// wait  after the first  transmitter start
	iowrite32(0xffff,(arinc_base_addr + WAIT_SIG_RX));   			// wait delayed switch after the receiver start
    }	
//--------------------------------------------------------------------------------------


int main()

{
	int tmp = -1;
	uint32_t enable;
	int cnt;
	int i;
	//int j;
	//int k;
	enable =0xffffffff;
	uint32_t tmp_r =-1;
	cnt = 0;
  uint32_t addr;
	uint32_t addr_rd;
	uint32_t *trg = (uint32_t *) 0x40000;
	uint32_t *rrg = (uint32_t *) 0x50000;
  uint32_t  tx_mem = 0x40000;
	uint32_t  rx_mem = 0x50000;
	
	rumboot_printf("copy %d bytes\n", sizeof(tx_array32));  //transmit	
	memcpy(trg, tx_array32,sizeof(tx_array32));
    memcpy(rrg, rx_array32,sizeof(rx_array32));
	
	arinc_init(ARINC_BASE);
    iowrite32(enable,ARINC_BASE + CHANNEL_EN); // run transaction
	rumboot_printf("ARINC LABEL LOOPBACK START \n");
	//check setting of the end of transaction delivery
	for (i = 0; i< 16 ; i++)
	{
	//rumboot_printf("ARINC CH=0x%x\n", i); 	
	while (tmp != 0x00000002) {

	tmp = ioread32(ARINC_BASE + STAT_E_TX + i*4);
	tmp = 0x07FFFFFF & tmp;
	//rumboot_printf("ARINC CH=0x%x\n", i); 
   // rumboot_printf("ARINC SIZE=0x%x\n", tmp); //check status
		}
	iowrite32( ((1 << (0x10000 +i) )*0x10000),ARINC_BASE + CHANNEL_DIS); // stop transmitter
  //rumboot_printf("channel_dis=0x%x\n", (1 << (16 +i))); 
    }
     
	 
	
 	rumboot_printf("ARINC check status RX\n"); 
	for (i = 0; i< 16 ; i++) 
	{
	while (tmp_r != status_success_bit) {			 
	tmp = ioread32(ARINC_BASE + STAT_E_RX +i*4);
    tmp_r = tmp & status_success_bit;

	if ((++cnt == ARINC_ATTEMPT)& ((i==1)||(i==22)))  {
            rumboot_printf("No end exchange!\n");
           return TEST_ERROR;
				}
			
		}
	//rumboot_printf("i=0x%x\n", i); //check status		
	}
	rumboot_printf("ARINC Compare arrays \n");
	
for (i = 0; i< 32 ; i++)

	{
	addr 		=	tx_mem + 4*i;
	addr_rd 	=	rx_mem + 4*i;

	tmp			= 	ioread32(addr);
	//rumboot_printf("READ_etalon_adr=0x%x\n", addr);
	//rumboot_printf("READ_etalon_DATA=0x%x\n", tmp);

	//rumboot_printf("READ_adr=0x%x\n", addr_rd);
	tmp_r 		= 	ioread32(addr_rd);
	//rumboot_printf("READ_DATA=0x%x\n", tmp_r);

	tmp  = tmp -tmp_r;
	
	if ((tmp_r != 0x40414243)& (i==0)) {		//	(addr_rd == 0x50000) & (tmp_r == 0x40414243)
		rumboot_printf("ARINC429 LOOPBACK test ERROR \n");
		return TEST_ERROR;
		}
	
	if ((tmp != 0)& (i==22)) {				//(addr_rd == 0x50058) & (tmp_r == 0x50212231)
		rumboot_printf("ARINC429 LOOPBACK test ERROR \n");
		return TEST_ERROR;
		}
		
		
		
	}
	
	//if (tmp == ARINC_OK) {
	 rumboot_printf("ARINC429 LOOPBACK test OK \n");
	 return TEST_OK;

			
}
	
/*
	
	
	
for (i = 0; i< 32 ; i++)
	{
	addr_rd 	=	rx_mem + 4*i;
	tmp_r = ioread32(addr_rd);
	if ( (addr_rd == 0x50058) & (tmp_r == 0x50212231)){
	   rumboot_printf("ARINC LOOPBACK label OK \n");
		}
		
	if ((addr_rd == 0x50000) & (tmp_r == 0x40414243)){
	    rumboot_printf("ARINC LOOPBACK label OK \n");
		return TEST_OK;
		} 
		
	 else {
		rumboot_printf("ARINC LOOPBACK test ERROR \n");
	 return TEST_ERROR; }
		
	}
	

			
}
*/