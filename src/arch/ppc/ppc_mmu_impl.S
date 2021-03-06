#include <arch/ppc_476fp_asm.h>
#include <arch/ppc_476fp_mmu_fields.h>
#include <arch/ppc_476fp_ctrl_fields.h>


.section ".ppc470s_invalidate_all_tlb_entries_nosnoop.mmu.init.text","ax",@progbits
.global ppc470s_invalidate_all_tlb_entries_nosnoop
ppc470s_invalidate_all_tlb_entries_nosnoop:
    load_const  r3, MMU_UTLB_INDEXES_N
    mtctr       r3

    load_const  r3, MMU_MMUCR( 0b0,  0b0,0b0,    0b0,     0x0000 ) /* STID = 0x0000 */
    mtspr       SPR_MMUCR, r3

    load_const  r3, (0x01000 << MMU_TLBE_TAG_EPN_i)
    load_const  r4, (0x00000 << MMU_TLBE_TAG_EPN_i) | (MMU_TLBE_DSIZ_16MB << MMU_TLBE_TAG_DSIZ_i)
    load_const  r7, (MMU_TLBWE_WAY_0 << MMU_TLBE_RA_WAY_i) | (MMU_TLBWE_BE_UND << MMU_TLBE_RA_BE_i)
    load_const  r8, (MMU_TLBWE_WAY_1 << MMU_TLBE_RA_WAY_i) | (MMU_TLBWE_BE_UND << MMU_TLBE_RA_BE_i)
    load_const  r9, (MMU_TLBWE_WAY_2 << MMU_TLBE_RA_WAY_i) | (MMU_TLBWE_BE_UND << MMU_TLBE_RA_BE_i)
    load_const  r10,(MMU_TLBWE_WAY_3 << MMU_TLBE_RA_WAY_i) | (MMU_TLBWE_BE_UND << MMU_TLBE_RA_BE_i)

.align 4
invalidate_tlb_entry: /* Invalidate entris for ways 0-3, even if bolted entry */
    tlbwe       r4, r7, MMU_TLB_ENTRY_TAG
    tlbwe       r4, r8, MMU_TLB_ENTRY_TAG
    tlbwe       r4, r9, MMU_TLB_ENTRY_TAG
    tlbwe       r4, r10,MMU_TLB_ENTRY_TAG

goto_next_tlb_entry:
    add         r4, r4, r3 /* Calculate next entry EPN (index) */
    bdnz+       invalidate_tlb_entry
    blr

.section ".write_tlb_entries.mmu.init.text","ax",@progbits
/*Details about trace infrastructure convention see in trace.S.h
Caller must preserve r3,r4,r8,r9,r10,r11 before calling
r3 - TLB entries array pointer
r4 - TLB entries count
r8,r9,r10,r11 - tmp registers
returns nothing*/
.global write_tlb_entries
write_tlb_entries:
    cmplwi      r4, 0
    beqlr-

    mr          r8, r28
    mr          r9, r29
    mr          r10,r30
    mr          r11,r31

.align 4
write_next_etry:
    lmw         r28,0(r3)
    addi        r3, r3, MMU_TLB_ENTRY_SIZE

    mtspr       SPR_MMUCR,  r28
    tlbwe       r29,r31,MMU_TLB_ENTRY_TAG
    tlbwe       r30,r31,MMU_TLB_ENTRY_DATA
    tlbwe       r31,r31,MMU_TLB_ENTRY_ATTR

    subic.      r4, r4, 1
    bne+        write_next_etry

write_tlb_entries_exit:
    mfspr       r3, SPR_SRR0
    mfspr       r4, SPR_SRR1

    mfmsr       r31
    mtspr       SPR_SRR1, r31
    load_addr   r31, write_tlb_entries_return
    mtspr       SPR_SRR0, r31

    rfi

write_tlb_entries_return:
    mtspr       SPR_SRR1, r4
    mtspr       SPR_SRR0, r3

    mr          r28,r8
    mr          r29,r9
    mr          r30,r10
    mr          r31,r11

    blr

