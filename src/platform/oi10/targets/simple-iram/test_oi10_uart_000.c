#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <platform/devices.h>
#include <platform/interrupts.h>
#include <platform/regs/fields/mpic128.h>
#include <platform/devices/uart.h>
#include <platform/test_assert.h>
#include <regs/regs_uart.h>
#include <devices/uart.h>


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
#define     UARTFR_MSK          0x1FF
#define     UARTILPR_MSK        0xFF
#define     UARTIBRD_MSK        0xFFFF
#define     UARTFBRD_MSK        0x3F
#define     UARTLCR_H_MSK       0xFF
#define     UARTCR_MSK          0xFF87
#define     UARTIFLS_MSK        0x3F
#define     UARTIMSC_MSK        0x7FF
#define     UARTRIS_MSK         0x7FF
#define     UARTMIS_MSK         0x7FF
#define     UARTICR_MSK         0x7FF
#define     UARTDMACR_MSK       0x7
#define     UARTPeriphID0_MSK   0xFF
#define     UARTPeriphID1_MSK   0xFF
#define     UARTPeriphID2_MSK   0xFF
#define     UARTPeriphID3_MSK   0xFF
#define     UARTPCellID0_MSK    0xFF
#define     UARTPCellID1_MSK    0xFF
#define     UARTPCellID2_MSK    0xFF
#define     UARTPCellID3_MSK    0xFF


#define TEST_OK     0
#define TEST_ERROR  1

#define UART_TEST_DATA_LEN  32

#define CHECK_DEFAULT_VALUES 1


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



uint32_t read_from_uart(uint32_t base_addr, const uint8_t* data);

static volatile uint32_t READ_FROM_FIFO;

static void irq_handler_uart0(int irq, void* arg)
{
    uint32_t uart0_status;
    rumboot_printf("UART0 interrupt handler\n");

    uart0_status = reg_read(UART0_BASE, UARTRIS);
    if (uart0_status & 0x10){

        read_from_uart(UART0_BASE, data_seq_arr);

        READ_FROM_FIFO = 1;
        reg_write(UART0_BASE, UARTICR, 0x7FF);//clear interrupts
    }
}

static void irq_handler_uart1(int irq, void* arg)
{
    uint32_t uart1_status;
    rumboot_printf ("UART1 interrupt handler\n");

    uart1_status = reg_read(UART1_BASE, UARTRIS);
    if (uart1_status & 0x10){

        read_from_uart(UART1_BASE, data_seq_arr);

        READ_FROM_FIFO = 1;
        reg_write(UART1_BASE, UARTICR, 0x7FF);//clear interrupts
    }
}



uint32_t wait_uart_int(){
    unsigned t;

    rumboot_printf("wait_uart_int\n");

    for (t=1; t<=UART_TIMEOUT; t++){
        if (READ_FROM_FIFO == 1){
            READ_FROM_FIFO = 0;
            break;
        }
    }
    if (t >= UART_TIMEOUT) {
        rumboot_printf("Error! READ_FROM_FIFO flag wait timeout!\n");
        return TEST_ERROR;
    }
    return TEST_OK;
}


