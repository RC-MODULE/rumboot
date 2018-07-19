/*
 * regs_lscb.h
 *
 *  Created on: Jul 19, 2018
 *      Author: d.arsentiev
 */

#ifndef REGS_LSCB_H_
#define REGS_LSCB_H_

#include <platform/common_macros/common_macros.h>

BEGIN_ENUM( MSP_REG )
    DECLARE_ENUM_VAL( MSP_IMR,              0x00 )
    DECLARE_ENUM_VAL( MSP_CFG1_,            0x01 )
    DECLARE_ENUM_VAL( MSP_CFG2_,            0x02 )
    DECLARE_ENUM_VAL( MSP_SRR,              0x03 )
    DECLARE_ENUM_VAL( MSP_BC_CW,            0X04 )
    DECLARE_ENUM_VAL( MSP_TT_REG,           0X05 )
    DECLARE_ENUM_VAL( MSP_ISR,              0x06 )
    DECLARE_ENUM_VAL( MSP_CFG3_,            0x07 )
    DECLARE_ENUM_VAL( MSP_CFG4_,            0x08 )
    DECLARE_ENUM_VAL( MSP_CFG5_,            0x09 )
    DECLARE_ENUM_VAL( MSP_RM_DSA,           0x0A )
    DECLARE_ENUM_VAL( MSP_BC_FTR,           0x0B )
    DECLARE_ENUM_VAL( MSP_BC_TRNM,          0x0C )
    DECLARE_ENUM_VAL( MSP_BCFT_RTLC_MTTW,   0x0D )
    DECLARE_ENUM_VAL( MSP_RT_SW,            0x0E )
    DECLARE_ENUM_VAL( MSP_RT_BIT_W,         0x0F )
    DECLARE_ENUM_VAL( MSP_CFG6_,            0x18 )
END_ENUM( MSP_REG )

#define MSP_CSP     MSP_SRR
#define MSP_RT_CW   MSP_BC_CW


#endif /* REGS_LSCB_H_ */
