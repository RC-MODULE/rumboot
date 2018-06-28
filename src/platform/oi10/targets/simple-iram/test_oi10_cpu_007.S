#include <platform/trace.S.h>
#include <platform/test_assert.S.h>
#include <platform/test_event_asm.h>
#include <platform/devices.h>

#define DEBUG_DCR_TEST
#include <platform/arch/ppc/test_macro_asm.S.h>

/* MPIC Registers */
#include <platform/regs/regs_mpic128.h>

#define CVR3				r21,r22,r23
#define MPIC128A(OFFSET)	(DCR_MPIC128_BASE + (OFFSET))

#define VPx_REGS_COUNT		MPIC128_VPx_REGS_COUNT
#define DSTx_REGS_COUNT		MPIC128_DSTx_REGS_COUNT
#define VPx_REGS_STEP		MPIC128_VP1  - MPIC128_VP0
#define DSTx_REGS_STEP		MPIC128_DST1 - MPIC128_DST0

#define eq6                 cr6*4+eq
#define eq7                 cr7*4+eq

/*	prefix r=register, v=value, s=string
	rA		- reg for (dcr) address;
	rR		- reg for read data;
	rE		- reg for expecting value;
	vAddr	- DCR address value;
	vC		- Count of I/O-regs;
	vI		- Increment of addr;
	vExp	- Expected value;
	sName	- User friendly name for debug. */
.macro check_multiple_values sName,rA,rR,rE,vAddr,vC,vI,vExp
	rumboot_putstring	"\sName \t-> "
	load_const	\rA,	\vAddr
	load_const	\rR,	\vC
	load_const	\rE,	\vExp
	mtctr		\rR
71:
	putchar		'*'
	mfdcrx		\rR,	\rA
	cmpw cr7,	\rR,	\rE
    crand       eq6,    eq6,    eq7
	TEST_ASSERT(eq,cr7,"Expected: \sName = \vExp")
#ifdef STOP_ON_ERROR
    bne- cr6,   test_error
#endif
	addi		\rA,	\rA,	\vI
	bdnz		71b
	putchar		'\n'
.endm

.macro chk_value rA,rR,rE,vAddr,vExp,sName
    rumboot_putstring   "Check \sName ... \t"
    load_const  \rE,    \vExp
    load_const  \rA,    \vAddr
    mfdcrx      \rR,    \rA
    cmpw cr7,   \rR,    \rE
    crand       eq6,    eq6,    eq7
    TEST_ASSERT(eq,cr7,"DCRE: expected \sName = \vExp (address: \vAddr) ")
    bne- cr7,   72f
    rumboot_putstring "ok!\n"
72:
#ifdef STOP_ON_ERROR
    bne- cr6,   test_error
#endif
.endm


.section ".text","ax",@progbits

.global main

main:
    /* set main ok status */
    xor         r20, r20, r20
    cmpwi cr6,  r20, 0x0000
    /* reset mpic */
    load_const	r25, MPIC128_R_BIT
    load_const	r24, MPIC128A(MPIC128_GCF0)
    mtdcrx		r24, r25

read_MPICx_GCF0_R_BIT:
    mfdcrx		r26, r24
    cmpw cr0,	r26, r24
    beq+ cr0,	read_MPICx_GCF0_R_BIT

