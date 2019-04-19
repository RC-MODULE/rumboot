
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

int i2c_check_id (uint32_t i2c_base_addr) {
       unsigned ack;
	   unsigned id = 0x12c012c;
       ack = ioread32(i2c_base_addr);
	   rumboot_printf("I2C0_ID=0x%x\n",ack);	   
       if (ack == id )
           return TEST_OK;
       else {
           rumboot_printf(" no id\n");
           return I2C_FAILED;
       }
   }

int i2c_set_prescale(uint32_t i2c_base_addr, uint32_t scl_freq){
	uint32_t ps = scl_freq;
	uint32_t tmp_ps;
	iowrite32(ps, I2C_BASE + I2C_PRESCALE );
	tmp_ps = ioread32(I2C_BASE + I2C_PRESCALE );
	if (tmp_ps == ps )
	return TEST_OK;
    else
	return I2C_FAILED;
	}

int i2c_ini( uint32_t scl_freq){
	uint32_t tmp_ctrl;
    i2c_check_id(I2C_BASE);
	i2c_set_prescale(I2C_BASE,scl_freq);
	tmp_ctrl = ioread32(I2C_BASE + I2C_PRESCALE);
	rumboot_printf("PRESCALE_REG=0x%x\n",tmp_ctrl);
	if (tmp_ctrl != scl_freq ) {
	rumboot_printf("PRESCALE_REG=0x%x\n",tmp_ctrl);	
	return I2C_FAILED;
	}
	
    iowrite32((0x00040001),( I2C_BASE + I2C_FIFOFIL));
	tmp_ctrl = ioread32(I2C_BASE + I2C_FIFOFIL);
	rumboot_printf("I2C_FIFOFIL=0x%x\n",(tmp_ctrl));
	if (tmp_ctrl != 0x00040001 ) {
	rumboot_printf("I2C_FIFOFIL=0x%x\n",(tmp_ctrl));	
	return I2C_FAILED; }		
   
	iowrite32((0x4),( I2C_BASE + I2C_NUMBER));//number of bytes
	tmp_ctrl = ioread32(I2C_BASE + I2C_NUMBER);
	rumboot_printf("I2C_NUMBER=0x%x\n",(tmp_ctrl));
	if (tmp_ctrl != 4 ) {
	rumboot_printf("I2C_NUMBER=0x%x\n",tmp_ctrl);
	rumboot_printf("I2C test ERROR!\n");
	return I2C_FAILED; }		
	tmp_ctrl = ioread32( I2C_BASE + I2C_STATUS);
	tmp_ctrl =  tmp_ctrl & 0xF0;
	if (tmp_ctrl == 0x90 )
	return TEST_OK;
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
		return TEST_OK;
		
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
		return TEST_OK;
		
}


int i2c_wait_trn_compl(uint32_t i2c_base_addr) {

	 int tmp = -1;
    //rumboot_printf ("I2C wait\n");
    
    while (tmp != 0x10) {		    // old 0x4 
    
		tmp = ioread32( I2C_BASE + I2C_STATUS);		
        tmp = 0x10 & tmp;       
  }   // while (tmp != 0x4)
        tmp = ioread32(i2c_base_addr + I2C_ISR);
		rumboot_printf("I2C_ISR_last=0x%x\n",tmp);			
		return TEST_OK;
		
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
		return TEST_OK;
		
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
    return TEST_OK;
}

	
int i2c_write_array_two(uint32_t i2c_base_addr, uint32_t addr_h,
	uint32_t addr_l,uint32_t byte_array_size, uint32_t run_wr, uint32_t data) {
		
	int i = 0;	
    int tmp = -1;
	uint32_t dat[i];
	dat[i] = (i+1)  * data;
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
	{dat[i] = (i+1)  * data; 
	iowrite32((dat[i]),(i2c_base_addr + I2C_TRANSMIT));} 	 
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
    return TEST_OK;
}


