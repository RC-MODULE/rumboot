#ifndef REGS_DMA2PLB6_H__
#define REGS_DMA2PLB6_H__

#include <arch/common_macros.h>
#include <arch/ibm_bit_ordering_macros.h>
#include <arch/regs/fields/dma2plb6.h>


#define CHANNEL_ADDR_OFF    0x10

//CHANNEL_REG_OFFSETS
#define PLB6_DMA_CR        0x00
#define PLB6_DMA_CTC       0x01
#define PLB6_DMA_DSTR      0x02
#define PLB6_DMA_SAH       0x03
#define PLB6_DMA_SAL       0x04
#define PLB6_DMA_DAH       0x05
#define PLB6_DMA_DAL       0x06
#define PLB6_DMA_SGH       0x07
#define PLB6_DMA_SGL       0x08
#define PLB6_DMA_SGC       0x09
#define PLB6_DMA_CHRES     0x0A
#define PLB6_DMA_CHLOCK    0x0B //RC
#define PLB6_DMA_SGCA      0x0C


//DMA2PLB6_REGS
#define PLB6_DMA_CR0         0x00
#define PLB6_DMA_CTC0        0x01
#define PLB6_DMA_DSTR0       0x02
#define PLB6_DMA_SAH0        0x03
#define PLB6_DMA_SAL0        0x04
#define PLB6_DMA_DAH0        0x05
#define PLB6_DMA_DAL0        0x06
#define PLB6_DMA_SGH0        0x07
#define PLB6_DMA_SGL0        0x08
#define PLB6_DMA_SGC0        0x09
#define PLB6_DMA_CHRES0      0x0A
#define PLB6_DMA_CHLOCK0     0x0B //RC
#define PLB6_DMA_SGCA0       0x0C
#define PLB6_DMA_CR1         0x10
#define PLB6_DMA_CTC1        0x11
#define PLB6_DMA_DSTR1       0x12
#define PLB6_DMA_SAH1        0x13
#define PLB6_DMA_SAL1        0x14
#define PLB6_DMA_DAH1        0x15
#define PLB6_DMA_DAL1        0x16
#define PLB6_DMA_SGH1        0x17
#define PLB6_DMA_SGL1        0x18
#define PLB6_DMA_SGC1        0x19
#define PLB6_DMA_CHRES1      0x1A
#define PLB6_DMA_CHLOCK1     0x1B //RC
#define PLB6_DMA_SGCA1       0x1C
#define PLB6_DMA_CR2         0x20
#define PLB6_DMA_CTC2        0x21
#define PLB6_DMA_DSTR2       0x22
#define PLB6_DMA_SAH2        0x23
#define PLB6_DMA_SAL2        0x24
#define PLB6_DMA_DAH2        0x25
#define PLB6_DMA_DAL2        0x26
#define PLB6_DMA_SGH2        0x27
#define PLB6_DMA_SGL2        0x28
#define PLB6_DMA_SGC2        0x29
#define PLB6_DMA_CHRES2      0x2A
#define PLB6_DMA_CHLOCK2     0x2B //RC
#define PLB6_DMA_SGCA2       0x2C
#define PLB6_DMA_CR3         0x30
#define PLB6_DMA_CTC3        0x31
#define PLB6_DMA_DSTR3       0x32
#define PLB6_DMA_SAH3        0x33
#define PLB6_DMA_SAL3        0x34
#define PLB6_DMA_DAH3        0x35
#define PLB6_DMA_DAL3        0x36
#define PLB6_DMA_SGH3        0x37
#define PLB6_DMA_SGL3        0x38
#define PLB6_DMA_SGC3        0x39
#define PLB6_DMA_CHRES3      0x3A
#define PLB6_DMA_CHLOCK3     0x3B //RC
#define PLB6_DMA_SGCA3       0x3C
#define PLB6_DMA_SR          0x40 //RC
#define PLB6_DMA_PMEAH       0x41 //R
#define PLB6_DMA_PMEAL       0x42 //R
#define PLB6_DMA_PSE         0x43 //RC
#define PLB6_DMA_PSEAH       0x44 //R
#define PLB6_DMA_PSEAL       0x45 //R
#define PLB6_DMA_CRS         0x46 //RC
#define PLB6_DMA_REVID       0x47 //R
#define PLB6_DMA_ARB         0x48
#define PLB6_DMA_OPTIONS     0x49
#define PLB6_DMA_PERM        0x4A
#define PLB6_DMA_PNC         0x4B //R

