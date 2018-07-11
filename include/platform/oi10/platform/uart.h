#ifndef UART_H
#define UART_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/**
 * \defgroup devices_uart UART
 * \ingroup devices
 *
 *  UART function libriary.
 *  Contains functions such as:
 *  - Init uart
 *  - Enable uart
 *  - Disable uart
 *  - Write char to uart
 *  - Read char from uart
 *

 *
 *  Usage example:
 *
 *
 * \addtogroup devices_uart
 *
 * @{
 */

typedef enum {
    UART_word_length_5bit = (0x0),
    UART_word_length_6bit = (0x1),
    UART_word_length_7bit = (0x2),
    UART_word_length_8bit = (0x3)
}UART_word_length;

typedef enum {
    UART_even_parity = (1 << 0),
    UART_odd_parity  = (1 << 1),
    UART_no_parity   = (1 << 2)
}UART_parity;

/**
 * muart_conf						: Structure contains configuration parameters
 * wlen				  				- word length - 5..8 bit
 * stp2									- number of stop bits
 * is_even							- Choose either even or odd parity
 * is_parity_available	- Choose either parity available or not
 * mode									- mode of MUART
 * is_loopback					- Choose either loopback is or not?
 * baud_rate						- baud rate
 * dma_en								- Choose either DMA enable or not?
 */
struct muart_conf {
								enum WORD_LENGTH wlen;
								enum STOP_BITS_N stp2;
								bool is_even;
								bool is_parity_available;
								enum MUART_MODE mode;
								bool is_loopback;
								uint32_t baud_rate;
								bool dma_en;
};

/**
 * brief Init UART
 * @param base_addr base address of UART
 * @param wlen word length
 * @param baud_rate baud rate
 * @param parity parity
 * @param loopback loopback
 */
void uart_init(uint32_t base_addr, UART_word_length wlen, uint32_t baud_rate, UART_parity parity, int loopback);

/**
 * brief Enable UART
 * @param base_adr base address of UART
 * @param enabled enable/disable flag
 */
void uart_enable(uint32_t  base_addr, bool enabled);

/**
 * brief Enable UART RX
 * @param base_adr base address of UART
 * @param enabled enable/disable flag
 */
void uart_rx_enable(uint32_t  base_addr, bool enabled);

/**
 * brief Enable UART TX
 * @param base_adr base address of UART
 * @param enabled enable/disable flag
 */
void uart_tx_enable(uint32_t  base_addr, bool enabled);

/**
 * brief Enable UART loopback
 * @param base_adr base address of UART
 * @param enabled enable/disable flag
 */
void uart_loopback_enable(uint32_t  base_addr, bool enabled);

/**
 * brief Set baudrate for UART
 * @param base_adr base address of UART
 * @param baudrate_hz baudrate frequency in Hz
 */
void uart_set_baudrate(uint32_t  base_addr, uint32_t baudrate_hz);

/**
 * brief Put char via UART
 * @param base_adr base address of UART
 * @param test_char char to be put to UART
 */
void uart_putc(uint32_t  base_addr,unsigned char test_char);

/**
 * brief Get char from UART
 * @param base_adr base address of UART
 */
char uart_getc(uint32_t  base_addr);

/**
 * brief Stop UART device
 * @param base_adr base address of UART
 */
void uart_stop(uint32_t base_addr);

/**
 * brief Get receive status of UART
 * @param base_adr base address of UART
 */
uint16_t uart_get_recv_status(uint32_t  base_addr);

/**
 * brief Clear status of UART
 * @param base_adr base address of UART
 */
void uart_clear_status(uint32_t  base_addr);

/**
 * brief Set interrupt mask for UART
 * @param base_adr base address of UART
 * @param mask interrupt mask
 */
void uart_set_interrupt_mask(uint32_t  base_addr, short mask);

/**
 * brief Get MIS register of UART
 * @param base_adr base address of UART
 */
short uart_get_mis(uint32_t  base_addr);

/**
 * @}
 */
#endif
