
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
#include "constants.h"


int i2c_ini( uint32_t scl_freq){
	uint32_t tmp_ctrl;
	tmp_ctrl = -1;
	iowrite32(scl_freq, I2C_BASE + I2C_PRESCALE );	
    iowrite32((0x00010001),( I2C_BASE + I2C_FIFOFIL));	
	iowrite32((0x1),( I2C_BASE + I2C_NUMBER));//number of bytes
	tmp_ctrl = ioread32( I2C_BASE + I2C_STATUS);
	tmp_ctrl =  tmp_ctrl & 0xF0;
	if (tmp_ctrl == 0x90 )
	return I2C_OK;
    else {
	rumboot_printf("I2C_status=0x%x\n",tmp_ctrl);
	rumboot_printf("I2C test ERROR!\n");
	return I2C_FAILED; }
	}

int i2c_wait(uint32_t i2c_base_addr) {

	 int tmp = -1;
   // rumboot_printf ("I2C wait\n");
    iowrite32((0x4), (i2c_base_addr + I2C_IER));  //set interrupt enable
	   
    while (tmp != 0x10) {		
    
		tmp = ioread32( I2C_BASE + I2C_STATUS);		
        tmp = 0x10 & tmp;       
  }   // while (tmp != 0x10)
        tmp = ioread32(I2C_BASE + I2C_ISR);		
		return I2C_OK;
		
}



int i2c_wait_done(uint32_t i2c_base_addr) {

	 int tmp = -1;
   // rumboot_printf ("I2C wait\n");	   
    while (tmp != 0x400) {		
    
		tmp = ioread32( I2C_BASE + I2C_STATUS);		
        tmp = 0x400 & tmp;       
  }   // while (tmp != 0x1)
        tmp = ioread32(i2c_base_addr + I2C_ISR);
	//	rumboot_printf("I2C_ISR_done=0x%x\n",tmp);			
		return I2C_OK;
		
}

/*
int i2c_wait_trn_compl(uint32_t i2c_base_addr) {

	 int tmp = -1;
    //rumboot_printf ("I2C wait\n");
    
    while (tmp != 0x10) {		    // old 0x4 
    
		tmp = ioread32( I2C_BASE + I2C_STATUS);		
        tmp = 0x10 & tmp;       
  }   // while (tmp != 0x4)
        tmp = ioread32(i2c_base_addr + I2C_ISR);		
		return I2C_OK;
		
}

*/
int i2c_wait_rd(uint32_t i2c_base_addr) {


	 int tmp = -1;
  //  rumboot_printf ("I2C wait\n");
    iowrite32((0x4), (i2c_base_addr + I2C_IER));  //set interrupt enable
	   
    while (tmp != 0x8) {		
    
		tmp = ioread32( I2C_BASE + I2C_STATUS);		
        tmp = 0x8 & tmp;      //almost full  
  }   // while (tmp != 0x4)
        tmp = ioread32(i2c_base_addr + I2C_ISR);		
		return I2C_OK;
		
}

int i2c_write_array(uint32_t i2c_base_addr, uint32_t i2c_addr, 
	 uint32_t run_wr) {
		
    int tmp = -1;	  
    // Write device address

	iowrite32((i2c_addr /* I2C_WR*/) ,(i2c_base_addr + I2C_TRANSMIT));	 
	iowrite32((run_wr),(i2c_base_addr + I2C_CTRL));  //run I2C write
	 tmp = i2c_wait(i2c_base_addr);
    if (tmp == I2C_FAILED) return I2C_FAILED;
    return I2C_OK;
}

	
int i2c_write_array_two(uint32_t i2c_base_addr,
	uint32_t addr_l,uint32_t byte_array_size, uint32_t run_wr, uint32_t data[]) {
		
	int i = 0;	
    int tmp = -1;
	
	// Write low subaddress
	iowrite32((addr_l),(i2c_base_addr + I2C_TRANSMIT)); 
    // Write data
	iowrite32((run_wr),(i2c_base_addr + I2C_CTRL));  //run I2C write
	for (i = 0; i< byte_array_size; i++)
	{ iowrite32((data[i]),(i2c_base_addr + I2C_TRANSMIT)); }

	 tmp = i2c_wait(i2c_base_addr);
    if (tmp == I2C_FAILED) return I2C_FAILED;
   // rumboot_printf("    Write successful\n");	
    return I2C_OK;
}

