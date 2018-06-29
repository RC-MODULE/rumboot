
#include <platform/arch/ppc/ppc_476fp_asm.h>
#include <platform/test_event_asm.h>
#include <platform/test_assert.S.h>

#define DEBUG_DCR_TEST

#include <platform/arch/ppc/test_macro_asm.S.h>
#include <platform/devices.h>
#include <platform/regs/regs_plb6dma.h>

#define CVR3					r4,r5,r6 	/* Check Value Regs triade */
#define DP6A(OFFSET)			(DCR_DMAPLB6_BASE + (OFFSET))


.section ".text","ax",@progbits

.global main

main:
/* PLB6_DMA_0 */
	check_value CVR3, DP6A(CR0    ), CR0_VALUE,			"DMAPLB6_CR0"
	check_value CVR3, DP6A(CTC0   ), CTC0_VALUE,		"DMAPLB6_CTC0"
	check_value CVR3, DP6A(DSTR0  ), DSTR0_VALUE,		"DMAPLB6_DSTR0"
	check_value CVR3, DP6A(SAH0   ), SAH0_VALUE,		"DMAPLB6_SAH0"
	check_value CVR3, DP6A(SAL0   ), SAL0_VALUE,		"DMAPLB6_SAL0"
	check_value CVR3, DP6A(DAH0   ), DAH0_VALUE,		"DMAPLB6_DAH0"
	check_value CVR3, DP6A(DAL0   ), DAL0_VALUE,		"DMAPLB6_DAL0"
	check_value CVR3, DP6A(SGH0   ), SGH0_VALUE,		"DMAPLB6_SGH0"
	check_value CVR3, DP6A(SGL0   ), SGL0_VALUE,		"DMAPLB6_SGL0"
	check_value CVR3, DP6A(SGC0   ), SGC0_VALUE,		"DMAPLB6_SGC0"
	check_value CVR3, DP6A(CHRES0 ), CHRES0_VALUE,		"DMAPLB6_CHRES0"
	check_value CVR3, DP6A(CHLOCK0), CHLOCK0_VALUE,		"DMAPLB6_CHLOCK0"
	check_value CVR3, DP6A(SGCA0  ), SGCA0_VALUE,		"DMAPLB6_SGCA0"

	check_value CVR3, DP6A(CR1    ), CR1_VALUE,			"DMAPLB6_CR1"
	check_value CVR3, DP6A(CTC1   ), CTC1_VALUE,		"DMAPLB6_CTC1"
    check_value CVR3, DP6A(DSTR1  ), DSTR1_VALUE,		"DMAPLB6_DSTR1"
	check_value CVR3, DP6A(SAH1   ), SAH1_VALUE,		"DMAPLB6_SAH1"
	check_value CVR3, DP6A(SAL1   ), SAL1_VALUE,		"DMAPLB6_SAL1"
	check_value CVR3, DP6A(DAH1   ), DAH1_VALUE,		"DMAPLB6_DAH1"
	check_value CVR3, DP6A(DAL1   ), DAL1_VALUE,		"DMAPLB6_DAL1"
	check_value CVR3, DP6A(SGH1   ), SGH1_VALUE,		"DMAPLB6_SGH1"
	check_value CVR3, DP6A(SGL1   ), SGL1_VALUE,		"DMAPLB6_SGL1"
	check_value CVR3, DP6A(SGC1   ), SGC1_VALUE,		"DMAPLB6_SGC1"
	check_value CVR3, DP6A(CHRES1 ), CHRES1_VALUE,		"DMAPLB6_CHRES1"
	check_value CVR3, DP6A(CHLOCK1), CHLOCK1_VALUE,		"DMAPLB6_CHLOCK1"
	check_value CVR3, DP6A(SGCA1  ), SGCA1_VALUE,		"DMAPLB6_SGCA1"

	check_value CVR3, DP6A(CR2    ), CR2_VALUE,			"DMAPLB6_CR2"
	check_value CVR3, DP6A(CTC2   ), CTC2_VALUE,		"DMAPLB6_CTC2"
	check_value CVR3, DP6A(DSTR2  ), DSTR2_VALUE,		"DMAPLB6_DSTR2"
	check_value CVR3, DP6A(SAH2   ), SAH2_VALUE,		"DMAPLB6_SAH2"
	check_value CVR3, DP6A(SAL2   ), SAL2_VALUE,		"DMAPLB6_SAL2"
	check_value CVR3, DP6A(DAH2   ), DAH2_VALUE,		"DMAPLB6_DAH2"
	check_value CVR3, DP6A(DAL2   ), DAL2_VALUE,		"DMAPLB6_DAL2"
	check_value CVR3, DP6A(SGH2   ), SGH2_VALUE,		"DMAPLB6_SGH2"
	check_value CVR3, DP6A(SGL2   ), SGL2_VALUE,		"DMAPLB6_SGL2"
	check_value CVR3, DP6A(SGC2   ), SGC2_VALUE,		"DMAPLB6_SGC2"
	check_value CVR3, DP6A(CHRES2 ), CHRES2_VALUE,		"DMAPLB6_CHRES2"
	check_value CVR3, DP6A(CHLOCK2), CHLOCK2_VALUE,		"DMAPLB6_CHLOCK2"
	check_value CVR3, DP6A(SGCA2  ), SGCA2_VALUE,		"DMAPLB6_SGCA2"

	check_value CVR3, DP6A(CR3    ), CR3_VALUE,			"DMAPLB6_CR3"
	check_value CVR3, DP6A(CTC3   ), CTC3_VALUE,		"DMAPLB6_CTC3"
	check_value CVR3, DP6A(DSTR3  ), DSTR3_VALUE,		"DMAPLB6_DSTR3"
	check_value CVR3, DP6A(SAH3   ), SAH3_VALUE,		"DMAPLB6_SAH3"
	check_value CVR3, DP6A(SAL3   ), SAL3_VALUE,		"DMAPLB6_SAL3"
	check_value CVR3, DP6A(DAH3   ), DAH3_VALUE,		"DMAPLB6_DAH3"
	check_value CVR3, DP6A(DAL3   ), DAL3_VALUE,		"DMAPLB6_DAL3"
	check_value CVR3, DP6A(SGH3   ), SGH3_VALUE,		"DMAPLB6_SGH3"
	check_value CVR3, DP6A(SGL3   ), SGL3_VALUE,		"DMAPLB6_SGL3"
	check_value CVR3, DP6A(SGC3   ), SGC3_VALUE,		"DMAPLB6_SGC3"
	check_value CVR3, DP6A(CHRES3 ), CHRES3_VALUE,		"DMAPLB6_CHRES3"
	check_value CVR3, DP6A(CHLOCK3), CHLOCK3_VALUE,		"DMAPLB6_CHLOCK3"
	check_value CVR3, DP6A(SGCA3  ), SGCA3_VALUE,		"DMAPLB6_SGCA3"

	check_value CVR3, DP6A(SR     ), SR_VALUE,			"DMAPLB6_SR"
	check_value CVR3, DP6A(MPEAH  ), MPEAH_VALUE,		"DMAPLB6_MPEAH"
	check_value CVR3, DP6A(PMEAL  ), PMEAL_VALUE,		"DMAPLB6_PMEAL"
	check_value CVR3, DP6A(PSE    ), PSE_VALUE,			"DMAPLB6_PSE"
	check_value CVR3, DP6A(PSEAH  ), PSEAH_VALUE,		"DMAPLB6_PSEAH"
	check_value CVR3, DP6A(PSEAL  ), PSEAL_VALUE,		"DMAPLB6_PSEAL"
	check_value CVR3, DP6A(CRS    ), CRS_VALUE,			"DMAPLB6_CRS"
	check_value CVR3, DP6A(REVID  ), REVID_VALUE,		"DMAPLB6_REVID"
	check_value CVR3, DP6A(ARB    ), ARB_VALUE,			"DMAPLB6_ARB"
	check_value CVR3, DP6A(OPTIONS), OPTIONS_VALUE,		"DMAPLB6_OPTIONS"
	check_value CVR3, DP6A(PERM   ), PERM_VALUE,		"DMAPLB6_PERM"
	check_value CVR3, DP6A(PNC    ), P6DMA0_PNC_VALUE,	"DMAPLB6_PNC"

    rumboot_putstring "CHECK DEFAULT VALUES DONE\n"

