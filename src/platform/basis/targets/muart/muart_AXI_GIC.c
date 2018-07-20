#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <rumboot/bootheader.h>
#include <rumboot/testsuite.h>
#include <rumboot/timer.h>
#include <rumboot/rumboot.h>
#include <platform/devices.h>
#include <platform/interrupts.h>
#include <regs/regs_muart.h>
#include <devices/muart.h>
#include <regs/regs_mdma.h>
#include <devices/mdma_chan_api.h>


struct rumboot_irq_entry *tbl;

static uint32_t BASE[] = {UART0_BASE, UART1_BASE};
static uint32_t INTR[] = {UART0_INTR, UART1_INTR};
volatile uint32_t M[2];

static void handler(int irq, void *arg)
{
        volatile uint32_t *M = arg;
        uint32_t reg;
        M[1]++;
        reg = ioread32(M[0] + MUART_STATUS);
        rumboot_printf("MUART_STATUS = %x\n", reg);
        rumboot_printf("IRQ arrived, arg=%x base=%x irq=%d, count=%d\n",
                      arg, M[0], irq, M[1]);
}

static void set_muart(uint32_t base)
{
    struct muart_conf conf;
       conf.dum = 1;
       conf.wlen = WL_8;
       conf.stp2 = STP1;
       conf.is_even = 0;
       conf.is_parity = 1;
       conf.mode = RS_422;
       conf.is_loopback = 1;       //is_loopback
       conf.baud_rate = 12500000;  // bod/s (bdiv=4)
       conf.is_dma = 1;            // Using MDMA


    muart_soft_reset(base);
    muart_init(base, &conf);
}

static bool test_AXI_GIC(uint32_t uart_N)
{
    struct mdma_chan *chan_wr;
    struct mdma_chan *chan_rd;

    uint32_t base = BASE[uart_N];
    uint32_t intr = INTR[uart_N];
    uint32_t  reg_fifo_state=0;
#define FIFOWM  0x0100010
    int size = (FIFOWM & 0xFF)+4;
    char * dst_addr = NULL, *src_addr = NULL;
    int heap_id = 0, i;

    if((dst_addr=(char *)rumboot_malloc_from_heap(heap_id,size+4)) == NULL){
           rumboot_printf("UNTESTED: ERROR rumboot_malloc_from_heap\n");
           return 2;
       }
       if((src_addr = (char *)rumboot_malloc_from_heap(heap_id,size+4)) == NULL){
           rumboot_printf("UNTESTED: ERROR rumboot_malloc_from_heap\n");
           return 2;
       }
       for (i = 0; i < size; i++){
          src_addr[i] = i;
          dst_addr[i] = 0;
      }

    rumboot_printf("====== Interrupt by FIFO filling\n");

    //Set handler
/*    M[0]=base;
    M[1]=0;
    rumboot_irq_set_handler(tbl, intr, 0, handler, (void *) &M);
    // Configure and Activate the table
    rumboot_irq_table_activate(tbl);
    rumboot_irq_enable(intr); //UART0_INTR);
    rumboot_irq_sei();
*/
    set_muart(base);
    set_reg(base, MUART_FIFOWM, FIFOWM); //0x2000200
    set_reg(base, MUART_MASK, 0x1FF); //[0:8]
    muart_enable(base);
    muart_dump(base);

    // Interrupt call
    rumboot_printf("====== Interrupt call\n");

    // Create DMA channel for read.  dma --> uart
    rumboot_printf("======= Create DMA channel for read.  dma --> uart\n");
    chan_rd = muart_set_chan_mdma_R(base,(void *)src_addr, size);
    if (chan_rd == NULL) {
       rumboot_printf("READ FAIL\n");
       rumboot_free(src_addr);
       rumboot_free(dst_addr);
       return 1;
    }
    // Create DMA channel for write.  uart --> dma
    rumboot_printf("======= Create DMA channel for write.  uart --> dma\n");
    chan_wr = muart_set_chan_mdma_W(base, (void *)dst_addr, size);
    if (chan_wr == NULL) {
       rumboot_printf("WRITE FAIL\n");
       return 1;
    }
    ///////////////////////////////
    //Set handler
    M[0]=base;
    M[1]=0;
    rumboot_irq_set_handler(tbl, intr, 0, handler, (void *) &M);
    // Configure and Activate the table
    rumboot_irq_table_activate(tbl);
    rumboot_irq_enable(intr); //UART0_INTR);
    rumboot_irq_sei();
    //////////////////////////////////////////////
    mdma_chan_start(chan_wr); // uart --> dma
    mdma_chan_start(chan_rd); //dma --> uart

    reg_fifo_state = ioread32(base + MUART_FIFO_STATE);
    rumboot_printf("fifo_state=%x limit=%x\n",reg_fifo_state,FIFOWM&0xFF);
    rumboot_printf("======= Wait transmit\n");
    if (mdma_chan_wait_trans_end(chan_wr, 1000)) {
           rumboot_printf("TEST FAIL: wait_trans_end error\n");
    }
    reg_fifo_state = ioread32(base + MUART_FIFO_STATE);
    rumboot_printf("fifo_state=%x limit=%x\n",reg_fifo_state,FIFOWM&0xFF);
    if ( M[1] == 0) {
        rumboot_printf("FAIL:Uart interrupt is not happened \n");
        return false;
    }
//    reg_fifo_state = ioread32(base + MUART_FIFO_STATE);
//    rumboot_printf("fifo_state=%x limit=%x\n",reg_fifo_state,FIFOWM&0xFF);
//    if (reg_fifo_state < (FIFOWM&0xFF)) {
//        rumboot_printf("FAIL:Uart interrupt is happened before\n");
//        return false;
//    }
    return true;
}
/* Declare the testsuite structure */
TEST_SUITE_BEGIN(AXI_GIC_test, "AXI Interface and GIC")
TEST_ENTRY("test_AXI_GIC_base_0", test_AXI_GIC, 0), //UART0_BASE
TEST_ENTRY("test_AXI_GIC_base_1", test_AXI_GIC, 1), //UART1_BASE
TEST_SUITE_END();

uint32_t main()
{
    int res = 0;

    rumboot_printf("Test 1.4 MUART AXI GIC\n");

    /* Disable all interrupts */
    rumboot_irq_cli();

    /* Create an IRQ table */
    tbl = rumboot_irq_create(NULL);

    /* Run AXI Interface and GIC test suite */
    res = test_suite_run(NULL, &AXI_GIC_test);
    if (res == 0) {
       rumboot_printf("TEST PASS: %d errors happened\n", res);
    }else{
        rumboot_printf("TEST FAIL: %d errors happened\n", res);
    }

    /* Deinit */
    rumboot_irq_table_activate(NULL);
    rumboot_irq_free(tbl);
    return res;
}
