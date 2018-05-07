/*
 * reg_plb4plb6.h
 *
 *  Created on: May 4, 2018
 *      Author: d.arsentiev
 */

#ifndef REG_PLB4PLB6_H_
#define REG_PLB4PLB6_H_

/* P4P6_0 */
#define P4P6_0_P46CR          (DCR_PLB4PLB6_0_BASE + P46CR)
#define P4P6_0_ESR            (DCR_PLB4PLB6_0_BASE + ESR)
#define P4P6_0_P46REV         (DCR_PLB4PLB6_0_BASE + P46REV)
#define P4P6_0_EARH           (DCR_PLB4PLB6_0_BASE + EARH)
#define P4P6_0_EARL           (DCR_PLB4PLB6_0_BASE + EARL)
#define P4P6_0_SNOOP_UADDRH0  (DCR_PLB4PLB6_0_BASE + SNOOP_UADDRH0)
#define P4P6_0_SNOOP_LADDRH0  (DCR_PLB4PLB6_0_BASE + SNOOP_LADDRH0)
#define P4P6_0_SNOOP_UADDRL0  (DCR_PLB4PLB6_0_BASE + SNOOP_UADDRL0)
#define P4P6_0_SNOOP_LADDRL0  (DCR_PLB4PLB6_0_BASE + SNOOP_LADDRL0)
#define P4P6_0_SNOOP_UADDRH1  (DCR_PLB4PLB6_0_BASE + SNOOP_UADDRH1)
#define P4P6_0_SNOOP_LADDRH1  (DCR_PLB4PLB6_0_BASE + SNOOP_LADDRH1)
#define P4P6_0_SNOOP_UADDRL1  (DCR_PLB4PLB6_0_BASE + SNOOP_UADDRL1)
#define P4P6_0_SNOOP_LADDRL1  (DCR_PLB4PLB6_0_BASE + SNOOP_LADDRL1)
#define P4P6_0_TESR           (DCR_PLB4PLB6_0_BASE + TESR)

/*from /trunk/cpu/units/plb4plb6_v001/verilog/rtl/PLB4PLB6.defines.v */
#define PLB4PLB6_WATERMARK_VALUE 0x00000112
#define P4P6_0_I_M_SELF_ID          0x2
#define P4P6_1_I_M_SELF_ID          0x5
#define P4P6_2_I_M_SELF_ID          0x6
/* See table  2-5 plb4to6bridge_ds_v14_2012SEP17_pub.pdf */
#define P4P6_0_PNCR_VALUE           0x0E
/* See table  2-5 plb4to6bridge_ds_v14_2012SEP17_pub.pdf */
#define P4P6_1_PNCR_VALUE           0xBB
/* See table  2-5 plb4to6bridge_ds_v14_2012SEP17_pub.pdf */
#define P4P6_2_PNCR_VALUE           0x8A

#define P46_0_CR_VALUE          reg_field(31,P4P6_0_PNCR_VALUE) |\
								reg_field(21,0x1)   |\
								reg_field(20,0x1)   |\
								reg_field(19,0)     |\
								reg_field(18,0b011) |\
								reg_field(5,0b001)  |\
								reg_field(2,0b001)
#define P46_1_CR_VALUE          reg_field(31,P4P6_1_PNCR_VALUE) |\
								reg_field(21,0x1)   |\
								reg_field(20,0x1)   |\
								reg_field(19,0)     |\
								reg_field(18,0b011) |\
								reg_field(5,0b001)  |\
								reg_field(2,0b001)
#define P46_2_CR_VALUE          reg_field(31,P4P6_2_PNCR_VALUE) |\
								reg_field(21,0x1)   |\
								reg_field(20,0x1)   |\
								reg_field(19,0)     |\
								reg_field(18,0b011) |\
								reg_field(5,0b001)  |\
								reg_field(2,0b001)


#endif /* REG_PLB4PLB6_H_ */
