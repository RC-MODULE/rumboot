//-----------------------------------------------------------------------------
//  This program is for checking ARINC429 transactions.
// 
//  Test includes:
//  - 16 ARINC429 transmitters and receivers delivery through the external loopback
//	- 16  receivers accept short arrays without checking labels 
//  - configured  frequency =100KHz
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
#include <regs/regs_global_timers.h>

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

void arinc_init_freq(uint32_t tx_freq, uint32_t rx_freq) {
     int i =0;
	//uint32_t  init_axi_mst=0x0;
	uint32_t size = 0x2;
	rumboot_printf("===> %x %x\n", tx_freq, rx_freq);

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
	//iowrite32(init_axi_mst,(ARINC_BASE + AXI_CTRL)); 			// AXI parameters	
	//iowrite32(0xffff,(ARINC_BASE + WAIT_TMR_TX));   			// wait  delayed switch after the transmitter start
	//iowrite32(0xffff,(ARINC_BASE + WAIT_SIG_RX));   			// wait delayed switch after the receiver start		    
	}
    

int arinc_status_check16(uint32_t status_x_tx, uint32_t status_x_rx) {

	uint32_t tmp_r= -1;
	int cnt=0;
	uint32_t tmp; 
	//uint32_t frc_L;
	int i;
	for (i = 0; i< 16 ; i++)
	{
	rumboot_printf("ARINC CH=0x%x\n", i); 	
       while (tmp != 0x00000002) {

	tmp = ioread32(ARINC_BASE + STAT_E_TX + i*4);
	rumboot_printf("ARINC STAT_E_TX =0x%x\n", tmp);
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
	uint32_t frc_L;
	uint32_t tmp1 = -1;
	uint32_t  delta;
	rumboot_printf("copy %d bytes\n", sizeof(tx_array32));  //transmit
	uint32_t *tx_mem = rumboot_malloc_from_heap_aligned(heap_0, sizeof(tx_array32), 4);
	uint32_t *trg = (uint32_t *) tx_mem;
	memcpy(trg, tx_array32,sizeof(tx_array32));
	 
	uint32_t *rx_mem = rumboot_malloc_from_heap_aligned(heap_1, sizeof(tx_array32), 4);	
	

		for (i = 0; i< 16; i++)	{
			iowrite32((uint32_t) &tx_mem[2*i],(ARINC_BASE + AG_E_TX + i*4)); 	// dma rd channel memory address
			iowrite32((uint32_t) &rx_mem[2*i],(ARINC_BASE + AG_E_RX + i*4)); 	// dma wr channel memory address

	}
	arinc_init_freq(TX_FREQ, RX_FREQ);
	iowrite32(1,ARINC_BASE + TEST_FRC_TS); // test mode for FREE COUNTER	
    iowrite32(enable,ARINC_BASE + CHANNEL_EN); // run transaction
    frc_L = ioread32(GLOBAL_TIMERS + FREE_RUN_L); //begin frc count
	rumboot_printf("frc_L =0x%x\n", frc_L);
	rumboot_printf("ARINC START \n");
	
	iowrite32(0x1,(GLOBAL_TIMERS  + ENABLE));     //enable signal
	tmp = ioread32(GLOBAL_TIMERS + ENABLE);
	rumboot_printf("GLOBAL_TIMERS_ENABLE=0x%x\n", tmp);

	iowrite32(0x176 /*0x13E*/,(GLOBAL_TIMERS  + 0x100  + TMR_0_LIMIT));//interval= 1900 (0x16E)
	tmp = ioread32(GLOBAL_TIMERS + 0x100 + TMR_0_LIMIT);
	rumboot_printf("GLOBAL_TIMERS_TMR_0_LIMIT=0x%x\n", tmp);	

	iowrite32(0x10003,(GLOBAL_TIMERS + 0x100 + TMR_0_STATE));//one pass
	tmp = ioread32(GLOBAL_TIMERS + 0x100 + TMR_0_STATE);
	rumboot_printf("GLOBAL_TIMERS_TMR_0_STATE=0x%x\n", tmp);
		
	iowrite32(0x1,(GLOBAL_TIMERS + FIX_CMD)); //FIX Time of Free Run Counter
	
	 
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
	
	for (i = 0; i< 16 ; i++)
	{		
	tmp1 = ioread32(ARINC_BASE + TRF_E_TX + i*4);
	if (tmp1 <= frc_L) {
	rumboot_printf("frc_L=0x%x\n", frc_L);	
	rumboot_printf("ARINC TRF_E_TX=0x%x\n", tmp1);	
	return TEST_ERROR;}
	rumboot_printf("ARINC TRF0_E_TX =0x%x\n", tmp1);
	tmp = ioread32(ARINC_BASE + TLF_E_TX + i*4);
	if (tmp <= frc_L) {		
	rumboot_printf("ARINC TLF0_E_TX =0x%x\n", tmp);	
	return TEST_ERROR; }
	rumboot_printf("ARINC TLF0_E_TX =0x%x\n", tmp);
	
	tmp = ioread32(ARINC_BASE + TRL_E_TX + i*4);
	if (tmp <= frc_L) {
		rumboot_printf("ARINC TLF0_E_TX =0x%x\n", tmp);		
	return TEST_ERROR; }
	delta = tmp -tmp1;
	rumboot_printf("ARINC TRL0_E_TX =0x%x,delta=0x%x\n", tmp,delta);
	
	//tmp = ioread32(ARINC_BASE + TRF_O_TX + i*4);
	//rumboot_printf("ARINC TRF0_O_TX =0x%x\n", tmp);

	//tmp = ioread32(ARINC_BASE + TLF_O_TX + i*4);
	//rumboot_printf("ARINC TLF0_O_TX =0x%x\n", tmp);
	
	//tmp = ioread32(ARINC_BASE + TRL_O_TX + i*4);
	//rumboot_printf("ARINC TRL0_O_TX =0x%x\n", tmp);	
	
//--------------------------------------------------------
	tmp1 = ioread32(ARINC_BASE + TRF_E_RX + i*4);
	if (tmp1 <= frc_L) {
	rumboot_printf("ARINC TRF0_E_RX =0x%x\n", tmp1);
	return TEST_ERROR;}
	rumboot_printf("ARINC TRF0_E_RX =0x%x\n", tmp1);
 
	tmp = ioread32(ARINC_BASE + TLF_E_RX + i*4);
	if (tmp <= frc_L) return TEST_ERROR;
	rumboot_printf("ARINC TLF0_E_RX =0x%x\n", tmp);
	
	tmp = ioread32(ARINC_BASE + TRL_E_RX + i*4);
	if (tmp <= frc_L) {
	rumboot_printf("ARINC TRL0_E_RX =0x%x\n", tmp);	
	return TEST_ERROR;}	
	delta = tmp -tmp1;
	rumboot_printf("ARINC TRL0_E_RX =0x%x,delta=0x%x\n", tmp, delta);
	}
		
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