running_0_1_dma0:
    load_const r5, 0x00000000
    load_const r4, DCR_DMAPLB6_BASE + SAH0
    mtdcrx r4, r5
    mfdcrx r6, r4
    cmp cr7,0, r6,r5
    TEST_ASSERT(eq,cr7,"Read/write error in DCR_DMAPLB6_BASE + SAH0 (running 1 init)")

dma0_running_1:
    load_const r5, 0x00000001
    load_const r7, 1
    load_const r8, 0x80000000
dma0_loop_1:
    mtdcrx r4, r5
    mfdcrx r6, r4
    cmp cr7,0, r6,r5
    TEST_ASSERT(eq,cr7,"Read/write error in DCR_DMAPLB6_BASE + SAH0 (running 1)")
    cmp cr0,0, r5,r8
    beq dma0_running_0
    slw r5, r5, r7
    b dma0_loop_1

dma0_running_0:
    load_const r5, 0xFFFFFFFF
    load_const r4, DCR_DMAPLB6_BASE + SAH0
    mtdcrx r4, r5
    mfdcrx r6, r4
    cmp cr7,0, r6,r5
    TEST_ASSERT(eq,cr7,"Read/write error in DCR_DMAPLB6_BASE + SAH0 (running 0 init)")

    load_const r5, 0xFFFFFFFE
    load_const r7, 1
    load_const r8, 0x7FFFFFFF
dma0_loop_0:
    mtdcrx r4, r5
    mfdcrx r6, r4
    cmp cr7,0, r6,r5
    TEST_ASSERT(eq,cr7,"Read/write error in DCR_DMAPLB6_BASE + SAH0 (running 0)")
    cmp cr0, 0, r5, r8
    beq dma0_done
    slw r5, r5, r7
    ori r5, r5, 1
    b dma0_loop_0

dma0_done:
    rumboot_putstring "PLB6_DMA_0 running 0/1 done\n"


test_ok:
	rumboot_putstring "TEST OK\n"
	test_event		EVENT_OK
	load_const r3,	RESULT_OK
	b finish

error:
	rumboot_putstring "TEST ERROR\n"
	test_event		EVENT_ERROR
	load_const r3,	RESULT_ERROR

finish:
	blr

