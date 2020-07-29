#include <platform/devices/quad_spi.h>
#include <regs/quad_spi.h> // regs names Quad SPI

#include <platform/common_for_tests.h>

#include <rumboot/printf.h>
#include <rumboot/io.h>

#include <stdint.h> // for uintptr_t, uint32_t
// #include <time.h>
#include <assert.h>

#define DEBUG
#ifdef DEBUG
  #define dbg(x, ...) rumboot_printf("qspi: " x, ##__VA_ARGS__) 
#else
  #define dbg(x, ...)
#endif

static int generate_control_reg(Quad_spi_config const *config, uint32_t *control) {
  struct Reg_control {
    uint32_t rx_length : 6;
    uint32_t edq_mode : 2;
    uint32_t ss_source : 1;
    uint32_t sw_ss : 1;
    uint32_t length1 : 6;
    uint32_t clock_divider : 8;
    uint32_t phase : 1;
    uint32_t polarity : 1;
    uint32_t frame_format : 2;
    uint32_t length0 : 4;
  } __attribute__((packed));

  struct Reg_control reg;

  switch(config->mode) {
    case SPI:
      reg.edq_mode = 0x0;
      reg.frame_format = 0x0;

      // check correctness input data
      assert (config->rx_data_size == config->tx_data_size);
      assert ((config->tx_data_size >= 4) && (config->tx_data_size <= 64));
      
      reg.rx_length = 0x0; // reserve
      reg.length1 = config->tx_data_size - 1;
      reg.length0 = 0x0; // reserve
      break;

    case DUAL:
      reg.edq_mode = 0x1;
      reg.frame_format = 0x0;

      // check correctness input data
      assert ((config->tx_data_size >= 4) && (config->tx_data_size <= 64) && !(config->tx_data_size % 2));
      assert (((config->empty_data_size >=1) && (config->empty_data_size <= 15)) 
        || (config->empty_data_size == 0));
      assert ((config->rx_data_size >= 4) && (config->rx_data_size <= 64) && !(config->rx_data_size % 2) );

      reg.rx_length = config->rx_data_size - 1;
      reg.length1 = config->tx_data_size - 1;
      reg.length0 = config->empty_data_size;
      break;

    case QUAD:
      reg.edq_mode = 0x2;
      reg.frame_format = 0x0;

      // check correctness input data
      assert (((config->rx_data_size >= 8) && (config->rx_data_size <= 64) && !(config->rx_data_size % 4))
        || (config->rx_data_size == 0));
      assert ((config->empty_data_size >= 0) && (config->empty_data_size <= 15));
      assert ((config->tx_data_size >= 8) && (config->tx_data_size <= 64) && !(config->tx_data_size % 4));
  
      reg.rx_length = config->rx_data_size - 1;
      reg.length1 = config->tx_data_size - 1;
      reg.length0 = config->empty_data_size;
      break;

    case TEXAS_INSTRUMENTS:
      reg.edq_mode = 0x0;
      reg.frame_format = 0x1;
      reg.rx_length = 0x0; // reserve
      
      // check correctness input data
      assert (config->rx_data_size == config->tx_data_size);
      assert ((config->rx_data_size >= 4) && (config->rx_data_size <= 64));

      reg.length1 = config->rx_data_size - 1;
      reg.length0 = 0x0; //reserve
      break;

    case MICROWIRE:
      reg.edq_mode = 0x0;
      reg.frame_format = 0x2;

      // check correctness input data
      assert ((config->tx_data_size >= 8) && (config->tx_data_size <= 16));
      assert ((config->rx_data_size >= 8) && (config->rx_data_size <= 64));

      reg.rx_length = 0x0; //reserve
      reg.length1 = config->rx_data_size - 1;
      reg.length0 = config->tx_data_size - 1;
      break;

    default:
      reg.rx_length = 0x0;
      reg.length0 = 0x0;
      reg.frame_format = 0x0;
      reg.length1 = 0x0;
      reg.edq_mode = 0x0;
      break;
  }

  reg.ss_source = config->ss_source;
  reg.sw_ss = 0x0;

  reg.clock_divider = config->base_frequency / (2 * config->out_frequency); // integer division
  assert ((reg.clock_divider >= 1) && (reg.clock_divider <= 255));
  
  reg.phase = config->phase;
  reg.polarity = config->polarity;

  // rumboot_printf(
  //   "rx_length=%x\nedq_mode=%x\nss_source=%x\nsw_ss=%x\nlength1=%x\nclock_divider=%x\nphase=%x\npolarity=%x\nframe_format=%x\nlength0=%x\n", 
  //   reg.rx_length, reg.edq_mode, reg.ss_source, reg.sw_ss, reg.length1, 
  //   reg.clock_divider, reg.phase, reg.polarity, reg.frame_format, reg.length0
  // );

  *control = reg.rx_length << RX_LENGTH_LSB 
    | reg.edq_mode << EDQ_MODE_LSB 
    | reg.ss_source << SS_SOURCE 
    | reg.sw_ss << SW_SS
    | reg.length1 << LENGTH1_LSB
    | reg.clock_divider << CLOCK_DIVIDER_LSB
    | reg.phase << PHASE
    | reg.polarity << POLARITY
    | reg.frame_format << FRAME_FORMAT_LSB
    | reg.length0 << LENGTH0_LSB
    ;

  return 0;
}


