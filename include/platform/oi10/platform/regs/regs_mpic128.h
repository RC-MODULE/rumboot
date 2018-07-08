
#ifndef REGS_MPIC_H
#define REGS_MPIC_H


#include <platform/common_macros/common_macros.h>
#include <platform/arch/ppc/ibm_bit_ordering_macros.h>


#define MPIC128_VIR0_VALUE		reg_field(31,0x14) | reg_field(23,0x50)
#define MPIC128_REVID_VALUE		MPIC128_REVID_DFLT

/* Values template for tests */

#define MPIC128_R_BIT			(0x80000000)
/* MPIC RESET VALUES */
#define MPIC128_VP0_VALUE		reg_field(8,1) | reg_field(0,1)
#define MPIC128_DST0_VALUE		0x00000000
#define MPIC128_CTP0_VALUE		0x0000000F
#define MPIC128_CTP1_VALUE		0x0000000F
#define MPIC128_CTP2_VALUE		0x0000000F
#define MPIC128_CTP3_VALUE		0x0000000F
#define MPIC128_WAI0_VALUE		0x00000000
#define MPIC128_WAI1_VALUE		0x00000000
#define MPIC128_WAI2_VALUE		0x00000000
#define MPIC128_WAI3_VALUE		0x00000000
#define MPIC128_SPV_VALUE		reg_field(31,0xFF)
#define MPIC128_NCIAR0_VALUE	MPIC128_SPV_VALUE
#define MPIC128_NCIAR1_VALUE	MPIC128_SPV_VALUE
#define MPIC128_NCIAR2_VALUE	MPIC128_SPV_VALUE
#define MPIC128_NCIAR3_VALUE	MPIC128_SPV_VALUE
#define MPIC128_NCEOI0_VALUE	0x00000000
#define MPIC128_NCEOI1_VALUE	0x00000000
#define MPIC128_NCEOI2_VALUE	0x00000000
#define MPIC128_NCEOI3_VALUE	0x00000000
#define MPIC128_CIAR0_VALUE		MPIC128_SPV_VALUE
#define MPIC128_CIAR1_VALUE		MPIC128_SPV_VALUE
#define MPIC128_CIAR2_VALUE		MPIC128_SPV_VALUE
#define MPIC128_CIAR3_VALUE		MPIC128_SPV_VALUE
#define MPIC128_CEOI0_VALUE		0x00000000
#define MPIC128_CEOI1_VALUE		0x00000000
#define MPIC128_CEOI2_VALUE		0x00000000
#define MPIC128_CEOI3_VALUE		0x00000000
#define MPIC128_IAR0_VALUE		MPIC128_SPV_VALUE
#define MPIC128_IAR1_VALUE		MPIC128_SPV_VALUE
#define MPIC128_IAR2_VALUE		MPIC128_SPV_VALUE
#define MPIC128_IAR3_VALUE		MPIC128_SPV_VALUE
#define MPIC128_EOI0_VALUE		0x00000000
#define MPIC128_EOI1_VALUE		0x00000000
#define MPIC128_EOI2_VALUE		0x00000000
#define MPIC128_EOI3_VALUE		0x00000000
#define MPIC128_FRG0_VALUE		reg_field(31,2) | reg_field(23,3) | reg_field(15,127)
#define MPIC128_GCF0_VALUE		reg_field(31,0xF)
#define MPIC128_VITC_VALUE		reg_field(31,0x10) | reg_field(23,0x10)
#define MPIC128_VRID_VALUE		0x00000000
#define MPIC128_PINI_VALUE		0x00000000
#define MPIC128_IVP0_VALUE		reg_field(0,0x1)
#define MPIC128_IVP1_VALUE		reg_field(0,0x1)
#define MPIC128_IVP2_VALUE		reg_field(0,0x1)
#define MPIC128_IVP3_VALUE		reg_field(0,0x1)
#define MPIC128_TFR_VALUE		0x00000000
#define MPIC128_TCC0_VALUE		0x00000000
#define MPIC128_TCC1_VALUE		0x00000000
#define MPIC128_TCC2_VALUE		0x00000000
#define MPIC128_TCC3_VALUE		0x00000000
#define MPIC128_TBC0_VALUE		reg_field(0,0x1)
#define MPIC128_TBC1_VALUE		reg_field(0,0x1)
#define MPIC128_TBC2_VALUE		reg_field(0,0x1)
#define MPIC128_TBC3_VALUE		reg_field(0,0x1)
#define MPIC128_TVP0_VALUE		reg_field(0,0x1)
#define MPIC128_TVP1_VALUE		reg_field(0,0x1)
#define MPIC128_TVP2_VALUE		reg_field(0,0x1)
#define MPIC128_TVP3_VALUE		reg_field(0,0x1)
#define MPIC128_TDST0_VALUE		0x00000000
#define MPIC128_TDST1_VALUE		0x00000000
#define MPIC128_TDST2_VALUE		0x00000000
#define MPIC128_TDST3_VALUE		0x00000000

#define MPIC128_VPx_REGS_COUNT    128
#define MPIC128_DSTx_REGS_COUNT   128
#define MPIC128_REGS_COUNT_4      4

