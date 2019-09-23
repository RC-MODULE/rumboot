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


#define TRAN_FLAG       0x00000020
#define LAST_FLAG       0x00000009
    
#define INT_STAT    0x4
#define BAD_STAT    0x8
#define STOP_STAT   0x10

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
#define TX_ENDIAN               0x00000002
#define RX_ENDIAN               0x00000004
#define TIME_MODE               0x00000008
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

struct hscb_trans_tbl {
//    int      heap_id;
    uint32_t base_addr;
    uint32_t r_tbl_addr;
    uint32_t src_addr;
    uint32_t w_tbl_addr;
    uint32_t dst_addr;
};

uint32_t change_endian (uint32_t data_in){
    uint32_t data_out = 0;

//    if(change_endian)
//    {
        data_out = data_in << 24 & 0xff000000;
        data_out = data_out | (data_in << 8  & 0x00ff0000);
        data_out = data_out | (data_in >> 8  & 0x0000ff00);
        data_out = data_out | (data_in >> 24 & 0x000000ff);
//    }
//    else
//        data_out = data_in;
    return data_out;
}

/*
void hscb_create_trans_tbl (struct hscb_trans_tbl * hscb, char* desc_heap, char* data_heap, uint32_t desc_num) {
    int id = 0;
    
    rumboot_printf("hscb_create_trans_tbl for %x\n", hscb->base_addr);
    
//    id = rumboot_malloc_heap_by_name(desc_heap);
//    hscb->r_tbl_addr = (uint32_t) rumboot_malloc_from_heap_aligned(id, desc_num*8, 8);
//    hscb->w_tbl_addr = (uint32_t) rumboot_malloc_from_heap_aligned(id, desc_num*8, 8);
    hscb->r_tbl_addr = 0xC0010000;
    hscb->w_tbl_addr = 0xC0010100;
    
    hscb->heap_id = rumboot_malloc_heap_by_name(data_heap);
    
    iowrite32(hscb->r_tbl_addr, hscb->base_addr + RDMA_SYS_ADDR );    // Set read channel first descriptor address
    iowrite32((desc_num+1)*8  , hscb->base_addr + RDMA_TBL_SIZE );    // Set read channel first descriptor address
    iowrite32(hscb->r_tbl_addr, hscb->base_addr + RDMA_DESC_ADDR);    // Set read channel first descriptor address
    
    iowrite32(hscb->w_tbl_addr, hscb->base_addr + WDMA_SYS_ADDR );    // Set read channel first descriptor address
    iowrite32((desc_num+1)*8  , hscb->base_addr + WDMA_TBL_SIZE );    // Set read channel first descriptor address
    iowrite32(hscb->w_tbl_addr, hscb->base_addr + WDMA_DESC_ADDR);    // Set read channel first descriptor address
}
*/

void hscb_create_trans_tbl (struct hscb_trans_tbl * hscb, uint32_t r_addr, uint32_t w_addr, uint32_t desc_num) {
    
    rumboot_printf("hscb_create_trans_tbl for %x\n", hscb->base_addr);
    
    hscb->r_tbl_addr = r_addr;
    hscb->w_tbl_addr = w_addr;
    
    iowrite32(hscb->r_tbl_addr, hscb->base_addr + RDMA_SYS_ADDR );    // Set read channel first descriptor address
    iowrite32((desc_num+1)*8  , hscb->base_addr + RDMA_TBL_SIZE );    // Set read channel first descriptor address
    iowrite32(hscb->r_tbl_addr, hscb->base_addr + RDMA_DESC_ADDR);    // Set read channel first descriptor address
    
    iowrite32(hscb->w_tbl_addr, hscb->base_addr + WDMA_SYS_ADDR );    // Set read channel first descriptor address
    iowrite32((desc_num+1)*8  , hscb->base_addr + WDMA_TBL_SIZE );    // Set read channel first descriptor address
    iowrite32(hscb->w_tbl_addr, hscb->base_addr + WDMA_DESC_ADDR);    // Set read channel first descriptor address
}

