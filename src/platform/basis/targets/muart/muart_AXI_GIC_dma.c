#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <regs/regs_muart.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>

#include <platform/devices.h>
#include <platform/interrupts.h>
#include <devices/muart.h>
#include <rumboot/testsuite.h>
#include <rumboot/timer.h>
#include <regs/regs_mdma.h>
#include <devices/mdma_chan_api.h>
#include <rumboot/rumboot.h>

struct rumboot_irq_entry *tbl;

static uint32_t BASE[] = {UART0_BASE, UART1_BASE};
static uint32_t INTR[] = {UART0_INTR, UART1_INTR};
struct mdma_gp_dev {
        void *base_addr;
        int irq;

        struct mdma_chan *chan_rd;
//        struct mdma_chan *chan_wr;
};
int cnt_rd=0, cnt=0;

static void handler(int irq, void *arg)
{
    struct mdma_gp_dev *dev = (struct mdma_gp_dev *)arg;
    struct mdma_chan *chan=NULL;
    int ret;
    uint32_t base, reg, reg_gen;

    cnt++;
    base = (uint32_t)dev->base_addr;
    rumboot_printf("IRQ arrived, base=%x irq=%d, count=%d\n",
                  base, irq, cnt);
    reg_gen = ioread32(base + MUART_GEN_STATUS);
    reg = ioread32(base + MUART_STATUS);
    rumboot_printf("MUART_STATUS = 0x%x\n", reg);
    rumboot_printf("MUART_GEN_STATUS = 0x%x\n", reg_gen);

    if(reg_gen == 0) {
        rumboot_printf("TEST FAIL:There is no MUART interrupt\n");
        rumboot_printf("MUART_GEN_STATUS is null\n");
        cnt=0;
        return;
    }
    if (reg_gen & 1) { //MUART interrupt
        rumboot_printf("TEST FAIL:MUART interrupt\n");
        rumboot_printf("Expact:MDMA channel interrupt\n");
    }
    if (reg_gen & 2) { //MDMA read channel interrupt
        chan = dev->chan_rd;
        cnt_rd++;
        rumboot_printf("TEST PASS: MDMA read channel interrupt\n");
    }
/*    if (reg_gen & 4) { //MDMA write channel interrupt
        chan = dev->chan_wr;
        cnt_wr++;
        rumboot_printf("TEST PASS: MDMA write channel interrupt\n");
    }
*/
    if(chan != NULL){
        ret = mdma_chan_irq_cb(chan);
        rumboot_printf("Channel interrupt = 0x%x\n", ret);
    }
    rumboot_printf("Counts cnt_rd=%d cnt=%d\n", cnt_rd,cnt);
    return;

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
////       conf.is_loopback = 1;       //is_loopback
       conf.baud_rate = 12500000;  // bod/s (bdiv=4)
       conf.is_dma = 1;            // Using MDMA


    muart_soft_reset(base);
    muart_init(base, &conf);
}
static struct mdma_chan *muart_set_chan_interrupt(uint32_t base,
                                         void *data, size_t len,int R_W)
{
    struct mdma_chan *chan_uart = NULL;
    struct mdma_cfg cfg;
    int  ret = 0;
    //   uint32_t reg;
    uint32_t R=0;

    //CONFIG DMA
    cfg.mode = MDMA_CHAN_INTERRUPT;
 //   cfg.mode = MDMA_CHAN_POLLING;
    cfg.desc_kind = LONG_DESC;  //NORMAL_DESC;
    cfg.heap_id = 1;
    cfg.burst_width = MDMA_BURST_WIDTH4; // !!!!!
    cfg.event_indx = -1;
    cfg.add_info = true;  //true for custom !!!!!
    cfg.addr_inc = true;

    if(R_W == R){
        R = TX_CHANNEL_R;
    }else{
        R = RX_CHANNEL_W;
    }
    chan_uart = mdma_chan_create((void *)(base + R), &cfg);
    if (!chan_uart) { //goto test_exit_2;
          ret = -2;       goto to_exit;
    }
    if (mdma_trans_create(chan_uart, MDMA_TRANS_QUEUE)) {
           ret = -4;        goto to_exit;
    }
           // Init and add description to write
    if (mdma_trans_add_group(chan_uart, data, len, 0, true)) {
             ret = -7;          goto to_exit;
    }
    if (mdma_chan_config(chan_uart)) {
           ret = -8;        goto to_exit;
    }

    to_exit:
       if(ret < 0) {
               mdma_chan_destroy(chan_uart);
               chan_uart=NULL;
       }

    return chan_uart;
}

static bool test_AXI_GIC(uint32_t uart_N)
{
    struct mdma_gp_dev dev;
    struct mdma_chan *chan_rd;
    uint32_t   reg_fifo_state=0;
    int size = 0x16;
    char * dst_addr = NULL, *src_addr = NULL;
    int heap_id = 0;
    uint32_t base =BASE[uart_N];
    uint32_t intr =INTR[uart_N];


       if((src_addr = (char *)rumboot_malloc_from_heap_aligned(heap_id,size+4, ALIGN4)) == NULL){
           rumboot_printf("UNTESTED: ERROR rumboot_malloc_from_heap\n");
           return false;
       }

    rumboot_printf("====== Interrupt by finish read channel dma\n");

    //Set  MUART
    set_muart(base);
    set_reg(base, MUART_MASK, 0x1FF); //[0:8]
    muart_enable(base);
    muart_dump(base);

    // Create DMA channel for read.  dma --> uart
    rumboot_printf("======= Create DMA channel for read.  dma --> uart\n");
    chan_rd = muart_set_chan_interrupt(base,(void *)src_addr, size,0);
    if (chan_rd == NULL) {
       rumboot_printf("READ FAIL\n");
       rumboot_free(src_addr);
       rumboot_free(dst_addr);
       return false;
    }

    dev.irq=intr;
    dev.chan_rd=chan_rd;
    dev.base_addr=(void *)base;

    ///////////////////////////////
    //Set handler
    rumboot_irq_set_handler(tbl, intr, 0, handler, (void *)&dev);
    // Configure and Activate the table
    rumboot_irq_table_activate(tbl);
    rumboot_irq_enable(intr); //UART0_INTR);
    rumboot_irq_sei();
	cnt_rd=0; cnt=0;
    //////////////////////////////////////////////

    mdma_chan_start(chan_rd); //dma --> uart
    reg_fifo_state = ioread32(base + MUART_FIFO_STATE);
    rumboot_printf("fifo_state=%x \n",reg_fifo_state);

    rumboot_printf("======= Wait transmit\n");
    if (mdma_chan_wait_trans_end(chan_rd, 1000)) {
           rumboot_printf("TEST FAIL: wait_trans_end error\n");
           return false;
    }

   reg_fifo_state = ioread32(base + MUART_FIFO_STATE);
    rumboot_printf("fifo_state=%x \n",reg_fifo_state);
    if ( cnt == 0) {
        rumboot_printf("FAIL:Uart interrupt is not happened \n");
        return false;
    }

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
    rumboot_printf("Test 1.5 MUART AXI EXT_IRQ_GEN\n");
    rumboot_printf("Interrupt by finish mdma\n");

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