int i2c_write_array_last(uint32_t i2c_base_addr, 
	uint32_t byte_array_size, uint32_t run_wr_rpt, uint32_t data[]) {
		
	int i = 0;	
    int tmp = -1;
	
    rumboot_printf ("I2C WRITE REPEAT OPERATION\n");
  
    // Write data
    rumboot_printf("    Write data\n"); 
	rumboot_printf("    START write data\n");
	for (i = 0; i< byte_array_size; i++)
	{ iowrite32((data[i+4]),(i2c_base_addr + I2C_TRANSMIT)); }

	iowrite32((run_wr_rpt),(i2c_base_addr + I2C_CTRL));  //run I2C write
	 tmp = i2c_wait_trn_compl(i2c_base_addr);
	 rumboot_printf("I2C wake up!\n");
    if (tmp == I2C_FAILED) return I2C_FAILED;
    rumboot_printf("    Write successful\n");	
    return TEST_OK;
}

unsigned  i2c_read_check_array (uint32_t i2c_base_addr,uint32_t i2c_addr, uint32_t addr_h,uint32_t addr_l,
		  uint32_t byte_array_size,uint32_t run_wr, uint32_t data/*[]*/) {
    int tmp = -1;
	int i = 0;
    unsigned result;    // read data
    uint32_t dat[i];
	dat[i] = data * (i+1);
	
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

    if (result  != dat[i]) 
    {
	 rumboot_printf("I2C test ERROR!\n");
	 rumboot_printf("i2c_read_result =0x%x\n",result);   
	return TEST_ERROR; }
	}	
	else
		{	dat[i] = data * (i+1);
			result = ioread32(i2c_base_addr + I2C_RECEIVE);
   // compare results
    if (result  != dat[i]) {
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
	uint32_t i2c_addr = 0xA0; 
	uint32_t i2c_base_addr = I2C_BASE;
	uint32_t addr_h = 0x00;		//write subaddress
	uint32_t addr_l = 0x00;
	uint32_t scl_frq =0xC7;
	
	tmp =i2c_ini(scl_frq);
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
			addr_l, 4,0x11/*I2C_CR_WR_RUN_EN*/,  0x10);
	if (tmp == I2C_FAILED)
	{rumboot_printf("I2C write test ERROR!\n");
	return TEST_ERROR;}
	
	
	
	
	iowrite32((I2C_STATRST ),(I2C_BASE + I2C_STAT_RST));
	tmp = ioread32(I2C_BASE + I2C_STATUS);
	rumboot_printf("i2c_read_STATUS =0x%x\n",tmp);
		
    // do settings to avoid the influence of  write instruction
	
    iowrite32((0x0),(i2c_base_addr + I2C_STAT_RST)); //reset STATRST reg
	iowrite32((0x1),(i2c_base_addr + I2C_STAT_RST)); //reset STATUS reg
	
	iowrite32((0x41),(i2c_base_addr + I2C_CTRL)); //set STOP bit in CR_REG		
		// READ & COMPARE operation
		
	tmp = i2c_write_array_three(i2c_base_addr, i2c_addr, addr_h,
			addr_l, 0x13/*I2C_CR_WR_RUN_EN*/);
	if (tmp == I2C_FAILED)
	{rumboot_printf("I2C write test ERROR!\n");
	return TEST_ERROR;}	
		
   
	tmp = 	i2c_read_check_array (i2c_base_addr,i2c_addr, addr_h,addr_l,
			4/*size_array*/,0x6B,   0x10);
		if (tmp == I2C_FAILED)                  
	{rumboot_printf("I2C read test ERROR!\n");
		return TEST_ERROR;}
	
    tmp = i2c_wait_done(i2c_base_addr);
	if (tmp == I2C_FAILED)
	{rumboot_printf("I2C STOP test ERROR!\n");	
	  return TEST_ERROR;}
	rumboot_printf("I2C TEST OK \n");	
	return TEST_OK; 
	  
}