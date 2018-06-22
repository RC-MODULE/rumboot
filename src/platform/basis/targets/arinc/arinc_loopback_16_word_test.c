//-----------------------------------------------------------------------------
//  This program is for checking ARINC429 transactions.
// 
//  Test includes:
//  - 16 ARINC429 transmitters and receivers delivery through the external loopback
//	- 16  receivers accept word arrays without checking labels
//	- TEST ENABLE mode is turned ON
//  - external loop is provided by  multiplexing TESTA&B and TXA&B with TESTA and TESTB used as inputs
//  - for every exchange. The received array is choosed sequentally by addressing only one receiver
//  - configured  frequency =100KHz by default value
//	- used delay control for neighboring arrays switch for correct read state registers
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
		0xFF0EFEFC,
		0xFF1EFDFC,
		0xF01973FC,
		0xF11873FC,		
		0x70718283,
        0x60413253,
        0x50212223,
		0x10111213,
        0x41045a06,
		0x04152637,
        0x02532456,
        0x05122334,
        0x50112233,
        0x7F8091A6,
		0x8F90A162,
		0xDF90A6B2
};

void arinc_init (uint32_t arinc_base_addr){
	//uint32_t receiver_number;	
     int i =0;
	uint32_t  init_axi_mst;
	uint32_t size;

	init_axi_mst =0x0;
	size   = 0x1;

	uint32_t  tx_mem = 0x40000;
	uint32_t  rx_mem = 0x50000;

//-------------------------------------------------------------------------------------
//  Set parameters for DMA exchange loopback  connected on chip pins in testbench
//-------------------------------------------------------------------------------------

		for (i = 0; i< 16; i++)	{
	iowrite32(i,(arinc_base_addr + RNUM_RX +4*i));      			// receiver buffer ch number
	iowrite32((tx_mem + i*4),(arinc_base_addr + AG_E_TX  + i*4));   // dma rd channel memory address

	iowrite32(size,(arinc_base_addr + SZ_E_TX + i*4));    			// dma rd channel size
	iowrite32((rx_mem + i*4),(arinc_base_addr + AG_E_RX + i*4));  	// dma wr channel memory address
	iowrite32(size,(arinc_base_addr + SZ_E_RX + i*4));    			// dma wr channel size
	}
	iowrite32(init_axi_mst,(arinc_base_addr + AXI_CTRL)); 			// AXI parameters
	iowrite32(0xffff,(arinc_base_addr + WAIT_TMR_TX));   			// wait  after the first  transmitter start
	iowrite32(0xffff,(arinc_base_addr + WAIT_SIG_RX));   			// wait delayed switch after the receiver start
//--------------------------------------------------------------------------------------
}

int main()
{
	int tmp = -1;

	int cnt;
	int i;

	int32_t tmp_r =-1;
	cnt = 0;
    uint32_t addr;
	uint32_t addr_rd;
	uint32_t *trg = (uint32_t *) 0x40000;
    uint32_t  tx_mem = 0x40000;
	uint32_t  rx_mem = 0x50000;
	uint32_t tst_en =0x10;
	
	rumboot_printf("copy %d bytes\n", sizeof(tx_array32));  //transmit
	memcpy(trg, tx_array32,sizeof(tx_array32));

	arinc_init(ARINC_BASE);
  
	rumboot_printf("ARINC START \n");
	//check setting of the end of transaction delivery
	for (i = 0; i< 16 ; i++)
	{
	iowrite32((tst_en +i),(ARINC_BASE + TEST_EN)); //test mode	
	iowrite32( (1 << i) + (1 << (16 +i)),ARINC_BASE + CHANNEL_EN); // run transaction	
	rumboot_printf("ARINC CH=0x%x\n", i); 
	tmp_r = -1;
	cnt = 0;
       while (tmp_r != 0x00000001) {
	tmp = ioread32(ARINC_BASE + STAT_E_TX + i*4);
	tmp_r = 0x2FFFFFFF & tmp;
		if (++cnt == ARINC_ATTEMPT) {
		rumboot_printf("ARINC STATUS_TR=0x%x\n", tmp); //check status
		rumboot_printf("ARINC address_tr =0x%x\n", (STAT_E_TX +i*4)); //check status		  
		rumboot_printf("No end Transmit exchange!\n");
		rumboot_printf("ARINC test ERROR!\n");
         return TEST_ERROR;
			}
	
    //rumboot_printf("ARINC SIZE=0x%x\n", tmp); //check status
		}   
 	//rumboot_printf("rcv_channel_num=0x%x\n", (1 << i));
	iowrite32(( (1 << i) + (1 << (16 +i)) ),ARINC_BASE + CHANNEL_DIS); // stop transmitter
	//rumboot_printf("channel_dis=0x%x\n", ((1 << (0x10000 +i) )*0x10000));
     
	tmp_r = -1;	
	cnt =0;
	while (tmp_r != 0x00000001) {
	tmp = ioread32(ARINC_BASE + STAT_E_RX +i*4);
	//rumboot_printf("ARINC STATUS=0x%x\n", tmp); //check status
	tmp_r = tmp & 0x2FFFFFFF ;
	if (++cnt == ARINC_ATTEMPT) {
		rumboot_printf("ARINC STATUS_RC=0x%x\n", tmp); //check status
		rumboot_printf("ARINC address_rc =0x%x\n", (STAT_E_RX +i*4)); //check status
		rumboot_printf("ARINC number =0x%x\n", i); //check status		  
		rumboot_printf("No end exchange!\n");
		rumboot_printf("ARINC test ERROR!\n");
           return TEST_ERROR;
			}
		}
	}


for (i = 0; i< 16 ; i++)

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
	      rumboot_printf("ARINC test ERROR!\n");
			return TEST_ERROR; }
}

