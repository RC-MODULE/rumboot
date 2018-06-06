
#include <platform/oi10/platform/arch/ppc/ppc_476fp_asm.h>
#include <platform/oi10/platform/test_event_asm.h>
// #include <mivem_mem_map.h>
#include <platform/oi10/platform/test_assert.S.h>
#include <platform/oi10/platform/arch/ppc/test_macro_asm.S.h>


//#include "test_mpw_cpu_015_0.h"
//PLB6_DMA0

#define PLB6_DMA0_CR0        DMA2PLB6_0_BASE + 0x00
#define PLB6_DMA0_CTC0       DMA2PLB6_0_BASE + 0x01
#define PLB6_DMA0_DSTR0      DMA2PLB6_0_BASE + 0x02
#define PLB6_DMA0_SAH0       DMA2PLB6_0_BASE + 0x03
#define PLB6_DMA0_SAL0       DMA2PLB6_0_BASE + 0x04
#define PLB6_DMA0_DAH0       DMA2PLB6_0_BASE + 0x05
#define PLB6_DMA0_DAL0       DMA2PLB6_0_BASE + 0x06
#define PLB6_DMA0_SGH0       DMA2PLB6_0_BASE + 0x07
#define PLB6_DMA0_SGL0       DMA2PLB6_0_BASE + 0x08
#define PLB6_DMA0_SGC0       DMA2PLB6_0_BASE + 0x09
#define PLB6_DMA0_CHRES0     DMA2PLB6_0_BASE + 0x0A
#define PLB6_DMA0_CHLOCK0    DMA2PLB6_0_BASE + 0x0B
#define PLB6_DMA0_SGCA0      DMA2PLB6_0_BASE + 0x0C
#define PLB6_DMA0_CR1        DMA2PLB6_0_BASE + 0x10
#define PLB6_DMA0_CTC1       DMA2PLB6_0_BASE + 0x11
#define PLB6_DMA0_DSTR1      DMA2PLB6_0_BASE + 0x12
#define PLB6_DMA0_SAH1       DMA2PLB6_0_BASE + 0x13
#define PLB6_DMA0_SAL1       DMA2PLB6_0_BASE + 0x14
#define PLB6_DMA0_DAH1       DMA2PLB6_0_BASE + 0x15
#define PLB6_DMA0_DAL1       DMA2PLB6_0_BASE + 0x16
#define PLB6_DMA0_SGH1       DMA2PLB6_0_BASE + 0x17
#define PLB6_DMA0_SGL1       DMA2PLB6_0_BASE + 0x18
#define PLB6_DMA0_SGC1       DMA2PLB6_0_BASE + 0x19
#define PLB6_DMA0_CHRES1     DMA2PLB6_0_BASE + 0x1A
#define PLB6_DMA0_CHLOCK1    DMA2PLB6_0_BASE + 0x1B
#define PLB6_DMA0_SGCA1      DMA2PLB6_0_BASE + 0x1C
#define PLB6_DMA0_CR2        DMA2PLB6_0_BASE + 0x20
#define PLB6_DMA0_CTC2       DMA2PLB6_0_BASE + 0x21
#define PLB6_DMA0_DSTR2      DMA2PLB6_0_BASE + 0x22
#define PLB6_DMA0_SAH2       DMA2PLB6_0_BASE + 0x23
#define PLB6_DMA0_SAL2       DMA2PLB6_0_BASE + 0x24
#define PLB6_DMA0_DAH2       DMA2PLB6_0_BASE + 0x25
#define PLB6_DMA0_DAL2       DMA2PLB6_0_BASE + 0x26
#define PLB6_DMA0_SGH2       DMA2PLB6_0_BASE + 0x27
#define PLB6_DMA0_SGL2       DMA2PLB6_0_BASE + 0x28
#define PLB6_DMA0_SGC2       DMA2PLB6_0_BASE + 0x29
#define PLB6_DMA0_CHRES2     DMA2PLB6_0_BASE + 0x2A
#define PLB6_DMA0_CHLOCK2    DMA2PLB6_0_BASE + 0x2B
#define PLB6_DMA0_SGCA2      DMA2PLB6_0_BASE + 0x2C
#define PLB6_DMA0_CR3        DMA2PLB6_0_BASE + 0x30
#define PLB6_DMA0_CTC3       DMA2PLB6_0_BASE + 0x31
#define PLB6_DMA0_DSTR3      DMA2PLB6_0_BASE + 0x32
#define PLB6_DMA0_SAH3       DMA2PLB6_0_BASE + 0x33
#define PLB6_DMA0_SAL3       DMA2PLB6_0_BASE + 0x34
#define PLB6_DMA0_DAH3       DMA2PLB6_0_BASE + 0x35
#define PLB6_DMA0_DAL3       DMA2PLB6_0_BASE + 0x36
#define PLB6_DMA0_SGH3       DMA2PLB6_0_BASE + 0x37
#define PLB6_DMA0_SGL3       DMA2PLB6_0_BASE + 0x38
#define PLB6_DMA0_SGC3       DMA2PLB6_0_BASE + 0x39
#define PLB6_DMA0_CHRES3     DMA2PLB6_0_BASE + 0x3A
#define PLB6_DMA0_CHLOCK3    DMA2PLB6_0_BASE + 0x3B
#define PLB6_DMA0_SGCA3      DMA2PLB6_0_BASE + 0x3C
#define PLB6_DMA0_SR         DMA2PLB6_0_BASE + 0x40
#define PLB6_DMA0_MPEAH      DMA2PLB6_0_BASE + 0x41
#define PLB6_DMA0_PMEAL      DMA2PLB6_0_BASE + 0x42
#define PLB6_DMA0_PSE        DMA2PLB6_0_BASE + 0x43
#define PLB6_DMA0_PSEAH      DMA2PLB6_0_BASE + 0x44
#define PLB6_DMA0_PSEAL      DMA2PLB6_0_BASE + 0x45
#define PLB6_DMA0_CRS        DMA2PLB6_0_BASE + 0x46
#define PLB6_DMA0_REVID      DMA2PLB6_0_BASE + 0x47
#define PLB6_DMA0_ARB        DMA2PLB6_0_BASE + 0x48
#define PLB6_DMA0_OPTIONS    DMA2PLB6_0_BASE + 0x49
#define PLB6_DMA0_PERM       DMA2PLB6_0_BASE + 0x4A
#define PLB6_DMA0_PNC        DMA2PLB6_0_BASE + 0x4B

