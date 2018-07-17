#include <stdint.h>
#include <stdbool.h>

#include <devices/uart.h>
#include <regs/regs_uart.h>
#include <rumboot/io.h>
#include <rumboot/timer.h>
#include <rumboot/printf.h>


static void reg_write(uint32_t base, uint32_t reg_offset, uint32_t value)
{
    iowrite32(value, base + reg_offset);
}

static uint32_t reg_read(uint32_t base, uint32_t reg_offset)
{
    return ioread32(base + reg_offset);
}

static void reg_set(uint32_t base, uint32_t reg_offset, uint32_t value)
{
    uint32_t reg = ioread32(base + reg_offset);

    reg |= (value);
    iowrite32(reg, base + reg_offset);
}

static void reg_clear(uint32_t base, uint32_t reg_offset, uint32_t value)
{
    uint32_t reg = ioread32(base + reg_offset);

    reg &= ~(value);
    iowrite32(reg, base + reg_offset);
}

static bool reg_check(uint32_t base, uint32_t reg_offset, uint32_t value)
{
    uint32_t reg = ioread32(base + reg_offset);

    if ((reg & value) == value)
        return true;

    return false;
}


void uart_set_word_length(uint32_t base_addr, UART_word_length word_length);
void uart_set_parity(uint32_t base_addr, UART_parity parity);


void uart_init(uint32_t base_addr, UART_word_length wlen, uint32_t baud_rate, UART_parity parity, short int int_mask, int loopback){
    uart_set_baudrate(base_addr,baud_rate);
    uart_set_word_length(base_addr,wlen);
    if(loopback == 0x1)//loopback
        uart_loopback_enable(base_addr,loopback);
    //uart_rx_enable(base_addr,true);
    //uart_tx_enable(base_addr,true);
    uart_set_parity(base_addr,parity);
    uart_set_interrupt_mask(base_addr, int_mask);
    //uart_enable(base_addr,true);
}

void uart_enable(uint32_t base_addr, bool enabled){
    if(enabled){
        reg_set(base_addr, UARTCR, UARTCR__UARTEN);
    } else {
        reg_clear(base_addr, UARTCR, UARTCR__UARTEN);
    }
}

void uart_rx_enable(uint32_t base_addr, bool enabled){
    if(enabled){
        reg_set(base_addr, UARTCR, UARTCR__RXE);
    } else {
        reg_clear(base_addr, UARTCR, UARTCR__RXE);
    }
}

void uart_tx_enable(uint32_t base_addr, bool enabled){
    if(enabled){
        reg_set(base_addr, UARTCR, UARTCR__TXE);
    } else {
        reg_clear(base_addr, UARTCR, UARTCR__TXE);
    }
}

void uart_loopback_enable(uint32_t base_addr, bool enabled){
    if(enabled){
        reg_set(base_addr, UARTCR, UARTCR__LBE);
    } else {
        reg_clear(base_addr, UARTCR, UARTCR__LBE);
    }
}

void uart_set_parity(uint32_t base_addr, UART_parity parity){
    switch(parity){
    case UART_parity_no:
        //clear bit 1 and 2
        reg_clear(base_addr, UARTLCR_H, (UARTLCR_H__PEN | UARTLCR_H__EPS));
        break;
    case UART_parity_even:
        //set bit 1 and 2
        reg_set(base_addr, UARTLCR_H, (UARTLCR_H__PEN | UARTLCR_H__EPS));
        break;
    case UART_parity_odd:
        //clear bit 2, set bit 1
        reg_clear(base_addr, UARTLCR_H, UARTLCR_H__EPS);
        reg_set(base_addr, UARTLCR_H, UARTLCR_H__PEN);
        break;
    }
}

//disable FIFO to flush
void uart_fifos_enable(uint32_t base_addr, bool enabled){
    if(enabled){
        reg_set(base_addr, UARTLCR_H, UARTLCR_H__FEN);
    } else {
        reg_clear(base_addr, UARTLCR_H, UARTLCR_H__FEN);
    }
}

void uart_rts_cts_enable(uint32_t base_addr, bool enabled){
    if(enabled){
        reg_set(base_addr, UARTCR, (UARTCR__RTSEN | UARTCR__CTSEN));
    } else {
        reg_clear(base_addr, UARTCR, (UARTCR__RTSEN | UARTCR__CTSEN));
    }
}

