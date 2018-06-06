
#include <platform/oi10/platform/arch/ppc/ppc_476fp_asm.h>
#include <platform/oi10/platform/test_event_asm.h>
#include <platform/oi10/platform/test_assert.S.h>
#include <platform/oi10/platform/arch/ppc/test_macro_asm.S.h>
#include <platform/devices.h>
#include <platform/oi10/platform/regs/regs_plb6dma.h>


/* DEFAULT VALUES */
/* from trunk/cpu/units/dma2plb6_v001/verilog/rtl/DMA2PLB6.defines.v */
#define DMA2PLB6_REV_NBR_VALUE        0x001
#define DMA2PLB6_BRANCH_REV_NBR_VALUE  0x04

#define CR0_VALUE       0x00000000
#define CTC0_VALUE      0x00000000
#define DSTR0_VALUE     0x00000000
#define SAH0_VALUE      0x00000000
#define SAL0_VALUE      0x00000000
#define DAH0_VALUE      0x00000000
#define DAL0_VALUE      0x00000000
#define SGH0_VALUE      0x00000000
#define SGL0_VALUE      0x00000000
#define SGC0_VALUE      0x00000000
#define CHRES0_VALUE    0x00000000
#define CHLOCK0_VALUE   0x00000000
#define SGCA0_VALUE     0x00000000
#define CR1_VALUE       0x00000000
#define CTC1_VALUE      0x00000000
#define DSTR1_VALUE     0x00000000
#define SAH1_VALUE      0x00000000
#define SAL1_VALUE      0x00000000
#define DAH1_VALUE      0x00000000
#define DAL1_VALUE      0x00000000
#define SGH1_VALUE      0x00000000
#define SGL1_VALUE      0x00000000
#define SGC1_VALUE      0x00000000
#define CHRES1_VALUE    0x00000000
#define CHLOCK1_VALUE   0x00000000
#define SGCA1_VALUE     0x00000000
#define CR2_VALUE       0x00000000
#define CTC2_VALUE      0x00000000
#define DSTR2_VALUE     0x00000000
#define SAH2_VALUE      0x00000000
#define SAL2_VALUE      0x00000000
#define DAH2_VALUE      0x00000000
#define DAL2_VALUE      0x00000000
#define SGH2_VALUE      0x00000000
#define SGL2_VALUE      0x00000000
#define SGC2_VALUE      0x00000000
#define CHRES2_VALUE    0x00000000
#define CHLOCK2_VALUE   0x00000000
#define SGCA2_VALUE     0x00000000
#define CR3_VALUE       0x00000000
#define CTC3_VALUE      0x00000000
#define DSTR3_VALUE     0x00000000
#define SAH3_VALUE      0x00000000
#define SAL3_VALUE      0x00000000
#define DAH3_VALUE      0x00000000
#define DAL3_VALUE      0x00000000
#define SGH3_VALUE      0x00000000
#define SGL3_VALUE      0x00000000
#define SGC3_VALUE      0x00000000
#define CHRES3_VALUE    0x00000000
#define CHLOCK3_VALUE   0x00000000
#define SGCA3_VALUE     0x00000000
#define SR_VALUE        0x00000000
#define MPEAH_VALUE     0x00000000
#define PMEAL_VALUE     0x00000000
#define PSE_VALUE       0x00000000
#define PSEAH_VALUE     0x00000000
#define PSEAL_VALUE     0x00000000
#define CRS_VALUE       0x00000000
#define REVID_VALUE     reg_field(31,DMA2PLB6_BRANCH_REV_NBR_VALUE) | reg_field(23,DMA2PLB6_REV_NBR_VALUE)
#define ARB_VALUE       0x00000000
#define OPTIONS_VALUE   0x00010000
#define PERM_VALUE      0xFFFF0000

/* Table 2-1 */
#define P6DMA0_PNC_VALUE    0x41
#define P6DMA1_PNC_VALUE    0xE0


.section ".text","ax",@progbits

.global image_start
.global main

image_start:
main:

/* PLB6_DMA_0 */
    check_value r4,r5,r6, PLB6_DMA0_CR0, CR0_VALUE, "PLB6_DMA0_CR0"
    check_value r4,r5,r6, PLB6_DMA0_CTC0, CTC0_VALUE, "PLB6_DMA0_CTC0"
    check_value r4,r5,r6, PLB6_DMA0_DSTR0, DSTR0_VALUE, "PLB6_DMA0_DSTR0"
    check_value r4,r5,r6, PLB6_DMA0_SAH0, SAH0_VALUE, "PLB6_DMA0_SAH0"
    check_value r4,r5,r6, PLB6_DMA0_SAL0, SAL0_VALUE, "PLB6_DMA0_SAL0"
    check_value r4,r5,r6, PLB6_DMA0_DAH0, DAH0_VALUE, "PLB6_DMA0_DAH0"
    check_value r4,r5,r6, PLB6_DMA0_DAL0, DAL0_VALUE, "PLB6_DMA0_DAL0"
    check_value r4,r5,r6, PLB6_DMA0_SGH0, SGH0_VALUE, "PLB6_DMA0_SGH0"
    check_value r4,r5,r6, PLB6_DMA0_SGL0, SGL0_VALUE, "PLB6_DMA0_SGL0"
    check_value r4,r5,r6, PLB6_DMA0_SGC0, SGC0_VALUE, "PLB6_DMA0_SGC0"
    check_value r4,r5,r6, PLB6_DMA0_CHRES0, CHRES0_VALUE, "PLB6_DMA0_CHRES0"
    check_value r4,r5,r6, PLB6_DMA0_CHLOCK0, CHLOCK0_VALUE, "PLB6_DMA0_CHLOCK0"
    check_value r4,r5,r6, PLB6_DMA0_SGCA0, SGCA0_VALUE, "PLB6_DMA0_SGCA0"

    check_value r4,r5,r6, PLB6_DMA0_CR1, CR1_VALUE, "PLB6_DMA0_CR1"
    check_value r4,r5,r6, PLB6_DMA0_CTC1, CTC1_VALUE, "PLB6_DMA0_CTC1"
    check_value r4,r5,r6, PLB6_DMA0_DSTR1, DSTR1_VALUE, "PLB6_DMA0_DSTR1"
    check_value r4,r5,r6, PLB6_DMA0_SAH1, SAH1_VALUE, "PLB6_DMA0_SAH1"
    check_value r4,r5,r6, PLB6_DMA0_SAL1, SAL1_VALUE, "PLB6_DMA0_SAL1"
    check_value r4,r5,r6, PLB6_DMA0_DAH1, DAH1_VALUE, "PLB6_DMA0_DAH1"
    check_value r4,r5,r6, PLB6_DMA0_DAL1, DAL1_VALUE, "PLB6_DMA0_DAL1"
    check_value r4,r5,r6, PLB6_DMA0_SGH1, SGH1_VALUE, "PLB6_DMA0_SGH1"
    check_value r4,r5,r6, PLB6_DMA0_SGL1, SGL1_VALUE, "PLB6_DMA0_SGL1"
    check_value r4,r5,r6, PLB6_DMA0_SGC1, SGC1_VALUE, "PLB6_DMA0_SGC1"
    check_value r4,r5,r6, PLB6_DMA0_CHRES1, CHRES1_VALUE, "PLB6_DMA0_CHRES1"
    check_value r4,r5,r6, PLB6_DMA0_CHLOCK1, CHLOCK1_VALUE, "PLB6_DMA0_CHLOCK1"
    check_value r4,r5,r6, PLB6_DMA0_SGCA1, SGCA1_VALUE, "PLB6_DMA0_SGCA1"

    check_value r4,r5,r6, PLB6_DMA0_CR2, CR2_VALUE, "PLB6_DMA0_CR2"
    check_value r4,r5,r6, PLB6_DMA0_CTC2, CTC2_VALUE, "PLB6_DMA0_CTC2"
    check_value r4,r5,r6, PLB6_DMA0_DSTR2, DSTR2_VALUE, "PLB6_DMA0_DSTR2"
    check_value r4,r5,r6, PLB6_DMA0_SAH2, SAH2_VALUE, "PLB6_DMA0_SAH2"
    check_value r4,r5,r6, PLB6_DMA0_SAL2, SAL2_VALUE, "PLB6_DMA0_SAL2"
    check_value r4,r5,r6, PLB6_DMA0_DAH2, DAH2_VALUE, "PLB6_DMA0_DAH2"
    check_value r4,r5,r6, PLB6_DMA0_DAL2, DAL2_VALUE, "PLB6_DMA0_DAL2"
    check_value r4,r5,r6, PLB6_DMA0_SGH2, SGH2_VALUE, "PLB6_DMA0_SGH2"
    check_value r4,r5,r6, PLB6_DMA0_SGL2, SGL2_VALUE, "PLB6_DMA0_SGL2"
    check_value r4,r5,r6, PLB6_DMA0_SGC2, SGC2_VALUE, "PLB6_DMA0_SGC2"
    check_value r4,r5,r6, PLB6_DMA0_CHRES2, CHRES2_VALUE, "PLB6_DMA0_CHRES2"
    check_value r4,r5,r6, PLB6_DMA0_CHLOCK2, CHLOCK2_VALUE, "PLB6_DMA0_CHLOCK2"
    check_value r4,r5,r6, PLB6_DMA0_SGCA2, SGCA2_VALUE, "PLB6_DMA0_SGCA2"

    check_value r4,r5,r6, PLB6_DMA0_CR3, CR3_VALUE, "PLB6_DMA0_CR3"
    check_value r4,r5,r6, PLB6_DMA0_CTC3, CTC3_VALUE, "PLB6_DMA0_CTC3"
    check_value r4,r5,r6, PLB6_DMA0_DSTR3, DSTR3_VALUE, "PLB6_DMA0_DSTR3"
    check_value r4,r5,r6, PLB6_DMA0_SAH3, SAH3_VALUE, "PLB6_DMA0_SAH3"
    check_value r4,r5,r6, PLB6_DMA0_SAL3, SAL3_VALUE, "PLB6_DMA0_SAL3"
    check_value r4,r5,r6, PLB6_DMA0_DAH3, DAH3_VALUE, "PLB6_DMA0_DAH3"
    check_value r4,r5,r6, PLB6_DMA0_DAL3, DAL3_VALUE, "PLB6_DMA0_DAL3"
    check_value r4,r5,r6, PLB6_DMA0_SGH3, SGH3_VALUE, "PLB6_DMA0_SGH3"
    check_value r4,r5,r6, PLB6_DMA0_SGL3, SGL3_VALUE, "PLB6_DMA0_SGL3"
    check_value r4,r5,r6, PLB6_DMA0_SGC3, SGC3_VALUE, "PLB6_DMA0_SGC3"
    check_value r4,r5,r6, PLB6_DMA0_CHRES3, CHRES3_VALUE, "PLB6_DMA0_CHRES3"
    check_value r4,r5,r6, PLB6_DMA0_CHLOCK3, CHLOCK3_VALUE, "PLB6_DMA0_CHLOCK3"
    check_value r4,r5,r6, PLB6_DMA0_SGCA3, SGCA3_VALUE, "PLB6_DMA0_SGCA3"

    check_value r4,r5,r6, PLB6_DMA0_SR, SR_VALUE, "PLB6_DMA0_SR"
    check_value r4,r5,r6, PLB6_DMA0_MPEAH, MPEAH_VALUE, "PLB6_DMA0_MPEAH"
    check_value r4,r5,r6, PLB6_DMA0_PMEAL, PMEAL_VALUE, "PLB6_DMA0_PMEAL"
    check_value r4,r5,r6, PLB6_DMA0_PSE, PSE_VALUE, "PLB6_DMA0_PSE"
    check_value r4,r5,r6, PLB6_DMA0_PSEAH, PSEAH_VALUE, "PLB6_DMA0_PSEAH"
    check_value r4,r5,r6, PLB6_DMA0_PSEAL, PSEAL_VALUE, "PLB6_DMA0_PSEAL"
    check_value r4,r5,r6, PLB6_DMA0_CRS, CRS_VALUE, "PLB6_DMA0_CRS"
    check_value r4,r5,r6, PLB6_DMA0_REVID, REVID_VALUE, "PLB6_DMA0_REVID"
    check_value r4,r5,r6, PLB6_DMA0_ARB, ARB_VALUE, "PLB6_DMA0_ARB"
    check_value r4,r5,r6, PLB6_DMA0_OPTIONS, OPTIONS_VALUE, "PLB6_DMA0_OPTIONS"
    check_value r4,r5,r6, PLB6_DMA0_PERM, PERM_VALUE, "PLB6_DMA0_PERM"
    check_value r4,r5,r6, PLB6_DMA0_PNC, P6DMA0_PNC_VALUE, "PLB6_DMA0_PNC"

    rumboot_putstring "CHECK DEFAULT VALUES DONE\n"