//PLB6_DMA1

#define PLB6_DMA1_CR0        DMA2PLB6_1_BASE + 0x00
#define PLB6_DMA1_CTC0       DMA2PLB6_1_BASE + 0x01
#define PLB6_DMA1_DSTR0      DMA2PLB6_1_BASE + 0x02
#define PLB6_DMA1_SAH0       DMA2PLB6_1_BASE + 0x03
#define PLB6_DMA1_SAL0       DMA2PLB6_1_BASE + 0x04
#define PLB6_DMA1_DAH0       DMA2PLB6_1_BASE + 0x05
#define PLB6_DMA1_DAL0       DMA2PLB6_1_BASE + 0x06
#define PLB6_DMA1_SGH0       DMA2PLB6_1_BASE + 0x07
#define PLB6_DMA1_SGL0       DMA2PLB6_1_BASE + 0x08
#define PLB6_DMA1_SGC0       DMA2PLB6_1_BASE + 0x09
#define PLB6_DMA1_CHRES0     DMA2PLB6_1_BASE + 0x0A
#define PLB6_DMA1_CHLOCK0    DMA2PLB6_1_BASE + 0x0B
#define PLB6_DMA1_SGCA0      DMA2PLB6_1_BASE + 0x0C
#define PLB6_DMA1_CR1        DMA2PLB6_1_BASE + 0x10
#define PLB6_DMA1_CTC1       DMA2PLB6_1_BASE + 0x11
#define PLB6_DMA1_DSTR1      DMA2PLB6_1_BASE + 0x12
#define PLB6_DMA1_SAH1       DMA2PLB6_1_BASE + 0x13
#define PLB6_DMA1_SAL1       DMA2PLB6_1_BASE + 0x14
#define PLB6_DMA1_DAH1       DMA2PLB6_1_BASE + 0x15
#define PLB6_DMA1_DAL1       DMA2PLB6_1_BASE + 0x16
#define PLB6_DMA1_SGH1       DMA2PLB6_1_BASE + 0x17
#define PLB6_DMA1_SGL1       DMA2PLB6_1_BASE + 0x18
#define PLB6_DMA1_SGC1       DMA2PLB6_1_BASE + 0x19
#define PLB6_DMA1_CHRES1     DMA2PLB6_1_BASE + 0x1A
#define PLB6_DMA1_CHLOCK1    DMA2PLB6_1_BASE + 0x1B
#define PLB6_DMA1_SGCA1      DMA2PLB6_1_BASE + 0x1C
#define PLB6_DMA1_CR2        DMA2PLB6_1_BASE + 0x20
#define PLB6_DMA1_CTC2       DMA2PLB6_1_BASE + 0x21
#define PLB6_DMA1_DSTR2      DMA2PLB6_1_BASE + 0x22
#define PLB6_DMA1_SAH2       DMA2PLB6_1_BASE + 0x23
#define PLB6_DMA1_SAL2       DMA2PLB6_1_BASE + 0x24
#define PLB6_DMA1_DAH2       DMA2PLB6_1_BASE + 0x25
#define PLB6_DMA1_DAL2       DMA2PLB6_1_BASE + 0x26
#define PLB6_DMA1_SGH2       DMA2PLB6_1_BASE + 0x27
#define PLB6_DMA1_SGL2       DMA2PLB6_1_BASE + 0x28
#define PLB6_DMA1_SGC2       DMA2PLB6_1_BASE + 0x29
#define PLB6_DMA1_CHRES2     DMA2PLB6_1_BASE + 0x2A
#define PLB6_DMA1_CHLOCK2    DMA2PLB6_1_BASE + 0x2B
#define PLB6_DMA1_SGCA2      DMA2PLB6_1_BASE + 0x2C
#define PLB6_DMA1_CR3        DMA2PLB6_1_BASE + 0x30
#define PLB6_DMA1_CTC3       DMA2PLB6_1_BASE + 0x31
#define PLB6_DMA1_DSTR3      DMA2PLB6_1_BASE + 0x32
#define PLB6_DMA1_SAH3       DMA2PLB6_1_BASE + 0x33
#define PLB6_DMA1_SAL3       DMA2PLB6_1_BASE + 0x34
#define PLB6_DMA1_DAH3       DMA2PLB6_1_BASE + 0x35
#define PLB6_DMA1_DAL3       DMA2PLB6_1_BASE + 0x36
#define PLB6_DMA1_SGH3       DMA2PLB6_1_BASE + 0x37
#define PLB6_DMA1_SGL3       DMA2PLB6_1_BASE + 0x38
#define PLB6_DMA1_SGC3       DMA2PLB6_1_BASE + 0x39
#define PLB6_DMA1_CHRES3     DMA2PLB6_1_BASE + 0x3A
#define PLB6_DMA1_CHLOCK3    DMA2PLB6_1_BASE + 0x3B
#define PLB6_DMA1_SGCA3      DMA2PLB6_1_BASE + 0x3C
#define PLB6_DMA1_SR         DMA2PLB6_1_BASE + 0x40
#define PLB6_DMA1_MPEAH      DMA2PLB6_1_BASE + 0x41
#define PLB6_DMA1_PMEAL      DMA2PLB6_1_BASE + 0x42
#define PLB6_DMA1_PSE        DMA2PLB6_1_BASE + 0x43
#define PLB6_DMA1_PSEAH      DMA2PLB6_1_BASE + 0x44
#define PLB6_DMA1_PSEAL      DMA2PLB6_1_BASE + 0x45
#define PLB6_DMA1_CRS        DMA2PLB6_1_BASE + 0x46
#define PLB6_DMA1_REVID      DMA2PLB6_1_BASE + 0x47
#define PLB6_DMA1_ARB        DMA2PLB6_1_BASE + 0x48
#define PLB6_DMA1_OPTIONS    DMA2PLB6_1_BASE + 0x49
#define PLB6_DMA1_PERM       DMA2PLB6_1_BASE + 0x4A
#define PLB6_DMA1_PNC        DMA2PLB6_1_BASE + 0x4B