.section ".get_physical_addr.mmu.init.text","ax",@progbits
/*r3 - parameter, 32bit effective address (EA)*/
/*r4 - TS (translation space)
/*r4,r3 - result, 42bit real address (int64_t)*/
/*if return value <0 then tlb entry was not found*/
/*r5,r6  - tmp register*/
.global get_physical_addr
get_physical_addr:
    mfspr       r5, SPR_MMUCR
    copy_field( r5, field_begin(MMU_MMUCR_STS), MMU_MMUCR_STS_e, r4, IBM_BIT_INDEX(32, 0) )         /* copy STS field */

    load_const  r4, (0x0000 << MMU_PID_i)                                                           /* first try with PID == 0x0000 (shared) */
    copy_field( r5, field_begin(MMU_MMUCR_STID), MMU_MMUCR_STID_e, r4, field_begin(MMU_PID) )
    mtspr       SPR_MMUCR, r5
    tlbsx.      r4, 0, r3                                                                           /* search for tlb entry */
    beq+        tlbsx_ok                                                                            /* (CR[CR0[2]] == 1), see 4.8.1. PowerPC 476FP Embedded Processor Core. User's Manual */

    mfspr       r4, SPR_PID                                                                         /* second try with actual PID */
    copy_field( r5, field_begin(MMU_MMUCR_STID), MMU_MMUCR_STID_e, r4, field_begin(MMU_PID) )
    mtspr       SPR_MMUCR, r5
    tlbsx.      r4, 0, r3                                                                           /* search for tlb entry */
    bne-        tlbsx_error                                                                         /* (CR[CR0[2]] == 0), see 4.8.1. PowerPC 476FP Embedded Processor Core. User's Manual */

tlbsx_ok:
/*    mfctr       r7*/
    tlbre       r5, r4, MMU_TLB_ENTRY_TAG                                                           /* RT[32:60] <= tlbentry[EPN(0:19), V, TS, DSIZ(0:5), BLTD] */
    extrwi      r5, r5, MMU_TLBE_TAG_DSIZ_n, field_begin(MMU_TLBE_TAG_DSIZ)                         /* RT <= DSIZ(0:5) */
    cntlzw      r5, r5                                                                              /* RT <= 6(MMU_TLBE_DSIZ_4KB); 5(MMU_TLBE_DSIZ_16KB); */
    subi        r5, r5, 32-MMU_TLBE_TAG_DSIZ_n                                                      /*       ...; 0(MMU_TLBE_DSIZ_1GB)*/
    slwi        r5, r5, 3                                                                           /* RT <= RT * 8(two instructions) */
    load_addr   r6, offset
    add         r6, r6, r5
    mtctr       r6
    bctr
offset:
offset_1g:
    clrlwi      r5, r3, 2                                                                           /* RT <= RT & 0x3FFFFFFF */
    b           offset_done
offset_256m:
    clrlwi      r5, r3, 4                                                                           /* RT <= RT & 0x0FFFFFFF */
    b           offset_done
offset_16m:
    clrlwi      r5, r3, 8                                                                           /* RT <= RT & 0x00FFFFFF */
    b           offset_done
offset_1m:
    clrlwi      r5, r3, 12                                                                          /* RT <= RT & 0x000FFFFF */
    b           offset_done
offset_64k:
    clrlwi      r5, r3, 16                                                                          /* RT <= RT & 0x0000FFFF */
    b           offset_done
offset_16k:
    clrlwi      r5, r3, 18                                                                          /* RT <= RT & 0x00003FFF */
    b           offset_done
offset_4k:
    clrlwi      r5, r3, 20                                                                          /* RT <= RT & 0x00000FFF */
    b           offset_done

offset_done:
    tlbre       r4, r4, MMU_TLB_ENTRY_DATA                                                          /* RT[32:51] <= RPN, RT[54:63] <= ERPN*/
    clrlwi      r3, r4, field_begin(MMU_TLBE_DATA_ERPN)                                             /* RT <= ERPN */
    clrrwi      r4, r4, MMU_TLBE_DATA_RPN_i                                                         /* RT <= RPN */
    or          r4, r4, r5                                                                          /* RT <= RPN | OFFSET */
/*    mtcrt       r7*/
    blr

tlbsx_error:
    load_const  r3, -1 /*return invalid value (< 0) if an error*/
    blr
