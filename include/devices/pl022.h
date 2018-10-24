#ifndef PL022_H
#define PL022_H

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
void pl022_init(uint32_t base, struct pl022_config *conf);
/**
 * Calculate the divisors and set SSP speed
 * @param  base base address
 * @param  conf pointer to ssp configuration structure
 * @return
 */
int pl022_set_speed(uint32_t base, struct pl022_config *conf);
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
int pl022_xfer(uint32_t base, unsigned char *wrbuf, unsigned char *rdbuf, size_t len);

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

int pl022_xfer_timeout(uint32_t base, unsigned char *wrbuf, unsigned char *rdbuf, size_t len, uint32_t timeout);
/**
 * Clears the RX FIFO
 * @param base SSP base address
 */
void pl022_clear_rx_buf(uint32_t base);

/**
* @}
*/

#endif /* end of include guard: PL022_H */
