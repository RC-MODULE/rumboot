/*
 * regs_plb6dma.h
 *
 *  Created on: May 7, 2018
 *      Author: d.arsentiev
 */

#ifndef REGS_PLB6DMA_H_
#define REGS_PLB6DMA_H_

#define CR0        0x00
#define CTC0       0x01
#define DSTR0      0x02
#define SAH0       0x03
#define SAL0       0x04
#define DAH0       0x05
#define DAL0       0x06
#define SGH0       0x07
#define SGL0       0x08
#define SGC0       0x09
#define CHRES0     0x0A
#define CHLOCK0    0x0B
#define SGCA0      0x0C
#define CR1        0x10
#define CTC1       0x11
#define DSTR1      0x12
#define SAH1       0x13
#define SAL1       0x14
#define DAH1       0x15
#define DAL1       0x16
#define SGH1       0x17
#define SGL1       0x18
#define SGC1       0x19
#define CHRES1     0x1A
#define CHLOCK1    0x1B
#define SGCA1      0x1C
#define CR2        0x20
#define CTC2       0x21
#define DSTR2      0x22
#define SAH2       0x23
#define SAL2       0x24
#define DAH2       0x25
#define DAL2       0x26
#define SGH2       0x27
#define SGL2       0x28
#define SGC2       0x29
#define CHRES2     0x2A
#define CHLOCK2    0x2B
#define SGCA2      0x2C
#define CR3        0x30
#define CTC3       0x31
#define DSTR3      0x32
#define SAH3       0x33
#define SAL3       0x34
#define DAH3       0x35
#define DAL3       0x36
#define SGH3       0x37
#define SGL3       0x38
#define SGC3       0x39
#define CHRES3     0x3A
#define CHLOCK3    0x3B
#define SGCA3      0x3C
#define SR         0x40
#define MPEAH      0x41
#define PMEAL      0x42
#define PSE        0x43
#define PSEAH      0x44
#define PSEAL      0x45
#define CRS        0x46
#define REVID      0x47
#define ARB        0x48
#define OPTIONS    0x49
#define PERM       0x4A
#define PNC        0x4B

/* DEFAULT VALUES */
/* from trunk/cpu/units/dma2plb6_v001/verilog/rtl/DMA2PLB6.defines.v */
#define DMA2PLB6_REV_NBR_VALUE        0x001
#define DMA2PLB6_BRANCH_REV_NBR_VALUE  0x04

#define CR0_VALUE       	0x00000000
#define CTC0_VALUE      	0x00000000
#define DSTR0_VALUE     	0x00000000
#define SAH0_VALUE      	0x00000000
#define SAL0_VALUE      	0x00000000
#define DAH0_VALUE      	0x00000000
#define DAL0_VALUE      	0x00000000
#define SGH0_VALUE      	0x00000000
#define SGL0_VALUE      	0x00000000
#define SGC0_VALUE      	0x00000000
#define CHRES0_VALUE    	0x00000000
#define CHLOCK0_VALUE   	0x00000000
#define SGCA0_VALUE     	0x00000000
#define CR1_VALUE       	0x00000000
#define CTC1_VALUE      	0x00000000
#define DSTR1_VALUE     	0x00000000
#define SAH1_VALUE      	0x00000000
#define SAL1_VALUE      	0x00000000
#define DAH1_VALUE      	0x00000000
#define DAL1_VALUE      	0x00000000
#define SGH1_VALUE      	0x00000000
#define SGL1_VALUE      	0x00000000
#define SGC1_VALUE      	0x00000000
#define CHRES1_VALUE    	0x00000000
#define CHLOCK1_VALUE   	0x00000000
#define SGCA1_VALUE     	0x00000000
#define CR2_VALUE       	0x00000000
#define CTC2_VALUE      	0x00000000
#define DSTR2_VALUE     	0x00000000
#define SAH2_VALUE      	0x00000000
#define SAL2_VALUE      	0x00000000
#define DAH2_VALUE      	0x00000000
#define DAL2_VALUE      	0x00000000
#define SGH2_VALUE      	0x00000000
#define SGL2_VALUE      	0x00000000
#define SGC2_VALUE      	0x00000000
#define CHRES2_VALUE    	0x00000000
#define CHLOCK2_VALUE   	0x00000000
#define SGCA2_VALUE     	0x00000000
#define CR3_VALUE       	0x00000000
#define CTC3_VALUE      	0x00000000
#define DSTR3_VALUE     	0x00000000
#define SAH3_VALUE      	0x00000000
#define SAL3_VALUE      	0x00000000
#define DAH3_VALUE      	0x00000000
#define DAL3_VALUE      	0x00000000
#define SGH3_VALUE      	0x00000000
#define SGL3_VALUE      	0x00000000
#define SGC3_VALUE      	0x00000000
#define CHRES3_VALUE    	0x00000000
#define CHLOCK3_VALUE   	0x00000000
#define SGCA3_VALUE     	0x00000000
#define SR_VALUE        	0x00000000
#define MPEAH_VALUE     	0x00000000
#define PMEAL_VALUE     	0x00000000
#define PSE_VALUE       	0x00000000
#define PSEAH_VALUE     	0x00000000
#define PSEAL_VALUE     	0x00000000
#define CRS_VALUE       	0x00000000
#define REVID_VALUE     	reg_field(31,DMA2PLB6_BRANCH_REV_NBR_VALUE) |\
							reg_field(23,DMA2PLB6_REV_NBR_VALUE)
#define ARB_VALUE       	0x00000000
#define OPTIONS_VALUE   	0x00010000
#define PERM_VALUE      	0xFFFF0000

/* Table 2-1 */

#define P6DMA0_PNC_VALUE    0x75

/*
#define P6DMA0_PNC_VALUE    0x41
#define P6DMA1_PNC_VALUE    0xE0
*/

#endif /* REGS_PLB6DMA_H_ */