BEGIN_ENUM( MPIC128_REG )
/* MPIC Vector/Priority Register */
#define				MPIC128_VP(x)		(0x00010000 + (0x0020*(x)))
DECLARE_ENUM_VAL(	MPIC128_VP0,		MPIC128_VP(0)   )
DECLARE_ENUM_VAL(	MPIC128_VP1,		MPIC128_VP(1)   )
DECLARE_ENUM_VAL(	MPIC128_VP2,		MPIC128_VP(2)   )
DECLARE_ENUM_VAL(	MPIC128_VP3,		MPIC128_VP(3)   )
DECLARE_ENUM_VAL(	MPIC128_VP4,		MPIC128_VP(4)   )
DECLARE_ENUM_VAL(	MPIC128_VP5,		MPIC128_VP(5)   )
DECLARE_ENUM_VAL(	MPIC128_VP6,		MPIC128_VP(6)   )
DECLARE_ENUM_VAL(	MPIC128_VP7,		MPIC128_VP(7)   )
DECLARE_ENUM_VAL(	MPIC128_VP8,		MPIC128_VP(8)   )
DECLARE_ENUM_VAL(	MPIC128_VP9,		MPIC128_VP(9)   )
DECLARE_ENUM_VAL(	MPIC128_VP10,		MPIC128_VP(10)  )
DECLARE_ENUM_VAL(	MPIC128_VP11,		MPIC128_VP(11)  )
DECLARE_ENUM_VAL(	MPIC128_VP12,		MPIC128_VP(12)  )
DECLARE_ENUM_VAL(	MPIC128_VP13,		MPIC128_VP(13)  )
DECLARE_ENUM_VAL(	MPIC128_VP14,		MPIC128_VP(14)  )
DECLARE_ENUM_VAL(	MPIC128_VP15,		MPIC128_VP(15)  )
DECLARE_ENUM_VAL(	MPIC128_VP16,		MPIC128_VP(16)  )
DECLARE_ENUM_VAL(	MPIC128_VP17,		MPIC128_VP(17)  )
DECLARE_ENUM_VAL(	MPIC128_VP18,		MPIC128_VP(18)  )
DECLARE_ENUM_VAL(	MPIC128_VP19,		MPIC128_VP(19)  )
DECLARE_ENUM_VAL(	MPIC128_VP20,		MPIC128_VP(20)  )
DECLARE_ENUM_VAL(	MPIC128_VP21,		MPIC128_VP(21)  )
DECLARE_ENUM_VAL(	MPIC128_VP22,		MPIC128_VP(22)  )
DECLARE_ENUM_VAL(	MPIC128_VP23,		MPIC128_VP(23)  )
DECLARE_ENUM_VAL(	MPIC128_VP24,		MPIC128_VP(24)  )
DECLARE_ENUM_VAL(	MPIC128_VP25,		MPIC128_VP(25)  )
DECLARE_ENUM_VAL(	MPIC128_VP26,		MPIC128_VP(26)  )
DECLARE_ENUM_VAL(	MPIC128_VP27,		MPIC128_VP(27)  )
DECLARE_ENUM_VAL(	MPIC128_VP28,		MPIC128_VP(28)  )
DECLARE_ENUM_VAL(	MPIC128_VP29,		MPIC128_VP(29)  )
DECLARE_ENUM_VAL(	MPIC128_VP30,		MPIC128_VP(30)  )
DECLARE_ENUM_VAL(	MPIC128_VP31,		MPIC128_VP(31)  )
DECLARE_ENUM_VAL(	MPIC128_VP32,		MPIC128_VP(32)  )
DECLARE_ENUM_VAL(	MPIC128_VP33,		MPIC128_VP(33)  )
DECLARE_ENUM_VAL(	MPIC128_VP34,		MPIC128_VP(34)  )
DECLARE_ENUM_VAL(	MPIC128_VP35,		MPIC128_VP(35)  )
DECLARE_ENUM_VAL(	MPIC128_VP36,		MPIC128_VP(36)  )
DECLARE_ENUM_VAL(	MPIC128_VP37,		MPIC128_VP(37)  )
DECLARE_ENUM_VAL(	MPIC128_VP38,		MPIC128_VP(38)  )
DECLARE_ENUM_VAL(	MPIC128_VP39,		MPIC128_VP(39)  )
DECLARE_ENUM_VAL(	MPIC128_VP40,		MPIC128_VP(40)  )
DECLARE_ENUM_VAL(	MPIC128_VP41,		MPIC128_VP(41)  )
DECLARE_ENUM_VAL(	MPIC128_VP42,		MPIC128_VP(42)  )
DECLARE_ENUM_VAL(	MPIC128_VP43,		MPIC128_VP(43)  )
DECLARE_ENUM_VAL(	MPIC128_VP44,		MPIC128_VP(44)  )
DECLARE_ENUM_VAL(	MPIC128_VP45,		MPIC128_VP(45)  )
DECLARE_ENUM_VAL(	MPIC128_VP46,		MPIC128_VP(46)  )
DECLARE_ENUM_VAL(	MPIC128_VP47,		MPIC128_VP(47)  )
DECLARE_ENUM_VAL(	MPIC128_VP48,		MPIC128_VP(48)  )
DECLARE_ENUM_VAL(	MPIC128_VP49,		MPIC128_VP(49)  )
DECLARE_ENUM_VAL(	MPIC128_VP50,		MPIC128_VP(50)  )
DECLARE_ENUM_VAL(	MPIC128_VP51,		MPIC128_VP(51)  )
DECLARE_ENUM_VAL(	MPIC128_VP52,		MPIC128_VP(52)  )
DECLARE_ENUM_VAL(	MPIC128_VP53,		MPIC128_VP(53)  )
DECLARE_ENUM_VAL(	MPIC128_VP54,		MPIC128_VP(54)  )
DECLARE_ENUM_VAL(	MPIC128_VP55,		MPIC128_VP(55)  )
DECLARE_ENUM_VAL(	MPIC128_VP56,		MPIC128_VP(56)  )
DECLARE_ENUM_VAL(	MPIC128_VP57,		MPIC128_VP(57)  )
DECLARE_ENUM_VAL(	MPIC128_VP58,		MPIC128_VP(58)  )
DECLARE_ENUM_VAL(	MPIC128_VP59,		MPIC128_VP(59)  )
DECLARE_ENUM_VAL(	MPIC128_VP60,		MPIC128_VP(60)  )
DECLARE_ENUM_VAL(	MPIC128_VP61,		MPIC128_VP(61)  )
DECLARE_ENUM_VAL(	MPIC128_VP62,		MPIC128_VP(62)  )
DECLARE_ENUM_VAL(	MPIC128_VP63,		MPIC128_VP(63)  )
DECLARE_ENUM_VAL(	MPIC128_VP64,		MPIC128_VP(64)  )
DECLARE_ENUM_VAL(	MPIC128_VP65,		MPIC128_VP(65)  )
DECLARE_ENUM_VAL(	MPIC128_VP66,		MPIC128_VP(66)  )
DECLARE_ENUM_VAL(	MPIC128_VP67,		MPIC128_VP(67)  )
DECLARE_ENUM_VAL(	MPIC128_VP68,		MPIC128_VP(68)  )
DECLARE_ENUM_VAL(	MPIC128_VP69,		MPIC128_VP(69)  )
DECLARE_ENUM_VAL(	MPIC128_VP70,		MPIC128_VP(70)  )
DECLARE_ENUM_VAL(	MPIC128_VP71,		MPIC128_VP(71)  )
DECLARE_ENUM_VAL(	MPIC128_VP72,		MPIC128_VP(72)  )
DECLARE_ENUM_VAL(	MPIC128_VP73,		MPIC128_VP(73)  )
DECLARE_ENUM_VAL(	MPIC128_VP74,		MPIC128_VP(74)  )
DECLARE_ENUM_VAL(	MPIC128_VP75,		MPIC128_VP(75)  )
DECLARE_ENUM_VAL(	MPIC128_VP76,		MPIC128_VP(76)  )
DECLARE_ENUM_VAL(	MPIC128_VP77,		MPIC128_VP(77)  )
DECLARE_ENUM_VAL(	MPIC128_VP78,		MPIC128_VP(78)  )
DECLARE_ENUM_VAL(	MPIC128_VP79,		MPIC128_VP(79)  )
DECLARE_ENUM_VAL(	MPIC128_VP80,		MPIC128_VP(80)  )
DECLARE_ENUM_VAL(	MPIC128_VP81,		MPIC128_VP(81)  )
DECLARE_ENUM_VAL(	MPIC128_VP82,		MPIC128_VP(82)  )
DECLARE_ENUM_VAL(	MPIC128_VP83,		MPIC128_VP(83)  )
DECLARE_ENUM_VAL(	MPIC128_VP84,		MPIC128_VP(84)  )
DECLARE_ENUM_VAL(	MPIC128_VP85,		MPIC128_VP(85)  )
DECLARE_ENUM_VAL(	MPIC128_VP86,		MPIC128_VP(86)  )
DECLARE_ENUM_VAL(	MPIC128_VP87,		MPIC128_VP(87)  )
DECLARE_ENUM_VAL(	MPIC128_VP88,		MPIC128_VP(88)  )
DECLARE_ENUM_VAL(	MPIC128_VP89,		MPIC128_VP(89)  )
DECLARE_ENUM_VAL(	MPIC128_VP90,		MPIC128_VP(90)  )
DECLARE_ENUM_VAL(	MPIC128_VP91,		MPIC128_VP(91)  )
DECLARE_ENUM_VAL(	MPIC128_VP92,		MPIC128_VP(92)  )
DECLARE_ENUM_VAL(	MPIC128_VP93,		MPIC128_VP(93)  )
DECLARE_ENUM_VAL(	MPIC128_VP94,		MPIC128_VP(94)  )
DECLARE_ENUM_VAL(	MPIC128_VP95,		MPIC128_VP(95)  )
DECLARE_ENUM_VAL(	MPIC128_VP96,		MPIC128_VP(96)  )
DECLARE_ENUM_VAL(	MPIC128_VP97,		MPIC128_VP(97)  )
DECLARE_ENUM_VAL(	MPIC128_VP98,		MPIC128_VP(98)  )
DECLARE_ENUM_VAL(	MPIC128_VP99,		MPIC128_VP(99)  )
DECLARE_ENUM_VAL(	MPIC128_VP100,		MPIC128_VP(100) )
DECLARE_ENUM_VAL(	MPIC128_VP101,		MPIC128_VP(101) )
DECLARE_ENUM_VAL(	MPIC128_VP102,		MPIC128_VP(102) )
DECLARE_ENUM_VAL(	MPIC128_VP103,		MPIC128_VP(103) )
DECLARE_ENUM_VAL(	MPIC128_VP104,		MPIC128_VP(104) )
DECLARE_ENUM_VAL(	MPIC128_VP105,		MPIC128_VP(105) )
DECLARE_ENUM_VAL(	MPIC128_VP106,		MPIC128_VP(106) )
DECLARE_ENUM_VAL(	MPIC128_VP107,		MPIC128_VP(107) )
DECLARE_ENUM_VAL(	MPIC128_VP108,		MPIC128_VP(108) )
DECLARE_ENUM_VAL(	MPIC128_VP109,		MPIC128_VP(109) )
DECLARE_ENUM_VAL(	MPIC128_VP110,		MPIC128_VP(110) )
DECLARE_ENUM_VAL(	MPIC128_VP111,		MPIC128_VP(111) )
DECLARE_ENUM_VAL(	MPIC128_VP112,		MPIC128_VP(112) )
DECLARE_ENUM_VAL(	MPIC128_VP113,		MPIC128_VP(113) )
DECLARE_ENUM_VAL(	MPIC128_VP114,		MPIC128_VP(114) )
DECLARE_ENUM_VAL(	MPIC128_VP115,		MPIC128_VP(115) )
DECLARE_ENUM_VAL(	MPIC128_VP116,		MPIC128_VP(116) )
DECLARE_ENUM_VAL(	MPIC128_VP117,		MPIC128_VP(117) )
DECLARE_ENUM_VAL(	MPIC128_VP118,		MPIC128_VP(118) )
DECLARE_ENUM_VAL(	MPIC128_VP119,		MPIC128_VP(119) )
DECLARE_ENUM_VAL(	MPIC128_VP120,		MPIC128_VP(120) )
DECLARE_ENUM_VAL(	MPIC128_VP121,		MPIC128_VP(121) )
DECLARE_ENUM_VAL(	MPIC128_VP122,		MPIC128_VP(122) )
DECLARE_ENUM_VAL(	MPIC128_VP123,		MPIC128_VP(123) )
DECLARE_ENUM_VAL(	MPIC128_VP124,		MPIC128_VP(124) )
DECLARE_ENUM_VAL(	MPIC128_VP125,		MPIC128_VP(125) )
DECLARE_ENUM_VAL(	MPIC128_VP126,		MPIC128_VP(126) )
DECLARE_ENUM_VAL(	MPIC128_VP127,		MPIC128_VP(127) )

