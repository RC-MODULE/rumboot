//#define EN_PRINT

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <platform/devices.h>
#include <platform/trace.h>
#include <platform/interrupts.h>
#include <platform/regs/regs_hscb.h>
#include <rumboot/irq.h>
#include <rumboot/rumboot.h>
#include <platform/devices/hscb.h>
#include <rumboot/timer.h>


#define TRAN_FLAG       0x00000020
#define LAST_FLAG       0x00000009
    
#define INT_STAT    0x4
#define BAD_STAT    0x8
#define STOP_STAT   0x10

#ifdef ON_BOARD
    #define WAIT_LIMIT  100000
#else
    #define WAIT_LIMIT  1
#endif

#define MAX_ATTEMPT 10

// Addresses
// -------- System --------
#define ID                       0x00000000       // Read only
#define VERSION                  0x00000004       // Read only
#define SW_RESET                 0x00000008
#define SETTINGS                 0x0000000c
#define STATUS                   0x00000010       // Read only
#define IRQ_MASK                 0x00000014
#define TRANS_CLK                0x00000018
#define TIMINGS                  0x0000001c
#define INTERVAL                 0x00000020
#define TIME_CODE_INNER          0x00000024
#define TIME_CODE_OUTER          0x00000028       // Read only
#define DATA_SEND_H              0x0000002c
#define DATA_SEND_L              0x00000030
#define DSTRB_SEND               0x00000034
#define SEND_LEN                 0x00000038
#define DATA_REC_H               0x0000003c       // Read only
#define DATA_REC_L               0x00000040       // Read only
#define DSTRB_REC                0x00000044       // Read only
#define REC_LEN                  0x00000048       // Read only
#define RMAP_PARAMS              0x0000004c
#define RMAP_HIGH_ADDR           0x00000050
#define RMAP_LOW_ADDR            0x00000054
#define RMAP_STATUS              0x00000058       // Read only
#define RMAP_PERV_STATUS         0x0000005c       // Read only
// -------- ADMA --------          00000
#define ADMA_ADMA_ID	         0x00000800
#define ADMA_REVISION            0x00000804
#define ADMA_SW_RST	             0x00000808
#define ADMA_CH_STATUS           0x0000080c
#define ADMA_ARLEN               0x00000810
#define ADMA_AWLEN               0x00000814
#define ADMA_BRESP               0x00000818
#define ADMA_RAXI_ERR_ADDR       0x0000081c
// ------- RDMA --------           00000
#define RDMA_SETTINGS            0x00000900
#define RDMA_STATUS              0x00000904
#define RDMA_SYS_ADDR            0x00000908
#define RDMA_TBL_SIZE            0x0000090c
#define RDMA_ADDR                0x00000910
#define RDMA_LEN                 0x00000914
#define RDMA_DESC_ADDR           0x00000918
// ------- WDMA --------           00000
#define WDMA_SETTINGS            0x00000a00
#define WDMA_STATUS              0x00000a04
#define WDMA_SYS_ADDR            0x00000a08
#define WDMA_TBL_SIZE            0x00000a0c
#define WDMA_ADDR                0x00000a10
#define WDMA_LEN                 0x00000a14
#define WDMA_DESC_ADDR           0x00000a18

// Statuses
#define DISCONNECT_ERR          0x00000001
#define PARITY_ERR              0x00000002
#define ESC_ERR                 0x00000004
#define TIME_CODE               0x00000020
#define TIME_CODE_ERR           0x00000040
#define TX_CREDIT_ERR           0x00000080
#define RX_CREDIT_ERR           0x00000100
#define ACTIVE_LINK             0x00000200

#define STATE_IS_RUN            0x30000000

// Settings
#define EN_HSCB                 0x00000001
#define TIME_MODE               0x00000008
#define RESERVE_PATH_EN         0x00000010
#define LOOP_BACK               0x00000020
#define EN_RMAP                 0x00000040
#define EN_TRIM_CLK             0x00000080
#define RX_FIX_EN               0x00000100
#define DISCARD_RMAP            0x00000200

#define DMA_DESC_INT            0x00000001
#define DMA_DESC_END            0x00000002
#define DMA_BAD_DESC            0x00000004
#define DMA_AXI_ERROR           0x00000020
#define DMA_BUF_FULL            0x00000040
#define DMA_BUF_EMPTY           0x00000080
#define EN_DMA                  0x70000000
#define CANCEL_DMA              0x80000000


