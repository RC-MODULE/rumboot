// Test: write and read count_iteration data in flash W25Q32JV via the Quad_SPI controller

#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>

#include <platform/devices.h> // memory map scr1
#include <regs/quad_spi.h> // regs names Quad SPI
#include <devices/quad_spi.h> // lib for work with Quad SPI

#include <platform/common_for_tests.h>

#include <stdio.h>
#include <stdint.h> // for uint32_t


int main() {
  rumboot_printf("TEST write_read_w25q start\n");
  
  const unsigned char byte = 8;

  // command FLASH W25Q32JV
  const int W25Q_COMMAND_WRITE_ENABLE = 0x06;

  uint64_t read_data;

  // configuration for FLASH W25Q32JV
  Quad_spi_config w25q_config = {
    .base_frequency = 2500, // ?????
    .out_frequency = 50, // ?????
    .phase = 1, // вроде так по аналогии с task spi_flash_model_W25Q32JV
    .polarity = 1, // вроде так по аналогии с task spi_flash_model_W25Q32JV
    .ss_source = HARDWARE,
    .empty_data_size = 0,
    .tx_fifo_irq_level = 0,
    .rx_fifo_irq_level = 7
  };

  // M25P_COMMAND_WRITE_ENABLE
  w25q_config.mode = SPI;
  w25q_config.rx_data_size = byte;
  w25q_config.tx_data_size = byte; 
  assert (!quad_spi_init(SPI_CONTROL_BASE, &w25q_config));
  assert (!quad_spi_write_data(
    SPI_CONTROL_BASE, 
    SPI_FIFO_BASE, 
    w25q_config.tx_data_size, 
    W25Q_COMMAND_WRITE_ENABLE
  ));
  assert (!quad_spi_read_data(
    SPI_CONTROL_BASE, 
    SPI_FIFO_BASE, 
    w25q_config.rx_data_size, 
    &read_data
  ));



  // W25Q_COMMAND_READ_STATUS_REGISTER
  w25q_config.rx_data_size = 2*byte;



  rumboot_printf("TEST write_read_w25q PASSED\n");
  return 0;
}
