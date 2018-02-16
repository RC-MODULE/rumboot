#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <regs/regs_i2c.h>
#include <rumboot/irq.h>
#include <platform/devices.h>

#define I2C_BASE I2C0_BASE

#define I2C_ARRAY_SIZE  0x4
#define I2C_CR_RD_RUN_EN    0x13
#define I2C_CR_RPT_RD_RUN_EN 0x33
#define I2C_CR_STAT_RST     0x08
#define I2C_STAT_RST     0x01
#define I2C_CR_WR_RUN_EN    0x3
#define I2C_CR_WR_STAT_RST   0xB

#define I2C_WR  (1 << 10)

#define I2C_SYS_FREQ_HZ    0x5F5E100  //100 MHz

#define SCL_FREQ    0x13   //100 kHz = 0x186A0//400kHz = 0x61A80//3.4 MHz //1MHz = 0xF4240 //5 MHz = 0x4C4B40
#define EN     0x0080
#define IEN     0x0040
//transfer reg
#define TDATA 1
//command register
#define I2C_STA (1 << 7)
#define I2C_STO (1 << 6)
#define I2C_RD  0x300

#define I2C_ACK (1 << 3)
#define I2C_IACK (1 << 0)

//status register
#define RxACK  (1 << 7)
#define BUSY  (1 << 6)
#define AL   (1 << 5)
#define I2C_TIP  0x4//(1 << 2)
#define IF   (1 << 0)


#define I2C_OK    0
#define I2C_FAILED   1

#define TEST_OK    0
#define TEST_ERROR   1

#define I2C_IRQ    0
#define I2C_TIMEOUT   100000
#define I2C_ATTEMPT   1000

#define I2C_DEV_ADDR       0x50
#define I2C_DEV_SUBADDR 0x08 // write subaddress


int i2c_check_id(uint32_t i2c_base_addr)
{
	unsigned ack;
	unsigned id = 0x12c012c;

	ack = ioread32(i2c_base_addr);
	rumboot_printf("I2C0_ID=0x%x\n", ack);
	if (ack == id) {
		return I2C_OK;
	} else {
		rumboot_printf(" no id\n");
		return I2C_FAILED;
	}
}

int i2c_set_prescale(uint32_t i2c_base_addr, uint32_t scl_freq)
{
	uint32_t ps = scl_freq;
	uint32_t tmp_ps;

	iowrite32(ps, I2C_BASE + I2C_CLKPR);
	//rumboot_printf("I2C0_WR=0x%x\n",ps);
	tmp_ps = ioread32(I2C_BASE + I2C_CLKPR);
	//rumboot_printf("I2C0_RD=0x%x\n", tmp_ps);
	if (tmp_ps == ps)
		return I2C_OK;
	else
		return I2C_FAILED;
}

int i2c_initialization(uint32_t scl_freq)
{
	uint32_t tmp_ctrl;

	i2c_check_id(I2C_BASE);
	i2c_set_prescale(I2C_BASE, scl_freq);
	tmp_ctrl = ioread32(I2C_BASE + I2C_CLKPR);
	rumboot_printf("PRESCALE_REG=0x%x\n", tmp_ctrl);
	if (tmp_ctrl != scl_freq) {
		rumboot_printf("PRESCALE_REG=0x%x\n", tmp_ctrl);
		return I2C_FAILED;
	}

	iowrite32((0x00070001), (I2C_BASE + I2C_FIFOFIL));
	tmp_ctrl = ioread32(I2C_BASE + I2C_FIFOFIL);
	rumboot_printf("I2C_FIFOFIL=0x%x\n", (tmp_ctrl));
	if (tmp_ctrl != 0x00070001) {
		rumboot_printf("I2C_FIFOFIL=0x%x\n", (tmp_ctrl));
		return I2C_FAILED;
	}

	iowrite32((0x8), (I2C_BASE + I2C_NUMB));//number of bytes
	tmp_ctrl = ioread32(I2C_BASE + I2C_NUMB);
	rumboot_printf("I2C_NUMBER=0x%x\n", (tmp_ctrl));
	if (tmp_ctrl != 8) {
		rumboot_printf("I2C_NUMBER=0x%x\n", (tmp_ctrl));
		rumboot_printf("I2C_NUMBER=0x%x\n", tmp_ctrl);
		rumboot_printf("I2C test ERROR!\n");
		return I2C_FAILED;
	}
	tmp_ctrl = ioread32(I2C_BASE + I2C_SR);
	tmp_ctrl = tmp_ctrl & 0xFF;
	if (tmp_ctrl == 0x90) {
		return I2C_OK;
	} else {
		rumboot_printf("I2C_SR=0x%x\n", tmp_ctrl);
		rumboot_printf("I2C test ERROR!\n");
		return I2C_FAILED;
	}
}

