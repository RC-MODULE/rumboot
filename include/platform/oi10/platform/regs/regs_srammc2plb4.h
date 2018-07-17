/*
 * regs_srammc2plb4.h
 *
 *  Created on: May 4, 2018
 *      Author: d.arsentiev
 */

#ifndef REGS_SRAMMC2PLB4_H_
#define REGS_SRAMMC2PLB4_H_

#include <platform/common_macros/common_macros.h>

DECLARE_CONST( SRAMMC2PLB4_BANK_N,      4)
DECLARE_CONST( SRAMMC2PLB4_MASTER_N,    8)


DECLARE_CONST( SRAMMC2PLB4_SBxCR_BAS_e,     19 )
DECLARE_CONST( SRAMMC2PLB4_SBxCR_BAS_n,     20 )
DECLARE_CONST( SRAMMC2PLB4_SBxCR_BAS_i,     IBM_BIT_INDEX( 32, SRAMMC2PLB4_SBxCR_BAS_e ) )

DECLARE_CONST( SRAMMC2PLB4_SBxCR_BS_e,      22 )
DECLARE_CONST( SRAMMC2PLB4_SBxCR_BS_n,      3 )
DECLARE_CONST( SRAMMC2PLB4_SBxCR_BS_i,      IBM_BIT_INDEX( 32, SRAMMC2PLB4_SBxCR_BS_e ) )

BEGIN_ENUM(SRAMMC2PLB4_SBxCR_BS)
DECLARE_ENUM_VAL( SRAMMC2PLB4_SBxCR_BS_4KB,     0b000 )
DECLARE_ENUM_VAL( SRAMMC2PLB4_SBxCR_BS_8KB,     0b001 )
DECLARE_ENUM_VAL( SRAMMC2PLB4_SBxCR_BS_16KB,    0b010 )
DECLARE_ENUM_VAL( SRAMMC2PLB4_SBxCR_BS_32KB,    0b011 )
DECLARE_ENUM_VAL( SRAMMC2PLB4_SBxCR_BS_64KB,    0b100 )
DECLARE_ENUM_VAL( SRAMMC2PLB4_SBxCR_BS_128KB,   0b101 )
END_ENUM(SRAMMC2PLB4_SBxCR_BS)

DECLARE_CONST( SRAMMC2PLB4_SBxCR_BU_e,      24 )
DECLARE_CONST( SRAMMC2PLB4_SBxCR_BU_n,      2 )
DECLARE_CONST( SRAMMC2PLB4_SBxCR_BU_i,      IBM_BIT_INDEX( 32, SRAMMC2PLB4_SBxCR_BU_e ) )

BEGIN_ENUM(SRAMMC2PLB4_SBxCR_BU)
DECLARE_ENUM_VAL( SRAMMC2PLB4_SBxCR_BU_Disabled,    0b00 )
DECLARE_ENUM_VAL( SRAMMC2PLB4_SBxCR_BU_RO,          0b01 )
DECLARE_ENUM_VAL( SRAMMC2PLB4_SBxCR_BU_RW,          0b11 )
END_ENUM(SRAMMC2PLB4_SBxCR_BU)

DECLARE_CONST( SRAMMC2PLB4_SBxCR_UA_e,      31 )
DECLARE_CONST( SRAMMC2PLB4_SBxCR_UA_n,      4 )
DECLARE_CONST( SRAMMC2PLB4_SBxCR_UA_i,      IBM_BIT_INDEX( 32, SRAMMC2PLB4_SBxCR_UA_e ) )


DECLARE_CONST( SRAMMC2PLB4_BEAR_ABE_e,      31 )
DECLARE_CONST( SRAMMC2PLB4_BEAR_ABE_n,      32 )
DECLARE_CONST( SRAMMC2PLB4_BEAR_ABE_i,      IBM_BIT_INDEX( 32, SRAMMC2PLB4_BEAR_ABE_e ) )

BEGIN_ENUM(SRAMMC2PLB4_BESRx_ALy)
DECLARE_ENUM_VAL( SRAMMC2PLB4_BESRx_ALy_not_correspond, 0b0 )
DECLARE_ENUM_VAL( SRAMMC2PLB4_BESRx_ALy_correspond,     0b1 )
END_ENUM(SRAMMC2PLB4_BESRx_ALy)


DECLARE_CONST( SRAMMC2PLB4_PMEG_PM_EN_e,    0 )
DECLARE_CONST( SRAMMC2PLB4_PMEG_PM_EN_n,    1 )
DECLARE_CONST( SRAMMC2PLB4_PMEG_PM_EN_i,    IBM_BIT_INDEX( 32, SRAMMC2PLB4_PMEG_PM_EN_e ) )

DECLARE_CONST( SRAMMC2PLB4_PMEG_PM_CNT_e,   6 )
DECLARE_CONST( SRAMMC2PLB4_PMEG_PM_CNT_n,   6 )
DECLARE_CONST( SRAMMC2PLB4_PMEG_PM_CNT_i,   IBM_BIT_INDEX( 32, SRAMMC2PLB4_PMEG_PM_CNT_e ) )

