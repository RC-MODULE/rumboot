#ifndef TEST_EVENT_ASM_H_
#define TEST_EVENT_ASM_H_

#include <platform/oi10/platform/ppc_476fp_asm.h>
#include <platform/oi10/platform/test_event_codes.h>

.macro test_event test_event_code
    load_const  r12, \test_event_code
    mtspr   SPR_USPGR0, r12
    load_const  r12, EVENT_RESERVED1
    mtspr   SPR_USPGR0, r12
.endm

#endif /* TEST_EVENT_ASM_H_ */