int i2c_wait(uint32_t i2c_base_addr)
{
	int tmp = -1;

	rumboot_printf("I2C wait\n");
	iowrite32((0x4), (i2c_base_addr + I2C_IER)); //set interrupt enable

	while (tmp != 0x10) {
		tmp = ioread32(I2C_BASE + I2C_SR);
		tmp = 0x10 & tmp;
	} // while (tmp != 0x10)
	tmp = ioread32(i2c_base_addr + I2C_ISR);
	rumboot_printf("I2C_ISR_last=0x%x\n", tmp);
	return I2C_OK;
}

int i2c_wait_trn_compl(uint32_t i2c_base_addr)
{
	int tmp = -1;

	//rumboot_printf ("I2C wait\n");

	while (tmp != 0x4) {
		tmp = ioread32(I2C_BASE + I2C_SR);
		tmp = 0x4 & tmp;
	} // while (tmp != 0x4)
	tmp = ioread32(i2c_base_addr + I2C_ISR);
	rumboot_printf("I2C_ISR_last=0x%x\n", tmp);
	return I2C_OK;
}


int i2c_wait_rd(uint32_t i2c_base_addr)
{
	int tmp = -1;

	rumboot_printf("I2C wait\n");
	iowrite32((0x4), (i2c_base_addr + I2C_IER)); //set interrupt enable

	while (tmp != 0x4) {
		tmp = ioread32(I2C_BASE + I2C_SR);
		tmp = 0x4 & tmp;
	} // while (tmp != 0x4)
	tmp = ioread32(i2c_base_addr + I2C_ISR);
	rumboot_printf("I2C_ISR_last=0x%x\n", tmp);
	return I2C_OK;
}

int i2c_write_array(uint32_t i2c_base_addr, uint32_t i2c_addr, uint32_t addr_h,
		    uint32_t addr_l, uint32_t byte_array_size, uint32_t run_wr, uint32_t data[])
{
	int i = 0;
	int tmp = -1;

	rumboot_printf("I2C WRITE OPERATION\n");
	// Write device address
	rumboot_printf("    Write device address\n");
	iowrite32((i2c_addr /* I2C_WR*/), (i2c_base_addr + I2C_TXBUF));

	// Write  high subaddress
	rumboot_printf("    Write high subaddr\n");
	iowrite32((addr_h), (i2c_base_addr + I2C_TXBUF));

	// Write low subaddress
	rumboot_printf("    Write low subaddress\n");

	iowrite32((addr_l), (i2c_base_addr + I2C_TXBUF));
	// Write data
	rumboot_printf("    Write data\n");
	// iowrite32((data),(i2c_base_addr + I2C_TXBUF));
	rumboot_printf("    START write data\n");
	for (i = 0; i < byte_array_size; i++) iowrite32((data[i]), (i2c_base_addr + I2C_TXBUF));
	iowrite32((run_wr), (i2c_base_addr + I2C_CR));  //run I2C write
	tmp = i2c_wait(i2c_base_addr);
	rumboot_printf("I2C wake up!\n");
	if (tmp == I2C_FAILED) return I2C_FAILED;
	rumboot_printf("    Write successful\n");
	return I2C_OK;
}

int i2c_write_array_last(uint32_t i2c_base_addr,
			 uint32_t byte_array_size, uint32_t run_wr_rpt, uint32_t data[])
{
	int i = 0;
	int tmp = -1;

	rumboot_printf("I2C WRITE REPEAT OPERATION\n");

	// Write data
	rumboot_printf("    Write data\n");
	// iowrite32((data),(i2c_base_addr + I2C_TXBUF));
	rumboot_printf("    START write data\n");
	for (i = 0; i < byte_array_size; i++) iowrite32((data[i + 4]), (i2c_base_addr + I2C_TXBUF));
	iowrite32((run_wr_rpt), (i2c_base_addr + I2C_CR));  //run I2C write
	tmp = i2c_wait_trn_compl(i2c_base_addr);
	rumboot_printf("I2C wake up!\n");
	if (tmp == I2C_FAILED) return I2C_FAILED;
	rumboot_printf("    Write successful\n");
	return I2C_OK;
}

