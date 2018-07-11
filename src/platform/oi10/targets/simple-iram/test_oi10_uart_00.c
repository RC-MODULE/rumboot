#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <platform/devices.h>
#include <regs/regs_uart.h>


#define UART0_IRQ_o             101
#define UART1_IRQ_o             102

//---------------------UART registers----------------------------
#define UART_FIFO_DEEP      32
#define UART_TEST_DATA_LEN  4
#define UARTDR              0x000
#define UARTRSR             0x004
#define UARTFR              0x018
#define UARTILPR            0x020
#define UARTIBRD            0x024
#define UARTFBRD            0x028
#define UARTLCR_H           0x02C
#define UARTCR              0x030
#define UARTIFLS            0x034
#define UARTIMSC            0x038
#define UARTRIS             0x03C
#define UARTMIS             0x040
#define UARTICR             0x044
#define UARTDMACR           0x048
//---------------------UART constants---------------------------
#define UART_TIMEOUT        0x500


//GPIO regs
#define GPIODATA    0x000
#define GPIODIR     0x400
#define GPIOIS      0x404
#define GPIOIBE     0x408
#define GPIOIEV     0x40C
#define GPIOIE      0x410
#define GPIORIS     0x414
#define GPIOMIS     0x418
#define GPIOIC      0x41C
#define GPIOAFSEL   0x420
#define GPIOPeriphID0   0xFE0
#define GPIOPeriphID1   0xFE4
#define GPIOPeriphID2   0xFE8
#define GPIOPeriphID3   0xFEC
#define GPIOPCellID0    0xFF0
#define GPIOPCellID1    0xFF4
#define GPIOPCellID2    0xFF8
#define GPIOPCellID3    0xFFC



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



static volatile uint32_t READ_FROM_FIFO;

