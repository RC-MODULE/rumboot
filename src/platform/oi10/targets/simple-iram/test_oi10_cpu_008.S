
#include <platform/ppc470s/mmu.S.h>
#include <platform/test_event_asm.h>
#include <platform/test_assert.S.h>
#include <platform/devices.h>
#include <platform/arch/ppc/test_macro_asm.S.h>
#include <platform/arch/ppc/ppc_476fp_ctrl_fields.h>
#include <platform/regs/regs_mpic128.h>
#include <platform/regs/regs_plb6bc.h>

#define R_ADDR                  r20     /* Address of DCR reg    */
#define R_EXPV                  r21     /* Expected value        */
#define R_DATA                  r22     /* Readed data for check */
#define R_SHFT                  r23     /* Shift value           */
#define R_TERM                  r24     /* Termination template  */
#define R_FILL                  r25     /* Fill after shift mask */
#define R_RMSK                  r26     /* Read mask             */
#define R_WMSK                  r27     /* Write mask            */
#define R_BACK                  r28     /* Backup DCR register   */

#define CVR3						r20,r21,r22		/* CheckValue Regs Triade */
#define P6BCA(OFFSET)				(DCR_PLB6_BC_BASE + (OFFSET))
#define SGD_NON_SYNTH_VALUE         0xFFFFFFF0

#define eq6							cr6*4+eq
#define eq7							cr7*4+eq

#define PLB6_BC_CR0_STU             (0b1 << PLB6BC_CR0_BC_DSBS_i      )\
                                  | (0b1 << PLB6BC_CR0_BC_MASK_MSRSP_i)\
                                  | (0b1 << PLB6BC_CR0_BC_MASK_SRSNZ_i)

#define PLB6_BC_SGD1_STU            0x00000010
#define PLB6_BC_SGD2_STU            0x00000010
#define PLB6_BC_SGD3_STU            0x00000020
#define PLB6_BC_HCPP_VALUE          PLB6BC_HCPP_4K

/* DCR register value checker */
.macro chk_sgdwr rA,rE,rR,vAddr,sName
    /* load address */
    load_const	\rA,	\vAddr
    /* load value */
    load_const	\rE,	SGD_NON_SYNTH_VALUE
    /* write value to SGDx */
    mtdcrx		\rA,	\rE
    /* read */
    mfdcrx		\rR,	\rA
    cmpw cr7,	\rR,	\rE
    crand		eq6,	eq6,	eq7
    TEST_ASSERT(eq,cr7,"DCR error: In \sName was expected value \vExp (address: \vAddr) ")
.endm

.macro chk_value rA,rE,rR,vAddr,vExp,sName
    load_const	\rE,	\vExp
    load_const	\rA,	\vAddr
    mfdcrx		\rR,	\rA
    cmpw cr7,	\rR,	\rE
    crand		eq6,	eq6,	eq7
    TEST_ASSERT(eq,cr7,"DCRE: expected \sName = \vExp (address: \vAddr) ")
.endm

.global main

.section ".text","ax",@progbits

main:
	rumboot_putstring "Start testing...\n"

	load_const	r20,	0x00000000
	cmpwi cr6,	r20,	0x0000		/* General test status in cr6 */

    /* check all default values */
    chk_value CVR3,	P6BCA(PLB6BC_CR0),			PLB6_BC_CR0_STU,            "PLB6BC_CR0         "
    chk_value CVR3,	P6BCA(PLB6BC_PRI),			PLB6_BC_PRI_VALUE,			"PLB6BC_PRI         "
    chk_value CVR3,	P6BCA(PLB6BC_TSNOOP),		PLB6_BC_TSNOOP_VALUE,		"PLB6BC_TSNOOP      "
    chk_value CVR3,	P6BCA(PLB6BC_PAAM_WIN_EXT),	PLB6_BC_PAAM_WIN_EXT_VALUE,	"PLB6BC_PAAM_WIN_EXT"
    /* non-used segments must be written with value 0xFFFFFFF0. */
    chk_value CVR3,	P6BCA(PLB6BC_SGD1),			PLB6_BC_SGD1_STU,       	"PLB6BC_SGD1        "
    chk_value CVR3,	P6BCA(PLB6BC_SGD2),			PLB6_BC_SGD2_STU,           "PLB6BC_SGD2        "
    chk_value CVR3,	P6BCA(PLB6BC_SGD3),			PLB6_BC_SGD3_STU,           "PLB6BC_SGD3        "
    chk_sgdwr CVR3,	P6BCA(PLB6BC_SGD4),										"PLB6BC_SGD4        "
    chk_sgdwr CVR3,	P6BCA(PLB6BC_SGD5),										"PLB6BC_SGD5        "
    chk_sgdwr CVR3,	P6BCA(PLB6BC_SGD6),										"PLB6BC_SGD6        "
    chk_sgdwr CVR3,	P6BCA(PLB6BC_SGD7),										"PLB6BC_SGD7        "
