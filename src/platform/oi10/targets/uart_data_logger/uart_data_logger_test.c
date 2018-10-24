#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>

#include <platform/devices.h>
#include <platform/test_assert.h>
#include <platform/test_event_c.h>


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

static inline void reg_write(uint32_t addr, uint32_t value)
{
    iowrite32(value, addr);
}

static inline uint32_t reg_read(uint32_t addr)
{
    return ioread32(addr);
}

static inline void reg_set(uint32_t addr, uint32_t value)
{
    iowrite32(ioread32(addr) | value, addr);
}

static inline void reg_clear(uint32_t addr, uint32_t value)
{
    iowrite32(ioread32(addr) & ~value, addr);
}

static inline bool reg_check(uint32_t addr, uint32_t value)
{
    return ((ioread32(addr) & value) == value);
}


static void uart_init(uint32_t freq, uint32_t baudrate)
{
    //Set dividers according to frequency and baudrate
    reg_write(UART_BASE_ADDR + UARTIBRD, (uint32_t)(freq/(16*baudrate)));
    reg_write(UART_BASE_ADDR + UARTFBRD, (uint32_t)(freq%(16*baudrate)));
    
    //World length == 8bit 
    reg_set(UART_BASE_ADDR + UARTLCR_H, (0x03 << 5));
    
    //FIFO enable
    reg_set(UART_BASE_ADDR + UARTLCR_H, (0x01 << 4));
}

static void uart_enable()
{
    reg_set(UART_BASE_ADDR + UARTCR, (0x01 << 8));
    reg_set(UART_BASE_ADDR + UARTCR, (0x01 << 9));

    reg_set(UART_BASE_ADDR + UARTCR, 0x01);
}

static void uart_write_char(uint8_t data)
{
    while (reg_check(UART_BASE_ADDR + UARTFR, (0x01 << 5)))
        ;

    reg_write(UART_BASE_ADDR + UARTDR, data);
}

static void uart_disable()
{
    reg_clear(UART_BASE_ADDR + UARTCR, 0x01);

    reg_clear(UART_BASE_ADDR + UARTCR, (0x01 << 8));
    reg_clear(UART_BASE_ADDR + UARTCR, (0x01 << 9));
}


const uint32_t TEST_EVENT_UART_DATA_LOGGER_TX = TEST_EVENT_CODE_MIN + 0x100;

int main()
{
    test_event_send_test_id("test_oi10_uart_data_logger");

    const char msg[] = "\x55H\nello\nworld\n"; 
    
    size_t msg_len = ARRAY_SIZE(msg);
   
    uart_disable();
    uart_init(100000000, 6250000);
    uart_enable();
    
    for (size_t i = 0; i < msg_len; i++)
    {
        uart_write_char((uint8_t)msg[i]);
    }
    
    //Waiting TFIFO to be empty.
    while (!reg_check(UART_BASE_ADDR + UARTFR, (0x01 << 7)))
        ;
        

    test_event(TEST_EVENT_UART_DATA_LOGGER_TX);

    uint32_t data = 0;

    //Waiting RFIFO to be full.
    for (int i = 0; i < 256; i++)
    {
        while (reg_check(UART_BASE_ADDR + UARTFR, (0x01 << 4)))
            ;

        data = reg_read(UART_BASE_ADDR + UARTDR);

        rumboot_printf("UART data received: 0x%x\n", data);

        TEST_ASSERT(data == i, "Incorrect data received.");
    }

    uart_disable();

	return 0;
}
