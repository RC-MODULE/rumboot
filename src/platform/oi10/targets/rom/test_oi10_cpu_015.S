
#include <platform/oi10/platform/arch/ppc/ppc_476fp_asm.h>
#include <platform/oi10/platform/test_event_asm.h>
#include <platform/oi10/platform/test_assert.S.h>
#include <platform/oi10/platform/arch/ppc/test_macro_asm.S.h>
#include <platform/devices.h>
#include <platform/oi10/platform/regs/regs_plb6dma.h>

.section ".text","ax",@progbits

.global image_start
.global main

image_start:
main:

/* PLB6_DMA_0 */
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + CR0, CR0_VALUE, "DCR_DMAPLB6_BASE + CR0"
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + CTC0, CTC0_VALUE, "DCR_DMAPLB6_BASE + CTC0"
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + DSTR0, DSTR0_VALUE, "DCR_DMAPLB6_BASE + DSTR0"
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + SAH0, SAH0_VALUE, "DCR_DMAPLB6_BASE + SAH0"
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + SAL0, SAL0_VALUE, "DCR_DMAPLB6_BASE + SAL0"
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + DAH0, DAH0_VALUE, "DCR_DMAPLB6_BASE + DAH0"
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + DAL0, DAL0_VALUE, "DCR_DMAPLB6_BASE + DAL0"
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + SGH0, SGH0_VALUE, "DCR_DMAPLB6_BASE + SGH0"
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + SGL0, SGL0_VALUE, "DCR_DMAPLB6_BASE + SGL0"
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + SGC0, SGC0_VALUE, "DCR_DMAPLB6_BASE + SGC0"
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + CHRES0, CHRES0_VALUE, "DCR_DMAPLB6_BASE + CHRES0"
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + CHLOCK0, CHLOCK0_VALUE, "DCR_DMAPLB6_BASE + CHLOCK0"
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + SGCA0, SGCA0_VALUE, "DCR_DMAPLB6_BASE + SGCA0"

    check_value r4,r5,r6, DCR_DMAPLB6_BASE + CR1, CR1_VALUE, "DCR_DMAPLB6_BASE + CR1"
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + CTC1, CTC1_VALUE, "DCR_DMAPLB6_BASE + CTC1"
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + DSTR1, DSTR1_VALUE, "DCR_DMAPLB6_BASE + DSTR1"
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + SAH1, SAH1_VALUE, "DCR_DMAPLB6_BASE + SAH1"
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + SAL1, SAL1_VALUE, "DCR_DMAPLB6_BASE + SAL1"
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + DAH1, DAH1_VALUE, "DCR_DMAPLB6_BASE + DAH1"
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + DAL1, DAL1_VALUE, "DCR_DMAPLB6_BASE + DAL1"
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + SGH1, SGH1_VALUE, "DCR_DMAPLB6_BASE + SGH1"
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + SGL1, SGL1_VALUE, "DCR_DMAPLB6_BASE + SGL1"
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + SGC1, SGC1_VALUE, "DCR_DMAPLB6_BASE + SGC1"
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + CHRES1, CHRES1_VALUE, "DCR_DMAPLB6_BASE + CHRES1"
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + CHLOCK1, CHLOCK1_VALUE, "DCR_DMAPLB6_BASE + CHLOCK1"
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + SGCA1, SGCA1_VALUE, "DCR_DMAPLB6_BASE + SGCA1"

    check_value r4,r5,r6, DCR_DMAPLB6_BASE + CR2, CR2_VALUE, "DCR_DMAPLB6_BASE + CR2"
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + CTC2, CTC2_VALUE, "DCR_DMAPLB6_BASE + CTC2"
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + DSTR2, DSTR2_VALUE, "DCR_DMAPLB6_BASE + DSTR2"
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + SAH2, SAH2_VALUE, "DCR_DMAPLB6_BASE + SAH2"
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + SAL2, SAL2_VALUE, "DCR_DMAPLB6_BASE + SAL2"
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + DAH2, DAH2_VALUE, "DCR_DMAPLB6_BASE + DAH2"
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + DAL2, DAL2_VALUE, "DCR_DMAPLB6_BASE + DAL2"
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + SGH2, SGH2_VALUE, "DCR_DMAPLB6_BASE + SGH2"
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + SGL2, SGL2_VALUE, "DCR_DMAPLB6_BASE + SGL2"
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + SGC2, SGC2_VALUE, "DCR_DMAPLB6_BASE + SGC2"
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + CHRES2, CHRES2_VALUE, "DCR_DMAPLB6_BASE + CHRES2"
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + CHLOCK2, CHLOCK2_VALUE, "DCR_DMAPLB6_BASE + CHLOCK2"
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + SGCA2, SGCA2_VALUE, "DCR_DMAPLB6_BASE + SGCA2"

    check_value r4,r5,r6, DCR_DMAPLB6_BASE + CR3, CR3_VALUE, "DCR_DMAPLB6_BASE + CR3"
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + CTC3, CTC3_VALUE, "DCR_DMAPLB6_BASE + CTC3"
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + DSTR3, DSTR3_VALUE, "DCR_DMAPLB6_BASE + DSTR3"
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + SAH3, SAH3_VALUE, "DCR_DMAPLB6_BASE + SAH3"
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + SAL3, SAL3_VALUE, "DCR_DMAPLB6_BASE + SAL3"
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + DAH3, DAH3_VALUE, "DCR_DMAPLB6_BASE + DAH3"
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + DAL3, DAL3_VALUE, "DCR_DMAPLB6_BASE + DAL3"
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + SGH3, SGH3_VALUE, "DCR_DMAPLB6_BASE + SGH3"
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + SGL3, SGL3_VALUE, "DCR_DMAPLB6_BASE + SGL3"
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + SGC3, SGC3_VALUE, "DCR_DMAPLB6_BASE + SGC3"
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + CHRES3, CHRES3_VALUE, "DCR_DMAPLB6_BASE + CHRES3"
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + CHLOCK3, CHLOCK3_VALUE, "DCR_DMAPLB6_BASE + CHLOCK3"
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + SGCA3, SGCA3_VALUE, "DCR_DMAPLB6_BASE + SGCA3"

    check_value r4,r5,r6, DCR_DMAPLB6_BASE + SR, SR_VALUE, "DCR_DMAPLB6_BASE + SR"
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + MPEAH, MPEAH_VALUE, "DCR_DMAPLB6_BASE + MPEAH"
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + PMEAL, PMEAL_VALUE, "DCR_DMAPLB6_BASE + PMEAL"
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + PSE, PSE_VALUE, "DCR_DMAPLB6_BASE + PSE"
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + PSEAH, PSEAH_VALUE, "DCR_DMAPLB6_BASE + PSEAH"
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + PSEAL, PSEAL_VALUE, "DCR_DMAPLB6_BASE + PSEAL"
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + CRS, CRS_VALUE, "DCR_DMAPLB6_BASE + CRS"
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + REVID, REVID_VALUE, "DCR_DMAPLB6_BASE + REVID"
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + ARB, ARB_VALUE, "DCR_DMAPLB6_BASE + ARB"
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + OPTIONS, OPTIONS_VALUE, "DCR_DMAPLB6_BASE + OPTIONS"
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + PERM, PERM_VALUE, "DCR_DMAPLB6_BASE + PERM"
    check_value r4,r5,r6, DCR_DMAPLB6_BASE + PNC, P6DMA0_PNC_VALUE, "DCR_DMAPLB6_BASE + PNC"

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
    test_event EVENT_OK
    rumboot_putstring "TEST OK\n"
    load_const r3, 0x00
    b finish

error:
     test_event EVENT_ERROR
     rumboot_putstring "TEST ERROR\n"
     load_const r3, 0x01

finish:
	blr

