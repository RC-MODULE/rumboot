#include <platform/arch/ppc/ppc_476fp_asm.h>
#include <platform/arch/ppc/ppc_476fp_mmu_fields.h>
#include <platform/arch/ppc/ppc_476fp_ctrl_fields.h>
#include <platform/trace.S.h>

.section ".mmu.init.text","ax",@progbits

/*Details about trace infrastructure convention see in trace.S.h
Caller must preserve r3,r4,r8,r9,r10 before calling
r3 - TLB entries array pointer
r4 - TLB entries count
r8,r9,r10 - tmp registers
returns nothing*/
.global write_tlb_entries
.global ppc470s_invalidate_all_tlb_entries_nosnoop
ppc470s_invalidate_all_tlb_entries_nosnoop:
    load_const  r6, 256       //set counter to 1024 iterations (there are 1024 TLB entries)
    mtspr SPR_CTR, r6
    lis         r6, 0x100      //iteration index = 0
    load_const  r9, 0          //r9 = 0
    load_const  r8, (MMU_TLBE_DSIZ_16MB    << MMU_TLBE_TAG_DSIZ_i   )
    lis         r3, 0x8000
    lis         r4, 0xa000
    lis         r5, 0xc000
    lis         r7, 0xe000

.align 4
invalidate_tlb_entry:
//Calculate current entry way and index
    add     r8, r8, r6
    copy_field(r3, 40, 47, r8, 32)
    copy_field(r4, 40, 47, r8, 32)
    copy_field(r5, 40, 47, r8, 32)
    copy_field(r7, 40, 47, r8, 32)

//Invalidate entry, even if bolted entry: (r8[60] == 1 && r7[33:34] == 0b00).
    tlbwe   r8, r3, 0

    tlbwe   r8, r4, 0

    tlbwe   r8, r5, 0

    tlbwe   r8, r7, 0

goto_next_tlb_entry:
    bdnz+   invalidate_tlb_entry //decrement cnt and check for exit from loop
    blr

.align 4
write_tlb_entries:
    cmplwi      r4, 0
    beqlr-

    mfspr       r9, SPR_RSTCFG

    subi        r3, r3, MMU_TLB_ENTRY_FIELD_SIZE

write_next_etry:
    lwzu        r8, MMU_TLB_ENTRY_FIELD_SIZE(r3)
    mtspr       SPR_MMUCR, r8

    lwzu        r10, MMU_TLB_ENTRY_FIELD_SIZE(r3)

    lwzu        r8, MMU_TLB_ENTRY_FIELD_SIZE(r3)
    tlbwe       r8, r10, MMU_TLB_ENTRY_TAG

    lwzu        r8, MMU_TLB_ENTRY_FIELD_SIZE(r3)
    tlbwe       r8, r10, MMU_TLB_ENTRY_DATA

    lwzu        r8, MMU_TLB_ENTRY_FIELD_SIZE(r3)
    copy_field( r8, field_begin( MMU_TLBE_ATTR_U ), MMU_TLBE_ATTR_U_e, r9, field_begin( CTRL_RSTCFG_U ) )
    tlbwe       r8, r10, MMU_TLB_ENTRY_ATTR

    subic.      r4, r4, 1
    bne         write_next_etry

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

