#ifndef TRACE_S_H_
#define TRACE_S_H_


#include <platform/arch/ppc/ppc_476fp_asm.h>
#include <platform/test_event_asm.h>
#include <platform/test_event_codes.h>


.macro putchar char, tmp_reg=r5, tmp_reg_msr=r6
    disable_ext_irq     \tmp_reg, \tmp_reg_msr

    load_const          \tmp_reg, \char
    rumboot_event_arg1_ EVENT_STDOUT, \tmp_reg

    mtmsr               \tmp_reg_msr
.endm


.macro rumboot_putstring text, tmp_reg=r5, tmp_reg_msr=r6
    b 2f

1:  .ascii "\text"
    .byte  0x0 /*end of string*/

.align 2, 0
2:  disable_ext_irq     \tmp_reg, \tmp_reg_msr

    load_addr           \tmp_reg, 1b
    mtspr               SPR_SPRG3, \tmp_reg
    load_const          \tmp_reg, EVENT_TRACE_MESSAGE
    rumboot_event_arg1_ EVENT_TESTEVENT, \tmp_reg

    mtmsr               \tmp_reg_msr
.endm


.macro rumboot_puthex hex_reg, tmp_reg=r5, tmp_reg_msr=r6
    disable_ext_irq     \tmp_reg, \tmp_reg_msr

    mtspr               SPR_SPRG3, \hex_reg
    load_const          \tmp_reg, EVENT_TRACE_HEX
    rumboot_event_arg1_ EVENT_TESTEVENT, \tmp_reg

    mtmsr               \tmp_reg_msr
.endm


.macro _rumboot_putdump tmp_reg_start_addr, tmp_reg_length_in_bytes, tmp_reg=r5, tmp_reg_msr=r6
    disable_ext_irq     \tmp_reg, \tmp_reg_msr

    mtspr               SPR_SPRG3, \tmp_reg_start_addr
    mtspr               SPR_SPRG4, \tmp_reg_length_in_bytes
    load_const          \tmp_reg, EVENT_TRACE_DUMP
    rumboot_event_arg1_ EVENT_TESTEVENT, \tmp_reg

    mtmsr               \tmp_reg_msr
.endm


.macro rumboot_putdump start_addr, length_in_bytes, tmp_reg=r5, tmp_reg_msr=r6
    disable_ext_irq     \tmp_reg, \tmp_reg_msr

    load_const          \tmp_reg, \start_addr
    mtspr               SPR_SPRG3, \tmp_reg
    load_const          \tmp_reg, \length_in_bytes
    mtspr               SPR_SPRG4, \tmp_reg
    load_const          \tmp_reg, EVENT_TRACE_DUMP
    rumboot_event_arg1_ EVENT_TESTEVENT, \tmp_reg

    mtmsr               \tmp_reg_msr
.endm


.macro _rumboot_issdump tmp_reg_start_addr, tmp_reg_length_in_bytes, tmp_reg=r5, tmp_reg_msr=r6
    disable_ext_irq     \tmp_reg, \tmp_reg_msr

    mtspr               SPR_SPRG3, \tmp_reg_start_addr
    mtspr               SPR_SPRG4, \tmp_reg_length_in_bytes
    load_const          \tmp_reg, TEST_EVENT_CREATE_ISS_DUMP
    rumboot_event_arg1_ EVENT_TESTEVENT, \tmp_reg

    mtmsr               \tmp_reg_msr
.endm


.macro rumboot_issdump start_addr, length_in_bytes, tmp_reg=r5, tmp_reg_msr=r6
    disable_ext_irq     \tmp_reg, \tmp_reg_msr

    load_const          \tmp_reg, \start_addr
    mtspr               SPR_SPRG3, \tmp_reg
    load_const          \tmp_reg, \length_in_bytes
    mtspr               SPR_SPRG4, \tmp_reg
    load_const          \tmp_reg, TEST_EVENT_CREATE_ISS_DUMP
    rumboot_event_arg1_ EVENT_TESTEVENT, \tmp_reg

    mtmsr               \tmp_reg_msr
.endm


#endif /* TRACE_S_H_ */