/* MPIC Destination Register */
#define				MPIC128_DST(x)	(0x00010010 + (0x0020*(x)))
DECLARE_ENUM_VAL(	MPIC128_DST0,		MPIC128_DST(0)   )
DECLARE_ENUM_VAL(	MPIC128_DST1,		MPIC128_DST(1)   )
DECLARE_ENUM_VAL(	MPIC128_DST2,		MPIC128_DST(2)   )
DECLARE_ENUM_VAL(	MPIC128_DST3,		MPIC128_DST(3)   )
DECLARE_ENUM_VAL(	MPIC128_DST4,		MPIC128_DST(4)   )
DECLARE_ENUM_VAL(	MPIC128_DST5,		MPIC128_DST(5)   )
DECLARE_ENUM_VAL(	MPIC128_DST6,		MPIC128_DST(6)   )
DECLARE_ENUM_VAL(	MPIC128_DST7,		MPIC128_DST(7)   )
DECLARE_ENUM_VAL(	MPIC128_DST8,		MPIC128_DST(8)   )
DECLARE_ENUM_VAL(	MPIC128_DST9,		MPIC128_DST(9)   )
DECLARE_ENUM_VAL(	MPIC128_DST10,		MPIC128_DST(10)  )
DECLARE_ENUM_VAL(	MPIC128_DST11,		MPIC128_DST(11)  )
DECLARE_ENUM_VAL(	MPIC128_DST12,		MPIC128_DST(12)  )
DECLARE_ENUM_VAL(	MPIC128_DST13,		MPIC128_DST(13)  )
DECLARE_ENUM_VAL(	MPIC128_DST14,		MPIC128_DST(14)  )
DECLARE_ENUM_VAL(	MPIC128_DST15,		MPIC128_DST(15)  )
DECLARE_ENUM_VAL(	MPIC128_DST16,		MPIC128_DST(16)  )
DECLARE_ENUM_VAL(	MPIC128_DST17,		MPIC128_DST(17)  )
DECLARE_ENUM_VAL(	MPIC128_DST18,		MPIC128_DST(18)  )
DECLARE_ENUM_VAL(	MPIC128_DST19,		MPIC128_DST(19)  )
DECLARE_ENUM_VAL(	MPIC128_DST20,		MPIC128_DST(20)  )
DECLARE_ENUM_VAL(	MPIC128_DST21,		MPIC128_DST(21)  )
DECLARE_ENUM_VAL(	MPIC128_DST22,		MPIC128_DST(22)  )
DECLARE_ENUM_VAL(	MPIC128_DST23,		MPIC128_DST(23)  )
DECLARE_ENUM_VAL(	MPIC128_DST24,		MPIC128_DST(24)  )
DECLARE_ENUM_VAL(	MPIC128_DST25,		MPIC128_DST(25)  )
DECLARE_ENUM_VAL(	MPIC128_DST26,		MPIC128_DST(26)  )
DECLARE_ENUM_VAL(	MPIC128_DST27,		MPIC128_DST(27)  )
DECLARE_ENUM_VAL(	MPIC128_DST28,		MPIC128_DST(28)  )
DECLARE_ENUM_VAL(	MPIC128_DST29,		MPIC128_DST(29)  )
DECLARE_ENUM_VAL(	MPIC128_DST30,		MPIC128_DST(30)  )
DECLARE_ENUM_VAL(	MPIC128_DST31,		MPIC128_DST(31)  )
DECLARE_ENUM_VAL(	MPIC128_DST32,		MPIC128_DST(32)  )
DECLARE_ENUM_VAL(	MPIC128_DST33,		MPIC128_DST(33)  )
DECLARE_ENUM_VAL(	MPIC128_DST34,		MPIC128_DST(34)  )
DECLARE_ENUM_VAL(	MPIC128_DST35,		MPIC128_DST(35)  )
DECLARE_ENUM_VAL(	MPIC128_DST36,		MPIC128_DST(36)  )
DECLARE_ENUM_VAL(	MPIC128_DST37,		MPIC128_DST(37)  )
DECLARE_ENUM_VAL(	MPIC128_DST38,		MPIC128_DST(38)  )
DECLARE_ENUM_VAL(	MPIC128_DST39,		MPIC128_DST(39)  )
DECLARE_ENUM_VAL(	MPIC128_DST40,		MPIC128_DST(40)  )
DECLARE_ENUM_VAL(	MPIC128_DST41,		MPIC128_DST(41)  )
DECLARE_ENUM_VAL(	MPIC128_DST42,		MPIC128_DST(42)  )
DECLARE_ENUM_VAL(	MPIC128_DST43,		MPIC128_DST(43)  )
DECLARE_ENUM_VAL(	MPIC128_DST44,		MPIC128_DST(44)  )
DECLARE_ENUM_VAL(	MPIC128_DST45,		MPIC128_DST(45)  )
DECLARE_ENUM_VAL(	MPIC128_DST46,		MPIC128_DST(46)  )
DECLARE_ENUM_VAL(	MPIC128_DST47,		MPIC128_DST(47)  )
DECLARE_ENUM_VAL(	MPIC128_DST48,		MPIC128_DST(48)  )
DECLARE_ENUM_VAL(	MPIC128_DST49,		MPIC128_DST(49)  )
DECLARE_ENUM_VAL(	MPIC128_DST50,		MPIC128_DST(50)  )
DECLARE_ENUM_VAL(	MPIC128_DST51,		MPIC128_DST(51)  )
DECLARE_ENUM_VAL(	MPIC128_DST52,		MPIC128_DST(52)  )
DECLARE_ENUM_VAL(	MPIC128_DST53,		MPIC128_DST(53)  )
DECLARE_ENUM_VAL(	MPIC128_DST54,		MPIC128_DST(54)  )
DECLARE_ENUM_VAL(	MPIC128_DST55,		MPIC128_DST(55)  )
DECLARE_ENUM_VAL(	MPIC128_DST56,		MPIC128_DST(56)  )
DECLARE_ENUM_VAL(	MPIC128_DST57,		MPIC128_DST(57)  )
DECLARE_ENUM_VAL(	MPIC128_DST58,		MPIC128_DST(58)  )
DECLARE_ENUM_VAL(	MPIC128_DST59,		MPIC128_DST(59)  )
DECLARE_ENUM_VAL(	MPIC128_DST60,		MPIC128_DST(60)  )
DECLARE_ENUM_VAL(	MPIC128_DST61,		MPIC128_DST(61)  )
DECLARE_ENUM_VAL(	MPIC128_DST62,		MPIC128_DST(62)  )
DECLARE_ENUM_VAL(	MPIC128_DST63,		MPIC128_DST(63)  )
DECLARE_ENUM_VAL(	MPIC128_DST64,		MPIC128_DST(64)  )
DECLARE_ENUM_VAL(	MPIC128_DST65,		MPIC128_DST(65)  )
DECLARE_ENUM_VAL(	MPIC128_DST66,		MPIC128_DST(66)  )
DECLARE_ENUM_VAL(	MPIC128_DST67,		MPIC128_DST(67)  )
DECLARE_ENUM_VAL(	MPIC128_DST68,		MPIC128_DST(68)  )
DECLARE_ENUM_VAL(	MPIC128_DST69,		MPIC128_DST(69)  )
DECLARE_ENUM_VAL(	MPIC128_DST70,		MPIC128_DST(70)  )
DECLARE_ENUM_VAL(	MPIC128_DST71,		MPIC128_DST(71)  )
DECLARE_ENUM_VAL(	MPIC128_DST72,		MPIC128_DST(72)  )
DECLARE_ENUM_VAL(	MPIC128_DST73,		MPIC128_DST(73)  )
DECLARE_ENUM_VAL(	MPIC128_DST74,		MPIC128_DST(74)  )
DECLARE_ENUM_VAL(	MPIC128_DST75,		MPIC128_DST(75)  )
DECLARE_ENUM_VAL(	MPIC128_DST76,		MPIC128_DST(76)  )
DECLARE_ENUM_VAL(	MPIC128_DST77,		MPIC128_DST(77)  )
DECLARE_ENUM_VAL(	MPIC128_DST78,		MPIC128_DST(78)  )
DECLARE_ENUM_VAL(	MPIC128_DST79,		MPIC128_DST(79)  )
DECLARE_ENUM_VAL(	MPIC128_DST80,		MPIC128_DST(80)  )
DECLARE_ENUM_VAL(	MPIC128_DST81,		MPIC128_DST(81)  )
DECLARE_ENUM_VAL(	MPIC128_DST82,		MPIC128_DST(82)  )
DECLARE_ENUM_VAL(	MPIC128_DST83,		MPIC128_DST(83)  )
DECLARE_ENUM_VAL(	MPIC128_DST84,		MPIC128_DST(84)  )
DECLARE_ENUM_VAL(	MPIC128_DST85,		MPIC128_DST(85)  )
DECLARE_ENUM_VAL(	MPIC128_DST86,		MPIC128_DST(86)  )
DECLARE_ENUM_VAL(	MPIC128_DST87,		MPIC128_DST(87)  )
DECLARE_ENUM_VAL(	MPIC128_DST88,		MPIC128_DST(88)  )
DECLARE_ENUM_VAL(	MPIC128_DST89,		MPIC128_DST(89)  )
DECLARE_ENUM_VAL(	MPIC128_DST90,		MPIC128_DST(90)  )
DECLARE_ENUM_VAL(	MPIC128_DST91,		MPIC128_DST(91)  )
DECLARE_ENUM_VAL(	MPIC128_DST92,		MPIC128_DST(92)  )
DECLARE_ENUM_VAL(	MPIC128_DST93,		MPIC128_DST(93)  )
DECLARE_ENUM_VAL(	MPIC128_DST94,		MPIC128_DST(94)  )
DECLARE_ENUM_VAL(	MPIC128_DST95,		MPIC128_DST(95)  )
DECLARE_ENUM_VAL(	MPIC128_DST96,		MPIC128_DST(96)  )
DECLARE_ENUM_VAL(	MPIC128_DST97,		MPIC128_DST(97)  )
DECLARE_ENUM_VAL(	MPIC128_DST98,		MPIC128_DST(98)  )
DECLARE_ENUM_VAL(	MPIC128_DST99,		MPIC128_DST(99)  )
DECLARE_ENUM_VAL(	MPIC128_DST100,		MPIC128_DST(100) )
DECLARE_ENUM_VAL(	MPIC128_DST101,		MPIC128_DST(101) )
DECLARE_ENUM_VAL(	MPIC128_DST102,		MPIC128_DST(102) )
DECLARE_ENUM_VAL(	MPIC128_DST103,		MPIC128_DST(103) )
DECLARE_ENUM_VAL(	MPIC128_DST104,		MPIC128_DST(104) )
DECLARE_ENUM_VAL(	MPIC128_DST105,		MPIC128_DST(105) )
DECLARE_ENUM_VAL(	MPIC128_DST106,		MPIC128_DST(106) )
DECLARE_ENUM_VAL(	MPIC128_DST107,		MPIC128_DST(107) )
DECLARE_ENUM_VAL(	MPIC128_DST108,		MPIC128_DST(108) )
DECLARE_ENUM_VAL(	MPIC128_DST109,		MPIC128_DST(109) )
DECLARE_ENUM_VAL(	MPIC128_DST110,		MPIC128_DST(110) )
DECLARE_ENUM_VAL(	MPIC128_DST111,		MPIC128_DST(111) )
DECLARE_ENUM_VAL(	MPIC128_DST112,		MPIC128_DST(112) )
DECLARE_ENUM_VAL(	MPIC128_DST113,		MPIC128_DST(113) )
DECLARE_ENUM_VAL(	MPIC128_DST114,		MPIC128_DST(114) )
DECLARE_ENUM_VAL(	MPIC128_DST115,		MPIC128_DST(115) )
DECLARE_ENUM_VAL(	MPIC128_DST116,		MPIC128_DST(116) )
DECLARE_ENUM_VAL(	MPIC128_DST117,		MPIC128_DST(117) )
DECLARE_ENUM_VAL(	MPIC128_DST118,		MPIC128_DST(118) )
DECLARE_ENUM_VAL(	MPIC128_DST119,		MPIC128_DST(119) )
DECLARE_ENUM_VAL(	MPIC128_DST120,		MPIC128_DST(120) )
DECLARE_ENUM_VAL(	MPIC128_DST121,		MPIC128_DST(121) )
DECLARE_ENUM_VAL(	MPIC128_DST122,		MPIC128_DST(122) )
DECLARE_ENUM_VAL(	MPIC128_DST123,		MPIC128_DST(123) )
DECLARE_ENUM_VAL(	MPIC128_DST124,		MPIC128_DST(124) )
DECLARE_ENUM_VAL(	MPIC128_DST125,		MPIC128_DST(125) )
DECLARE_ENUM_VAL(	MPIC128_DST126,		MPIC128_DST(126) )
DECLARE_ENUM_VAL(	MPIC128_DST127,		MPIC128_DST(127) )

