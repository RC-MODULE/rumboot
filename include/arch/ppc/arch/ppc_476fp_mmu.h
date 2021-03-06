#ifndef PPC_476FP_MMU_H
#define PPC_476FP_MMU_H


#include <arch/ppc_476fp_lib_c.h>
#include <arch/ppc_476fp_config.h>
#include <arch/mem_window.h>
#include <arch/ppc_476fp_mmu_fields.h>


inline static void set_mem_window( MEM_WINDOW const window ) {
    spr_write( SPR_PID, window );
    isync();
}
inline static MEM_WINDOW get_mem_window() {
    return ( ( MEM_WINDOW )spr_read( SPR_PID ) );
}

typedef struct tlb_entry {
    uint32_t mmucr;
    uint32_t tag;
    uint32_t data;
//    union {
//    uint32_t ra;
    uint32_t attributes;
//    };
} __attribute__(( aligned(16) )) tlb_entry;

void write_tlb_entries( tlb_entry const * entries, uint32_t n );

/**
 * Gets physical address for virtual address
 * @param ea virtual address
 * @param ts translation space (usually 0)
 * @return if found physical address, value < 0 otherwise
 */
int64_t get_physical_addr( uint32_t ea, uint32_t ts );


#endif // PPC_476FP_MMU_H
