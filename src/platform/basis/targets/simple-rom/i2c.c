#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/io.h>
#include <basis/defs.h>

#define I2C0_BASE      0x0102A000
#define I2C_SYS_FREQ_HZ   	0xBEBC200  //200 MHz

#define I2C_PRESCALE		0x00
#define I2C_CTRL			0x04
#define I2C_TRANSMIT		0x08
#define I2C_RECEIVE			0x08
#define I2C_COMMAND			0x0C
#define I2C_STATUS			0x0C


#define SCL_FREQ 			0x186a0   //100 kHz  //400kHz //3.4 MHz
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

#define I2C0_DEV_ADDR       0xA0
#define I2C0_DEV_SUBADDR	0x08	// write subaddress


   // unsigned prescale = 0x41;       // prescale for 33 MHz
   // unsigned data = 0x8d;           // write data
   // unsigned address = 0x08;        // write subaddress
   // unsigned result;                // read data

   int i2c_ack (uint32_t i2c_base_addr) {
       unsigned ack = 0xffff;
   	  rumboot_printf("  write acknowledge\n");
       // test acknoledge
       ack = ioread32(i2c_base_addr + I2C_STATUS);
       rumboot_printf("read i2c_write_result =0x%x\n",ack);
       if ((ack & RxACK) == 0)
           return I2C_OK;
       else {
           rumboot_printf("I2C no acknolege!\n");
           return I2C_FAILED;
       }
   }

int i2c_set_prescale(uint32_t i2c_base_addr, uint32_t scl_freq){
	uint32_t ps = (I2C_SYS_FREQ_HZ / (5 * scl_freq)) - 1; //0x41
	uint32_t tmp_ps;
	iowrite32(ps, I2C0_BASE + I2C_PRESCALE );
	rumboot_printf("I2C0_WR=0x%x\n",ps);
	tmp_ps = ioread32(I2C0_BASE + I2C_PRESCALE );
	rumboot_printf("I2C0_RD=0x%x\n", tmp_ps);
	if (tmp_ps == ps )
	return I2C_OK;
    else
	return I2C_FAILED;
	}

int i2c_init(uint32_t i2c_base_addr, uint32_t scl_freq){
	uint32_t tmp_ctrl;

	i2c_set_prescale(I2C0_BASE,scl_freq);
	iowrite32((EN | IEN),( I2C0_BASE + I2C_CTRL));//EN and IEN
	rumboot_printf("ENABLE=0x%x\n",(EN | IEN));
	tmp_ctrl = ioread32( I2C0_BASE + I2C_CTRL);

	rumboot_printf("CTRL_REG=0x%x\n",tmp_ctrl);
	if (tmp_ctrl == (EN | IEN) )
	return I2C_OK;
    else
	return I2C_FAILED;
	}

