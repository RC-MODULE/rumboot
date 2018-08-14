//-----------------------------------------------------------------------------
//  This program is for checking I2C transactions with interrupt checking.
//
//  Test includes:
//  -  I2c master in polling mode
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
#include <rumboot/boot.h>
#include <platform/devices.h>
#include <regs/regs_i2c.h>
#include "constants.h"

#include <platform/interrupts.h>


int main()
{
   int tmp =-1;
   //init
 	iowrite32((0x13),( I2C_BASE + I2C_PRESCALE));
	iowrite32((0x1),( I2C_BASE + I2C_NUMBER));
	iowrite32((0x00010001),( I2C_BASE + I2C_FIFOFIL));

//--------------------------preliminary write 1 byte into sensor-------
	iowrite32((0xcc),( I2C_BASE + I2C_TRANSMIT));//I2C_TRANSMIT  ee -sensor dev. address
	iowrite32((0x00),( I2C_BASE + I2C_TRANSMIT));//first address, I2C_TRANSMIT
	iowrite32((0x60),( I2C_BASE + I2C_TRANSMIT)); //data
	iowrite32((0x13),( I2C_BASE + I2C_CTRL));//I2c_CTRL   start, en, write

   // do settings to avoid the influence of  write instruction to the next start

  	iowrite32((0x1 ),(I2C_BASE + I2C_STAT_RST));
	tmp = ioread32(I2C_BASE + I2C_STATUS);
	//rumboot_printf("i2c_read_STATUS =0x%x\n",tmp);

	iowrite32((0x0),(I2C_BASE + I2C_STAT_RST)); //reset STATRST reg
	iowrite32((0x1),(I2C_BASE + I2C_STAT_RST)); //reset STATUS reg
	tmp = -1;

//----------- intr check -----------------------------------
     while (tmp != 0x10) {
		tmp = ioread32( I2C_BASE + I2C_STATUS);
        tmp = 0x10 & tmp;        // if trn_empty
  }   // while (tmp != 0x10)
   iowrite32((0x0 ),(I2C_BASE + I2C_STAT_RST)); //I2C_STAT_RST
  iowrite32((0x1 ),(I2C_BASE + I2C_STAT_RST)); //I2C_STAT_RST
  iowrite32((0x0 ),(I2C_BASE + I2C_STAT_RST)); //I2C_STAT_RST
//-------------------------- turn off Controller and I2C--------
	iowrite32((0x41),( I2C_BASE + I2C_CTRL));//I2C_CTRL   en, stop
     while (tmp != 0x400) {
		tmp = ioread32( I2C_BASE + I2C_STATUS);
        tmp = 0x400 & tmp;        // if done
  }   // while (tmp != 0x400)

//----------------------- - begin write_read instruction---------------
//data write instruction
 	iowrite32((0x00),( I2C_BASE + I2C_STAT_RST));//reset I2C_STAT_RST

	iowrite32((0xcc),( I2C_BASE + I2C_TRANSMIT));//I2C_TRANSMIT   ee -sensor dev. address
	iowrite32((0x00),( I2C_BASE + I2C_TRANSMIT)); //first address
	iowrite32((0x13),( I2C_BASE + I2C_CTRL));//  I2C_CTRL 0x010   start, en, write
//----------- intr check -----------------------------------
      tmp =-1;
      rumboot_printf("i2c_tmp_before =0x%x\n",tmp);
	  while (tmp != 0x10) {
		tmp = ioread32( I2C_BASE + I2C_STATUS);
        tmp = 0x10 & tmp;        // if trn_empty
    }   // while (tmp != 0x10)

//--data read instruction-------------------
	iowrite32((0xcd),( I2C_BASE + I2C_TRANSMIT));//I2C_TRANSMIT  ee -sensor dev. address
	iowrite32((0x6B),( I2C_BASE + I2C_CTRL));//I2C_CTRL   start, en, read, rpt
 //----------- intr check -----------------------------------
  tmp =-1;
	     while (tmp != 0x8) {
		tmp = ioread32( I2C_BASE + I2C_STATUS);
        tmp = 0x8 & tmp;        //almost full
  }   // while (tmp != 0x8)

	 tmp =  ioread32( I2C_BASE + I2C_RECEIVE);
 if (tmp == 0x60)
   {rumboot_printf("I2C  test OK!\n");
	   return TEST_OK; }
	else
	{rumboot_printf("I2C TEST ERROR \n");
	return TEST_ERROR;}
}
