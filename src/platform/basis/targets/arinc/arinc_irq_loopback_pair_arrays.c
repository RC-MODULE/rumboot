//-----------------------------------------------------------------------------
//  This program is for checking ARINC429 transactions.
// 
//  Test includes:
//  - ARINC429 transmitter and receiver transition through the external loopback
//	-  Receiver accept "even" and "odd" short array awithout checking labels 
//  - configured  frequency =100KHz
//	- received arrays are compared with ethalon values	
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

static const int32_t tx_array_double[] = {
	0x10818283,
	0x20414243,
	0x40212223,
	0x80111213,
	0xF8090a0B,
	0xF4152637,
	0xF2132456,
	0xF1122334,
	0xF0112233,
	0x1F8091AB,
	0x7F90A1B2,
	0x2F90A1B2,
	0x1090A1B2,
	0x1F90A1B2,
	0x0790A1B2,
	0x0B90A1B2,
	0x0FFEFDFC
};
void arinc_init (){

	uint32_t receiver_number = 0x0;
	uint32_t init_axi_mst;
	uint32_t size;
	init_axi_mst =0x0;
	size   = 0x2;
	
//-------------------------------------------------------------------------------------
//  Set parameters for DMA exchange loopback  connected on chip pins in testbench
//-------------------------------------------------------------------------------------
	
	iowrite32(receiver_number,(ARINC_BASE + RNUM_RX)); //receiver ch number			
	iowrite32(size,(ARINC_BASE + SZ_E_TX));    	// dma rd channel size	
	iowrite32(size,(ARINC_BASE + SZ_E_RX));    	// dma wr channel size
	
	iowrite32(size,(ARINC_BASE + SZ_O_TX));    	// dma rd channel size	
	iowrite32(size,(ARINC_BASE + SZ_O_RX));    	// dma wr channel size
	
	iowrite32(init_axi_mst,(ARINC_BASE + AXI_CTRL)); //AXI parameters
	iowrite32(0x1,(ARINC_BASE + INT_TX));			 //INT TX
	iowrite32(0x1,(ARINC_BASE + INT_RX));			 //INT RX
	iowrite32(0x1,(ARINC_BASE + INT_LONG));		//INT level
}

int arinc_status_check(uint32_t status_x_tx, uint32_t status_x_rx) {

	uint32_t tmp_r= -1;
	int cnt=0;
	uint32_t tmp; 
	while (tmp_r != 0x00000002) {

	tmp = ioread32(ARINC_BASE + status_x_tx);
	tmp_r = 0x07FFFFFF & tmp;
	if (++cnt == ARINC_ATTEMPT) {
            rumboot_printf("No end exchange!\n");
            return TEST_ERROR;
		}	
	}

    cnt =0;
	tmp_r = -1;
	while (tmp_r != status_success_bit) {
	tmp = ioread32(ARINC_BASE + status_x_rx);
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
	uint32_t tmp = -1;
	uint32_t enable;
	//int cnt=0;
	int i;
	enable = 0x10001;
	uint32_t tmp_r =-1;

    uint32_t addr;
	uint32_t addr_rd;

	 if (tmp !=0) {return -1;}
	rumboot_printf("DDR0_BASE=0x%x\n", DDR0_BASE);
	uint32_t *tx_mem = rumboot_malloc_from_heap_aligned(0, sizeof(tx_array32), 8);
	uint32_t *trg = (uint32_t *) tx_mem;
	memcpy(trg, tx_array32,sizeof(tx_array32));
	
	 uint32_t *rx_mem = rumboot_malloc_from_heap_aligned(0, sizeof(tx_array_double), 8);	

	uint32_t *tx_mem_double = rumboot_malloc_from_heap_aligned(0, sizeof(tx_array_double), 8);
	uint32_t *trg_double = (uint32_t *) tx_mem_double;
	 memcpy(trg_double, tx_array_double,sizeof(tx_array_double));
	
	 uint32_t *rx_mem_double = rumboot_malloc_from_heap_aligned(0, sizeof(tx_array_double), 8);	

	iowrite32((uint32_t) &tx_mem[0],(ARINC_BASE + AG_E_TX));    	// dma rd channel memory address	
	iowrite32((uint32_t) &rx_mem[0],(ARINC_BASE + AG_E_RX));    	// dma wr channel memory address
	iowrite32((uint32_t) &tx_mem_double[0],(ARINC_BASE + AG_O_TX)); // dma rd channel memory address	
	iowrite32((uint32_t) &rx_mem_double[0],(ARINC_BASE + AG_O_RX)); // dma wr channel memory address
		
    arinc_init();
	
    iowrite32(enable,ARINC_BASE + CHANNEL_EN); // run transaction
	rumboot_printf("ARINC START CH0\n");
	
	
	//check setting of the end of transaction delivery
   tmp = arinc_status_check(STAT_E_TX, STAT_E_RX);
	if (tmp == TEST_ERROR)  {
		 rumboot_printf(" TEST_ERROR\n");
		 return TEST_ERROR;}
	tmp_r = -1;
	
	for (i = 0; i< 2 ; i++)
	{
	addr 		=(uint32_t)	&tx_mem[i];
	addr_rd 	=(uint32_t)	&rx_mem[i];

	tmp			= 	ioread32(addr);
	tmp_r 		= 	ioread32(addr_rd);

	tmp  = tmp -tmp_r;
	if (tmp != 0)
	{
		rumboot_printf("READ_etalon_adr=0x%x\n", addr);
		rumboot_printf("READ_etalon_DATA=0x%x\n", tmp);
		rumboot_printf("READ_adr=0x%x\n", addr_rd);
		rumboot_printf("READ_DATA=0x%x\n", tmp_r);
	    rumboot_printf("ARINC test ERROR!\n");
			return TEST_ERROR; }		
	}

			
	tmp_r = -1;

	tmp = arinc_status_check(STAT_O_TX, STAT_O_RX);
	if (tmp == TEST_ERROR)  {
		 rumboot_printf(" TEST_ERROR\n");
		 return TEST_ERROR;}

    iowrite32(enable,ARINC_BASE + CHANNEL_DIS); // stop transmitter
	
	for (i = 0; i< 2 ; i++)
	{
	addr 		=(uint32_t)	&tx_mem_double[i];
	addr_rd 	=(uint32_t)	&rx_mem_double[i];

	tmp			= 	ioread32(addr);
	tmp_r 		= 	ioread32(addr_rd);
	tmp  = tmp -tmp_r;
	
	if (tmp != 0)
	{
		rumboot_printf("READ_etalon_adr_o=0x%x\n", addr);
		rumboot_printf("READ_etalon_DATA_o=0x%x\n", tmp);
		rumboot_printf("READ_adr_o=0x%x\n", addr_rd);
		rumboot_printf("READ_DATA_o=0x%x\n", tmp_r);
	    rumboot_printf("ARINC test ERROR!\n");

			return TEST_ERROR; }		
	}
	
	if (tmp == ARINC_OK) {
	 rumboot_printf("ARINC LOOPBACK test OK \n");
	 return TEST_OK;
	}
    else {
	      rumboot_printf("ARINC test ERROR!\n");
			return TEST_ERROR; }
}	
	


