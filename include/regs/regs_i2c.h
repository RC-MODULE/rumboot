//REGISTERS
#define I2C_ID    0x0
#define I2C_ISR    0x004
#define I2C_IER    0x008
#define I2C_SOFTR   0x00C
#define I2C_TXBUF  0x018
#define I2C_RXBUF   0x020
#define I2C_CR   0x010
#define I2C_NUMB   0x01C
#define I2C_SR   0x014
#define I2C_CLKPR  0x024
#define I2C_FIFOFIL   0x028
#define I2C_STATRST   0x02C

//Interrupt status register
#define INT_TRN_EMPTY 2
#define INT_NADDR_i 6
#define INT_NACK_i 7

//Status register
#define IBUSY_i 0
#define RX_FULL_i 5 

//Control register
#define I2C_EN_i 0
#define START_i 1
#define RD_i 3
#define WR_i 4
#define REPEAT_i 5
#define STOP_i 6
