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
#include <devices/arinc.h>
#include <rumboot/irq.h>
#include <platform/devices.h>

#define tx_mem				0x40000
#define rx_mem				0x50000

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

void arinc_init (uint32_t arinc_base_addr){
	uint32_t receiver_number;
	//uint32_t tx_mem;
	//uint32_t rx_mem;
	//uint32_t rx_data;

	uint32_t  init_axi_mst;
	uint32_t size;

	init_axi_mst =0x0;
	size   = 0x2;//0x10;

	//void *tx_mem =rumboot_malloc(8192);
	receiver_number = 0x0;
//-------------------------------------------------
//  Create  array for reading by transmitter
//-------------------------------------------------
	//memcpy (tx_mem,tx_array32,size);
//		for (i = 0; i< (sizeof(tx_array32) / 4); i++)
//	{
//	addr = tx_mem +	i*4;
//	iowrite32((tx_array32[i]), addr);//tx_array
//		}


//-------------------------------------------------------------------------------------
//  Set parameters for DMA exchange loopback  connected on chip pins in testbench
//-------------------------------------------------------------------------------------
	//iowrite32(receiver_number,(arinc_base_addr + SELF_RX)); //receiver ch number
	iowrite32(receiver_number,(arinc_base_addr + RNUM_RX)); //receiver ch number
	iowrite32(tx_mem,(arinc_base_addr + AG_E_TX));    	// dma rd channel memory address
	iowrite32(size,(arinc_base_addr + SZ_E_TX));    	// dma rd channel size
	iowrite32(rx_mem,(arinc_base_addr + AG_E_RX));    	// dma wr channel memory address
	iowrite32(size,(arinc_base_addr + SZ_E_RX));    	// dma wr channel size
	iowrite32(init_axi_mst,(arinc_base_addr + AXI_CTRL)); //AXI parameters
//--------------------------------------------------------------------------------------
}

int main()
{
	int tmp = -1;
	uint32_t enable;
	int cnt;
	int i;
	enable =0x10001;
	int32_t tmp_r =-1;
	cnt = 0;
    uint32_t addr;
	uint32_t addr_rd;
	uint32_t *trg = (uint32_t *) 0x40000;

	rumboot_printf("copy %d bytes\n", sizeof(tx_array32));  //transmi
	memcpy(trg, tx_array32,sizeof(tx_array32));

	arinc_init(ARINC_BASE);
    iowrite32(enable,ARINC_BASE + CHANNEL_EN); // run transaction
	rumboot_printf("ARINC START CH0\n");
	//iowrite32(enable,ARINC_BASE + CHANNEL_DIS); // stop transaction
	//check setting of the end of transaction delivery
	while (tmp_r != 0x00000002) {

	tmp = ioread32(ARINC_BASE + STAT_E_TX);
	tmp_r = 0x07FFFFFF & tmp;
   // rumboot_printf("ARINC SIZE=0x%x\n", tmp); //check status
	}
	iowrite32(enable,ARINC_BASE + CHANNEL_DIS); // stop transmitter !!!!


	while (tmp_r != status_success_bit) {
	tmp = ioread32(ARINC_BASE + STAT_E_RX);
	//rumboot_printf("ARINC STATUS=0x%x\n", tmp); //check status
	tmp_r = tmp & status_success_bit;
	if (++cnt == ARINC_ATTEMPT) {
            rumboot_printf("No end exchange!\n");
           // return ARINC_FAILED;
		}
	}

	for (i = 0; i< 2 ; i++)

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
	if (tmp != 0) return TEST_ERROR;
	}
	if (tmp == ARINC_OK) {
	 rumboot_printf("ARINC LOOPBACK test OK \n");
	 return TEST_OK;
	}
    else {
	      rumboot_printf("A test ERROR!\n");
			return TEST_ERROR; }
}
