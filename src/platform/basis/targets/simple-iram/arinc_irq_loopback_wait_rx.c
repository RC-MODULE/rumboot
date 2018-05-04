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
#include <rumboot/io.h>
#include <rumboot/testsuite.h>
#include <rumboot/platform.h>
#include <platform/interrupts.h>

#define tx_mem				0x40000
#define rx_mem				0x50000

#define tx_mem_double		0x60000
#define rx_mem_double		0x70000

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

volatile uint32_t expected = 0;
volatile uint32_t irq_arrived = 0;


static void handler(int irq, void *arg)
{
	uint32_t int_status;
	//uint32_t tmp_r;
	uint32_t tmp;
	
	
	irq_arrived++;
	
	int_status = ioread32(ARINC_BASE + STAT_E_RX);
	rumboot_printf("ARINC STATUS=0x%x\n", int_status); //check status
	int_status = int_status & status_success_bit;
	rumboot_printf("STATUS=0x%x\n" , int_status);
		
//-------------------------------------------	
	int_status = -1;
	while (int_status != 0x00000002) {
	tmp = ioread32(ARINC_BASE + STAT_E_TX);
	rumboot_printf("ARINC SIZE=0x%x\n", tmp); //check status
	int_status = 0x07FFFFFF & tmp;	
  // rumboot_printf("ARINC SIZE=0x%x\n", tmp); //check status
	}
	rumboot_printf("ARINC SIZE=0x%x\n", tmp);
//--------------------------------------------						
	if ( int_status != expected) {
	rumboot_platform_panic("IRQ: expected %x got %x\n", expected, int_status);
	}
	iowrite32(0x1,(ARINC_BASE + INT_DIS));	 //INT RESET	
}		
void arinc_init (uint32_t arinc_base_addr){
	uint32_t receiver_number;

	uint32_t  init_axi_mst;
	uint32_t size;

	init_axi_mst =0x0;
	size   = 0x2;

	//void *tx_mem =rumboot_malloc(8192);
	receiver_number = 0x0;
//-------------------------------------------------------------------------------------
//  Set parameters for DMA exchange loopback  connected on chip pins in testbench
//-------------------------------------------------------------------------------------

	iowrite32(receiver_number,(arinc_base_addr + RNUM_RX)); //receiver ch number
	iowrite32(tx_mem,(arinc_base_addr + AG_E_TX));    	// dma rd channel memory address
	iowrite32(size,(arinc_base_addr + SZ_E_TX));    	// dma rd channel size
	iowrite32(rx_mem,(arinc_base_addr + AG_E_RX));    	// dma wr channel memory address
	iowrite32(size,(arinc_base_addr + SZ_E_RX));    	// dma wr channel size
	
	iowrite32(tx_mem_double,(arinc_base_addr + AG_O_TX));   // dma rd channel memory address
	iowrite32(size,(arinc_base_addr + SZ_O_TX));    	    // dma rd channel size
	iowrite32(rx_mem_double,(arinc_base_addr + AG_O_RX));   // dma wr channel memory address
	iowrite32(size,(arinc_base_addr + SZ_O_RX));    	    // dma wr channel size
	
	iowrite32(init_axi_mst,(arinc_base_addr + AXI_CTRL)); //AXI parameters
	//iowrite32(0x1,(arinc_base_addr + WAIT_TMR_TX));   			// wait  delayed switch after the transmitter start
	iowrite32(0x1,(arinc_base_addr + WAIT_SIG_RX));   			// wait delayed switch after the receiver start
	//iowrite32(0x1,(arinc_base_addr + INT_TX));			 //INT TX
	iowrite32(0x1,(arinc_base_addr + INT_RX));			//INT RX
	iowrite32(0x1,(arinc_base_addr + INT_LONG));		//INT level
	
//--------------------------------------------------------------------------------------
}

static void call_arinc_irq_even_array()
{
	uint32_t enable;
	enable =0x10001;
 
	arinc_init(ARINC_BASE);
    iowrite32(enable,ARINC_BASE + CHANNEL_EN); // run transaction
	//iowrite32(enable,ARINC_BASE + CHANNEL_DIS); // stop transmitter
	rumboot_printf("ARINC START CH0\n");									
}
void wait_irq() {
	while(!irq_arrived) { };
	irq_arrived = 0;;
}

void arinc_stop() {
uint32_t enable = 0x10001;	
iowrite32(enable,ARINC_BASE + CHANNEL_DIS); // stop transmitter	
}
static bool test_arinc_irq_even_array(uint32_t arg)
{
	expected = 0x00000002;
	call_arinc_irq_even_array();
	wait_irq() ;
	arinc_stop();	
	rumboot_printf("Interrupt expected=0x%x\n" , expected);	
	return true;
}
/* Declare the testsuite structure */
TEST_SUITE_BEGIN(arinc_irq_test, "ARINC and IRQ Subsystem")
TEST_ENTRY("ARINC_IRQ_EVEN_ARRAY", 	test_arinc_irq_even_array, 	  1000),
TEST_SUITE_END();

/* Call the whole test suite */
int main()
{
	
	uint32_t *trg 		 = (uint32_t *) 0x40000;
	uint32_t *trg_double = (uint32_t *) 0x60000;

	rumboot_printf("copy %d bytes\n", sizeof(tx_array32));  //transmi
	memcpy(trg, tx_array32,sizeof(tx_array32));

	rumboot_printf("copy %d bytes\n", sizeof(tx_array_double));  //transmi
	memcpy(trg_double, tx_array_double,sizeof(tx_array32));
	
	rumboot_irq_cli(); //Disable all interrupts
	
	struct rumboot_irq_entry *tbl = rumboot_irq_create(NULL); //Create an IRQ table	
	
	rumboot_irq_set_handler(tbl, ARINC_INT, 0, handler, NULL); //Configure handler
	
	rumboot_irq_table_activate(tbl); //activate the table

	//rumboot_irq_enable(0); //enable IRQ

	rumboot_irq_sei(); //allow interrupt handling

    int ret = test_suite_run(NULL, &arinc_irq_test); //run irq test suite
    rumboot_printf("%d tests from suite failed\n", ret);

	/* Deinit */
	rumboot_irq_table_activate(NULL);
	rumboot_irq_free(tbl);

    return ret;
}