/* MPIC Inter processor Interrupt Dispatch */
#define				MPIC128_IPID(x)		(0x00020040 + (0x1010*(x)))
DECLARE_ENUM_VAL(	MPIC128_IPID0,		MPIC128_IPID(0)  )
DECLARE_ENUM_VAL(	MPIC128_IPID1,		MPIC128_IPID(1)  )
DECLARE_ENUM_VAL(	MPIC128_IPID2,		MPIC128_IPID(2)  )
DECLARE_ENUM_VAL(	MPIC128_IPID3,		MPIC128_IPID(3)  )

/* Current Task Priority Register */
DECLARE_ENUM_VAL(	MPIC128_CTP_PR,		0x00000080 )
#define				MPIC128_CTP(x)		(0x00020080 + (0x1000*(x)))
DECLARE_ENUM_VAL(	MPIC128_CTP0,		MPIC128_CTP(0)   )
DECLARE_ENUM_VAL(	MPIC128_CTP1,		MPIC128_CTP(1)   )
DECLARE_ENUM_VAL(	MPIC128_CTP2,		MPIC128_CTP(2)   )
DECLARE_ENUM_VAL(	MPIC128_CTP3,		MPIC128_CTP(3)   )

/* Who Am I Register */
DECLARE_ENUM_VAL(	MPIC128_WAI_PR,		0x00000090       )
#define				MPIC128_WAI(x)		(0x00020090 + (0x1000*(x)))
DECLARE_ENUM_VAL(	MPIC128_WAI0,		MPIC128_WAI(0)   )
DECLARE_ENUM_VAL(	MPIC128_WAI1,		MPIC128_WAI(1)   )
DECLARE_ENUM_VAL(	MPIC128_WAI2,		MPIC128_WAI(2)   )
DECLARE_ENUM_VAL(	MPIC128_WAI3,		MPIC128_WAI(3)   )

