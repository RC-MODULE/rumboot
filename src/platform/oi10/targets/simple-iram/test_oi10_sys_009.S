/*
 * test_oi10_sys_009.S
 *
 *  Created on: Aug 14, 2018
 *      Author: a.tarasov
 */
#include <platform/arch/ppc/test_macro_asm.S.h>
#include <platform/test_event_asm.h>
#include <platform/trace.S.h>
#include <platform/test_assert.S.h>
#include <platform/arch/ppc/ppc_476fp_timer_fields.h>


.global main

.section ".text","ax",@progbits
event_pointer:
.long 0


boot_init:
    load_const  r0, 0x00000000  // make r0 "all zeros" source
    load_const  r1, 0xFFFFFFFF  // make r1 "all ones" source
    load_const  r2, 0x00000000
    load_const  r3, 0x00000000
    load_const  r4, 0x00000000
    load_const  r5, 0x00000000
    load_const  r6, 0x00000000
    load_const  r7, 0x00000000
    load_const  r8, 0x00000000
    load_const  r9, 0x00000000
    load_const  r10,0x00000000
    load_const  r11,0x00000000
    load_const  r12,0x00000000
//    load_const  r13,0x00000000
    load_const  r14,0x00000000
    load_const  r15,0x00000000
    load_const  r16,0x00000000
    load_const  r17,0x00000000
    load_const  r18,0x00000000
    load_const  r19,0x00000000
    load_const  r20,0x00000000
    load_const  r21,0x00000000
    load_const  r22,0x00000000
    load_const  r23,0x00000000
    load_const  r24,0x00000000
    load_const  r25,0x00000000
    load_const  r26,0x00000000
    load_const  r27,0x00000000
    load_const  r28,0x00000000
    load_const  r29,0x00000000
    load_const  r30,0x00000000
    load_const  r31,0x00000000

//Before editing TLB let's initialize all SPR registers to default values by the spec
//Most of them have X-state after reset and therefore CPU model cannot work properly

    mtspr       SPR_LR, r0
    mtspr       SPR_CTR, r0
    mtcr        r0
    mtspr       SPR_XER, r0

    mtspr       SPR_SRR0, r0
    mtspr       SPR_SRR1, r0
    mtspr       SPR_CSRR0, r0
    mtspr       SPR_CSRR1, r0
    mtspr       SPR_MCSRR0, r0
    mtspr       SPR_MCSRR1, r0
    mtspr       SPR_MCSR_C, r1
    mtspr       SPR_ESR, r0
    mtspr       SPR_DCESR, r0
    mtspr       SPR_DEAR, r0

    mtspr       SPR_TCR, r0
    mtspr       SPR_TBL_W, r0
    mtspr       SPR_TBU_W, r0
    mtspr       SPR_DEC, r0
    mtspr       SPR_DECAR, r0
    load_const  r3, (0b1    << TIMER_TSR_EVW_i)\
                  | (0b1    << TIMER_TSR_WIS_i)\
                  | (0b1    << TIMER_TSR_DIS_i)\
                  | (0b1    << TIMER_TSR_FIS_i)
    mtspr       SPR_TSR_RC, r3

    mtspr       SPR_SPRG0, r0
    mtspr       SPR_SPRG1, r0
    mtspr       SPR_SPRG2, r0
    mtspr       SPR_SPRG3, r0
    mtspr       SPR_SPRG4, r0
    mtspr       SPR_SPRG5, r0
    mtspr       SPR_SPRG6, r0
    mtspr       SPR_SPRG7, r0
    mtspr       SPR_SPRG8, r0

    mtspr       SPR_IAC1, r0
    mtspr       SPR_IAC2, r0
    mtspr       SPR_IAC3, r0
    mtspr       SPR_IAC4, r0
    mtspr       SPR_DAC1, r0
    mtspr       SPR_DAC2, r0
    mtspr       SPR_DVC1, r0
    mtspr       SPR_DVC2, r0
    mtspr       SPR_DBCR1, r0
    mtspr       SPR_DBCR2, r0
    mtspr       SPR_DBDR, r0
    mtspr       SPR_RMPD, r0



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
DECLARE_ENUM_VAL( TB_RESULT_UNDEFINED,  0xFFFFFFFF )
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
    cmpw        cr0, r3, r4
    TEST_ASSERT(eq, cr0, "DBDR check failed")

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