//default value
#define PLB6_DMA_CR0_DEFAULT          0x00
#define PLB6_DMA_CTC0_DEFAULT         0x00
#define PLB6_DMA_DSTR0_DEFAULT        0x00
#define PLB6_DMA_SAH0_DEFAULT         0x00
#define PLB6_DMA_SAL0_DEFAULT         0x00
#define PLB6_DMA_DAH0_DEFAULT         0x00
#define PLB6_DMA_DAL0_DEFAULT         0x00
#define PLB6_DMA_SGH0_DEFAULT         0x00
#define PLB6_DMA_SGL0_DEFAULT         0x00
#define PLB6_DMA_SGC0_DEFAULT         0x00
#define PLB6_DMA_CHRES0_DEFAULT       0x00
#define PLB6_DMA_CHLOCK0_DEFAULT      0x00
#define PLB6_DMA_SGCA0_DEFAULT        0x00
#define PLB6_DMA_CR1_DEFAULT          0x00
#define PLB6_DMA_CTC1_DEFAULT         0x00
#define PLB6_DMA_DSTR1_DEFAULT        0x00
#define PLB6_DMA_SAH1_DEFAULT         0x00
#define PLB6_DMA_SAL1_DEFAULT         0x00
#define PLB6_DMA_DAH1_DEFAULT         0x00
#define PLB6_DMA_DAL1_DEFAULT         0x00
#define PLB6_DMA_SGH1_DEFAULT         0x00
#define PLB6_DMA_SGL1_DEFAULT         0x00
#define PLB6_DMA_SGC1_DEFAULT         0x00
#define PLB6_DMA_CHRES1_DEFAULT       0x00
#define PLB6_DMA_CHLOCK1_DEFAULT      0x00
#define PLB6_DMA_SGCA1_DEFAULT        0x00
#define PLB6_DMA_CR2_DEFAULT          0x00
#define PLB6_DMA_CTC2_DEFAULT         0x00
#define PLB6_DMA_DSTR2_DEFAULT        0x00
#define PLB6_DMA_SAH2_DEFAULT         0x00
#define PLB6_DMA_SAL2_DEFAULT         0x00
#define PLB6_DMA_DAH2_DEFAULT         0x00
#define PLB6_DMA_DAL2_DEFAULT         0x00
#define PLB6_DMA_SGH2_DEFAULT         0x00
#define PLB6_DMA_SGL2_DEFAULT         0x00
#define PLB6_DMA_SGC2_DEFAULT         0x00
#define PLB6_DMA_CHRES2_DEFAULT       0x00
#define PLB6_DMA_CHLOCK2_DEFAULT      0x00
#define PLB6_DMA_SGCA2_DEFAULT        0x00
#define PLB6_DMA_CR3_DEFAULT          0x00
#define PLB6_DMA_CTC3_DEFAULT         0x00
#define PLB6_DMA_DSTR3_DEFAULT        0x00
#define PLB6_DMA_SAH3_DEFAULT         0x00
#define PLB6_DMA_SAL3_DEFAULT         0x00
#define PLB6_DMA_DAH3_DEFAULT         0x00
#define PLB6_DMA_DAL3_DEFAULT         0x00
#define PLB6_DMA_SGH3_DEFAULT         0x00
#define PLB6_DMA_SGL3_DEFAULT         0x00
#define PLB6_DMA_SGC3_DEFAULT         0x00
#define PLB6_DMA_CHRES3_DEFAULT       0x00
#define PLB6_DMA_CHLOCK3_DEFAULT      0x00
#define PLB6_DMA_SGCA3_DEFAULT        0x00
#define PLB6_DMA_SR_DEFAULT           0x00
#define PLB6_DMA_PMEAH_DEFAULT        0x00
#define PLB6_DMA_PMEAL_DEFAULT        0x00
#define PLB6_DMA_PSE_DEFAULT          0x00
#define PLB6_DMA_PSEAH_DEFAULT        0x00
#define PLB6_DMA_PSEAL_DEFAULT        0x00
#define PLB6_DMA_CRS_DEFAULT          0x00
#define PLB6_DMA_REVID_DEFAULT        0x104
#define PLB6_DMA_ARB_DEFAULT          0x00
#define PLB6_DMA_OPTIONS_DEFAULT      0x10000
#define PLB6_DMA_PERM_DEFAULT         0xFFFF0000
//#define PLB6_DMA_PNC_DEFAULT        ???

//Mask
#define PLB6_DMA_CR0_MSK        0xFF3FF000
#define PLB6_DMA_CTC0_MSK       0x000FFFFF
#define PLB6_DMA_SGC0_MSK       0x80000000
#define PLB6_DMA_CHRES0_MSK     0x80000000
#define PLB6_DMA_CHLOCK0_MSK    0x80000000
#define PLB6_DMA_SGCA0_MSK      0x80000000
#define PLB6_DMA_CR1_MSK        0xFF3FF000
#define PLB6_DMA_CTC1_MSK       0x000FFFFF
#define PLB6_DMA_SGC1_MSK       0x80000000
#define PLB6_DMA_CHRES1_MSK     0x80000000
#define PLB6_DMA_CHLOCK1_MSK    0x80000000
#define PLB6_DMA_SGCA1_MSK      0x80000000
#define PLB6_DMA_CR2_MSK        0xFF3FF000
#define PLB6_DMA_CTC2_MSK       0x000FFFFF
#define PLB6_DMA_SGC2_MSK       0x80000000
#define PLB6_DMA_CHRES2_MSK     0x80000000
#define PLB6_DMA_CHLOCK2_MSK    0x80000000
#define PLB6_DMA_SGCA2_MSK      0x80000000
#define PLB6_DMA_CR3_MSK        0xFF3FF000
#define PLB6_DMA_CTC3_MSK       0x000FFFFF
#define PLB6_DMA_SGC3_MSK       0x80000000
#define PLB6_DMA_CHRES3_MSK     0x80000000
#define PLB6_DMA_CHLOCK3_MSK    0x80000000
#define PLB6_DMA_SGCA3_MSK      0x80000000
#define PLB6_DMA_SR_MSK         0xFFFFFFF0
#define PLB6_DMA_PSE_MSK        0xFFFFC000
#define PLB6_DMA_CRS_MSK        0xF0000000
#define PLB6_DMA_REVID_MSK      0x000FFFFF
#define PLB6_DMA_ARB_MSK        0xC0000000
#define PLB6_DMA_OPTIONS_MSK    0xFFFF8000
#define PLB6_DMA_PERM_MSK       0xFFFF0000
#define PLB6_DMA_PNC_MSK        0x000000FF

#endif // REGS_DMA2PLB6_H__
