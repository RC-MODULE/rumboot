#ifndef REGS_UART_H
#define REGS_UART_H

/**
 *
 * \defgroup regs_UART
 * \ingroup uart
 *
 * \addtogroup regs_UART
 * @{
 */
#define UARTDR          0x000
#define UARTRSR         0x004
#define UARTECR         0x004
#define UARTFR          0x018
#define UARTILPR        0x020
#define UARTIBRD        0x024
#define UARTFBRD        0x028
#define UARTLCR_H       0x02C
#define UARTCR          0x030
#define UARTIFLS        0x034
#define UARTIMSC        0x038
#define UARTRIS         0x03C
#define UARTMIS         0x040
#define UARTICR         0x044
#define UARTDMACR       0x048
#define UARTPeriphID0   0xFE0
#define UARTPeriphID1   0xFE4
#define UARTPeriphID2   0xFE8
#define UARTPeriphID3   0xFEC
#define UARTPCellID0    0xFF0
#define UARTPCellID1    0xFF4
#define UARTPCellID2    0xFF8
#define UARTPCellID3    0xFFC
/**
  * @}
*/
 /**
  *
  * \defgroup uart_bit_positions Bit_positions
  * \ingroup aurt
  *
  * \addtogroup uart_bit_positions
  * @{
  */
#define UARTCR__UARTEN         (1 << 0)
#define UARTCR__SIREN          (1 << 1)
#define UARTCR__SIRLP          (1 << 2)
#define UARTCR__LBE            (1 << 7)
#define UARTCR__TXE            (1 << 8)
#define UARTCR__RXE            (1 << 9)
#define UARTCR__DTR            (1 << 10)
#define UARTCR__RTS            (1 << 11)
#define UARTCR__OUT1           (1 << 12)
#define UARTCR__OUT2           (1 << 13)
#define UARTCR__RTSEN          (1 << 14)
#define UARTCR__CTSEN          (1 << 15)

#define UARTLCR_H__BRK         (1 << 0)
#define UARTLCR_H__PEN         (1 << 1)
#define UARTLCR_H__EPS         (1 << 2)
#define UARTLCR_H__STP2        (1 << 3)
#define UARTLCR_H__FEN         (1 << 4)
#define UARTLCR_H__WLEN        (3 << 5)
#define UARTLCR_H__SPS         (1 << 7)
#define UARTLCR_H__WLEN_SHIFT  5

#define UARTFR__CTS            (1 << 0)
#define UARTFR__DSR            (1 << 1)
#define UARTFR__DCD            (1 << 2)
#define UARTFR__BUSY           (1 << 3)
#define UARTFR__RXFE           (1 << 4)
#define UARTFR__TXFF           (1 << 5)
#define UARTFR__RXFF           (1 << 6)
#define UARTFR__TXFE           (1 << 7)
#define UARTFR__RI             (1 << 8)
/**
 * @}

 * \defgroup uart_constants  Constants
 * \ingroup uart
 *
 * \addtogroup uart_constants
 * @{
 */
#define UART_SYS_FREQ_HZ 100000000
#define UART_DEFAULT_BAUDRATE 38400
#define UART_TIMEOUT 150000 //>8-bit at 1200 baudrate @ 500MHz sys clock
/**
 * @}
 */
#endif