unsigned  i2c_read_check_array(uint32_t i2c_base_addr, uint32_t i2c_addr, uint32_t addr_h, uint32_t addr_l,
			       uint32_t byte_array_size, uint32_t run_wr, uint32_t data[])
{
	int tmp = -1;
	int i = 0;
	unsigned result; // read data

	for (i = 0; i < (byte_array_size); i++) {
		if (i == 0) {
			rumboot_printf("I2C READ OPERATION\n");
			// Write device address
			rumboot_printf("    Write device address\n");
			iowrite32((i2c_addr /*+ I2C_WR*/), (i2c_base_addr + I2C_TXBUF));
			// rumboot_printf("i2c_addr_h =0x%x\n",i2c_addr_h);
			iowrite32((addr_h), (i2c_base_addr + I2C_TXBUF));
			//rumboot_printf("i2c_addr_l =0x%x\n",i2c_addr_l);
			iowrite32((addr_l), (i2c_base_addr + I2C_TXBUF));

			rumboot_printf("    READ data\n");

			iowrite32((i2c_addr /*+ I2C_RD */), (i2c_base_addr + I2C_TXBUF));
			rumboot_printf("    START read data\n");
			iowrite32((I2C_STAT_RST), (I2C_BASE + I2C_STATRST));//reset status
			iowrite32((run_wr), (i2c_base_addr + I2C_CR));
			tmp = i2c_wait_trn_compl(i2c_base_addr);

			if (tmp == I2C_FAILED) return I2C_FAILED;
			result = ioread32(i2c_base_addr + I2C_RXBUF);
			//rumboot_printf("read_num =0x%x\n",i);
			// rumboot_printf("check_array32 =0x%x\n",data[i]);
			//  rumboot_printf("i2c_read_result =0x%x\n",result);
			if (result != data[i]) {
				rumboot_printf("I2C test ERROR!\n");
				rumboot_printf("i2c_read_result =0x%x\n", result);
				return TEST_ERROR;
			}
		} else {
			iowrite32((I2C_BASE + I2C_RD), (I2C_BASE + I2C_TXBUF));
			result = ioread32(i2c_base_addr + I2C_RXBUF);
			// compare results
			if (result != data[i]) {
				rumboot_printf("I2C test ERROR!\n");
				rumboot_printf("i2c_read_result =0x%x\n", result);
				return TEST_ERROR;
			}
		}
	}

	return I2C_OK;
}


int main()
{
	int tmp = -1;
	uint32_t i2c_addr = 0x50;
	uint32_t i2c_base_addr = I2C_BASE;
	uint32_t addr_h = 0x00;         //write subaddress
	uint32_t addr_l = 0x00;
	//uint32_t size_array;

	uint32_t check_array32[] = {
		0x80,
		0x40,
		0x20,
		0x10,
		0x20,
		0x04,
		0xFE,
		0xFF
	};

//interrupt for last READ, interrupt for last write
	// size_array = sizeof(check_array32) / 4;
	tmp = i2c_initialization(SCL_FREQ);
	if (tmp == I2C_FAILED) {
		rumboot_printf("I2C init test ERROR!\n");
		return TEST_ERROR;
	}
	// WRITE operation
	tmp = i2c_write_array(i2c_base_addr, i2c_addr, addr_h,
			      addr_l, 4, 0x03 /*I2C_CR_WR_RUN_EN*/, check_array32);
	if (tmp == I2C_FAILED) {
		rumboot_printf("I2C write test ERROR!\n");
		return TEST_ERROR;
	}

	iowrite32((I2C_STAT_RST), (I2C_BASE + I2C_STATRST));
	tmp = ioread32(I2C_BASE + I2C_SR);
	rumboot_printf("i2c_read_STATUS =0x%x\n", tmp);

	tmp = i2c_write_array_last(i2c_base_addr, 4, 0x43, check_array32);
	if (tmp == I2C_FAILED) {
		rumboot_printf("I2C write repeat test ERROR!\n");
		return TEST_ERROR;
	}

	// do settings to avoid the influence of  write instruction

	iowrite32((0x0), (i2c_base_addr + I2C_STATRST));        //reset STATRST reg
	iowrite32((0x1), (i2c_base_addr + I2C_CR));           //reset START bit in CR_REG
	iowrite32((0x1), (i2c_base_addr + I2C_STATRST));        //reset STATUS reg

	// READ & COMPARE operation

	tmp = i2c_read_check_array(i2c_base_addr, i2c_addr, addr_h, addr_l,
				   7 /*size_array*/, I2C_CR_RPT_RD_RUN_EN, check_array32);

	if (tmp == I2C_FAILED) {
		rumboot_printf("I2C read test ERROR!\n");
		return TEST_ERROR;
	}

	rumboot_printf("I2C TEST OK \n");
	return I2C_OK;
}
