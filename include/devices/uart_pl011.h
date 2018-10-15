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
    UART_parity_even = (1 << 0),
    UART_parity_odd  = (1 << 1),
    UART_parity_no   = (1 << 2)
}UART_parity;

typedef enum {
    UART_RX_FIFO_LEVEL_NONE   = -1,
    UART_RX_FIFO_LEVEL_GT_1_8 = 0,
    UART_RX_FIFO_LEVEL_GT_1_4 = 1,
    UART_RX_FIFO_LEVEL_GT_1_2 = 2,
    UART_RX_FIFO_LEVEL_GT_3_4 = 3,
    UART_RX_FIFO_LEVEL_GT_7_8 = 4
} UART_RX_FIFO_LEVEL;

typedef enum {
    UART_TX_FIFO_LEVEL_NONE   = -1,
    UART_TX_FIFO_LEVEL_LT_1_8 = 0,
    UART_TX_FIFO_LEVEL_LT_1_4 = 1,
    UART_TX_FIFO_LEVEL_LT_1_2 = 2,
    UART_TX_FIFO_LEVEL_LT_3_4 = 3,
    UART_TX_FIFO_LEVEL_LT_7_8 = 4
} UART_TX_FIFO_LEVEL;


typedef struct
{
    UART_word_length wlen;
    uint32_t uart_sys_freq_hz;
    uint32_t baud_rate;
    UART_parity parity;
    UART_RX_FIFO_LEVEL rx_fifo_level;
    UART_TX_FIFO_LEVEL tx_fifo_level;
    short int int_mask;
    bool use_rts_cts;
    bool loopback;
} uart_init_params;


/**
 * brief Init UART
 * @param base_addr base address of UART
 * @param init_params pointer to initialization parameters
 */
void uart_init(uint32_t base_addr, const uart_init_params* init_params);

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
 * brief Enable UART RTS ans CTS functionality
 * @param base_adr base address of UART
 * @param enabled enable/disable flag
 */

void uart_rts_cts_enable(uint32_t base_addr, bool enabled);

/**
 * brief Enable UART fifos
 * @param base_adr base address of UART
 * @param enabled enable/disable flag
 */
void uart_fifos_enable(uint32_t  base_addr, bool enabled);

/**
 * brief Set UART rx fifo level
 * @param base_adr base address of UART
 * @param rx_fifo_level Trigger level of RX FIFO
 */
void uart_fifos_set_rx_fifo_level(uint32_t base_addr, UART_RX_FIFO_LEVEL rx_fifo_level);

/**
 * brief Set UART tx fifo level
 * @param base_adr base address of UART
 * @param tx_fifo_level Trigger level of TX FIFO
 */
void uart_fifos_set_tx_fifo_level(uint32_t base_addr, UART_TX_FIFO_LEVEL tx_fifo_level);


/**
 * brief Set baudrate for UART
 * @param base_adr base address of UART
 * @param uart_sys_freq_hz system frequency in hertzs
 * @param baudrate_hz baudrate frequency in Hz
 */
void uart_set_baudrate(uint32_t  base_addr, uint32_t uart_sys_freq_hz, uint32_t baudrate_hz);

/**
 * brief Put char via UART
 * @param base_adr base address of UART
 * @param test_char char to be put to UART
 * @param timeout_us_operation timeout in microseconds
 */
int8_t uart_putc(uint32_t  base_addr, unsigned char test_char, uint32_t timeout_us);

/**
 * brief Get char from UART
 * @param base_adr base address of UART
 * @param timeout_us operation timeout in microseconds
 */
char uart_getc(uint32_t  base_addr, uint32_t timeout_us);

/**
 * brief Waiting while UART busy
 * @param base_adr base address of UART
 * @param timeout_us operation timeout in microseconds
 */
uint32_t uart_wait_busy(uint32_t base_addr, uint32_t timeout_us);

/**
 * brief Check UART RFIFO FULL
 * @param base_adr base address of UART
 */
uint32_t uart_check_rfifo_full(uint32_t base_addr);

/**
 * brief Check UART RFIFO EMPTY
 * @param base_adr base address of UART
 */
uint32_t uart_check_rfifo_empty(uint32_t base_addr);

/**
 * brief Check UART TFIFO EMPTY
 * @param base_adr base address of UART
 */
uint32_t uart_check_tfifo_empty(uint32_t base_addr);

/**
 * brief Check UART TFIFO FULL
 * @param base_adr base address of UART
 */
uint32_t uart_check_tfifo_full(uint32_t base_addr);

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
