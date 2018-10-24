#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <platform/devices.h>
#include <devices/i2c.h>
#include <rumboot/io.h>
#include <regs/regs_i2c.h> 
#include <rumboot/testsuite.h>

#include "constants.h"
#include <platform/interrupts.h>


volatile uint32_t expected = 0;
volatile uint32_t irq_arrived = 0;
volatile uint32_t rd_data = 0;

static void handler(int irq, void *arg)
{
	uint32_t int_status;
	uint32_t int_mask;
	
	irq_arrived++;

	int_status = ioread32( I2C_BASE + I2C_ISR);
	rumboot_printf("ISR=0x%x\n" , int_status);
	int_mask = ioread32( I2C_BASE + I2C_IER);
	int_status = int_status & int_mask; 
	
	if ( int_status != expected) {
		rumboot_platform_panic("IRQ: expected %x got %x\n", expected, int_status);
	}
				
}



static void i2c__init()
{
	//init
	iowrite32((0x13),( I2C_BASE + I2C_PRESCALE));
	ioread32( I2C_BASE + I2C_ISR); //reset for not masked/incorrect interrupts
 	iowrite32((0x00),( I2C_BASE + I2C_STAT_RST));	
	iowrite32((0x00),( I2C_BASE + I2C_FIFOFIL));	
}

static void i2c_master_stop()
{
  	iowrite32((0x41),( I2C_BASE + I2C_CTRL)); //  en, stop 
	iowrite32(0x1,(I2C_BASE + I2C_STAT_RST)); // set STAT_RST
	iowrite32((0x0),(I2C_BASE  + I2C_STAT_RST)); //reset STATRST reg
}


static void call_trn_empty()
{
	iowrite32((0x4),( I2C_BASE + I2C_IER));   // create mask for trn_empty 
	iowrite32((0xA0),( I2C_BASE + I2C_TRANSMIT));// A0 = eeprom device address, ee -sensor dev. address
	iowrite32((0x0),( I2C_BASE + I2C_TRANSMIT));  //first address
	iowrite32((0x0),( I2C_BASE + I2C_TRANSMIT));  //second address
	iowrite32((0x60),( I2C_BASE + I2C_TRANSMIT)); //data
	iowrite32((0x13),( I2C_BASE + I2C_CTRL));  //start, en, write 
}


static void call_stop()
{	
	iowrite32((0x1),( I2C_BASE + I2C_IER));   // create mask for trn_empty_almost 
	iowrite32((0x1 ),(I2C_BASE + I2C_STAT_RST));
	ioread32(I2C_BASE + I2C_STATUS);
	iowrite32((0x0),(I2C_BASE + I2C_STAT_RST)); //reset STATRST reg
	iowrite32((0x1),(I2C_BASE + I2C_STAT_RST)); //reset STATUS reg 
	
	iowrite32((0x0),(I2C_BASE + I2C_STAT_RST)); //reset STATRST reg
	iowrite32((0x1),(I2C_BASE + I2C_STAT_RST)); //reset STATUS reg 
	iowrite32((0x41),( I2C_BASE + I2C_CTRL));//I2C_CTRL   en, stop
}

static void call_write()
{
	iowrite32((0x4),( I2C_BASE + I2C_IER));   // create mask for trn_empty 
	iowrite32((0x00),( I2C_BASE + I2C_STAT_RST));//reset I2C_STAT_RST
	iowrite32((0xA0),( I2C_BASE + I2C_TRANSMIT));//I2C_TRANSMIT   ee -sensor dev. address
	iowrite32((0x00),( I2C_BASE + I2C_TRANSMIT)); //first addres
	iowrite32((0x0),( I2C_BASE + I2C_TRANSMIT));  //second address
	iowrite32((0x13),( I2C_BASE + I2C_CTRL));//  I2C_CTRL 0x010   start, en, write 
}

static void call_read()
{
	iowrite32((0x0),( I2C_BASE + I2C_IER));   // create mask 	
	iowrite32((0x00010001),( I2C_BASE + I2C_FIFOFIL));
	iowrite32((0x1),( I2C_BASE + I2C_NUMBER)); //NUMBR 
	iowrite32((0x20),( I2C_BASE + I2C_IER));   // create mask for rcv_full_alm 	
	iowrite32((0xA1),( I2C_BASE + I2C_TRANSMIT));//I2C_TRANSMIT  A0 -EEPROM dev. address
	iowrite32((0x1 ),(I2C_BASE +  I2C_STAT_RST));//reset status	
	iowrite32((0x6B),( I2C_BASE + I2C_CTRL));//I2C_CTRL   start, en, read, rpt
}






void wait_irq() {
	while(!irq_arrived) { };
	irq_arrived = 0;;
}


static bool test_rcv_full_alm(uint32_t arg)
{
	expected = 0x4;

	
	i2c__init();
	call_trn_empty();
	wait_irq();	
	rumboot_printf("expected=0x%x\n" , expected);
	expected = 0x1;
	call_stop();
	wait_irq();	
	rumboot_printf("expected=0x%x\n" , expected);
	expected = 0x4;	
	call_write();
	wait_irq();
	rumboot_printf("expected=0x%x\n" , expected);
	expected = 0x20;
	call_read();
	wait_irq();	
	rumboot_printf("expected=0x%x\n" , expected);
	rd_data = ioread32( I2C_BASE + I2C_RECEIVE);
	if (rd_data != 0x60)
	//rumboot_printf("ERROR READ DATA!\n");	
    return false;
  	i2c_master_stop();
					
	return true;
}
/* Declare the testsuite structure */
TEST_SUITE_BEGIN(i2c_irq_test, "I2C and IRQ Subsystem")
TEST_ENTRY("I2C_RCV_FULL_ALM", 				test_rcv_full_alm, 	  1000),

TEST_SUITE_END();

/* Call the whole test suite */
int main()
{
	/* Disable all interrupts */
	rumboot_irq_cli();

	/* Create an IRQ table */
	struct rumboot_irq_entry *tbl = rumboot_irq_create(NULL);

	/* Configure handler */
	rumboot_irq_set_handler(tbl, I2C_IRQ, 0, handler, NULL);
	
	/* Activate the table */
	rumboot_irq_table_activate(tbl);

	/* Enable IRQ */
	rumboot_irq_enable(I2C_IRQ);

	/* Allow interrupt handling */
	rumboot_irq_sei();

	/* Run our irq test suite */
    int ret = test_suite_run(NULL, &i2c_irq_test);
    rumboot_printf("%d tests from suite failed\n", ret);

	/* Deinit */
	rumboot_irq_table_activate(NULL);
	rumboot_irq_free(tbl);

    return ret;
}

