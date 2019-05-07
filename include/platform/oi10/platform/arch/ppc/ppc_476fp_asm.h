#ifndef PPC_476FP_ASM_H
#define PPC_476FP_ASM_H


#include <arch/ppc_476fp_config.h>
#include <platform/common_macros/common_macros.h>
#include <platform/arch/ppc/ibm_bit_ordering_macros.h>


#define copy_field( reg_dest,\
                        reg_dest_field_begin,\
                        reg_dest_field_end,\
                    reg_src,\
                        reg_src_field_begin )\
    rlwimi  reg_dest, reg_src,\
            (((reg_src_field_begin)%32) + (32 - ((reg_dest_field_begin)%32)))%32,\
            (reg_dest_field_begin)%32, (reg_dest_field_end)%32

#define field_begin( field )  CAT( field,_e ) - ( CAT( field,_n ) - 1 )

#ifdef __ASSEMBLER__
#define ASM_BEGIN
#define ASM_TEXT(text...)   text
#define ASM_ESC(text...)    \text
#define ASM_END
#else
#define ASM_BEGIN   asm (
#define ASM_TEXT(text...)   __stringify_1(text)"\n"
#define ASM_ESC(text...)    \\text
#define ASM_END     );
#endif

ASM_BEGIN
ASM_TEXT(   .macro  load_const rD, constant                                                                             )
ASM_TEXT(   .if ((ASM_ESC(constant) & 0xFFFF8000) == 0xFFFF8000) || ((ASM_ESC(constant) & 0xFFFF8000) == 0x00000000)    )
ASM_TEXT(       li      ASM_ESC(rD), ASM_ESC(constant)                                                                  )
ASM_TEXT(   .else                                                                                                       )
ASM_TEXT(       lis     ASM_ESC(rD), ASM_ESC(constant@h)                                                                )
ASM_TEXT(   .if ((ASM_ESC(constant) & 0x0000FFFF) != 0x00000000)                                                        )
ASM_TEXT(       ori     ASM_ESC(rD), ASM_ESC(rD), ASM_ESC(constant@l)                                                   )
ASM_TEXT(   .endif                                                                                                      )
ASM_TEXT(   .endif                                                                                                      )
ASM_TEXT(   .endm                                                                                                       )
ASM_END

ASM_BEGIN
ASM_TEXT(   .macro  load_addr rD, addr                                                                                  )
ASM_TEXT(       lis     ASM_ESC(rD), ASM_ESC(addr@h)                                                                    )
ASM_TEXT(       ori     ASM_ESC(rD), ASM_ESC(rD), ASM_ESC(addr@l)                                                       )
ASM_TEXT(   .endm                                                                                                       )
ASM_END

ASM_BEGIN
ASM_TEXT(   .macro  load_size rD, size                                                                                  )
ASM_TEXT(       lis     ASM_ESC(rD), ASM_ESC(size@h)                                                                    )
ASM_TEXT(       ori     ASM_ESC(rD), ASM_ESC(rD), ASM_ESC(size@l)                                                       )
ASM_TEXT(   .endm                                                                                                       )
ASM_END

ASM_BEGIN
ASM_TEXT(   .macro  store_spr SPR, rT, rA, D                                                                            )
ASM_TEXT(       mfspr ASM_ESC(rT),ASM_ESC(SPR)                                                                          )
ASM_TEXT(       stw ASM_ESC(rT),ASM_ESC(D)(ASM_ESC(rA))                                                                 )
ASM_TEXT(   .endm                                                                                                       )
ASM_END

ASM_BEGIN
ASM_TEXT(   .macro  store_cr rT, rA, D                                                                                  )
ASM_TEXT(       mfcr ASM_ESC(rT)                                                                                        )
ASM_TEXT(       stw ASM_ESC(rT),ASM_ESC(D)(ASM_ESC(rA))                                                                 )
ASM_TEXT(   .endm                                                                                                       )
ASM_END

