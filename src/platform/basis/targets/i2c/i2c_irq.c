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



/*
static void delay(uint32_t deadline)
{
	while (deadline--) {
		asm volatile ("nop");
	}
}*/

static void i2c__init()
{
	//init
	iowrite32((0x13),( I2C_BASE + I2C_PRESCALE));
	ioread32( I2C_BASE + I2C_ISR); //reset for not masked/incorrect interrupts
	//data transmit
 	iowrite32((0x00),( I2C_BASE + I2C_STAT_RST));	
	iowrite32((0x00),( I2C_BASE + I2C_FIFOFIL));	
}

static void i2c_master_stop()
{
  	iowrite32((0x41),( I2C_BASE + I2C_CTRL)); //  en, stop 
	iowrite32(0x1,(I2C_BASE + I2C_STAT_RST)); // set STAT_RST
	iowrite32((0x0),(I2C_BASE  + I2C_STAT_RST)); //reset STATRST reg
}
static void call_nack()
{
	iowrite32((0x40),( I2C_BASE + I2C_IER));   // create mask for NACK 
	iowrite32((0x00),( I2C_BASE + I2C_TRANSMIT));// A0 = eeprom device address, ee -sensor dev. address
	iowrite32((0x13),( I2C_BASE + I2C_CTRL)); // start, en, write 
}

static void call_done()
{
	iowrite32((0x1),( I2C_BASE + I2C_IER));   // create mask for  interpt expected 
	iowrite32((0xA0),( I2C_BASE + I2C_TRANSMIT));// A0 = eeprom device address, ee -sensor dev. address
	iowrite32((0x13),( I2C_BASE + I2C_CTRL)); // start, en, write 
}

static void call_trn_empty()
{
	iowrite32((0x4),( I2C_BASE + I2C_IER));   // create mask for trn_empty 
	iowrite32((0xA0),( I2C_BASE + I2C_TRANSMIT));// A0 = eeprom device address, ee -sensor dev. address
	iowrite32((0x13),( I2C_BASE + I2C_CTRL));  //start, en, write 
}

static void call_trn_empty_alm()
{
	iowrite32((0x10),( I2C_BASE + I2C_IER));   // create mask for trn_empty_almost 
	iowrite32((0xA0),( I2C_BASE + I2C_TRANSMIT));// A0 = eeprom device address, ee -sensor dev. address
	iowrite32((0x13),( I2C_BASE + I2C_CTRL)); // start, en, write 
}


void wait_irq() {
	while(!irq_arrived) { };
	irq_arrived = 0;;
}

static bool test_nack(uint32_t arg)
{
	expected = 0x40;
	i2c__init();
	call_nack();
	wait_irq() ;
	i2c_master_stop();	
	rumboot_printf("Interrupt expected=0x%x\n" , expected);
	
	return true;
}

static bool test_done(uint32_t arg)
{

	 expected = 0x1;
	i2c__init();
	call_done();
	wait_irq() ;
	//expected = 0x0; // because stop calls one more done
  	i2c_master_stop();	
	rumboot_printf("Interrupt expected=0x%x\n" , expected);		
	return true;
}


static bool test_trn_empty(uint32_t arg)
{
	
	expected = 0x4;
	
	i2c__init();
	call_trn_empty();
	wait_irq(); 
	rumboot_printf("expected=0x%x\n" , expected);
  	i2c_master_stop();

	return true;
}

static bool test_trn_empty_alm(uint32_t arg)
{
	expected = 0x10;

	
	i2c__init();
	call_trn_empty_alm();
	wait_irq();
	rumboot_printf("expected=0x%x\n" , expected);	
  	i2c_master_stop();
					
	return true;
}
/* Declare the testsuite structure */
TEST_SUITE_BEGIN(i2c_irq_test, "I2C and IRQ Subsystem")
TEST_ENTRY("I2C_NACK", 						test_nack, 	  1000),
TEST_ENTRY("I2C_DONE",   					test_done,    1000),
TEST_ENTRY("I2C_TXBUF_EMPTY_ALMOST",    	test_trn_empty_alm, 1000),
TEST_ENTRY("I2C_TXBUF_EMPTY",    			test_trn_empty, 1000),
//TEST_ENTRY("I2C_DONE",   					test_done,    1000),
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
	rumboot_irq_enable(0);

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

