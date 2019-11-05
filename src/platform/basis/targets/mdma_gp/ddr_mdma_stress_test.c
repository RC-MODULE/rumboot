#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/timer.h>
#include <rumboot/rumboot.h>
#include <rumboot/ddr_test_lib.h>

#include <regs/regs_mdma.h>

#include <platform/devices.h>


#define STOP_FLAG       0x10000000
#define INT_FLAG        0x20000000
    
#define INT_STAT    0x4
#define BAD_STAT    0x8
#define STOP_STAT   0x10
//#define EN_PRINT
#define MDMAx4

struct mdma_trans_tbl {
    int      r_heap_id;
    int      w_heap_id;
    uint32_t base_addr;
    uint32_t r_tbl_addr;
    uint32_t src_addr;
    uint32_t w_tbl_addr;
    uint32_t dst_addr;
};

void mdma_create_trans_tbl (struct mdma_trans_tbl * mdma, uint32_t desc_num, char* desc_heap, char* r_heap, char* w_heap) {
    int id = 0;
    
//    rumboot_printf("mdma_create_trans_tbl for %x\n", mdma->base_addr);
    
    id = rumboot_malloc_heap_by_name(desc_heap);

    mdma->r_tbl_addr = (uint32_t) rumboot_malloc_from_heap_aligned(id, (desc_num+1)*8, 8);
    mdma->w_tbl_addr = (uint32_t) rumboot_malloc_from_heap_aligned(id, (desc_num+1)*8, 8);
    
    mdma->r_heap_id = rumboot_malloc_heap_by_name(r_heap);
    mdma->w_heap_id = rumboot_malloc_heap_by_name(w_heap);
    
    iowrite32(mdma->r_tbl_addr, mdma->base_addr + MDMA_DESC_ADDR_R);    // Set read channel first descriptor address
    iowrite32(mdma->w_tbl_addr, mdma->base_addr + MDMA_DESC_ADDR_W);    // Set read channel first descriptor address
}

void mdma_create_trans_desc (struct mdma_trans_tbl* mdma, uint32_t len, uint32_t desc_num) {

#ifdef EN_PRINT    
    rumboot_printf("mdma_create_trans_desc for %x\n", mdma->base_addr);
#endif
    uint32_t i = 0;
    
    // Read channel
    for (i=0;i<desc_num-1; i++){
        iowrite32(mdma->src_addr, mdma->r_tbl_addr + i*8);
        iowrite32(len, mdma->r_tbl_addr + i*8 + 4);
    }
    iowrite32(mdma->src_addr, mdma->r_tbl_addr + i*8);
    iowrite32((len | STOP_FLAG), mdma->r_tbl_addr + i*8 + 4);
    iowrite32(0xffffffff, mdma->r_tbl_addr+ i*8 + 12); // to stop

    // Write channel
    for (i=0;i<desc_num-1; i++){
        iowrite32(mdma->dst_addr, mdma->w_tbl_addr + i*8);
        iowrite32(len, mdma->w_tbl_addr + i*8 + 4);
    }
    iowrite32(mdma->dst_addr, mdma->w_tbl_addr + i*8);
    iowrite32((len | STOP_FLAG), mdma->w_tbl_addr + i*8 + 4);
    iowrite32(0xffffffff, mdma->w_tbl_addr+ i*8 + 12); // to stop

    rumboot_printf("Create descriptors for %x\n", mdma->base_addr);
}

void mdma_reuse_trans_desc (struct mdma_trans_tbl* mdma, uint32_t len, uint32_t desc_num) {

    uint32_t i = 0;
    
#ifdef EN_PRINT
    rumboot_printf("mdma_reuse_trans_desc for %x\n", mdma->base_addr);
#endif
    // Read channel
    for (i=0;i<desc_num-1; i++){
        iowrite32(len, mdma->r_tbl_addr + i*8 + 4);
    }
    iowrite32((len | STOP_FLAG), mdma->r_tbl_addr + i*8 + 4);

    // Write channel
    for (i=0;i<desc_num-1; i++){
        iowrite32(len, mdma->w_tbl_addr + i*8 + 4);
    }
    iowrite32((len | STOP_FLAG), mdma->w_tbl_addr + i*8 + 4);

    iowrite32(mdma->r_tbl_addr, mdma->base_addr + MDMA_DESC_ADDR_R);    // Set read channel first descriptor address
    iowrite32(mdma->w_tbl_addr, mdma->base_addr + MDMA_DESC_ADDR_W);    // Set read channel first descriptor address
#ifdef EN_PRINT
    rumboot_printf("ReStart!\n");
#endif

}

