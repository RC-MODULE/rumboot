#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>

#include <platform/devices.h> // memory map
#include <devices/quad_spi.h> // offset in quad_spi`s memory map

#include "common_for_tests.h"

#include <stdio.h>
#include <stdint.h> // for uint32_t


int main() {
  rumboot_printf("TEST_PROBA\n");

  // check ID, VSERSION
  assert (ioread32(SPI_CONTROL_BASE + ID) == 0xCC9B9);
  assert (ioread32(SPI_CONTROL_BASE + VERSION) == 0x1);

  // check, that controller doesn't work
  uint32_t fifo_status; 
  do {
    fifo_status = ioread32(SPI_CONTROL_BASE + FIFO_STATUS);
  } while(bit_in_reg(fifo_status, SPI_BUSY));

  // program reset
  uint32_t control = ioread32(SPI_CONTROL_BASE + CONTROL);
  iowrite32(0x1, SPI_CONTROL_BASE + SW_RESET); 
  uint32_t sw_reset;
  do {
    sw_reset = ioread32(SPI_CONTROL_BASE + SW_RESET);
  } while(bit_in_reg(sw_reset, SW_RESET));

  // check regs after program reset
  assert (control == ioread32(SPI_CONTROL_BASE + CONTROL));
  assert (ioread32(SPI_CONTROL_BASE + FIFO_WATERMARK) == 0x0);
  assert (ioread32(SPI_CONTROL_BASE + IRQ_STATUS) == 0xE);
  assert (ioread32(SPI_CONTROL_BASE + MASK) == 0x0);
  assert (ioread32(SPI_CONTROL_BASE + FIFO_STATUS) == 0x300);
  assert (ioread32(SPI_CONTROL_BASE + SW_RESET) == 0x0);
  assert (ioread32(SPI_CONTROL_BASE + RX_DATA0) == 0x0);
  assert (ioread32(SPI_CONTROL_BASE + RX_DATA1) == 0x0);
  assert (ioread32(SPI_CONTROL_BASE + TX_DATA0) == 0x0);
  assert (ioread32(SPI_CONTROL_BASE + TX_DATA1) == 0x0);
 
  // iowrite32()
  // rumboot_printf("rand:\n");
  // for(int i = 0; i < 50; ++i) {
  //   rumboot_printf("%d ", randomize(1, 5));
  // }
  // rumboot_printf("rand.\n");

  // rumboot_printf("rand_control=%x\n", rand_control());
  iowrite32(rand_control(), SPI_CONTROL_BASE + CONTROL);
  iowrite32(rand_fifo_watermark(), SPI_CONTROL_BASE + FIFO_WATERMARK);
  rumboot_printf("SPI_CONTROL_BASE + CONTROL=%x\n", SPI_CONTROL_BASE + CONTROL);
  iowrite32(0x11, SPI_FIFO_BASE + TX_DATA1);
  rumboot_printf("SPI_FIFO_BASE + TX_DATA1=%x\n", SPI_FIFO_BASE + TX_DATA1);
  
  // do {
  //   fifo_status = ioread32(SPI_CONTROL_BASE + FIFO_STATUS);
  // } while(bit_in_reg(fifo_status, SPI_BUSY));

  rumboot_printf("TEST PASSED\n");
  return 0;
}
