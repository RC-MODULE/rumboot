#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/io.h>
#include <platform/defs.h>


#define CHANNEL_EN			0x0004
#define CHANNEL_DIS			0x0008
#define CHANNEL_RST			0x000C
#define PARITY_BIT			0x0010
#define PARITY_ODD			0x0014
#define TEST_EN				0x0018

#define WAIT_SIG_RX			0x001C
#define TMR_MASK_RX			0x0020
#define SW_SIG_RX			0x0024
#define WAIT_TMR_TX			0x0028
#define SW_SIG_TX			0x002C
#define LABEL_EN_RX			0x0030
#define AXI_CTRL			0x0034
#define FIFO_EN_RX			0x0038
#define LAST_WD_TMR			0x003C
#define INT_TX				0x0040
#define INT_RX				0x0044

#define AG_E_RX				0x1880
#define AG_O_RX				0x1900
#define SZ_E_RX				0x1980
#define SZ_O_RX				0x1a00


#define TRF_E_TX			0x1380
#define TLF_E_TX			0x1400
#define TRL_E_TX			0x1480
#define TRF_O_TX			0x1500
#define TLF_O_TX			0x1580
#define TRL_O_TX			0x1600

#define AG_E_TX				0x1080
#define AG_O_TX				0x1100
#define SZ_E_TX				0x1180
#define SZ_O_TX				0x1200

#define STAT_E_TX			0x1280
#define STAT_O_TX			0x1300

#define STAT_E_RX			0x1A80
#define STAT_O_RX			0x1B00
#define TRF_E_RX			0x1B80
#define TLF_E_RX			0x1C00
#define TRL_E_RX			0x1C80
#define TRF_O_RX			0x1D00
#define TLF_O_RX			0x1D80
#define TRL_O_RX			0x1E00
#define LABEL_RX			0x1E80
#define RNUM_RX				0x1F00

#define FREQ_TX				0x0440
#define FREQ_RX				0x0c40
#define SELF_RX				0x0C80


#define ARINC_OK			0
#define ARINC429_FAILED		1

#define TEST_OK				0
#define TEST_ERROR			1

#define arlock				0x0
#define arprot				0x0
#define arcache				0x0
#define awlock				0x0
#define awprot				0x0
#define awcache				0x0
#define status_success_bit  (1 << 31)

#define ARINC_ATTEMPT			10000
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

void arinc_init (uint32_t arinc_base_addr){
	//uint32_t receiver_number;	
     int i =0;
	uint32_t  init_axi_mst;
	uint32_t size;
   int j;
	init_axi_mst =0x0;
	size   = 0x2;	

	uint32_t  tx_mem = 0x40000;
	uint32_t  rx_mem = 0x50000;

//-------------------------------------------------------------------------------------
//  Set parameters for DMA exchange loopback  connected on chip pins in testbench
//-------------------------------------------------------------------------------------
	//iowrite32(receiver_number,(arinc_base_addr + SELF_RX)); //receiver ch number
	
		for (i = 0; i< 16; i++)	{    
	iowrite32(i,(arinc_base_addr + RNUM_RX +4*i));      			// even receiver buffer ch number
	//rumboot_printf("RCV_NUM=0x%x\n", i); 
	iowrite32((tx_mem + i*8),(arinc_base_addr + AG_E_TX  + i*4));   // dma rd channel memory address
	//rumboot_printf("AG_E_TX=0x%x\n", (tx_mem + i*8)); 
	iowrite32(size,(arinc_base_addr + SZ_E_TX + i*4));    			// dma rd channel size
	iowrite32((rx_mem + i*8),(arinc_base_addr + AG_E_RX + i*4));  	// dma wr channel memory address
	iowrite32(0x1,(arinc_base_addr + SZ_E_RX + i*4)); // dma wr channel size

	}
	iowrite32(init_axi_mst,(arinc_base_addr + AXI_CTRL)); 	//AXI parameters
	for (j = 0; j< 8; j++)	{ 								    // label fifo depth
	iowrite32(0x83838383, arinc_base_addr + LABEL_RX );
	iowrite32(0x23232323, arinc_base_addr + LABEL_RX +4);
	iowrite32(0x0B0B0B0B, arinc_base_addr + LABEL_RX +8);
	iowrite32(0x56565656, arinc_base_addr + LABEL_RX +12);
	
	iowrite32(0x33333333, arinc_base_addr + LABEL_RX + 16);
	iowrite32(0xB2B2B2B2, arinc_base_addr + LABEL_RX + 20);
	iowrite32(0xB0B0B0B0, arinc_base_addr + LABEL_RX + 24);
	iowrite32(0xB6B6B6B6, arinc_base_addr + LABEL_RX + 28);

	iowrite32(0xFCFCFCFC, arinc_base_addr + LABEL_RX + 32);
	iowrite32(0xFEFEFEFE, arinc_base_addr + LABEL_RX + 36);
	iowrite32(0x81818181, arinc_base_addr + LABEL_RX + 40);
	iowrite32(0x31313131, arinc_base_addr + LABEL_RX + 44);
	
	iowrite32(0x06060606, arinc_base_addr + LABEL_RX + 48);
	iowrite32(0x56565656, arinc_base_addr + LABEL_RX + 52);
	iowrite32(0x30303030, arinc_base_addr + LABEL_RX + 56);
	iowrite32(0x62626262, arinc_base_addr + LABEL_RX + 60);	
		}	
	iowrite32(0xFFFF,(arinc_base_addr  + LABEL_EN_RX));		// receiver LABEL enable   
   	iowrite32(init_axi_mst,(arinc_base_addr + AXI_CTRL)); 			// AXI parameters
	iowrite32(0xffff,(arinc_base_addr + WAIT_TMR_TX));   			// wait  after the first  transmitter start
   }	
