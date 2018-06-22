//-----------------------------------------------------------------------------
//  This program is for checking ARINC429 transactions.
// 
//  Test includes:
//  - 16 ARINC429 transmitters and receivers delivery through the external loopback
//	- 16  receivers accept short arrays awithout checking labels 
//  - configured  frequency =100KHz
//	- use delay control for neighboring arrays switch for correct read state registers
//	- received arrays compared with ethalon values	
//    Test duration (RTL): < 
//-----------------------------------------------------------------------------#include <stdio.h>

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
	size   = 0x2;//0x10;

	uint32_t  tx_mem = 0x40000;
	uint32_t  rx_mem = 0x50000;
	uint32_t   freq_tx = 0xe30083e6;//freq =12,5KHz
	uint32_t   freq_rx = 0xe01f43e6;//freq =12,5KHz
//-------------------------------------------------------------------------------------
//  Set parameters for DMA exchange loopback  connected on chip pins in testbench
//-------------------------------------------------------------------------------------
	//iowrite32(receiver_number,(arinc_base_addr + SELF_RX)); //receiver ch number
	
		for (i = 0; i< 16; i++)	{
	iowrite32(i,(arinc_base_addr + RNUM_RX +4*i));      			// even receiver buffer ch number
	//rumboot_printf("RCV_NUM=0x%x\n", i); 
//iowrite32(i,(arinc_base_addr + RNUM_RX +(4*i +64))); 		// odd receiver buffer ch number
	//rumboot_printf("RCV16_NUM=0x%x\n", (i+16)); 
	iowrite32((tx_mem + i*8),(arinc_base_addr + AG_E_TX  + i*4));   // dma rd channel memory address
	//rumboot_printf("AG_E_TX=0x%x\n", (tx_mem + i*8)); 
	iowrite32(size,(arinc_base_addr + SZ_E_TX + i*4));    			// dma rd channel size
	iowrite32((rx_mem + i*8),(arinc_base_addr + AG_E_RX + i*4));  	// dma wr channel memory address
	iowrite32(size,(arinc_base_addr + SZ_E_RX + i*4));    			// dma wr channel size
	iowrite32(freq_tx,(arinc_base_addr + FREQ_TX + i*4));           //freq TX 50Mhz
	iowrite32(freq_rx,(arinc_base_addr + FREQ_RX + i*4));			//freq RX 50MHz
	}
	iowrite32(init_axi_mst,(arinc_base_addr + AXI_CTRL)); 			// AXI parameters
	iowrite32(0xffff,(arinc_base_addr + WAIT_TMR_TX));   			// wait  delayed switch after the transmitter start
	iowrite32(0xffff,(arinc_base_addr + WAIT_SIG_RX));   			// wait delayed switch after the receiver start
//--------------------------------------------------------------------------------------
}

int main()
{
	int tmp = -1;
	uint32_t enable;
	int cnt;
	int i;

	enable =0xffffffff;
	int32_t tmp_r =-1;
	cnt = 0;
    uint32_t addr;
	uint32_t addr_rd;
	uint32_t *trg = (uint32_t *) 0x40000;
    uint32_t  tx_mem = 0x40000;
	uint32_t  rx_mem = 0x50000;
	
	rumboot_printf("copy %d bytes\n", sizeof(tx_array32));  //transmit
	memcpy(trg, tx_array32,sizeof(tx_array32));

	arinc_init(ARINC_BASE);
    iowrite32(enable,ARINC_BASE + CHANNEL_EN); // run transaction
	rumboot_printf("ARINC START \n");
	//check setting of the end of transaction delivery
	for (i = 0; i< 16 ; i++)
	{tmp_r =-1;
	rumboot_printf("ARINC CH=0x%x\n", i); 	
       while (tmp_r != 0x00000002) {
	tmp = ioread32(ARINC_BASE + STAT_E_TX + i*4);
	tmp_r = 0x07FFFFFF & tmp;
    //rumboot_printf("ARINC SIZE=0x%x\n", tmp); //check status
		}   
 	//rumboot_printf("rcv_channel_num=0x%x\n", (1 << i));
	iowrite32( ((1 << (0x10000 +i) )*0x10000),ARINC_BASE + CHANNEL_DIS); // stop transmitter
	//rumboot_printf("channel_dis=0x%x\n", ((1 << (0x10000 +i) )*0x10000));
    }
     
	 
	for (i = 0; i< 16 ; i++) 
	{
	rumboot_printf("ARINC number=0x%x\n", i);
	cnt =0;	
	while (tmp_r != status_success_bit) {
	tmp = ioread32(ARINC_BASE + STAT_E_RX +i*4);
	//rumboot_printf("ARINC STATUS=0x%x\n", tmp); //check status
	tmp_r = tmp & status_success_bit;
	if (++cnt == ARINC_ATTEMPT) {
            rumboot_printf("No end exchange!\n");
           // return ARINC_FAILED;
			}
		}
	}


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