#define RX                      0
#define TX                      1
#define NO_INIT                 0
#define INIT                    1


//int64_t rumboot_virt_to_phys(volatile void *addr);

struct hscb_trans_tbl {
    uint32_t base_addr;
    uint32_t tbl_size;
    uint32_t tbl_addr;
    uint32_t prep_desc;
    uint32_t proc_desc;
};
// ----------------- change_endian -----------------
uint32_t change_endian (uint32_t data_in){
    uint32_t data_out = 0;

    data_out = data_in << 24 & 0xff000000;
    data_out = data_out | (data_in << 8  & 0x00ff0000);
    data_out = data_out | (data_in >> 8  & 0x0000ff00);
    data_out = data_out | (data_in >> 24 & 0x000000ff);

    return data_out;
}
// ----------------- get_addr -----------------
uint32_t get_addr(int heap_id, uint32_t data_size){
    uint32_t alloc_addr = 0;
    int64_t virt_addr = 0;
    uint32_t res_addr = 0;
    
//    rumboot_printf("get_addr function\n");
    alloc_addr = (uint32_t) rumboot_malloc_from_heap_aligned(heap_id, data_size, 8);
//    rumboot_printf("rumboot_malloc_from_heap_aligned success!\n");
    virt_addr = rumboot_virt_to_phys((void*) alloc_addr);
//    rumboot_printf("rumboot_virt_to_phys success!\n");
    virt_addr = virt_addr & 0x00000000ffffffff;
    res_addr = (uint32_t) virt_addr;
//    rumboot_printf("Address = %x\n", res_addr);
    return res_addr;
    
}
// ----------------- hscb_create_trans_tbl -----------------
void hscb_create_desc_tbl (struct hscb_trans_tbl * hscb, char* desc_heap, bool type, uint32_t desc_num) {
    int id = 0;
    
//    rumboot_printf("\nhscb_create_trans_tbl for %x\n", hscb->base_addr);
    
    hscb->tbl_size = (desc_num+1)*8;
    id = rumboot_malloc_heap_by_name(desc_heap);
    hscb->tbl_addr = get_addr(id, hscb->tbl_size);
    

    if (type) { // TX
        iowrite32(hscb->tbl_addr, 0xc0300a08);
        iowrite32(hscb->tbl_size, hscb->base_addr + RDMA_TBL_SIZE );
        iowrite32(hscb->tbl_addr, hscb->base_addr + RDMA_DESC_ADDR);
    }
    else {      // RX
        iowrite32(hscb->tbl_addr, hscb->base_addr + WDMA_SYS_ADDR );
        iowrite32(hscb->tbl_size, hscb->base_addr + WDMA_TBL_SIZE );
        iowrite32(hscb->tbl_addr, hscb->base_addr + WDMA_DESC_ADDR);
    }
    hscb->prep_desc = hscb->tbl_addr;
    hscb->proc_desc = hscb->tbl_addr;
//    rumboot_printf("hscb->prep_desc %x\n", hscb->prep_desc);
//    rumboot_printf("hscb->proc_desc %x\n", hscb->proc_desc);
}
// ----------------- hscb_create_desc -----------------
void hscb_create_desc (struct hscb_trans_tbl* hscb, uint32_t desc_size, uint32_t desc_addr) {

//    rumboot_printf("Create descriptor with address %x for HSCB %x\n", desc_addr, hscb->base_addr);
    
    iowrite32(change_endian(desc_size<<6 | TRAN_FLAG | LAST_FLAG), hscb->prep_desc);
//    rumboot_printf("Desc_params: %x\n", ioread32(hscb->prep_desc));
    hscb->prep_desc = hscb->prep_desc + 4;
    iowrite32(change_endian(desc_addr), hscb->prep_desc);
//    rumboot_printf("Desc_address: %x\n", ioread32(hscb->prep_desc));
    hscb->prep_desc = hscb->prep_desc + 4;
    iowrite32(0xffffffff, hscb->prep_desc)  ; // to stop
    iowrite32(0xffffffff, hscb->prep_desc+4); // to stop
//    rumboot_printf("End of hscb_create_desc\n");
}

