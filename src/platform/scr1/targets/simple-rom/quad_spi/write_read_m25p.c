// Test: write and read count_iteration data in flash M25P10A via the Quad_SPI controller

#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>

#include <platform/devices.h> // memory map scr1
#include <regs/quad_spi.h> // regs names Quad SPI
#include <platform/devices/quad_spi.h> // lib for work with Quad SPI

#include <platform/common_for_tests.h>
// #include <platform/quad_spi/common_quad_spi.h>

#include <stdio.h>
#include <stdint.h> // for uint32_t
#include <assert.h>


int main() {
  rumboot_printf("TEST write_read_m25p start\n");
  
  const unsigned char byte = 8;

  // command FLASH M25P10A
  const int M25P_COMMAND_WRITE_ENABLE = 0x06;
  const int M25P_COMMAND_READ_STATUS_REGISTER = 0x05;
  const int M25P_COMMAND_SECTOR_ERASE = 0xD8;
  const int M25P_COMMAND_PAGE_PROGRAM = 0x02;
  const int M25P_COMMAND_READ_DATA_BYTES = 0x03;

  const int M25P_WIP = 0;
  const int M25P_WEL = 1;

  // configuration for FLASH M25P10A
  Quad_spi_config m25p_config = {
    .mode = SPI,
    .base_frequency = 5200, // ?????
    .out_frequency = 50, // ?????
    .phase = 1, // вроде так по аналогии с task spi_flash_model
    .polarity = 1, // вроде так
    .ss_source = HARDWARE,
    .rx_data_size = 8, // потом меняется
    .tx_data_size = 8, // потом меняется
    .empty_data_size = 0,
    .tx_fifo_irq_level = 0,
    .rx_fifo_irq_level = 7
  };
  
  uint64_t status_register;
  uint64_t read_data;

  // M25P_COMMAND_WRITE_ENABLE
  m25p_config.tx_data_size = byte;
  m25p_config.rx_data_size = byte;
  assert (!quad_spi_init(SPI_CONTROL_BASE, &m25p_config));
  assert (!quad_spi_write_data(
    SPI_CONTROL_BASE, 
    SPI_FIFO_BASE, 
    m25p_config.tx_data_size, 
    M25P_COMMAND_WRITE_ENABLE
  ));

  // M25P_COMMAND_READ_STATUS_REGISTER
  m25p_config.tx_data_size = byte + 1*byte;
  m25p_config.rx_data_size = byte + 1*byte;
  assert (!quad_spi_init(SPI_CONTROL_BASE, &m25p_config));
  assert (!quad_spi_write_data(
    SPI_CONTROL_BASE, 
    SPI_FIFO_BASE, 
    m25p_config.tx_data_size, 
    M25P_COMMAND_READ_STATUS_REGISTER << byte
  ));
  assert (!quad_spi_read_data(
    SPI_CONTROL_BASE, 
    SPI_FIFO_BASE, 
    m25p_config.rx_data_size, 
    &status_register
  ));

  // M25P_COMMAND_SECTOR_ERASE
  if(bit_in_reg(status_register, M25P_WEL)) {
    m25p_config.tx_data_size = byte + 3*byte;
    m25p_config.rx_data_size = byte + 3*byte;
    assert (!quad_spi_init(SPI_CONTROL_BASE, &m25p_config));
    assert (!quad_spi_write_data(
      SPI_CONTROL_BASE, 
      SPI_FIFO_BASE, 
      m25p_config.tx_data_size, 
      M25P_COMMAND_SECTOR_ERASE << 3*byte
    )); 
    assert (!quad_spi_read_data(
      SPI_CONTROL_BASE, 
      SPI_FIFO_BASE, 
      m25p_config.rx_data_size, 
      &read_data
    )); // clear rx-buffer

    do {
      // M25P_COMMAND_READ_STATUS_REGISTER
      m25p_config.tx_data_size = byte + 1*byte;
      m25p_config.rx_data_size = byte + 1*byte;
      assert (!quad_spi_init(SPI_CONTROL_BASE, &m25p_config));
      assert (!quad_spi_write_data(
        SPI_CONTROL_BASE, 
        SPI_FIFO_BASE, 
        m25p_config.tx_data_size, 
        M25P_COMMAND_READ_STATUS_REGISTER << byte
      ));
      assert (!quad_spi_read_data(
        SPI_CONTROL_BASE, 
        SPI_FIFO_BASE, 
        m25p_config.rx_data_size, 
        &status_register
      ));
    } while(bit_in_reg(status_register, M25P_WIP));
  }
  
  // write in flash
  rumboot_printf("WRITE IN FLASH\n");

  const int count_iterations = 5; // any
  uint32_t address[] = {0x0, 0x100, 0x200, 0x300, 0x400}; // any
  unsigned char data_size[] = {byte, 2*byte, 3*byte, 4*byte, byte}; // any
  uint32_t write_data[] = {0x1, 0x2, 0x3, 0x4, 0x5}; // any

  for(int i = 0; i < count_iterations; ++i) {
    rumboot_printf("WRITE_ITERATION_%d\n", i);

    // M25P_COMMAND_WRITE_ENABLE
    m25p_config.tx_data_size = byte;
    m25p_config.rx_data_size = byte;
    assert (!quad_spi_init(SPI_CONTROL_BASE, &m25p_config));
    assert (!quad_spi_write_data(
      SPI_CONTROL_BASE, 
      SPI_FIFO_BASE, 
      m25p_config.tx_data_size, 
      M25P_COMMAND_WRITE_ENABLE
    )); 

    // M25P_COMMAND_PAGE_PROGRAM
    m25p_config.tx_data_size = byte + 3*byte + data_size[i]; // >= 33 = command+addr+data = 8+3*8+...
    m25p_config.rx_data_size = byte + 3*byte + data_size[i];
    assert (!quad_spi_init(SPI_CONTROL_BASE, &m25p_config));
    assert (!quad_spi_write_data(
      SPI_CONTROL_BASE, 
      SPI_FIFO_BASE, 
      m25p_config.tx_data_size, 
      ((uint64_t)M25P_COMMAND_PAGE_PROGRAM) << (3*byte + data_size[i]) | 
      ((uint64_t)address[i]) << data_size[i] |
      ((uint64_t)write_data[i])
    )); 
    assert (!quad_spi_read_data(
      SPI_CONTROL_BASE, 
      SPI_FIFO_BASE, 
      m25p_config.rx_data_size, 
      &read_data
    )); // clear rx-buffer

    do {
      // M25P_COMMAND_READ_STATUS_REGISTER
      m25p_config.tx_data_size = byte + 1*byte;
      m25p_config.rx_data_size = byte + 1*byte;
      assert (!quad_spi_init(SPI_CONTROL_BASE, &m25p_config));
      assert (!quad_spi_write_data(
        SPI_CONTROL_BASE, 
        SPI_FIFO_BASE, 
        m25p_config.tx_data_size, 
        M25P_COMMAND_READ_STATUS_REGISTER << byte
      ));
      assert (!quad_spi_read_data(SPI_CONTROL_BASE, SPI_FIFO_BASE, m25p_config.rx_data_size, &status_register));
    } while(bit_in_reg(status_register, M25P_WIP));
  }

  // read from flash
  rumboot_printf("READ FROM FLASH\n");
  
  for(int i = 0; i < count_iterations; ++i) {
    rumboot_printf("READ_ITERATION_%d\n", i);

    // M25P_COMMAND_READ_DATA_BYTES
    m25p_config.tx_data_size = byte + 3*byte + data_size[i];
    m25p_config.rx_data_size = byte + 3*byte + data_size[i];
    assert (!quad_spi_init(SPI_CONTROL_BASE, &m25p_config));
    assert (!quad_spi_write_data(
      SPI_CONTROL_BASE, 
      SPI_FIFO_BASE, 
      m25p_config.tx_data_size, 
      ((uint64_t)M25P_COMMAND_READ_DATA_BYTES) << (3*byte + data_size[i]) | 
      ((uint64_t)address[i]) << data_size[i]
    ));
    assert (!quad_spi_read_data(SPI_CONTROL_BASE, SPI_FIFO_BASE, m25p_config.rx_data_size, &read_data));
    
    rumboot_printf("write_data[%d]=%x, read_data[%d]=%x\n", i, write_data[i], i, (uint32_t)(read_data & 0xFFFFFFFF));
    assert (write_data[i] == (uint32_t)(read_data & 0xFFFFFFFF));
  }

  rumboot_printf("TEST write_read_m25p PASSED\n");
  return 0;
}