int uart_check_defaults(uint32_t base_addr)
{
    int result = 0;

    uint32_t value;

    value = reg_read(base_addr, UARTRSR);
    TEST_ASSERT((value & UARTRSR_DEFAULT) == UARTRSR_DEFAULT, "UARTRSR - incorrect default value!");

    value = reg_read(base_addr, UARTFR);
    TEST_ASSERT((value & UARTFR_DEFAULT) == UARTFR_DEFAULT, "UARTFR - incorrect default value!");

    value = reg_read(base_addr, UARTILPR);
    TEST_ASSERT((value & UARTILPR_DEFAULT) == UARTILPR_DEFAULT, "UARTILPR - incorrect default value!");

    value = reg_read(base_addr, UARTIBRD);
    TEST_ASSERT((value & UARTIBRD_DEFAULT) == UARTIBRD_DEFAULT, "UARTIBRD - incorrect default value!");

    value = reg_read(base_addr, UARTFBRD);
    TEST_ASSERT((value & UARTFBRD_DEFAULT) == UARTFBRD_DEFAULT, "UARTFBRD - incorrect default value!");

    value = reg_read(base_addr, UARTLCR_H);
    TEST_ASSERT((value & UARTLCR_H_DEFAULT) == UARTLCR_H_DEFAULT, "UARTLCR_H - incorrect default value!");

    value = reg_read(base_addr, UARTCR);
    TEST_ASSERT((value & UARTCR_DEFAULT) == UARTCR_DEFAULT, "UARTCR - incorrect default value!");

    value = reg_read(base_addr, UARTIFLS);
    TEST_ASSERT((value & UARTIFLS_DEFAULT) == UARTIFLS_DEFAULT, "UARTIFLS - incorrect default value!");

    value = reg_read(base_addr, UARTIMSC);
    TEST_ASSERT((value & UARTIMSC_DEFAULT) == UARTIMSC_DEFAULT, "UARTIMSC - incorrect default value!");

    value = reg_read(base_addr, UARTRIS);
    TEST_ASSERT((value & UARTRIS_DEFAULT) == UARTRIS_DEFAULT, "UARTRIS - incorrect default value!");

    value = reg_read(base_addr, UARTMIS);
    TEST_ASSERT((value & UARTMIS_DEFAULT) == UARTMIS_DEFAULT, "UARTMIS - incorrect default value!");

    value = reg_read(base_addr, UARTDMACR);
    TEST_ASSERT((value & UARTDMACR_DEFAULT) == UARTDMACR_DEFAULT, "UARTDMACR - incorrect default value!");

    value = reg_read(base_addr, UARTPeriphID0);
    TEST_ASSERT((value & UARTPeriphID0_DEFAULT) == UARTPeriphID0_DEFAULT, "UARTPeriphID0 - incorrect default value!");

    value = reg_read(base_addr, UARTPeriphID1);
    TEST_ASSERT((value & UARTPeriphID1_DEFAULT) == UARTPeriphID1_DEFAULT, "UARTPeriphID1 - incorrect default value!");

    value = reg_read(base_addr, UARTPeriphID2);
    TEST_ASSERT((value & UARTPeriphID2_DEFAULT) == UARTPeriphID2_DEFAULT, "UARTPeriphID2 - incorrect default value!");

    value = reg_read(base_addr, UARTPeriphID3);
    TEST_ASSERT((value & UARTPeriphID3_DEFAULT) == UARTPeriphID3_DEFAULT, "UARTPeriphID3 - incorrect default value!");

    value = reg_read(base_addr, UARTPCellID0);
    TEST_ASSERT((value & UARTPCellID0_DEFAULT) == UARTPCellID0_DEFAULT, "UARTPCellID0 - incorrect default value!");

    value = reg_read(base_addr, UARTPCellID1);
    TEST_ASSERT((value & UARTPCellID1_DEFAULT) == UARTPCellID1_DEFAULT, "UARTPCellID1 - incorrect default value!");

    value = reg_read(base_addr, UARTPCellID2);
    TEST_ASSERT((value & UARTPCellID2_DEFAULT) == UARTPCellID2_DEFAULT, "UARTPCellID2 - incorrect default value!");

    value = reg_read(base_addr, UARTPCellID3);
    TEST_ASSERT((value & UARTPCellID3_DEFAULT) == UARTPCellID3_DEFAULT, "UARTPCellID3 - incorrect default value!");

    return result;
}

void write_to_uart(uint32_t base_addr, const uint8_t* data, size_t data_len)
{
    rumboot_printf("Write:\n");
    uint32_t j;
    for (j = 0; j < data_len; j++){
        rumboot_printf("data_seq_arr[%d] = 0x%x\n", j, data[j]);
        uart_putc(base_addr, data[j]);
    }
    rumboot_printf("Finish Write\n");
}


static uint32_t data_index = 0;

uint32_t read_from_uart(uint32_t base_addr, const uint8_t* data)
{
    rumboot_printf("Read:\n");
    while(!uart_check_rfifo_empty(base_addr))
    {
        uint8_t readval = uart_getc(base_addr);

        rumboot_printf("UART_BASE+UARTDR = 0x%x\n", base_addr + UARTDR);
        rumboot_printf("readval = 0x%x\n", readval);
        if (readval != data[data_index++]){
            rumboot_printf("Error on compare transmit and receive data!\n");
            rumboot_printf("Read: readval = 0x%x\n", readval);
            rumboot_printf("Expected: data_seq_arr[%d] = 0x%x\n", data_index, data[data_index]);
            return 1;
        }
    }

    return 0;
}


