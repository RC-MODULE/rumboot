#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/io.h>
#include <basis/defs.h>


#define I2C_SYS_FREQ_HZ   	0x5F5E100  //100 MHz

#define SCL_FREQ 			0xF4240   //100 kHz = 0x186A0//400kHz = 0x61A80//3.4 MHz //1MHz = 0xF4240 //5 MHz = 0x4C4B40
#define EN					0x0080
#define IEN 				0x0040
//transfer reg
#define TDATA	1
//command register
#define I2C_STA (1 << 7)
#define I2C_STO (1 << 6)
#define I2C_RD  (1 << 5)
#define I2C_WR  (1 << 4)
#define I2C_ACK (1 << 3)
#define I2C_IACK (1 << 0)
//status register
#define RxACK		(1 << 7)
#define BUSY		(1 << 6)
#define AL			(1 << 5)
#define I2C_TIP		(1 << 1)
#define IF			(1 << 0)

#define I2C_OK				0
#define I2C_FAILED			1

#define TEST_OK				0
#define TEST_ERROR			1

#define I2C_IRQ				0
#define I2C_TIMEOUT			100000
#define I2C_ATTEMPT			1000

#define I2C_DEV_ADDR       0xA0
#define I2C_DEV_SUBADDR	0x08	// write subaddress

#define I2C_BASE I2C0_BASE

 

   int i2c_ack (uint32_t i2c_base_addr) {
       unsigned ack = 0xffff;
   	//  rumboot_printf("  write acknowledge\n");
       // test acknoledge
       ack = ioread32(i2c_base_addr + I2C_STATUS);
     //  rumboot_printf("read ack =0x%x\n",ack);
       if ((ack & RxACK) == 0)
           return I2C_OK;
       else {
           rumboot_printf(" no ack\n");
           return I2C_FAILED;
       }
   }