//  chk_value CVR3,	P6BCA(PLB6BC_ERR),          PLB6_BC_ERR_VALUE,			"PLB6BC_ERR         "
    chk_value CVR3,	P6BCA(PLB6BC_MSRSP),        PLB6_BC_MSRSP_VALUE,		"PLB6BC_MSRSP       "
    chk_value CVR3,	P6BCA(PLB6BC_HCPP),         PLB6_BC_HCPP_VALUE,			"PLB6BC_HCPP        "
    chk_value CVR3,	P6BCA(PLB6BC_HD),			PLB6_BC_HD_VALUE,			"PLB6BC_HD          "
    chk_value CVR3,	P6BCA(PLB6BC_QD),			PLB6_BC_QD_VALUE,			"PLB6BC_QD          "
    chk_value CVR3,	P6BCA(PLB6BC_SHD),			PLB6_BC_SHD_VALUE,			"PLB6BC_SHD         "
    chk_value CVR3,	P6BCA(PLB6BC_REVID),		PLB6_BC_REVID_VALUE,		"PLB6BC_REVID       "
    /* ADDR bus now is checked */

    rumboot_putstring   "PLB6BC_ERR="
    load_const  R_ADDR, P6BCA(PLB6BC_ERR)
    mfdcrx      r3,     R_ADDR
    mtdcrx      R_ADDR, r3
    rumboot_puthex      r3

    /* check DATA I/O buses */
running_1_low:
    load_const  R_SHFT, 0x00000001
 // b           running_1_high
    rumboot_putstring   "Check running <1> and <0> PLB6BC_PRI [0:15] ...\n"
    rumboot_putstring   "Running <1> (init)\n"
    load_const  R_TERM, 0x80000000
    load_const  R_EXPV, 0x00000000
    load_const  R_RMSK, 0xFFFF0000
    load_const  R_WMSK, 0xFFFF0000
    load_const  R_ADDR, P6BCA(PLB6BC_HD)
    mtdcrx      R_ADDR, R_WMSK              /* Clear hang detect */
    load_const  R_ADDR, P6BCA(PLB6BC_PRI)
    mfdcrx      R_BACK, R_ADDR
    mtdcrx      R_ADDR, R_EXPV
    mfdcrx      R_DATA, R_ADDR
    cmpw cr7,   R_EXPV, R_DATA
    crand       eq6,    eq6,    eq7
    TEST_ASSERT(eq,cr7,"PLB6 DCRDATA: Running <1> [0:15] error (init)")
    load_const  R_EXPV, 0x00010000
    rumboot_putstring   "Running <1> [0:15] ...\n"
loop_1_low:
    mtdcrx      R_ADDR, R_EXPV
    mfdcrx      R_DATA, R_ADDR
    cmpw cr7,   R_EXPV, R_DATA
    mr          r3,     R_EXPV
    rumboot_puthex      r3
    mr          r3,     R_DATA
    rumboot_puthex      r3
    crand		eq6,	eq6,	eq7
    TEST_ASSERT(eq, cr7, "PLB6 DCRDATA: Running <1> [0:15] error (loop)")
    cmpw cr0,   R_EXPV, R_TERM
    beq- cr0,   running_0_low
    slw         R_EXPV, R_EXPV, R_SHFT
    b           loop_1_low

running_0_low:
    rumboot_putstring   "Running <0> [0:15] (init)\n"
    load_const  R_FILL, 0x00010000
    load_const  R_TERM, 0x7FFF0000
    load_const  R_EXPV, 0xFFFF0000
    mtdcrx      R_ADDR, R_EXPV
    mfdcrx      R_DATA, R_ADDR
    cmpw cr7,   R_EXPV, R_DATA
    crand       eq6,    eq6,    eq7
    TEST_ASSERT(eq,cr7,"PLB6 DCRDATA: Running <0> [0:15] error (init)")
    load_const  R_EXPV, 0xFFFE0000
    rumboot_putstring   "Running <0> ...\n"
