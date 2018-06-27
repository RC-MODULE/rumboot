
#include <platform/ppc470s/mmu.S.h>
#include <platform/test_event_asm.h>
#include <platform/test_assert.S.h>
#include <platform/devices.h>
#include <platform/arch/ppc/test_macro_asm.S.h>
#include <platform/arch/ppc/ppc_476fp_ctrl_fields.h>
#include <platform/regs/regs_mpic128.h>
#include <platform/regs/regs_plb6bc.h>

#define CVR3						r21,r22,r23		/* CheckValue Regs Triade */
#define P6BCA(OFFSET)				(DCR_PLB6_BC_BASE + (OFFSET))
#define SGD_NON_SYNTH_VALUE         0xFFFFFFF0

#define eq6							cr6*4+eq
#define eq7							cr7*4+eq


/* DCR register value checker */
.macro chk_sgdwr rE,rA,rR,raddress,reg_name
    /* load address */
    load_const	\rA,	\raddress
    /* load value */
    load_const	\rE,	SGD_NON_SYNTH_VALUE
    /* write value to SGDx */
    mtdcrx		\rA,	\rE
    /* read */
    mfdcrx		\rR,	\rA
    cmpw cr7,	\rR,	\rE
    crand		eq6,	eq6,	eq7
    TEST_ASSERT(eq,cr7,"DCR error: In \reg_name was expected value \exp_value (address \raddress) ")
.endm

.macro load_spr tR sprReg sprVal
	load_const	\tR,     \sprVal
	mtspr		\sprReg, \tR
.endm

.macro chk_value rA,rR,rE,vAddr,vExp,sName
    load_const	\rE,	\vExp
    load_const	\rA,	\vAddr
    mfdcrx		\rR,	\rA
    cmpw cr7,	\rR,	\rE
    crand		eq6,	eq6,	eq7
    TEST_ASSERT(eq,cr7,"DCRE: expected \reg_name = \exp_value (\raddress) ")
.endm

.global main

.section ".text","ax",@progbits

main:
	rumboot_putstring "Start testing...\n"

	load_const	r21,	0x00000000
	cmpwi cr6,	r21,	0x0000		/* General test status in cr6 */

    /* check all default values */
    chk_value CVR3,	P6BCA(PLB6BC_CR0),			PLB6_BC_CR0_VALUE,			"PLB6BC_CR0         "
    chk_value CVR3,	P6BCA(PLB6BC_PRI),			PLB6_BC_PRI_VALUE,			"PLB6BC_PRI         "
    chk_value CVR3,	P6BCA(PLB6BC_TSNOOP),		PLB6_BC_TSNOOP_VALUE,		"PLB6BC_TSNOOP      "
    chk_value CVR3,	P6BCA(PLB6BC_PAAM_WIN_EXT),	PLB6_BC_PAAM_WIN_EXT_VALUE,	"PLB6BC_PAAM_WIN_EXT"
    /* non-used segments must be written with value 0xFFFFFFF0. */
    chk_value CVR3,	P6BCA(PLB6BC_SGD1),			PLB6_BC_SGD1_VALUE,			"PLB6BC_SGD1        "
    chk_value CVR3,	P6BCA(PLB6BC_SGD2),			PLB6_BC_SGD2_VALUE,			"PLB6BC_SGD2        "
    chk_value CVR3,	P6BCA(PLB6BC_SGD3),			PLB6_BC_SGD3_VALUE			"PLB6BC_SGD3        "
    chk_sgdwr CVR3,	P6BCA(PLB6BC_SGD4),										"PLB6BC_SGD4        "
    chk_sgdwr CVR3,	P6BCA(PLB6BC_SGD5),										"PLB6BC_SGD5        "
    chk_sgdwr CVR3,	P6BCA(PLB6BC_SGD6),										"PLB6BC_SGD6        "
    chk_sgdwr CVR3,	P6BCA(PLB6BC_SGD7),										"PLB6BC_SGD7        "
    chk_value CVR3,	P6BCA(PLB6BC_ERR),          PLB6_BC_ERR_VALUE,			"PLB6BC_ERR         "
    chk_value CVR3,	P6BCA(PLB6BC_MSRSP),        PLB6_BC_MSRSP_VALUE,		"PLB6BC_MSRSP       "
    chk_value CVR3,	P6BCA(PLB6BC_HCPP),         PLB6_BC_HCPP_VALUE,			"PLB6BC_HCPP        "
    chk_value CVR3,	P6BCA(PLB6BC_HD),			PLB6_BC_HD_VALUE,			"PLB6BC_HD          "
    chk_value CVR3,	P6BCA(PLB6BC_QD),			PLB6_BC_QD_VALUE,			"PLB6BC_QD          "
    chk_value CVR3,	P6BCA(PLB6BC_SHD),			PLB6_BC_SHD_VALUE,			"PLB6BC_SHD         "
    chk_value CVR3,	P6BCA(PLB6BC_REVID),		PLB6_BC_REVID_VALUE,		"PLB6BC_REVID       "
    /* ADDR bus now is checked */

    /* check DATA I/O buses */
    rumboot_putstring "\nSliding '1' and '0' (low) ... \n <1> "