int i2c_set_prescale(uint32_t i2c_base_addr, uint32_t scl_freq){
	uint32_t ps = (I2C_SYS_FREQ_HZ / (5 * scl_freq)) - 1;
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

int i2c_init(uint32_t i2c_base_addr, uint32_t scl_freq){
	uint32_t tmp_ctrl;

	i2c_set_prescale(I2C_BASE,scl_freq);
	iowrite32((EN | IEN),( I2C_BASE + I2C_CTRL));//EN and IEN	 //!!!!!!
	//rumboot_printf("ENABLE=0x%x\n",(EN | IEN));
	rumboot_printf("TEST I2C ARRAY\n");	
	
	tmp_ctrl = ioread32( I2C_BASE + I2C_CTRL);

//	rumboot_printf("CTRL_REG=0x%x\n",tmp_ctrl);
	if (tmp_ctrl == (EN | IEN) )
	return I2C_OK;
    else
	return I2C_FAILED;
	}

int i2c_wait(uint32_t i2c_base_addr) {


//	#ifndef I2C_IRQ
	 int tmp = -1;
  //  rumboot_printf ("I2C wait\n");
    int cnt = 0;

	   tmp = ioread32(i2c_base_addr + I2C_STATUS);	
	//	rumboot_printf("I2C_STATUS=0x%x\n",tmp);   
    while ((tmp & I2C_TIP) != 0) {
        tmp = ioread32(i2c_base_addr + I2C_STATUS); 
	//	rumboot_printf("I2C_TIMEOUT=0x%x\n",tmp);  

        if (++cnt == I2C_TIMEOUT) {
         //   rumboot_printf("I2C timeout!\n");
            return I2C_FAILED;
        }   // if (cnt == I2C_TIMEOUT)
    }   // while ((tmp & I2C_TIP) != 0)
//	#else
//    tmp = i2c_wait_irq ();
//    if (tmp == I2C_FAILED) return I2C_FAILED;   // timeout wait interrupt
//	#endif
    return I2C_OK;
}
int i2c_trans(uint32_t i2c_base_addr, uint32_t data, uint32_t condition) {
    int tmp = -1;

    iowrite32(data,(i2c_base_addr + I2C_TRANSMIT));
//	rumboot_printf("ADDR_DEV_REG=0x%x\n",tmp_addr_dev);
    iowrite32(condition,(i2c_base_addr + I2C_COMMAND));

    tmp = i2c_wait(i2c_base_addr);
//	 rumboot_printf("I2C wake up!\n");
    if (tmp == I2C_FAILED) return I2C_FAILED;
    tmp = i2c_ack(i2c_base_addr);
    if (tmp == I2C_FAILED) return I2C_FAILED;
    return I2C_OK;
}

int i2c_write(uint32_t i2c_base_addr, uint32_t i2c_addr, uint32_t addr_h,uint32_t addr_l, uint32_t data) {
    int tmp = -1;
    int cnt = 0;
//    rumboot_printf ("I2C WRITE OPERATION\n");
    // Write device address
 //   rumboot_printf ("    Write device address\n");
    while (tmp != I2C_OK) {
        tmp = i2c_trans (i2c_base_addr, i2c_addr, (I2C_STA | I2C_WR));
//		rumboot_printf("read i2c_write_result =0x%x\n",tmp);

        if (++cnt == I2C_ATTEMPT) {
            rumboot_printf("No I2C device!\n");
            return I2C_FAILED;
        }
    }   // while (tmp != I2C_OK)
    // Write  high subaddress
//    rumboot_printf("    Write subaddress\n");
    tmp = i2c_trans (i2c_base_addr, addr_h, I2C_WR);
    if (tmp == I2C_FAILED) return I2C_FAILED;
	// Write low subaddress
//    rumboot_printf("    Write subaddress\n");
    tmp = i2c_trans (i2c_base_addr, addr_l, I2C_WR);
    if (tmp == I2C_FAILED) return I2C_FAILED;

    // Write data
//    rumboot_printf("    Write data\n");
    tmp = i2c_trans(i2c_base_addr, data, (I2C_STO| I2C_WR)); //???? was STO
    if (tmp == I2C_FAILED) return I2C_FAILED;

 //   rumboot_printf("    Write successful\n");
    return I2C_OK;
}


unsigned  i2c_check_value (uint32_t i2c_base_addr,uint32_t i2c_addr, uint32_t i2c_addr_h,uint32_t i2c_addr_l,uint32_t data) {
    int tmp = -1;
    int cnt = 0;	
    unsigned result;    // read data
  //  rumboot_printf ("I2C READ OPERATION\n");
    // Write device address
 //   rumboot_printf("    Write device address\n");
 //   rumboot_printf("i2c_addr_h =0x%x\n",i2c_addr_h);
//	rumboot_printf("i2c_addr_l =0x%x\n",i2c_addr_l);
    cnt = 0;
    while (tmp != I2C_OK) {
        tmp = i2c_trans (i2c_base_addr, i2c_addr, (I2C_STA | I2C_WR));
        if (++cnt == I2C_ATTEMPT) {
 //           rumboot_printf("No I2C device!\n");
            return I2C_FAILED;
        }
    }   // while (tmp != I2C_OK)
    // Write subaddress
 //   rumboot_printf ("    Write subaddress high\n");
    tmp = i2c_trans (i2c_base_addr, i2c_addr_h, I2C_WR);
    if (tmp == I2C_FAILED) return I2C_FAILED;
//	 rumboot_printf ("    Write subaddress low\n");
    tmp = i2c_trans (i2c_base_addr, i2c_addr_l, I2C_WR);
    if (tmp == I2C_FAILED) return I2C_FAILED;
    rumboot_printf ("Write data\n");
    //rumboot_printf ("    Write device address and READ condition\n");
    tmp = -1;
    rumboot_printf ("READ data\n");

    iowrite32((I2C_STO | I2C_RD | I2C_ACK),(i2c_base_addr + I2C_COMMAND));

    tmp = i2c_wait(i2c_base_addr);
    if (tmp == I2C_FAILED) return I2C_FAILED;

    tmp = ioread32(i2c_base_addr + I2C_RECEIVE);
  //  rumboot_printf("i2c_read_result =0x%x\n",tmp);	
    result = data - tmp;
 //   rumboot_printf("i2c_read_result =0x%x\n",result);	
    
    return result;
}
int main()
{
    int tmp = -1;
	int i = 0;
	uint32_t addr_h = 0x00;// 0x08;		//write subaddress
	uint32_t addr_l[i];
	//uint32_t data	 = 0x8d;   	//write data
	unsigned result;          	//read data
	
	static int32_t check_array32[] = {
        0x80,
        0x40,
       // 0x20,
		//0x10,
       // 0x08,
       // 0x04,
       // 0x02,
       // 0x01,
      //  0x00,
       // 0x7F,
      //  0xBF,
      //  0xDF,
      //  0xEF,
      //  0xF7,
      //  0xFB,
      //  0xFD,
        0xFE,
		0xFF
};
	
//4 interrupts for last READ, 4 interrupt for last write
	
	tmp =i2c_init(I2C_BASE,SCL_FREQ);
	if (tmp == I2C_FAILED)
		//rumboot_printf("I2C init test ERROR!\n");
        return TEST_ERROR;
	iowrite32((EN),( I2C_BASE + I2C_CTRL));//EN and IEN   // mask the interrupt 		
	for (i = 0; i< (sizeof(check_array32) / 4); i++)		
	{
	addr_l[i] =	i;
	 if (i ==  (sizeof(check_array32) / 4) -1) {
	iowrite32((EN | IEN),( I2C_BASE + I2C_CTRL));//EN and IEN   // mask the interrupt		 
	i2c_write(I2C_BASE, I2C_DEV_ADDR,addr_h,addr_l[i], check_array32[i]);
	iowrite32((EN),( I2C_BASE + I2C_CTRL));//EN
	}
    else {
	iowrite32((EN),( I2C_BASE + I2C_CTRL));//EN	
	i2c_write(I2C_BASE, I2C_DEV_ADDR,addr_h,addr_l[i], check_array32[i]);}
	
	if (i == (sizeof(check_array32) / 4) -1) {
	iowrite32((EN | IEN),( I2C_BASE + I2C_CTRL));	
	result = i2c_check_value(I2C_BASE, I2C_DEV_ADDR, addr_h,addr_l[i],check_array32[i]);
	iowrite32((EN),( I2C_BASE + I2C_CTRL));//EN
	}	
	else
	{
	iowrite32((EN ),( I2C_BASE + I2C_CTRL)); 	
	result = i2c_check_value(I2C_BASE, I2C_DEV_ADDR, addr_h,addr_l[i],check_array32[i]);
	}	
	//rumboot_printf("Read result:");
//    rumboot_printf("i2c_check_data =0x%x\n",result);  	
	}
	rumboot_printf("Last intrpt\n");
	

	 
	//rumboot_printf("ENABLE=0x%x\n",(EN | IEN));
//return result;
   // compare results
    if (result  == 0) {
		// Reset interrupt 
    iowrite32((0x1),( I2C_BASE + I2C_COMMAND));//set IACK =1 to clear int reg 
	tmp = ioread32(I2C_BASE + I2C_STATUS); 
	tmp = 0x0001 & tmp;
	if (tmp == I2C_FAILED) return TEST_ERROR; 						
        rumboot_printf("I2C test OK \n");
        return TEST_OK;
		}   // if (result == data)
    else {
        rumboot_printf("I2C test ERROR!\n");
        return TEST_ERROR;
		}   // else (if (result != data))

}
