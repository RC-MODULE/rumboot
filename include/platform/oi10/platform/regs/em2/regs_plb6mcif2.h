/*
 * regs_em2_plb6mcif2.h
 *
 *  Created on: June 25, 2018
 *      Author: s.chernousov
 */

#ifndef REGS_PLB6MCIF2_H_
#define REGS_PLB6MCIF2_H_

#define PLB6MCIF2_BESR_read	 0x00
#define PLB6MCIF2_BESR_clear 0x00
#define PLB6MCIF2_BESR_set	 0x01
#define PLB6MCIF2_BEARL		0x02
#define PLB6MCIF2_BEARU		0x03
#define PLB6MCIF2_INTR_EN	0x04
#define PLB6MCIF2_PLBASYNC	0x07
#define PLB6MCIF2_PLBORD	0x08
#define PLB6MCIF2_PLBCFG	0x09
#define PLB6MCIF2_MAXMEM	0x0F
#define PLB6MCIF2_PUABA		0x10
#define PLB6MCIF2_MR0CF		0x11
#define PLB6MCIF2_MR1CF		0x12
#define PLB6MCIF2_MR2CF		0x13
#define PLB6MCIF2_MR3CF		0x14
#define PLB6MCIF2_SR0CF		0x15
#define PLB6MCIF2_SR1CF		0x16
#define PLB6MCIF2_SR2CF		0x17
#define PLB6MCIF2_SR3CF		0x18
#define PLB6MCIF2_P6BMTAG1	0x1D
#define PLB6MCIF2_P6BMTAG2	0x1E
#define PLB6MCIF2_P6BMTAG3	0x1F
#define PLB6MCIF2_STATUS	0x20
#define PLB6MCIF2_RID		0x21
#define PLB6MCIF2_MAP0CF	0x22
#define PLB6MCIF2_MAP1CF	0x23
#define PLB6MCIF2_MAP2CF	0x24
#define PLB6MCIF2_MAP3CF	0x25

#define PLB6MCIF2_WATERMARK	0x00000707

#define PLB6MCIF2_MEMRANK_N 4

BEGIN_ENUM( PLB6MCIF2_REG_DFLT )
DECLARE_ENUM_VAL( PLB6MCIF2_BEARL_DFLT,     0x00000000 )
DECLARE_ENUM_VAL( PLB6MCIF2_BEARU_DFLT,     0x00000000 )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_DFLT,      0x00000000 )
DECLARE_ENUM_VAL( PLB6MCIF2_INTR_EN_DFLT,   0xFFE001C0 )
DECLARE_ENUM_VAL( PLB6MCIF2_MAPxCF_DFLT,    0x00000000 )
DECLARE_ENUM_VAL( PLB6MCIF2_MAXMEM_DFLT,    0xC0000000 )
DECLARE_ENUM_VAL( PLB6MCIF2_MRxCF_DFLT,     0x00000000 )
DECLARE_ENUM_VAL( PLB6MCIF2_P6BMTAG1_DFLT,  0x00000000 )
DECLARE_ENUM_VAL( PLB6MCIF2_P6BMTAG2_DFLT,  0x00000000 )
DECLARE_ENUM_VAL( PLB6MCIF2_P6BMTAG3_DFLT,  0x00000000 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBASYNC_DFLT,  0x00000000 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBCFG_DFLT,    0x200000F0 )
DECLARE_ENUM_VAL( PLB6MCIF2_PUABA_DFLT,     0x00000000 )
DECLARE_ENUM_VAL( PLB6MCIF2_SRxCF_DFLT,     0x00000000 )
DECLARE_ENUM_VAL( PLB6MCIF2_STATUS_DFLT,    0x80000000 )
DECLARE_ENUM_VAL( PLB6MCIF2_RID_DFLT,       PLB6MCIF2_WATERMARK )
END_ENUM( PLB6MCIF2_REG_DFLT )

BEGIN_ENUM( PLB6MCIF2_REG_MSK )
DECLARE_ENUM_VAL( PLB6MCIF2_MAXMEM_MSK,     0xC0000000 )
DECLARE_ENUM_VAL( PLB6MCIF2_MRxCF_MSK,      0xFFF8F001 )
DECLARE_ENUM_VAL( PLB6MCIF2_P6BMTAG1_MSK,   0xFFFFFFFC )
DECLARE_ENUM_VAL( PLB6MCIF2_P6BMTAG2_MSK,   0xFFFFFFFC )
DECLARE_ENUM_VAL( PLB6MCIF2_P6BMTAG3_MSK,   0xFFFFF000 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBASYNC_MSK,   0xCFFFFF00 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBORD_MSK,     0xF100E000 )
DECLARE_ENUM_VAL( PLB6MCIF2_SRxCF_MSK,      0xFFF8F001 )
DECLARE_ENUM_VAL( PLB6MCIF2_STATUS_MSK,     0x80000000 )
END_ENUM( PLB6MCIF2_REG_MSK )

#endif /* REGS_PLB6MCIF2_H_ */
