#ifndef TEST_EVENT_ASM_H_
#define TEST_EVENT_ASM_H_

#include <platform/arch/ppc/ppc_476fp_asm.h>
#include <platform/oi10/platform/test_event_codes.h>


.macro gtube_init
    load_const  r0, 0x00000000  // make r0 "all zeros" source
    mtspr       SPR_SPRG0, r0
    mtspr       SPR_SPRG1, r0
    mtspr       SPR_SPRG2, r0
    mtspr       SPR_SPRG3, r0
    mtspr       SPR_SPRG4, r0
    mtspr       SPR_SPRG5, r0
    mtspr       SPR_SPRG6, r0
    mtspr       SPR_SPRG7, r0
    mtspr       SPR_SPRG8, r0
    load_const  r1, 0x0003c0de;
    mtspr       SPR_SPRG0, r1
.endm

.macro exit code
    load_const  r2, 1;
    load_const  r1, \code; /* test stdout */
    mtspr       SPR_SPRG2, r1

    mtspr       SPR_SPRG1, r2
    mtspr       SPR_SPRG1, r0
.endm

#define EVENT_TESTEVENT 0x11
.macro test_event test_event_code
    load_const  r2, EVENT_TESTEVENT;
    load_const  r1, \test_event_code;
    mtspr       SPR_SPRG2, r1

    mtspr       SPR_SPRG1, r2
    mtspr       SPR_SPRG1, r0
.endm

.macro test_event_send_test_id test_id_ptr
    load_addr  r2, \test_id_ptr
    mtspr       SPR_SPRG3, r2
    test_event TEST_EVENT_NEW_TEST_STARTED
.endm

#endif /* TEST_EVENT_ASM_H_ */
