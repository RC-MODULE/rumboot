#ifndef PPC_476FP_MMU_S_H
#define PPC_476FP_MMU_S_H


#include <arch/ppc_476fp_asm.h>
#include <platform/arch/ppc/ppc_476fp_mmu_fields.h>
#include <platform/ppc470s/mmu/mem_window.h>


.macro  set_mem_window rT, window
    load_const  \rT, \window
    mtspr       SPR_PID, \rT
    isync
.endm


.global write_tlb_entries


#endif // PPC_476FP_MMU_S_H