/* Non Critical Interrupt Acknowledge Register */
DECLARE_ENUM_VAL(	MPIC128_NCIAR_PR,	0x000000A0 )
#define				MPIC128_NCIAR(x)	(0x000200A0 + (0x1000*(x)))
DECLARE_ENUM_VAL(	MPIC128_NCIAR0,		MPIC128_NCIAR(0) )
DECLARE_ENUM_VAL(	MPIC128_NCIAR1,		MPIC128_NCIAR(1) )
DECLARE_ENUM_VAL(	MPIC128_NCIAR2,		MPIC128_NCIAR(2) )
DECLARE_ENUM_VAL(	MPIC128_NCIAR3,		MPIC128_NCIAR(3) )

/* Non Critical End of Interrupt Register */
DECLARE_ENUM_VAL(	MPIC128_NCEOI_PR,	0x000000B0 )
#define				MPIC128_NCEOI(x)	(0x000200B0 + (0x1000*(x)))
DECLARE_ENUM_VAL(	MPIC128_NCEOI0,		MPIC128_NCEOI(0) )
DECLARE_ENUM_VAL(	MPIC128_NCEOI1,		MPIC128_NCEOI(1) )
DECLARE_ENUM_VAL(	MPIC128_NCEOI2,		MPIC128_NCEOI(2) )
DECLARE_ENUM_VAL(	MPIC128_NCEOI3,		MPIC128_NCEOI(3) )

