//-----------------------------------------------------------------------------
//  This program is for checking I2C transactions in mulimaster mode.
// 
//  Test includes:
//  - 2 I2c masters working as transmitters and receivers with 24FC1025 EEPROM simulation model
//  - configured  frequency =1MHz
//	- use I2c arbitration lost signal generation and checking for both controllers
//	- received array is  compared with ethalon values	
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

int i2c_set_prescale(uint32_t i2c_base_addr, uint32_t scl_freq){
	uint32_t ps = scl_freq;
	uint32_t tmp_ps;
	iowrite32(ps, I2C_BASE + I2C_PRESCALE );
	//rumboot_printf("I2C0_WR=0x%x\n",ps);
	tmp_ps = ioread32(I2C_BASE + I2C_PRESCALE );
	//rumboot_printf("I2C0_RD=0x%x\n", tmp_ps);
	if (tmp_ps == ps )
	return I2C_OK;
    else
	return I2C_FAILED;
	}

int i2c_ini( uint32_t scl_freq){
	uint32_t tmp_ctrl;

	i2c_set_prescale(I2C_BASE,scl_freq);
	 iowrite32(0x13, (I2C_BASE + 0x1000) + I2C_PRESCALE );
    iowrite32((0x00040001),( I2C_BASE + I2C_FIFOFIL));
	iowrite32((0x00040001),( (I2C_BASE +0x1000) + I2C_FIFOFIL));		
	iowrite32((0x4),( I2C_BASE + I2C_NUMBER));//number of bytes
	iowrite32((0x4),( (I2C_BASE +0x1000) + I2C_NUMBER));//number of bytes			
	tmp_ctrl = ioread32( I2C_BASE + I2C_STATUS);
	tmp_ctrl =  tmp_ctrl & 0xF0;
	if (tmp_ctrl == 0x90 )
	return I2C_OK;
    else {
	rumboot_printf("I2C_status=0x%x\n",tmp_ctrl);
	rumboot_printf("I2C test ERROR!\n");
	return I2C_FAILED; }
	tmp_ctrl = ioread32( (I2C_BASE +0x1000) + I2C_STATUS);
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
    rumboot_printf ("I2C wait\n");
    iowrite32((0x4), (i2c_base_addr + I2C_IER));  //set interrupt enable
	   
    while (tmp != 0x10) {		
    
		tmp = ioread32( I2C_BASE + I2C_STATUS);		
        tmp = 0x10 & tmp;       
  }   // while (tmp != 0x10)
        tmp = ioread32(i2c_base_addr + I2C_ISR);
		rumboot_printf("I2C_ISR_last=0x%x\n",tmp);			
		return I2C_OK;
		
}

int i2c_wait_done(uint32_t i2c_base_addr) {

	 int tmp = -1;
    rumboot_printf ("I2C wait\n");	   
    while (tmp != 0x4) {		
    
		tmp = ioread32( I2C_BASE + I2C_STATUS);		
        tmp = 0x4 & tmp;       
  }   // while (tmp != 0x1)
        tmp = ioread32(i2c_base_addr + I2C_ISR);
		rumboot_printf("I2C_ISR_done=0x%x\n",tmp);			
		return I2C_OK;
		
}


int i2c_wait_trn_compl(uint32_t i2c_base_addr) {

	 int tmp = -1;
    //rumboot_printf ("I2C wait\n");
    
    while (tmp != 0x10) {
    
		tmp = ioread32( I2C_BASE + I2C_STATUS);		
        tmp = 0x10 & tmp;       
  }   // while (tmp != 0x4)
        tmp = ioread32(i2c_base_addr + I2C_ISR);
		rumboot_printf("I2C_ISR_last=0x%x\n",tmp);			
		return I2C_OK;
		
}


