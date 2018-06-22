#include <platform/test_assert.S.h>
#include <platform/test_event_asm.h>
#include <platform/trace.S.h>
#include <platform/arch/ppc/ppc_476fp_asm.h>
#include <platform/arch/ppc/ppc_476fp_ctrl_fields.h>
#include <platform/arch/ppc/ppc_476fp_itrpt_fields.h>


/* BC - Base Constant */
#define GENCONST1(BC)			(0x00007F00 + (BC))
#define GENCONST2(BC)			(0x00005F00 + (BC))

.macro GPR_LOAD_CONST reg, value
    li				\reg,		\value
    cmpwi cr7,		\reg,		\value
    TEST_ASSERT(eq, cr7, "GPR\reg load value from const: Expected \"\value\"")
    bne-  cr7,		test_error
.endm

.macro GPR_LOAD_MEM reg_test, reg_temp, address, idx, value
	load_addr		\reg_temp,	\address
    lwz				\reg_test,	((\idx)*4)(\reg_temp)
    cmpwi cr7,		\reg_test,	\value
    TEST_ASSERT(eq, cr7, "GPR\reg_test load value from memory: Expected \"\value\"")
    bne-  cr7,		test_error
.endm

.macro SPRG_LOAD reg, value
    li				r6,			\value
    mtspr			\reg,		r6
    mfspr			r7,			\reg
    cmpw  cr7,		r7,			r6
    TEST_ASSERT(eq, cr7, "SPRG\reg load value: Expected \"\value\"")
    bne-  cr7,		test_error
.endm


.section ".text","ax",@progbits

.global main

main:
check_gpr_lis_ori:
	.align 2
	mtspr			SPR_SPRG7,		r1				/* Save stack ptr */
	rumboot_putstring "check_gpr_lis_ori\n"
    GPR_LOAD_CONST	r0,				GENCONST1( 0)
    GPR_LOAD_CONST	r1,				GENCONST1( 1)
    GPR_LOAD_CONST	r2,				GENCONST1( 2)
    GPR_LOAD_CONST	r3,				GENCONST1( 3)
    GPR_LOAD_CONST	r4,				GENCONST1( 4)
    GPR_LOAD_CONST	r5,				GENCONST1( 5)
    GPR_LOAD_CONST	r6,				GENCONST1( 6)
    GPR_LOAD_CONST	r7,				GENCONST1( 7)
    GPR_LOAD_CONST	r8,				GENCONST1( 8)
    GPR_LOAD_CONST	r9,				GENCONST1( 9)
    GPR_LOAD_CONST	r10,			GENCONST1(10)
    GPR_LOAD_CONST	r11,			GENCONST1(11)
    GPR_LOAD_CONST	r12,			GENCONST1(12)
    GPR_LOAD_CONST	r13,			GENCONST1(13)
    GPR_LOAD_CONST	r14,			GENCONST1(14)
    GPR_LOAD_CONST	r15,			GENCONST1(15)
    GPR_LOAD_CONST	r16,			GENCONST1(16)
    GPR_LOAD_CONST	r17,			GENCONST1(17)
    GPR_LOAD_CONST	r18,			GENCONST1(18)
    GPR_LOAD_CONST	r19,			GENCONST1(19)
    GPR_LOAD_CONST	r20,			GENCONST1(20)
    GPR_LOAD_CONST	r21,			GENCONST1(21)
    GPR_LOAD_CONST	r22,			GENCONST1(22)
    GPR_LOAD_CONST	r23,			GENCONST1(23)
    GPR_LOAD_CONST	r24,			GENCONST1(24)
    GPR_LOAD_CONST	r25,			GENCONST1(25)
    GPR_LOAD_CONST	r26,			GENCONST1(26)
    GPR_LOAD_CONST	r27,			GENCONST1(27)
    GPR_LOAD_CONST	r28,			GENCONST1(28)
    GPR_LOAD_CONST	r29,			GENCONST1(29)
    GPR_LOAD_CONST	r30,			GENCONST1(30)
    GPR_LOAD_CONST	r31,			GENCONST1(31)