/* Critical Interrupt Acknowledge Register */
DECLARE_ENUM_VAL(	MPIC128_CIAR_PR,	0x000000C0 )
#define				MPIC128_CIAR(x)		(0x000200C0 + (0x1000*(x)))
DECLARE_ENUM_VAL(	MPIC128_CIAR0,		MPIC128_CIAR(0)  )
DECLARE_ENUM_VAL(	MPIC128_CIAR1,		MPIC128_CIAR(1)  )
DECLARE_ENUM_VAL(	MPIC128_CIAR2,		MPIC128_CIAR(2)  )
DECLARE_ENUM_VAL(	MPIC128_CIAR3,		MPIC128_CIAR(3)  )

/* Critical End of Interrupt Register */
DECLARE_ENUM_VAL(	MPIC128_CEOI_PR,	0x000000D0       )
#define				MPIC128_CEOI(x)		(0x000200D0 + (0x1000*(x)))
DECLARE_ENUM_VAL(	MPIC128_CEOI0,		MPIC128_CEOI(0)  )
DECLARE_ENUM_VAL(	MPIC128_CEOI1,		MPIC128_CEOI(1)  )
DECLARE_ENUM_VAL(	MPIC128_CEOI2,		MPIC128_CEOI(2)  )
DECLARE_ENUM_VAL(	MPIC128_CEOI3,		MPIC128_CEOI(3)  )

