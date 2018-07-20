#ifndef MUART_H
#define MUART_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/**
 * \defgroup devices_muart MUART
 * \ingroup devices
 *
 *  MUART function libriary.
 *  Contains functions such as:
 *  - Init muart
 *  - Enable muart
 *  - Disable muart
 *  - Write char to muart
 *  - Read char from muart
 *  - muart soft reset
 *

 *
 *  Usage example:
 *
 *
 * \addtogroup devices_muart
 *
 * @{
 */

enum WORD_LENGTH {
        WL_5 = 0,
        WL_6,
        WL_7,
        WL_8,
};

enum STOP_BITS_N {
        STP1 = 0,
        STP3_2,
        STP2,
};

enum MUART_MODE {
        RS_232 = 0,
        RS_485,
        RS_422,
};

//Structure contains configuration parameters
struct muart_conf {
        bool dum;               //data with flags (0) or not (1) for DMA
        enum WORD_LENGTH wlen;  //word length - 5..8 bit
        enum STOP_BITS_N stp2;  //number of stop bits
        bool is_even;           //Even Parity Select (EPS)
        bool is_parity;         //Parity Enable (PEN)
        enum MUART_MODE mode;   //Choose mode  RS_232, RS_485 or RS_422
        bool is_loopback;       //loopback is enable (LBE)
        uint32_t baud_rate;     //baud rate
        bool is_dma;            //Choose either DMA or APB (APB_MD)
};

/**
 * brief Set MUART register
 * @param base base address of MUART
 * @param reg_offset [description]
 * @param value
 */
void set_reg(uint32_t base, uint32_t reg_offset, uint32_t value);
 
/**
 * brief Init MUART
 * @param base base address of MUART
 * @param conf [description]
 */
void muart_init(const uint32_t base, const struct muart_conf *conf);

/**
 * brief Enable MUART
 * @param base base address of MUART
 */
void muart_enable(const uint32_t base);

/**
 * brief Disable MUART
 * @param base base address of MUART
 */
void muart_disable(const uint32_t base);

/**
 * brief Write char to MUART
 * @param base base address of MUART
 * @param ch   written symbol
 */
void muart_write_char(const uint32_t base, char ch);

/**
 * brief Read char from MUART
 * @param  base base address of MUART
 * @return     read symbol
 */
char muart_read_char(const uint32_t base);

/**
 * brief Write data throught APB
 * base1: base address of UART
 * data: pointer to written data
 * size: size of transmitted data
 */
void muart_write_data_apb(uint32_t base, volatile void* data, size_t size);

/**
 * brief Read data throught APB
 * base1: base address of UART
 * data: pointer to read data
 * size: size of transmitted data
 */
void muart_read_data_apb(uint32_t base, volatile void* data, size_t size);

/**
 * brief Transmit data throught APB
 * base1: base address of UART
 * base2: base address of UART
 * data: pointer to transmitted data
 * size: size of transmitted data
 * return if data was transmitted OK - 0, else - value less than 0

int muart_transmit_data_throught_apb(uint32_t base1, volatile void* data, size_t size);
*/

/**
 * brief Write data throught MDMA
 * base1: base address of UART
 * data: pointer to written data
 * len: size of transmitted data
 * return if OK - point on mdma write channel, else - NULL
 */
struct mdma_chan * muart_set_chan_mdma_W(uint32_t base,
                           void *dst_addr, size_t len);

/**
 * brief Read data throught MDMA
 * base1: base address of UART
 * data: pointer to read data
 * len: size of transmitted data
 * return if OK - point on mdma read channel, else - NULL
 */
struct mdma_chan * muart_set_chan_mdma_R(uint32_t base,
                           void *src_addr, size_t len);


void muart_finish_transmit_mdma(struct mdma_chan *chan_rd,
                               struct mdma_chan *chan_wr);

/**
 * brief Transmit data throught MDMA
 * base1: base address of MDMA with write enabled channel
 * base2: base address of UART with read enabled channel
 * dest: pointer to transmitted data
 * src: pointer to received data
 * len: length of data
 * return if data was transmitted OK -0, else - value less than 0
 */
int muart_transmit_data_throught_mdma(
        uint32_t src_base, uint32_t dst_base,
        void *src_addr, void *dst_addr, size_t len);

/**
 * brief MUART registers dump
 * base: base address of MUART
 */
void muart_dump(uint32_t base);

/**
 * brief soft reset of MUART
 * base: base address of MUART
 */
void muart_soft_reset(uint32_t base);

//void set_bit(uint32_t base, uint32_t reg_offset, uint32_t bit_pos);
void reset_bit(uint32_t base, uint32_t reg_offset, uint32_t bit_pos);

/**
 * @}
 */

#endif
