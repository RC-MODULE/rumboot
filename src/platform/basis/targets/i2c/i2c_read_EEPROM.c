//-----------------------------------------------------------------------------
//  This program is for checking I2C transactions with interrupt checking.
// 
//  Test includes:
//  -  I2c master uses preliminary write
//	- one byte instruction with write and stop and start sequential read after stop
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
#include <rumboot/bootheader.h>
#include <platform/devices.h>
#include <regs/regs_i2c.h> 
#include "constants.h"

#include <platform/interrupts.h>
-
	
int main()
{  
   int tmp =-1;
   //init
 	iowrite32((0xC7),( I2C_BASE + I2C_PRESCALE));//I2C_PRESCALE
	iowrite32((0x1),( I2C_BASE + I2C_NUMBER));//I2C_NUMBER 	 		
	iowrite32((0x00010001),( I2C_BASE + I2C_FIFOFIL));//I2C_FIFOFIL
	
//-------------- preliminary write address_device & stop into EEPROM-------
	iowrite32((0xA0),( I2C_BASE + I2C_TRANSMIT));//I2C_TRANSMIT  ee -sensor dev. address	
	iowrite32((0x00),( I2C_BASE + I2C_TRANSMIT)); //first address
	iowrite32((0x00),( I2C_BASE + I2C_TRANSMIT)); //second address
	iowrite32((0x39),( I2C_BASE + I2C_TRANSMIT)); //data
	iowrite32((0x53),( I2C_BASE + I2C_CTRL));//  I2C_CTRL 0x010   start, en, write 
	tmp = -1;
	//----------- intr check trn_empty------------------------------
  tmp =-1;
     while (tmp != 0x10) {	   
		tmp = ioread32( I2C_BASE + I2C_STATUS);		
        tmp = 0x10 & tmp;        // if trn_empty
  }   // while (tmp != 0x10)	
	iowrite32((0x0),(I2C_BASE + I2C_STAT_RST)); //reset STATRST reg
	iowrite32((0x1),(I2C_BASE + I2C_STAT_RST)); //reset STATUS reg 
	iowrite32((0x0),(I2C_BASE + I2C_STAT_RST)); //reset STATRST reg
 	
//-------------------------- turn off I2C & check done------		
     while (tmp != 0x400) {	   
		tmp = ioread32( I2C_BASE + I2C_STATUS);		
        tmp = 0x400 & tmp;        // if done
  }   // while (tmp != 0x400) 				
   iowrite32((0x0 ),(I2C_BASE + I2C_STAT_RST)); //I2C_STAT_RST  
  iowrite32((0x1 ),(I2C_BASE + I2C_STAT_RST)); //I2C_STAT_RST
  iowrite32((0x0 ),(I2C_BASE + I2C_STAT_RST)); //I2C_STAT_RST
//----------------------- - begin write_read instruction---------------
//data write instruction
 	iowrite32((0x00),( I2C_BASE + I2C_STAT_RST));//0x02C	reset I2C_STAT_RST

	iowrite32((0xA0),( I2C_BASE + I2C_TRANSMIT));//I2C_TRANSMIT   ee -sensor dev. address
	iowrite32((0x00),( I2C_BASE + I2C_TRANSMIT)); //first address
	iowrite32((0x00),( I2C_BASE + I2C_TRANSMIT)); //second address
//--------------------------reset status before next start----------------------	
	 iowrite32((0x1 ),(I2C_BASE + I2C_STAT_RST)); //I2C_STAT_RST
     iowrite32((0x0 ),(I2C_BASE + I2C_STAT_RST)); //I2C_STAT_RST
//---------------------------------------------------------	
	iowrite32((0x13),( I2C_BASE + I2C_CTRL));//  I2C_CTRL   start, en, write 
//----------- intr check -----------------------------------
      tmp =-1;
      rumboot_printf("i2c_tmp_before =0x%x\n",tmp);	  
	  while (tmp != 0x10) {		   
		tmp = ioread32( I2C_BASE + I2C_STATUS);		
        tmp = 0x10 & tmp;        // if trn_empty
    }   // while (tmp != 0x10)
	  // rumboot_printf("I2C wake up!\n");		

//--data read instruction-------------------
	iowrite32((0xA1),( I2C_BASE + I2C_TRANSMIT));//I2C_TRANSMIT  A1 -sensor dev. address
	iowrite32((0x4B),( I2C_BASE + I2C_CTRL));//I2C_CTRL   start, en, read
//----------- intr check -----------------------------------
      tmp =-1;	
	     while (tmp != 0x10) {		   
		tmp = ioread32( I2C_BASE + I2C_STATUS);		
        tmp = 0x10 & tmp;        //empty
  }   // while (tmp != 0x10)
	  
   if (tmp == 0x10)
   {rumboot_printf("I2C test OK!\n");	
	return TEST_OK; }
	else
	{rumboot_printf("I2C TEST ERROR \n");	
	return TEST_ERROR;} 
}