check_gpr_lw:
	rumboot_putstring "check_gpr_lw\n"
    GPR_LOAD_MEM	r0,		r31, mem_tbl,  0, GENCONST2( 0)
    GPR_LOAD_MEM	r1,		r31, mem_tbl,  1, GENCONST2( 1)
    GPR_LOAD_MEM	r2,		r31, mem_tbl,  2, GENCONST2( 2)
    GPR_LOAD_MEM	r3,		r31, mem_tbl,  3, GENCONST2( 3)
    GPR_LOAD_MEM	r4,		r31, mem_tbl,  4, GENCONST2( 4)
    GPR_LOAD_MEM	r5,		r31, mem_tbl,  5, GENCONST2( 5)
    GPR_LOAD_MEM	r6,		r31, mem_tbl,  6, GENCONST2( 6)
    GPR_LOAD_MEM	r7,		r31, mem_tbl,  7, GENCONST2( 7)
    GPR_LOAD_MEM	r8,		r31, mem_tbl,  8, GENCONST2( 8)
    GPR_LOAD_MEM	r9,		r31, mem_tbl,  9, GENCONST2( 9)
    GPR_LOAD_MEM	r10,	r31, mem_tbl, 10, GENCONST2(10)
    GPR_LOAD_MEM	r11,	r31, mem_tbl, 11, GENCONST2(11)
    GPR_LOAD_MEM	r12,	r31, mem_tbl, 12, GENCONST2(12)
    GPR_LOAD_MEM	r13,	r31, mem_tbl, 13, GENCONST2(13)
    GPR_LOAD_MEM	r14,	r31, mem_tbl, 14, GENCONST2(14)
    GPR_LOAD_MEM	r15,	r31, mem_tbl, 15, GENCONST2(15)
    GPR_LOAD_MEM	r16,	r31, mem_tbl, 16, GENCONST2(16)
    GPR_LOAD_MEM	r17,	r31, mem_tbl, 17, GENCONST2(17)
    GPR_LOAD_MEM	r18,	r31, mem_tbl, 18, GENCONST2(18)
    GPR_LOAD_MEM	r19,	r31, mem_tbl, 19, GENCONST2(19)
    GPR_LOAD_MEM	r20,	r31, mem_tbl, 20, GENCONST2(20)
    GPR_LOAD_MEM	r21,	r31, mem_tbl, 21, GENCONST2(21)
    GPR_LOAD_MEM	r22,	r31, mem_tbl, 22, GENCONST2(22)
    GPR_LOAD_MEM	r23,	r31, mem_tbl, 23, GENCONST2(23)
    GPR_LOAD_MEM	r24,	r31, mem_tbl, 24, GENCONST2(24)
    GPR_LOAD_MEM	r25,	r31, mem_tbl, 25, GENCONST2(25)
    GPR_LOAD_MEM	r26,	r31, mem_tbl, 26, GENCONST2(26)
    GPR_LOAD_MEM	r27,	r31, mem_tbl, 27, GENCONST2(27)
    GPR_LOAD_MEM	r28,	r31, mem_tbl, 28, GENCONST2(28)
    GPR_LOAD_MEM	r29,	r31, mem_tbl, 29, GENCONST2(29)
    GPR_LOAD_MEM	r30,	r31, mem_tbl, 30, GENCONST2(30)
    GPR_LOAD_MEM	r31,	r30, mem_tbl, 31, GENCONST2(31)

check_sprg_ppc:
	rumboot_putstring "check_sprg_ppc\n"
	SPRG_LOAD SPR_SPRG0, 0x5FA0
/*	SPRG_LOAD SPR_SPRG1, 0x5EA1 */
	SPRG_LOAD SPR_SPRG2, 0x5DA2
	SPRG_LOAD SPR_SPRG3, 0x5CA3
	SPRG_LOAD SPR_SPRG4, 0x5BA4
	SPRG_LOAD SPR_SPRG5, 0x5AA5
	SPRG_LOAD SPR_SPRG6, 0x59A6
	SPRG_LOAD SPR_SPRG7, 0x58A7
	SPRG_LOAD SPR_SPRG8, 0x57A8

test_ok:
    rumboot_putstring "\nTEST OK\n"
    test_event		EVENT_OK
    load_const	r3,	RESULT_OK
    b finish

test_error:
    rumboot_putstring "\nTEST ERROR\n"
    test_event		EVENT_ERROR
    load_const	r3,	RESULT_ERROR

finish:
    mfspr		r1,	SPR_SPRG7		/* Restore stack ptr */
    blr


mem_tbl:
	.align 2
    .long GENCONST2( 0)
    .long GENCONST2( 1)
    .long GENCONST2( 2)
    .long GENCONST2( 3)
    .long GENCONST2( 4)
    .long GENCONST2( 5)
    .long GENCONST2( 6)
    .long GENCONST2( 7)
    .long GENCONST2( 8)
    .long GENCONST2( 9)
    .long GENCONST2(10)
    .long GENCONST2(11)
    .long GENCONST2(12)
    .long GENCONST2(13)
    .long GENCONST2(14)
    .long GENCONST2(15)
    .long GENCONST2(16)
    .long GENCONST2(17)
    .long GENCONST2(18)
    .long GENCONST2(19)
    .long GENCONST2(20)
    .long GENCONST2(21)
    .long GENCONST2(22)
    .long GENCONST2(23)
    .long GENCONST2(24)
    .long GENCONST2(25)
    .long GENCONST2(26)
    .long GENCONST2(27)
    .long GENCONST2(28)
    .long GENCONST2(29)
    .long GENCONST2(30)
    .long GENCONST2(31)


