#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <rumboot/regpoker.h>

#include <regs/regs_uart_pl011.h>
#include <devices/uart_pl011.h>

#include <platform/devices.h>
#include <platform/interrupts.h>

/* FixMe: Temporary hack for ppc-specific asserts */
#if !defined( __PPC__ )
#include <assert.h>
#define TEST_ASSERT(expr, why) assert(expr && why)
#else
#include <platform/test_assert.h>
#endif

//default values
#define     UARTRSR_DEFAULT         0x0
#define     UARTECR_DEFAULT         0x0
#define     UARTFR_DEFAULT          0x090
#define     UARTILPR_DEFAULT        0x0
#define     UARTIBRD_DEFAULT        0x0
#define     UARTFBRD_DEFAULT        0x0
#define     UARTLCR_H_DEFAULT       0x0
#define     UARTCR_DEFAULT          0x0300
#define     UARTIFLS_DEFAULT        0x12
#define     UARTIMSC_DEFAULT        0x0
#define     UARTRIS_DEFAULT         0x0
#define     UARTMIS_DEFAULT         0x0
#define     UARTDMACR_DEFAULT       0x0
#define     UARTPeriphID0_DEFAULT   0x11
#define     UARTPeriphID1_DEFAULT   0x10
#define     UARTPeriphID2_DEFAULT   0x4
#define     UARTPeriphID3_DEFAULT   0x0
#define     UARTPCellID0_DEFAULT    0x0D
#define     UARTPCellID1_DEFAULT    0xF0
#define     UARTPCellID2_DEFAULT    0x05
#define     UARTPCellID3_DEFAULT    0xB1

//Mask
#define     UARTDR_MSK          0x000
#define     UARTRSR_MSK         0xF
#define     UARTECR_MSK         0xF
#define     UARTFR_MSK          0b011111000
#define     UARTILPR_MSK        0xFF
#define     UARTIBRD_MSK        0xFFFF
#define     UARTFBRD_MSK        0x3F
#define     UARTLCR_H_MSK       0xFF
#define     UARTCR_MSK          0xFF87
#define     UARTIFLS_MSK        0x3F
#define     UARTIMSC_MSK        0x7FF
#define     UARTRIS_MSK         0x7F0
#define     UARTMIS_MSK         0x7F0
#define     UARTICR_MSK         0x7FF
#define     UARTDMACR_MSK       0x7
#define     UARTPeriphID0_MSK   0xFF
#define     UARTPeriphID1_MSK   0xFF
#define     UARTPeriphID2_MSK   0x0F
#define     UARTPeriphID3_MSK   0xFF
#define     UARTPCellID0_MSK    0xFF
#define     UARTPCellID1_MSK    0xFF
#define     UARTPCellID2_MSK    0xFF
#define     UARTPCellID3_MSK    0xFF


#define TEST_OK     0
#define TEST_ERROR  1

#define UART_TEST_DATA_LEN  32


//array for testing data exchange
static const uint8_t data_seq_arr[UART_TEST_DATA_LEN] = {
        0x00,
        0x01,
        0x02,
        0x03,
        0x04,
        0x05,
        0x06,
        0x07,
        0x08,
        0x09,
        0x0a,
        0x0b,
        0x0c,
        0x0d,
        0x0e,
        0x0f,
        0x10,
        0x11,
        0x12,
        0x13,
        0x14,
        0x15,
        0x16,
        0x17,
        0x18,
        0x19,
        0x1a,
        0x1b,
        0x1c,
        0x1d,
        0x1e,
        0x1f
};



static void reg_write(uint32_t base, uint32_t reg_offset, uint32_t value)
{
    iowrite32(value, base + reg_offset);
}

static uint32_t reg_read(uint32_t base, uint32_t reg_offset)
{
    return ioread32(base + reg_offset);
}



uint32_t check_read_from_uart(uint32_t base_addr, const uint8_t* data);

static volatile uint32_t READ_FROM_FIFO;
static volatile uint32_t UART_TX_CTS_INT;

static void irq_handler_uart_receiver(int irq, void* arg)
{
    uint32_t uart_status;
    uart_status = reg_read(UARTRX_BASE, UARTRIS);

    rumboot_printf("UART receiver interrupt handler - 0x%x\n", uart_status);

    if (uart_status & 0x10)
    {
        if (check_read_from_uart(UARTRX_BASE, data_seq_arr))
            exit(1);

        READ_FROM_FIFO = 1;
    }

    reg_write(UARTRX_BASE, UARTICR, 0x7FF);//clear interrupts
}

static void irq_handler_uart_transmitter(int irq, void* arg)
{
    uint32_t uart_status;
    uart_status = reg_read(UARTTX_BASE, UARTRIS);

    rumboot_printf("UART transmitter interrupt handler - 0x%x\n", uart_status);

    if (uart_status & 0x2)
    {
        UART_TX_CTS_INT = 1;
    }

    reg_write(UARTTX_BASE, UARTICR, 0x7FF);//clear interrupts
}


