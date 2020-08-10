// Test: loopback Quad_SPI controller

#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>

#include <platform/devices.h> // memory map scr1
#include <regs/quad_spi.h> // regs names Quad SPI
#include <platform/devices/quad_spi.h> // lib for work with Quad SPI

#include <platform/common_for_tests.h>

#include <stdio.h>
#include <stdint.h> // for uint32_t
#include <assert.h>


int main() {
  rumboot_printf("TEST loopback start\n");
  
  // configuration
  Quad_spi_config config = {
    .mode = SPI,
    .base_frequency = 2500,
    .out_frequency = 50,
    .phase = 1, 
    .polarity = 1,
    .ss_source = HARDWARE,
    .rx_data_size = 8,
    .tx_data_size = 8,
    .empty_data_size = 0,
    .tx_fifo_irq_level = 0,
    .rx_fifo_irq_level = 7
  };

  // set configuration
  assert (!quad_spi_init(SPI_CONTROL_BASE, &config));
  
  // write in tx-buffer
  const int count_iteration = 5; // any
  uint64_t write_data[] = {0x1, 0x2, 0x3, 0x4, 0x5}; // any

  rumboot_printf("WRITE IN TX-BUFFER\n");
  for(int i = 0; i < count_iteration; ++i) {
    rumboot_printf("WRITE_ITERATION_%d\n", i);
    assert (!quad_spi_write_data(
      SPI_CONTROL_BASE, 
      SPI_FIFO_BASE, 
      config.tx_data_size, 
      write_data[i]   
    ));
  }

  // read from rx-buffer
  uint64_t read_data;

  rumboot_printf("READ FROM RX-BUFFER\n");

  for(int i = 0; i < count_iteration; ++i) {
    rumboot_printf("READ_ITERATION_%d\n", i);
    assert (!quad_spi_read_data(SPI_CONTROL_BASE, SPI_FIFO_BASE, config.rx_data_size, &read_data));
    
    rumboot_printf("write_data[%d]=%x, ", i, write_data[i]);
    rumboot_printf("read_data[%d]=%x\n", i, (uint32_t)(read_data & 0xFFFFFFFF));
    assert (write_data[i] == (uint32_t)(read_data & 0xFFFFFFFF));
  }


  rumboot_printf("TEST loopback PASSED\n");
  return 0;
}