void uart_set_baudrate(uint32_t base_addr, uint32_t baudrate_hz){
    uint32_t divisor_i = UART_SYS_FREQ_HZ / (16 * baudrate_hz);//integer
    uint32_t  modulo = UART_SYS_FREQ_HZ % (16 * baudrate_hz);//modulo
    //uint32_t divisor_f = (uint32_t)((modulo * 64 / (16 * baudrate_hz))+0.5);
    uint32_t divisor_f = (uint32_t)(((modulo * 64 / (16 * baudrate_hz)) * 10 + 5) / 10);

    //float fraction = modulo / (16 * baudrate_hz);
    //uint32_t divisor_f = (uint32_t)((fraction * 64)+0.5);

    reg_write(base_addr, UARTIBRD, divisor_i);
    reg_write(base_addr, UARTFBRD, divisor_f);
}

void uart_set_word_length(uint32_t base_addr, UART_word_length word_length){
    reg_clear(base_addr, UARTLCR_H, UARTLCR_H__WLEN);
    reg_set(base_addr, UARTLCR_H, (word_length << UARTLCR_H__WLEN_SHIFT));
}

static uint32_t uart_wait_busy(uint32_t base_addr){
    uint32_t timeout = UART_TIMEOUT;
    while(--timeout){
        if(reg_check(base_addr, UARTFR, UARTFR__BUSY)){
            ;//BUSY bit is set..
        } else
            break;
    }
    if(timeout == 0)
        return 0;
    else
        return 1;
}

//return OK when FULL and ERROR when EMPTY
uint32_t uart_check_rfifo_full(uint32_t base_addr){
    if(reg_check(base_addr, UARTFR, UARTFR__RXFF))
        return 1;//fifo/holding_reg empty
    else
        return 0;
}

//return OK when EMPTY and ERROR when FULL
uint32_t uart_check_rfifo_empty(uint32_t base_addr){
    if(reg_check(base_addr, UARTFR, UARTFR__RXFE))
        return 1;//fifo/holding_reg empty
    else
        return 0;
}

//return OK when EMPTY and ERROR when FULL
uint32_t uart_check_tfifo_empty(uint32_t base_addr){
    if(reg_check(base_addr, UARTFR, UARTFR__TXFE))
        return 1;
    else
        return 0;
}

//return OK when FULL and ERROR when EMPTY
uint32_t uart_check_tfifo_full(uint32_t base_addr){
    if(reg_check(base_addr, UARTFR, UARTFR__TXFF))
        return 1;
    else
        return 0;
}

uint16_t uart_get_recv_status(uint32_t base_addr){
    return reg_read(base_addr, UARTRSR);
}

void uart_clear_status(uint32_t base_addr){
    reg_write(base_addr, UARTECR, 0x0); //data is not important
}

void uart_putc(uint32_t base_addr,unsigned char test_char){
    //single transfer
    uint32_t timeout = UART_TIMEOUT;
    while((uart_check_tfifo_full(base_addr) == 1) && timeout--) //wait tfifo not full
            ;

    if (!timeout)
        rumboot_printf("ERROR! Timeout on TFIFO FULL.\n");

    reg_write(base_addr, UARTDR, test_char);
    uart_wait_busy(base_addr);
}

char uart_getc(uint32_t base_addr){
    uint32_t timeout = UART_TIMEOUT;
    while((uart_check_rfifo_empty(base_addr) == 1) && timeout--) //wait rfifo not empty
        ;

    if (!timeout)
        rumboot_printf("ERROR! Timeout on RFIFO EMPTY.\n");

    return reg_read(base_addr, UARTDR) & 0xFF;
}

void uart_set_interrupt_mask(uint32_t base_addr, short mask){
    reg_write(base_addr, UARTIMSC, mask & 0x7FF) ;//bit's [15:11] are reserved
}

short uart_get_mis(uint32_t base_addr){
    return reg_read(base_addr, UARTMIS) & 0x7FF;
}

void uart_stop(uint32_t base_addr){
    //UART has no soft reset
    uart_enable(base_addr,false);
    uart_fifos_enable(base_addr,false);//flush the FIFO's
}

