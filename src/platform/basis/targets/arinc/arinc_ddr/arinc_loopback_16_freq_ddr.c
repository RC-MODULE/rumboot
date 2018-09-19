//-----------------------------------------------------------------------------
//  This program is for checking ARINC429 transactions.
// 
//  Test includes:
//  - 16 ARINC429 transmitters and receivers delivery through the external loopback
//	- 16  receivers accept short arrays without checking labels 
//  - configured  frequency =12,5KHz / 50KHz /100KHz
//	- heap type 0,1,2,3,4,5
//	- use delay control for neighboring arrays switch for correct read state registers
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

void arinc_init_freq(uint32_t tx_freq, uint32_t rx_freq) {
     int i =0;
	uint32_t  init_axi_mst=0x0;
	uint32_t size = 0x2;

//-------------------------------------------------------------------------------------
//  Set parameters for DMA exchange loopback  connected on chip pins in testbench
//-------------------------------------------------------------------------------------	
		for (i = 0; i< 16; i++)	{
	iowrite32(i,(ARINC_BASE + RNUM_RX +4*i));      			// even receiver buffer ch number
	iowrite32(size,(ARINC_BASE + SZ_E_TX + i*4));    				// dma rd channel size
	iowrite32(size,(ARINC_BASE + SZ_E_RX + i*4));    				// dma wr channel size
	iowrite32(tx_freq,(ARINC_BASE + FREQ_TX + i*4));			//freq TX 100Mhz
	iowrite32(rx_freq,(ARINC_BASE + FREQ_RX + i*4));			//freq RX 100MHz	
		}
	iowrite32(init_axi_mst,(ARINC_BASE + AXI_CTRL)); 			// AXI parameters	
	iowrite32(0xffff,(ARINC_BASE + WAIT_TMR_TX));   			// wait  delayed switch after the transmitter start
	iowrite32(0xffff,(ARINC_BASE + WAIT_SIG_RX));   			// wait delayed switch after the receiver start
	}


int arinc_status_check16(uint32_t status_x_tx, uint32_t status_x_rx) {

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
int main()
{
    int i =0;
	
	//uint32_t size = 0x2;
	int tmp = -1;
	uint32_t tmp_r =-1;	
	uint32_t enable=0xffffffff;
	uint32_t addr;
	uint32_t addr_rd;
	
	
	//rumboot_printf("copy %d bytes\n", sizeof(tx_array32));  //transmit
	uint32_t *tx_mem = rumboot_malloc_from_heap_aligned(heap_0, 128, 4);
	uint32_t *rx_mem = rumboot_malloc_from_heap_aligned(heap_1,	128, 4);	
	
		for (i = 0; i< 16; i++)	{
			//rumboot_printf("tx_mem=0x%x\n", ((uint32_t) &tx_mem[2*i]));
			//rumboot_printf("rx_mem=0x%x\n", ((uint32_t) &rx_mem[2*i]));
			
			iowrite32((uint32_t) &tx_mem[2*i],(ARINC_BASE + AG_E_TX + i*4)); 	// dma rd channel memory address except DDR
			iowrite32((uint32_t) &rx_mem[2*i],(ARINC_BASE + AG_E_RX + i*4)); 	// dma wr channel memory address

	}
   
	
	for (i = 0; i< 32; i++)	{
	//rumboot_printf("tx_mem=0x%x\n", ((uint32_t) &tx_mem[i])); 
	//rumboot_printf("data_tx_mem=0x%x\n", (1 << i)); 
	iowrite32(((1 << i)),((uint32_t) &tx_mem[i])); 	// dma wr channel memory address
	
	}

	arinc_init_freq(TX_FREQ, RX_FREQ);	
    iowrite32(enable,ARINC_BASE + CHANNEL_EN); // run transaction
	rumboot_printf("ARINC START \n");
	 
	tmp = arinc_status_check16(STAT_E_TX, STAT_E_RX);
	if (tmp == TEST_ERROR)  {
		 rumboot_printf(" TEST_ERROR\n");
		 return TEST_ERROR;}


		 
for (i = 0; i< 32 ; i++)

	{
	addr 		=	(uint32_t)	&tx_mem[i];
	addr_rd 	=	(uint32_t)	&rx_mem[i];

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