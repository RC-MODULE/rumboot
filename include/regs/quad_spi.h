#define ID                  0x00
#define VERSION             0x04
#define CONTROL             0x08
#define FIFO_WATERMARK      0x0C
#define IRQ_STATUS          0x10
#define MASK                0x14
#define FIFO_STATUS         0x18
#define SW_RESET            0x1C
#define RX_DATA0            0x20
#define RX_DATA1            0x24
#define TX_DATA0            0x228
#define TX_DATA1            0x22C

// CONTROL
#define RX_LENGTH_MSB 31
#define RX_LENGTH_LSB 26
#define EDQ_MODE_MSB  25
#define EDQ_MODE_LSB  24
#define SS_SOURCE     23
#define SW_SS 22
#define LENGTH1_MSB 21
#define LENGTH1_LSB 16
#define CLOCK_DIVIDER_MSB 15
#define CLOCK_DIVIDER_LSB 8
#define PHASE 7
#define POLARITY 6
#define FRAME_FORMAT_MSB 5
#define FRAME_FORMAT_LSB 4
#define LENGTH0_MSB 3
#define LENGTH0_LSB 0

// FIFO_WATERMARK
#define TX_FIFO_IRQ_LEVEL_MSB 5
#define TX_FIFO_IRQ_LEVEL_LSB 3
#define RX_FIFO_IRQ_LEVEL_MSB 2
#define RX_FIFO_IRQ_LEVEL_LSB 0

// IRQ_STATUS, MASK
#define TX_EMPTY 3
#define RX_IRQ 2
#define TX_IRQ 1
#define RX_OVERRUN 0

// FIFO_STATUS
#define SPI_BUSY 12
#define RX_FIFO_FULL 11
#define RX_FIFO_NOT_EMPTY 10
#define TX_FIFO_NOT_FULL 9
#define TX_FIFO_EMPTY 8
#define RX_WORD_AMOUNT_MSB 7
#define RX_WORD_AMOUNT_LSB 4
#define TX_WORD_AMOUNT_MSB 3
#define TX_WORD_AMOUNT_LSB 0

// SW_RESET
#define SW_RESET_MSB 0 // other name