static inline void mdma_start (struct mdma_trans_tbl* mdma){
    iowrite32(1, mdma->base_addr + MDMA_ENABLE_W);
    iowrite32(1, mdma->base_addr + MDMA_ENABLE_R);    
}

uint32_t mdma_wait_end_trans (struct mdma_trans_tbl* mdma) {
    uint32_t status = 0;
    uint32_t tmp = 0;

    while (!status){
        status = ioread32(mdma->base_addr + MDMA_STATUS_W) & (BAD_STAT | STOP_STAT);
    }
    if (status & BAD_STAT){
        tmp = ioread32(mdma->base_addr + MDMA_CUR_DESC_ADDR_W);
        rumboot_printf("Bad descriptor with address %x\n", tmp);
        return 1;
    }
    
    return 0;
}

uint32_t mdma_trans_restart (struct mdma_trans_tbl* mdma, uint32_t data_size, uint32_t desc_num) {

//    mdma->src_addr = mdma->dst_addr;
//    mdma->dst_addr = (uint32_t) rumboot_malloc_from_heap_aligned(mdma->heap_id, data_size, 8);
    
    mdma_reuse_trans_desc(mdma, data_size, desc_num);
    mdma_start(mdma);
    
    return 0;
}

int mdma_data_check(struct mdma_trans_tbl* mdma, uint32_t len, uint32_t init_val) {
    
    int i = 0;
    uint32_t tmp = 0;
    
    rumboot_printf("Check from address %x\n", mdma->dst_addr);
    for (i=0; i<len; i+=4){
        tmp = ioread32(mdma->dst_addr);
#ifdef EN_PRINT
        rumboot_printf("%x: %x, must be %x\n", mdma->dst_addr, tmp, init_val);
#endif
        if (init_val != tmp){
            rumboot_printf("Check fail\n \t %x: %x, must be %x!\n", mdma->dst_addr, tmp, init_val);
            return 1;
        }
        init_val = init_val + 0x04040404;
        mdma->dst_addr += 4;
    }
    return 0;
}

