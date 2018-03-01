#ifndef I2C_CONSTANTS_H
#define I2C_CONSTANTS_H

//#define I2C_BASE I2C0_BASE

#define I2C_ARRAY_SIZE  0x4
#define I2C_CR_RD_RUN_EN    0x13
#define I2C_CR_RPT_RD_RUN_EN 0x33
#define I2C_CR_STAT_RST     0x08
#define I2C_STATRST     0x01
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


#endif