/*r3 - parameter, 32bit effective address (EA)*/
/*r4 - TS (translation space)
/*r4,r3 - result, 42bit real address (int64_t)*/
/*if return value <0 then tlb entry was not found*/
/*r5  - tmp register*/
.global get_physical_addr
get_physical_addr:
    mfspr       r5, SPR_MMUCR
    copy_field( r5, field_begin( MMU_MMUCR_STS ), MMU_MMUCR_STS_e, r4, IBM_BIT_INDEX( 32, 0 ) )     /* copy STS field */

    load_const  r4, ( 0x0000 << MMU_PID_i )                                                         /* first try with PID == 0x0000 (shared) */
    copy_field( r5, field_begin( MMU_MMUCR_STID ), MMU_MMUCR_STID_e, r4, field_begin( MMU_PID ) )
    mtspr       SPR_MMUCR, r5
    tlbsx.      r4, 0, r3                                                                           /* search for tlb entry */
    beq+        tlbsx_ok                                                                            /* (CR[CR0[2]] == 1), see 4.8.1. PowerPC 476FP Embedded Processor Core. User's Manual */

    mfspr       r4, SPR_PID                                                                         /* second try with actual PID */
    copy_field( r5, field_begin( MMU_MMUCR_STID ), MMU_MMUCR_STID_e, r4, field_begin( MMU_PID ) )
    mtspr       SPR_MMUCR, r5
    tlbsx.      r4, 0, r3                                                                           /* search for tlb entry */
    bne-        tlbsx_error                                                                         /* (CR[CR0[2]] == 0), see 4.8.1. PowerPC 476FP Embedded Processor Core. User's Manual */

tlbsx_ok:
    mr          r5, r3                                                                              /* save EA */

    tlbre       r3, r4, MMU_TLB_ENTRY_TAG                                                           /* RT[32:60] <= tlbentry[EPN(0:19), V, TS, DSIZ(0:5), BLTD] */
    andi.       r3, r3, ( 0x3F << MMU_TLBE_TAG_DSIZ_i )
offset_4k:
    cmpwi       r3, ( MMU_TLBE_DSIZ_4KB << MMU_TLBE_TAG_DSIZ_i )
    bne         offset_16k
    clrlwi      r5, r5, 20                                                                          /* RT <= RT & 0x00000FFF */
    b           offset_done
offset_16k:
    cmpwi       r3, ( MMU_TLBE_DSIZ_16KB << MMU_TLBE_TAG_DSIZ_i )
    bne         offset_64k
    clrlwi      r5, r5, 18                                                                          /* RT <= RT & 0x00003FFF */
    b           offset_done
offset_64k:
    cmpwi       r3, ( MMU_TLBE_DSIZ_64KB << MMU_TLBE_TAG_DSIZ_i )
    bne         offset_1m
    clrlwi      r5, r5, 16                                                                          /* RT <= RT & 0x0000FFFF */
    b           offset_done
offset_1m:
    cmpwi       r3, ( MMU_TLBE_DSIZ_1MB << MMU_TLBE_TAG_DSIZ_i )
    bne         offset_16m
    clrlwi      r5, r5, 12                                                                          /* RT <= RT & 0x000FFFFF */
    b           offset_done
offset_16m:
    cmpwi       r3, ( MMU_TLBE_DSIZ_16MB << MMU_TLBE_TAG_DSIZ_i )
    bne         offset_256m
    clrlwi      r5, r5, 8                                                                           /* RT <= RT & 0x00FFFFFF */
    b           offset_done
offset_256m:
    cmpwi       r3, ( MMU_TLBE_DSIZ_256MB << MMU_TLBE_TAG_DSIZ_i )
    bne         offset_1g
    clrlwi      r5, r5, 4                                                                           /* RT <= RT & 0x0FFFFFFF */
    b           offset_done
offset_1g:
    cmpwi       r3, ( MMU_TLBE_DSIZ_1GB << MMU_TLBE_TAG_DSIZ_i )
    bne         tlbsx_error /* default */
    clrlwi      r5, r5, 2                                                                           /* RT <= RT & 0xFFFFFFFF */
/*  b           offset_done*/

offset_done:
    tlbre       r4, r4, MMU_TLB_ENTRY_DATA                                                          /* RT[32:51] <= RPN, RT[54:63] <= ERPN*/
    clrlwi      r3, r4, 22                                                                          /* RT <= ERPN */
    clrrwi      r4, r4, 12                                                                          /* RT <= RPN */
    add         r4, r4, r5                                                                          /* RT <= RPN | OFFSET */
    blr

tlbsx_error:
    load_const  r3, -1 /*return invalid value (< 0) if an error*/
    blr