int i2c_write_array_three(uint32_t i2c_base_addr, uint32_t i2c_addr,
	uint32_t addr_l, uint32_t run_wr) {
		
    int tmp = -1;
	
    // Write device address
	iowrite32((i2c_addr /* I2C_WR*/) ,(i2c_base_addr + I2C_TRANSMIT));

	// Write low subaddress

	iowrite32((addr_l),(i2c_base_addr + I2C_TRANSMIT)); 
	iowrite32((run_wr),(i2c_base_addr + I2C_CTRL));  //run I2C write	
	 tmp = i2c_wait(i2c_base_addr);
    if (tmp == I2C_FAILED) return I2C_FAILED;
    return I2C_OK;
}


unsigned  i2c_read_check_array (uint32_t i2c_base_addr,uint32_t i2c_addr, uint32_t addr_l,
		  uint32_t byte_array_size,uint32_t run_wr, uint32_t data[]) {
    int tmp = -1;
	int i = 0;
    unsigned result;    // read data

//for (i = 0; i< (byte_array_size); i++)
	{	
	if (i ==  0) {
	// I2C READ OPERATION  
    // Write device address

	iowrite32((i2c_addr + 1) ,(i2c_base_addr + I2C_TRANSMIT));  	
	iowrite32((I2C_STATRST ),(I2C_BASE + I2C_STAT_RST));//reset status
	iowrite32((run_wr),(i2c_base_addr + I2C_CTRL));
	
	tmp = i2c_wait_rd(i2c_base_addr);	
    if (tmp == I2C_FAILED) return I2C_FAILED;
	
	result = ioread32(i2c_base_addr + I2C_RECEIVE);
	rumboot_printf("i2c_read_result =0x%x\n",result); 
    if (result  != data[0]) 
    {
	 rumboot_printf("I2C test ERROR!\n");
	 rumboot_printf("i2c_read_result =0x%x\n",result);   
	return TEST_ERROR; }
		}	
	}	  
  return I2C_OK;  
	}


int main()
{
    int tmp = -1;
	uint32_t i2c_addr = 0xcc;      //0xA0;
	uint32_t i2c_base_addr = I2C_BASE;
	uint32_t addr_l = 0x00;


	static uint32_t check_array32[] = {
        0x80
};

  // size_array = sizeof(check_array32) / 4;
	tmp =i2c_ini(SCL_FREQ);
	if (tmp == I2C_FAILED)
	{rumboot_printf("I2C init test ERROR!\n");
	return TEST_ERROR;}
		// WRITE operation
	tmp = i2c_write_array(i2c_base_addr, i2c_addr,
			0x13);      //0x03  old ver.
	if (tmp == I2C_FAILED)
	{rumboot_printf("I2C write test ERROR!\n");
	return TEST_ERROR;}
	
	tmp = i2c_write_array_two(i2c_base_addr,
			addr_l, 1,0x11/*I2C_CR_WR_RUN_EN*/, check_array32);
	if (tmp == I2C_FAILED)
	{rumboot_printf("I2C write test ERROR!\n");
	return TEST_ERROR;}
	

	iowrite32((I2C_STATRST ),(I2C_BASE + I2C_STAT_RST));
	tmp = ioread32(I2C_BASE + I2C_STATUS);
	//rumboot_printf("i2c_read_STATUS =0x%x\n",tmp);
	
    // do settings to avoid the influence of  write instruction
	
    iowrite32((0x0),(i2c_base_addr + I2C_STAT_RST)); //reset STATRST reg
	iowrite32((0x1),(i2c_base_addr + I2C_STAT_RST)); //reset STATUS reg
	
	
	tmp = -1;
	iowrite32((0x41),(i2c_base_addr + I2C_CTRL)); //set STOP bit in CR_REG		
		// READ & COMPARE operation
		
		tmp = i2c_wait_done(i2c_base_addr);	
    if (tmp == I2C_FAILED) return I2C_FAILED;
				
	tmp = i2c_write_array_three(i2c_base_addr, i2c_addr,
			addr_l, 0x13);
	if (tmp == I2C_FAILED)
	{rumboot_printf("I2C write test ERROR!\n");
	return TEST_ERROR;}	
		
   
	tmp = 	i2c_read_check_array (i2c_base_addr,i2c_addr, addr_l,
			1/*size_array*/,0x6B,  check_array32);
		if (tmp == I2C_FAILED)                  
	{rumboot_printf("I2C read test ERROR!\n");
		return TEST_ERROR;}

		
    tmp = i2c_wait_done(i2c_base_addr);
	if (tmp == I2C_FAILED)
	{rumboot_printf("I2C STOP test ERROR!\n");	
	  return TEST_ERROR;}
	rumboot_printf("I2C TEST OK \n");	
	return I2C_OK; 
	  
}