//DEFAULT VALUES
// from trunk/cpu/units/dma2plb6_v001/verilog/rtl/DMA2PLB6.defines.v
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

//Table 2-1
#define P6DMA0_PNC_VALUE    0x41
#define P6DMA1_PNC_VALUE    0xE0


.section ".BOOT_ROM.text","ax",@progbits

.global image_start

image_start:

//PLB6_DMA_0
    check_value r0,r1,r2, PLB6_DMA0_CR0, CR0_VALUE, "PLB6_DMA0_CR0"
    check_value r0,r1,r2, PLB6_DMA0_CTC0, CTC0_VALUE, "PLB6_DMA0_CTC0"
    check_value r0,r1,r2, PLB6_DMA0_DSTR0, DSTR0_VALUE, "PLB6_DMA0_DSTR0"
    check_value r0,r1,r2, PLB6_DMA0_SAH0, SAH0_VALUE, "PLB6_DMA0_SAH0"
    check_value r0,r1,r2, PLB6_DMA0_SAL0, SAL0_VALUE, "PLB6_DMA0_SAL0"
    check_value r0,r1,r2, PLB6_DMA0_DAH0, DAH0_VALUE, "PLB6_DMA0_DAH0"
    check_value r0,r1,r2, PLB6_DMA0_DAL0, DAL0_VALUE, "PLB6_DMA0_DAL0"
    check_value r0,r1,r2, PLB6_DMA0_SGH0, SGH0_VALUE, "PLB6_DMA0_SGH0"
    check_value r0,r1,r2, PLB6_DMA0_SGL0, SGL0_VALUE, "PLB6_DMA0_SGL0"
    check_value r0,r1,r2, PLB6_DMA0_SGC0, SGC0_VALUE, "PLB6_DMA0_SGC0"
    check_value r0,r1,r2, PLB6_DMA0_CHRES0, CHRES0_VALUE, "PLB6_DMA0_CHRES0"
    check_value r0,r1,r2, PLB6_DMA0_CHLOCK0, CHLOCK0_VALUE, "PLB6_DMA0_CHLOCK0"
    check_value r0,r1,r2, PLB6_DMA0_SGCA0, SGCA0_VALUE, "PLB6_DMA0_SGCA0"

    check_value r0,r1,r2, PLB6_DMA0_CR1, CR1_VALUE, "PLB6_DMA0_CR1"
    check_value r0,r1,r2, PLB6_DMA0_CTC1, CTC1_VALUE, "PLB6_DMA0_CTC1"
    check_value r0,r1,r2, PLB6_DMA0_DSTR1, DSTR1_VALUE, "PLB6_DMA0_DSTR1"
    check_value r0,r1,r2, PLB6_DMA0_SAH1, SAH1_VALUE, "PLB6_DMA0_SAH1"
    check_value r0,r1,r2, PLB6_DMA0_SAL1, SAL1_VALUE, "PLB6_DMA0_SAL1"
    check_value r0,r1,r2, PLB6_DMA0_DAH1, DAH1_VALUE, "PLB6_DMA0_DAH1"
    check_value r0,r1,r2, PLB6_DMA0_DAL1, DAL1_VALUE, "PLB6_DMA0_DAL1"
    check_value r0,r1,r2, PLB6_DMA0_SGH1, SGH1_VALUE, "PLB6_DMA0_SGH1"
    check_value r0,r1,r2, PLB6_DMA0_SGL1, SGL1_VALUE, "PLB6_DMA0_SGL1"
    check_value r0,r1,r2, PLB6_DMA0_SGC1, SGC1_VALUE, "PLB6_DMA0_SGC1"
    check_value r0,r1,r2, PLB6_DMA0_CHRES1, CHRES1_VALUE, "PLB6_DMA0_CHRES1"
    check_value r0,r1,r2, PLB6_DMA0_CHLOCK1, CHLOCK1_VALUE, "PLB6_DMA0_CHLOCK1"
    check_value r0,r1,r2, PLB6_DMA0_SGCA1, SGCA1_VALUE, "PLB6_DMA0_SGCA1"

    check_value r0,r1,r2, PLB6_DMA0_CR2, CR2_VALUE, "PLB6_DMA0_CR2"
    check_value r0,r1,r2, PLB6_DMA0_CTC2, CTC2_VALUE, "PLB6_DMA0_CTC2"
    check_value r0,r1,r2, PLB6_DMA0_DSTR2, DSTR2_VALUE, "PLB6_DMA0_DSTR2"
    check_value r0,r1,r2, PLB6_DMA0_SAH2, SAH2_VALUE, "PLB6_DMA0_SAH2"
    check_value r0,r1,r2, PLB6_DMA0_SAL2, SAL2_VALUE, "PLB6_DMA0_SAL2"
    check_value r0,r1,r2, PLB6_DMA0_DAH2, DAH2_VALUE, "PLB6_DMA0_DAH2"
    check_value r0,r1,r2, PLB6_DMA0_DAL2, DAL2_VALUE, "PLB6_DMA0_DAL2"
    check_value r0,r1,r2, PLB6_DMA0_SGH2, SGH2_VALUE, "PLB6_DMA0_SGH2"
    check_value r0,r1,r2, PLB6_DMA0_SGL2, SGL2_VALUE, "PLB6_DMA0_SGL2"
    check_value r0,r1,r2, PLB6_DMA0_SGC2, SGC2_VALUE, "PLB6_DMA0_SGC2"
    check_value r0,r1,r2, PLB6_DMA0_CHRES2, CHRES2_VALUE, "PLB6_DMA0_CHRES2"
    check_value r0,r1,r2, PLB6_DMA0_CHLOCK2, CHLOCK2_VALUE, "PLB6_DMA0_CHLOCK2"
    check_value r0,r1,r2, PLB6_DMA0_SGCA2, SGCA2_VALUE, "PLB6_DMA0_SGCA2"

    check_value r0,r1,r2, PLB6_DMA0_CR3, CR3_VALUE, "PLB6_DMA0_CR3"
    check_value r0,r1,r2, PLB6_DMA0_CTC3, CTC3_VALUE, "PLB6_DMA0_CTC3"
    check_value r0,r1,r2, PLB6_DMA0_DSTR3, DSTR3_VALUE, "PLB6_DMA0_DSTR3"
    check_value r0,r1,r2, PLB6_DMA0_SAH3, SAH3_VALUE, "PLB6_DMA0_SAH3"
    check_value r0,r1,r2, PLB6_DMA0_SAL3, SAL3_VALUE, "PLB6_DMA0_SAL3"
    check_value r0,r1,r2, PLB6_DMA0_DAH3, DAH3_VALUE, "PLB6_DMA0_DAH3"
    check_value r0,r1,r2, PLB6_DMA0_DAL3, DAL3_VALUE, "PLB6_DMA0_DAL3"
    check_value r0,r1,r2, PLB6_DMA0_SGH3, SGH3_VALUE, "PLB6_DMA0_SGH3"
    check_value r0,r1,r2, PLB6_DMA0_SGL3, SGL3_VALUE, "PLB6_DMA0_SGL3"
    check_value r0,r1,r2, PLB6_DMA0_SGC3, SGC3_VALUE, "PLB6_DMA0_SGC3"
    check_value r0,r1,r2, PLB6_DMA0_CHRES3, CHRES3_VALUE, "PLB6_DMA0_CHRES3"
    check_value r0,r1,r2, PLB6_DMA0_CHLOCK3, CHLOCK3_VALUE, "PLB6_DMA0_CHLOCK3"
    check_value r0,r1,r2, PLB6_DMA0_SGCA3, SGCA3_VALUE, "PLB6_DMA0_SGCA3"

    check_value r0,r1,r2, PLB6_DMA0_SR, SR_VALUE, "PLB6_DMA0_SR"
    check_value r0,r1,r2, PLB6_DMA0_MPEAH, MPEAH_VALUE, "PLB6_DMA0_MPEAH"
    check_value r0,r1,r2, PLB6_DMA0_PMEAL, PMEAL_VALUE, "PLB6_DMA0_PMEAL"
    check_value r0,r1,r2, PLB6_DMA0_PSE, PSE_VALUE, "PLB6_DMA0_PSE"
    check_value r0,r1,r2, PLB6_DMA0_PSEAH, PSEAH_VALUE, "PLB6_DMA0_PSEAH"
    check_value r0,r1,r2, PLB6_DMA0_PSEAL, PSEAL_VALUE, "PLB6_DMA0_PSEAL"
    check_value r0,r1,r2, PLB6_DMA0_CRS, CRS_VALUE, "PLB6_DMA0_CRS"
    check_value r0,r1,r2, PLB6_DMA0_REVID, REVID_VALUE, "PLB6_DMA0_REVID"
    check_value r0,r1,r2, PLB6_DMA0_ARB, ARB_VALUE, "PLB6_DMA0_ARB"
    check_value r0,r1,r2, PLB6_DMA0_OPTIONS, OPTIONS_VALUE, "PLB6_DMA0_OPTIONS"
    check_value r0,r1,r2, PLB6_DMA0_PERM, PERM_VALUE, "PLB6_DMA0_PERM"
    check_value r0,r1,r2, PLB6_DMA0_PNC, P6DMA0_PNC_VALUE, "PLB6_DMA0_PNC"

