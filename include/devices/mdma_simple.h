#ifndef MDMA_SIMPLE_
#define MDMA_SIMPLE_

//enum mdmaRES {
//    OK,
//    ERROR
//};

struct simple_descr {
    void*       addr_src    ;
    void*       addr_dst    ;
    uint32_t    data_size   ;
};

struct mdma_descr {
    uint32_t data_addr  : 32;   // only for 32b address
    uint32_t length     : 14;
    uint32_t cstm_lngth : 12;
    uint32_t err        : 1 ;
    uint32_t incr       : 1 ;
    uint32_t stop       : 1 ;
    uint32_t intr       : 1 ;
    uint32_t link       : 1 ;
    uint32_t own        : 1 ;

    uint32_t reserve    : 32;
    uint32_t str_lnth   : 16;
    uint32_t pitch      : 16;
} __attribute__((packed));

void simple_config_mdma(uintptr_t base);

void simple_descr_form (struct mdma_descr* p_mdma_descr, void* addr, uint32_t size);

int simple_mdma_exec (int heap_id, uintptr_t base, void* addr_src, void* addr_dst, uint32_t data_size);

int simple_mdma_end_wait(uintptr_t base, uint32_t size);

#endif