running_0_1_dma0:
    load_const r5, 0x00000000
    load_const r4, PLB6_DMA0_SAH0
    mtdcrx r4, r5
    mfdcrx r6, r4
    cmp cr7,0, r6,r5
    TEST_ASSERT(eq,cr7,"Read/write error in PLB6_DMA0_SAH0 (running 1 init)")

dma0_running_1:
    load_const r5, 0x00000001
    load_const r7, 1
    load_const r8, 0x80000000
dma0_loop_1:
    mtdcrx r4, r5
    mfdcrx r6, r4
    cmp cr7,0, r6,r5
    TEST_ASSERT(eq,cr7,"Read/write error in PLB6_DMA0_SAH0 (running 1)")
    cmp cr0,0, r5,r8
    beq dma0_running_0
    slw r5, r5, r7
    b dma0_loop_1

dma0_running_0:
    load_const r5, 0xFFFFFFFF
    load_const r4, PLB6_DMA0_SAH0
    mtdcrx r4, r5
    mfdcrx r6, r4
    cmp cr7,0, r6,r5
    TEST_ASSERT(eq,cr7,"Read/write error in PLB6_DMA0_SAH0 (running 0 init)")

    load_const r5, 0xFFFFFFFE
    load_const r7, 1
    load_const r8, 0x7FFFFFFF
dma0_loop_0:
    mtdcrx r4, r5
    mfdcrx r6, r4
    cmp cr7,0, r6,r5
    TEST_ASSERT(eq,cr7,"Read/write error in PLB6_DMA0_SAH0 (running 0)")
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

