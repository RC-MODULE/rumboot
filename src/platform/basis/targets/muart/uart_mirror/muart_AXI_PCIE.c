#include <stdio.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <regs/regs_muart.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>

#include <rumboot/rumboot.h>
#include <rumboot/timer.h>
#include <platform/devices.h>
#include <platform/interrupts.h>
#include <devices/muart.h>
#include <regs/regs_mdma.h>
#include <devices/mdma_chan_api.h>


#define MDMA_GP_SEGMENT_DDR	512
#define MDMA_GP_SEGMENT_IM0	128
#define MDMA_GP_SEGMENT_IM1	256

static void *all_addr[4][2];

static int test_alloc_mem(const char *name, void *seg_addr[], int seg_size)
{
        int i, id;
        int ret = 0;
        id = rumboot_malloc_heap_by_name(name);
        if (id < 0) {
                return -1;
        }
        rumboot_printf("test -  allocate memory\n");
        for (i = 0; i < 2; i++) {
            seg_addr[i] = rumboot_malloc_from_heap_aligned(id, seg_size, ALIGN8);
            if (!seg_addr[i]) {
                ret = -2;
                break;
            }
        }
        if (i != 2) {
                for (; i > 0; i--) rumboot_free(seg_addr[i - 1]);
        }
        return ret;
}
static struct mdma_chan *muart_set_chan_DDR(uint32_t base,
                                         void *data, size_t len,int R_W)
{
    struct mdma_chan *chan_uart = NULL;
    struct mdma_cfg cfg;
    int  ret = 0;
    uint32_t R=0;
    //CONFIG DMA
    cfg.mode = MDMA_CHAN_POLLING;
    cfg.desc_kind = LONG_DESC;  //NORMAL_DESC;
    cfg.heap_id = 1;
    cfg.burst_width = MDMA_BURST_WIDTH8; // !!!!!
    cfg.event_indx = -1;
    cfg.add_info = true;  //true for custom !!!!!
    cfg.addr_inc = true;

    if(R_W == R){
        R = TX_CHANNEL_R;
    }else{
        R = RX_CHANNEL_W;
    }
    chan_uart = mdma_chan_create((void *)(base + R), &cfg);
    if (!chan_uart) {
          ret = 2;       goto to_exit;
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

int AXI_mem_access(int heap_id)
{
    uint32_t BASE[] = {UART0_BASE, UART1_BASE};
    struct muart_conf conf;
    size_t size = 8;
    struct mdma_chan *chan_wr;
    struct mdma_chan *chan_rd;

    int b, i, res=0;
    uint32_t  reg;
    char * dst_addr = NULL, *src_addr = NULL;

    if((dst_addr=(char *)rumboot_malloc_from_heap_aligned(heap_id,size+8,ALIGN8)) == NULL){
        rumboot_printf("UNTESTED: ERROR rumboot_malloc_from_heap\n");
        return 2;
    }
    if((src_addr = (char *)rumboot_malloc_from_heap_aligned(heap_id,size+8,ALIGN8)) == NULL){
        rumboot_printf("UNTESTED: ERROR rumboot_malloc_from_heap\n");
        return 2;
    }
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
        conf.mode = RS_422;
        conf.is_loopback = 0;
        conf.baud_rate = 12500000;  // bod/s (bdiv=4)
        conf.is_dma = 1;            // Using MDMA

        muart_init(BASE[b], &conf);
        muart_enable(BASE[b]);
        muart_dump(BASE[b]);
    } //base loop

    // Create DMA channel for read.  dma --> uart
//    chan_rd = muart_set_chan_mdma_R(BASE[0],(void *)src_addr, size);
    chan_rd = muart_set_chan_DDR(BASE[0],(void *)src_addr, size,0);
    if (chan_rd == NULL) {
        rumboot_printf("READ FAIL\n");
        rumboot_free(src_addr);
        rumboot_free(dst_addr);
        return 1;
    }
    // Create DMA channel for write.  uart --> dma
//    chan_wr = muart_set_chan_mdma_W(BASE[1], (void *)dst_addr, size);
    chan_wr = muart_set_chan_DDR(BASE[1],(void *)dst_addr, size,1);
    if (chan_wr == NULL) {
        rumboot_printf("WRITE FAIL\n");
        return 1;
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
        rumboot_printf("read MDMA_STATE=%x expact DUMMY=%x\n",
                        reg,MDMA_CHAN_DUMMY);
        reg = mdma_chan_get_state(chan_wr);
        rumboot_printf("write MDMA_STATE=%x expact DUMMY=%x\n",
                        reg,MDMA_CHAN_DUMMY);
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
    rumboot_free(src_addr);
    rumboot_free(dst_addr);
    if(res != 0){
       rumboot_printf("TEST FAIL:\n");
       return 1;
    }
    rumboot_printf("TEST PASS:\n");
    return 0;
}

uint32_t main()
{
    int i, res=0, res_fail=0, cnt_test=0;
    int heap_id = 0, heap_cnt=0;
    char *heaps_name[10];
#define t_heap 1
//    char *testing_heaps[t_heap] = {"IM0", "DDR0", "DDR1", "PCIE-IM0"};
    char *testing_heaps[t_heap] = {"PCIE-IM0"};

    rumboot_printf("Test 2.4 \n");
    rumboot_printf("Check access MUART to PCIE-IM0 memory\n");

    heap_cnt = rumboot_malloc_num_heaps();
    if (heap_cnt == 0){
        rumboot_printf("UNTESTED: Heap count %d\n", heap_cnt);
    }
    for (i = 0; i < heap_cnt; i++) {
        heaps_name[i] = (char *)rumboot_malloc_heap_name(i);
    }
    rumboot_printf("Avaleble heaps\n");
    for (i = 0; i < heap_cnt; i++) {
        rumboot_printf("        %d. %s start 0x%x -- %x \n",
                i, heaps_name[i],
                rumboot_platform_runtime_info.heaps[i].start,
                rumboot_platform_runtime_info.heaps[i].end );
    }

    // Choose heap and start
    for (heap_id = 0; heap_id < heap_cnt; heap_id++) {
        for(i = 0; i < t_heap; i++) {
            if ((strcmp(heaps_name[heap_id],testing_heaps[i])) == 0) break;
        }
        if(i >= t_heap) continue;
        rumboot_printf("test_alloc_mem  %s heap=%d\n",heaps_name[heap_id],heap_id);
        if (test_alloc_mem(heaps_name[heap_id], all_addr[i], MDMA_GP_SEGMENT_IM0)) {
                rumboot_printf("ERROR:%s\n",heaps_name[heap_id]);
                return -2;
        }
        rumboot_printf("START TEST FOR HEAP %d. %s\n", heap_id, heaps_name[heap_id]);
        cnt_test++;
        res=AXI_mem_access(heap_id);
        if (res != 0)
           res_fail++;
    }

    if (cnt_test == 0) {
        rumboot_printf("UNTESTED: Needed heap is not found\n");
        res_fail++;
    }
    if (res_fail == 0) {
           rumboot_printf("TEST PASS: %d errors happened\n", res_fail);
    }else{
            rumboot_printf("TEST FAIL: %d errors happened\n", res_fail);
    }
    return res_fail;
}
