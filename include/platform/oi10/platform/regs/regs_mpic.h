
#ifndef REGS_MPIC_H
#define REGS_MPIC_H

#define MPIC128_VIR0_VALUE		reg_field(31,0x14) | reg_field(23,0x50)
#define MPIC128_REVID_VALUE		reg_field(31,0xF)

#define VP0			(0x0FC10000)
#define DST0		(0x0FC10010)
#define CTP0		(0x0FC20080)
#define CTP1		(0x0FC21080)
#define CTP2		(0x0FC22080)
#define CTP3		(0x0FC23080)
#define WAI0		(0x0FC20090)
#define WAI1		(0x0FC21090)
#define WAI2		(0x0FC22090)
#define WAI3		(0x0FC23090)
#define NCIAR0		(0x0FC200A0)
#define NCIAR1		(0x0FC210A0)
#define NCIAR2		(0x0FC220A0)
#define NCIAR3		(0x0FC230A0)
#define NCEOI0		(0x0FC200B0)
#define NCEOI1		(0x0FC210B0)
#define NCEOI2		(0x0FC220B0)
#define NCEOI3		(0x0FC230B0)
#define CIAR0		(0x0FC200C0)
#define CIAR1		(0x0FC210C0)
#define CIAR2		(0x0FC220C0)
#define CIAR3		(0x0FC230C0)
#define CEOI0		(0x0FC200D0)
#define CEOI1		(0x0FC210D0)
#define CEOI2		(0x0FC220D0)
#define CEOI3		(0x0FC230D0)
#define IAR0		(0x0FC200E0)
#define IAR1		(0x0FC210E0)
#define IAR2		(0x0FC220E0)
#define IAR3		(0x0FC230E0)
#define EOI0		(0x0FC200F0)
#define EOI1		(0x0FC210F0)
#define EOI2		(0x0FC220F0)
#define EOI3		(0x0FC230F0)
#define FRG0		(0x0FC01000)
#define GCF0		(0x0FC01020)
#define VITC		(0x0FC01040)
#define VRID		(0x0FC01050)
#define REVID		(0x0FC01070)
#define VIR0		(0x0FC01080)
#define PINI		(0x0FC01090)
#define IVP0		(0x0FC010A0)
#define IVP1		(0x0FC010B0)
#define IVP2		(0x0FC010C0)
#define IVP3		(0x0FC010D0)
#define SPV			(0x0FC010E0)
#define TFR			(0x0FC010F0)
#define TCC0		(0x0FC01100)
#define TCC1		(0x0FC01140)
#define TCC2		(0x0FC01180)
#define TCC3		(0x0FC011C0)
#define TBC0		(0x0FC01110)
#define TBC1		(0x0FC01150)
#define TBC2		(0x0FC01190)
#define TBC3		(0x0FC011D0)
#define TVP0		(0x0FC01120)
#define TVP1		(0x0FC01160)
#define TVP2		(0x0FC011A0)
#define TVP3		(0x0FC011E0)
#define TDST0		(0x0FC01130)
#define TDST1		(0x0FC01170)
#define TDST2		(0x0FC011B0)
#define TDST3		(0x0FC011F0)

#define MPIC128_R_BIT				(0x80000000)
/* MPIC RESET VALUES */
#define MPIC128_VP0_VALUE		reg_field(8,1) | reg_field(0,1)
#define MPIC128_DST0_VALUE	0x00000000
#define MPIC128_CTP0_VALUE	0x0000000F
#define MPIC128_CTP1_VALUE	0x0000000F
#define MPIC128_CTP2_VALUE	0x0000000F
#define MPIC128_CTP3_VALUE	0x0000000F
#define MPIC128_WAI0_VALUE	0xFFFFFFFF
#define MPIC128_WAI1_VALUE	0xFFFFFFFF
#define MPIC128_WAI2_VALUE	0xFFFFFFFF
#define MPIC128_WAI3_VALUE	0xFFFFFFFF
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

BEGIN_ENUM( MPIC128x_REG )
#define                         MPIC128x_IPID(x)    (0x00020040 + (0x1010*(x))) // MPIC Inter processor Interrupt Dispatch
DECLARE_ENUM_VAL(               MPIC128x_IPID0,     MPIC128x_IPID(0) )
DECLARE_ENUM_VAL(               MPIC128x_IPID1,     MPIC128x_IPID(1) )
DECLARE_ENUM_VAL(               MPIC128x_IPID2,     MPIC128x_IPID(2) )
DECLARE_ENUM_VAL(               MPIC128x_IPID3,     MPIC128x_IPID(3) )

