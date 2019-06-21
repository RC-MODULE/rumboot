//-----------------------------------------------------------------------------
//  This program is for checking I2C transactions with interrupt checking.
//
//  Test includes:
//  -  I2c master in polling mode
//	- one byte instruction with write and stop and start random read after stop
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
  	rumboot_printf("START I2C \n"); 
   //init
 	iowrite32((0xc7),( I2C_BASE + I2C_PRESCALE));
	iowrite32((0x1),( I2C_BASE + I2C_NUMBER));
	iowrite32((0x00010001),( I2C_BASE + I2C_FIFOFIL));

//-------------- preliminary write address_device & stop into EEPROM-------
	iowrite32((0xA0),( I2C_BASE + I2C_TRANSMIT));//I2C_TRANSMIT  A0 - dev. address
	iowrite32((0x53),( I2C_BASE + I2C_CTRL));//I2c_CTRL   start, en, write,stop
	//----------- intr check trn_empty------------------------------
    bool time_out = true;
	while (tmp != 0x10) {
		tmp = ioread32( I2C_BASE + I2C_STATUS);
        tmp = 0x10 & tmp;        // if trn_empty
  }   // while (tmp != 0x10)
	if (i2c_is_nack(I2C_BASE)) {
		rumboot_printf( "error: WRITE operation nack while writing eeprom offset \n");
		goto bailout;
	}
		if (i2c_is_arblost(I2C_BASE)) {
		rumboot_printf( "error: WRITE operation arbitration lost while writing eeprom offset \n");
		goto bailout;
	} 

	iowrite32((0x0),(I2C_BASE + I2C_STAT_RST)); //reset STATRST reg
	iowrite32((0x1),(I2C_BASE + I2C_STAT_RST)); //reset STATUS reg
	iowrite32((0x0),(I2C_BASE + I2C_STAT_RST)); //reset STATUS reg
//-------------------------- turn off I2C & check done------
  tmp =-1;
	while (tmp != 0x400) {
		tmp = ioread32( I2C_BASE + I2C_STATUS);
        tmp = 0x400 & tmp;        // if done
  }   // while (tmp != 0x400)