void hscb_create_trans_desc (struct hscb_trans_tbl* hscb, uint32_t len, uint32_t init_val) {

    rumboot_printf("hscb_create_trans_desc for %x\n", hscb->base_addr);
    // Read channel
//    rumboot_printf("Read channel  ");
    iowrite32(change_endian(len<<6 | TRAN_FLAG | LAST_FLAG), hscb->r_tbl_addr);
    hscb->r_tbl_addr = hscb->r_tbl_addr + 4;
    iowrite32(change_endian(hscb->src_addr), hscb->r_tbl_addr);
    hscb->r_tbl_addr = hscb->r_tbl_addr + 4;
    iowrite32(0xffffffff, hscb->r_tbl_addr); // to stop
    iowrite32(0xffffffff, hscb->r_tbl_addr+4); // to stop
//    rumboot_printf("%x: ", hscb->src_addr);    
//    rumboot_printf("%x_", ioread32(hscb->r_tbl_addr-4));
//    rumboot_printf("%x.\n", ioread32(hscb->r_tbl_addr-8));
    if (init_val != 0){
//        rumboot_printf("Fill descriptor.\n");
        uint32_t cur_addr = hscb->src_addr;
        uint32_t data = init_val;
        while(cur_addr <= (hscb->src_addr + len)){            
            iowrite32(data, cur_addr);
//            rumboot_printf("%x: %x\n", cur_addr, ioread32(cur_addr));
            data = data + 0x04040404;
            cur_addr +=4;
        }
    }
    
    // Write channel
//    rumboot_printf("Write channel ");
    iowrite32(change_endian(len<<6 | TRAN_FLAG | LAST_FLAG), hscb->w_tbl_addr);
    hscb->w_tbl_addr = hscb->w_tbl_addr + 4;
    iowrite32(change_endian(hscb->dst_addr), hscb->w_tbl_addr);
    hscb->w_tbl_addr = hscb->w_tbl_addr + 4;
    iowrite32(0xffffffff, hscb->w_tbl_addr); // to stop
    iowrite32(0xffffffff, hscb->w_tbl_addr+4); // to stop
//    rumboot_printf("%x: ", hscb->dst_addr);    
//    rumboot_printf("%x_", ioread32(hscb->w_tbl_addr-4));
//    rumboot_printf("%x.\n", ioread32(hscb->w_tbl_addr-8));
}


uint32_t hscb_wait_end_trans (struct hscb_trans_tbl* hscb) {
    uint32_t status = 0;

    while (!status){
        status = ioread32(hscb->base_addr + WDMA_STATUS) & (DMA_DESC_INT | DMA_DESC_END | DMA_BAD_DESC);
    }
    rumboot_printf("WDMA status %x\n", status);
    
    return 0;
}


int hscb_data_check(struct hscb_trans_tbl* hscb, uint32_t len, uint32_t init_val) {
    
    int i = 0;
    uint32_t tmp = 0;
    
    rumboot_printf("Check from address %x\n", hscb->dst_addr);
    for (i=0; i<len; i+=4){
        tmp = ioread32(hscb->dst_addr);
        rumboot_printf("%x: %x, must be %x\n", hscb->dst_addr, tmp, init_val);
        if (init_val != tmp){
            rumboot_printf("Check fail\n \t %x: %x, must be %x!\n", hscb->dst_addr, tmp, init_val);
            return 1;
        }
        init_val = init_val + 0x04040404;
        hscb->dst_addr += 4;
    }
    return 0;
}

