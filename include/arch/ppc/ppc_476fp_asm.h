#ifndef PPC_476FP_ASM_H
#define PPC_476FP_ASM_H


#include <arch/ppc/ppc_476fp_config.h>
#include <common_macros/common_macros.h>
#include <arch/ppc/ibm_bit_ordering_macros.h>


#define reg_field(field_right_bit_num_from_ppc_user_manual, value)\
    ((value) << IBM_BIT_INDEX( 32, field_right_bit_num_from_ppc_user_manual ))
#define copy_field( reg_dest,\
                        reg_dest_field_begin,\
                        reg_dest_field_end,\
                    reg_src,\
                        reg_src_field_begin )\
    rlwimi  reg_dest, reg_src,\
            (((reg_src_field_begin)%32) + (32 - ((reg_dest_field_begin)%32)))%32,\
            (reg_dest_field_begin)%32, (reg_dest_field_end)%32

.macro  load_const rD, constant
.if ((\constant & 0xFFFF8000) == 0xFFFF8000) || ((\constant & 0xFFFF8000) == 0x00000000)
    li      \rD, \constant
.else
    lis     \rD, \constant@h
.if ((\constant & 0x0000FFFF) != 0x00000000)
    ori     \rD, \rD, \constant@l
.endif
.endif
.endm

.macro  load_addr rD, addr
    lis     \rD, \addr@h
    ori     \rD, \rD, \addr@l
.endm

.macro  load_size rD, size
    lis     \rD, \size@h
    ori     \rD, \rD, \size@l
.endm

.macro  store_spr SPR, rT, rA, D
    mfspr \rT,\SPR
    stw \rT,\D(\rA)
.endm

.macro  store_cr rT, rA, D
    mfcr \rT
    stw \rT,\D(\rA)
.endm

#define DTLB_ERROR_EA 0x00000000
.macro run_in_itrpt_handler_dtlb_error USER_HANDLER_NAME
setup_dtlb_error_interrupt:
    load_addr r1, dtlb_error_int_handler
    lis r2, 0xFFFF
    and r2, r1, r2
    mtspr SPR_IVPR, r2
    andi. r3, r1, 0xFFF0
    mtspr SPR_IVOR13, r3
    /*Generate dtlb_error exception*/
    load_const r1, DTLB_ERROR_EA
    lwz r2, 0(r1)
    /*infinite loop*/
wait_interrupt_dtlb_error:
    b wait_interrupt_dtlb_error
.align 4
dtlb_error_int_handler:
    /*code for clear interrupt*/

    b \USER_HANDLER_NAME
.endm

.macro run_in_itrpt_handler_alignment USER_HANDLER_NAME
setup_alignment_interrupt:
    load_addr r1, alignment_int_handler
    lis r2, 0xFFFF
    and r2, r1, r2
    mtspr SPR_IVPR, r2
    andi. r3, r1, 0xFFF0
    mtspr SPR_IVOR5, r3
    /*Generate alignment exception*/
    mfspr r1, SPR_CCR0
    load_const r2, (1 << CTRL_CCR0_FLSTA_i)
    or r1, r1, r2
    mtspr SPR_CCR0, r1
    load_addr r3, alignment_int_handler
    lwz r1, 1(r3)
    /*infinite loop*/
wait_interrupt_alignment:
    b wait_interrupt_alignment
.align 4
alignment_int_handler:
    /*code for clear interrupt*/
    /*Turn back CCR0.FLSTA (clear interrupt)\n");*/
    mfspr r1, SPR_CCR0
    load_const r2, ~(1 << CTRL_CCR0_FLSTA_i)
    and r1, r1, r2
    mtspr SPR_CCR0, r1
    b \USER_HANDLER_NAME
.endm

.macro  write_tlb_entry     rA, rB, rC,     ERPN, RPN, EPN, DSIZ,  IL1I, IL1D, W, I, M, G,   UX, UW, UR,     SX, SW, SR
/*TLB_RS_word0              EPN RS[32:51]             V RS[52]            TS RS[53]        DSIZ RS[54:59]*/
    load_const \rA, reg_field(51, \EPN) | reg_field(52, 0b1) | reg_field(53, 0b0) | reg_field(59, \DSIZ)
    tlbwe   \rA, \rB, 0

/*TLB_RS_word1         RPN RS[32:51]                   ERPN RS[54:63]*/
    load_const \rA, reg_field(51, \RPN) | reg_field(63, \ERPN)
    /*rlwimi_copy_field(\rA, 54, 63, \rC, 2) //RS[54:63] = RSTCFG[2:11]. See ERPN field in RSTCFG and tlbwe*/
    tlbwe   \rA, \rB, 1

/*TLB_RS_word2         IL1I                  IL1D                      U                         W                    I                    M                    G               E                     UX                    UW                   UR                  SX                   SW                   SR*/
    load_const \rA, reg_field(46, \IL1I) | reg_field(47, \IL1D) /*| reg_field(51, \U)*/ | reg_field(52, \W) | reg_field(53, \I) | reg_field(54, \M) | reg_field(55, \G) | /*reg_field(56, \E) |*/ reg_field(58, \UX) | reg_field(59, \UW) | reg_field(60, \UR) | reg_field(61, \SX) | reg_field(62, \SW) | reg_field(63, \SR)
    copy_field(\rA, 56, 56, \rC, 17)  /*RS[56] = RSTCFG[17]. See E field in RSTCFG and tlbwe*/
    copy_field(\rA, 48, 51, \rC, 28)  /*RS[48:51] = RSTCFG[28:31]. See U field in RSTCFG and tlbwe*/
    tlbwe   \rA, \rB, 2
.endm


#endif // PPC_476FP_ASM_H
