#ifndef __MPIC128_REGS_H__
#define __MPIC128_REGS_H__


//#include <mivem_types.h>
#include <common_macros/common_macros.h>
#include <arch/ppc/ibm_bit_ordering_macros.h>
#include <platform/oi10/platform/boost/preprocessor/iteration/local.hpp>
#include <platform/oi10/platform/boost/preprocessor/arithmetic/dec.hpp>
#include <platform/oi10/platform/boost/preprocessor/cat.hpp>

#include <platform/oi10/platform/mpicOI10/mpicOI10_reg_fields.h>


#define MPIC128x_VPx_REGS_COUNT    128
#define MPIC128x_DSTx_REGS_COUNT   128
#define MPIC128x_REGS_COUNT_4      4

BEGIN_ENUM( MPIC128x_REG )
#define                         MPIC128x_VP(x)      (0x00010000 + (0x0020*(x))) // MPIC Vector/Priority Register
#define BOOST_PP_LOCAL_MACRO( x )\
DECLARE_ENUM_VAL( BOOST_PP_CAT( MPIC128x_VP,x ),    MPIC128x_VP(x) )
#define BOOST_PP_LOCAL_LIMITS (0, BOOST_PP_DEC(MPIC128x_VPx_REGS_COUNT))
#include BOOST_PP_LOCAL_ITERATE()

#define                         MPIC128x_DST(x)     (0x00010010 + (0x0020*(x))) // MPIC Destination Register
#define BOOST_PP_LOCAL_MACRO( x )\
DECLARE_ENUM_VAL( BOOST_PP_CAT( MPIC128x_DST,x ),   MPIC128x_DST(x) )
#define BOOST_PP_LOCAL_LIMITS (0, BOOST_PP_DEC(MPIC128x_DSTx_REGS_COUNT))
#include BOOST_PP_LOCAL_ITERATE()

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

#endif // __MPIC128_REGS_H__
