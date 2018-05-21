#include <platform/oi10/platform/test_assert.S.h>
#include <platform/oi10/platform/arch/ppc/ppc_476fp_asm.h>
#include <platform/oi10/platform/test_event_asm.h>


#define CONST           0x00007FFF

.macro GPR_LOAD_CONST reg, value
    load_const \reg, \value
    cmpi cr7, 0, \reg, \value
    TEST_ASSERT(eq, cr7, "GPR\reg load value from const: Expected \"\value\"")
    bne cr7, error
.endm

.macro GPR_LOAD_MEM reg_test, reg_temp, address, value
    lis \reg_temp, \address@h
    ori \reg_temp, \reg_temp, \address@l
    lwz \reg_test, 0(\reg_temp)
    cmpi cr7, 0, \reg_test, \value
    TEST_ASSERT(eq, cr7, "GPR\reg_test load value from memory: Expected \"\value\"")
    bne cr7, error
.endm

.macro SPRG_LOAD reg, value
    load_const r6, \value
    mtspr \reg, r6
    mfspr r7, \reg
    cmp cr7, 0, r7, r6
    TEST_ASSERT(eq, cr7, "SPRG\reg load value: Expected \"\value\"")
    bne cr7, error
.endm

.section ".init.rodata","a",@progbits
.global value
value:
    .long 0x00007FFF


.section ".text","ax",@progbits

.global image_start
.global main

main:
image_start:
check_gpr_lis_ori:
	mr r31,	r1 /* AG: Save stack ptr */
    GPR_LOAD_CONST r0, CONST
    GPR_LOAD_CONST r1, CONST
    GPR_LOAD_CONST r2, CONST
    GPR_LOAD_CONST r3, CONST
    GPR_LOAD_CONST r4, CONST
    GPR_LOAD_CONST r5, CONST
    GPR_LOAD_CONST r6, CONST
    GPR_LOAD_CONST r7, CONST
    GPR_LOAD_CONST r8, CONST
    GPR_LOAD_CONST r9, CONST
    GPR_LOAD_CONST r10, CONST
    GPR_LOAD_CONST r11, CONST
    GPR_LOAD_CONST r12, CONST
    GPR_LOAD_CONST r13, CONST
    GPR_LOAD_CONST r14, CONST
    GPR_LOAD_CONST r15, CONST
    GPR_LOAD_CONST r16, CONST
    GPR_LOAD_CONST r17, CONST
    GPR_LOAD_CONST r18, CONST
    GPR_LOAD_CONST r19, CONST
    GPR_LOAD_CONST r20, CONST
    GPR_LOAD_CONST r21, CONST
    GPR_LOAD_CONST r22, CONST
    GPR_LOAD_CONST r23, CONST
    GPR_LOAD_CONST r24, CONST
    GPR_LOAD_CONST r25, CONST
    GPR_LOAD_CONST r26, CONST
    GPR_LOAD_CONST r27, CONST
    GPR_LOAD_CONST r28, CONST
    GPR_LOAD_CONST r29, CONST
    GPR_LOAD_CONST r30, CONST
    mr r1,	r31 /* AG: Restore stack ptr */
    GPR_LOAD_CONST r31, CONST

check_gpr_lw:
	mr r30, r1 /* AG: Save stack ptr */
    GPR_LOAD_MEM r0, r31, value, CONST
    GPR_LOAD_MEM r1, r31, value, CONST
    GPR_LOAD_MEM r2, r31, value, CONST
    GPR_LOAD_MEM r3, r31, value, CONST
    GPR_LOAD_MEM r4, r31, value, CONST
    GPR_LOAD_MEM r5, r31, value, CONST
    GPR_LOAD_MEM r6, r31, value, CONST
    GPR_LOAD_MEM r7, r31, value, CONST
    GPR_LOAD_MEM r8, r31, value, CONST
    GPR_LOAD_MEM r9, r31, value, CONST
    GPR_LOAD_MEM r10, r31, value, CONST
    GPR_LOAD_MEM r11, r31, value, CONST
    GPR_LOAD_MEM r12, r31, value, CONST
    GPR_LOAD_MEM r13, r31, value, CONST
    GPR_LOAD_MEM r14, r31, value, CONST
    GPR_LOAD_MEM r15, r31, value, CONST
    GPR_LOAD_MEM r16, r31, value, CONST
    GPR_LOAD_MEM r17, r31, value, CONST
    GPR_LOAD_MEM r18, r31, value, CONST
    GPR_LOAD_MEM r19, r31, value, CONST
    GPR_LOAD_MEM r20, r31, value, CONST
    GPR_LOAD_MEM r21, r31, value, CONST
    GPR_LOAD_MEM r22, r31, value, CONST
    GPR_LOAD_MEM r23, r31, value, CONST
    GPR_LOAD_MEM r24, r31, value, CONST
    GPR_LOAD_MEM r25, r31, value, CONST
    GPR_LOAD_MEM r26, r31, value, CONST
    GPR_LOAD_MEM r27, r31, value, CONST
    GPR_LOAD_MEM r28, r31, value, CONST
    GPR_LOAD_MEM r29, r31, value, CONST
    mr r1, r30 /* AG: Restore stack ptr */
    GPR_LOAD_MEM r30, r31, value, CONST
    GPR_LOAD_MEM r31, r30, value, CONST


is_supervisor_mode:
    load_const r6, 0x00020000
    mfmsr r7
    and r3, r7, r6
    cmpi cr7, 0, r3, 0x0
    TEST_ASSERT(eq, cr7, "Supervisor mode disabled in CPU")
    bne  cr7, error

check_sprg_ppc:
    SPRG_LOAD SPR_SPRG0, CONST
    SPRG_LOAD SPR_SPRG1, CONST
    SPRG_LOAD SPR_SPRG2, CONST
    SPRG_LOAD SPR_SPRG3, CONST
    SPRG_LOAD SPR_SPRG4, CONST
    SPRG_LOAD SPR_SPRG5, CONST
    SPRG_LOAD SPR_SPRG6, CONST
    SPRG_LOAD SPR_SPRG7, CONST
    SPRG_LOAD SPR_SPRG8, CONST

test_ok:
    test_event EVENT_OK
    load_const r3, 0x00
    rumboot_putstring "\nTEST OK\n"
    b finish

error:
    test_event EVENT_ERROR
    load_const r3, 0x01
    rumboot_putstring "\nTEST ERROR\n"

finish:
    blr