int i2c_wait_rd(uint32_t i2c_base_addr) {


	 int tmp = -1;
    rumboot_printf ("I2C wait\n");
    iowrite32((0x4), (i2c_base_addr + I2C_IER));  //set interrupt enable
	   
    while (tmp != 0x8) {		
    
		tmp = ioread32( I2C_BASE + I2C_STATUS);		
        tmp = 0x8 & tmp;      //almost full  
  }   // while (tmp != 0x4)
        tmp = ioread32(i2c_base_addr + I2C_ISR);
		rumboot_printf("I2C_ISR_last=0x%x\n",tmp);			
		return I2C_OK;
		
}
int i2c_wait_al(uint32_t i2c_base_addr) {


	 int tmp = -1;
    //rumboot_printf ("I2C wait\n");

	   
    while (tmp != 0x2) {		
    
		tmp = ioread32( i2c_base_addr + I2C_STATUS);		
        tmp = 0x2 & tmp;      //i2c_al  
  }   // while (tmp != 0x2)
		rumboot_printf("I2C_al_1=0x%x\n",tmp);			
		return I2C_OK;
		
}
int i2c_write_array(uint32_t i2c_base_addr, uint32_t i2c_addr, 
	 uint32_t run_wr) {
		

    int tmp = -1;
	
    rumboot_printf ("I2C WRITE OPERATION\n");
    // Write device address
    rumboot_printf ("    Write device address\n");
	iowrite32((i2c_addr /* I2C_WR*/) ,(i2c_base_addr + I2C_TRANSMIT));
	
 
	iowrite32((run_wr),(i2c_base_addr + I2C_CTRL));  //run I2C write
	 tmp = i2c_wait(i2c_base_addr);
	 rumboot_printf("I2C wake up!\n");
    if (tmp == I2C_FAILED) return I2C_FAILED;
    rumboot_printf("    Write successful\n");	
    return I2C_OK;
}

	
int i2c_write_array_two(uint32_t i2c_base_addr, uint32_t addr_h,
	uint32_t addr_l,uint32_t byte_array_size, uint32_t run_wr, uint32_t data[]) {
		
	int i = 0;	
    int tmp = -1;
	
	 // Write  high subaddress
    rumboot_printf("    Write high subaddr\n");	
    iowrite32((addr_h),(i2c_base_addr + I2C_TRANSMIT)); 
	

	// Write low subaddress
    rumboot_printf("    Write low subaddress\n"); 
	iowrite32((addr_l),(i2c_base_addr + I2C_TRANSMIT)); 
    // Write data
    rumboot_printf("    Write data\n");
	
	iowrite32((run_wr),(i2c_base_addr + I2C_CTRL));  //run I2C write
	for (i = 0; i< byte_array_size; i++)
	{ iowrite32((data[i]),(i2c_base_addr + I2C_TRANSMIT)); }

	 tmp = i2c_wait(i2c_base_addr);
	 rumboot_printf("I2C wake up!\n");
    if (tmp == I2C_FAILED) return I2C_FAILED;
    rumboot_printf("    Write successful\n");	
    return I2C_OK;
}

int i2c_write_array_three(uint32_t i2c_base_addr, uint32_t i2c_addr, uint32_t addr_h,
	uint32_t addr_l, uint32_t run_wr) {
		
    int tmp = -1;
	
	rumboot_printf ("I2C WRITE OPERATION\n");
    // Write device address
    rumboot_printf ("    Write device address\n");
	iowrite32((i2c_addr /* I2C_WR*/) ,(i2c_base_addr + I2C_TRANSMIT));
	
  	  // Write  high subaddress
    rumboot_printf("    Write high subaddr\n");	
    iowrite32((addr_h),(i2c_base_addr + I2C_TRANSMIT)); 
  	
 
	// Write low subaddress
    rumboot_printf("    Write low subaddress\n"); 
	iowrite32((addr_l),(i2c_base_addr + I2C_TRANSMIT)); 
 
	iowrite32((run_wr),(i2c_base_addr + I2C_CTRL));  //run I2C write	
	 tmp = i2c_wait(i2c_base_addr);
	 rumboot_printf("I2C wake up!\n");
    if (tmp == I2C_FAILED) return I2C_FAILED;
    rumboot_printf("    Write successful\n");	
    return I2C_OK;
}


int i2c_write_array_last(uint32_t i2c_base_addr, 
	uint32_t byte_array_size, uint32_t run_wr_rpt, uint32_t data[]) {
		
	int i = 0;	
    int tmp = -1;
	
    rumboot_printf ("I2C WRITE REPEAT OPERATION\n");
  
    // Write data
    rumboot_printf("    Write data\n");
    // iowrite32((data),(i2c_base_addr + I2C_TRANSMIT));  
	rumboot_printf("    START write data\n");
	for (i = 0; i< byte_array_size; i++)
	{ iowrite32((data[i+4]),(i2c_base_addr + I2C_TRANSMIT)); }

	iowrite32((run_wr_rpt),(i2c_base_addr + I2C_CTRL));  //run I2C write
	 tmp = i2c_wait_trn_compl(i2c_base_addr);
	 rumboot_printf("I2C wake up!\n");
    if (tmp == I2C_FAILED) return I2C_FAILED;
    rumboot_printf("    Write successful\n");	
    return I2C_OK;
}

