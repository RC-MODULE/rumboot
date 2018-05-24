/*
 * write_tlb_entries.S.h
 *
 *  Created on: May 24, 2018
 *      Author: d.arsentiev
 */

#ifndef WRITE_TLB_ENTRIES_S_H_
#define WRITE_TLB_ENTRIES_S_H_

.macro implement__write_tlb_entries tlb_p tlb_c trA, trB, trC
/*Details about trace infrastructure convention see in trace.S.h
Caller must preserve \tlb_p,\tlb_c,\trA,\trB,\trC before calling
\tlb_p - TLB entries array pointer
\tlb_c - TLB entries count
\trA,\trB,\trC - tmp registers
returns nothing*/
.global write_tlb_entries
write_tlb_entries:
    mfspr       \trB, SPR_RSTCFG

    subi        \tlb_p, \tlb_p, MMU_TLB_ENTRY_FIELD_SIZE

write_next_etry:
    cmplwi      cr7, \tlb_c, 0
    beq-        cr7, write_tlb_entries_exit

    lwzu        \trA, MMU_TLB_ENTRY_FIELD_SIZE(\tlb_p)
    mtspr       SPR_MMUCR, \trA

    lwzu        \trC, MMU_TLB_ENTRY_FIELD_SIZE(\tlb_p)

    lwzu        \trA, MMU_TLB_ENTRY_FIELD_SIZE(\tlb_p)
    tlbwe       \trA, \trC, MMU_TLB_ENTRY_TAG

    lwzu        \trA, MMU_TLB_ENTRY_FIELD_SIZE(\tlb_p)
    tlbwe       \trA, \trC, MMU_TLB_ENTRY_DATA

    lwzu        \trA, MMU_TLB_ENTRY_FIELD_SIZE(\tlb_p)
/*    copy_field( \trA, MMU_MMU_TLB_ENTRY_ATTR_E_e-(MMU_MMU_TLB_ENTRY_ATTR_E_n-1), MMU_MMU_TLB_ENTRY_ATTR_E_e, \trB, CTRL_RSTCFG_E_e-(CTRL_RSTCFG_E_n-1) )*/
    copy_field( \trA, MMU_TLBE_ATTR_U_e-(MMU_TLBE_ATTR_U_n-1), MMU_TLBE_ATTR_U_e, \trB, CTRL_RSTCFG_U_e-(CTRL_RSTCFG_U_n-1) )
    tlbwe       \trA, \trC, MMU_TLB_ENTRY_ATTR

    subi        \tlb_c, \tlb_c, 1
    b           write_next_etry

write_tlb_entries_exit:
    mfspr       \trB, SPR_SRR0
    mfspr       \trC, SPR_SRR1

    mfmsr       \trA
    mtspr       SPR_SRR1, \trA
    load_addr   \trA, write_tlb_entries_return
    mtspr       SPR_SRR0, \trA

    rfi

write_tlb_entries_return:
    mtspr       SPR_SRR1, \trC
    mtspr       SPR_SRR0, \trB

    blr

.endm



#endif /* WRITE_TLB_ENTRIES_S_H_ */
