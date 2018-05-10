/*
 * reg_plb4plb6.h
 *
 *  Created on: May 4, 2018
 *      Author: d.arsentiev
 */

#ifndef REG_PLB4PLB6_H_
#define REG_PLB4PLB6_H_

/* OFFSET */
#define P46CR          0x0
#define ESR            0x1
#define P46REV         0x2
#define EARH           0x3
#define EARL           0x4
#define SNOOP_UADDRH0  0x5
#define SNOOP_LADDRH0  0x6
#define SNOOP_UADDRL0  0x7
#define SNOOP_LADDRL0  0x8
#define SNOOP_UADDRH1  0x9
#define SNOOP_LADDRH1  0xA
#define SNOOP_UADDRL1  0xB
#define SNOOP_LADDRL1  0xC
#define TESR           0xD /* WO */

/*from /trunk/cpu/units/plb4plb6_v001/verilog/rtl/PLB4PLB6.defines.v */
#define PLB4PLB6_WATERMARK_VALUE	0x00000112
#define P4P6_0_I_M_SELF_ID          0x2
#define P4P6_1_I_M_SELF_ID          0x5
#define P4P6_2_I_M_SELF_ID          0x6
/* See table  2-5 plb4to6bridge_ds_v14_2012SEP17_pub.pdf */
#define P4P6_0_PNCR_VALUE           0x0E
/* See table  2-5 plb4to6bridge_ds_v14_2012SEP17_pub.pdf */
#define P4P6_1_PNCR_VALUE           0xBB
/* See table  2-5 plb4to6bridge_ds_v14_2012SEP17_pub.pdf */
#define P4P6_2_PNCR_VALUE           0x8A

#define P46_0_CR_VALUE          	reg_field(31,P4P6_0_PNCR_VALUE) |\
									reg_field(21,0x1)   |\
									reg_field(20,0x1)   |\
									reg_field(19,0)     |\
									reg_field(18,0b011) |\
									reg_field(5,0b001)  |\
									reg_field(2,0b001)
#define P46_1_CR_VALUE          	reg_field(31,P4P6_1_PNCR_VALUE) |\
									reg_field(21,0x1)   |\
									reg_field(20,0x1)   |\
									reg_field(19,0)     |\
									reg_field(18,0b011) |\
									reg_field(5,0b001)  |\
									reg_field(2,0b001)
#define P46_2_CR_VALUE          	reg_field(31,P4P6_2_PNCR_VALUE) |\
									reg_field(21,0x1)   |\
									reg_field(20,0x1)   |\
									reg_field(19,0)     |\
									reg_field(18,0b011) |\
									reg_field(5,0b001)  |\
									reg_field(2,0b001)

#define ESR_VALUE            		0x00000000
#define P46REV_VALUE         		PLB4PLB6_WATERMARK_VALUE
#define EARH_VALUE           		0x00000000
#define EARL_VALUE           		0x00000000
#define SNOOP_UADDRH0_VALUE  		0x00000000
#define SNOOP_LADDRH0_VALUE  		0x00000000
#define SNOOP_UADDRL0_VALUE  		0x00000000
#define SNOOP_LADDRL0_VALUE  		0x00000000
#define SNOOP_UADDRH1_VALUE			0x00000000
#define SNOOP_LADDRH1_VALUE			0x00000000
#define SNOOP_UADDRL1_VALUE			0x00000000
#define SNOOP_LADDRL1_VALUE			0x00000000


#endif /* REG_PLB4PLB6_H_ */