//--------------  write data byte & stop into EEPROM-------
	iowrite32((0xA0),( I2C_BASE + I2C_TRANSMIT));//I2C_TRANSMIT  ee -sensor dev. address
	iowrite32((0x00),( I2C_BASE + I2C_TRANSMIT));//first address, I2C_TRANSMIT
	iowrite32((0x00),( I2C_BASE + I2C_TRANSMIT));//second address, I2C_TRANSMIT
	iowrite32((0x9F),( I2C_BASE + I2C_TRANSMIT)); //data
	iowrite32((0x53),( I2C_BASE + I2C_CTRL));//I2c_CTRL   start, en, write,stop

	//----------- intr check trn_empty------------------------------
	time_out =1;

     while (tmp != 0x10) {
		tmp = ioread32( I2C_BASE + I2C_STATUS);
      tmp = 0x10 & tmp;        // if trn_empty
		 if (tmp ==0x10) 
		time_out = false;
		}   // while (tmp != 0x10	
  
		if (i2c_is_nack(I2C_BASE)) {
			rumboot_printf( "error: WRITE_operation nack while writing eeprom offset \n");
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

	iowrite32((0x0),(I2C_BASE + I2C_STAT_RST)); //reset STATRST reg
	iowrite32((0x1),(I2C_BASE + I2C_STAT_RST)); //reset STATUS reg
	iowrite32((0x0),(I2C_BASE + I2C_STAT_RST)); //reset STATUS reg

//-------------------------- turn off I2C & check done------
  
     #ifdef CMAKE_BUILD_TYPE_POSTPRODUCTION
	   delay_cycle (20000);
	#endif

  tmp =-1;
  time_out = true;

	while (tmp != 0x400) {
		tmp = ioread32( I2C_BASE + I2C_STATUS);
		rumboot_printf("read_status =0x%x\n",tmp);
        tmp = 0x400 & tmp;        // if done
		
		 if (tmp ==0x400) time_out = false;
  }   // while (tmp != 0x400)
	  
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
//rumboot_printf("GO 4  \n");	
//----------------------- - begin write_read instruction---------------

 //  Not mandatory delay
   	iowrite32((0x31),( I2C_BASE + I2C_PRESCALE));//I2C_PRESCALE
	iowrite32(1, I2C_BASE + I2C_NUMBER);
	iowrite32((0x00010001),( I2C_BASE + I2C_FIFOFIL));//I2C_FIFOFIL
  // delay_cycle (20000); //
    #ifdef CMAKE_BUILD_TYPE_POSTPRODUCTION
	   delay_cycle (20000);
	#endif
//data write instruction
 	iowrite32((0x00),( I2C_BASE + I2C_STAT_RST));//0x02C	reset I2C_STAT_RST

	iowrite32((0xA0),( I2C_BASE + I2C_TRANSMIT));//I2C_TRANSMIT   A0 - dev. address
	iowrite32((0x00),( I2C_BASE + I2C_TRANSMIT)); //first address
	iowrite32((0x00),( I2C_BASE + I2C_TRANSMIT)); //second address
	//iowrite32((0x31),( I2C_BASE + I2C_TRANSMIT)); //data
//--------------------------reset status before next start----------------------
	 iowrite32((0x1 ),(I2C_BASE + I2C_STAT_RST));
     iowrite32((0x0 ),(I2C_BASE + I2C_STAT_RST));
//---------------------------------------------------------
	iowrite32((0x13),( I2C_BASE + I2C_CTRL));//  I2C_CTRL 0x010   start, en, write
//----------- intr check -----------------------------------
      	bool timeout = true;
	  
	  tmp =-1;
 //     rumboot_printf("i2c_tmp_before =0x%x\n",tmp);
	  while (tmp != 0x10) {
		tmp = ioread32( I2C_BASE + I2C_STATUS);
        tmp = 0x10 & tmp;        // if trn_empty
		timeout = false;
    }   // while (tmp != 0x10)
		#ifdef CMAKE_BUILD_TYPE_POSTPRODUCTION
		delay_cycle (20000);
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
//--data read instruction-------------------
	iowrite32((0xA1),( I2C_BASE + I2C_TRANSMIT));//I2C_TRANSMIT  ee -sensor dev. address
	iowrite32((0x6B),( I2C_BASE + I2C_CTRL));//I2C_CTRL   start, en, read, rpt
//----------- intr check -----------------------------------
	     tmp =-1;
	     while (tmp != 0x08) {
		tmp = ioread32( I2C_BASE + I2C_STATUS);
        tmp = 0x08 & tmp;        //almost full
  }   // while (tmp != 0x8)

	 tmp =  ioread32( I2C_BASE + I2C_RECEIVE);
	rumboot_printf("i2c_read_data =0x%x\n",tmp);
	#ifdef CMAKE_BUILD_TYPE_POSTPRODUCTION
		delay_cycle (20000);
		#else
		delay_cycle (500);	
		#endif
	if (((i2c_is_nack(I2C_BASE)) == 0)) {
				rumboot_printf( "error: unexpected nack at byte \n");				
				rumboot_printf("nack =0x%x\n",i2c_is_nack(I2C_BASE));
				rumboot_printf("bit_nack =0x%x\n",(i2c_is_nack(I2C_BASE)));
				goto bailout;
			}

		if (i2c_is_arblost(I2C_BASE)) {
				rumboot_printf( "error: arbitration lost at byte \n");
				goto bailout;
			}
 
	
   if (tmp == 0x9F)
   {rumboot_printf("I2C test OK!\n");
	return TEST_OK; }
	else
bailout:
	i2c_stat_rst(I2C_BASE);
	i2c_reset(I2C_BASE);	
	{rumboot_printf("I2C TEST ERROR \n");	
	return TEST_ERROR;}
}