//PLB6_DMA_1
     check_value r0,r1,r2, PLB6_DMA1_CR0, CR0_VALUE, "PLB6_DMA1_CR0"
    check_value r0,r1,r2, PLB6_DMA1_CTC0, CTC0_VALUE, "PLB6_DMA1_CTC0"
    check_value r0,r1,r2, PLB6_DMA1_DSTR0, DSTR0_VALUE, "PLB6_DMA1_DSTR0"
    check_value r0,r1,r2, PLB6_DMA1_SAH0, SAH0_VALUE, "PLB6_DMA1_SAH0"
    check_value r0,r1,r2, PLB6_DMA1_SAL0, SAL0_VALUE, "PLB6_DMA1_SAL0"
    check_value r0,r1,r2, PLB6_DMA1_DAH0, DAH0_VALUE, "PLB6_DMA1_DAH0"
    check_value r0,r1,r2, PLB6_DMA1_DAL0, DAL0_VALUE, "PLB6_DMA1_DAL0"
    check_value r0,r1,r2, PLB6_DMA1_SGH0, SGH0_VALUE, "PLB6_DMA1_SGH0"
    check_value r0,r1,r2, PLB6_DMA1_SGL0, SGL0_VALUE, "PLB6_DMA1_SGL0"
    check_value r0,r1,r2, PLB6_DMA1_SGC0, SGC0_VALUE, "PLB6_DMA1_SGC0"
    check_value r0,r1,r2, PLB6_DMA1_CHRES0, CHRES0_VALUE, "PLB6_DMA1_CHRES0"
    check_value r0,r1,r2, PLB6_DMA1_CHLOCK0, CHLOCK0_VALUE, "PLB6_DMA1_CHLOCK0"
    check_value r0,r1,r2, PLB6_DMA1_SGCA0, SGCA0_VALUE, "PLB6_DMA1_SGCA0"

    check_value r0,r1,r2, PLB6_DMA1_CR1, CR1_VALUE, "PLB6_DMA1_CR1"
    check_value r0,r1,r2, PLB6_DMA1_CTC1, CTC1_VALUE, "PLB6_DMA1_CTC1"
    check_value r0,r1,r2, PLB6_DMA1_DSTR1, DSTR1_VALUE, "PLB6_DMA1_DSTR1"
    check_value r0,r1,r2, PLB6_DMA1_SAH1, SAH1_VALUE, "PLB6_DMA1_SAH1"
    check_value r0,r1,r2, PLB6_DMA1_SAL1, SAL1_VALUE, "PLB6_DMA1_SAL1"
    check_value r0,r1,r2, PLB6_DMA1_DAH1, DAH1_VALUE, "PLB6_DMA1_DAH1"
    check_value r0,r1,r2, PLB6_DMA1_DAL1, DAL1_VALUE, "PLB6_DMA1_DAL1"
    check_value r0,r1,r2, PLB6_DMA1_SGH1, SGH1_VALUE, "PLB6_DMA1_SGH1"
    check_value r0,r1,r2, PLB6_DMA1_SGL1, SGL1_VALUE, "PLB6_DMA1_SGL1"
    check_value r0,r1,r2, PLB6_DMA1_SGC1, SGC1_VALUE, "PLB6_DMA1_SGC1"
    check_value r0,r1,r2, PLB6_DMA1_CHRES1, CHRES1_VALUE, "PLB6_DMA1_CHRES1"
    check_value r0,r1,r2, PLB6_DMA1_CHLOCK1, CHLOCK1_VALUE, "PLB6_DMA1_CHLOCK1"
    check_value r0,r1,r2, PLB6_DMA1_SGCA1, SGCA1_VALUE, "PLB6_DMA1_SGCA1"

    check_value r0,r1,r2, PLB6_DMA1_CR2, CR2_VALUE, "PLB6_DMA1_CR2"
    check_value r0,r1,r2, PLB6_DMA1_CTC2, CTC2_VALUE, "PLB6_DMA1_CTC2"
    check_value r0,r1,r2, PLB6_DMA1_DSTR2, DSTR2_VALUE, "PLB6_DMA1_DSTR2"
    check_value r0,r1,r2, PLB6_DMA1_SAH2, SAH2_VALUE, "PLB6_DMA1_SAH2"
    check_value r0,r1,r2, PLB6_DMA1_SAL2, SAL2_VALUE, "PLB6_DMA1_SAL2"
    check_value r0,r1,r2, PLB6_DMA1_DAH2, DAH2_VALUE, "PLB6_DMA1_DAH2"
    check_value r0,r1,r2, PLB6_DMA1_DAL2, DAL2_VALUE, "PLB6_DMA1_DAL2"
    check_value r0,r1,r2, PLB6_DMA1_SGH2, SGH2_VALUE, "PLB6_DMA1_SGH2"
    check_value r0,r1,r2, PLB6_DMA1_SGL2, SGL2_VALUE, "PLB6_DMA1_SGL2"
    check_value r0,r1,r2, PLB6_DMA1_SGC2, SGC2_VALUE, "PLB6_DMA1_SGC2"
    check_value r0,r1,r2, PLB6_DMA1_CHRES2, CHRES2_VALUE, "PLB6_DMA1_CHRES2"
    check_value r0,r1,r2, PLB6_DMA1_CHLOCK2, CHLOCK2_VALUE, "PLB6_DMA1_CHLOCK2"
    check_value r0,r1,r2, PLB6_DMA1_SGCA2, SGCA2_VALUE, "PLB6_DMA1_SGCA2"

    check_value r0,r1,r2, PLB6_DMA1_CR3, CR3_VALUE, "PLB6_DMA1_CR3"
    check_value r0,r1,r2, PLB6_DMA1_CTC3, CTC3_VALUE, "PLB6_DMA1_CTC3"
    check_value r0,r1,r2, PLB6_DMA1_DSTR3, DSTR3_VALUE, "PLB6_DMA1_DSTR3"
    check_value r0,r1,r2, PLB6_DMA1_SAH3, SAH3_VALUE, "PLB6_DMA1_SAH3"
    check_value r0,r1,r2, PLB6_DMA1_SAL3, SAL3_VALUE, "PLB6_DMA1_SAL3"
    check_value r0,r1,r2, PLB6_DMA1_DAH3, DAH3_VALUE, "PLB6_DMA1_DAH3"
    check_value r0,r1,r2, PLB6_DMA1_DAL3, DAL3_VALUE, "PLB6_DMA1_DAL3"
    check_value r0,r1,r2, PLB6_DMA1_SGH3, SGH3_VALUE, "PLB6_DMA1_SGH3"
    check_value r0,r1,r2, PLB6_DMA1_SGL3, SGL3_VALUE, "PLB6_DMA1_SGL3"
    check_value r0,r1,r2, PLB6_DMA1_SGC3, SGC3_VALUE, "PLB6_DMA1_SGC3"
    check_value r0,r1,r2, PLB6_DMA1_CHRES3, CHRES3_VALUE, "PLB6_DMA1_CHRES3"
    check_value r0,r1,r2, PLB6_DMA1_CHLOCK3, CHLOCK3_VALUE, "PLB6_DMA1_CHLOCK3"
    check_value r0,r1,r2, PLB6_DMA1_SGCA3, SGCA3_VALUE, "PLB6_DMA1_SGCA3"

    check_value r0,r1,r2, PLB6_DMA1_SR, SR_VALUE, "PLB6_DMA1_SR"
    check_value r0,r1,r2, PLB6_DMA1_MPEAH, MPEAH_VALUE, "PLB6_DMA1_MPEAH"
    check_value r0,r1,r2, PLB6_DMA1_PMEAL, PMEAL_VALUE, "PLB6_DMA1_PMEAL"
    check_value r0,r1,r2, PLB6_DMA1_PSE, PSE_VALUE, "PLB6_DMA1_PSE"
    check_value r0,r1,r2, PLB6_DMA1_PSEAH, PSEAH_VALUE, "PLB6_DMA1_PSEAH"
    check_value r0,r1,r2, PLB6_DMA1_PSEAL, PSEAL_VALUE, "PLB6_DMA1_PSEAL"
    check_value r0,r1,r2, PLB6_DMA1_CRS, CRS_VALUE, "PLB6_DMA1_CRS"
    check_value r0,r1,r2, PLB6_DMA1_REVID, REVID_VALUE, "PLB6_DMA1_REVID"
    check_value r0,r1,r2, PLB6_DMA1_ARB, ARB_VALUE, "PLB6_DMA1_ARB"
    check_value r0,r1,r2, PLB6_DMA1_OPTIONS, OPTIONS_VALUE, "PLB6_DMA1_OPTIONS"
    check_value r0,r1,r2, PLB6_DMA1_PERM, PERM_VALUE, "PLB6_DMA1_PERM"
    check_value r0,r1,r2, PLB6_DMA1_PNC, P6DMA1_PNC_VALUE, "PLB6_DMA1_PNC"

    trace_msg "CHECK DEFAULT VALUES DONE\n"

