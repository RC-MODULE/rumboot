#ifndef PPC_476FP_MMU_H
#define PPC_476FP_MMU_H


#include <common_macros/common_macros.h>
#include <arch/ppc/ibm_bit_ordering_macros.h>
#include <platform/oi10/platform/ppc_c.h>
#include <platform/oi10/platform/ppc470s/mmu/mem_window.h>
#include <arch/ppc/ppc_476fp_mmu_fields.h>


inline static void tlbsync() {
    asm volatile( "tlbsync \n\t" );
}


inline static void set_mem_window( MEM_WINDOW const window ) {
    SPR_PID_write( window );
    isync();
}
inline static MEM_WINDOW get_mem_window() {
    return( (MEM_WINDOW)SPR_PID_read() );
}


typedef struct tlb_entry
{
    uint32_t    mmucr;
    uint32_t    ra;
    uint32_t    tag;
    uint32_t    data;
    uint32_t    attributes;
} tlb_entry;

void write_tlb_entries( tlb_entry const * entries, uint32_t n );

int64_t get_physical_addr(uint32_t ea);

#endif // PPC_476FP_MMU_H