void hscb_reuse_desc(
    struct hscb_trans_tbl* hscb,
    uint32_t desc_addr,
    uint32_t data_addr,
    uint32_t desc_size,
    bool type,
    bool init
) {
    int i = 0;
    rumboot_printf("Reuse descriptor at addr %x with addr %x and len %x\n", desc_addr, data_addr, desc_size);
    hscb->prep_desc = desc_addr;
    // Set address
    if (type) iowrite32(hscb->prep_desc, hscb->base_addr + RDMA_DESC_ADDR);
    else      iowrite32(hscb->prep_desc, hscb->base_addr + WDMA_DESC_ADDR);

    hscb_create_desc(hscb, desc_size, data_addr);
    // Init memory for RX
    if (init) {
        for (i=0; i<desc_size; i+=4){
            iowrite32(0, data_addr);
            data_addr +=4;
        }
    }
    // Enable
    if (type) iowrite32(EN_DMA, hscb->base_addr + RDMA_SETTINGS); // TX
    else      iowrite32(EN_DMA, hscb->base_addr + WDMA_SETTINGS); // RX
}
// ----------------- hscb_get_len -----------------
uint32_t hscb_get_len (struct hscb_trans_tbl* hscb) {
    uint32_t len = change_endian(ioread32(hscb->proc_desc)) >> 6;
//    rumboot_printf("Descriptor %x_%x has length %d\n", change_endian(ioread32(hscb->proc_desc)), change_endian(ioread32(hscb->proc_desc+4)), len);
//    hscb->proc_desc = hscb->proc_desc+8;
    return len;
}
// ----------------- hscb_get_addr -----------------
uint32_t hscb_get_addr (struct hscb_trans_tbl* hscb) {
    uint32_t addr = change_endian(ioread32(hscb->proc_desc+4));
//    rumboot_printf("Descriptor %x_%x has address %x\n", change_endian(ioread32(hscb->proc_desc)), change_endian(ioread32(hscb->proc_desc+4)), addr);
    return addr;
}

// ----------------- hscb_data_print -----------------
void hscb_data_print(struct hscb_trans_tbl* hscb) {
    int i = 0;

    rumboot_printf("Print from descriptor %x_%x\n", ioread32(hscb->proc_desc), ioread32(hscb->proc_desc+4));
    uint32_t len = hscb_get_len(hscb);
    uint32_t addr = hscb_get_addr(hscb);
    
    for (i=0; i<len; i+=4){
        rumboot_printf("%x: %x\n", addr, ioread32(addr));
        addr += 4;
    }
}

// ----------------- hscb_wait_end_trans -----------------
uint32_t hscb_wait_end_trans (struct hscb_trans_tbl* hscb) {
    uint32_t status = 0;
    int cnt = 0;

//    rumboot_printf("\nWait finish...\n");

    while (!status){
        if (cnt == WAIT_LIMIT) {
            status = ioread32(hscb->base_addr + WDMA_STATUS);
//            rumboot_printf("\nHSCB status %x\n", ioread32(hscb->base_addr + STATUS));
//            rumboot_printf("WDMA status %x\n", status);
            status = status & (DMA_DESC_INT | DMA_DESC_END | DMA_BAD_DESC);
        }
        else
            cnt++;
    }
   
    return 0;
}

// ----------------- hscb_data_check -----------------
int hscb_data_check(struct hscb_trans_tbl* hscb, uint32_t init_val, uint32_t inc_val) {
    
    int i = 0;
    uint32_t tmp = 0;
    uint32_t addr = 0;
    uint32_t len = 0;
    
    addr = hscb_get_addr(hscb);
    len = hscb_get_len(hscb);
   
//    rumboot_printf("Check from descriptor at address %x\n   address: %x data length: %d\n", hscb->proc_desc, addr, len);
    for (i=0; i<len; i+=4){
        tmp = ioread32(addr);
//        rumboot_printf("%x: %x, must be %x\n", addr, tmp, init_val);
        if (init_val != tmp){
            rumboot_printf("Check fail\n \t %x: %x, must be %x!\n", addr, tmp, init_val);
            return 1;
        }
        init_val = init_val + inc_val;
        addr += 4;
    }
//    rumboot_printf("Data check success!\n");
    return 0;
}