running_0_1_dma0:
    load_const r1, 0x00000000
    load_const r0, PLB6_DMA0_SAH0
    mtdcrx r0, r1
    mfdcrx r2, r0
    cmp cr7,0, r2,r1
    TEST_ASSERT(eq,cr7,"Read/write error in PLB6_DMA0_SAH0 (running 1 init)")

dma0_running_1:
    load_const r1, 0x00000001
    load_const r3, 1
    load_const r4, 0x80000000
dma0_loop_1:
    mtdcrx r0, r1
    mfdcrx r2, r0
    cmp cr7,0, r2,r1
    TEST_ASSERT(eq,cr7,"Read/write error in PLB6_DMA0_SAH0 (running 1)")
    cmp 0, 0, r1, r4
    beq dma0_running_0
    slw r1, r1, r3
    b dma0_loop_1

dma0_running_0:
    load_const r1, 0xFFFFFFFF
    load_const r0, PLB6_DMA0_SAH0
    mtdcrx r0, r1
    mfdcrx r2, r0
    cmp cr7,0, r2,r1
    TEST_ASSERT(eq,cr7,"Read/write error in PLB6_DMA0_SAH0 (running 0 init)")

    load_const r1, 0xFFFFFFFE
    load_const r3, 1
    load_const r4, 0x7FFFFFFF
