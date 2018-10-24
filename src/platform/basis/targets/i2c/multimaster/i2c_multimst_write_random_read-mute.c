//-----------------------------------------------------------------------------
//  This program is for checking I2C transactions in mulimaster mode.
// 
//  Test includes:
//  - 2 I2c masters working as transmitters and receivers with sensor simulation model
//  - configured  frequency =1MHz
//	- use I2c arbitration lost signal generation and checking for both controllers	
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
#include <platform/devices.h>
#include <regs/regs_i2c.h> 
#include "../constants.h"

int main()
{  
   int tmp =-1;
   int i = 0;
   //init
 	iowrite32((0x13),( I2C_BASE + I2C_PRESCALE));//I2C_PRESCALE
	iowrite32((0x1),( I2C_BASE + I2C_NUMBER));//I2C_NUMBER
	iowrite32((0x00080008),( I2C_BASE + 0x028));//I2C_FIFOFIL
	
 	iowrite32((0x13),( I2C_BASE + 0x1000 + I2C_PRESCALE));//I2C_PRESCALE
	iowrite32((0x1),( I2C_BASE+ 0x1000  + I2C_NUMBER));// I2C_NUMBER
	iowrite32((0x00080008),( I2C_BASE+ 0x1000 + I2C_FIFOFIL));//I2C_FIFOFIL
//--------------------------preliminary write  into sensor-------
	iowrite32((0xcc),( I2C_BASE + I2C_TRANSMIT));//I2C_TRANSMIT  ee -sensor dev. address
	iowrite32((0x00),( I2C_BASE + I2C_TRANSMIT));//first address, I2C_TRANSMIT
	  while (i != 0x7) {	   
		iowrite32((0x60),( I2C_BASE + I2C_TRANSMIT)); //data		
        i = i+1;        // count = 7
	  }
	iowrite32((0xcc),( I2C_BASE + 0x1000 + I2C_TRANSMIT));//I2C_TRANSMIT  ee -sensor dev. address
	iowrite32((0x00),( I2C_BASE + 0x1000 + I2C_TRANSMIT));//first address, I2C_TRANSMIT
	  while (i != 0x7) {	   
		iowrite32((0x60),( I2C_BASE + 0x1000 + I2C_TRANSMIT)); //data		
        i = i+1;        // count = 7
	  }
	
	
	iowrite32((0x13),( I2C_BASE + I2C_CTRL));//I2Cc_CTRL   start, en, write 
		iowrite32((0x13),( I2C_BASE  + 0x1000 + I2C_CTRL));//I2c_CTRL   start, en, write 

      tmp =-1;
    //  rumboot_printf("i2c_tmp_before =0x%x\n",tmp);	  
	  while (tmp != 0x2) {		   
		tmp = ioread32( I2C_BASE + I2C_STATUS);		
        tmp = 0x2 & tmp;        // if trn_empty
    }   // while (tmp != 0x10)
	   rumboot_printf("I2C0 ARB LOST interrupt!\n");
  
//--intr check-------------------
      tmp =-1;
     // rumboot_printf("i2c_tmp_before =0x%x\n",tmp);	  
	  while (tmp != 0x200) {		   
		tmp = ioread32( I2C_BASE + 0x1000 + I2C_STATUS);		
        tmp = 0x200 & tmp;        // if trn_empty
    }   // while (tmp != 0x10)
	   rumboot_printf("I2C1 NACK interrupt!\n");
  
 if (tmp == I2C_FAILED)
	{rumboot_printf("I2C STOP test ERROR!\n");	
	  return TEST_ERROR;}
	{rumboot_printf("I2C TEST OK \n");	
	return TEST_OK;} 
}