static int generate_fifo_watermark_reg(Quad_spi_config const *config, uint32_t *fifo_watermark) {
  // check correctness input data
  assert ((config->tx_fifo_irq_level >= 0) && (config->tx_fifo_irq_level <= 7));
  assert ((config->rx_fifo_irq_level >= 0) && (config->rx_fifo_irq_level <= 7));

  *fifo_watermark = (config->tx_fifo_irq_level) << TX_FIFO_IRQ_LEVEL_LSB
    | (config->rx_fifo_irq_level) << RX_FIFO_IRQ_LEVEL_LSB
    ;

  return 0;
}


int quad_spi_init(uintptr_t base, Quad_spi_config const *config) {
  dbg("quad_spi_init\n");

  // check, that controller doesn't work in current moment
  int is_busy;

  do {
    is_busy = quad_spi_busy(base);
  } while(is_busy);

  // program reset
  iowrite32(0x1, base + SW_RESET); 
  
  uint32_t sw_reset;
  
  do {
    sw_reset = ioread32(base + SW_RESET);
  } while(bit_in_reg(sw_reset, SW_RESET_MSB));

  // check regs after program reset
  // assert (ioread32(base + ID) == 0xCC9B9);
  // assert (ioread32(base + VERSION) == 0x1);
  // assert (ioread32(base + FIFO_WATERMARK) == 0x0);
  // assert (ioread32(base + IRQ_STATUS) == 0xE);
  // assert (ioread32(base + MASK) == 0x0);
  // assert (ioread32(base + FIFO_STATUS) == 0x300);
  // assert (ioread32(base + SW_RESET) == 0x0);
  // assert (ioread32(base + RX_DATA0) == 0x0);
  // assert (ioread32(base + RX_DATA1) == 0x0);
  // assert (ioread32(base + TX_DATA0) == 0x0);
  // assert (ioread32(base + TX_DATA1) == 0x0);

  // set configuration
  uint32_t control, fifo_watermark;

  assert (!generate_control_reg(config, &control));
  iowrite32(control, base + CONTROL);

  assert (!generate_fifo_watermark_reg(config, &fifo_watermark));
  iowrite32(fifo_watermark, base + FIFO_WATERMARK);
  
  return 0;
}


int quad_spi_set_mask(uintptr_t base, Quad_spi_mask const *mask) {
  dbg("quad_spi_set_mask\n");

  uint32_t reg = (mask->tx_empty) << TX_EMPTY
    | (mask->rx_irq) << RX_IRQ
    | (mask->tx_irq) << TX_IRQ
    | (mask->rx_overrun) << RX_OVERRUN
    ;

  iowrite32(reg, base + MASK);

  return 0;
}


int quad_spi_tx_fifo_full(uintptr_t base) {
  dbg("quad_spi_tx_fifo_full\n");

  return bit_in_reg(ioread32(base + FIFO_STATUS), TX_FIFO_NOT_FULL) ? 0 : 1;
}


int quad_spi_rx_fifo_empty(uintptr_t base) {
  dbg("quad_spi_rx_fifo_empty\n");

  return bit_in_reg(ioread32(base + FIFO_STATUS), RX_FIFO_NOT_EMPTY) ? 0 : 1;
}


int quad_spi_busy(uintptr_t base) {
  dbg("quad_spi_busy\n");

  return bit_in_reg(ioread32(base + FIFO_STATUS), SPI_BUSY) ? 1 : 0;
}


int quad_spi_write_data(uintptr_t control_base, uintptr_t fifo_base, unsigned int size, uint64_t const data) {
  if(quad_spi_tx_fifo_full(control_base)) {
    return 1;
  }

  dbg("quad_spi_write_data\n");

  if(size > 32) {
    iowrite32((uint32_t)(data & 0xFFFFFFFF), fifo_base + TX_DATA1);
    iowrite32((uint32_t)(data >> 32), fifo_base + TX_DATA0);
  } else {
    iowrite32((uint32_t)(data & 0xFFFFFFFF), fifo_base + TX_DATA1);
  }

  return 0;
}


int quad_spi_read_data(uintptr_t control_base, uintptr_t fifo_base, unsigned int size, uint64_t *data) {
  int is_empty;

  do {
    is_empty = quad_spi_rx_fifo_empty(control_base);
  } while(is_empty);

  dbg("quad_spi_read_data\n");

  uint32_t lsbs = ioread32(fifo_base + RX_DATA1);
  uint32_t msbs = 0x0;

  if(size > 32) {
    msbs = ioread32(fifo_base + RX_DATA0);
  }

  *data = (((uint64_t)msbs) << 32) | (uint64_t)lsbs;
 
  // rumboot_printf("data_msbs=%x, data_lsbs=%x\n", 
    // (uint32_t)(*data >> 32), 
    // (uint32_t)(*data & 0xFFFFFFFF)
  // );
  dbg("quad_spi_read_data is finished\n");

  return 0;
}


int quad_spi_reset_irq(uintptr_t base, Quad_spi_irq_status *irq_status) {
  dbg("quad_spi_reset_irq\n");

  uint32_t reg = ioread32(base + IRQ_STATUS);

  irq_status->tx_empty = bit_in_reg(reg, 3);
  irq_status->rx_irq = bit_in_reg(reg, 2);
  irq_status->tx_irq = bit_in_reg(reg, 1);
  irq_status->rx_overrun = bit_in_reg(reg, 0);

  return 0;
}