/* Machine Check Interrupt Acknowledge Register */
DECLARE_ENUM_VAL(	MPIC128_IAR_PR,		0x000000E0       )
#define				MPIC128_IAR(x)		(0x000200E0 + (0x1000*(x)))
DECLARE_ENUM_VAL(	MPIC128_IAR0,		MPIC128_IAR(0)   )
DECLARE_ENUM_VAL(	MPIC128_IAR1,		MPIC128_IAR(1)   )
DECLARE_ENUM_VAL(	MPIC128_IAR2,		MPIC128_IAR(2)   )
DECLARE_ENUM_VAL(	MPIC128_IAR3,		MPIC128_IAR(3)   )

/* Machine Check End of Interrupt Register */
DECLARE_ENUM_VAL(	MPIC128_EOI_PR,		0x000000F0       )
#define				MPIC128_EOI(x)		(0x000200F0 + (0x1000*(x)))
DECLARE_ENUM_VAL(	MPIC128_EOI0,		MPIC128_EOI(0)   )
DECLARE_ENUM_VAL(	MPIC128_EOI1,		MPIC128_EOI(1)   )
DECLARE_ENUM_VAL(	MPIC128_EOI2,		MPIC128_EOI(2)   )
DECLARE_ENUM_VAL(	MPIC128_EOI3,		MPIC128_EOI(3)   )

DECLARE_ENUM_VAL(	MPIC128_FRG0,		0x00001000       ) /* Feature Reporting Register 0 */
DECLARE_ENUM_VAL(	MPIC128_GCF0,		0x00001020       ) /* Global Configuration Register 0 */
DECLARE_ENUM_VAL(	MPIC128_VITC,		0x00001040       ) /* Vendor Interrupt Type Configuration Register */
DECLARE_ENUM_VAL(	MPIC128_VRID,		0x00001050       ) /* Vendor Raw Interrupt Debug Register */
DECLARE_ENUM_VAL(	MPIC128_REVID,		0x00001070       ) /* Vendor Soft Core Revision ID Register */
DECLARE_ENUM_VAL(	MPIC128_VIR0,		0x00001080       ) /* Vendor Identification Register 0 */
DECLARE_ENUM_VAL(	MPIC128_PINI,		0x00001090       ) /* Processor Initialization Register */

/* IPI Vector/Priority Register */
#define				MPIC128_IVP(x)		(0x000010A0 + (0x0010*(x)))
DECLARE_ENUM_VAL(	MPIC128_IVP0,		MPIC128_IVP(0)   )
DECLARE_ENUM_VAL(	MPIC128_IVP1,		MPIC128_IVP(1)   )
DECLARE_ENUM_VAL(	MPIC128_IVP2,		MPIC128_IVP(2)   )
DECLARE_ENUM_VAL(	MPIC128_IVP3,		MPIC128_IVP(3)   )

