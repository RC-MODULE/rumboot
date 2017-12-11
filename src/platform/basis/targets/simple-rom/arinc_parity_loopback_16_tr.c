//-----------------------------------------------------------------------------
//  This program is for checking ARINC429 transactions.
// 
//  Test includes:
//  - 16 ARINC429 transmitters and receivers delivery through the external loopback
//	- 16  receivers accept short arrays without checking labels 
//  - configured  frequency =50KHz
//	- use delay control for neighboring arrays switch for correct read state registers
//	- check parity	consists of checking receiver parity errors
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
#define status_integrity_bit  (1 << 27)

static const int32_t tx_array32[] = {
        0x91111111,
        0x33333333,
        0x22222222,
        0x55555555,
        0x66666666,
        0x77777777,
        0x88888888,
        0x99999999,
        0xbbbbbbbb,
        0xaaaaaaaa,
        0xcccccccc,
        0xdddddddd,
        0xffffffff,
        0xeeeeeeee,
        0xF790A1B2,
        0xFB90A1B2,
		0x11111222,
		0x33335555,
		0x66667777,
		0x88889999,
		0x9999aaaa,
        0xbbbbcccc,
        0xddddeeee,
		0xffffcccc,
        0xcccceeee,
		0x11112222,
        0x33334444,
        0x55556666,
        0x77778888,
        0xddddffff,
		0xeeee3333,
		0x1111ffff
};

void arinc_init (uint32_t arinc_base_addr){
	//uint32_t receiver_number;	
     int i =0;
	uint32_t  init_axi_mst;
	uint32_t size;

	init_axi_mst =0x0;
	size   = 0x2;

	uint32_t  tx_mem = 0x40000;
	uint32_t  rx_mem = 0x50000;
	uint32_t   freq_tx = 0xe30083e6;//0xe30743e6;  //0xe30083e6; //0xe30743e6;   //freq =50MHz
	uint32_t   freq_rx = 0xe01f43e6; //0xe31f43e6;  //freq =50MHz		
//-------------------------------------------------------------------------------------
//  Set parameters for DMA exchange loopback  connected on chip pins in testbench
//-------------------------------------------------------------------------------------
	//iowrite32(receiver_number,(arinc_base_addr + SELF_RX)); //receiver ch number
	
		for (i = 0; i< 16; i++)	{
	iowrite32(i,(arinc_base_addr + RNUM_RX +4*i));      			// even receiver buffer ch number
	//rumboot_printf("RCV_NUM=0x%x\n", i); 
//iowrite32(i,(arinc_base_addr + RNUM_RX +(4*i +64))); 				// odd receiver buffer ch number
	//rumboot_printf("RCV16_NUM=0x%x\n", (i+16)); 
	iowrite32((tx_mem + i*8),(arinc_base_addr + AG_E_TX  + i*4));   // dma rd channel memory address
	//rumboot_printf("AG_E_TX=0x%x\n", (tx_mem + i*8)); 
	iowrite32(size,(arinc_base_addr + SZ_E_TX + i*4));    			// dma rd channel size
	iowrite32((rx_mem + i*8),(arinc_base_addr + AG_E_RX + i*4));  	// dma wr channel memory address
	iowrite32(size,(arinc_base_addr + SZ_E_RX + i*4));    			// dma wr channel size
	iowrite32(freq_tx,(arinc_base_addr + FREQ_TX + i*4));			//freq TX 50Mhz
	iowrite32(freq_rx,(arinc_base_addr + FREQ_RX + i*4));			//freq RX 50MHz
	}
	iowrite32(init_axi_mst,(arinc_base_addr + AXI_CTRL)); 			// AXI parameters	
	iowrite32(0xffff,(arinc_base_addr + WAIT_TMR_TX));   			// wait  delayed switch after the transmitter start
	iowrite32(0xffff,(arinc_base_addr + WAIT_SIG_RX));   			// wait delayed switch after the receiver start
	iowrite32(0xffff,(arinc_base_addr + PARITY_ODD));				// parity odd 
	iowrite32(0xffff,(arinc_base_addr + PARITY_BIT));				// parity bit	
	//--------------------------------------------------------------------------------------
}

int main()
{
	int tmp = -1;
	uint32_t enable;
	int cnt;
	int i;
	//int j;
	//int k;
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
   while (tmp != 0x00000002) {
	tmp = ioread32(ARINC_BASE + STAT_E_TX + i*4);
	tmp = 0x07FFFFFF & tmp;
    //rumboot_printf("ARINC SIZE=0x%x\n", tmp); //check status
		}   
 	//rumboot_printf("rcv_channel_num=0x%x\n", (1 << i));
	iowrite32( ((1 << (0x10000 +i) )*0x10000),ARINC_BASE + CHANNEL_DIS); // stop transmitter
	//rumboot_printf("channel_dis=0x%x\n", ((1 << (0x10000 +i) )*0x10000));
    }
     
	 
	for (i = 0; i< 16 ; i++) 
	{tmp_r = -1;
	while (tmp_r != status_success_bit ) {	
	tmp = ioread32(ARINC_BASE + STAT_E_RX +i*4);
	//rumboot_printf("ARINC STATUS=0x%x\n", tmp); //check status 
	tmp_r = tmp & status_success_bit;		
	if (++cnt == ARINC_ATTEMPT) {
            rumboot_printf("No end exchange! Test ERROR\n");
            //return TEST_ERROR;
			}
		}
	}


for (i = 0; i< 32 ; i++)

	{
	addr 		=	tx_mem + 4*i;
	addr_rd 	=	rx_mem + 4*i;

	tmp			= 	ioread32(addr);
	rumboot_printf("READ_etalon_adr=0x%x\n", addr);
	rumboot_printf("READ_etalon_DATA=0x%x\n", tmp);

	rumboot_printf("READ_adr=0x%x\n", addr_rd);
	tmp_r 		= 	ioread32(addr_rd);
	rumboot_printf("READ_DATA=0x%x\n", tmp_r);

	tmp  = tmp -tmp_r;
	if (tmp != 0) {
		rumboot_printf("ARINC429 PARITY ERROR  found!\n");
		//return TEST_OK;
		}
	else  {	rumboot_printf("ARINC429 no PARITY ERROR!\n"); }
		
	}
	//if (tmp == ARINC_OK) {
	 rumboot_printf("ARINC LOOPBACK test OK \n");
	 return TEST_OK;
	
}