int uart_check_defaults(uint32_t base_addr)
{
    struct regpoker_checker check_array[] = {
     { "UARTRSR",       REGPOKER_READ32, UARTRSR,       UARTRSR_DEFAULT,        UARTRSR_MSK },
     { "UARTFR",        REGPOKER_READ32, UARTFR,        UARTFR_DEFAULT,         UARTFR_MSK },
     { "UARTILPR",      REGPOKER_READ32, UARTILPR,      UARTILPR_DEFAULT,       UARTILPR_MSK },
     { "UARTIBRD",      REGPOKER_READ32, UARTIBRD,      UARTIBRD_DEFAULT,       UARTIBRD_MSK },
     { "UARTFBRD",      REGPOKER_READ32, UARTFBRD,      UARTFBRD_DEFAULT,       UARTFBRD_MSK },
     { "UARTLCR_H",     REGPOKER_READ32, UARTLCR_H,     UARTLCR_H_DEFAULT,      UARTLCR_H_MSK },
     { "UARTCR",        REGPOKER_READ32, UARTCR,        UARTCR_DEFAULT,         UARTCR_MSK },
     { "UARTIFLS",      REGPOKER_READ32, UARTIFLS,      UARTIFLS_DEFAULT,       UARTIFLS_MSK },
     { "UARTIMSC",      REGPOKER_READ32, UARTIMSC,      UARTIMSC_DEFAULT,       UARTIMSC_MSK },
     { "UARTRIS",       REGPOKER_READ32, UARTRIS,       UARTRIS_DEFAULT,        UARTRIS_MSK },
     { "UARTMIS",       REGPOKER_READ32, UARTMIS,       UARTMIS_DEFAULT,        UARTMIS_MSK },
     { "UARTDMACR",     REGPOKER_READ32, UARTDMACR,     UARTDMACR_DEFAULT,      UARTDMACR_MSK },
     { "UARTPeriphID0", REGPOKER_READ32, UARTPeriphID0, UARTPeriphID0_DEFAULT,  UARTPeriphID0_MSK },
     { "UARTPeriphID1", REGPOKER_READ32, UARTPeriphID1, UARTPeriphID1_DEFAULT,  UARTPeriphID1_MSK },
     { "UARTPeriphID2", REGPOKER_READ32, UARTPeriphID2, UARTPeriphID2_DEFAULT,  UARTPeriphID2_MSK },
     { "UARTPeriphID3", REGPOKER_READ32, UARTPeriphID3, UARTPeriphID3_DEFAULT,  UARTPeriphID3_MSK },
     { "UARTPCellID0",  REGPOKER_READ32, UARTPCellID0,  UARTPCellID0_DEFAULT,   UARTPCellID0_MSK },
     { "UARTPCellID1",  REGPOKER_READ32, UARTPCellID1,  UARTPCellID1_DEFAULT,   UARTPCellID1_MSK },
     { "UARTPCellID2",  REGPOKER_READ32, UARTPCellID2,  UARTPCellID2_DEFAULT,   UARTPCellID2_MSK },
     { "UARTPCellID3",  REGPOKER_READ32, UARTPCellID3,  UARTPCellID3_DEFAULT,   UARTPCellID3_MSK },
     { "UARTILPR",      REGPOKER_WRITE32, UARTILPR,     0x00,                   UARTILPR_MSK },
     { "UARTIBRD",      REGPOKER_WRITE32, UARTIBRD,     0x00,                   UARTFBRD_MSK },
     { "UARTFBRD",      REGPOKER_WRITE32, UARTFBRD,     0x00,                   UARTFBRD_MSK },
     { "UARTLCR_H",     REGPOKER_WRITE32, UARTLCR_H,    0x00,                   UARTLCR_H_MSK },
     { "UARTCR",        REGPOKER_WRITE32, UARTCR,       0x00,                   0xFF86 },
     { "UARTIFLS",      REGPOKER_WRITE32, UARTIFLS,     0x00,                   UARTIFLS_MSK },
     { "UARTDMACR",     REGPOKER_WRITE32, UARTDMACR,    0x00,                   UARTDMACR_MSK },
     { /* Sentinel */ }
    };

    return rumboot_regpoker_check_array(check_array, base_addr);
}



void write_to_uart(uint32_t base_addr, const uint8_t* data, size_t data_len)
{
    rumboot_printf("Write:\n");
    for (uint32_t j = 0; j < data_len; j++)
    {
        rumboot_printf("data_seq_arr[%d] = 0x%x\n", j, data[j]);
        TEST_ASSERT((uart_putc(base_addr, data[j], UART_TIMEOUT) != -1), "Timeout while writing to UART!\n ");
    }
    rumboot_printf("Finish Write\n");
}


static uint32_t data_index = 0;

