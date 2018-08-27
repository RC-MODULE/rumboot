/*
 * test_oi10_sys_009.S
 *
 *  Created on: Aug 14, 2018
 *      Author: a.tarasov
 */
#include <platform/arch/ppc/test_macro_asm.S.h>
#include <platform/test_event_asm.h>
#include <platform/trace.S.h>
#include <platform/test_assert.h>

.global main

.section ".text","ax",@progbits
event_pointer:
.long 0

BEGIN_ENUM( test_event_code )
DECLARE_ENUM_VAL(   TEC_CHECK_DEBUG_GRPHLTR_GRPHLTS,    TEST_EVENT_CODE_MIN )
DECLARE_ENUM_NEXT(  TEC_CHECK_DEBUG_GRP0 )
DECLARE_ENUM_NEXT(  TEC_CHECK_DEBUG_GRP1 )
DECLARE_ENUM_NEXT(  TEC_CHECK_DEBUG_LDBO )
DECLARE_ENUM_NEXT(  TEC_CHECK_DEBUG_MACHINECHECK )
DECLARE_ENUM_NEXT(  TEC_CHECK_DEBUG_MASKOUT )
DECLARE_ENUM_NEXT(  TEC_CHECK_DEBUG_HALT )
DECLARE_ENUM_NEXT(  TEC_CHECK_DEBUG_UNCONDEVENT )
DECLARE_ENUM_NEXT(  TEC_CHECK_DEBUG_SYSTEMCONTROL )
DECLARE_ENUM_NEXT(  TEC_CHECK_DEBUG_JDCR_STO )
DECLARE_ENUM_NEXT(  TEC_CHECK_DEBUG_JDCR_UDE )
DECLARE_ENUM_NEXT(  TEC_CHECK_DEBUG_JDCR_RSDBSR )
DECLARE_ENUM_NEXT(  TEC_CHECK_DEBUG_SYSTEMSTATUS )
DECLARE_ENUM_NEXT(  TEC_CHECK_DEBUG_STUFF )
DECLARE_ENUM_NEXT(  TEC_CHECK_DEBUG_DBDR )
DECLARE_ENUM_NEXT(  TEC_CHECK_DEBUG_DBIMASK )
DECLARE_ENUM_NEXT(  TEC_CHECK_DEBUG_ICACHEWR )
DECLARE_ENUM_NEXT(  TEC_CHECK_DEBUG_FASTWR )
END_ENUM( test_event_code )

BEGIN_ENUM( tb_result )
DECLARE_ENUM_VAL( TB_RESULT_UNDEFINED,  0x8E591794 )
DECLARE_ENUM_VAL( TB_RESULT_OK,         0x8E591708 )
DECLARE_ENUM_VAL( TB_RESULT_ERROR,      0x8E5917E8 )
END_ENUM( tb_result )



main:
    mflr        r3              /* Save */
    mtspr       SPR_SPRG8,  r3  /*  LR  */
    test_event_send_test_id "test_oi10_sys_009"

    load_addr r3, event_pointer
    bl            rumboot_platform_event_get

//b test_ok

test_oi10_sys_009:

check_debug_dbdr:
    load_const  r3, TB_RESULT_UNDEFINED
    mtspr       SPR_DBDR, r3
    isync

    test_event  TEC_CHECK_DEBUG_DBDR
    msync

    lwz         r30, 0(r13)
    msync

    isync
    mfspr       r3, SPR_DBDR
    load_const  r4, TB_RESULT_OK
    cmpw        r3, r4
    TEST_ASSERT(eq, cr0, "DBDR check failed");

    b test_ok

test_error:
    rumboot_putstring "\nTEST ERROR\n", r15, r16, r17
    test_event      EVENT_ERROR, r15, r16, r17
    load_const  r3, RESULT_ERROR
    b finish

test_ok:
    rumboot_putstring "\nTEST OK\n", r15, r16, r17
    test_event      EVENT_OK, r15, r16, r17
    load_const  r3, RESULT_OK

finish:
    mfspr       r4, SPR_SPRG8   /* Restore */
    mtlr        r4              /*   LR    */
    blr
