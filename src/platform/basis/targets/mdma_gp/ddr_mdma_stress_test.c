#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/timer.h>
#include <rumboot/rumboot.h>

#include <regs/regs_mdma.h>

#include <platform/devices.h>


#define STOP_FLAG       0x10000000
#define INT_FLAG        0x20000000
    
#define INT_STAT    0x4
#define BAD_STAT    0x8
#define STOP_STAT   0x10

//#define EN_PRINT
#define MDMAx4
#define RANDOM_ACCESS

struct mdma_trans_tbl {
    int      heap_id;
    uint32_t base_addr;
    uint32_t r_tbl_addr;
    uint32_t src_addr;
    uint32_t w_tbl_addr;
    uint32_t dst_addr;
};

void mdma_create_trans_tbl (struct mdma_trans_tbl * mdma, char* desc_heap, char* data_heap) {
    int id = 0;
    
    rumboot_printf("mdma_create_trans_tbl for %x\n", mdma->base_addr);
    
    id = rumboot_malloc_heap_by_name(desc_heap);
    mdma->r_tbl_addr = (uint32_t) rumboot_malloc_from_heap_aligned(id, 4096, 8);
    mdma->w_tbl_addr = (uint32_t) rumboot_malloc_from_heap_aligned(id, 4096, 8);
    
    mdma->heap_id = rumboot_malloc_heap_by_name(data_heap);
    
    iowrite32(mdma->r_tbl_addr, mdma->base_addr + MDMA_DESC_ADDR_R);    // Set read channel first descriptor address
    iowrite32(mdma->w_tbl_addr, mdma->base_addr + MDMA_DESC_ADDR_W);    // Set read channel first descriptor address
}

void mdma_create_trans_desc (struct mdma_trans_tbl* mdma, uint32_t len, uint32_t init_val) {

#ifdef EN_PRINT    
    rumboot_printf("mdma_create_trans_desc for %x\n", mdma->base_addr);
#endif    
    // Read channel
#ifdef EN_PRINT
    rumboot_printf("Read channel  ");
#endif
    iowrite32(mdma->src_addr, mdma->r_tbl_addr);
    mdma->r_tbl_addr = mdma->r_tbl_addr + 4;
    iowrite32((len | STOP_FLAG), mdma->r_tbl_addr);
    mdma->r_tbl_addr = mdma->r_tbl_addr + 4;
    iowrite32(0xffffffff, mdma->r_tbl_addr+8); // to stop
#ifdef EN_PRINT    
    rumboot_printf("%x: ", mdma->src_addr);    
    rumboot_printf("%x_", ioread32(mdma->r_tbl_addr-4));
    rumboot_printf("%x.\n", ioread32(mdma->r_tbl_addr-8));
#endif
    if (init_val != 0){
//        rumboot_printf("Fill descriptor %x.\n", data_addr);
        uint32_t cur_addr = mdma->src_addr;
        uint32_t data = init_val;
        while(cur_addr <= (mdma->src_addr + len)){            
            iowrite32(data, cur_addr);
//            rumboot_printf("%x: %x\n", cur_addr, ioread32(cur_addr));
            data = data + 0x04040404;
            cur_addr +=4;
        }
    }
    
    // Write channel
#ifdef EN_PRINT
    rumboot_printf("Write channel ");
#endif    
    iowrite32(mdma->dst_addr, mdma->w_tbl_addr);
    mdma->w_tbl_addr = mdma->w_tbl_addr + 4;
    iowrite32((len | STOP_FLAG), mdma->w_tbl_addr);
    mdma->w_tbl_addr = mdma->w_tbl_addr + 4;
    iowrite32(0xffffffff, mdma->w_tbl_addr+8); // to stop
#ifdef EN_PRINT    
    rumboot_printf("%x: ", mdma->dst_addr);    
    rumboot_printf("%x_", ioread32(mdma->w_tbl_addr-4));
    rumboot_printf("%x.\n", ioread32(mdma->w_tbl_addr-8));
#endif
}

static inline void mdma_start (struct mdma_trans_tbl* mdma){
    iowrite32(1, mdma->base_addr + MDMA_ENABLE_W);
    iowrite32(1, mdma->base_addr + MDMA_ENABLE_R);    
}

uint32_t mdma_wait_end_trans (struct mdma_trans_tbl* mdma) {
    uint32_t status = 0;
    uint32_t tmp = 0;
#ifdef RANDOM_ACCESS
    uint32_t rand_addr = 0;
    if (rumboot_malloc_heap_name(mdma->heap_id) == "IM0")
        rand_addr = mdma->dst_addr + 0x10000;
    else
        rand_addr = mdma->dst_addr + 0x10000000;
    
    uint32_t rand_data = mdma->dst_addr;
#endif

    while (!status){
        status = ioread32(mdma->base_addr + MDMA_STATUS_W) & (BAD_STAT | STOP_STAT);
#ifdef RANDOM_ACCESS
        iowrite32(rand_data, rand_addr);
        tmp = ioread32(rand_addr);
        if (tmp != rand_data){
            rumboot_printf("Error random access to address %x\n", rand_addr);
            rumboot_printf("   Read data : %x\n", tmp);
            rumboot_printf("   Write data: %x\n", rand_data);
            return 1;
        }
        rand_addr = rand_addr + 0x1000;
        rand_data = rand_data + 0x01010101;
#endif
    }
    if (status & BAD_STAT){
        tmp = ioread32(mdma->base_addr + MDMA_CUR_DESC_ADDR_W);
        rumboot_printf("Bad descriptor with address %x\n", tmp);
        return 1;
    }
    
    return 0;
}