////////////////////////////////////////////////////////////////////////////////////
  // // set mask interrupt
  // Quad_spi_mask mask = {
  //   .tx_empty = 0,
  //   .rx_irq = 0,
  //   .tx_irq = 0,
  //   .rx_overrun = 0
  // };

  // assert (!quad_spi_set_mask(SPI_CONTROL_BASE, &mask));
 
  // // reset irq
  // Quad_spi_irq_status irq_status;

  // quad_spi_reset_irq(SPI_CONTROL_BASE, &irq_status);
  // rumboot_printf("irq_status=%d%d%d%d\n", 
  //   irq_status.tx_empty, irq_status.rx_irq, irq_status.tx_irq, irq_status.rx_overrun
  // );

  // // push data in tx-buf
  // uint64_t data[5] = {0x11, 0x12, 0x13, 0x14, 0x15};
  // int count_iterations = 5;
  
  // for(int i = 0; i < count_iterations; ++i) {
  //   if(!quad_spi_tx_fifo_full(SPI_CONTROL_BASE)) {
  //     quad_spi_write_data(SPI_FIFO_BASE, m25p_config.tx_data_size, data[i]);
  //     rumboot_printf("data[%d]=%x\n", i, data[i]);
  //     quad_spi_reset_irq(SPI_CONTROL_BASE, &irq_status);
  //     rumboot_printf("irq_status=%d%d%d%d\n", 
  //       irq_status.tx_empty, irq_status.rx_irq, irq_status.tx_irq, irq_status.rx_overrun
  //     );
  //   }
  // }

  // // pop data from rx-buf
  // int is_empty = 1;
  // uint64_t read_data;
  
  // for(int i = 0; i < count_iterations; ++i) {
  //   do {
  //     is_empty = quad_spi_rx_fifo_empty(SPI_CONTROL_BASE);
  //   } while(is_empty);

  //   quad_spi_read_data(SPI_FIFO_BASE, m25p_config.rx_data_size, &read_data);
  //   rumboot_printf("read_data[%d]=%x\n", i, read_data);
  //   assert (read_data == data[i]);
  // }

