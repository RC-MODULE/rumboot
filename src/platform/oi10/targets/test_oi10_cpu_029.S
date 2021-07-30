#include <arch/ppc_476fp_asm.h>
#include <platform/test_event_asm.h>
#include <platform/test_assert.S.h>
#include <platform/arch/ppc/test_macro_asm.S.h>

/* as defined in units/cpu/units/ppc_chip/verilog/rtl/PPC470S_TOP111323221111.v */
#define PIR_VALUE	0x00000000 /* Single CPU */

#ifdef PVR_OI10
    #define PVR_VALUE	0x7FF520C1 /* owner=0x7FF, proc ver=0x520C2 */
#endif

#ifdef PVR_O32T
    #define PVR_VALUE	0x7FF520C2 /* owner=0x7FF, proc ver=0x520C2 */
#endif

.section ".text","ax",@progbits

.global main

main:
    mfspr		r4, SPR_PIR
    mfspr		r5, SPR_PVR
    load_const	r6, PIR_VALUE
    load_const	r7, PVR_VALUE
    cmp cr6, 0, r4, r6
    cmp cr7, 0, r5, r7
    TEST_ASSERT(eq, cr6, "PIR value is incorrect.")
    TEST_ASSERT(eq, cr7, "PVR value is incorrect.")
    test_event	EVENT_CHECK_PIR_PVR
    bne- cr6,	test_error
    bne- cr7,	test_error


test_ok:
    rumboot_putstring "\nTEST OK\n"
    test_event		EVENT_OK
    load_const r3,	RESULT_OK
    b finish

test_error:
    rumboot_putstring "\nTEST ERROR\n"
    test_event		EVENT_ERROR
    load_const r3,	RESULT_ERROR

finish:
    blr


