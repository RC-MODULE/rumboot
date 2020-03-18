#ifndef TRACE_S_H_
#define TRACE_S_H_


#include <arch/ppc_476fp_asm.h>
#include <platform/test_event_asm.h>
#include <platform/test_event_codes.h>


ASM_BEGIN
ASM_TEXT(   .macro putchar char, tmp_reg=r5, tmp_reg_msr=r6                                                         )
ASM_TEXT(       disable_ext_irq     ASM_ESC(tmp_reg), ASM_ESC(tmp_reg_msr)                                          )
ASM_TEXT(                                                                                                           )
ASM_TEXT(       load_const          ASM_ESC(tmp_reg), ASM_ESC(char)                                                 )
ASM_TEXT(       rumboot_event_arg1_ EVENT_STDOUT, ASM_ESC(tmp_reg)                                                  )
ASM_TEXT(                                                                                                           )
ASM_TEXT(       mtmsr               ASM_ESC(tmp_reg_msr)                                                            )
ASM_TEXT(   .endm                                                                                                   )
ASM_END

ASM_BEGIN
ASM_TEXT(   .macro rumboot_putstring text, tmp_reg=r5, tmp_reg_msr=r6                                               )
ASM_TEXT(       b 2f                                                                                                )
ASM_TEXT(                                                                                                           )
ASM_TEXT(   1:  .ascii STRINGIZE(ASM_ESC(text))                                                                     )
                /*end of string*/
ASM_TEXT(       .byte  0x0                                                                                          )
ASM_TEXT(                                                                                                           )
ASM_TEXT(   .align 2, 0                                                                                             )
ASM_TEXT(   2:  disable_ext_irq     ASM_ESC(tmp_reg), ASM_ESC(tmp_reg_msr)                                          )
ASM_TEXT(                                                                                                           )
ASM_TEXT(       load_addr           ASM_ESC(tmp_reg), 1b                                                            )
ASM_TEXT(       mtspr               SPR_SPRG3, ASM_ESC(tmp_reg)                                                     )
ASM_TEXT(       load_const          ASM_ESC(tmp_reg), EVENT_TRACE_MESSAGE                                           )
ASM_TEXT(       rumboot_event_arg1_ EVENT_TESTEVENT, ASM_ESC(tmp_reg)                                               )
ASM_TEXT(                                                                                                           )
ASM_TEXT(       mtmsr               ASM_ESC(tmp_reg_msr)                                                            )
ASM_TEXT(   .endm                                                                                                   )
ASM_END

ASM_BEGIN
ASM_TEXT(   .macro rumboot_puthex hex_reg, tmp_reg=r5, tmp_reg_msr=r6                                               )
ASM_TEXT(       disable_ext_irq     ASM_ESC(tmp_reg), ASM_ESC(tmp_reg_msr)                                          )
ASM_TEXT(                                                                                                           )
ASM_TEXT(       mtspr               SPR_SPRG3, ASM_ESC(hex_reg)                                                     )
ASM_TEXT(       load_const          ASM_ESC(tmp_reg), EVENT_TRACE_HEX                                               )
ASM_TEXT(       rumboot_event_arg1_ EVENT_TESTEVENT, ASM_ESC(tmp_reg)                                               )
ASM_TEXT(                                                                                                           )
ASM_TEXT(       mtmsr               ASM_ESC(tmp_reg_msr)                                                            )
ASM_TEXT(   .endm                                                                                                   )
ASM_END

ASM_BEGIN
ASM_TEXT(   .macro _rumboot_putdump tmp_reg_start_addr, tmp_reg_length_in_bytes, tmp_reg=r5, tmp_reg_msr=r6         )
ASM_TEXT(       disable_ext_irq     ASM_ESC(tmp_reg), ASM_ESC(tmp_reg_msr)                                          )
ASM_TEXT(                                                                                                           )
ASM_TEXT(       mtspr               SPR_SPRG3, ASM_ESC(tmp_reg_start_addr)                                          )
ASM_TEXT(       mtspr               SPR_SPRG4, ASM_ESC(tmp_reg_length_in_bytes)                                     )
ASM_TEXT(       load_const          ASM_ESC(tmp_reg), EVENT_TRACE_DUMP                                              )
ASM_TEXT(       rumboot_event_arg1_ EVENT_TESTEVENT, ASM_ESC(tmp_reg)                                               )
ASM_TEXT(                                                                                                           )
ASM_TEXT(       mtmsr               ASM_ESC(tmp_reg_msr)                                                            )
ASM_TEXT(   .endm                                                                                                   )
ASM_END

ASM_BEGIN
ASM_TEXT(   .macro rumboot_putdump start_addr, length_in_bytes, tmp_reg=r5, tmp_reg_msr=r6                          )
ASM_TEXT(       disable_ext_irq     ASM_ESC(tmp_reg), ASM_ESC(tmp_reg_msr)                                          )
ASM_TEXT(                                                                                                           )
ASM_TEXT(       load_addr           ASM_ESC(tmp_reg), ASM_ESC(start_addr)                                           )
ASM_TEXT(       mtspr               SPR_SPRG3, ASM_ESC(tmp_reg)                                                     )
ASM_TEXT(       load_size           ASM_ESC(tmp_reg), ASM_ESC(length_in_bytes)                                      )
ASM_TEXT(       mtspr               SPR_SPRG4, ASM_ESC(tmp_reg)                                                     )
ASM_TEXT(       load_const          ASM_ESC(tmp_reg), EVENT_TRACE_DUMP                                              )
ASM_TEXT(       rumboot_event_arg1_ EVENT_TESTEVENT, ASM_ESC(tmp_reg)                                               )
ASM_TEXT(                                                                                                           )
ASM_TEXT(       mtmsr               ASM_ESC(tmp_reg_msr)                                                            )
ASM_TEXT(   .endm                                                                                                   )
ASM_END


#endif /* TRACE_S_H_ */
