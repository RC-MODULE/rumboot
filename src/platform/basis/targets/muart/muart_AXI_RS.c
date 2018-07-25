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

char * dst_addr = NULL, *src_addr = NULL;
#define size 16
static bool test_AXI_RS(uint32_t RS)
// RS_232 = 0
// RS_485 = 1
// RS_422 = 2
{
    uint32_t BASE[] = {UART0_BASE, UART1_BASE};
    struct muart_conf conf;

    struct mdma_chan *chan_wr;
    struct mdma_chan *chan_rd;

    int b, i,   res=0;
    uint32_t  reg;


    for (i = 0; i < size; i++){
       src_addr[i] = i;
       dst_addr[i] = 0;
    }

    for ( b=0; b<= 1; b++) {        //base loop
        muart_soft_reset(BASE[b]);
        conf.dum = 1;
        conf.wlen = WL_8;
        conf.stp2 = STP1;
        conf.is_even = 0;
        conf.is_parity = 1;
        conf.mode = RS;
        conf.is_loopback = 0;
        conf.baud_rate = 12500000;  // bod/s (bdiv=4)
        conf.is_dma = 1;            // Using MDMA

        muart_init(BASE[b], &conf);
        muart_enable(BASE[b]);
        muart_dump(BASE[b]);
    } //base loop

    // Create DMA channel for read.  dma --> uart
    chan_rd = muart_set_chan_mdma_R(BASE[0],(void *)src_addr, size);
    if (chan_rd == NULL) {
       rumboot_printf("READ FAIL\n");
       return false;
    }
    // Create DMA channel for write.  uart --> dma
    chan_wr = muart_set_chan_mdma_W(BASE[1], (void *)dst_addr, size);
    if (chan_wr == NULL) {
       rumboot_printf("WRITE FAIL\n");
       return false;
    }

    mdma_chan_start(chan_wr); // uart --> dma
    mdma_chan_start(chan_rd); //dma --> uart

    if (mdma_chan_wait_trans_end(chan_wr, 1000)) {
        rumboot_printf("TEST FAIL: wait_trans_end error\n");
        res=1;
    }else{
        for (int i = 0; i < size; i++) {
           if (dst_addr[i] != src_addr[i]) {
              rumboot_printf("TEST FAIL:dst_addr[%d]=%d src_addr[%d]=%d\n",
                        i,dst_addr[i],i,src_addr[i]);
              res=2;
            }
        }
    }


    if(res != 0){
        reg = mdma_chan_get_state(chan_rd);
        rumboot_printf("read MDMA_STATE=%x expact DUMMY=%x\n",reg,MDMA_CHAN_DUMMY);
        reg = mdma_chan_get_state(chan_wr);
        rumboot_printf("write MDMA_STATE=%x expact DUMMY=%x\n",reg,MDMA_CHAN_DUMMY);
        reg = ioread32(BASE[0] + MUART_FIFO_STATE);
        rumboot_printf("read MUART_FIFO_STATE=%x \n", reg);
        reg = ioread32(BASE[0] + MUART_STATUS);
        rumboot_printf("read MUART_STATUS = %x\n", reg);
        reg = ioread32(BASE[1] + MUART_FIFO_STATE);
        rumboot_printf("write MUART_FIFO_STATE=%x \n", reg);
        reg = ioread32(BASE[1] + MUART_STATUS);
        rumboot_printf("write MUART_STATUS = %x\n", reg);
    }
    muart_finish_transmit_mdma(chan_rd, chan_wr);

    if(res != 0){
        rumboot_printf("TEST FAIL:\n");
        return false;
    }
    rumboot_printf("TEST PASS:\n");
    return true;
}
/* Declare the testsuite structure */
TEST_SUITE_BEGIN(AXI_RS_test, "AXI control flow")
TEST_ENTRY("test_AXI_RS_232", test_AXI_RS, RS_232),
TEST_ENTRY("test_AXI_RS_485", test_AXI_RS, RS_485),
TEST_ENTRY("test_AXI_RS_422", test_AXI_RS, RS_422),
TEST_SUITE_END();

uint32_t main()
{
    int res = 0;
    int heap_id = 0;
//    char * dst_addr = NULL, *src_addr = NULL;

    rumboot_printf("Test 1.6 MUART AXI transmit in RS standards\n");

    if((dst_addr=(char *)rumboot_malloc_from_heap_aligned(heap_id,size+4,ALIGN4)) == NULL){
        rumboot_printf("UNTESTED: ERROR rumboot_malloc_from_heap\n");
        rumboot_free(dst_addr);
        return 2;
    }
    if((src_addr = (char *)rumboot_malloc_from_heap_aligned(heap_id,size+4,ALIGN4)) == NULL){
        rumboot_printf("UNTESTED: ERROR rumboot_malloc_from_heap\n");
        return 2;
    }
    res = test_suite_run(NULL, &AXI_RS_test);
    if (res == 0) {
       rumboot_printf("TEST PASS: %d errors happened\n", res);
    }else{
        rumboot_printf("TEST FAIL: %d errors happened\n", res);
    }

    rumboot_free(src_addr);
    rumboot_free(dst_addr);
    return res;
}
