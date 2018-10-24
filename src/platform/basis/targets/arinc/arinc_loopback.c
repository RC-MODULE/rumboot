//-----------------------------------------------------------------------------
//  This program is for checking ARINC429 transactions.
// 
//  Test includes:
//  - ARINC429 transmitters and receivers delivery through the external loopback
//	-  Receiver accept short arrays awithout checking labels 
//  - configured  frequency =100KHz
//	- received array is compared with ethalon values	
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
        0xDF90A1B2,
        0xE090A1B2,
        0xEF90A1B2,
        0xF790A1B2,
        0xFB90A1B2,
        0xFFFEFDFC
};

void arinc_init_one (){
	uint32_t receiver_number;

	uint32_t  init_axi_mst;
	uint32_t size;

	init_axi_mst =0x0;
	size   = 0x2;
	receiver_number = 0x0;
//-------------------------------------------------------------------------------------
//  Set parameters for DMA exchange loopback  connected on chip pins in testbench
//-------------------------------------------------------------------------------------
	iowrite32(receiver_number,(ARINC_BASE + RNUM_RX)); //receiver ch number
	//iowrite32(tx_mem,(ARINC_BASE + AG_E_TX));    	// dma rd channel memory address
	iowrite32(size,(ARINC_BASE + SZ_E_TX));    	// dma rd channel size
	//iowrite32(rx_mem,(ARINC_BASE + AG_E_RX));    	// dma wr channel memory address
	iowrite32(size,(ARINC_BASE + SZ_E_RX));    	// dma wr channel size
	iowrite32(init_axi_mst,(ARINC_BASE + AXI_CTRL)); //AXI parameters
//--------------------------------------------------------------------------------------
}
int arinc_status_check() {

	uint32_t tmp_r= -1;
	int cnt=0;
	uint32_t tmp; 
	
	while (tmp_r != 0x00000002) {
	tmp = ioread32(ARINC_BASE + STAT_E_TX);
	tmp_r = 0x07FFFFFF & tmp;
  // rumboot_printf("ARINC SIZE=0x%x\n", tmp); //check status
	}
	//iowrite32(enable,ARINC_BASE + CHANNEL_DIS); // stop transmitter

	tmp_r =-1;
	while (tmp_r != status_success_bit) {
	tmp = ioread32(ARINC_BASE + STAT_E_RX);
	//rumboot_printf("ARINC STATUS=0x%x\n", tmp); //check status
	tmp_r = tmp & status_success_bit;
	if (++cnt == ARINC_ATTEMPT) {
            rumboot_printf("No end exchange!\n");
            return TEST_ERROR;
		}
	}
 return TEST_OK;						
}
int main()
{
	int tmp = -1;
	uint32_t enable;
	int i;
	enable =0x10001;
	int32_t tmp_r =-1;
	
	uint32_t addr;
	uint32_t addr_rd;
	
	rumboot_printf("copy %d bytes\n", sizeof(tx_array32));  //transmit
	uint32_t *tx_mem = rumboot_malloc_from_heap_aligned(0, sizeof(tx_array32), 4);
	uint32_t *trg = (uint32_t *) tx_mem;
	memcpy(trg, tx_array32,sizeof(tx_array32));
	 
	uint32_t *rx_mem = rumboot_malloc_from_heap_aligned(0, sizeof(tx_array32), 4);
	
	iowrite32((uint32_t) &tx_mem[0],(ARINC_BASE + AG_E_TX)); 	// dma rd channel memory address
	iowrite32((uint32_t) &rx_mem[0],(ARINC_BASE + AG_E_RX));	// dma wr channel memory address

	arinc_init_one();
	
	iowrite32(enable,ARINC_BASE + CHANNEL_EN); // run transaction
	iowrite32(enable,ARINC_BASE + CHANNEL_DIS); // stop transmitter
	
	rumboot_printf("ARINC START CH0\n");
	
	//check setting of the end of transaction delivery
	tmp = arinc_status_check(STAT_E_TX, STAT_E_RX);
	if (tmp == TEST_ERROR)  {
		 rumboot_printf(" TEST_ERROR\n");
		 return TEST_ERROR;}

	for (i = 0; i< 2 ; i++)

	{
	addr 		=	(uint32_t)	&tx_mem[i];
	addr_rd 	=	(uint32_t)	&rx_mem[i];

	tmp			= 	ioread32(addr);
	tmp_r 		= 	ioread32(addr_rd);

	tmp  = tmp -tmp_r;
	if (tmp != 0) return TEST_ERROR;
	}
	if (tmp == TEST_OK) {
	 rumboot_printf("ARINC LOOPBACK test OK \n");
	 return TEST_OK;
	}
	else {
	      rumboot_printf("A test ERROR!\n");
			return TEST_ERROR; }
}