uint32_t test_uart(uint32_t UART_TRANSMITTER_BASE, uint32_t UART_RECEIVER_BASE, uint32_t irq_num) {
    rumboot_printf("------- Test UART -------\n");
    rumboot_printf("UART_BASE = 0x%x\n", UART_RECEIVER_BASE);

    uint32_t test_result, j;

    test_result = 0;



    uart_init(UART_TRANSMITTER_BASE, UART_word_length_8bit, 6250000, UART_parity_no, 0x30, 0);
    uart_fifos_enable(UART_TRANSMITTER_BASE, true);
    uart_rts_cts_enable(UART_TRANSMITTER_BASE, true);
    uart_tx_enable(UART_TRANSMITTER_BASE, true);
    uart_rx_enable(UART_TRANSMITTER_BASE, true);
    uart_enable(UART_TRANSMITTER_BASE, true);

    uart_init(UART_RECEIVER_BASE, UART_word_length_8bit, 6250000, UART_parity_no, 0x30, 0);
    uart_fifos_enable(UART_RECEIVER_BASE, true);
    uart_rts_cts_enable(UART_RECEIVER_BASE, true);
    uart_tx_enable(UART_RECEIVER_BASE, true);
    uart_rx_enable(UART_RECEIVER_BASE, true);
    uart_enable(UART_RECEIVER_BASE, true);


    rumboot_printf("Write:\n");
    for (j = 0; j < UART_TEST_DATA_LEN; j++){
        rumboot_printf("data_seq_arr[%d] = 0x%x\n", j, data_seq_arr[j]);
        uart_putc(UART_TRANSMITTER_BASE, data_seq_arr[j]);
    }
    rumboot_printf("Finish Write\n");


    rumboot_printf("Waiting for interrupt ...\n");
    if (wait_uart_int() == TEST_ERROR){
       return TEST_ERROR;
    }


//    rumboot_printf("Read:\n");
//    for (j = 0; j < UART_TEST_DATA_LEN; j++){
//        readval = uart_getc(UART_RECEIVER_BASE);
//
//        rumboot_printf("UART_BASE+UARTDR = 0x%x\n", UART_RECEIVER_BASE+UARTDR);
//        rumboot_printf("readval = 0x%x\n", readval);
//        if (readval != data_seq_arr[j]){
//            rumboot_printf("Error on compare transmit and receive data!\n");
//            rumboot_printf("Read: readval = 0x%x\n", readval);
//            rumboot_printf("Expected: data_seq_arr[%d] = 0x%x\n", j, data_seq_arr[j]);
//            test_result = TEST_ERROR;
//            break;
//        }
//    }

   return test_result;
}



// ================ Test modes declaration ===============

int main() {
    uint32_t test_result = 0;

    rumboot_printf("UART interface test has been started.\n");

    READ_FROM_FIFO = 0;

    rumboot_irq_cli();
    struct rumboot_irq_entry *tbl = rumboot_irq_create( NULL );

    rumboot_irq_set_handler(tbl, UART0_INT, (int_sense_level << MPIC128_VP_S_i) | (int_pol_high << MPIC128_VP_POL_i), irq_handler_uart0, (void*)0);
    rumboot_irq_set_handler(tbl, UART1_INT, (int_sense_level << MPIC128_VP_S_i) | (int_pol_high << MPIC128_VP_POL_i), irq_handler_uart1, (void*)0);

    /* Activate the table */
    rumboot_irq_table_activate( tbl );
    rumboot_irq_enable( UART0_INT );
    rumboot_irq_enable( UART1_INT );
    rumboot_irq_sei();

#ifdef CHECK_DEFAULT_VALUES

    rumboot_putstring("Checking UART0 registers default values ...\n");
    uart_check_defaults(UART0_BASE);

    rumboot_putstring("Checking UART1 registers default values ...\n");
    uart_check_defaults(UART1_BASE);

#endif //CHECK_DEFAULT_VALUES

    rumboot_printf("Testing UART0 ...\n");
    test_result = test_uart(UART1_BASE, UART0_BASE, UART0_INT);
    if (test_result == TEST_ERROR){
        rumboot_printf("Error while testing UART0.\n");
        return 1;
    }

    rumboot_printf ("Testing UART1\n");
    test_result = test_uart(UART0_BASE, UART1_BASE, UART1_INT);
    if (test_result == TEST_ERROR){
        rumboot_printf("Error while testing UART1.\n");
        return 1;
    }


    rumboot_irq_table_activate(NULL);
    rumboot_irq_free(tbl);

    rumboot_printf("UART interface test has been finished.\n");

    return test_result;
}
