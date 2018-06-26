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


.macro rumboot_putstring text, tmp_reg_nop_or_addr=r5, tmp_reg_opcode_testevent=r6, tmp_reg_event_code=r7
    b 2f
1:
    .ascii "\text"
    .byte  0x0
    /*end of string*/
.align 2, 0
2:
    load_addr  \tmp_reg_nop_or_addr, 1b
    mtspr       SPR_SPRG3, \tmp_reg_nop_or_addr
    test_event EVENT_TRACE_MESSAGE, \tmp_reg_nop_or_addr, \tmp_reg_opcode_testevent, \tmp_reg_event_code
.endm

.macro rumboot_puthex hex_reg, tmp_reg_opcode_testevent=r6, tmp_reg_event_code=r7
    mtspr       SPR_SPRG3, \hex_reg
    test_event EVENT_TRACE_HEX, \hex_reg, \tmp_reg_opcode_testevent, \tmp_reg_event_code
.endm

.macro _rumboot_putdump tmp_reg_start_addr, tmp_reg_length_in_bytes, tmp_reg_event_code=r7
    mtspr SPR_SPRG3, \tmp_reg_start_addr
    mtspr SPR_SPRG4, \tmp_reg_length_in_bytes
    test_event EVENT_TRACE_DUMP, \tmp_reg_start_addr, \tmp_reg_length_in_bytes, \tmp_reg_event_code
.endm

.macro rumboot_putdump start_addr, length_in_bytes, tmp_reg_start_addr=r5, tmp_reg_length_in_bytes=r6, tmp_reg_event_code=r7
    load_const \tmp_reg_start_addr, \start_addr
    load_const \tmp_reg_length_in_bytes, \length_in_bytes
    _rumboot_putdump \tmp_reg_start_addr, \tmp_reg_length_in_bytes, \tmp_reg_event_code
.endm

.macro _rumboot_issdump tmp_reg_start_addr, tmp_reg_length_in_bytes, tmp_reg_event_code=r7
    mtspr SPR_SPRG3, \tmp_reg_start_addr
    mtspr SPR_SPRG4, \tmp_reg_length_in_bytes
    test_event TEST_EVENT_CREATE_ISS_DUMP, \tmp_reg_start_addr, \tmp_reg_length_in_bytes, \tmp_reg_event_code
.endm

.macro rumboot_issdump start_addr, length_in_bytes, tmp_reg_start_addr=r5, tmp_reg_length_in_bytes=r6, tmp_reg_event_code=r7
    load_const \tmp_reg_start_addr, \start_addr
    load_const \tmp_reg_length_in_bytes, \length_in_bytes
    _rumboot_issdump \tmp_reg_start_addr, \tmp_reg_length_in_bytes, \tmp_reg_event_code
.endm

#endif /* TRACE_S_H_ */