int mem_mdma_test (
    char* desc_heap,
    char* r_heap,
    char* w_heap,
    uint32_t data_size,
    uint32_t desc_num
) {

    int i = 0;
    
    struct mdma_trans_tbl mdma0_tbl;
    struct mdma_trans_tbl mdma1_tbl;

    struct mdma_trans_tbl* mdma0;
    struct mdma_trans_tbl* mdma1;
    
    mdma0 = &mdma0_tbl;
    mdma1 = &mdma1_tbl;
    
    if (mdma0 == NULL) {
        rumboot_printf("mdma0 == NULL\n");
        return 1;
    }
    if (mdma1 == NULL) {
        rumboot_printf("mdma1 == NULL\n");
        return 1;
    }
    
    mdma0->base_addr = MDMA0_BASE;
    mdma1->base_addr = MDMA1_BASE;

    rumboot_printf("MDMA 0\n");
    mdma_create_trans_tbl(mdma0, desc_num, desc_heap, r_heap, w_heap);
    mdma0->src_addr = (uint32_t) rumboot_malloc_from_heap_aligned(mdma0->r_heap_id, data_size, 8);
#ifdef DDR_WR
    uint32_t cur_addr = mdma0->src_addr;
    uint32_t data = 0x00010203;
    while(cur_addr <= (mdma0->src_addr + data_size)){            
        iowrite32(data, cur_addr);
        data = data + 0x04040404;
        cur_addr +=4;
    }
#endif    
    mdma0->dst_addr = (uint32_t) rumboot_malloc_from_heap_aligned(mdma0->w_heap_id, data_size, 8);
    mdma_create_trans_desc(mdma0, data_size, desc_num);

    rumboot_printf("MDMA 1\n");
    mdma_create_trans_tbl(mdma1, desc_num, desc_heap, r_heap, w_heap);
    mdma1->src_addr = mdma0->src_addr;
    mdma1->dst_addr = (uint32_t) rumboot_malloc_from_heap_aligned(mdma1->w_heap_id, data_size, 8);
    mdma_create_trans_desc(mdma1, data_size, desc_num);

#ifdef MDMAx4
   
    struct mdma_trans_tbl mdma2_tbl;
    struct mdma_trans_tbl mdma3_tbl;

    struct mdma_trans_tbl* mdma2;
    struct mdma_trans_tbl* mdma3;
    
    mdma2 = &mdma2_tbl;
    mdma3 = &mdma3_tbl;

    if (mdma2 == NULL) {
        rumboot_printf("mdma2 == NULL\n");
        return 1;
    }
    if (mdma3 == NULL) {
        rumboot_printf("mdma3 == NULL\n");
        return 1;
    }
    
    mdma2->base_addr = MDMA2_BASE;
    mdma3->base_addr = MDMA3_BASE;

    rumboot_printf("MDMA 2\n");
    mdma_create_trans_tbl(mdma2, desc_num, desc_heap, r_heap, w_heap);
    mdma2->src_addr = mdma0->src_addr;
    mdma2->dst_addr = (uint32_t) rumboot_malloc_from_heap_aligned(mdma2->w_heap_id, data_size, 8);
    mdma_create_trans_desc(mdma2, data_size, desc_num);

    rumboot_printf("MDMA 3\n");
    mdma_create_trans_tbl(mdma3, desc_num, desc_heap, r_heap, w_heap);
    mdma3->src_addr = mdma0->src_addr;
    mdma3->dst_addr = (uint32_t) rumboot_malloc_from_heap_aligned(mdma3->w_heap_id, data_size, 8);
    mdma_create_trans_desc(mdma3, data_size, desc_num);
#endif
    int delay = 0;
    int max_delay = 100000;
    
    rumboot_printf("Start 0!");
    mdma_start(mdma0);
    delay = 0;
//    while (delay < max_delay) {delay++;}
    rumboot_printf("Start 1!");
    mdma_start(mdma1);
    
#ifdef MDMAx4
    delay = 0;
//    while (delay < max_delay) {delay++;}
    rumboot_printf("Start 2!");
    mdma_start(mdma2);
    delay = 0;
//    while (delay < max_delay) {delay++;}
    rumboot_printf("Start 3!");
    mdma_start(mdma3);
#endif
    
    i = 0;
//    for (i=0; i<cycles_limit; i++){
    while(1){
//    while(i<2){
        if (i%1000 == 0){
            rumboot_printf("Cycle %d\n", i);
        }
        
        if (mdma_wait_end_trans(mdma0))
            return 1;
        mdma_trans_restart(mdma0, data_size, desc_num);

        if (mdma_wait_end_trans(mdma1))
            return 1;
        mdma_trans_restart(mdma1, data_size, desc_num);
#ifdef MDMAx4
        if (mdma_wait_end_trans(mdma2))
            return 1;
        mdma_trans_restart(mdma2, data_size, desc_num);

        if (mdma_wait_end_trans(mdma3))
            return 1;
        mdma_trans_restart(mdma3, data_size, desc_num);
        
#endif
        i++;
    }

    return 0;
}

int main() {
    if (ddr_init (DDR0_BASE) != 0)
        return -1;
    uint32_t desc_num = 64; // max 511
//    uint32_t data_size = 8192;
//    uint32_t data_size = 1024;
    uint32_t data_size = 4096;
    
#ifdef DDR_WR    
    rumboot_printf("Start WRITE to DDR0\n");
    if (mem_mdma_test("IM0", "IM1", "DDR0", data_size, desc_num))
        return 1;
#endif

#ifdef DDR_RD    
    if (ddr_init (DDR1_BASE) != 0)
        return -1;
    rumboot_printf("Start READ from DDR0\n");
    if (mem_mdma_test("IM0", "DDR0", "DDR1", data_size, desc_num))
        return 1;
#endif


    rumboot_printf("Finish work!\n");
    return 0;
}