check_cpu_ppc_mpic_3:
    rumboot_putstring "Start checking default values...\n"
    chk_value CVR3, MPIC128A(MPIC128_REVID ), MPIC128_REVID_VALUE,  "MPIC128_REVID "
    chk_value CVR3, MPIC128A(MPIC128_VIR0  ), MPIC128_VIR0_VALUE,   "MPIC128_VP0   "
    chk_value CVR3, MPIC128A(MPIC128_CTP0  ), MPIC128_CTP0_VALUE,   "MPIC128_CTP0  "
    chk_value CVR3, MPIC128A(MPIC128_CTP1  ), MPIC128_CTP1_VALUE,   "MPIC128_CTP1  "
    chk_value CVR3, MPIC128A(MPIC128_CTP2  ), MPIC128_CTP2_VALUE,   "MPIC128_CTP2  "
    chk_value CVR3, MPIC128A(MPIC128_CTP3  ), MPIC128_CTP3_VALUE,   "MPIC128_CTP3  "
    chk_value CVR3, MPIC128A(MPIC128_NCIAR0), MPIC128_NCIAR0_VALUE, "MPIC128_NCIAR0"
    chk_value CVR3, MPIC128A(MPIC128_NCIAR1), MPIC128_NCIAR1_VALUE, "MPIC128_NCIAR1"
    chk_value CVR3, MPIC128A(MPIC128_NCIAR2), MPIC128_NCIAR2_VALUE, "MPIC128_NCIAR2"
    chk_value CVR3, MPIC128A(MPIC128_NCIAR3), MPIC128_NCIAR3_VALUE, "MPIC128_NCIAR3"
    chk_value CVR3, MPIC128A(MPIC128_NCEOI0), MPIC128_NCEOI0_VALUE, "MPIC128_NCEOI0"
    chk_value CVR3, MPIC128A(MPIC128_NCEOI1), MPIC128_NCEOI1_VALUE, "MPIC128_NCEOI1"
    chk_value CVR3, MPIC128A(MPIC128_NCEOI2), MPIC128_NCEOI2_VALUE, "MPIC128_NCEOI2"
    chk_value CVR3, MPIC128A(MPIC128_NCEOI3), MPIC128_NCEOI3_VALUE, "MPIC128_NCEOI3"
    chk_value CVR3, MPIC128A(MPIC128_CIAR0 ), MPIC128_CIAR0_VALUE,  "MPIC128_CIAR0 "
    chk_value CVR3, MPIC128A(MPIC128_CIAR1 ), MPIC128_CIAR1_VALUE,  "MPIC128_CIAR1 "
    chk_value CVR3, MPIC128A(MPIC128_CIAR2 ), MPIC128_CIAR2_VALUE,  "MPIC128_CIAR2 "
    chk_value CVR3, MPIC128A(MPIC128_CIAR3 ), MPIC128_CIAR3_VALUE,  "MPIC128_CIAR3 "
    chk_value CVR3, MPIC128A(MPIC128_CEOI0 ), MPIC128_CEOI0_VALUE,  "MPIC128_CEOI0 "
    chk_value CVR3, MPIC128A(MPIC128_CEOI1 ), MPIC128_CEOI1_VALUE,  "MPIC128_CEOI1 "
    chk_value CVR3, MPIC128A(MPIC128_CEOI2 ), MPIC128_CEOI2_VALUE,  "MPIC128_CEOI2 "
    chk_value CVR3, MPIC128A(MPIC128_CEOI3 ), MPIC128_CEOI3_VALUE,  "MPIC128_CEOI3 "
    chk_value CVR3, MPIC128A(MPIC128_IAR0  ), MPIC128_IAR0_VALUE,   "MPIC128_IAR0  "
    chk_value CVR3, MPIC128A(MPIC128_IAR1  ), MPIC128_IAR1_VALUE,   "MPIC128_IAR1  "
    chk_value CVR3, MPIC128A(MPIC128_IAR2  ), MPIC128_IAR2_VALUE,   "MPIC128_IAR2  "
    chk_value CVR3, MPIC128A(MPIC128_IAR3  ), MPIC128_IAR3_VALUE,   "MPIC128_IAR3  "
    chk_value CVR3, MPIC128A(MPIC128_EOI0  ), MPIC128_EOI0_VALUE,   "MPIC128_EOI0  "
    chk_value CVR3, MPIC128A(MPIC128_EOI1  ), MPIC128_EOI1_VALUE,   "MPIC128_EOI1  "
    chk_value CVR3, MPIC128A(MPIC128_EOI2  ), MPIC128_EOI2_VALUE,   "MPIC128_EOI2  "
    chk_value CVR3, MPIC128A(MPIC128_EOI3  ), MPIC128_EOI3_VALUE,   "MPIC128_EOI3  "
    chk_value CVR3, MPIC128A(MPIC128_FRG0  ), MPIC128_FRG0_VALUE,   "MPIC128_FRG0  "
    chk_value CVR3, MPIC128A(MPIC128_GCF0  ), MPIC128_GCF0_VALUE,   "MPIC128_GCF0  "
    chk_value CVR3, MPIC128A(MPIC128_VITC  ), MPIC128_VITC_VALUE,   "MPIC128_VITC  "
    chk_value CVR3, MPIC128A(MPIC128_VRID  ), MPIC128_VRID_VALUE,   "MPIC128_VRID  "
    chk_value CVR3, MPIC128A(MPIC128_PINI  ), MPIC128_PINI_VALUE,   "MPIC128_PINI  "
    chk_value CVR3, MPIC128A(MPIC128_IVP0  ), MPIC128_IVP0_VALUE,   "MPIC128_IVP0  "
    chk_value CVR3, MPIC128A(MPIC128_IVP1  ), MPIC128_IVP1_VALUE,   "MPIC128_IVP1  "
    chk_value CVR3, MPIC128A(MPIC128_IVP2  ), MPIC128_IVP2_VALUE,   "MPIC128_IVP2  "
    chk_value CVR3, MPIC128A(MPIC128_IVP3  ), MPIC128_IVP3_VALUE,   "MPIC128_IVP3  "
    chk_value CVR3, MPIC128A(MPIC128_SPV   ), MPIC128_SPV_VALUE,    "MPIC128_SPV   "
    chk_value CVR3, MPIC128A(MPIC128_TFR   ), MPIC128_TFR_VALUE,    "MPIC128_TFR   "
    chk_value CVR3, MPIC128A(MPIC128_TCC0  ), MPIC128_TCC0_VALUE,   "MPIC128_TCC0  "
    chk_value CVR3, MPIC128A(MPIC128_TCC1  ), MPIC128_TCC1_VALUE,   "MPIC128_TCC1  "
    chk_value CVR3, MPIC128A(MPIC128_TCC2  ), MPIC128_TCC2_VALUE,   "MPIC128_TCC2  "
    chk_value CVR3, MPIC128A(MPIC128_TCC3  ), MPIC128_TCC3_VALUE,   "MPIC128_TCC3  "
    chk_value CVR3, MPIC128A(MPIC128_TBC0  ), MPIC128_TBC0_VALUE,   "MPIC128_TBC0  "
    chk_value CVR3, MPIC128A(MPIC128_TBC1  ), MPIC128_TBC1_VALUE,   "MPIC128_TBC1  "
    chk_value CVR3, MPIC128A(MPIC128_TBC2  ), MPIC128_TBC2_VALUE,   "MPIC128_TBC2  "
    chk_value CVR3, MPIC128A(MPIC128_TBC3  ), MPIC128_TBC3_VALUE,   "MPIC128_TBC3  "
    chk_value CVR3, MPIC128A(MPIC128_TVP0  ), MPIC128_TVP0_VALUE,   "MPIC128_TVP0  "
    chk_value CVR3, MPIC128A(MPIC128_TVP1  ), MPIC128_TVP1_VALUE,   "MPIC128_TVP1  "
    chk_value CVR3, MPIC128A(MPIC128_TVP2  ), MPIC128_TVP2_VALUE,   "MPIC128_TVP2  "
    chk_value CVR3, MPIC128A(MPIC128_TVP3  ), MPIC128_TVP3_VALUE,   "MPIC128_TVP3  "
    chk_value CVR3, MPIC128A(MPIC128_TDST0 ), MPIC128_TDST0_VALUE,  "MPIC128_TDST0 "
    chk_value CVR3, MPIC128A(MPIC128_TDST1 ), MPIC128_TDST1_VALUE,  "MPIC128_TDST1 "
    chk_value CVR3, MPIC128A(MPIC128_TDST2 ), MPIC128_TDST2_VALUE,  "MPIC128_TDST2 "
    chk_value CVR3, MPIC128A(MPIC128_TDST3 ), MPIC128_TDST3_VALUE,  "MPIC128_TDST3 "
    chk_value CVR3, MPIC128A(MPIC128_WAI0  ), MPIC128_WAI0_VALUE,   "MPIC128_WAI0  "
    chk_value CVR3, MPIC128A(MPIC128_WAI1  ), MPIC128_WAI1_VALUE,   "MPIC128_WAI1  "
    chk_value CVR3, MPIC128A(MPIC128_WAI2  ), MPIC128_WAI2_VALUE,   "MPIC128_WAI2  "
    chk_value CVR3, MPIC128A(MPIC128_WAI3  ), MPIC128_WAI3_VALUE,   "MPIC128_WAI3  "