DECLARE_CONST( SRAMMC2PLB4_PMEG_PM_DFLT_e,  10 )
DECLARE_CONST( SRAMMC2PLB4_PMEG_PM_DFLT_n,  4 )
DECLARE_CONST( SRAMMC2PLB4_PMEG_PM_DFLT_i,  IBM_BIT_INDEX( 32, SRAMMC2PLB4_PMEG_PM_DFLT_e ) )

DECLARE_CONST( SRAMMC2PLB4_PMEG_PM_DFLT_Hardwired,  0xF )


DECLARE_CONST( SRAMMC2PLB4_CID_e,   31 )
DECLARE_CONST( SRAMMC2PLB4_CID_n,   32 )
DECLARE_CONST( SRAMMC2PLB4_CID_i,   IBM_BIT_INDEX( 32, SRAMMC2PLB4_CID_e ) )


DECLARE_CONST( SRAMMC2PLB4_REVID_REV_e,     23 )
DECLARE_CONST( SRAMMC2PLB4_REVID_REV_n,     12 )
DECLARE_CONST( SRAMMC2PLB4_REVID_REV_i,     IBM_BIT_INDEX( 32, SRAMMC2PLB4_REVID_REV_e ) )

DECLARE_CONST( SRAMMC2PLB4_REVID_BRANCH_e,  31 )
DECLARE_CONST( SRAMMC2PLB4_REVID_BRANCH_n,  8 )
DECLARE_CONST( SRAMMC2PLB4_REVID_BRANCH_i,  IBM_BIT_INDEX( 32, SRAMMC2PLB4_REVID_BRANCH_e ) )


DECLARE_CONST( SRAMMC2PLB4_DPC_e,   0 )
DECLARE_CONST( SRAMMC2PLB4_DPC_n,   1 )
DECLARE_CONST( SRAMMC2PLB4_DPC_i,   IBM_BIT_INDEX( 32, SRAMMC2PLB4_DPC_e ) )

BEGIN_ENUM(SRAMMC2PLB4_DataParityCheck)
DECLARE_ENUM_VAL( SRAMMC2PLB4_DPC_Disabled, 0b0 )
DECLARE_ENUM_VAL( SRAMMC2PLB4_DPC_Enabled,  0b1 )
END_ENUM(SRAMMC2PLB4_DataParityCheck)

BEGIN_ENUM( SRAMMC2PLB4 )
DECLARE_ENUM_VAL( SRAMMC2PLB4_BEAR,                                     0x04 )
#define SRAMMC2PLB4_BESRx_N 2
#define SRAMMC2PLB4_BESRx( x, z )\
DECLARE_ENUM_VAL( CAT( SRAMMC2PLB4_BESR,x ),                   0x05+x )
REPEAT(DEC(SRAMMC2PLB4_BESRx_N),SRAMMC2PLB4_BESRx,z)
DECLARE_ENUM_VAL( SRAMMC2PLB4_PMEG,                                     0x07 )
DECLARE_ENUM_VAL( SRAMMC2PLB4_CID,                                      0x08 )
DECLARE_ENUM_VAL( SRAMMC2PLB4_REVID,                                    0x09 )
DECLARE_ENUM_VAL( SRAMMC2PLB4_DPC,                                      0x0A )
END_ENUM( SRAMMC2PLB4 )


DECLARE_CONST( SRAMMC2PLB4_BEAR_DFLT,   0x00000000 )
DECLARE_CONST( SRAMMC2PLB4_BESRx_DFLT,  0x00000000 )
DECLARE_CONST( SRAMMC2PLB4_PMEG_DFLT,   0x00000000 )
DECLARE_CONST( SRAMMC2PLB4_CID_DFLT,    0x322B0000 )
DECLARE_CONST( SRAMMC2PLB4_REVID_DFLT,  0x00000160 )
DECLARE_CONST( SRAMMC2PLB4_DPC_DFLT,    0x01E00000 )

/* values from units/cpu/units/ppc_chip/verilog/rtl/PPC470S_TOP111323221111.v */
#define SRAMMC2PLB4_0_SB0CR_RESET_VAL	0xFFFF048F
#define SRAMMC2PLB4_0_SB1CR_RESET_VAL	0xFFFF448F
#define SRAMMC2PLB4_0_SB2CR_RESET_VAL	0xFFFF848F
#define SRAMMC2PLB4_0_SB3CR_RESET_VAL	0xFFFFC48F

#define SRAMMC2PLB4_1_SB0CR_RESET_VAL	0x80000780
#define SRAMMC2PLB4_1_SB1CR_RESET_VAL	0x80008780
#define SRAMMC2PLB4_1_SB2CR_RESET_VAL	0x80010780
#define SRAMMC2PLB4_1_SB3CR_RESET_VAL	0x80018780


#endif /* REGS_SRAMMC2PLB4_H_ */
