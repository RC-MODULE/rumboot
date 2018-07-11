#include <stdint.h>
#include <stdbool.h>

#include <platform/uart.h>
#include <regs/regs_uart.h>
#include <rumboot/io.h>
#include <rumboot/timer.h>


static void set_field(uint32_t base, uint32_t reg_offset, uint32_t bit_pos, uint32_t mask)
{
    uint32_t reg = ioread32(base + reg_offset);

    reg |= (mask << bit_pos);
    iowrite32(reg, base + reg_offset);
}

static void reset_field(uint32_t base, uint32_t reg_offset, uint32_t bit_pos, uint32_t mask)
{
    uint32_t reg = ioread32(base + reg_offset);

    reg &= ~(mask << bit_pos);
    iowrite32(reg, base + reg_offset);
}



static void reg_write(uint32_t base, uint32_t reg_offset, uint32_t value)
{
    iowrite32(value, base + reg_offset);
}

static uint32_t reg_read(uint32_t base, uint32_t reg_offset)
{
    return ioread32(value, base + reg_offset);
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



void uart_init(uint32_t base_addr, UART_word_length wlen, uint32_t baud_rate, UART_parity parity, int loopback){
    uart_set_baudrate(base_addr,baud_rate);
    uart_set_word_length(base_addr,wlen);
    if(loopback == 0x1)//loopback
        uart_loopback_enable(base_addr,loopback);
    uart_rx_enable(base_addr,true);
    uart_tx_enable(base_addr,true);
    uart_set_parity(base_addr,parity);
    uart_enable(base_addr,true);
}

void uart_enable(uint32_t base_addr, bool enabled){
    if(enabled){
        reg_set(base_addr, UARTCR, UARTCR__UARTEN);
        //TEST_ASSERT((UART_read_UARTCR(base_addr) & UARTEN),"UART_EN not set");
    } else {
        reg_clear(base_addr, UARTCR, UARTCR__UARTEN);
        //TEST_ASSERT((UART_read_UARTCR(base_addr) & (UARTEN == false)), "UART_EN no cleared");
    }
}

void uart_rx_enable(uint32_t base_addr, bool enabled){
    if(enabled){
        reg_set(base_addr, UARTCR, UARTCR__RXE);
        //TEST_ASSERT((UART_read_UARTCR(base_addr) & RXE),"RXE not set");
    } else {
        reg_clear(base_addr, UARTCR, UARTCR__RXE);
        //TEST_ASSERT((UART_read_UARTCR(base_addr) & (RXE == false)), "RXE not cleared");
    }
}

void uart_tx_enable(uint32_t base_addr, bool enabled){
    if(enabled){
        reg_set(base_addr, UARTCR, UARTCR__TXE);
        //TEST_ASSERT((UART_read_UARTCR(base_addr) & TXE),"TXE not set");
    } else {
        reg_clear(base_addr, UARTCR, UARTCR__TXE);
        //TEST_ASSERT((UART_read_UARTCR(base_addr) & (TXE == false)),"TXE not cleared");
    }
}

void uart_loopback_enable(uint32_t base_addr, bool enabled){
    if(enabled){
        reg_set(base_addr, UARTCR, UARTCR__LBE);
        //TEST_ASSERT((UART_read_UARTCR(base_addr) & LBE),"LBE not set");
    } else {
        reg_clear(base_addr, UARTCR, UARTCR__LBE);
        //TEST_ASSERT((UART_read_UARTCR(base_addr) & (LBE == false)), "LBE not cleared");
    }
}

void uart_set_parity(uint32_t base_addr, UART_parity parity){
    switch(parity){
    case UART_no_parity:
        //clear bit 1 and 2
        reg_clear(base_addr, UARTLCR_H, (UARTLCR_H__PEN | UARTLCR_H__EPS));
        //TEST_ASSERT((UART_read_UARTLCR_H(base_addr) & ((PEN | EPS) == false)), "PEN, EPS not cleared");
        break;
    case UART_even_parity:
        //set bit 1 and 2
        reg_set(base_addr, UARTLCR_H, (UARTLCR_H__PEN | UARTLCR_H__EPS));
        //TEST_ASSERT((UART_read_UARTLCR_H(base_addr) & ((PEN | EPS) == true)), "PEN, EPS not set");
        break;
    case UART_odd_parity:
        //clear bit 2, set bit 1
        reg_clear(base_addr, UARTLCR_H, UARTLCR_H__EPS);
        reg_set(base_addr, UARTLCR_H, UARTLCR_H__PEN);
        //TEST_ASSERT((UART_read_UARTLCR_H(base_addr) & (PEN == true)),"PEN not set");
        //TEST_ASSERT((UART_read_UARTLCR_H(base_addr) & (EPS == false)), "EPS not cleared");
        break;
    }
}

//disable FIFO to flush
void uart_fifos_enable(uint32_t base_addr, uint32_t enabled){
    if(enabled){
        reg_set(base_addr, UARTLCR_H, UARTLCR_H__FEN);
        //TEST_ASSERT((UART_read_UARTLCR_H(base_addr) & FEN), "FEN not set");
    } else {
        reg_clear(base_addr, UARTLCR_H, UART_LCR_H__FEN);
        //TEST_ASSERT((UART_read_UARTLCR_H(base_addr) & (FEN == false)), "FEN not cleared");
    }
}

void uart_set_baudrate(uint32_t base_addr, uint32_t baudrate_hz){
    uint32_t divisor_i = UART_SYS_FREQ_HZ / (16 * baudrate_hz);//integer
    uint32_t  modulo = UART_SYS_FREQ_HZ % (16 * baudrate_hz);//modulo
    uint32_t divisor_f = (uint32_t)((modulo * 64 / (16 * baudrate_hz))+0.5);
    //float fraction = modulo / (16 * baudrate_hz);
    //uint32_t divisor_f = (uint32_t)((fraction * 64)+0.5);

    reg_write(base_addr, UARTIBRD, divisor_i);
    //TEST_ASSERT((UART_read_UARTIBRD(base_addr) == divisor_i),"IBRD not set");
    reg_write(base_addr, UARTFBRD, divisor_f);
    //TEST_ASSERT((UART_read_UARTFBRD(base_addr) == divisor_f), "FBRD not set");
}

void uart_set_word_length(uint32_t base_addr, UART_word_length word_length){
    reg_clear(base_addr, UARTLCR_H, UARTLCR_H__WLEN);
    //TEST_ASSERT((UART_read_UARTLCR_H(base_addr) &  (WLEN == false)), "WLEN no cleared");
    reg_set(base_addr, UARTLCR_H, (word_length << UARTLCR_H__WLEN_SHIFT));
    //TEST_ASSERT((UART_read_UARTLCR_H(base_addr) & (WLEN ==  (word_length << WLEN_SHIFT))),"WLEN not set");
}

uint32_t uart_wait_busy(uint32_t base_addr){
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
    reg_write(base_addr, UARTDR, test_char);
    uart_wait_busy(base_addr);
}

char uart_getc(uint32_t base_addr){
    while(uart_check_rfifo_empty(base_addr) != 1) //wait rfifo not empty
        ;
    return reg_read(base_addr, UARTDR) & 0xFF;
}

void uart_set_interrupt_mask(uint32_t base_addr, short mask){
    reg_write(base_addr, UARTIMSC, mask & 0x7FF) ;//bit's [15:11] are reserved
    //TEST_ASSERT((UART_read_UARTIMSC(base_addr) == (mask & 0x7FF)), "INTMSK not set");
}

short uart_get_mis(uint32_t base_addr){
    return reg_read(base_addr, UARTMIS) & 0x7FF;
}

void uart_stop(uint32_t base_addr){
    //UART has no soft reset
    uart_enable(base_addr,false);
    uart_fifos_enable(base_addr,false);//flush the FIFO's
}