uint32_t mdma_trans_restart (struct mdma_trans_tbl* mdma, uint32_t data_size) {

    mdma->src_addr = mdma->dst_addr;
    mdma->dst_addr = (uint32_t) rumboot_malloc_from_heap_aligned(mdma->heap_id, data_size, 8);
    
    mdma_create_trans_desc(mdma, data_size, 0);
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

int mem_mdma_test (char* test_heap, char* desc_heap, uint32_t data_size, uint32_t cycles_limit) {

    int i = 0;
    uint32_t delay_limit = 40000;
    uint32_t mdma0_init = 0x00010203;
    uint32_t mdma1_init = 0x01020304;
    
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
    mdma_create_trans_tbl(mdma0, desc_heap, test_heap);
    mdma0->src_addr = (uint32_t) rumboot_malloc_from_heap_aligned(mdma0->heap_id, data_size, 8);
    mdma0->dst_addr = (uint32_t) rumboot_malloc_from_heap_aligned(mdma0->heap_id, data_size, 8);
    mdma_create_trans_desc(mdma0, data_size, mdma0_init);

    rumboot_printf("MDMA 1\n");
    mdma_create_trans_tbl(mdma1, desc_heap, test_heap);
    mdma1->src_addr = (uint32_t) rumboot_malloc_from_heap_aligned(mdma1->heap_id, data_size, 8);
    mdma1->dst_addr = (uint32_t) rumboot_malloc_from_heap_aligned(mdma1->heap_id, data_size, 8);
    mdma_create_trans_desc(mdma1, data_size, mdma1_init);

#ifdef MDMAx4
    uint32_t mdma2_init = 0x02030405;
    uint32_t mdma3_init = 0x03040506;
    
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
    mdma_create_trans_tbl(mdma2, desc_heap, test_heap);
    mdma2->src_addr = (uint32_t) rumboot_malloc_from_heap_aligned(mdma2->heap_id, data_size, 8);
    mdma2->dst_addr = (uint32_t) rumboot_malloc_from_heap_aligned(mdma2->heap_id, data_size, 8);
    mdma_create_trans_desc(mdma2, data_size, mdma2_init);

    rumboot_printf("MDMA 3\n");
    mdma_create_trans_tbl(mdma3, desc_heap, test_heap);
    mdma3->src_addr = (uint32_t) rumboot_malloc_from_heap_aligned(mdma3->heap_id, data_size, 8);
    mdma3->dst_addr = (uint32_t) rumboot_malloc_from_heap_aligned(mdma3->heap_id, data_size, 8);
    mdma_create_trans_desc(mdma3, data_size, mdma1_init);
#endif
    
    rumboot_printf("Start 0!");
    mdma_start(mdma0);
    // Delay
    rumboot_printf("Start 1!");
//    i = 0;
//    while (i<delay_limit) {i++;}
//    for (i=0; i<delay_limit; i++){} // Delay
    mdma_start(mdma1);
    
#ifdef MDMAx4
    rumboot_printf("Start 2!");
    mdma_start(mdma2);
    rumboot_printf("Start 3!");
    mdma_start(mdma3);
#endif
    
    for (i=0; i<cycles_limit; i++){
        rumboot_printf("Cycle %d\n", i);
        
        if (mdma_wait_end_trans(mdma0))
            return 1;
        if (i != cycles_limit-1)
            mdma_trans_restart(mdma0, data_size);

        if (mdma_wait_end_trans(mdma1))
            return 1;
        if (i != cycles_limit-1)
            mdma_trans_restart(mdma1, data_size);
#ifdef MDMAx4
        if (mdma_wait_end_trans(mdma2))
            return 1;
        if (i != cycles_limit-1)
            mdma_trans_restart(mdma2, data_size);

        if (mdma_wait_end_trans(mdma3))
            return 1;
        if (i != cycles_limit-1)
            mdma_trans_restart(mdma3, data_size);
#endif
    }
    
    if (mdma_data_check(mdma0, data_size, mdma0_init))
        return 1;
    if (mdma_data_check(mdma1, data_size, mdma1_init))
        return 1;
#ifdef MDMAx4
    if (mdma_data_check(mdma2, data_size, mdma2_init))
        return 1;
    if (mdma_data_check(mdma3, data_size, mdma3_init))
        return 1;
#endif

    return 0;
}

int main() {
    uint32_t cycles_limit = 4; // max 511
    uint32_t data_size = 32768;
    
    rumboot_printf("Start test with %d cycles\n", cycles_limit);
    
    if (mem_mdma_test("DDR0", "DDR0", data_size, cycles_limit))
        return 1;
    
    rumboot_printf("Finish work!\n");
    return 0;
}