unsigned  i2c_read_check_array (uint32_t i2c_base_addr,uint32_t i2c_addr, uint32_t addr_h,uint32_t addr_l,
		  uint32_t byte_array_size,uint32_t run_wr, uint32_t data[]) {
    int tmp = -1;
	int i = 0;
    unsigned result;    // read data

for (i = 0; i< (byte_array_size); i++)
	{	
	if (i ==  0) {
	   rumboot_printf ("I2C READ OPERATION\n");
    // Write device address
    rumboot_printf("    Write device address\n");
	iowrite32((i2c_addr + 1) ,(i2c_base_addr + I2C_TRANSMIT));
   	
    rumboot_printf("    START read data\n"); 
	iowrite32((I2C_STATRST ),(I2C_BASE + I2C_STAT_RST));//reset status
	iowrite32((run_wr),(i2c_base_addr + I2C_CTRL)); 
	tmp = i2c_wait_rd(i2c_base_addr);
	
    if (tmp == I2C_FAILED) return I2C_FAILED;
	result = ioread32(i2c_base_addr + I2C_RECEIVE);
	 //rumboot_printf("read_num =0x%x\n",i);
	 // rumboot_printf("check_array32 =0x%x\n",data[i]); 
	 //  rumboot_printf("i2c_read_result =0x%x\n",result);  
    if (result  != data[i]) 
    {
	 rumboot_printf("I2C test ERROR!\n");
	 rumboot_printf("i2c_read_result =0x%x\n",result);   
	return TEST_ERROR; }
	}	
	else
		{result = ioread32(i2c_base_addr + I2C_RECEIVE);
   // compare results
    if (result  != data[i]) {
	  rumboot_printf("I2C test ERROR!\n");
	  rumboot_printf("i2c_read_result =0x%x\n",result);  	
        return TEST_ERROR;
			} 	

		}
	  }
	  
  return I2C_OK;  
	}


int main()
{
    int tmp = -1;
	uint32_t i2c_addr = 0xA0;      //0x50; old ver.
	uint32_t i2c_base_addr = I2C_BASE;
	uint32_t addr_h = 0x00;		//write subaddress
	uint32_t addr_l = 0x00;
	//uint32_t size_array;

	static uint32_t check_array32[] = {
        0x80,
        0x40,
        0x20,
		0x10
};

static uint32_t check_array[] = {
        0x20,
        0x04,
        0xFE,
		0xFF
};
//-------------------------------I2C0 write---------------------------------
  // size_array = sizeof(check_array32) / 4;
	tmp =i2c_ini(SCL_FREQ);
	if (tmp == I2C_FAILED)
	{rumboot_printf("I2C init test ERROR!\n");
	return TEST_ERROR;}
		// WRITE operation
	tmp = i2c_write_array(i2c_base_addr, i2c_addr,
			0x13/*I2C_CR_WR_RUN_EN*/);
	if (tmp == I2C_FAILED)
	{rumboot_printf("I2C write test ERROR!\n");
	return TEST_ERROR;}
	
	tmp = i2c_write_array_two(i2c_base_addr,addr_h,
			addr_l, 4,0x11/*I2C_CR_WR_RUN_EN*/, check_array32); //old 0x3
	if (tmp == I2C_FAILED)
	{rumboot_printf("I2C write test ERROR!\n");
	return TEST_ERROR;}
	
//-------------------------------I2C1 write---------------------------------
tmp = i2c_write_array((i2c_base_addr +0x1000), i2c_addr,
			0x13/*I2C_CR_WR_RUN_EN*/);      //0x03  old ver.
	if (tmp == I2C_FAILED)
	{rumboot_printf("I2C write test ERROR!\n");
	return TEST_ERROR;}
	
	tmp = i2c_write_array_two((i2c_base_addr +0x1000),addr_h,
			addr_l, 4,0x11/*I2C_CR_WR_RUN_EN*/, check_array); //old 0x3
	if (tmp == I2C_FAILED)
	{rumboot_printf("I2C write test ERROR!\n");
	return TEST_ERROR;}
		
//--------------------------------------------------------------------------	
	
	iowrite32((I2C_STATRST ),(I2C_BASE + I2C_STAT_RST));
	tmp = ioread32(I2C_BASE + I2C_STATUS);
	rumboot_printf("i2c_read_STATUS =0x%x\n",tmp);
	
    // do settings to avoid the influence of  write instruction
	
    iowrite32((0x0),(i2c_base_addr + I2C_STAT_RST)); //reset STATRST reg
	iowrite32((0x1),(i2c_base_addr + I2C_STAT_RST)); //reset STATUS reg
	
	iowrite32((0x41),(i2c_base_addr + I2C_CTRL)); //set STOP bit in CR_REG		
		// READ & COMPARE operation
	
	tmp = i2c_wait_al(I2C_BASE  +0x1000);
	if (tmp == I2C_FAILED)
	{rumboot_printf(" no I2C write arlost1 ERROR!\n");
	return TEST_ERROR;}	
	rumboot_printf("  I2C write arlost1 ERROR!\n");
	iowrite32((0x0),(I2C_BASE + 0x1000  + I2C_CTRL)); //reset TXFIFO	
	
	tmp = i2c_write_array_three(i2c_base_addr, i2c_addr, addr_h,
			addr_l, 0x13/*I2C_CR_WR_RUN_EN*/);
	if (tmp == I2C_FAILED)
	{rumboot_printf("I2C write test ERROR!\n");
	return TEST_ERROR;}	
	
	
	tmp = 	i2c_read_check_array (i2c_base_addr,i2c_addr, addr_h,addr_l,
			4/*size_array*/,0x6B,  check_array32);
		if (tmp == I2C_FAILED)                  
	{rumboot_printf("I2C read test ERROR!\n");
		return TEST_ERROR;}
	rumboot_printf("I2C TEST OK \n");	
	return I2C_OK; 
	  
}