DECLARE_ENUM_VAL(               MPIC128x_CTP_PR,    0x00000080 )                // Current Task Priority Register
#define                         MPIC128x_CTP(x)     (0x00020080 + (0x1000*(x)))
DECLARE_ENUM_VAL(               MPIC128x_CTP0,      MPIC128x_CTP(0) )
DECLARE_ENUM_VAL(               MPIC128x_CTP1,      MPIC128x_CTP(1) )
DECLARE_ENUM_VAL(               MPIC128x_CTP2,      MPIC128x_CTP(2) )
DECLARE_ENUM_VAL(               MPIC128x_CTP3,      MPIC128x_CTP(3) )

DECLARE_ENUM_VAL(               MPIC128x_WAI_PR,    0x00000090 )                // Who Am I Register
#define                         MPIC128x_WAI(x)     (0x00020090 + (0x1000*(x)))
DECLARE_ENUM_VAL(               MPIC128x_WAI0,      MPIC128x_WAI(0) )
DECLARE_ENUM_VAL(               MPIC128x_WAI1,      MPIC128x_WAI(1) )
DECLARE_ENUM_VAL(               MPIC128x_WAI2,      MPIC128x_WAI(2) )
DECLARE_ENUM_VAL(               MPIC128x_WAI3,      MPIC128x_WAI(3) )

DECLARE_ENUM_VAL(               MPIC128x_NCIAR_PR,  0x000000A0 )                // Non Critical Interrupt Acknowledge Register
#define                         MPIC128x_NCIAR(x)   (0x000200A0 + (0x1000*(x)))
DECLARE_ENUM_VAL(               MPIC128x_NCIAR0,    MPIC128x_NCIAR(0) )
DECLARE_ENUM_VAL(               MPIC128x_NCIAR1,    MPIC128x_NCIAR(1) )
DECLARE_ENUM_VAL(               MPIC128x_NCIAR2,    MPIC128x_NCIAR(2) )
DECLARE_ENUM_VAL(               MPIC128x_NCIAR3,    MPIC128x_NCIAR(3) )

DECLARE_ENUM_VAL(               MPIC128x_NCEOI_PR,  0x000000B0 )                // Non Critical End of Interrupt Register
#define                         MPIC128x_NCEOI(x)   (0x000200B0 + (0x1000*(x)))
DECLARE_ENUM_VAL(               MPIC128x_NCEOI0,    MPIC128x_NCEOI(0) )
DECLARE_ENUM_VAL(               MPIC128x_NCEOI1,    MPIC128x_NCEOI(1) )
DECLARE_ENUM_VAL(               MPIC128x_NCEOI2,    MPIC128x_NCEOI(2) )
DECLARE_ENUM_VAL(               MPIC128x_NCEOI3,    MPIC128x_NCEOI(3) )

DECLARE_ENUM_VAL(               MPIC128x_CIAR_PR,   0x000000C0 )                // Critical Interrupt Acknowledge Register
#define                         MPIC128x_CIAR(x)    (0x000200C0 + (0x1000*(x)))
DECLARE_ENUM_VAL(               MPIC128x_CIAR0,     MPIC128x_CIAR(0) )
DECLARE_ENUM_VAL(               MPIC128x_CIAR1,     MPIC128x_CIAR(1) )
DECLARE_ENUM_VAL(               MPIC128x_CIAR2,     MPIC128x_CIAR(2) )
DECLARE_ENUM_VAL(               MPIC128x_CIAR3,     MPIC128x_CIAR(3) )

DECLARE_ENUM_VAL(               MPIC128x_CEOI_PR,   0x000000D0 )                // Critical End of Interrupt Register
#define                         MPIC128x_CEOI(x)    (0x000200D0 + (0x1000*(x)))
DECLARE_ENUM_VAL(               MPIC128x_CEOI0,     MPIC128x_CEOI(0) )
DECLARE_ENUM_VAL(               MPIC128x_CEOI1,     MPIC128x_CEOI(1) )
DECLARE_ENUM_VAL(               MPIC128x_CEOI2,     MPIC128x_CEOI(2) )
DECLARE_ENUM_VAL(               MPIC128x_CEOI3,     MPIC128x_CEOI(3) )