dma0_loop_0:
    mtdcrx r0, r1
    mfdcrx r2, r0
    cmp cr7,0, r2,r1
    TEST_ASSERT(eq,cr7,"Read/write error in PLB6_DMA0_SAH0 (running 0)")
    cmp 0, 0, r1, r4
    beq dma0_done
    slw r1, r1, r3
    ori r1, r1, 1
    b dma0_loop_0

dma0_done:
    trace_msg "PLB6_DMA_0 running 0/1 done\n"

/////////////////////////////////////////////////////////////////////////////

running_0_1_dma1:
    load_const r1, 0x00000000
    load_const r0, PLB6_DMA1_SAH0
    mtdcrx r0, r1
    mfdcrx r2, r0
    cmp cr7,0, r2,r1
    TEST_ASSERT(eq,cr7,"Read/write error in PLB6_DMA1_SAH0 (running 1 init)")

dma1_running_1:
    load_const r1, 0x00000001
    load_const r3, 1
    load_const r4, 0x80000000
dma1_loop_1:
    mtdcrx r0, r1
    mfdcrx r2, r0
    cmp cr7,0, r2,r1
    TEST_ASSERT(eq,cr7,"Read/write error in PLB6_DMA1_SAH0 (running 1)")
    cmp 0, 0, r1, r4
    beq dma1_running_0
    slw r1, r1, r3
    b dma1_loop_1

dma1_running_0:
    load_const r1, 0xFFFFFFFF
    load_const r0, PLB6_DMA1_SAH0
    mtdcrx r0, r1
    mfdcrx r2, r0
    cmp cr7,0, r2,r1
    TEST_ASSERT(eq,cr7,"Read/write error in PLB6_DMA1_SAH0 (running 0 init)")

    load_const r1, 0xFFFFFFFE
    load_const r3, 1
    load_const r4, 0x7FFFFFFF
dma1_loop_0:
    mtdcrx r0, r1
    mfdcrx r2, r0
    cmp cr7,0, r2,r1
    TEST_ASSERT(eq,cr7,"Read/write error in PLB6_DMA1_SAH0 (running 0)")
    cmp 0, 0, r1, r4
    beq dma1_done
    slw r1, r1, r3
    ori r1, r1, 1
    b dma1_loop_0

dma1_done:
    trace_msg "PLB6_DMA_1 running 0/1 done\n"


test_ok:
    test_event EVENT_OK
    b finish

error:
     test_event EVENT_ERROR

finish:
    b finish