running_1_low:
    load_const	r20,	P6BCA(PLB6BC_SGD1)  /* DCR addr */
    load_const	r21,	0x00000010          /* begin val */
    load_const	r23, 	0x00000001          /* shift */
    load_const	r24, 	0x80000000          /* max val for compare */
loop_1_low:
	mtdcrx		r20,	r21
	mfdcrx		r22,	r20
	cmpw cr7,	r21,	r22
	crand		eq6,	eq6,	eq7
	TEST_ASSERT(eq, cr7, "PLB6 DCRDATA: Running one error (loop)")
	cmpw cr0,	r21,	r24
	/* beq running_1_hi */
	beq- cr0,	running_0_low
	slw			r21,	r21,	r23
	b loop_1_low

running_0_low:
	rumboot_putstring "\n<0> "
    load_const	r21,	0xFFFFFFF0		/* begin val */

    mtdcrx		r20,	r21
    mfdcrx		r22,	r20
    cmpw cr7,	r21,	r22
    crand		eq6,	eq6,	eq7
    TEST_ASSERT(eq,cr7,"PLB6 DCRDATA: Running zero error (init)")
    load_const	r21,	0xFFFFFFE0		/* begin val  */
    load_const	r24,	0x7FFFFFF0		/* finish val */
loop_0_low:
    mtdcrx		r20,	r21
    mfdcrx		r22,	r20
    cmpw cr7,	r21,	r22
    crand		eq6,	eq6,	eq7
    TEST_ASSERT(eq,cr7,"PLB6 DCRDATA: Running zero error (loop)")
loop_0_low_ok:
    cmpw cr0,	r21,	r24
    /* beq running_0_hi */
    beq- cr0,	loop_0_low_end
    slw			r21,	r21,	r23
    ori			r21,	r21,	0x0010
    b loop_0_low
loop_0_low_end:

    rumboot_putstring "\nSliding '1' and '0' (high) ... \n <1> "
running_1_high:
    load_const	r20,	P6BCA(PLB6BC_HCPP)  /* DCR addr */
    load_const	r21,	0x00000001          /* begin val */
    load_const	r24, 	0x00000008          /* max val for compare */
	load_const	r25,	0x0000000F			/* bits mask */
loop_1_high:
	ori			r26,	r21,	0x0400
	mtdcrx		r20,	r26
	mfdcrx		r22,	r20
	cmpw cr7,	r26,	r22
	crand		eq6,	eq6,	eq7
	TEST_ASSERT(eq, cr7, "PLB6 DCRDATA: Running one (high) error (loop)")
	cmpw cr0,	r21,	r24
	/* beq running_1_hi */
	beq- cr0,	running_0_high
	slw			r21,	r21,	r23
	b loop_1_high

running_0_high:
	rumboot_putstring "\n<0> "
    load_const	r21,	0x03FFFFFF		/* begin val */

    mtdcrx		r20,	r21
    mfdcrx		r22,	r20
    cmpw cr7,	r21,	r22
    crand		eq6,	eq6,	eq7
    TEST_ASSERT(eq,cr7,"PLB6 DCRDATA: Running zero (high) error (init)")
    load_const	r21,	0x0000000E		/* begin val  */
    load_const	r24,	0x00000007		/* finish val */
loop_0_high:
	ori			r26,	r21,	0x0400
	mtdcrx		r20,	r26
    mfdcrx		r22,	r20
    cmpw cr7,	r26,	r22
    crand		eq6,	eq6,	eq7
    TEST_ASSERT(eq,cr7,"PLB6 DCRDATA: Running zero (high) error (loop)")
    cmpw cr0,	r21,	r24
    /* beq running_0_hi */
    beq- cr0,	loop_0_high_end
    slw			r21,	r21,	r23
    ori			r21,	r21,	0x0001
    and			r21,	r21,	r25
    b loop_0_high
loop_0_high_end:

	beq+ cr6,	test_ok
	b			test_error

test_ok:
    rumboot_putstring "\nTEST OK\n"
	test_event EVENT_OK
    exit RESULT_OK

test_error:
	rumboot_putstring "\nTEST ERROR\n"
	test_event  EVENT_ERROR
    exit RESULT_ERROR



