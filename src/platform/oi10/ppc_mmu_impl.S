#include <arch/ppc/ppc_476fp_asm.h>
#include <arch/ppc/ppc_476fp_mmu_fields.h>
#include <arch/ppc/ppc_476fp_ctrl_fields.h>


.section ".init.text","ax",@progbits


//Details about trace infrastructure convention see in trace.S.h
//Caller must preserve r3,r4,r8,r9,r10 before calling
//r3 - TLB entries array pointer
//r4 - TLB entries count
//r8,r9,r10 - tmp registers
//returns nothing
.global write_tlb_entries
write_tlb_entries:
    mfspr       r9, SPR_RSTCFG

    subi        r3, r3, MMU_TLB_ENTRY_FIELD_SIZE

write_next_etry:
    cmplwi      cr7, r4, 0
    beq-        cr7, write_tlb_entries_exit

    lwzu        r8, MMU_TLB_ENTRY_FIELD_SIZE(r3)
    mtspr       SPR_MMUCR, r8

    lwzu        r10, MMU_TLB_ENTRY_FIELD_SIZE(r3)

    lwzu        r8, MMU_TLB_ENTRY_FIELD_SIZE(r3)
    tlbwe       r8, r10, MMU_TLB_ENTRY_TAG

    lwzu        r8, MMU_TLB_ENTRY_FIELD_SIZE(r3)
    tlbwe       r8, r10, MMU_TLB_ENTRY_DATA

    lwzu        r8, MMU_TLB_ENTRY_FIELD_SIZE(r3)
//    copy_field( r8, MMU_MMU_TLB_ENTRY_ATTR_E_e-(MMU_MMU_TLB_ENTRY_ATTR_E_n-1), MMU_MMU_TLB_ENTRY_ATTR_E_e, r9, CTRL_RSTCFG_E_e-(CTRL_RSTCFG_E_n-1) )
    copy_field( r8, MMU_TLBE_ATTR_U_e-(MMU_TLBE_ATTR_U_n-1), MMU_TLBE_ATTR_U_e, r9, CTRL_RSTCFG_U_e-(CTRL_RSTCFG_U_n-1) )
    tlbwe       r8, r10, MMU_TLB_ENTRY_ATTR

    subi        r4, r4, 1
    b           write_next_etry

write_tlb_entries_exit:
    mfspr       r9, SPR_SRR0
    mfspr       r10, SPR_SRR1

    mfmsr       r8
    mtspr       SPR_SRR1, r8
    load_addr   r8, write_tlb_entries_return
    mtspr       SPR_SRR0, r8

    rfi

write_tlb_entries_return:
    mtspr       SPR_SRR1, r10
    mtspr       SPR_SRR0, r9

    blr

//r3 - parameter, 32bit effective address (EA)
//r4,r3 - result, 42bit real address (int64_t)
//if return value <0 (bit 31 = 1) then tlb entry was not found
//r5,r6,r7,r8,r9,r10 - tmp registers
.global get_physical_addr
get_physical_addr:
    mfMMUCR r10
    //clear r10[16:31]
    load_const r9, 0xFFFF0000
    and r10, r9, r10
    //PID->MMUCR[STID]
    mfPID r9
    andi. r9, r9, 0xFFFF
    or r10, r9, r10
    mtspr 946,r10
    //save EA
    ori r9, r3,0
    //search for tlb entry
    tlbsx. r5,0,r3
    //Check tlb entry found (CR[CR0[2]] == 1) 4.8.1. UG
    bne ra_error
    //if WS=0 RT[54:63] <= ERPN, RT[32:60] <= tlbentry[EPN(0:19), V, TS, DSIZ(0:5), BLTD]
    tlbre r6, r5, 0
    srwi r6,r6, 0x4
    andi. r6,r6,0x3F
    //r6[58:63] = DSIZ, swith(DSIZ):
    cmpwi cr7, r6, 0x0
    beq cr7, dsiz_4k
    cmpwi cr7, r6, 0x1
    beq cr7, dsiz_16k
    cmpwi cr7, r6, 0x3
    beq cr7, dsiz_64k
    cmpwi cr7, r6, 0x7
    beq cr7, dsiz_1m
    cmpwi cr7, r6, 0xF
    beq cr7, dsiz_16m
    cmpwi cr7, r6, 0x1F
    beq cr7, dsiz_256m
    cmpwi cr7, r6, 0x3F
    beq cr7, dsiz_1g
    //default - error
    b ra_error
//Offset mask  - R8
dsiz_4k:
    load_const r8, 20
    b dsiz_done
dsiz_16k:
    load_const r8, 18
    b dsiz_done
dsiz_64k:
    load_const r8, 16
    b dsiz_done
dsiz_1m:
    load_const r8, 12
    b dsiz_done
dsiz_16m:
    load_const r8, 8
    b dsiz_done
dsiz_256m:
    load_const r8, 4
    b dsiz_done
dsiz_1g:
    load_const r8, 2
    b dsiz_done
dsiz_done:
    //if WS=1 RT[32:51] <= RPN, RT[54:63] <= ERPN
    tlbre r6, r5, 1
    //r3 <= ERPN
    andi. r3,r6,0x3FF
    //clear R6[52:63] by mask, shift RPN
    load_const r7, 0xFFFFF000
    and r6,r6,r7
    //Offset
    load_const r10, 0xFFFFFFFF
    srw r10, r10, r8
    and r9,r9,r10
    //get R4=RPN(r6) | OFFSET(r9)
    or r4, r6, r9
    blr
ra_error:
    //return invalid value (< 0) if an error
    load_const r3, -1
    blr

