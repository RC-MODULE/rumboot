#include <rumboot/printf.h>

#include <devices/quad_spi.h> // offset in quad_spi`s memory map
#include "common_quad_spi.h"


uint32_t rand_control() {
  struct Reg_CONTROL x = {0, 0, 1, 0, 3, 2, 0, 0, 0, 0};

  uint32_t reg_x = x.rx_length << RX_LENGTH_LSB 
    | x.edq_mode << EDQ_MODE_LSB 
    | x.ss_source << SS_SOURCE 
    | x.sw_ss << SW_SS
    | x.length1 << LENGTH1_LSB
    | x.clock_divider << CLOCK_DIVIDER_LSB
    | x.phase << PHASE
    | x.polarity << POLARITY
    | x.frame_format << FRAME_FORMAT_LSB
    | x.length0 << LENGTH0_LSB
    ;

  return reg_x;

  // random 
  uint32_t edq_mode;

  uint32_t rx_length;
 
  do {
    rx_length = randomize(0, 63) & (~1); // only even
  } while(rx_length != 1 && rx_length != 2); // it's reserve
  rx_length = rx_length << RX_LENGTH_LSB;




  return 0x10;
  // rumboot_printf("rx_length=%d", rx_length);
  // rumboot_printf("rx_length=%d", rx_length);
}


uint32_t rand_fifo_watermark() {
  struct Reg_FIFO_WATERMARK x = {7, 7};

  uint32_t reg_x = x.tx_fifo_irq_level << TX_FIFO_IRQ_LEVEL_LSB 
    | x.rx_fifo_irq_level << RX_FIFO_IRQ_LEVEL_LSB
    ;

  return reg_x;
}



void print(struct Reg_CONTROL reg) {
  rumboot_printf("%d%d%d%d%d%d%d%d%d%d", reg.rx_length, reg.edq_mode, reg.ss_source, reg.sw_ss, reg.length1, reg.clock_divider, reg.phase, reg.polarity, reg.frame_format, reg.length0);
}