void hscb_gen_data (uint32_t addr, uint32_t data_size, uint32_t init_val, uint32_t inc_val){
    int i = 0;
    uint32_t data = init_val;
    
//    rumboot_printf("\n Generate data for test\n");
    for (i=0; i<data_size; i+=4){
        iowrite32(data, addr);
//        rumboot_printf("%x: %x\n", addr, ioread32(addr));
        data = data + inc_val;
        addr = addr + 4;
    }
}

uint32_t hscb_board_loop_test (
    uint32_t base_addr,
    char* desc_heap,
    char* data_heap
) {
    int i = 0;
#ifndef ON_BOARD    
    uint32_t page_size = 64;
#else    
    uint32_t page_size = 8192;
#endif

    uint32_t rx_addr = 0;
    uint32_t rx_len = 0;

    struct hscb_trans_tbl hscb_rx_tbl;
    struct hscb_trans_tbl* hscb_rx;
    hscb_rx = &hscb_rx_tbl;

    struct hscb_trans_tbl hscb_tx_tbl;
    struct hscb_trans_tbl* hscb_tx;
    hscb_tx = &hscb_tx_tbl;

    hscb_rx->base_addr = base_addr;
    hscb_tx->base_addr = base_addr;
    
    hscb_create_desc_tbl(hscb_rx, desc_heap, RX, 1);
    hscb_create_desc_tbl(hscb_tx, desc_heap, TX, 1);
    
    // Prepare descriptors
    hscb_create_desc(hscb_rx, page_size, (uint32_t) get_addr(rumboot_malloc_heap_by_name(data_heap), page_size));
    hscb_create_desc(hscb_tx, page_size, (uint32_t) get_addr(rumboot_malloc_heap_by_name(data_heap), page_size));
    
#ifndef ON_BOARD
    struct hscb_trans_tbl hscb_tx_test_tbl;
    struct hscb_trans_tbl* hscb_tx_test;
    hscb_tx_test = &hscb_tx_test_tbl;
    
    hscb_tx_test->base_addr = HSCB1_BASE;
    hscb_create_desc_tbl(hscb_tx_test, desc_heap, TX, 1);
    uint32_t test_addr = (uint32_t)get_addr(rumboot_malloc_heap_by_name(data_heap), 32);
    hscb_gen_data(test_addr, 32, 0x11121314, 0x01010101);
    hscb_create_desc(hscb_tx_test, 32, test_addr);
#endif    
    //start for RX
//    iowrite32((EN_HSCB | DISCARD_RMAP| EN_TRIM_CLK | RESERVE_PATH_EN | RX_FIX_EN), base_addr + SETTINGS);
//    iowrite32((EN_HSCB | DISCARD_RMAP| EN_TRIM_CLK |                   RX_FIX_EN), base_addr + SETTINGS);
//    iowrite32((EN_HSCB | DISCARD_RMAP| EN_TRIM_CLK | RESERVE_PATH_EN            ), base_addr + SETTINGS);
    iowrite32((EN_HSCB | DISCARD_RMAP| EN_TRIM_CLK), base_addr + SETTINGS);
    iowrite32(EN_DMA, hscb_rx->base_addr + WDMA_SETTINGS);
    
#ifndef ON_BOARD    
    while (i<2){
#else
    while (1){
#endif
        rumboot_printf("Cycle %d\n", i);
#ifndef ON_BOARD
        iowrite32(EN_DMA, hscb_tx_test->base_addr + RDMA_SETTINGS);
#endif
        if (hscb_wait_end_trans (hscb_rx)) return 1;
        rx_addr = hscb_get_addr(hscb_rx);
        rx_len = hscb_get_len(hscb_rx);
        hscb_reuse_desc(hscb_tx, hscb_rx->prep_desc-8, rx_addr, rx_len, TX, NO_INIT);
        iowrite32(EN_DMA, hscb_tx->base_addr + RDMA_SETTINGS);
        hscb_data_print(hscb_rx);
        hscb_reuse_desc(hscb_rx, hscb_rx->prep_desc-8, rx_addr, page_size, RX, INIT);
        iowrite32(EN_DMA, hscb_rx->base_addr + WDMA_SETTINGS);
        i++;
#ifndef ON_BOARD
        hscb_reuse_desc(hscb_tx_test, hscb_tx_test->prep_desc-8, test_addr, 32, TX, NO_INIT);
#endif
    }
    return 0;
}

int hscb_loop_iteration (
    uint32_t src_base_addr,
    uint32_t dst_base_addr,
    char* desc_heap,
    char* data_heap,
    uint32_t page_size,
    uint32_t hscb_src_speed
) {
    uint32_t src_tx_data_addr;
    uint32_t dst_tx_data_addr;
    
    uint32_t src_init_val = 0x10111213;
    uint32_t dst_init_val = 0x11121314;
    uint32_t inc_val = 0x01010101;
    
    struct hscb_trans_tbl src_rx_tbl;
    struct hscb_trans_tbl* src_rx;
    src_rx = &src_rx_tbl;
    struct hscb_trans_tbl src_tx_tbl;
    struct hscb_trans_tbl* src_tx;
    src_tx = &src_tx_tbl;

    struct hscb_trans_tbl dst_rx_tbl;
    struct hscb_trans_tbl* dst_rx;
    dst_rx = &dst_rx_tbl;
    struct hscb_trans_tbl dst_tx_tbl;
    struct hscb_trans_tbl* dst_tx;
    dst_tx = &dst_tx_tbl;

    src_rx->base_addr = src_base_addr;
    src_tx->base_addr = src_base_addr;
    dst_rx->base_addr = dst_base_addr;
    dst_tx->base_addr = dst_base_addr;
    
    hscb_create_desc_tbl(src_rx, desc_heap, RX, 1);
    hscb_create_desc_tbl(src_tx, desc_heap, TX, 1);
    hscb_create_desc_tbl(dst_rx, desc_heap, RX, 1);
    hscb_create_desc_tbl(dst_tx, desc_heap, TX, 1);

    hscb_create_desc(src_rx, page_size, (uint32_t) get_addr(rumboot_malloc_heap_by_name(data_heap), page_size));
    src_tx_data_addr = (uint32_t) get_addr(rumboot_malloc_heap_by_name(data_heap), page_size);
    hscb_create_desc(src_tx, page_size, src_tx_data_addr);
    hscb_gen_data(src_tx_data_addr, page_size, src_init_val, inc_val);

    hscb_create_desc(dst_rx, page_size, (uint32_t) get_addr(rumboot_malloc_heap_by_name(data_heap), page_size));
    dst_tx_data_addr = (uint32_t) get_addr(rumboot_malloc_heap_by_name(data_heap), page_size);
    hscb_create_desc(dst_tx, page_size, dst_tx_data_addr);
    hscb_gen_data(dst_tx_data_addr, page_size, dst_init_val, inc_val);
    
    // Start
//    rumboot_printf("Start!\n");
    iowrite32(hscb_src_speed, src_base_addr + TRANS_CLK );
    
    iowrite32(EN_DMA, src_rx->base_addr + WDMA_SETTINGS);
    iowrite32(EN_DMA, dst_rx->base_addr + WDMA_SETTINGS);
    iowrite32(EN_DMA, src_tx->base_addr + RDMA_SETTINGS);
    iowrite32(EN_DMA, dst_tx->base_addr + RDMA_SETTINGS);
    
    // Wait finish transfer
//    rumboot_printf("Wait finish...\n");
    if (hscb_wait_end_trans (src_rx)) return 1;
    if (hscb_wait_end_trans (dst_rx)) return 1;
    
    // Check link state
    uint32_t hscb_status;
    hscb_status = ioread32(src_rx->base_addr + STATUS);
    if (hscb_status & (DISCONNECT_ERR | PARITY_ERR | ESC_ERR | TIME_CODE_ERR | TX_CREDIT_ERR | RX_CREDIT_ERR)){
        rumboot_printf("\n Error in HSCB src (addr %x) status!\n", src_rx->base_addr);
        rumboot_printf("    status = %x\n", hscb_status);
        return 1;
    }

    hscb_status = ioread32(dst_rx->base_addr + STATUS);
    if (hscb_status & (DISCONNECT_ERR | PARITY_ERR | ESC_ERR | TIME_CODE_ERR | TX_CREDIT_ERR | RX_CREDIT_ERR)){
        rumboot_printf("\n Error in HSCB dst (addr %x) status!\n", src_rx->base_addr);
        rumboot_printf("    status = %x\n", hscb_status);
        return 1;
    }
    
    //Check results
    if (hscb_data_check (src_rx, dst_init_val, inc_val)) return 1;
    if (hscb_data_check (dst_rx, src_init_val, inc_val)) return 1;
    
    return 0;
}


int hscb_internal_loop_iteration (
    uint32_t base_addr,
    char* desc_heap,
    char* data_heap,
    uint32_t page_size,
    uint32_t hscb_speed
) {
    uint32_t tx_data_addr;
    
    uint32_t init_val = 0x10111213;
    uint32_t inc_val = 0x01010101;
    
    struct hscb_trans_tbl rx_tbl;
    struct hscb_trans_tbl* rx;
    rx = &rx_tbl;
    struct hscb_trans_tbl tx_tbl;
    struct hscb_trans_tbl* tx;
    tx = &tx_tbl;

    rx->base_addr = base_addr;
    tx->base_addr = base_addr;
    
    hscb_create_desc_tbl(rx, desc_heap, RX, 1);
    hscb_create_desc_tbl(tx, desc_heap, TX, 1);

    hscb_create_desc(rx, page_size, (uint32_t) get_addr(rumboot_malloc_heap_by_name(data_heap), page_size));
    tx_data_addr = (uint32_t) get_addr(rumboot_malloc_heap_by_name(data_heap), page_size);
    hscb_create_desc(tx, page_size, tx_data_addr);
    hscb_gen_data(tx_data_addr, page_size, init_val, inc_val);

    
    // Start
    rumboot_printf("Start!\n");
    iowrite32(hscb_speed, base_addr + TRANS_CLK );
    
    iowrite32(EN_DMA, rx->base_addr + WDMA_SETTINGS);
    iowrite32(EN_DMA, tx->base_addr + RDMA_SETTINGS);
    
    // Wait finish transfer
    rumboot_printf("Wait finish...\n");
    if (hscb_wait_end_trans (rx)) return 1;
    
    //Check results
    if (hscb_data_check (rx, init_val, inc_val)) return 1;
    
    return 0;
}

int main() {
    rumboot_printf("\nStart of HSCB test\n");
    
#ifdef BOARD_TEST
    return hscb_board_loop_test(HSCB_BASE, "IM2", "IM2");
#endif

#ifdef LOOP_TEST
    int i = 0;
    uint32_t result = 0;
//    uint32_t speed[] = {20, 10, 1, 2, 3};
//    uint32_t speed[] = {1,0,1,0,1,0};
    uint32_t speed = 10;
    uint32_t page_size = 128;
    uint32_t hscb_status = 0;
    uint32_t cnt = 0;

    rumboot_printf("Reset HSCBs\n");
    iowrite32(1, HSCB_SRC_BASE + SW_RESET );
    iowrite32(1, HSCB_SRC_BASE + ADMA_SW_RST );
    
    iowrite32(1, HSCB_DST_BASE + SW_RESET );
    iowrite32(1, HSCB_DST_BASE + ADMA_SW_RST );

    rumboot_printf("Enable HSCBs\n");
    iowrite32((EN_HSCB | DISCARD_RMAP)             , HSCB_SRC_BASE + SETTINGS );
    iowrite32((EN_HSCB | DISCARD_RMAP| EN_TRIM_CLK), HSCB_DST_BASE + SETTINGS );
    
    // Wait link enable state
    rumboot_printf("Wait link\n");
    while(((hscb_status & ACTIVE_LINK) == 0) && (cnt <= MAX_ATTEMPT)){
        hscb_status = ioread32(HSCB_SRC_BASE + STATUS);
//        rumboot_printf("hscb_status = %x\n", hscb_status);
//        rumboot_printf("hscb_status & ACTIVE_LINK = %x\n", (hscb_status & ACTIVE_LINK));
        cnt++;
    }
    if (cnt >= MAX_ATTEMPT){
        rumboot_printf("\n\n HSCB test ERROR!\n    Can't link!!!\n");
        return 1;
    }
    rumboot_printf("Link enabled success!!!\n");
    // Clean HSCB_DST address
    hscb_status = ioread32(HSCB_DST_BASE + STATUS);
    
    // Loop test
    while (!result & (i < 51)){
//        rumboot_printf("Iteration number %d\n",i);
//        rumboot_printf("Speed = %x\n",speed);
        
        
        result = hscb_loop_iteration(HSCB_SRC_BASE, HSCB_DST_BASE, "IM1", "IM1", page_size, speed);
        if (result) { 
            rumboot_printf("\nIteration number %d\n",i);
            rumboot_printf("Speed = %x\n",speed);
            return 1;
        }
        i++;
        if (page_size == 128)
            page_size = 16;
        else
            page_size += 8;        
        if (speed == 15)
            speed = 2;
        else
            speed++;
    }
    rumboot_printf("\nTest OK!\n\n");
    return 0;
#endif

#ifdef INTERNAL_LOOP_TEST
    uint32_t internal_speed = 1;
    uint32_t internal_page_size = 128;

    rumboot_printf("Reset HSCBs\n");
    
    iowrite32(1, HSCB_BASE + SW_RESET );
    iowrite32(1, HSCB_BASE + ADMA_SW_RST );
    
    rumboot_printf("Enable HSCBs\n");
    iowrite32((EN_HSCB | DISCARD_RMAP | LOOP_BACK), HSCB_BASE + SETTINGS );
    
    return hscb_internal_loop_iteration(HSCB_BASE, "IM2", "IM2", internal_page_size, internal_speed);
#endif

#ifdef INTERNAL_LOOP_TEST_ALL
    uint32_t internal_speed = 1;
    uint32_t internal_page_size = 128;
    uint32_t result = 0;

    uint32_t wait_gpio_1_6  = 0;
    uint32_t value_gpio_1_7 = 0;
    uint32_t past_state_gpio_1_7 = 0;
    
    uint32_t hscb_base;
    
    uint32_t i;
    
    value_gpio_1_7 =  ioread32(GPIO_1_BASE+0x3fc) & 0x80;
    rumboot_printf("value_gpio_1_7 = 0x%x \n",value_gpio_1_7);
    past_state_gpio_1_7 = value_gpio_1_7;
    rumboot_printf("wait GPIO1[6] = 1 \n");
    rumboot_printf("gpio_1_6 = 0x%x \n",wait_gpio_1_6);
    while (wait_gpio_1_6 == 0) {
        wait_gpio_1_6 = ioread32(GPIO_1_BASE+0x3fc) & 0x40;
    }
    rumboot_printf("gpio_1_6 = 0x%x \n\n",wait_gpio_1_6);
    for (i=0; i<4; i++){
        switch (i)
        {
            case 0: 
                rumboot_printf("\nHSCB0\n\n");
                hscb_base = HSCB0_BASE;
                break;
    
            case 1:
                rumboot_printf("\nHSCB1\n\n");
                hscb_base = HSCB1_BASE;
                break;
    
            case 2:
                rumboot_printf("\nHSCB2\n\n");
                hscb_base = HSCB2_BASE;
                break;
    
            case 3:
                rumboot_printf("\nHSCB3\n\n");
                hscb_base = HSCB3_BASE;
                break;
        }
        rumboot_printf("Reset HSCB\n");
        iowrite32(1, hscb_base + SW_RESET );
        iowrite32(1, hscb_base + ADMA_SW_RST );
        
        rumboot_printf("Enable HSCB\n");
        iowrite32((EN_HSCB | DISCARD_RMAP | LOOP_BACK), hscb_base + SETTINGS );
    
        result = hscb_internal_loop_iteration(hscb_base, "IM2", "IM2", internal_page_size, internal_speed);
        if (result==1) {
            break;
        }   
        rumboot_printf("Finish HSCB\n");
        rumboot_printf("value_gpio_1_7 = 0x%x \n",value_gpio_1_7);
        rumboot_printf("wait change gpio_1_7 \n"); 
        while (value_gpio_1_7 == past_state_gpio_1_7) { 
            value_gpio_1_7 =  ioread32(GPIO_1_BASE+0x3fc) & 0x80;
        }
        rumboot_printf("value_gpio_1_7 = 0x%x \n\n",value_gpio_1_7);
        past_state_gpio_1_7 = value_gpio_1_7;
    }
    return result;
#endif
}
