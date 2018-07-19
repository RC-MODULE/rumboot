#ifndef PPC_476FP_MMU_H
#define PPC_476FP_MMU_H


#include <platform/arch/ppc/ppc_476fp_lib_c.h>
#include <platform/ppc470s/mmu/mem_window.h>
#include <platform/arch/ppc/ppc_476fp_mmu_fields.h>


inline static void set_mem_window( MEM_WINDOW const window ) {
    spr_write( SPR_PID, window );
    isync();
}
inline static MEM_WINDOW get_mem_window() {
    return ( ( MEM_WINDOW )spr_read( SPR_PID ) );
}

typedef struct tlb_entry {
    uint32_t mmucr;
    uint32_t ra;
    uint32_t tag;
    uint32_t data;
    uint32_t attributes;
} tlb_entry;

void write_tlb_entries( tlb_entry const * entries, uint32_t n );

int64_t get_physical_addr( uint32_t ea );


#endif // PPC_476FP_MMU_H
