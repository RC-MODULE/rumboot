#ifndef QUAD_SPI_H
#define QUAD_SPI_H

#include <stdint.h> // for uintptr_t, uint32_t

typedef enum Mode {
  SPI, DUAL, QUAD, TEXAS_INSTRUMENTS, MICROWIRE
} Mode;

typedef enum Source {
  PROGRAM, HARDWARE
} Source;


typedef struct Quad_spi_config {
  Mode mode; ///< mode: SPI, DUAL, QUAD, TEXAS_INSTRUMENTS, MICROWIRE
  unsigned int base_frequency; ///< input frequency
  unsigned int out_frequency; ///< Quad SPI Frequency
  unsigned int phase; ///< phase of the clock signal
  unsigned int polarity; ///< polarity of the clock signal
  Source ss_source; ///< selecting the source of line slave select
  unsigned int rx_data_size; ///< length of the rx-message: rx_length for DUAL, QUAD; length1 for TEXAS_INSTRUMENTS, MICROWIRE
  unsigned int tx_data_size; ///< length of the tx-message: length0 for MICROWIRE; length1 for SPI, DUAL, QUAD, TEXAS_INSTRUMENTS
  unsigned int empty_data_size; ///< number of empty bits: length0 for DUAL, QUAD

  unsigned int tx_fifo_irq_level; ///< number of words in the tx-buffer that the interrupt flag is set for
  unsigned int rx_fifo_irq_level; ///< number of words in the rx-buffer that the interrupt flag is set for
} Quad_spi_config;

typedef struct Quad_spi_mask {
  unsigned int tx_empty;
  unsigned int rx_irq;
  unsigned int tx_irq;
  unsigned int rx_overrun;
} Quad_spi_mask;

typedef struct Quad_spi_irq_status {
  unsigned int tx_empty;
  unsigned int rx_irq;
  unsigned int tx_irq;
  unsigned int rx_overrun;
} Quad_spi_irq_status;


int quad_spi_init(uintptr_t base, Quad_spi_config const *config);
int quad_spi_set_mask(uintptr_t base, Quad_spi_mask const *mask);
int quad_spi_tx_fifo_full(uintptr_t base);
int quad_spi_rx_fifo_empty(uintptr_t base);
int quad_spi_busy(uintptr_t base);
int quad_spi_write_data(uintptr_t control_base, uintptr_t fifo_base, unsigned int size, uint64_t const data);
int quad_spi_read_data(uintptr_t control_base, uintptr_t fifo_base, unsigned int size, uint64_t *data);
int quad_spi_reset_irq(uintptr_t base, Quad_spi_irq_status *irq_status);

// int generate_control_reg(Quad_spi_config const *config, uint32_t *control);
// int generate_fifo_watermark_reg(Quad_spi_config const *config, uint32_t *fifo_watermark);




// static char *Mode_str[] = {
  // "spi", "dual", "quad", "texas_instruments", "microwire"
// }; ///< for print enum Mode

#endif // QUAD_SPI_H