int i2c_wait(uint32_t i2c_base_addr) {


//	#ifndef I2C_IRQ
	 int tmp = -1;
    rumboot_printf ("I2C wait\n");
    int cnt = 0;

	   tmp = ioread32(i2c_base_addr + I2C_STATUS);		//!!!
		rumboot_printf("I2C_STATUS=0x%x\n",tmp);       //!!!
    while ((tmp & I2C_TIP) != 0) {
        tmp = ioread32(i2c_base_addr + I2C_STATUS);  //!!!
	//	rumboot_printf("I2C_TIMEOUT=0x%x\n",tmp);    //!!!

        if (++cnt == I2C_TIMEOUT) {
            rumboot_printf("I2C timeout!\n");
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
    uint32_t tmp_addr_dev;
    iowrite32(data,(i2c_base_addr + I2C_TRANSMIT));
	tmp_addr_dev = ioread32( I2C0_BASE + I2C_TRANSMIT);
	rumboot_printf("ADDR_DEV_REG=0x%x\n",tmp_addr_dev);
    iowrite32(condition,(i2c_base_addr + I2C_COMMAND));

    tmp = i2c_wait(i2c_base_addr);
	 rumboot_printf("I2C wake up!\n");
    if (tmp == I2C_FAILED) return I2C_FAILED;
    tmp = i2c_ack(i2c_base_addr);
    if (tmp == I2C_FAILED) return I2C_FAILED;
    return I2C_OK;
}

int i2c_write(uint32_t i2c_base_addr, uint32_t i2c_addr, uint32_t addr_h,uint32_t addr_l, uint32_t data) {
    int tmp = -1;
    int cnt = 0;
    rumboot_printf ("I2C WRITE OPERATION\n");
    // Write device address
    rumboot_printf ("    Write device address\n");
    while (tmp != I2C_OK) {
        tmp = i2c_trans (i2c_base_addr, i2c_addr, (I2C_STA | I2C_WR));
		rumboot_printf("read i2c_write_result =0x%x\n",tmp);

        if (++cnt == I2C_ATTEMPT) {
            rumboot_printf("No I2C device!\n");
            return I2C_FAILED;
        }
    }   // while (tmp != I2C_OK)
    // Write  high subaddress
    rumboot_printf("    Write subaddress\n");
    tmp = i2c_trans (i2c_base_addr, addr_h, I2C_WR);
    if (tmp == I2C_FAILED) return I2C_FAILED;
	// Write low subaddress
    rumboot_printf("    Write subaddress\n");
    tmp = i2c_trans (i2c_base_addr, addr_l, I2C_WR);
    if (tmp == I2C_FAILED) return I2C_FAILED;

    // Write data
    rumboot_printf("    Write data\n");
    tmp = i2c_trans(i2c_base_addr, data, (I2C_STO| I2C_WR)); //???? was STO
    if (tmp == I2C_FAILED) return I2C_FAILED;

    rumboot_printf("    Write successful\n");
    return I2C_OK;
}


unsigned  i2c_read (uint32_t i2c_base_addr,uint32_t i2c_addr, uint32_t i2c_addr_h,uint32_t i2c_addr_l) {
    int tmp = -1;
    int cnt = 0;
    unsigned result;    // read data
    rumboot_printf ("I2C READ OPERATION\n");
    // Write device address
    rumboot_printf("    Write device address\n");
    rumboot_printf("i2c_addr_h =0x%x\n",i2c_addr_h);
	rumboot_printf("i2c_addr_l =0x%x\n",i2c_addr_l);
    cnt = 0;
    while (tmp != I2C_OK) {
        tmp = i2c_trans (i2c_base_addr, i2c_addr, (I2C_STA | I2C_WR));
        if (++cnt == I2C_ATTEMPT) {
            rumboot_printf("No I2C device!\n");
            return I2C_FAILED;
        }
    }   // while (tmp != I2C_OK)
    // Write subaddress
    rumboot_printf ("    Write subaddress high\n");
    tmp = i2c_trans (i2c_base_addr, i2c_addr_h, I2C_WR);
    if (tmp == I2C_FAILED) return I2C_FAILED;
	 rumboot_printf ("    Write subaddress low\n");
    tmp = i2c_trans (i2c_base_addr, i2c_addr_l, I2C_WR);
    if (tmp == I2C_FAILED) return I2C_FAILED;

    rumboot_printf ("    Write device address and READ condition\n");
    cnt = 0;
    tmp = -1;
    while (tmp != I2C_OK) {
        tmp = i2c_trans (i2c_base_addr, (i2c_addr | 0x01), (I2C_STA | I2C_WR));
        if (++cnt == I2C_ATTEMPT) {
            rumboot_printf("No I2C device!\n");
            return I2C_FAILED;
        }
    }   // while (tmp != I2C_OK)
    rumboot_printf ("    READ data\n");

    iowrite32((I2C_STO | I2C_RD | I2C_ACK),(i2c_base_addr + I2C_COMMAND));

    tmp = i2c_wait(i2c_base_addr);
    if (tmp == I2C_FAILED) return I2C_FAILED;

    result = ioread32(i2c_base_addr + I2C_RECEIVE);
   // rumboot_printf("i2c_read_result =0x%x\n",result);

    return result;
}
int main()
{

	uint32_t addr_h = 0x00;// 0x08;		//write subaddress
	uint32_t addr_l = 0x00;
	uint32_t data	 = 0x8d;   	//write data
	unsigned result;          	//read data

	i2c_init(I2C0_BASE,SCL_FREQ);
	i2c_write(I2C0_BASE, I2C0_DEV_ADDR,addr_h,addr_l, data);
	result = i2c_read (I2C0_BASE, I2C0_DEV_ADDR, addr_h,addr_l);

	rumboot_printf ("Read result:");
    rumboot_printf("i2c_read_data =0x%x\n",result);
    if (result == I2C_FAILED) {
        rumboot_printf("I2C read failed!\n");
        return TEST_ERROR;
    }   // if (result == I2C_FAILED)
    // compare results
    if (result == data) {
        rumboot_printf("I2C test OK \n");
        return TEST_OK;
    }   // if (result == data)
    else {
        rumboot_printf("I2C test ERROR!\n");
        return TEST_ERROR;
    }   // else (if (result == data))


}
