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
#include <rumboot/rumboot.h>
#include <rumboot/platform.h>
#include <regs/arinc.h>
#include <rumboot/irq.h>
#include <platform/devices.h>


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

void arinc_init_par (){

	int i =0;
	uint32_t  init_axi_mst;
	uint32_t size;

	init_axi_mst =0x0;
	size   = 0x2;

	uint32_t   freq_tx = 0xe30083e6;//0xe30743e6;  //0xe30083e6; //0xe30743e6;   //freq =50MHz
	uint32_t   freq_rx = 0xe01f43e6; //0xe31f43e6;  //freq =50MHz		
//-------------------------------------------------------------------------------------
//  Set parameters for DMA exchange loopback  connected on chip pins in testbench
//-------------------------------------------------------------------------------------

	
		for (i = 0; i< 16; i++)	{
	iowrite32(i,(ARINC_BASE + RNUM_RX +4*i));      			// even receiver buffer ch number
	iowrite32(size,(ARINC_BASE + SZ_E_TX + i*4));    			// dma rd channel size
	iowrite32(size,(ARINC_BASE + SZ_E_RX + i*4));    			// dma wr channel size
	iowrite32(freq_tx,(ARINC_BASE + FREQ_TX + i*4));			//freq TX 50Mhz
	iowrite32(freq_rx,(ARINC_BASE + FREQ_RX + i*4));			//freq RX 50MHz
	}
	iowrite32(init_axi_mst,(ARINC_BASE + AXI_CTRL)); 			// AXI parameters	
	iowrite32(0xffff,(ARINC_BASE + WAIT_TMR_TX));   			// wait  delayed switch after the transmitter start
	iowrite32(0xffff,(ARINC_BASE + WAIT_SIG_RX));   			// wait delayed switch after the receiver start
	iowrite32(0xffff,(ARINC_BASE + PARITY_ODD));				// parity odd 
	iowrite32(0xffff,(ARINC_BASE + PARITY_BIT));				// parity bit	
	//--------------------------------------------------------------------------------------
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
	
	arinc_init_par();
	iowrite32(enable,ARINC_BASE + CHANNEL_EN); // run transaction
	rumboot_printf("ARINC START \n");
	
	//check setting of the end of transaction delivery
	
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
		 