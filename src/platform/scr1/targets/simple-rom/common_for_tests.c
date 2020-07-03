#include <rumboot/printf.h>

#include <devices/quad_spi.h> // offset in quad_spi`s memory map
#include "common_for_tests.h"

#include <stdlib.h> // for rand()
#include <time.h> // for srand()
// #include <assert.h>


uint32_t bit_in_reg(uint32_t reg, int bit) {
  return (reg & (1 << bit)) >> bit;
}


uint32_t bits_in_reg(uint32_t reg, int msb, int lsb) {
  uint32_t mask = 1 << lsb;

  for(int k = 1; k <= msb - lsb; ++k) {
    mask = mask | (mask << k);
  }

  return (reg & mask) >> lsb;
}


int randomize(int min, int max) { // random in [min, max]
  srand(time(NULL) + rand());
  
  int temp = max - min;
  temp = temp ? temp : 1;
  ++temp;

  return rand() % temp + min;
}


void print(struct Reg_CONTROL reg) {
  rumboot_printf("%d%d%d%d%d%d%d%d%d%d", reg.rx_length, reg.edq_mode, reg.ss_source, reg.sw_ss, reg.length1, reg.clock_divider, reg.phase, reg.polarity, reg.frame_format, reg.length0);
}


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

// int sum(int n, ...) {
//   int res = 0;
//   rumboot_printf("n=%d",n);
//   for(int *p = &n; n>0; n--){
//     rumboot_printf(*p);
//     res += *(++p);
//   }
//   return res;
// }


// uint32_t bits(uint32_t number, int count_bits, uint32_t left_border, ...) {
//   rumboot_printf("number=%x, count_bits=%d", number, count_bits);
//   uint32_t *p = &left_border;
//   uint32_t left = *p;
//   rumboot_printf("left=%d", left);
//   // ++p;
//   // uint32_t t = p;
//   // rumboot_printf("left=%d", *t);


//   if(count_bits == 1) {
//     return (number & (1 << left)) >> left;
//   } else {
//     rumboot_printf("point1");
//     int right;
//     // p++;
//     rumboot_printf("point2");
//     right = *(++p);
//     // right = *p;
//     rumboot_printf("point3");
//     rumboot_printf("right=%d", right);

//     uint32_t mask = 1 << right;
//     for(int k = 1; k <= left - right; ++k) {
//       mask = mask | (mask << k);
//     }

//     return (number & mask) >> right;
//   }
//   // return 0x10;
// }
