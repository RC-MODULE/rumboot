#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <platform/devices.h>


#define MEM8(addr) *((uint8_t*)(addr))
#define MEM32(addr) *((uint32_t*)(addr))

const uint32_t UART_BASE_ADDR = 0xC0029000; 

const uint32_t UARTDR       = 0x00;
const uint32_t UARTRSR_ECR  = 0x04;
const uint32_t UARTFR       = 0x18;
const uint32_t UARTILPR     = 0x20;
const uint32_t UARTIBRD     = 0x24;
const uint32_t UARTFBRD     = 0x28;
const uint32_t UARTLCR_H    = 0x2C;
const uint32_t UARTCR       = 0x30;
const uint32_t UARTIFLS     = 0x34;
const uint32_t UARTIMSC     = 0x38;
const uint32_t UARTRIS      = 0x3C;
const uint32_t UARTMIS      = 0x40;
const uint32_t UARTICR      = 0x44;
const uint32_t UARTDMACR    = 0x48;


typedef enum 
{
    uart_stopbits_1 = 0,
    uart_stopbits_1_5,
    uart_stopbits_2
} uart_stopbits;


static void uart_init(uint32_t freq, uint32_t baudrate)
{
    //Set dividers according to frequency and baudrate
    MEM32(UART_BASE_ADDR + UARTIBRD) = (uint32_t)(freq/(16*baudrate));
    MEM32(UART_BASE_ADDR + UARTFBRD) = (uint32_t)(freq%(16*baudrate));
    
    //World length == 8bit 
    MEM32(UART_BASE_ADDR + UARTLCR_H) |= (0x03 << 5);
    
    //FIFO enable
    MEM32(UART_BASE_ADDR + UARTLCR_H) |= (0x01 << 4);
}

static void uart_enable()
{
    MEM32(UART_BASE_ADDR + UARTCR) |= 0x01;
}

static void uart_write_char(uint8_t data)
{
    MEM8(UART_BASE_ADDR + UARTDR) = data;
}

static void uart_disable()
{
    MEM32(UART_BASE_ADDR + UARTCR) &= ~0x01;
}




int main()
{
    const char msg[] = "\x55H\nello\nworld\n"; 
    
    size_t msg_len = sizeof(msg)/sizeof(msg[0]);
   
    uart_disable();
    uart_init(100000000, 6250000);
    uart_enable();
    
    for (size_t i = 0; i < msg_len; i++)
    {
        uart_write_char((uint8_t)msg[i]);
    }    
        
    uart_disable();

	return 0;
}