loop_0_low:
    and         R_EXPV, R_EXPV, R_WMSK
    mtdcrx      R_ADDR, R_EXPV
    mfdcrx      R_DATA, R_ADDR
    and         R_DATA, R_DATA, R_RMSK
    cmpw cr7,   R_EXPV, R_DATA
    crand       eq6,	eq6,	eq7
    TEST_ASSERT(eq,cr7,"PLB6 DCRDATA: Running <0> [0:15] error (loop)")
    cmpw cr0,   R_EXPV, R_TERM
    beq- cr0,   loop_0_low_end
    slw         R_EXPV, R_EXPV, R_SHFT
    or          R_EXPV, R_EXPV, R_FILL
    b           loop_0_low
loop_0_low_end:
    mtdcrx      R_ADDR, R_BACK

running_1_high:
    rumboot_putstring   "Check running <1> and <0> PLB6BC_HCPP [16:31] ...\n"
    rumboot_putstring   "Running <1> (init)\n"
    load_const  R_ADDR, P6BCA(PLB6BC_HCPP)
    load_const  R_TERM, 0x00008000
    load_const  R_EXPV, 0x00000000
    load_const  R_RMSK, 0x0000FFFF
    load_const  R_WMSK, 0x0000FFFF
    mfdcrx      R_BACK, R_ADDR
    mtdcrx      R_ADDR, R_EXPV
    mfdcrx      R_DATA, R_ADDR
    cmpw cr7,   R_EXPV, R_DATA
    crand       eq6,    eq6,    eq7
    TEST_ASSERT(eq,cr7,"PLB6 DCRDATA: Running <1> [16:31] error (init)")
    load_const  R_EXPV, 0x00000001
    rumboot_putstring   "Running <1> ...\n"
loop_1_high:
    and         R_EXPV, R_EXPV, R_WMSK
    mtdcrx      R_ADDR, R_EXPV
    mfdcrx      R_DATA, R_ADDR
    and         R_DATA, R_DATA, R_RMSK
    cmpw cr7,   R_EXPV, R_DATA
    crand       eq6,    eq6,    eq7
    TEST_ASSERT(eq, cr7, "PLB6 DCRDATA: Running <1> [16:31] error (loop)")
    cmpw cr0,   R_EXPV, R_TERM
    beq- cr0,   running_0_high
    slw         R_EXPV, R_EXPV, R_SHFT
    b           loop_1_high

running_0_high:
    rumboot_putstring   "Running <0> [16:31] (init)\n"
    load_const  R_FILL, 0x00000001
    load_const  R_TERM, 0x00007FFF
    load_const  R_EXPV, 0x0000FFFF
    mtdcrx      R_ADDR, R_EXPV
    mfdcrx      R_DATA, R_ADDR
    cmpw cr7,   R_EXPV, R_DATA
    crand       eq6,    eq6,    eq7
    TEST_ASSERT(eq,cr7,"PLB6 DCRDATA: Running <0> [16:31] error (init)")
    load_const  R_EXPV, 0x0000FFFE
    rumboot_putstring   "Running <0> ...\n"
loop_0_high:
    and         R_EXPV, R_EXPV, R_WMSK
    mtdcrx      R_ADDR, R_EXPV
    mfdcrx      R_DATA, R_ADDR
    and         R_DATA, R_DATA, R_RMSK
    cmpw cr7,   R_EXPV, R_DATA
    crand       eq6,    eq6,    eq7
    TEST_ASSERT(eq,cr7,"PLB6 DCRDATA: Running <0> [16:31] error (loop)")
    cmpw cr0,   R_EXPV, R_TERM
    beq- cr0,   test_end
    slw         R_EXPV, R_EXPV, R_SHFT
    or          R_EXPV, R_EXPV, R_FILL
    b loop_0_high

test_end:
    mtdcrx      R_ADDR, R_BACK
    bne- cr6,   test_error

test_ok:
    rumboot_putstring "\nTEST OK\n"
	test_event EVENT_OK
    exit RESULT_OK

test_error:
	rumboot_putstring "\nTEST ERROR\n"
	test_event  EVENT_ERROR
    exit RESULT_ERROR



