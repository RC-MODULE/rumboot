#ifndef REGS_I2C_H
#define REGS_I2C_H


//REGISTERS
#define I2C_ID    0x0
#define I2C_ISR    0x004
#define I2C_IER    0x008
#define I2C_SOFTR   0x00C
#define I2C_TRANSMIT  0x018
#define I2C_RECEIVE   0x020
#define I2C_CTRL   0x010
#define I2C_NUMBER   0x01C
#define I2C_STATUS   0x014
#define I2C_PRESCALE  0x024
#define I2C_FIFOFIL   0x028
#define I2C_STAT_RST   0x02C

//Interrupt enable register
#define EN_INT_DONE 0
#define EN_INT_AL 1
#define EN_INT_TRN_EMPTY 2
#define EN_INT_RCV_FULL 3
#define EN_INT_TX_AL_EMPTY 4
#define EN_INT_RX_AL_FULL 5
#define EN_INT_NADDR 6
#define EN_INT_NACK 8

//Interrupt status register
#define INT_DONE 0
#define INT_AL 1
#define INT_TRN_EMPTY_i 2
#define INT_RCV_FULL 3
#define INT_TRN_EMPTY_ALM_i 4
#define INT_RCV_FULL_ALM_i 5
#define INT_NADDR_i 6
#define INT_NACK_i 7

//Status register
#define IBUSY_i 0
#define AL_i 1
#define DONE_i 2
#define RX_FULL_ALM_i 3
#define TX_EMPTY_i 4
#define RX_FULL_i 5
#define TX_FULL_i 6
#define RX_EMPTY_i 7
#define NADDR_i 8
#define NACK_i 9

//Control register
#define I2C_EN_i 0
#define START_i 1
#define RD_i 3
#define WR_i 4
#define REPEAT_i 5
#define STOP_i 6

#endif