//////////////////////////////////////////////////////////////////////////////////////

  // rumboot_printf("fifo_status=%x", ioread32(SPI_CONTROL_BASE + FIFO_STATUS));


  // uint64_t data1;
  // int is_empty = 1;
  // do {
  //   is_empty = quad_spi_rx_fifo_empty(SPI_CONTROL_BASE);
  // } while(is_empty);
  // if(!quad_spi_rx_fifo_empty(SPI_CONTROL_BASE)) {
    // quad_spi_read_data(SPI_FIFO_BASE, config.rx_data_size, &data1);
  // }

  // rumboot_printf("IRQ_STATUS=%x\n", ioread32(SPI_CONTROL_BASE + IRQ_STATUS));
  

  // assert (ioread32(SPI_CONTROL_BASE + IRQ_STATUS) == 0xA);

  
  // if(!quad_spi_tx_fifo_full(SPI_CONTROL_BASE)) {
    // quad_spi_write_data(SPI_FIFO_BASE, config.tx_data_size, 0x12);
  // }


  // for(int i = 0; i < 10; ++i) {
    // rumboot_printf("fifo_status=%x", ioread32(SPI_CONTROL_BASE + FIFO_STATUS));
  // }

  // quad_spi_reset_irq(SPI_CONTROL_BASE, &irq_status);
  // rumboot_printf("irq_status=%x\n", irq_status);
  

//////////////////////////////////////////////////////////////////////////////////////

  // // other functions
  // rumboot_printf("mode=%s\n", Mode_str[config.mode]);

  // int sum = summa(2, 1, 2);

  // rumboot_printf("irq_status\ntx_empty=%x\nrx_irq=%x\ntx_irq=%x\nrx_overrun=%x\n", 
  //   irq_status.tx_empty, irq_status.rx_irq, irq_status.tx_irq, irq_status.rx_overrun);