DECLARE_ENUM_VAL(               MPIC128x_IAR_PR,    0x000000E0 )                // Machine Check Interrupt Acknowledge Register
#define                         MPIC128x_IAR(x)     (0x000200E0 + (0x1000*(x)))
DECLARE_ENUM_VAL(               MPIC128x_IAR0,      MPIC128x_IAR(0) )
DECLARE_ENUM_VAL(               MPIC128x_IAR1,      MPIC128x_IAR(1) )
DECLARE_ENUM_VAL(               MPIC128x_IAR2,      MPIC128x_IAR(2) )
DECLARE_ENUM_VAL(               MPIC128x_IAR3,      MPIC128x_IAR(3) )

DECLARE_ENUM_VAL(               MPIC128x_EOI_PR,    0x000000F0 )                // Machine Check End of Interrupt Register
#define                         MPIC128x_EOI(x)     (0x000200F0 + (0x1000*(x)))
DECLARE_ENUM_VAL(               MPIC128x_EOI0,      MPIC128x_EOI(0) )
DECLARE_ENUM_VAL(               MPIC128x_EOI1,      MPIC128x_EOI(1) )
DECLARE_ENUM_VAL(               MPIC128x_EOI2,      MPIC128x_EOI(2) )
DECLARE_ENUM_VAL(               MPIC128x_EOI3,      MPIC128x_EOI(3) )

DECLARE_ENUM_VAL(               MPIC128x_FRG0,      0x00001000 )                // Feature Reporting Register 0
DECLARE_ENUM_VAL(               MPIC128x_GCF0,      0x00001020 )                // Global Configuration Register 0
DECLARE_ENUM_VAL(               MPIC128x_VITC,      0x00001040 )                // Vendor Interrupt Type Configuration Register
DECLARE_ENUM_VAL(               MPIC128x_VRID,      0x00001050 )                // Vendor Raw Interrupt Debug Register
DECLARE_ENUM_VAL(               MPIC128x_REVID,     0x00001070 )                // Vendor Soft Core Revision ID Register
DECLARE_ENUM_VAL(               MPIC128x_VIR0,      0x00001080 )                // Vendor Identification Register 0
DECLARE_ENUM_VAL(               MPIC128x_PINI,      0x00001090 )                // Processor Initialization Register

#define                         MPIC128x_IVP(x)     (0x000010A0 + (0x0010*(x))) // IPI Vector/Priority Register
DECLARE_ENUM_VAL(               MPIC128x_IVP0,      MPIC128x_IVP(0) )
DECLARE_ENUM_VAL(               MPIC128x_IVP1,      MPIC128x_IVP(1) )
DECLARE_ENUM_VAL(               MPIC128x_IVP2,      MPIC128x_IVP(2) )
DECLARE_ENUM_VAL(               MPIC128x_IVP3,      MPIC128x_IVP(3) )

DECLARE_ENUM_VAL(               MPIC128x_SPV,       0x000010E0 )                // Spurious Vector Register

DECLARE_ENUM_VAL(               MPIC128x_TFR,       0x000010F0 )                // Timer Frequency Reporting Register

#define                         MPIC128x_TCC(x)     (0x00001100 + (0x0040*(x))) // Timer Current Count Register
DECLARE_ENUM_VAL(               MPIC128x_TCC0,      MPIC128x_TCC(0) )
DECLARE_ENUM_VAL(               MPIC128x_TCC1,      MPIC128x_TCC(1) )
DECLARE_ENUM_VAL(               MPIC128x_TCC2,      MPIC128x_TCC(2) )
DECLARE_ENUM_VAL(               MPIC128x_TCC3,      MPIC128x_TCC(3) )

#define                         MPIC128x_TBC(x)     (0x00001110 + (0x0040*(x))) // Timer Base Count Register
DECLARE_ENUM_VAL(               MPIC128x_TBC0,      MPIC128x_TBC(0) )
DECLARE_ENUM_VAL(               MPIC128x_TBC1,      MPIC128x_TBC(1) )
DECLARE_ENUM_VAL(               MPIC128x_TBC2,      MPIC128x_TBC(2) )
DECLARE_ENUM_VAL(               MPIC128x_TBC3,      MPIC128x_TBC(3) )

