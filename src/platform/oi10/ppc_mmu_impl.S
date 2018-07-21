#include <platform/arch/ppc/ppc_476fp_asm.h>
#include <platform/arch/ppc/ppc_476fp_mmu_fields.h>
#include <platform/arch/ppc/ppc_476fp_ctrl_fields.h>


.section ".mmu.init.text","ax",@progbits

/*Details about trace infrastructure convention see in trace.S.h
Caller must preserve r3,r4,r8,r9,r10 before calling
r3 - TLB entries array pointer
r4 - TLB entries count
r8,r9,r10 - tmp registers
returns nothing*/
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
    copy_field( r8, field_begin( MMU_TLBE_ATTR_U ), MMU_TLBE_ATTR_U_e, r9, field_begin( CTRL_RSTCFG_U ) )
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

/*r3 - parameter, 32bit effective address (EA)*/
/*r4 - TS (translation space)
/*r4,r3 - result, 42bit real address (int64_t)*/
/*if return value <0 (bit 63 = 1) then tlb entry was not found*/
/*r5,r6,r7,r8,r9,r10 - tmp registers*/
.global get_physical_addr
get_physical_addr:
    mfspr       r10, SPR_MMUCR
    copy_field( r10, field_begin( MMU_MMUCR_STS ), MMU_MMUCR_STS_e, r4, IBM_BIT_INDEX( 32, 0 ) )    /* copy STS field */

    load_const  r9, ( 0x0000 << MMU_PID_i )                                                         /* first try with PID == 0x0000 (shared) */
    copy_field( r10, field_begin( MMU_MMUCR_STID ), MMU_MMUCR_STID_e, r9, field_begin( MMU_PID ) )
    mtspr       SPR_MMUCR, r10
    tlbsx.      r5, 0, r3                                                                           /* search for tlb entry */
    beq+        tlbsx_ok                                                                            /* (CR[CR0[2]] == 1) 4.8.1. UG */

    mfspr       r9, SPR_PID                                                                         /* second try with actual PID */
    copy_field( r10, field_begin( MMU_MMUCR_STID ), MMU_MMUCR_STID_e, r9, field_begin( MMU_PID ) )
    mtspr       SPR_MMUCR, r10
    tlbsx.      r5, 0, r3                                                                           /* search for tlb entry */
    bne-        tlbsx_error                                                                         /* (CR[CR0[2]] == 0) 4.8.1. UG */

tlbsx_ok:
    mr          r9, r3                                                                              /* save EA */

    tlbre       r6, r5, MMU_TLB_ENTRY_TAG /* RT[54:63] <= ERPN, RT[32:60] <= tlbentry[EPN(0:19), V, TS, DSIZ(0:5), BLTD] */
    srwi        r6,r6, 0x4
    andi.       r6,r6,0x3F
    /*r6[58:63] = DSIZ, swith(DSIZ):*/
    cmpwi       cr7, r6, 0x0
    beq         cr7, dsiz_4k
    cmpwi       cr7, r6, 0x1
    beq         cr7, dsiz_16k
    cmpwi       cr7, r6, 0x3
    beq         cr7, dsiz_64k
    cmpwi       cr7, r6, 0x7
    beq         cr7, dsiz_1m
    cmpwi       cr7, r6, 0xF
    beq         cr7, dsiz_16m
    cmpwi       cr7, r6, 0x1F
    beq         cr7, dsiz_256m
    cmpwi       cr7, r6, 0x3F
    beq         cr7, dsiz_1g
    b           tlbsx_error /* default */
/*Offset mask  - R8*/
dsiz_4k:
    load_const  r8, 20
    b           dsiz_done
dsiz_16k:
    load_const  r8, 18
    b           dsiz_done
dsiz_64k:
    load_const  r8, 16
    b           dsiz_done
dsiz_1m:
    load_const  r8, 12
    b           dsiz_done
dsiz_16m:
    load_const  r8, 8
    b           dsiz_done
dsiz_256m:
    load_const  r8, 4
    b           dsiz_done
dsiz_1g:
    load_const  r8, 2
    b           dsiz_done
dsiz_done:
    tlbre       r6, r5, MMU_TLB_ENTRY_DATA      /* RT[32:51] <= RPN, RT[54:63] <= ERPN*/
    andi.       r3,r6,0x3FF                     /*RT <= ERPN*/
    /*clear R6[52:63] by mask, shift RPN*/
    load_const  r7, 0xFFFFF000
    and         r6,r6,r7
    /*Offset*/
    load_const  r10, 0xFFFFFFFF
    srw         r10, r10, r8
    and         r9,r9,r10
    /*get R4=RPN(r6) | OFFSET(r9)*/
    or          r4, r6, r9
    blr

tlbsx_error:
    load_const  r3, -1 /*return invalid value (< 0) if an error*/
    blr


