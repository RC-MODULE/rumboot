//-----------------------------------------------------------------------------
//  This program is for checking I2C transactions with Status register checking.
//
//  Test includes:
//  -  I2c master write one data byte instruction
//  -  I2c master stop instruction after write and release I2C Interface
//	-  I2c master random  one byte read instruction  with stop I2C Interface
//
//    Test duration (RTL): <
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <devices/i2c.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <platform/devices.h>
#include <regs/regs_i2c.h>
#include "constants.h"

#include <platform/interrupts.h>

void delay_cycle (volatile uint32_t delay_value)
{
    for (volatile uint32_t i = 0; i < delay_value; i++)
        ;
}

static bool i2c_is_nack(uint32_t base)
{
	return ioread32(base + I2C_STATUS) & (1<<9);
}

static bool i2c_is_arblost(uint32_t base)
{
	return ioread32(base + I2C_STATUS) & (1<<1);
}

static void i2c_stat_rst(uint32_t base)
{
	iowrite32(0x0, base + I2C_STAT_RST);
	iowrite32(0x1, base + I2C_STAT_RST);
	iowrite32(0x0, base + I2C_STAT_RST);
}

static void i2c_reset(uint32_t base)
{
	iowrite32(1, base + I2C_SOFTR);
	iowrite32(0, base + I2C_SOFTR);
}

