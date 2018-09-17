#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

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

#define SSP_SYS_FREQ_HZ 500000000
//clock divider
#define CPSDVR  2

typedef enum{
    ssp_int = (0x1 << 0),
    gspi_int = (0x1 << 1),
    end_buf_write = (0x1 << 2),
    end_buf_read = (0x1 << 3),
    err_read = (0x1 << 4),
    err_write = (0x1 << 5),
    overflow_buf_write = (0x1 << 6)
}ssp_dma_interrupt;

typedef enum{
    disable = 0x0,
    transmit = 0x1,
    receive = 0x2,
    all = 0x3
}ssp_dma_enable;

typedef enum{
    master_mode = 0,
    slave_mode = 1
}ssp_mode;

typedef enum{
    ssp_motorola_fr_form = 0x00,
    ssp_ti_fr_form = 0x01,
    ssp_nmw_fr_form = 0x02
}ssp_frame_format;

typedef struct{
    uint32_t cpsdvr;
    uint8_t cpol;//spol
    uint8_t cpha;//sph
    uint8_t data_size;
    ssp_mode mode;
    bool loopback;
    ssp_frame_format fr_format;
}ssp_params;

typedef enum{
    circle_buf = 0,
    base_mode = 1
}dma_mode;

typedef enum{
    lsb = 0,
    msb = 1
}dma_endianness;


typedef struct{
    dma_endianness endian_read;
    uint8_t arlen;
    dma_endianness endian_write;
    uint8_t awlen;
}dma_params;


//global functions
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

//ssp functions
void gspi_init(uint32_t base_address, ssp_params params, struct pl022_config *conf);
/**
 * Read of Raw interrupt status register
 * @param base_address   SSP base address
 * @return              Raw interrupt status
 */
uint32_t gspi_get_ris(uint32_t base_address);
/**
 * Read of Masked interrupt status register
 * @param base_address   SSP base address
 * @return              Masked interrupt status
 */
uint32_t gspi_get_mis(uint32_t base_address);
/**
 * Read of status register
 * @param base_address   SSP base address
 * @return               Status
 */
uint32_t gspi_get_ssp_status(uint32_t base_addr);
/**
 * Set or clear Interrupt mask
 * @param base_address   SSP base address
 * @param mask           interrupt mask
 */
void gspi_set_int_mask(uint32_t base_address, uint16_t mask);
/**
 * Enable DMA
 * @param base_address   SSP base address
 * @param enabled        parameters of dma enable
 */
void gspi_dma_enable(uint32_t base_addr, ssp_dma_enable enabled);

void gspi_write_data(uint32_t base_addr, uint32_t data);
uint32_t gspi_read_data(uint32_t base_addr);
int gspi_send_word(uint32_t base_address, uint32_t word);
int gspi_get_word(uint32_t base_address, uint32_t * word);

//dma function
void gspi_dma_reset(uint32_t base_addr);
void gspi_dma_set_irq_mask(uint32_t base_addr, ssp_dma_interrupt interrupt);
uint32_t gspi_get_dma_status(uint32_t base_addr);
void gspi_dma_set_read_addr(uint32_t base_addr, uint32_t* r_addr, uint32_t byte_n);//for 8-bit data
void gspi_dma_set_write_addr(uint32_t base_addr, uint32_t* w_addr, uint32_t byte_n);//for 8-bit data
void gspi_dma_set_mode(uint32_t base_addr, dma_mode mode);
void gspi_dma_set_param(uint32_t base_addr, dma_params param);

#endif /* end of include guard: PL022_H */