//--------------------------------------------------------------------------------------


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
	rumboot_printf("ARINC LABEL LOOPBACK START \n");
	//check setting of the end of transaction delivery
	for (i = 0; i< 16 ; i++)
	{
	//rumboot_printf("ARINC CH=0x%x\n", i); 	
	while (tmp != 0x00000002) {

	tmp = ioread32(ARINC_BASE + STAT_E_TX + i*4);
	tmp = 0x07FFFFFF & tmp;
	//rumboot_printf("i=0x%x\n", i); 
    //rumboot_printf("ARINC STAT=0x%x\n", tmp); //check status
		}	
	iowrite32( ((1 << (0x10000 +i) )*0x10000),ARINC_BASE + CHANNEL_DIS); // stop transmitter
	//rumboot_printf("channel_dis=0x%x\n", ((1 << (0x10000 +i) )*0x10000)); 	
    }
	rumboot_printf("ARINC check status RX\n");      
	for (i = 0; i< 16 ; i++) 
	{
	while (tmp_r != status_success_bit) {
	tmp = ioread32(ARINC_BASE + STAT_E_RX +i*4);	
	//rumboot_printf("ARINC STATUS=0x%x\n", tmp); //check status
	//rumboot_printf("i=0x%x\n", i); //check status
	tmp_r = tmp & status_success_bit;
	//rumboot_printf("ARINC tmp_r=0x%x\n", tmp_r); //check status
	if (++cnt == ARINC_ATTEMPT) {
            rumboot_printf("No end exchange! test ERROR\n");
            return TEST_ERROR;
			}
		}
	//rumboot_printf("i=0x%x\n", i); //check status	
	}
	rumboot_printf("ARINC Compare array \n");

for (i = 0; i< 16 ; i++)

	{
	addr 		=	tx_mem + 8*i;
	addr_rd 	=	rx_mem + 8*i;

	tmp			= 	ioread32(addr);
	//rumboot_printf("READ_etalon_adr=0x%x\n", addr);
	//rumboot_printf("READ_etalon_DATA=0x%x\n", tmp);

	//rumboot_printf("READ_adr=0x%x\n", addr_rd);
	tmp_r 		= 	ioread32(addr_rd);
	//rumboot_printf("READ_DATA=0x%x\n", tmp_r);

	tmp  = tmp -tmp_r;
	if (tmp != 0) {
		rumboot_printf("ARINC429 LOOPBACK test ERROR \n");
		return TEST_ERROR;
		}
	}
	
	//if (tmp == ARINC_OK) {
	 rumboot_printf("ARINC429 LOOPBACK test OK \n");
	 return TEST_OK;

			
}