/* Temp register -vv */
ASM_BEGIN
ASM_TEXT(   .macro save_stack tR                                                                                        )
ASM_TEXT(       mtspr   SPR_SPRG7, r1                                                                                   )
ASM_TEXT(       mflr    ASM_ESC(tR)                                                                                     )
ASM_TEXT(       mtspr   SPR_SPRG8, ASM_ESC(tR)                                                                          )
ASM_TEXT(   .endm                                                                                                       )
ASM_END

/* Temp register -vv */
ASM_BEGIN
ASM_TEXT(   .macro rest_stack tR                                                                                        )
ASM_TEXT(       mfspr   ASM_ESC(tR), SPR_SPRG8                                                                          )
ASM_TEXT(       mtlr    ASM_ESC(tR)                                                                                     )
ASM_TEXT(       mfspr   r1, SPR_SPRG7                                                                                   )
ASM_TEXT(   .endm                                                                                                       )
ASM_END

#define PPC470_REG_SIZE     (4)
#define ITRPT_CONTEXT_SIZE  ((32+4+4)*PPC470_REG_SIZE)  // 32 -GPRs, 4|6 - SPRs, 2|0 - paddings, 1 - LR save Area, 1 - Back Chain (r1)

ASM_BEGIN
ASM_TEXT(   .macro full_prologue save_csrr=0                                                                            )
ASM_TEXT(       stwu        r1, -ITRPT_CONTEXT_SIZE(r1)                                                                 )
                /* Skip LR save Area and 2 paddings */
ASM_TEXT(       stmw        r0, (4*PPC470_REG_SIZE)(r1)                                                                 )
ASM_TEXT(       mflr        r31                                                                                         )
ASM_TEXT(       mfctr       r30                                                                                         )
ASM_TEXT(       mfcr        r29                                                                                         )
ASM_TEXT(       mfxer       r28                                                                                         )
ASM_TEXT(       stmw        r28, ((4+32)*PPC470_REG_SIZE)(r1)                                                           )
ASM_TEXT(   .if( ASM_ESC(save_csrr) != 0 )                                                                              )
ASM_TEXT(       mfspr       r31, SPR_CSRR1                                                                              )
ASM_TEXT(       mfspr       r30, SPR_CSRR0                                                                              )
                /* Skip LR save Area and save in 2 paddings */
ASM_TEXT(       stmw        r30, (2*PPC470_REG_SIZE)(r1)                                                                )
ASM_TEXT(   .endif                                                                                                      )
                /* TODO: remove setting CTR and LR to Program interrupt handler. It's only for modeling (https://jira.module.ru/jira/browse/OI10-205) */
ASM_TEXT(       load_addr   r3, rumboot_P_hdr                                                                           )
ASM_TEXT(       mtctr       r3                                                                                          )
ASM_TEXT(       mtlr        r3                                                                                          )
ASM_TEXT(   .endm                                                                                                       )
ASM_END

ASM_BEGIN
ASM_TEXT(   .macro full_epilogue restore_csrr=0                                                                         )
ASM_TEXT(   .if( ASM_ESC(restore_csrr) != 0 )                                                                           )
ASM_TEXT(       lmw         r30, (2*PPC470_REG_SIZE)(r1)                                                                )
ASM_TEXT(       mtspr       SPR_CSRR0, r30                                                                              )
ASM_TEXT(       mtspr       SPR_CSRR1, r31                                                                              )
ASM_TEXT(   .endif                                                                                                      )
ASM_TEXT(       lmw         r28, ((4+32)*PPC470_REG_SIZE)(r1)                                                           )
ASM_TEXT(       mtxer       r28                                                                                         )
ASM_TEXT(       mtcr        r29                                                                                         )
ASM_TEXT(       mtctr       r30                                                                                         )
ASM_TEXT(       mtlr        r31                                                                                         )
ASM_TEXT(       lmw         r2, ((4+2)*PPC470_REG_SIZE)(r1)                                                             )
ASM_TEXT(       lwz         r0, (4*PPC470_REG_SIZE)(r1)                                                                 )
ASM_TEXT(       addi        r1, r1, ITRPT_CONTEXT_SIZE                                                                  )
ASM_TEXT(   .endm                                                                                                       )
ASM_END

#endif // PPC_476FP_ASM_H