check_cpu_ppc_mpic_vpx:
	check_multiple_values	"MPIC128_VPx",          \
							CVR3,                   \
							MPIC128A(MPIC128_VP0),  \
							VPx_REGS_COUNT,         \
							VPx_REGS_STEP,          \
							MPIC128_VP0_VALUE

check_cpu_ppc_mpic_dstx:
	check_multiple_values	"MPIC128_DSTx",         \
							CVR3,                   \
							MPIC128A(MPIC128_DST0), \
							DSTx_REGS_COUNT,        \
							DSTx_REGS_STEP,         \
							MPIC128_DST0_VALUE

check_cpu_ppc_mpic_4_5:
    rumboot_putstring "Check running <1> in MPIC128_TFR...\n"
	load_const	r25, 0x00000000
	load_const	r24, MPIC128A(MPIC128_TFR)
	mtdcrx		r24, r25
	mfdcrx		r26, r24
	cmpw cr7,	r26, r25
    crand       eq6, eq6, eq7
	TEST_ASSERT(eq,cr7,"Read/write error in MPIC.TFR")
#ifdef STOP_ON_ERROR
    bne- cr6,   test_error
#endif

running_1:
    load_const	r25, 0x00000001
    load_const	r27, 0x00000001
    load_const	r28, 0x80000000