void NON_CR_INTERRUPT_HANDLER(void)
{
    uint32_t uart0_status, uart1_status;
    rumboot_printf ("NON_CR_INTERRUPT_HANDLER");
    //get interrupt source
    uint32_t source_vector = mpic_get_ncr_interrupt_vector(Processor0);// - clear interrupt by read NCIAR
    rumboot_printf("NON_CRITICAL int handler message");
    rumboot_printf("source_vector = 0x%x", source_vector);
    // rumboot_printf("", UART0_IRQ_o);
    if (source_vector == UART0_IRQ_o) {
        rumboot_printf("UART0_IRQ_o");
        uart0_status = reg_read(UART0_BASE, UARTRIS);
        if (uart0_status&0x10){
            READ_FROM_FIFO = 1;
            reg_write(UART0_BASE, UARTICR, 0x7FF);//clear interrupts
        }
    }
    // rumboot_printf("UART1_IRQ_o = 0x%x", UART1_IRQ_o);
    if (source_vector == UART1_IRQ_o) {
        rumboot_printf("UART1_IRQ_o");
        uart1_status = reg_read(UART1_BASE, UARTRIS);
        if (uart1_status&0x10){
            READ_FROM_FIFO = 1;
            reg_write(UART1_BASE, UARTICR, 0x7FF);//clear interrupts
        }
    }

    END_NONCR_INT_P0;
}
//---------------------------------------
void uart_irq_set(uint32_t irq_num) {
    // Initialization Inrerrupt controller
    READ_FROM_FIFO = 0;
    rumboot_printf("    Start MPIC initialization /n");
    mpic_reset();
    mpic_pass_through_disable();
    mpic_setup_ext_interrupt(irq_num, MPIC_PRIOR_1,int_sense_level,int_pol_pos,Processor0);
    SET_NONCR_INT_HANDLER(non_cr_interrupt_handler);
    ppc_noncr_int_enable();
    mpic_set_interrupt_borders(10, 5);//MCK, CR borders
    mpic_set_minimal_interrupt_priority_mask(Processor0, MPIC_PRIOR_0);
    rumboot_printf("    MPIC ave been initialized /n");
}
//---------------------------------------
uint32_t wait_uart_int(){
    unsigned t;

    rumboot_printf("wait_uart_int/n");

    for (t=1;t<=UART_TIMEOUT;t++){
        if (READ_FROM_FIFO==1){
            READ_FROM_FIFO = 0;
            break;
        }
    }
    if (t>=UART_TIMEOUT) {
        rumboot_printf("Error! READ_FROM_FIFO flag wait timeout!");
        return TEST_ERROR;
    }
    return TEST_OK;
}
//---------------------------------
void init_gpioN(unsigned GPIO_ADDR){
//  rumboot_printf("init_gpioN");
    reg_write(GPIO_ADDR, GPIOAFSEL, 0xFF);
}
//-----------------------------------
void init_gpio(){
//  rumboot_printf("init_gpio");
    init_gpioN(LSIF1_MGPIO2_BASE);
    init_gpioN(LSIF1_MGPIO3_BASE);
}
//---------------------------------
void uart_init(uint32_t UART_BASE){
    //Set int and fract divisors to get baud rate 230400
    rumboot_printf("uart_init/n");

    init_gpio();

    reg_write(UART_BASE, UARTIBRD, 0x09);//set integer part of baud rate divisor
    reg_write(UART_BASE, UARTFBRD, 0x02);//set fractional part of baud rate divisor

    reg_write(UART_BASE, UARTCR, 0x301);//set control register

    reg_write(UART_BASE, UARTLCR_H, 0x74);//set line control register 8 data bit transmited and fifo enable
    reg_write(UART_BASE, UARTIFLS, 0x00);//interrupt fifo level select register, here selec 1/8 full//
    reg_write(UART_BASE, UARTIMSC, 0x30);//interrupt mask set/clear register

    MEM32(UART_BASE+UARTDMACR) = 0x00;//DMA control register
}
//---------------------------------
uint32_t uart_write(uint32_t UART_BASE, uint32_t DATA){
    uint32_t flags, t;

    rumboot_printf("uart_write: ");
    for (t=1;t<=UART_TIMEOUT;t++){
            flags = MEM32(UART_BASE+UARTFR);
//          rumboot_printf("", flags);
            if (!(flags&0x20)){
                break;
            }
    }
    if (t==UART_TIMEOUT){
        rumboot_printf("Error! TX fifo full timeout!");
        return TEST_ERROR;
    }
    MEM32(UART_BASE+UARTDR) = DATA;

    return TEST_OK;
}
//---------------------------------
uint32_t test_uart(uint32_t UART_BASE, uint32_t irq_num) {
    rumboot_printf( "/n------- Test UART -------" );
    rumboot_printf("UART_BASE = 0x%x", UART_BASE);

    uint32_t test_result, j, t, readval,var_UARTDR, var_UARTRSR, var_UARTFR, var_UARTILPR, var_UARTIBRD, var_UARTFBRD, var_UARTLCR_H, var_UARTCR, var_UARTIFLS, var_UARTIMSC, var_UARTRIS, var_UARTMIS, var_UARTICR, var_UARTDMACR;
    test_result = 0;

    //array for testing data exchange
    uint8_t data_seq_arr[UART_TEST_DATA_LEN] = {
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

    uart_irq_set(irq_num);
    uart_init(UART_BASE);

#ifdef read_reg_UART
    var_UARTDR = MEM32(UART_BASE+UARTDR);
    rumboot_printf("UARTDR RW Reset 0x---");
    rumboot_printf("var_UARTDR = 0x%x", var_UARTDR);

    var_UARTRSR = MEM32(UART_BASE+UARTRSR);
    rumboot_printf("UARTRSR RW Reset 0x0");
    rumboot_printf("var_UARTRSR = 0x%x", var_UARTRSR);

    var_UARTFR = MEM32(UART_BASE+UARTFR);
    rumboot_printf("UARTFR RO Reset 0b-10010---");
    rumboot_printf("var_UARTFR = 0x%x", var_UARTFR);

    var_UARTILPR = MEM32(UART_BASE+UARTILPR);
    rumboot_printf("UARTILPR RW Reset 0x00");
    rumboot_printf("var_UARTILPR = 0x%x", var_UARTILPR);

    var_UARTIBRD = MEM32(UART_BASE+UARTIBRD);
    rumboot_printf("UARTIBRD RW Reset 0x0000");
    rumboot_printf("var_UARTIBRD = 0x%x", var_UARTIBRD);

    var_UARTFBRD = MEM32(UART_BASE+UARTFBRD);
    rumboot_printf("UARTFBRD RW Reset 0x00");
    rumboot_printf("var_UARTFBRD = 0x%x", var_UARTFBRD);

    var_UARTLCR_H = MEM32(UART_BASE+UARTLCR_H);
    rumboot_printf("UARTLCR_H RW Reset 0x00");
    rumboot_printf("var_UARTLCR_H = 0x%x", var_UARTLCR_H);

    var_UARTCR = MEM32(UART_BASE+UARTCR);
    rumboot_printf("UARTCR RW Reset 0x0300");
    rumboot_printf("var_UARTCR = 0x%x", var_UARTCR);

    var_UARTIFLS = MEM32(UART_BASE+UARTIFLS);
    rumboot_printf("UARTIFLS RW Reset 0x12");
    rumboot_printf("var_UARTIFLS = 0x%x", var_UARTIFLS);

    var_UARTIMSC = MEM32(UART_BASE+UARTIMSC);
    rumboot_printf("UARTIMSC RW Reset 0x000");
    rumboot_printf("var_UARTIMSC = 0x%x", var_UARTIMSC);

    var_UARTRIS = MEM32(UART_BASE+UARTRIS);
    rumboot_printf("UARTRIS RO Reset 0x00-");
    rumboot_printf("var_UARTRIS = 0x%x", var_UARTRIS);

    var_UARTMIS = MEM32(UART_BASE+UARTMIS);
    rumboot_printf("UARTMIS RO Reset 0x00-");
    rumboot_printf("var_UARTMIS = 0x%x", var_UARTMIS);

//  var_UARTICR = MEM32(UART_BASE+UARTICR);
//  rumboot_printf("UARTICR WO");
//  rumboot_printf("var_UARTICR = 0x%x", var_UARTICR);

    var_UARTDMACR = MEM32(UART_BASE+UARTDMACR);
    rumboot_printf("UARTDMACR RW Reset 0x00");
    rumboot_printf("var_UARTDMACR = 0x%x", var_UARTDMACR);

#endif

    rumboot_printf("Write:/n");
    for (j=0;j<UART_TEST_DATA_LEN;j++){
        rumboot_printf("data_seq_arr[%d] = 0x%x", data_seq_arr[j]);
        test_result = uart_write(UART_BASE, data_seq_arr[j]);
        if (test_result==TEST_ERROR){
            return TEST_ERROR;
        }
    }
    rumboot_printf("Finish Write/n");

    if (wait_uart_int()==TEST_ERROR){
       return TEST_ERROR;
    }

    rumboot_printf("Read:/n");
    for (j=0;j<UART_TEST_DATA_LEN;j++){
        readval = MEM32(UART_BASE+UARTDR);
        rumboot_printf("UART_BASE+UARTDR = 0x%x", UART_BASE+UARTDR);
        rumboot_printf("readval = 0x%x", readval);
        if (readval!=data_seq_arr[j]){
            rumboot_printf("Error on compare transmit and receive data!");
            rumboot_printf("Read: readval = 0x%x", readval);
            rumboot_printf("Expected: data_seq_arr[%d] = 0x%x", data_seq_arr[j]);
            test_result = TEST_ERROR;
            break;
        }
    }
   return test_result;
}



// ================ Test modes declaration ===============

int main() {
    uint32_t test_result = 0;

    rumboot_printf("test UART0");
    test_result = test_uart(UART0_BASE, UART0_IRQ_o);
    if (test_result==TEST_ERROR){
        return 1;
    }

    rumboot_printf ("test UART1");
    test_result = test_uart(UART1_BASE, UART1_IRQ_o);
    if (test_result==TEST_ERROR){
        return 1;
    }

    return test_result;
}