int hscb_loop_test (
//        char* test_heap,
//        char* desc_heap,
        uint32_t hscb_src_addr,
        uint32_t hscb_dst_addr,
        uint32_t data_size,
        uint32_t desc_num,
        uint32_t hscb_src_speed,
        uint32_t hscb_src_init
    ) {

//    uint32_t hscb_src_init = 0x00010203;
    uint32_t hscb_dst_init = hscb_src_init + 0x01010101;
    
    struct hscb_trans_tbl hscb_src_tbl;
    struct hscb_trans_tbl hscb_dst_tbl;

    struct hscb_trans_tbl* hscb_src;
    struct hscb_trans_tbl* hscb_dst;
    
    hscb_src = &hscb_src_tbl;
    hscb_dst = &hscb_dst_tbl;
    
    if (hscb_src == NULL) {
        rumboot_printf("hscb_src == NULL\n");
        return 1;
    }
    if (hscb_dst == NULL) {
        rumboot_printf("hscb_dst == NULL\n");
        return 1;
    }
    
    
    
    hscb_src->base_addr = hscb_src_addr;
    hscb_dst->base_addr = hscb_dst_addr;

    rumboot_printf("Reset HSCBs\n");
    iowrite32(1, hscb_src->base_addr + SW_RESET );
    iowrite32(1, hscb_src->base_addr + ADMA_SW_RST );
    
    iowrite32(1, hscb_dst->base_addr + SW_RESET );
    iowrite32(1, hscb_dst->base_addr + ADMA_SW_RST );
      
    rumboot_printf("Enable HSCBs\n");
    iowrite32((EN_HSCB | DISCARD_RMAP)             , hscb_src->base_addr + SETTINGS );
    iowrite32((EN_HSCB | DISCARD_RMAP| EN_TRIM_CLK), hscb_dst->base_addr + SETTINGS );

    rumboot_printf("hscb_src\n");
//    hscb_create_trans_tbl(hscb_src, desc_heap, test_heap, desc_num);
//    hscb_src->src_addr = (uint32_t) rumboot_malloc_from_heap_aligned(hscb_src->heap_id, data_size, 8);
//    hscb_src->dst_addr = (uint32_t) rumboot_malloc_from_heap_aligned(hscb_src->heap_id, data_size, 8);
    hscb_create_trans_tbl(hscb_src, 0xC0010000, 0xC0010100, desc_num);
    hscb_src->src_addr = 0xC0011000;
    hscb_src->dst_addr = 0xC0011400;
    hscb_create_trans_desc(hscb_src, data_size, hscb_src_init);

    rumboot_printf("hscb_dst\n");
//    hscb_create_trans_tbl(hscb_dst, desc_heap, test_heap, desc_num);
//    hscb_dst->src_addr = (uint32_t) rumboot_malloc_from_heap_aligned(hscb_dst->heap_id, data_size, 8);
//    hscb_dst->dst_addr = (uint32_t) rumboot_malloc_from_heap_aligned(hscb_dst->heap_id, data_size, 8);
    hscb_create_trans_tbl(hscb_dst, 0xC0010200, 0xC0010300, desc_num);
    hscb_dst->src_addr = 0xC0011800;
    hscb_dst->dst_addr = 0xC0011C00;
    hscb_create_trans_desc(hscb_dst, data_size, hscb_dst_init);

    rumboot_printf("Set speed hscb_src = %x\n", hscb_src_speed);
    iowrite32(hscb_src_speed, hscb_src->base_addr + TRANS_CLK );
    
    rumboot_printf("Start!\n");
    iowrite32(EN_DMA, hscb_src->base_addr + WDMA_SETTINGS);
    iowrite32(EN_DMA, hscb_dst->base_addr + WDMA_SETTINGS);
    iowrite32(EN_DMA, hscb_src->base_addr + RDMA_SETTINGS);
    iowrite32(EN_DMA, hscb_dst->base_addr + RDMA_SETTINGS);

    rumboot_printf("Wait finish!\n");
    hscb_wait_end_trans (hscb_src);
    hscb_wait_end_trans (hscb_dst);
    
    rumboot_printf("Check src to dst result!\n");
    if (hscb_data_check(hscb_dst, data_size, hscb_src_init))
        return 1;
    rumboot_printf("Check dst to src result!\n");
    if (hscb_data_check(hscb_src, data_size, hscb_dst_init))
        return 1;
    
    return 0;
}

int main() {
    uint32_t desc_num = 1; // max 511
    uint32_t data_size = 16;
    uint32_t speed = 0x14;
    uint32_t hscb_src_init = 0x00010203;
    int i = 0;
    int tmp = 0;
    
    rumboot_printf("Start test with %d cycles\n", desc_num);

    while (!tmp & (i < 3)){
        rumboot_printf("\n\nIteration %d\n", i);
        rumboot_printf("Set speed hscb_src = %x\n", speed);
        
        tmp = hscb_loop_test(HSCB_SRC_BASE, HSCB_DST_BASE, data_size, desc_num, speed, hscb_src_init);
        if (tmp)
            return 1;

        i++;
        hscb_src_init = hscb_src_init + 0x10101010;
        if (speed == 32)
            speed = 1;
        else
            speed++;
        if (desc_num == 128)
            desc_num = 16;
        else
            desc_num += 1;
    }


//    if (hscb_loop_test("IM1", "IM1", HSCB2_BASE, HSCB3_BASE, data_size, desc_num))
//        return 1;
    
    rumboot_printf("\n\nFinish work!\n");
    return 0;
}