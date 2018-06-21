#ifndef TRACE_S_H_
#define TRACE_S_H_

#include <platform/arch/ppc/ppc_476fp_asm.h>
#include <platform/oi10/platform/test_event_asm.h>
#include <platform/oi10/platform/test_event_codes.h>

.macro _putchar reg_data, reg_ctrl
    load_const  \reg_ctrl, 2;
    mtspr       SPR_SPRG2, \reg_data
    load_const  \reg_data, 0
    mtspr       SPR_SPRG1, r12
    mtspr       SPR_SPRG1, \reg_data
.endm

.macro putchar char
    load_const  r5, \char; /* test stdout */
    _putchar r5, r6
.endm


.macro rumboot_putstring text
    b 2f
1:
    .ascii "\text"
    .byte  0x0
    /*end of string*/
.align 2, 0
2:
    load_addr  r5, 1b
    mtspr       SPR_SPRG3, r5
    test_event EVENT_TRACE_MESSAGE
.endm


.macro rumboot_puthex hex_reg
    mtspr       SPR_SPRG3, \hex_reg
    test_event EVENT_TRACE_HEX
.endm


.macro rumboot_putdump start_addr, length_in_bytes
    mtspr SPR_SPRG3, \start_addr
    mtspr SPR_SPRG4, \length_in_bytes
    test_event EVENT_TRACE_DUMP
.endm


.macro rumboot_issdump start_addr, length_in_bytes
    mtspr SPR_SPRG3, \start_addr
    mtspr SPR_SPRG4, \length_in_bytes
    test_event TEST_EVENT_CREATE_ISS_DUMP
.endm

#endif /* TRACE_S_H_ */
