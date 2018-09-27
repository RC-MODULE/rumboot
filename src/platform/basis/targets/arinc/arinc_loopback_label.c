//-----------------------------------------------------------------------------
//  This program is for checking ARINC429 transactions.
// 
//  Test includes:
//  - 16 ARINC429 transmitters and receivers delivery through the external loopback
//	- 16  receivers accept short arrays awithout checking labels 
//  - configured  frequency =100KHz by default value
//	- use delay control for neighboring arrays switch for correct read state registers
//  - accept one 32 bit word from every  transmitted array by label's using for every receiver
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
		0x08090a0B,
		0x04152637,
		0x02132456,
		0x01122334,		
		0x00112233,
		0x7F8091AB,
		0x8F90A1B2,
		0xDF90A1B1,
		0xE090A1B0,
		0xEF90A1B7,
		0xF790A1B6,
		0xFB90A1B2,
		0xFF0EFEFC,
		0xFF1EFDFC,
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
     

    i = 0;
	for (i = 0; i< 16 ; i++) 
	{
	cnt=0;
	//rumboot_printf("STAT_E_RX=0x%x\n", (STAT_E_RX +i*4)); 
	tmp_r= -1;
	while (tmp_r != status_success_bit) {
	tmp = ioread32(ARINC_BASE + STAT_E_RX +i*4);
	tmp_r = tmp & status_success_bit;
	if (++cnt == ARINC_ATTEMPT) {
			rumboot_printf("ARINC STATUS=0x%x\n", tmp); //check status
            rumboot_printf("No end exchange!\n");
            return TEST_ERROR;
			}
		}
	}
 return TEST_OK;						
}	


void arinc_init (){
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
	//iowrite32((tx_mem + i*8),(ARINC_BASE + AG_E_TX  + i*4));   // dma rd channel memory address
	iowrite32(size,(ARINC_BASE + SZ_E_TX + i*4));    			// dma rd channel size
	//iowrite32((rx_mem + i*8),(ARINC_BASE + AG_E_RX + i*4));  	// dma wr channel memory address
	iowrite32(0x1,(ARINC_BASE + SZ_E_RX + i*4)); // dma wr channel size

	}
	iowrite32(init_axi_mst,(ARINC_BASE + AXI_CTRL)); 	//AXI parameters
	for (j = 0; j< 8; j++)	{ 								    // label fifo depth
	iowrite32(0x83838383, ARINC_BASE + LABEL_RX );
	iowrite32(0x23232323, ARINC_BASE + LABEL_RX +4);
	iowrite32(0x0B0B0B0B, ARINC_BASE + LABEL_RX +8);
	iowrite32(0x56565656, ARINC_BASE + LABEL_RX +12);
	
	iowrite32(0x33333333, ARINC_BASE + LABEL_RX + 16);
	iowrite32(0xB2B2B2B2, ARINC_BASE + LABEL_RX + 20);
	iowrite32(0xB0B0B0B0, ARINC_BASE + LABEL_RX + 24);
	iowrite32(0xB6B6B6B6, ARINC_BASE + LABEL_RX + 28);

	iowrite32(0xFCFCFCFC, ARINC_BASE + LABEL_RX + 32);
	iowrite32(0xFEFEFEFE, ARINC_BASE + LABEL_RX + 36);
	iowrite32(0x81818181, ARINC_BASE + LABEL_RX + 40);
	iowrite32(0x31313131, ARINC_BASE + LABEL_RX + 44);
	
	iowrite32(0x06060606, ARINC_BASE + LABEL_RX + 48);
	iowrite32(0x56565656, ARINC_BASE + LABEL_RX + 52);
	iowrite32(0x30303030, ARINC_BASE + LABEL_RX + 56);
	iowrite32(0x62626262, ARINC_BASE + LABEL_RX + 60);	
		}	
	iowrite32(0xFFFF,(ARINC_BASE  + LABEL_EN_RX));				// receiver LABEL enable   
   	iowrite32(init_axi_mst,(ARINC_BASE + AXI_CTRL)); 			// AXI parameters
	iowrite32(0xffff,(ARINC_BASE + WAIT_TMR_TX));   			// wait  after the first  transmitter start
	iowrite32(0xffff,(ARINC_BASE + WAIT_SIG_RX));   			// wait delayed switch after the receiver start
   }	
//--------------------------------------------------------------------------------------


int main()
{
	int tmp = -1;
	uint32_t enable;

	int i;
	enable =0xffffffff;
	int32_t tmp_r =-1;
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
	arinc_init();
    iowrite32(enable,ARINC_BASE + CHANNEL_EN); // run transaction
	rumboot_printf("ARINC LABEL LOOPBACK START \n");
	//check setting of the end of transaction delivery
tmp = arinc_status_check16();
	if (tmp == TEST_ERROR)  {
		rumboot_printf(" TEST_ERROR\n");
		return TEST_ERROR;}
	rumboot_printf("ARINC Compare array \n");

for (i = 0; i< 16 ; i++)

	{
	addr 		=	(uint32_t)	&tx_mem[2*i];
	addr_rd 	=	(uint32_t)	&rx_mem[2*i];

	tmp			= 	ioread32(addr);
	tmp_r 		= 	ioread32(addr_rd);

	tmp  = tmp -tmp_r;
	if (tmp != 0) return TEST_ERROR;
	}
	if (tmp == ARINC_OK) {
	 rumboot_printf("ARINC LOOPBACK test OK \n");
	 return TEST_OK;
	}
    else {
			rumboot_printf("READ_etalon_adr=0x%x\n", addr);
			rumboot_printf("READ_etalon_DATA=0x%x\n", tmp);
			rumboot_printf("READ_adr=0x%x\n", addr_rd);
			rumboot_printf("READ_DATA=0x%x\n", tmp_r);
			rumboot_printf("ARINC test ERROR!\n");
			return TEST_ERROR; }
}

