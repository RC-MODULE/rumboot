#ifndef TEST_EVENT_ASM_H_
#define TEST_EVENT_ASM_H_

#include <platform/arch/ppc/ppc_476fp_asm.h>
#include <platform/oi10/platform/test_event_codes.h>


.macro gtube_init tmp_reg=r5
    load_const  \tmp_reg, 0x00000000  // make r0 "all zeros" source
    mtspr       SPR_SPRG0, \tmp_reg
    mtspr       SPR_SPRG1, \tmp_reg
    mtspr       SPR_SPRG2, \tmp_reg
    mtspr       SPR_SPRG3, \tmp_reg
    mtspr       SPR_SPRG4, \tmp_reg
    mtspr       SPR_SPRG5, \tmp_reg
    mtspr       SPR_SPRG6, \tmp_reg
    mtspr       SPR_SPRG7, \tmp_reg
    mtspr       SPR_SPRG8, \tmp_reg
    load_const  \tmp_reg, 0x0003c0de;
    mtspr       SPR_SPRG0, \tmp_reg
.endm

.macro exit code tmp_reg_exit_code=r5, tmp_reg_nop=r6, tmp_reg_exit_opcode=r7
    test_event TEST_EVENT_NEW_TEST_FINISHED, \tmp_reg_exit_code, \tmp_reg_nop, \tmp_reg_exit_opcode
    load_const  \tmp_reg_nop, 0
    load_const  \tmp_reg_exit_opcode, 1
    load_const  \tmp_reg_exit_code, \code /* test stdout */
    mtspr       SPR_SPRG2, \tmp_reg_exit_code

    mtspr       SPR_SPRG1, \tmp_reg_exit_opcode
    mtspr       SPR_SPRG1, \tmp_reg_nop
.endm

#define EVENT_TESTEVENT 0x11
.macro test_event test_event_code, tmp_reg_nop=r5, tmp_reg_opcode_testevent=r6, tmp_reg_event_code=r7
    load_const  \tmp_reg_nop, 0
    load_const  \tmp_reg_opcode_testevent, EVENT_TESTEVENT;
    load_const  \tmp_reg_event_code, \test_event_code;
    mtspr       SPR_SPRG2, \tmp_reg_event_code

    mtspr       SPR_SPRG1, \tmp_reg_opcode_testevent
    mtspr       SPR_SPRG1, \tmp_reg_nop
.endm

.macro test_event_send_test_id test_id, tmp_reg_nop_or_addr=r5, tmp_reg_opcode_testevent=r6, tmp_reg_event_code=r7
    b 4f
3:
    .ascii "\test_id"
    .byte  0x0
    /*end of string*/
.align 2, 0
4:
    load_addr  \tmp_reg_nop_or_addr, 3b
    mtspr       SPR_SPRG3, \tmp_reg_nop_or_addr
    test_event TEST_EVENT_NEW_TEST_STARTED, \tmp_reg_nop_or_addr, \tmp_reg_opcode_testevent, \tmp_reg_event_code
.endm

#endif /* TEST_EVENT_ASM_H_ */
