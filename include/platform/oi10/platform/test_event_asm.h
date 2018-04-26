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

//TODO rewrite it!
.macro test_event test_event_code
/*
    load_const  r12, \test_event_code
    mtspr   SPR_USPGR0, r12
    load_const  r12, EVENT_RESERVED1
    mtspr   SPR_USPGR0, r12
*/
.endm

#endif /* TEST_EVENT_ASM_H_ */