loop_1:
    mtdcrx		r24, r25
    mfdcrx		r26, r24
    cmpw cr7,   r26, r25
    crand       eq6, eq6, eq7
    TEST_ASSERT(eq,cr7,"Read/write error in MPIC.TFR (running 1)")
#ifdef STOP_ON_ERROR
    bne- cr6,   test_error
#endif
    cmpw cr0,	r25, r28
    beq+ cr0,	running_0
    slw			r25, r25, r27
    b			loop_1

running_0:
    rumboot_putstring "Check running <0> in MPIC128_TFR...\n"
    load_const	r25, 0xFFFFFFFF
    load_const	r24, MPIC128A(MPIC128_TFR)
    mtdcrx		r24, r25
    mfdcrx		r26, r24
    cmpw cr7,	r26, r25
    crand       eq6, eq6, eq7
    TEST_ASSERT(eq,cr7,"Read/write error in MPIC.TFR (running 0)")
#ifdef STOP_ON_ERROR
    bne- cr6,   test_error
#endif

    load_const	r25, 0xFFFFFFFE
    load_const	r27, 0x00000001
    load_const	r28, 0x7FFFFFFF
loop_0:
    mtdcrx		r24, r25
    mfdcrx		r26, r24
    cmpw cr7,	r26, r25
    crand       eq6, eq6, eq7
    TEST_ASSERT(eq,cr7,"Read/write error in MPIC.TFR (running 0)")
#ifdef STOP_ON_ERROR
    bne- cr6,   test_error
#endif
    cmpw cr0,	r25, r28
    beq+ cr0,	test_end
    slw			r25, r25, r27
    ori			r25, r25, 0x00000001
    b			loop_0

test_end:
    beq+ cr6,   test_ok
    b           test_error

test_ok:
	rumboot_putstring "TEST OK\n"
	test_event		EVENT_OK
	load_const r3,	RESULT_OK
	b finish

test_error:
	rumboot_putstring "TEST ERROR\n"
	test_event		EVENT_ERROR
	load_const r3,	RESULT_ERROR

finish:
	blr