DECLARE_ENUM_VAL(	MPIC128_SPV,		0x000010E0       ) /* Spurious Vector Register */

DECLARE_ENUM_VAL(	MPIC128_TFR,		0x000010F0       ) /* Timer Frequency Reporting Register */

/* Timer Current Count Register */
#define				MPIC128_TCC(x)		(0x00001100 + (0x0040*(x)))
DECLARE_ENUM_VAL(	MPIC128_TCC0,		MPIC128_TCC(0)   )
DECLARE_ENUM_VAL(	MPIC128_TCC1,		MPIC128_TCC(1)   )
DECLARE_ENUM_VAL(	MPIC128_TCC2,		MPIC128_TCC(2)   )
DECLARE_ENUM_VAL(	MPIC128_TCC3,		MPIC128_TCC(3)   )

/* Timer Base Count Register */
#define				MPIC128_TBC(x)		(0x00001110 + (0x0040*(x)))
DECLARE_ENUM_VAL(	MPIC128_TBC0,   	MPIC128_TBC(0)   )
DECLARE_ENUM_VAL(	MPIC128_TBC1,		MPIC128_TBC(1)   )
DECLARE_ENUM_VAL(	MPIC128_TBC2,		MPIC128_TBC(2)   )
DECLARE_ENUM_VAL(	MPIC128_TBC3,		MPIC128_TBC(3)   )

/* Timer Vector/Priority Register */
#define				MPIC128_TVP(x)		(0x00001120 + (0x0040*(x)))
DECLARE_ENUM_VAL(	MPIC128_TVP0,		MPIC128_TVP(0) )
DECLARE_ENUM_VAL(	MPIC128_TVP1,		MPIC128_TVP(1) )
DECLARE_ENUM_VAL(	MPIC128_TVP2,		MPIC128_TVP(2) )
DECLARE_ENUM_VAL(	MPIC128_TVP3,		MPIC128_TVP(3) )

/* Timer n Destination Register */
#define				MPIC128_TDST(x)		(0x00001130 + (0x0040*(x)))
DECLARE_ENUM_VAL(	MPIC128_TDST0,		MPIC128_TDST(0) )
DECLARE_ENUM_VAL(	MPIC128_TDST1,		MPIC128_TDST(1) )
DECLARE_ENUM_VAL(	MPIC128_TDST2,		MPIC128_TDST(2) )
DECLARE_ENUM_VAL(	MPIC128_TDST3,		MPIC128_TDST(3) )
END_ENUM( MPIC128_REG )


BEGIN_ENUM( MPIC128_REG_DFLT )
DECLARE_ENUM_VAL(   MPIC128_VP_DFLT,       ( reg_field(8,1) | reg_field(0,1) ) )
DECLARE_ENUM_VAL(   MPIC128_DST_DFLT,      0x00000000 )
DECLARE_ENUM_VAL(   MPIC128_CTP_DFLT,      0x0000000F )
#define             MPIC128_WAI_DFLT       SPR_PIR_read() /* TODO: check this value */
DECLARE_ENUM_VAL(   MPIC128_SPV_DFLT,      reg_field(31,0xFF) )
DECLARE_ENUM_VAL(   MPIC128_NCIAR_DFLT,    MPIC128_SPV_DFLT )
DECLARE_ENUM_VAL(   MPIC128_NCEOI_DFLT,    0x00000000 )
DECLARE_ENUM_VAL(   MPIC128_CIAR_DFLT,     MPIC128_SPV_DFLT )
DECLARE_ENUM_VAL(   MPIC128_CEOI_DFLT,     0x00000000 )
DECLARE_ENUM_VAL(   MPIC128_IAR_DFLT,      MPIC128_SPV_DFLT )
DECLARE_ENUM_VAL(   MPIC128_EOI_DFLT,      0x00000000 )
DECLARE_ENUM_VAL(   MPIC128_FRG0_DFLT,     ( reg_field(31,2) | reg_field(23,3) | reg_field(15,127) ) )
DECLARE_ENUM_VAL(   MPIC128_GCF0_DFLT,     reg_field(31,0xF) )
DECLARE_ENUM_VAL(   MPIC128_VITC_DFLT,     ( reg_field(31,0x10) | reg_field(23,0x10) ) )
DECLARE_ENUM_VAL(   MPIC128_VRID_DFLT,     0x00000000 )
/* units/cpu/units/mpic128_v001/verilog/rtl/MPIC128_dcr_misc.v */
DECLARE_ENUM_VAL(   MPIC128_REVID_DFLT,    0x00000103 )
DECLARE_ENUM_VAL(   MPIC128_VIR0_DFLT,     0x00005014 ) /* set acc. to STRAP-pins */
DECLARE_ENUM_VAL(   MPIC128_PINI_DFLT,     0x00000000 )
DECLARE_ENUM_VAL(   MPIC128_IVP_DFLT,      reg_field(0,0x1) )
DECLARE_ENUM_VAL(   MPIC128_TFR_DFLT,      0x00000000 )
DECLARE_ENUM_VAL(   MPIC128_TCC_DFLT,      0x00000000 )
DECLARE_ENUM_VAL(   MPIC128_TBC_DFLT,      reg_field(0,0x1) )
DECLARE_ENUM_VAL(   MPIC128_TVP_DFLT,      reg_field(0,0x1) )
DECLARE_ENUM_VAL(   MPIC128_TDST_DFLT,     0x00000000 )
END_ENUM( MPIC128_REG_DFLT )


#endif // REGS_MPIC_H