uint32_t check_read_from_uart(uint32_t base_addr, const uint8_t* data)
{
    rumboot_printf("Read:\n");

    int readval = uart_getc(base_addr, UART_TIMEOUT);
    TEST_ASSERT((readval >= 0), "Timeout while reading from UART!\n");

    rumboot_printf("UART_BASE+UARTDR = 0x%x\n", base_addr + UARTDR);
    rumboot_printf("readval = 0x%x\n", readval);
    if (readval != data[data_index])
    {
        rumboot_printf("Error on compare transmit and receive data!\n");
        rumboot_printf("Read: readval = 0x%x\n", readval);
        rumboot_printf("Expected: data_seq_arr[%d] = 0x%x\n", data_index, data[data_index]);
        return 1;
    }
    data_index++;

    return 0;
}


bool wait_for_tx_cts_int(uint32_t timeout)
{
    while (!UART_TX_CTS_INT && --timeout)
        ;

    if (!timeout)
    {
        rumboot_putstring("Transmitter UART CTS interrupt waiting failed.\n");
        return false;
    }

    return true;
}



uint32_t test_uart(uint32_t UART_TRANSMITTER_BASE, uint32_t UART_RECEIVER_BASE)
{
    static const struct uart_init_params transmitter_init_params =
    {
        .wlen = UART_word_length_8bit,
        .uart_sys_freq_hz = UART_SYS_FREQ_HZ,
        .baud_rate = 6250000,
        .parity = UART_parity_no,
        .rx_fifo_level = UART_RX_FIFO_LEVEL_GT_7_8,
        .tx_fifo_level = UART_TX_FIFO_LEVEL_LT_1_8,
        .int_mask = 0x022,
        .use_rts_cts = true,
        .loopback = false
    };

    static const struct uart_init_params receiver_init_params =
    {
        .wlen = UART_word_length_8bit,
        .uart_sys_freq_hz = UART_SYS_FREQ_HZ,
        .baud_rate = 6250000,
        .parity = UART_parity_no,
        .rx_fifo_level = UART_RX_FIFO_LEVEL_GT_7_8,
        .tx_fifo_level = UART_TX_FIFO_LEVEL_LT_1_8,
        .int_mask = 0x410,
        .use_rts_cts = true,
        .loopback = false
    };

    rumboot_printf("------- Test UART -------\n");
    rumboot_printf("UART_BASE = 0x%x\n", UART_RECEIVER_BASE);


    uart_init(UART_TRANSMITTER_BASE, &transmitter_init_params);
    uart_tx_enable(UART_TRANSMITTER_BASE, true);
    uart_enable(UART_TRANSMITTER_BASE, true);


    uart_init(UART_RECEIVER_BASE, &receiver_init_params);
    uart_rx_enable(UART_RECEIVER_BASE, true);
    uart_enable(UART_RECEIVER_BASE, true);


    write_to_uart(UART_TRANSMITTER_BASE, data_seq_arr, UART_TEST_DATA_LEN);


    if (!wait_for_tx_cts_int(UART_TIMEOUT))
        return TEST_ERROR;

    if (uart_check_rfifo_empty(UART_RECEIVER_BASE)) {
        rumboot_printf("FATAL: RX Fifo is totally empty. It shouldn't be\n");
        return TEST_ERROR;
    }

    while(!uart_check_rfifo_empty(UART_RECEIVER_BASE))
    {
        if (check_read_from_uart(UART_RECEIVER_BASE, data_seq_arr))
            return TEST_ERROR;
    }

   return TEST_OK;
}



// ================ Test modes declaration ===============

int main() {
    uint32_t test_result = 0;

    rumboot_printf("UART interface test has been started.\n");

    READ_FROM_FIFO = 0;
    UART_TX_CTS_INT = 0;

    rumboot_irq_cli();
    struct rumboot_irq_entry *tbl = rumboot_irq_create( NULL );

    rumboot_irq_set_handler(tbl, UARTRX_INT, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, irq_handler_uart_receiver, (void*)0);
    rumboot_irq_set_handler(tbl, UARTTX_INT, RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, irq_handler_uart_transmitter, (void*)0);

    /* Activate the table */
    rumboot_printf("UART: RXIRQ %d TXIRQ %d\n", UARTRX_INT, UARTTX_INT);
    rumboot_irq_table_activate( tbl );
    rumboot_irq_enable( UARTRX_INT );
    rumboot_irq_enable( UARTTX_INT );
    rumboot_irq_sei();

#ifdef CHECK_REGISTERS

    rumboot_putstring("Checking UART registers default values ...\n");
    uart_check_defaults(UARTRX_BASE);

#endif //CHECK_REGISTERS

    rumboot_printf("Testing UART ...\n");
    test_result = test_uart(UARTTX_BASE, UARTRX_BASE);
    if (test_result == TEST_ERROR)
    {
        rumboot_printf("Error while testing UART.\n");
        return test_result;
    }

    rumboot_irq_table_activate(NULL);
    rumboot_irq_free(tbl);

    rumboot_printf("UART interface test has been finished.\n");

    return test_result;
}
