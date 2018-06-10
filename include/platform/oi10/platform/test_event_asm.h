#ifndef TEST_EVENT_ASM_H_
#define TEST_EVENT_ASM_H_

#include <platform/arch/ppc/ppc_476fp_asm.h>
#include <platform/oi10/platform/test_event_codes.h>


.macro gtube_init
    load_const  r5, 0x00000000  // make r0 "all zeros" source
    mtspr       SPR_SPRG0, r5
    mtspr       SPR_SPRG1, r5
    mtspr       SPR_SPRG2, r5
    mtspr       SPR_SPRG3, r5
    mtspr       SPR_SPRG4, r5
    mtspr       SPR_SPRG5, r5
    mtspr       SPR_SPRG6, r5
    mtspr       SPR_SPRG7, r5
    mtspr       SPR_SPRG8, r5
    load_const  r6, 0x0003c0de;
    mtspr       SPR_SPRG0, r6
.endm

.macro exit code
    load_const  r6, 0
    load_const  r7, 1
    load_const  r5, \code /* test stdout */
    mtspr       SPR_SPRG2, r5

    mtspr       SPR_SPRG1, r7
    mtspr       SPR_SPRG1, r6
.endm

#define EVENT_TESTEVENT 0x11
.macro test_event test_event_code
    load_const  r5, 0
    load_const  r6, EVENT_TESTEVENT;
    load_const  r7, \test_event_code;
    mtspr       SPR_SPRG2, r7

    mtspr       SPR_SPRG1, r6
    mtspr       SPR_SPRG1, r5
.endm

.macro test_event_send_test_id test_id
    b 4f
3:
    .ascii "\test_id"
    .byte  0x0
    /*end of string*/
.align 2, 0
4:
    load_addr  r5, 3b
    mtspr       SPR_SPRG3, r5
    test_event TEST_EVENT_NEW_TEST_STARTED
.endm

#endif /* TEST_EVENT_ASM_H_ */