#define                         MPIC128x_TVP(x)     (0x00001120 + (0x0040*(x))) // Timer Vector/Priority Register
DECLARE_ENUM_VAL(               MPIC128x_TVP0,      MPIC128x_TVP(0) )
DECLARE_ENUM_VAL(               MPIC128x_TVP1,      MPIC128x_TVP(1) )
DECLARE_ENUM_VAL(               MPIC128x_TVP2,      MPIC128x_TVP(2) )
DECLARE_ENUM_VAL(               MPIC128x_TVP3,      MPIC128x_TVP(3) )

#define                         MPIC128x_TDST(x)    (0x00001130 + (0x0040*(x))) // Timer n Destination Register
DECLARE_ENUM_VAL(               MPIC128x_TDST0,     MPIC128x_TDST(0) )
DECLARE_ENUM_VAL(               MPIC128x_TDST1,     MPIC128x_TDST(1) )
DECLARE_ENUM_VAL(               MPIC128x_TDST2,     MPIC128x_TDST(2) )
DECLARE_ENUM_VAL(               MPIC128x_TDST3,     MPIC128x_TDST(3) )
END_ENUM( MPIC128x_REG )


BEGIN_ENUM( MPIC128x_REG_DFLT )
DECLARE_ENUM_VAL(   MPIC128x_VP_DFLT,       ( reg_field(8,1) | reg_field(0,1) ) )
DECLARE_ENUM_VAL(   MPIC128x_DST_DFLT,      0x00000000 )
DECLARE_ENUM_VAL(   MPIC128x_CTP_DFLT,      0x0000000F )
#define             MPIC128x_WAI_DFLT       SPR_PIR_read() //TODO: check this value
DECLARE_ENUM_VAL(   MPIC128x_SPV_DFLT,      reg_field(31,0xFF) )
DECLARE_ENUM_VAL(   MPIC128x_NCIAR_DFLT,    MPIC128x_SPV_DFLT )
DECLARE_ENUM_VAL(   MPIC128x_NCEOI_DFLT,    0x00000000 )
DECLARE_ENUM_VAL(   MPIC128x_CIAR_DFLT,     MPIC128x_SPV_DFLT )
DECLARE_ENUM_VAL(   MPIC128x_CEOI_DFLT,     0x00000000 )
DECLARE_ENUM_VAL(   MPIC128x_IAR_DFLT,      MPIC128x_SPV_DFLT )
DECLARE_ENUM_VAL(   MPIC128x_EOI_DFLT,      0x00000000 )
DECLARE_ENUM_VAL(   MPIC128x_FRG0_DFLT,     ( reg_field(31,2) | reg_field(23,3) | reg_field(15,127) ) )
DECLARE_ENUM_VAL(   MPIC128x_GCF0_DFLT,     reg_field(31,0xF) )
DECLARE_ENUM_VAL(   MPIC128x_VITC_DFLT,     ( reg_field(31,0x10) | reg_field(23,0x10) ) )
DECLARE_ENUM_VAL(   MPIC128x_VRID_DFLT,     0x00000000 )
DECLARE_ENUM_VAL(   MPIC128x_REVID_DFLT,    0x00000103 ) //??? TODO: check this dummy-value
DECLARE_ENUM_VAL(   MPIC128x_VIR0_DFLT,     0x00005014 ) //set acc. to STRAP-pins
DECLARE_ENUM_VAL(   MPIC128x_PINI_DFLT,     0x00000000 )
DECLARE_ENUM_VAL(   MPIC128x_IVP_DFLT,      reg_field(0,0x1) )
DECLARE_ENUM_VAL(   MPIC128x_TFR_DFLT,      0x00000000 )
DECLARE_ENUM_VAL(   MPIC128x_TCC_DFLT,      0x00000000 )
DECLARE_ENUM_VAL(   MPIC128x_TBC_DFLT,      reg_field(0,0x1) )
DECLARE_ENUM_VAL(   MPIC128x_TVP_DFLT,      reg_field(0,0x1) )
DECLARE_ENUM_VAL(   MPIC128x_TDST_DFLT,     0x00000000 )
END_ENUM( MPIC128x_REG_DFLT )


#endif // REGS_MPIC_H

