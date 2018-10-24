//-----------------------------------------------------------------------------
//  This program is for checking ARINC429 transactions and interrupts.
// 
//  Test includes:
//  - ARINC429 transmitter's and receiver's delivery through the external loopback
//	- Receiver accept short arrays without checking labels 
//  - Configured  frequency =100KHz
//	- Received array is  compared with ethalon values
//	- Interrupts are  generated by external timer TMR_EVENT for receiver, 
//	  ARINC  switches only the receiver DMA buffer to even/odd array after TMR_EVENT
//	- Free Counter values fixed and checked for first and last words of receiver even and odd DMA array
//	- Free Counter values fixed and checked for first and last words of transmitter even DMA array
//	- Last stored word FRC and First stored word coinside in this test
//	  Test duration (RTL): < 
//-----------------------------------------------------------------------------


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <devices/arinc.h>
#include <rumboot/rumboot.h>
#include <rumboot/irq.h>
#include <platform/devices.h>
#include <rumboot/io.h>
#include <rumboot/testsuite.h>
#include <rumboot/platform.h>
#include <platform/interrupts.h>
#include <regs/regs_global_timers.h>


volatile uint32_t rd_data = 0;

static const int32_t tx_array32[] = {
        0x80818283,
        0x40414243,
        0x10818283,
        0x20414243,
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
        0xF0818283,
        0xF0414243,
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
volatile uint32_t chk = 0;

static void handler(int irq, void *arg)
{
	uint32_t int_status;
	uint32_t tmp;
	irq_arrived++;
	chk++;
	rumboot_printf("ARINC IRQ received=0x%x\n", irq_arrived); //check status
	rumboot_printf("ARINC chk=0x%x\n", chk); //check status	
//-------------------------------------------	
	int_status = -1;
	if  (chk==1) {
	
    int_status = -1;
	while (int_status != 0x00000001) {	   	
	//rumboot_printf("ARINC even rx data_1_E_status=0x%x\n", tmp); //signal_bit
	//tmp = ioread32(ARINC_BASE + STAT_O_RX);
	//rumboot_printf("ARINC even rx data_1_O_status=0x%x\n", tmp); //signal_bit
	tmp = ioread32(ARINC_BASE + STAT_E_RX);
	//rumboot_printf("ARINC even rx data_1_E_status=0x%x\n", tmp); //signal_bit
	int_status = 0xDFFFFFFF & tmp;
	expected = 0x00000001;
		}				
	if ( int_status != expected) {
	rumboot_platform_panic("IRQ: expected %x got %x\n", expected, int_status);
		}  	
	}	
     else if (chk ==2){
	//rumboot_printf("ARINC chk=0x%x\n", chk); //check status		 
	
    int_status = -1;
	expected =	0x00000001;
	while (int_status != 0x00000001) { 
	tmp = ioread32(ARINC_BASE + STAT_O_RX);
	//rumboot_printf("ARINC odd rx data_2_status=0x%x\n", tmp); //signal_bit
	int_status = 0xDFFFFFFF & tmp;	
		}				
	if ( int_status != expected) {
	rumboot_platform_panic("IRQ: expected %x got %x\n", expected, int_status);
		}
	}
	
	else if  (chk==3) {

    int_status = -1;
	expected =	0x00000001; 
	while (int_status != 0x00000001) {	   	
	tmp = ioread32(ARINC_BASE + STAT_E_RX);
	//rumboot_printf("ARINC even rx data_3_status=0x%x\n", tmp); //signal_bit
	int_status = 0xDFFFFFFF & tmp;	
		}	
	//rumboot_printf("RX:int masked_status =0x%x\n", tmp);				
	if ( int_status != expected) {
	rumboot_platform_panic("IRQ: expected %x got %x\n", expected, int_status);
		}
	}

	else if  (chk==4) {
    int_status = -1;
	expected =	0x00000001; //0x00000000
	while (int_status != 0x00000001) {	// (int_status != 0x00000000)
	tmp = ioread32(ARINC_BASE + STAT_O_RX);
	//rumboot_printf("ARINC even rx data_4_status=0x%x\n", tmp); //signal_bit
	int_status = 0xDFFFFFFF & tmp;	
		}					
	if ( int_status != expected) {
	rumboot_platform_panic("IRQ: expected %x got %x\n", expected, int_status);
		}
	}	
	iowrite32(0x1,(ARINC_BASE + INT_DIS));	 //INT RESET

}
		
void arinc_init (uint32_t arinc_base_addr){
	uint32_t receiver_number;

	uint32_t  init_axi_mst;
	uint32_t size;
	uint32_t tmp;
	
	init_axi_mst =0x0;
	size   = 0x11;

	
	receiver_number = 0x0;
//-------------------------------------------------------------------------------------
//  Set parameters for DMA exchange loopback  connected on chip pins in testbench
//-------------------------------------------------------------------------------------

	iowrite32(receiver_number,(arinc_base_addr + RNUM_RX)); //receiver ch number
	iowrite32(size,(arinc_base_addr + SZ_E_TX));    	// dma rd channel size
	iowrite32(size,(arinc_base_addr + SZ_E_RX));    	// dma wr channel size
	
	iowrite32(size,(arinc_base_addr + SZ_O_TX));    	    // dma rd channel size
	iowrite32(size,(arinc_base_addr + SZ_O_RX));    	    // dma wr channel size
	
	iowrite32(init_axi_mst,(arinc_base_addr + AXI_CTRL)); //AXI parameters
	iowrite32(0x1,(arinc_base_addr + WAIT_SIG_RX));   	// wait delayed switch after the receiver start
	iowrite32(0x1,(arinc_base_addr + INT_RX));			//INT RX
	iowrite32(0x1,(arinc_base_addr + INT_LONG));		//INT level
	iowrite32(0x1,(arinc_base_addr + TMR_MASK_RX));     //TMR_EVENT signal
	
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
//--------------------------------------------------------------------------------------
}

 void call_arinc_irq_even_array()
{

 
	arinc_init(ARINC_BASE);
 								
}
void wait_irq() {
	while(!irq_arrived) { };
	irq_arrived = 0;
}
 
void call_arinc_irq_delta_array()
{	
	iowrite32(0x1,(ARINC_BASE + INT_RX));			//INT RX
	iowrite32(0x1,(GLOBAL_TIMERS + FIX_CMD)); 		//FIX Time  FRC
}


static bool call_frc_first_e_check()
{
	uint32_t tmp;
	uint32_t frc_L;

	frc_L = ioread32(GLOBAL_TIMERS + FREE_RUN_L);	
	tmp = ioread32(ARINC_BASE + TRF_E_TX);
	if (frc_L > tmp) {
	rumboot_printf("ARINC TRF0_E_TX =0x%x\n", tmp);
	rumboot_printf("FREE_RUN_L =0x%x\n", frc_L ); 
	rumboot_printf("ARINC ERROR!\n");
    return false;
	}
	return true;
}
static bool call_frc_current_e_check()
{
	uint32_t tmp;
	uint32_t frc_L;
	
	frc_L = ioread32(GLOBAL_TIMERS + FREE_RUN_L);
	tmp = ioread32(ARINC_BASE + TRF_E_TX);
	if (frc_L < tmp) {
	rumboot_printf("ARINC TRF0_E_TX =0x%x\n", tmp);
	rumboot_printf("FREE_RUN_L =0x%x\n", frc_L ); 
	rumboot_printf("ARINC ERROR!\n");
    return false;
	}
	return true;	
}	
static bool call_frc_e_check()
{
	uint32_t tmp;
	uint32_t tmp1;
	uint32_t tmp2;
	uint32_t frc_L;
	uint32_t frc_H;
	frc_L = ioread32(GLOBAL_TIMERS + FREE_RUN_L);
	frc_H = ioread32(GLOBAL_TIMERS + FREE_RUN_H);
	
	
    tmp1 = ioread32(ARINC_BASE + TLF_E_TX);
	if ((frc_H != tmp1) & (tmp1 !=0)) {
	rumboot_printf("ARINC TLF_E_TX =0x%x\n", tmp1);
	rumboot_printf("FREE_RUN_H =0x%x\n", frc_H );
	rumboot_printf("ARINC ERROR!\n");
    return false;
	}
	tmp2 = ioread32(ARINC_BASE + TRL_E_TX);
	if (tmp2 >0)  {
	rumboot_printf("ARINC TRL_E_TX =0x%x\n", tmp2);	
	rumboot_printf("FREE_RUN_L =0x%x\n", frc_L );
	rumboot_printf("ARINC ERROR!\n");
    return false;
	}	
	tmp = ioread32(ARINC_BASE + TRF_E_RX);
	if (frc_L > tmp) {
	rumboot_printf("ARINC TRF_E_RX =0x%x\n", tmp);
	rumboot_printf("FREE_RUN_L =0x%x\n", frc_L );
	rumboot_printf("ARINC ERROR!\n");
    return false;
	}
    tmp1 = ioread32(ARINC_BASE + TLF_E_RX);
	if ((frc_H != tmp1) & (tmp1 !=0)) {
	rumboot_printf("ARINC TLF_E_RX =0x%x\n", tmp1);
	rumboot_printf("FREE_RUN_H =0x%x\n", frc_H );
	rumboot_printf("ARINC ERROR!\n");
    return false;
	}
	tmp2 = ioread32(ARINC_BASE + TRL_E_RX);
	if (frc_L > tmp2)  {
	rumboot_printf("ARINC TRL_E_RX =0x%x\n", tmp2);	
	rumboot_printf("FREE_RUN_L =0x%x\n", frc_L );
	rumboot_printf("ARINC ERROR!\n");
    return false;
	}
	if (tmp != tmp2)  {
	rumboot_printf("ARINC ERROR FRC store O_array!\n");
    return false;
	}
	
return true;	
}
static bool call_frc_o_check()
{
	uint32_t tmp;
	uint32_t tmp1;
	uint32_t tmp2;	
	uint32_t frc_L;
	uint32_t frc_H;
	frc_L = ioread32(GLOBAL_TIMERS + FREE_RUN_L);
	frc_H = ioread32(GLOBAL_TIMERS + FREE_RUN_H);
	
    tmp1 = ioread32(ARINC_BASE + TLF_E_TX);
	if ((frc_H != tmp1) & (tmp1 !=0)) {
	rumboot_printf("ARINC TLF_E_TX =0x%x\n", tmp1);
	rumboot_printf("FREE_RUN_H =0x%x\n", frc_H );
	rumboot_printf("ARINC ERROR!\n");
    return false;
	}
	tmp2 = ioread32(ARINC_BASE + TRL_E_TX);
	if ( tmp2 >0)  {
	rumboot_printf("ARINC TRL_E_TX =0x%x\n", tmp2);	
	rumboot_printf("FREE_RUN_L =0x%x\n", frc_L );
	rumboot_printf("ARINC ERROR!\n");
    return false;
	}	
	tmp = ioread32(ARINC_BASE + TRF_O_RX);
	if (frc_L > tmp) {
	rumboot_printf("ARINC TRF_O_RX =0x%x\n", tmp);
	rumboot_printf("FREE_RUN_L =0x%x\n", frc_L ); 
	rumboot_printf("ARINC ERROR!\n");
    return false;
	}
    tmp1 = ioread32(ARINC_BASE + TLF_O_RX);
	if ((frc_H != tmp1) & (tmp1 !=0)) {
	rumboot_printf("ARINC TLF_O_RX =0x%x\n", tmp1);
	rumboot_printf("FREE_RUN_H =0x%x\n", frc_H );
	rumboot_printf("ARINC ERROR!\n");
    return false;
	}
	tmp2 = ioread32(ARINC_BASE + TRL_O_RX);
	if (frc_L > tmp2)  {
	rumboot_printf("ARINC TRL_O_RX =0x%x\n", tmp2);	
	rumboot_printf("FREE_RUN_H =0x%x\n", frc_L );
	rumboot_printf("ARINC ERROR!\n");
    return false;
	}
		if (tmp != tmp2)  {
	rumboot_printf("ARINC FRC store O_array ERROR!\n");
    return false;
	}
	
 return true;
		
}
void arinc_stop() {
uint32_t enable = 0x10001;	
iowrite32(enable,ARINC_BASE + CHANNEL_DIS); // stop transmitter	
}


static bool test_arinc_irq_even_array(uint32_t arg)
	{
					
	uint32_t enable;
	enable =0x10001;

	rumboot_printf("copy %d bytes\n", sizeof(tx_array32)); 
	uint32_t *tx_mem = rumboot_malloc_from_heap_aligned(0,  sizeof(tx_array32), 128);	
	rumboot_printf("tx_mem=0x%x\n", tx_mem ); //check data
	
	uint32_t *trg = (uint32_t *) tx_mem;
	memcpy(trg, tx_array32,sizeof(tx_array32));
	
	uint32_t *rx_mem = rumboot_malloc_from_heap_aligned(0, sizeof(tx_array32), 128);
	rumboot_printf("rx_mem=0x%x\n", rx_mem ); //check data

	uint32_t *tx_mem_double = rumboot_malloc_from_heap_aligned(0, sizeof(tx_array_double), 128);
	uint32_t *trg_double = (uint32_t *) tx_mem_double;
	memcpy(trg_double, tx_array_double,sizeof(tx_array_double));
	rumboot_printf("tx_mem_double=0x%x\n", tx_mem_double );

	 uint32_t *rx_mem_double = rumboot_malloc_from_heap_aligned(0, sizeof(tx_array_double), 128);	
	rumboot_printf("rx_mem_double=0x%x\n", rx_mem_double );
	
	iowrite32((uint32_t) &tx_mem[0],(ARINC_BASE + AG_E_TX));    	// dma rd channel memory address	
	iowrite32((uint32_t) &rx_mem[0],(ARINC_BASE + AG_E_RX));    	// dma wr channel memory address
	iowrite32((uint32_t) &tx_mem_double[0],(ARINC_BASE + AG_O_TX)); // dma rd channel memory address	
	iowrite32((uint32_t) &rx_mem_double[0],(ARINC_BASE + AG_O_RX)); // dma wr channel memory address
	expected = 0x00000001;
	
	call_arinc_irq_even_array();
	
	iowrite32(enable,ARINC_BASE + CHANNEL_EN); // run transaction
	rumboot_printf("ARINC START CH0\n");
	
	wait_irq() ;
	//iowrite32((uint32_t) &rx_mem[1],(ARINC_BASE + AG_E_RX)); //mem RX addr update 
	rumboot_printf("READ DATA ARRAY\n");
	 rumboot_printf("ARINC rx_mem[0] addr=0x%x\n",((uint32_t) &rx_mem[0])); //check data
	rd_data	= ioread32((uint32_t) &rx_mem[0]);
	iowrite32((uint32_t) &rx_mem[1],(ARINC_BASE + AG_E_RX)); //mem RX addr update 
	rumboot_printf("ARINC data=0x%x\n", rd_data); //check data
	
	if (rd_data != 0x80818283) {
	//rumboot_printf("ERROR READ EVEN ARRAY 0w\n");
	rumboot_printf("ARINC data=0x%x\n", rd_data); //data
	rd_data	= ioread32((uint32_t) &rx_mem_double[0]);
	rumboot_printf("ARINC data=0x%x\n", ((uint32_t) &rx_mem_double[0])); //check address
	}
	if (rd_data != 0x80818283) {
	rumboot_printf("ERROR READ EVEN ARRAY 0w\n");	
    return false;
	}
	call_frc_first_e_check();
	call_frc_e_check();			
	call_arinc_irq_delta_array();
	wait_irq() ;
	iowrite32(((uint32_t) &rx_mem_double[1]),(ARINC_BASE + AG_O_RX)); //mem rx_double addr update
	rd_data	= ioread32((uint32_t) &rx_mem_double[0]);
	rumboot_printf("ARINC data=0x%x\n", rd_data); //check data
	if (rd_data != 0x40414243) {
	rumboot_printf("ERROR READ ODD ARRAY 1w\n");
	rumboot_printf("ARINC data=0x%x\n", rd_data); //check data
    return false;
	}
	call_frc_current_e_check();
	call_frc_o_check();	
	expected = 0x00000003;
	call_arinc_irq_delta_array();
	wait_irq() ;
	iowrite32(((uint32_t) &rx_mem[2]),(ARINC_BASE + AG_E_RX)); //mem RX addr update 
	
	rd_data	= 	ioread32((uint32_t) &rx_mem [1]);
	rumboot_printf("ARINC data=0x%x\n", rd_data); //check data
	rumboot_printf("ARINC  &rx_mem [1]=0x%x\n", ((uint32_t) &rx_mem [1]));
		if (rd_data != 0x10818283) {
	rumboot_printf("ERROR READ EVEN ARRAY 2w\n");	 
    return false;
		}
	call_frc_current_e_check();	
	call_frc_e_check();	
	call_arinc_irq_delta_array();                                                                                                                                                                                                    
	wait_irq() ;                                     
	iowrite32(((uint32_t) &rx_mem_double[2]),(ARINC_BASE + AG_O_RX)); //mem rx_double addr update
	rd_data	= ioread32((uint32_t) &rx_mem_double[1]);
	rumboot_printf("ARINC data=0x%x\n", rd_data); //check data
	rumboot_printf("ARINC &rx_mem_double[2]=0x%x\n", ((uint32_t) &rx_mem_double[2])); //check data
		if (rd_data != 0x20414243) {
	rumboot_printf("ERROR READ ODD ARRAY 3w\n");	
    return false;
		}
	call_frc_current_e_check();	
	call_frc_o_check();	
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