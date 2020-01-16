#ifndef TEST_EVENT_ASM_H_
#define TEST_EVENT_ASM_H_

#include <platform/arch/ppc/ppc_476fp_asm.h>
#include <arch/ppc_476fp_itrpt_fields.h>
#include <platform/test_event_codes.h>


/* TODO: remove repeated declaration */
#define EVENT_NOP               0
#define EVENT_TERM              1
#define EVENT_STDOUT            2
#define EVENT_TESTEVENT         17
#define EVENT_RELOCATE_RUNTIME  18


ASM_BEGIN
ASM_TEXT(   .macro gtube_init tmp_reg=r5                                                                        )
                /* make r0 "all zeros" source */
ASM_TEXT(       load_const          ASM_ESC(tmp_reg), 0x00000000                                                )
ASM_TEXT(       mtspr               SPR_SPRG0, ASM_ESC(tmp_reg)                                                 )
ASM_TEXT(       mtspr               SPR_SPRG1, ASM_ESC(tmp_reg)                                                 )
ASM_TEXT(       load_const          ASM_ESC(tmp_reg), 0x0003c0de                                                )
ASM_TEXT(       mtspr               SPR_SPRG0, ASM_ESC(tmp_reg)                                                 )
ASM_TEXT(   .endm                                                                                               )
ASM_END

ASM_BEGIN
ASM_TEXT(   .macro disable_ext_irq tmp_reg=r5, reg_old_msr=r6                                                   )
ASM_TEXT(       mfmsr               ASM_ESC(reg_old_msr)                                                        )
ASM_TEXT(       load_const          ASM_ESC(tmp_reg), ~( 1 << ITRPT_XSR_EE_i )                                  )
ASM_TEXT(       and                 ASM_ESC(tmp_reg), ASM_ESC(reg_old_msr), ASM_ESC(tmp_reg)                    )
ASM_TEXT(       mtmsr               ASM_ESC(tmp_reg)                                                            )
ASM_TEXT(   .endm                                                                                               )
ASM_END

ASM_BEGIN
ASM_TEXT(   .macro rumboot_event_arg1_ event_code, arg0_reg, tmp_reg=r5                                         )
ASM_TEXT(       mtspr               SPR_SPRG2, ASM_ESC(arg0_reg)                                                )
ASM_TEXT(       load_const          ASM_ESC(tmp_reg), ASM_ESC(event_code)                                       )
ASM_TEXT(       mtspr               SPR_SPRG1, ASM_ESC(tmp_reg)                                                 )
ASM_TEXT(       load_const          ASM_ESC(tmp_reg), EVENT_NOP                                                 )
ASM_TEXT(       mtspr               SPR_SPRG1, ASM_ESC(tmp_reg)                                                 )
ASM_TEXT(   .endm                                                                                               )
ASM_END

ASM_BEGIN
ASM_TEXT(   .macro exit exit_code, tmp_reg=r5, tmp_reg_msr=r6                                                   )
ASM_TEXT(       disable_ext_irq     ASM_ESC(tmp_reg), ASM_ESC(tmp_reg_msr)                                      )
ASM_TEXT(                                                                                                       )
ASM_TEXT(       load_const          ASM_ESC(tmp_reg), TEST_EVENT_NEW_TEST_FINISHED                              )
ASM_TEXT(       rumboot_event_arg1_ EVENT_TESTEVENT, ASM_ESC(tmp_reg)                                           )
ASM_TEXT(                                                                                                       )
ASM_TEXT(       load_const          ASM_ESC(tmp_reg), ASM_ESC(exit_code)                                        )
ASM_TEXT(       rumboot_event_arg1_ EVENT_TERM, ASM_ESC(tmp_reg)                                                )
ASM_TEXT(                                                                                                       )
ASM_TEXT(       mtmsr               ASM_ESC(tmp_reg_msr)                                                        )
ASM_TEXT(   .endm                                                                                               )
ASM_END

ASM_BEGIN
ASM_TEXT(   .macro gtube_init_runtime tmp_reg=r5, tmp_reg_msr=r6                                                )
ASM_TEXT(       disable_ext_irq     ASM_ESC(tmp_reg), ASM_ESC(tmp_reg_msr)                                      )
ASM_TEXT(                                                                                                       )
ASM_TEXT(       load_addr           ASM_ESC(tmp_reg), rumboot_platform_runtime_info                             )
ASM_TEXT(       rumboot_event_arg1_ EVENT_RELOCATE_RUNTIME, ASM_ESC(tmp_reg)                                    )
ASM_TEXT(                                                                                                       )
ASM_TEXT(       mtmsr               ASM_ESC(tmp_reg_msr)                                                        )
ASM_TEXT(   .endm                                                                                               )
ASM_END

ASM_BEGIN
ASM_TEXT(   .macro test_event test_event_code, tmp_reg=r5, tmp_reg_msr=r6                                       )
ASM_TEXT(       disable_ext_irq     ASM_ESC(tmp_reg), ASM_ESC(tmp_reg_msr)                                      )
ASM_TEXT(                                                                                                       )
ASM_TEXT(       load_const          ASM_ESC(tmp_reg), ASM_ESC(test_event_code)                                  )
ASM_TEXT(       rumboot_event_arg1_ EVENT_TESTEVENT, ASM_ESC(tmp_reg)                                           )
ASM_TEXT(                                                                                                       )
ASM_TEXT(       mtmsr               ASM_ESC(tmp_reg_msr)                                                        )
ASM_TEXT(   .endm                                                                                               )
ASM_END

ASM_BEGIN
ASM_TEXT(   .macro test_event_send_test_id test_id, tmp_reg=r5, tmp_reg_msr=r6                                  )
ASM_TEXT(       b 4f                                                                                            )
ASM_TEXT(                                                                                                       )
ASM_TEXT(   3:  .ascii STRINGIZE(ASM_ESC(test_id))                                                              )
/*end of string*/
ASM_TEXT(       .byte  0x0                                                                                      )
ASM_TEXT(                                                                                                       )
ASM_TEXT(   .align 2, 0                                                                                         )
ASM_TEXT(   4:  disable_ext_irq     ASM_ESC(tmp_reg), ASM_ESC(tmp_reg_msr)                                      )
ASM_TEXT(                                                                                                       )
ASM_TEXT(       load_addr           ASM_ESC(tmp_reg), 3b                                                        )
ASM_TEXT(       mtspr               SPR_SPRG3, ASM_ESC(tmp_reg)                                                 )
ASM_TEXT(       load_const          ASM_ESC(tmp_reg), TEST_EVENT_NEW_TEST_STARTED                               )
ASM_TEXT(       rumboot_event_arg1_ EVENT_TESTEVENT, ASM_ESC(tmp_reg)                                           )
ASM_TEXT(                                                                                                       )
ASM_TEXT(       mtmsr               ASM_ESC(tmp_reg_msr)                                                        )
ASM_TEXT(   .endm                                                                                               )
ASM_END

#endif /* TEST_EVENT_ASM_H_ */
