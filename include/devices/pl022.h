#ifndef PL022_H
#define PL022_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/**
 * \defgroup devices_pl022 PL022 SSP library
 * \ingroup devices
 *
 *  This is a simple library that provides communications over PL022 SSP.
 *
 * \code{.c}
 * #include <devices/pl022.h>
 * \endcode
 *
 *  Usage example:
 * \code{.c}
 *
 * struct pl022_config conf;
 * conf.ssp_clk = src->freq_khz * 1000;
 * conf.spi_clk = 12500000;
 * conf.data_size = 8;
 * pl022_init(base, &conf);
 * pl022_xfer(base, wrbuf, rdbuf, size_bytes);
 *
 * \endcode
 *
 * \addtogroup devices_pl022
 *
 * @{
 */

struct pl022_config {
	uint32_t ssp_clk; /** SSP input frequency from CRG (See SoC Doc!) */
	uint32_t spi_clk; /** Desired SPI Frequency */
	uint32_t data_size; /** Transfer unit size in bits (Usually 8) */
    uint32_t soft_cs; /** Use software chip-select control */
};

/**
 * Initialize the SSP.
 * @param base base address of the SSP
 * @param conf
 */

enum ssp_dma_interrupt{
    ssp_int = (0x1 << 0),
    gspi_int = (0x1 << 1),
    end_buf_write = (0x1 << 2),
    end_buf_read = (0x1 << 3),
    err_read = (0x1 << 4),
    err_write = (0x1 << 5),
    overflow_buf_write = (0x1 << 6)
};

enum ssp_dma_enable{
    disable = 0x0,
    transmit = 0x1,
    receive = 0x2,
    all = 0x3
};

enum ssp_mode{
    master_mode = 0,
    slave_mode = 1
};

enum ssp_frame_format{
    ssp_motorola_fr_form = 0x00,
    ssp_ti_fr_form = 0x01,
    ssp_nmw_fr_form = 0x02
};

struct ssp_params{
    uint8_t cpol;//spol
    uint8_t cpha;//sph
    enum ssp_mode mode;
    bool loopback;
    enum ssp_frame_format fr_format;
};

enum dma_mode{
    circle_buf = 0,
    base_mode = 1
};

enum dma_endianness{
    lsb = 0,
    msb = 1
};


struct dma_params{
    enum dma_endianness endian_read;
    uint8_t arlen;
    enum dma_endianness endian_write;
    uint8_t awlen;
};


//global functions
void pl022_init(uint32_t base, struct pl022_config const *conf);
/**
 * Calculate the divisors and set SSP speed
 * @param  base base address
 * @param  conf pointer to ssp configuration structure
 * @return
 */
int pl022_set_speed(uint32_t base, struct pl022_config const *conf);
/**
 * Software control over 'internal' chip-select line
 * FIXME: Not yet implemented
 * @param base   SSP base address
 * @param select CS signal value
 */
void pl022_internal_cs(uint32_t base, int select);
/**
 * Returns true if TX FIFO is empty
 * @param  base SSP base address
 * @return
 */
bool pl022_tx_empty(uint32_t base);
/**
 * Returns true if at least one element can be written to TX FIFO
 * @param  base SSP base address
 * @return
 */
bool pl022_tx_avail(uint32_t base);
/**
 * Returns true if RX FIFO is empty
 * @param  base SSP base address
 * @return
 */
bool pl022_rx_empty(uint32_t base);
/**
 * Dumps fifo state to stdout (for debugging)
 * @param base SSP base address
 */
void pl022_dump_fifo_state(uint32_t base);

/**
 * Transfer data via SSP. , rdbuf is filled with
 * data from SSP. wrbuf and rdbuf pointers may match
 * @param  base  SSP base address
 * @param  wrbuf Data to send via spi
 * @param  rdbuf Buffer for data read from SPI
 * @param  len   the length of the buffers
 * @return       returns the number of bytes transferred
 */
int pl022_xfer(uint32_t base, unsigned char const *wrbuf, unsigned char *rdbuf, size_t len);

/**
 * Transfer data via SSP with a timeout. If no activity (sending or receiving)
 * is detecting for timeout uS, the function will return.
 * If timeout is 0 this function is identical to pl022_xfer()
 *
 * @param  base  SSP base address
 * @param  wrbuf Data to send via spi
 * @param  rdbuf Buffer for data read from SPI
 * @param  len   the length of the buffers
 * @param  timeout Timeout uS for operations
 * @return the number of bytes transferred or -1 on timeout
 */

int pl022_xfer_timeout(uint32_t base, unsigned char const *wrbuf, unsigned char *rdbuf, size_t len, uint32_t timeout);
/**
 * Clears the RX FIFO
 * @param base SSP base address
 */
void pl022_clear_rx_buf(uint32_t base);

/**
* @}
*/

//ssp functions
/**
 * Read of Raw interrupt status register
 * @param base_address   SSP base address
 * @return              Raw interrupt status
 */
uint32_t pl022_get_ris(uint32_t base_address);
/**
 * Read of Masked interrupt status register
 * @param base_address   SSP base address
 * @return              Masked interrupt status
 */
uint32_t pl022_get_mis(uint32_t base_address);
/**
 * Read of status register
 * @param base_addr   SSP base address
 * @return               Status
 */
uint32_t pl022_get_ssp_status(uint32_t base_addr);
/**
 * Set or clear Interrupt mask
 * @param base_address   SSP base address
 * @param mask           interrupt mask
 */
void pl022_set_int_mask(uint32_t base_address, uint16_t mask);
/**
 * Enable DMA
 * @param base_addr   SSP base address
 * @param enabled        parameters of dma enable
 */
void pl022_dma_enable(uint32_t base_addr, enum ssp_dma_enable enabled);
/**
 * Write in data register
 * @param base_address   SSP base address
 * @param word           data
 */
void pl022_send_word(uint32_t base_address, uint32_t word);
/**
 * Read from data register
 * @param base_address   SSP base address
 * @return               read data
 */
uint32_t pl022_get_word(uint32_t base_address);
/**
 * Set parameters of SSP
 * @param base_address   SSP base address
 * @param params         parameters of SSP: cpol, cpha, mode, loopback, fr_format
 */
void pl022_set_param(uint32_t base_address, struct ssp_params const * params);


//dma function
/**
 * Reset DMA
 * @param base_address   SSP base address
 */
void pl022_dma_reset(uint32_t base_addr);
/**
 * Set interrupt mask of DMA
 * @param base_addr   SSP base address
 * @param interrupt           interrupt mask
 */
void pl022_dma_set_irq_mask(uint32_t base_addr, enum ssp_dma_interrupt interrupt);
/**
 * Read of DMA status register
 * @param base_addr   SSP base address
 * @return               DMA Status
 */
uint32_t pl022_get_dma_status(uint32_t base_addr);
/**
 * Set start and end address of buffer of read DMA for 8-bit data
 * @param base_addr   SSP base address
 * @param r_addr      start address of buffer of read DMA
 * @param byte_n      number of bytes
 */
void pl022_dma_set_read_addr(uint32_t base_addr, uint32_t const * r_addr, uint32_t byte_n);
/**
 * Set start and end address of buffer of write DMA for 8-bit data
 * @param base_addr   SSP base address
 * @param r_addr      start address of buffer of write DMA
 * @param byte_n      number of bytes
 */
void pl022_dma_set_write_addr(uint32_t base_addr, uint32_t const * w_addr, uint32_t byte_n);//for 8-bit data
/**
 * Set mode of DMA
 * @param base_address   SSP base address
 * @param mode           circular or basic mode
 */
void pl022_dma_set_mode(uint32_t base_addr, enum dma_mode mode);
/**
 * Set parameters of DMA
 * @param base_addr   SSP base address
 * @param param       parameters of channel of read/write: endianness, length of data in a package
 */
void pl022_dma_set_param(uint32_t base_addr, struct dma_params const * param);

#endif /* end of include guard: PL022_H */