int main()
{
	int tmp =-1;
	uint32_t  len = 0x1;
	int i = 0;
	//int delay_tr = 1000;
	int sda, scl;
	
   //init
 	iowrite32((0x31),( I2C_BASE + I2C_PRESCALE));//I2C_PRESCALE
	iowrite32(len, I2C_BASE + I2C_NUMBER);
	iowrite32((0x00010001),( I2C_BASE + I2C_FIFOFIL));//I2C_FIFOFIL

//--------------------------preliminary write 1 byte into sensor-------
	iowrite32((0xA0),( I2C_BASE + I2C_TRANSMIT));//I2C_TRANSMIT  ee -sensor dev. address
	iowrite32((0x00),( I2C_BASE + I2C_TRANSMIT));//first address, I2C_TRANSMIT
	iowrite32((0x00),( I2C_BASE + I2C_TRANSMIT));//second address, I2C_TRANSMIT
	iowrite32((0x55),( I2C_BASE + I2C_TRANSMIT)); //data	
	iowrite32((0x13),( I2C_BASE + I2C_CTRL));//I2c_CTRL   start, en, write
	
	
			bool time_out = true;


		  tmp =-1;
        while (tmp != 0x10) {
		tmp = ioread32( I2C_BASE + I2C_STATUS);
		//rumboot_printf( "check trn_empty for WRITE:  \n");
        tmp = 0x10 & tmp;        // if trn_empty
		 if (tmp ==0x10) time_out = false;
		}   // while (tmp != 0x10	
  
		if (i2c_is_nack(I2C_BASE)) {
			rumboot_printf( "error: WRITE operation nack while writing eeprom offset \n");
			goto bailout;
		}
		if (i2c_is_arblost(I2C_BASE)) {
			rumboot_printf( "error: WRITE operation arbitration lost while writing eeprom offset \n");
			goto bailout;
		}

	
	if (time_out) {
		rumboot_printf("error: time_out while writing eeprom offset  \n");
		goto bailout;
	}
	

	//----------- intr check -----------------------------------
  tmp =-1;
     while (tmp != 0x10) {
		tmp = ioread32( I2C_BASE + I2C_STATUS);
        tmp = 0x10 & tmp;        // if trn_empty
  }   // while (tmp != 0x10)
  iowrite32((0x0 ),(I2C_BASE + I2C_STAT_RST)); //I2C_STAT_RST
  iowrite32((0x1 ),(I2C_BASE + I2C_STAT_RST)); //I2C_STAT_RST
  iowrite32((0x0 ),(I2C_BASE + I2C_STAT_RST)); //I2C_STAT_RST
//-------------------------- turn off I2C--------
	iowrite32((0x41),( I2C_BASE + I2C_CTRL));//I2C_CTRL   en, stop
     while (tmp != 0x400) {
		tmp = ioread32( I2C_BASE + I2C_STATUS);
        tmp = 0x400 & tmp;        // if done
  }   // while (tmp != 0x400)
//----------------------- - begin write_read instruction---------------
    //  Not mandatory delay
       // delay_cycle (2000); 	   
       #ifdef CMAKE_BUILD_TYPE_POSTPRODUCTION
	   delay_cycle (2000);
	    #endif

	i2c_stat_rst(I2C_BASE);
  	tmp = ioread32(I2C_BASE + I2C_STATUS);
	scl = (tmp & (1 << 23)) ? 1 : 0;
	sda = (tmp & (1 << 25)) ? 1 : 0;
	if ((!sda) || (!scl)) {
		rumboot_printf("Invalid I2C line state: SDA: %d SCL: %d \n", sda, scl);
		goto bailout;
	}
 //---------------------- I2C line state----- 
  rumboot_printf(" I2C line state: SDA: %d SCL: %d \n", sda, scl);

   //init
 	iowrite32((0x31),( I2C_BASE + I2C_PRESCALE));//I2C_PRESCALE
	iowrite32(len, I2C_BASE + I2C_NUMBER);
	iowrite32((0x00010001),( I2C_BASE + I2C_FIFOFIL));//I2C_FIFOFIL
	
     #ifdef CMAKE_BUILD_TYPE_POSTPRODUCTION
	   delay_cycle (20000);
	#endif
  
//----------------------- - begin write_read instruction---------------
//data write instruction
 	iowrite32((0x00),( I2C_BASE + I2C_STATUS));//0x02C	reset I2C_STAT_RST

	iowrite32((0xA0),( I2C_BASE + I2C_TRANSMIT));//I2C_TRANSMIT   A0 - EEPROM dev. address
	iowrite32((0x00),( I2C_BASE + I2C_TRANSMIT)); //first address
	iowrite32((0x00),( I2C_BASE + I2C_TRANSMIT)); //second address
//--------------------------reset status before next start----------------------
	i2c_stat_rst(I2C_BASE);
//---------------------------------------------------------
	iowrite32((0x13),( I2C_BASE + I2C_CTRL));//  I2C_CTRL 0x010   start, en, write
//----------- intr check -----------------------------------

		bool timeout = true;

		tmp =-1;
        while (tmp != 0x10) {
		tmp = ioread32( I2C_BASE + I2C_STATUS);
		//rumboot_printf( "check trn_empty for READ:  \n");
        tmp = 0x10 & tmp;        // if trn_empty
		timeout = false;
		}
		// rumboot_printf(" I2C line state after write: SDA: %d SCL: %d \n", sda, scl);
		
		// delay_cycle (20000);
		#ifdef CMAKE_BUILD_TYPE_POSTPRODUCTION
	    delay_cycle (2000);
	    #endif
		 
		if (i2c_is_nack(I2C_BASE)) {
			rumboot_printf( "error: first nack while writing eeprom offset \n");
			goto bailout;
		}
		if (i2c_is_arblost(I2C_BASE)) {
			rumboot_printf( "error: arbitration lost while writing eeprom offset \n");
			goto bailout;
		}


	if (timeout) {
		rumboot_printf("error: timeout while writing eeprom offset  \n");
		goto bailout;
	}
	
		

//------------------------reset status before next start----------------------
	i2c_stat_rst(I2C_BASE);
//---------------------------------------------------------				
		
//--data read instruction-------------------
	iowrite32((0xA1),( I2C_BASE + I2C_TRANSMIT));//I2C_TRANSMIT A0 EEPROM  dev. address
	iowrite32((0x6B),( I2C_BASE + I2C_CTRL));//I2C_CTRL   start, en, read, rpt	
//----------- intr check -----------------------------------
	while (len--) {
	 uint32_t have_bytes = 0;
	 //uint32_t i = 0;
	 uint32_t delay =1000;
		for( i =0; i < (delay);  i++){
			have_bytes = !(ioread32(I2C_BASE + I2C_STATUS) & (1 << 7));
			//rumboot_printf("have_bytes =0x%x\n",(ioread32(I2C_BASE + I2C_STATUS) & (1 << 7)));
			//rumboot_printf("len =0x%x\n",len);
						
			if (have_bytes)
				break;
		}
			#ifdef CMAKE_BUILD_TYPE_POSTPRODUCTION
		delay_cycle (20000);
		#else
		delay_cycle (500);	
		#endif
		if (len && i2c_is_nack(I2C_BASE)) {
				rumboot_printf( "error: unexpected nack at byte \n");
				goto bailout;
			}

		if (i2c_is_arblost(I2C_BASE)) {
				rumboot_printf( "error: arbitration lost at byte \n");
				goto bailout;
			}

		if (!have_bytes) {
			rumboot_printf("error: operation time out at byte \n");
			goto bailout;
		}
		tmp = ioread32(I2C_BASE + I2C_RECEIVE);
		rumboot_printf("READ_DATA =0x%x\n",tmp);
		if ( tmp != 0x55) {
			rumboot_printf("I2C test ERROR!\n");
			rumboot_printf("i2c_read_result =0x%x\n",tmp);  				
			return TEST_ERROR;	
		}
		else
		 goto end;	
	bailout:
	i2c_stat_rst(I2C_BASE);
	i2c_reset(I2C_BASE);
	rumboot_printf( " TEST_ERROR  \n");
	return 1;	
		
	}
	end:
	return TEST_OK; 		
	
}
