#include <platform/oi10/platform/arch/ppc/ppc_476fp_asm.h>
#include <platform/oi10/platform/test_event_asm.h>
#include <platform/oi10/platform/test_assert.S.h>
#include <platform/oi10/platform/arch/ppc/test_macro_asm.S.h>

#define PVR_VALUE 0x7FF520C0

.section ".text","ax",@progbits

.global boot_init
.global main

main:
boot_init:
    mfspr r4, SPR_PIR
    mfspr r5, SPR_PVR
    load_const r6, PVR_VALUE
    cmp cr7, 0, r5, r6
    TEST_ASSERT(eq, cr7, "PVR value is incorrect.")
    test_event EVENT_CHECK_PIR_PVR

program_end:
    load_const r3, 0x00
    blr

