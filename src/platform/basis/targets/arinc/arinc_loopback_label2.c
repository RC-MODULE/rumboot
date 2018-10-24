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
#include <rumboot/rumboot.h>
#include <rumboot/platform.h>
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

void arinc_init_label (){

	int i =0;
	uint32_t  init_axi_mst;
	uint32_t size;
	int j;
	init_axi_mst =0x0;
	size   = 0x2;

//-------------------------------------------------------------------------------------
//  Set parameters for DMA exchange loopback  connected on chip pins in testbench
//-------------------------------------------------------------------------------------	
		for (i = 0; i< 16; i++)	{    
	iowrite32(i,(ARINC_BASE + RNUM_RX +4*i));      			// even receiver buffer ch number
	iowrite32(size,(ARINC_BASE + SZ_E_TX + i*4));    		// dma rd channel size
	iowrite32(0x1,(ARINC_BASE + SZ_E_RX + i*4)); // dma wr channel size	
	}
	iowrite32(init_axi_mst,(ARINC_BASE + AXI_CTRL)); 	//AXI parameters
	for (j = 0; j< 8; j++)	{ 							// label fifo depth
	iowrite32(0x43, ARINC_BASE + LABEL_RX );
	iowrite32(0x55, ARINC_BASE + LABEL_RX +4);
	iowrite32(0x66, ARINC_BASE + LABEL_RX +8);
	iowrite32(0x77, ARINC_BASE + LABEL_RX +12);
	
	iowrite32(0x88, ARINC_BASE + LABEL_RX + 16);
	iowrite32(0x99, ARINC_BASE + LABEL_RX + 20);
	iowrite32(0xAA, ARINC_BASE + LABEL_RX + 24);
	iowrite32(0xBB, ARINC_BASE + LABEL_RX + 28);

	iowrite32(0xCC, ARINC_BASE + LABEL_RX + 32);
	iowrite32(0xDD, ARINC_BASE + LABEL_RX + 36);
	iowrite32(0xEE, ARINC_BASE + LABEL_RX + 40);
	iowrite32(0x31, ARINC_BASE + LABEL_RX + 44);
	
	iowrite32(0x11, ARINC_BASE + LABEL_RX + 48);
	iowrite32(0x22, ARINC_BASE + LABEL_RX + 52);
	iowrite32(0x55, ARINC_BASE + LABEL_RX + 56);
	iowrite32(0x88, ARINC_BASE + LABEL_RX + 60);	
		}	
	iowrite32(0xFFFF,(ARINC_BASE  + LABEL_EN_RX));		// receiver LABEL enable   
	iowrite32(0xffff,(ARINC_BASE + WAIT_TMR_TX));   	// wait  after the first  transmitter start
	iowrite32(0xffff,(ARINC_BASE + WAIT_SIG_RX));   	// wait delayed switch after the receiver start
    }	
//--------------------------------------------------------------------------------------

int arinc_status_check16() {

	uint32_t tmp_r= -1;
	int cnt=0;
	uint32_t tmp; 
	int i;
	for (i = 0; i< 16 ; i++)
	{
	rumboot_printf("ARINC CH=0x%x\n", i); 	
       while (tmp != 0x00000002) {
	tmp = ioread32(ARINC_BASE + STAT_E_TX + i*4);
	tmp = 0x07FFFFFF & tmp;
		if (++cnt == ARINC_ATTEMPT) {
            rumboot_printf("No end exchange!\n");
            return TEST_ERROR;
				}	
			}   
	iowrite32(( (1 << i) + (1 << (16 +i)) ),ARINC_BASE + CHANNEL_DIS); // stop transmitter
    }
    i=0; 
	for (i = 0; i< 16 ; i++) 
{
	cnt =0;
	while (tmp_r != status_success_bit) {			 
	tmp = ioread32(ARINC_BASE + STAT_E_RX +i*4);
    tmp_r = tmp & status_success_bit;

	if ((++cnt == ARINC_ATTEMPT)/*& ((i==1)||(i==22))*/)  {
            rumboot_printf("No end exchange!\n");
           return TEST_ERROR;
				}
			
		}
	//rumboot_printf("i=0x%x\n", i); //check status		
	}
return TEST_OK;	
}


int main()

{
	int tmp = -1;
	uint32_t enable;
	int i;

	enable =0xffffffff;
	//uint32_t tmp_r =-1;
	
	uint32_t addr;
	uint32_t addr_rd;

	rumboot_printf("copy %d bytes\n", sizeof(tx_array32));  //transmit	
	uint32_t *tx_mem = rumboot_malloc_from_heap_aligned(0, sizeof(tx_array32), 4);
	uint32_t *trg = (uint32_t *) tx_mem;
	memcpy(trg, tx_array32,sizeof(tx_array32));
	
	uint32_t *rx_mem = rumboot_malloc_from_heap_aligned(0, sizeof(tx_array32), 4);

	
		for (i = 0; i< 16; i++)	{
			iowrite32((uint32_t) &tx_mem[2*i],(ARINC_BASE + AG_E_TX + i*4)); 	// dma rd channel memory address
			iowrite32((uint32_t) &rx_mem[2*i],(ARINC_BASE + AG_E_RX + i*4)); 	// dma wr channel memory address

	}
	
	arinc_init_label(ARINC_BASE);
    iowrite32(enable,ARINC_BASE + CHANNEL_EN); // run transaction
	rumboot_printf("ARINC LABEL LOOPBACK START \n");
	

	//check setting of the end of transaction delivery
	
	tmp = arinc_status_check16();
	if (tmp == TEST_ERROR)  {
		 rumboot_printf(" TEST_ERROR\n");
		 return TEST_ERROR;}
	
	rumboot_printf("ARINC Compare arrays \n");
	
for (i = 0; i< 32 ; i++)

	{
	
	addr 		= (uint32_t) &tx_mem[i];
	addr_rd 	= (uint32_t) &rx_mem[i];

	tmp			= 	ioread32(addr);

	
	if ((ioread32(addr_rd) != 0x40414243) && (i==0) && (tmp !=0)) {		//	(addr_rd == 0x50000) & (tmp_r == 0x40414243)
		rumboot_printf("ARINC429 LOOPBACK test ERROR \n");
		return TEST_ERROR;
		}
	
	if (((tmp - ioread32(addr_rd)) != 0)&& (i==22))  {				//(addr_rd == 0x50058) & (tmp_r == 0x50212231)
		rumboot_printf("ARINC429 LOOPBACK test ERROR \n");
		return TEST_ERROR;
		}		
	}
	 rumboot_printf("ARINC429 LOOPBACK test OK \n");
	 return TEST_OK;			
}
	
