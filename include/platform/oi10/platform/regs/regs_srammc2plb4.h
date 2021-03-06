/*
 * regs_srammc2plb4.h
 *
 *  Created on: May 4, 2018
 *      Author: d.arsentiev
 */

#ifndef REGS_SRAMMC2PLB4_H_
#define REGS_SRAMMC2PLB4_H_


#include <arch/common_macros.h>
#include <arch/ibm_bit_ordering_macros.h>


DECLARE_CONST( SRAMMC2PLB4_BANK_N,      4)
DECLARE_CONST( SRAMMC2PLB4_MASTER_N,    8)


BEGIN_ENUM( SRAMMC2PLB4_SBxCR_FIELD )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_SBxCR_BAS_e,      19 )    /**< Base Address Select */
    DECLARE_ENUM_VAL( SRAMMC2PLB4_SBxCR_BAS_n,      20 )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_SBxCR_BAS_i,      IBM_BIT_INDEX(32, SRAMMC2PLB4_SBxCR_BAS_e) )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_SBxCR_BAS_mask,   FIELD_MASK32(SRAMMC2PLB4_SBxCR_BAS_i, SRAMMC2PLB4_SBxCR_BAS_n) )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_SBxCR_BS_e,       22 )    /**< Bank Size */
    DECLARE_ENUM_VAL( SRAMMC2PLB4_SBxCR_BS_n,       3 )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_SBxCR_BS_i,       IBM_BIT_INDEX(32, SRAMMC2PLB4_SBxCR_BS_e) )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_SBxCR_BS_mask,    FIELD_MASK32(SRAMMC2PLB4_SBxCR_BS_i, SRAMMC2PLB4_SBxCR_BS_n) )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_SBxCR_BU_e,       24 )    /**< Bank Usage */
    DECLARE_ENUM_VAL( SRAMMC2PLB4_SBxCR_BU_n,       2 )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_SBxCR_BU_i,       IBM_BIT_INDEX(32, SRAMMC2PLB4_SBxCR_BU_e) )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_SBxCR_BU_mask,    FIELD_MASK32(SRAMMC2PLB4_SBxCR_BU_i, SRAMMC2PLB4_SBxCR_BU_n) )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_SBxCR_UA_e,       31 )    /**< Upper Address Select */
    DECLARE_ENUM_VAL( SRAMMC2PLB4_SBxCR_UA_n,       4 )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_SBxCR_UA_i,       IBM_BIT_INDEX(32, SRAMMC2PLB4_SBxCR_UA_e) )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_SBxCR_UA_mask,    FIELD_MASK32(SRAMMC2PLB4_SBxCR_UA_i, SRAMMC2PLB4_SBxCR_UA_n) )
END_ENUM( SRAMMC2PLB4_SBxCR_FIELD )
/** Bank Size */
BEGIN_ENUM( srammc2plb4_bs )
    DECLARE_ENUM_VAL( srammc2plb4_bs_4kb,   0b000 )
    DECLARE_ENUM_VAL( srammc2plb4_bs_8kb,   0b001 )
    DECLARE_ENUM_VAL( srammc2plb4_bs_16kb,  0b010 )
    DECLARE_ENUM_VAL( srammc2plb4_bs_32kb,  0b011 )
    DECLARE_ENUM_VAL( srammc2plb4_bs_64kb,  0b100 )
    DECLARE_ENUM_VAL( srammc2plb4_bs_128kb, 0b101 )
END_ENUM( srammc2plb4_bs )
/** Bank Usage */
BEGIN_ENUM( srammc2plb4_bu )
    DECLARE_ENUM_VAL( srammc2plb4_bu_disabled,  0b00 )
    DECLARE_ENUM_VAL( srammc2plb4_bu_ro,        0b01 )
    DECLARE_ENUM_VAL( srammc2plb4_bu_rw,        0b11 )
END_ENUM( srammc2plb4_bu )

BEGIN_ENUM( SRAMMC2PLB4_BEAR_FIELD )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BEAR_ABE_e,       31 )    /** Address of Bus Error */
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BEAR_ABE_n,       32 )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BEAR_ABE_i,       IBM_BIT_INDEX(32, SRAMMC2PLB4_BEAR_ABE_e) )
END_ENUM( SRAMMC2PLB4_BEAR_FIELD )

DECLARE_CONST( SRAMMC2PLB4_BESRx_N,         2 )
DECLARE_CONST( SRAMMC2PLB4_BESRx_MASTER_N,  SRAMMC2PLB4_MASTER_N/SRAMMC2PLB4_BESRx_N )
BEGIN_ENUM( SRAMMC2PLB4_BESRx_FIELD )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESRx_ETy_n,      3 )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR0_ET0_e,      2 )                                             /**< Error Type for master 0 */
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR0_ET0_i,      IBM_BIT_INDEX(32, SRAMMC2PLB4_BESR0_ET0_e) )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR0_ET0_mask,   FIELD_MASK32(SRAMMC2PLB4_BESR0_ET0_i, SRAMMC2PLB4_BESRx_ETy_n) )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR0_ET1_e,      8 )                                             /**< Error Type for master 1 */
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR0_ET1_i,      IBM_BIT_INDEX(32, SRAMMC2PLB4_BESR0_ET1_e) )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR0_ET1_mask,   FIELD_MASK32(SRAMMC2PLB4_BESR0_ET1_i, SRAMMC2PLB4_BESRx_ETy_n) )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR0_ET2_e,      14 )                                            /**< Error Type for master 2 */
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR0_ET2_i,      IBM_BIT_INDEX(32, SRAMMC2PLB4_BESR0_ET2_e) )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR0_ET2_mask,   FIELD_MASK32(SRAMMC2PLB4_BESR0_ET2_i, SRAMMC2PLB4_BESRx_ETy_n) )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR0_ET3_e,      20 )                                            /**< Error Type for master 3 */
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR0_ET3_i,      IBM_BIT_INDEX(32, SRAMMC2PLB4_BESR0_ET3_e) )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR0_ET3_mask,   FIELD_MASK32(SRAMMC2PLB4_BESR0_ET3_i, SRAMMC2PLB4_BESRx_ETy_n) )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR1_ET4_e,      SRAMMC2PLB4_BESR0_ET0_e )                       /**< Error Type for master 4 */
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR1_ET4_i,      SRAMMC2PLB4_BESR0_ET0_i )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR1_ET4_mask,   SRAMMC2PLB4_BESR0_ET0_mask )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR1_ET5_e,      SRAMMC2PLB4_BESR0_ET1_e )                       /**< Error Type for master 5 */
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR1_ET5_i,      SRAMMC2PLB4_BESR0_ET1_i )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR1_ET5_mask,   SRAMMC2PLB4_BESR0_ET1_mask )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR1_ET6_e,      SRAMMC2PLB4_BESR0_ET2_e )                       /**< Error Type for master 6 */
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR1_ET6_i,      SRAMMC2PLB4_BESR0_ET2_i )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR1_ET6_mask,   SRAMMC2PLB4_BESR0_ET2_mask )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR1_ET7_e,      SRAMMC2PLB4_BESR0_ET3_e )                       /**< Error Type for master 7 */
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR1_ET7_i,      SRAMMC2PLB4_BESR0_ET3_i )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR1_ET7_mask,   SRAMMC2PLB4_BESR0_ET3_mask )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESRx_RWSy_n,     1 )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR0_RWS0_e,     3 )                                             /**< Read/Write Status for master 0 */
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR0_RWS0_i,     IBM_BIT_INDEX(32, SRAMMC2PLB4_BESR0_RWS0_e) )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR0_RWS0_mask,  FIELD_MASK32(SRAMMC2PLB4_BESR0_RWS0_i, SRAMMC2PLB4_BESRx_RWSy_n) )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR0_RWS1_e,     9 )                                             /**< Read/Write Status for master 1 */
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR0_RWS1_i,     IBM_BIT_INDEX(32, SRAMMC2PLB4_BESR0_RWS1_e) )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR0_RWS1_mask,  FIELD_MASK32(SRAMMC2PLB4_BESR0_RWS1_i, SRAMMC2PLB4_BESRx_RWSy_n) )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR0_RWS2_e,     15 )                                            /**< Read/Write Status for master 2 */
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR0_RWS2_i,     IBM_BIT_INDEX(32, SRAMMC2PLB4_BESR0_RWS2_e) )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR0_RWS2_mask,  FIELD_MASK32(SRAMMC2PLB4_BESR0_RWS2_i, SRAMMC2PLB4_BESRx_RWSy_n) )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR0_RWS3_e,     21 )                                            /**< Read/Write Status for master 3 */
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR0_RWS3_i,     IBM_BIT_INDEX(32, SRAMMC2PLB4_BESR0_RWS3_e) )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR0_RWS3_mask,  FIELD_MASK32(SRAMMC2PLB4_BESR0_RWS3_i, SRAMMC2PLB4_BESRx_RWSy_n) )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR1_RWS4_e,     SRAMMC2PLB4_BESR0_RWS0_e )                      /**< Read/Write Status for master 4 */
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR1_RWS4_i,     SRAMMC2PLB4_BESR0_RWS0_i )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR1_RWS4_mask,  SRAMMC2PLB4_BESR0_RWS0_mask )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR1_RWS5_e,     SRAMMC2PLB4_BESR0_RWS1_e )                      /**< Read/Write Status for master 5 */
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR1_RWS5_i,     SRAMMC2PLB4_BESR0_RWS1_i )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR1_RWS5_mask,  SRAMMC2PLB4_BESR0_RWS1_mask )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR1_RWS6_e,     SRAMMC2PLB4_BESR0_RWS2_e )                      /**< Read/Write Status for master 6 */
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR1_RWS6_i,     SRAMMC2PLB4_BESR0_RWS2_i )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR1_RWS6_mask,  SRAMMC2PLB4_BESR0_RWS2_mask )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR1_RWS7_e,     SRAMMC2PLB4_BESR0_RWS3_e )                      /**< Read/Write Status for master 7 */
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR1_RWS7_i,     SRAMMC2PLB4_BESR0_RWS3_i )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR1_RWS7_mask,  SRAMMC2PLB4_BESR0_RWS3_mask )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESRx_FLy_n,      1 )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR0_FL0_e,      4 )                                             /**< Field Lock for master 0 */
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR0_FL0_i,      IBM_BIT_INDEX(32, SRAMMC2PLB4_BESR0_FL0_e) )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR0_FL0_mask,   FIELD_MASK32(SRAMMC2PLB4_BESR0_FL0_i, SRAMMC2PLB4_BESRx_FLy_n) )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR0_FL1_e,      10 )                                            /**< Field Lock for master 1 */
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR0_FL1_i,      IBM_BIT_INDEX(32, SRAMMC2PLB4_BESR0_FL1_e) )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR0_FL1_mask,   FIELD_MASK32(SRAMMC2PLB4_BESR0_FL1_i, SRAMMC2PLB4_BESRx_FLy_n) )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR0_FL2_e,      16 )                                            /**< Field Lock for master 2 */
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR0_FL2_i,      IBM_BIT_INDEX(32, SRAMMC2PLB4_BESR0_FL2_e) )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR0_FL2_mask,   FIELD_MASK32(SRAMMC2PLB4_BESR0_FL2_i, SRAMMC2PLB4_BESRx_FLy_n) )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR0_FL3_e,      22 )                                            /**< Field Lock for master 3 */
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR0_FL3_i,      IBM_BIT_INDEX(32, SRAMMC2PLB4_BESR0_FL3_e) )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR0_FL3_mask,   FIELD_MASK32(SRAMMC2PLB4_BESR0_FL3_i, SRAMMC2PLB4_BESRx_FLy_n) )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR1_FL4_e,      SRAMMC2PLB4_BESR0_FL0_e )                       /**< Field Lock for master 4 */
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR1_FL4_i,      SRAMMC2PLB4_BESR0_FL0_i )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR1_FL4_mask,   SRAMMC2PLB4_BESR0_FL0_mask )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR1_FL5_e,      SRAMMC2PLB4_BESR0_FL1_e )                       /**< Field Lock for master 5 */
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR1_FL5_i,      SRAMMC2PLB4_BESR0_FL1_i )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR1_FL5_mask,   SRAMMC2PLB4_BESR0_FL1_mask )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR1_FL6_e,      SRAMMC2PLB4_BESR0_FL2_e )                       /**< Field Lock for master 6 */
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR1_FL6_i,      SRAMMC2PLB4_BESR0_FL2_i )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR1_FL6_mask,   SRAMMC2PLB4_BESR0_FL2_mask )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR1_FL7_e,      SRAMMC2PLB4_BESR0_FL3_e )                       /**< Field Lock for master 7 */
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR1_FL7_i,      SRAMMC2PLB4_BESR0_FL3_i )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR1_FL7_mask,   SRAMMC2PLB4_BESR0_FL3_mask )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESRx_ALy_n,      1 )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR0_AL0_e,      4 )                                             /**< BEAR Address Lock for master 0 */
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR0_AL0_i,      IBM_BIT_INDEX(32, SRAMMC2PLB4_BESR0_AL0_e) )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR0_AL0_mask,   FIELD_MASK32(SRAMMC2PLB4_BESR0_AL0_i, SRAMMC2PLB4_BESRx_ALy_n) )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR0_AL1_e,      10 )                                            /**< BEAR Address Lock for master 1 */
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR0_AL1_i,      IBM_BIT_INDEX(32, SRAMMC2PLB4_BESR0_AL1_e) )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR0_AL1_mask,   FIELD_MASK32(SRAMMC2PLB4_BESR0_AL1_i, SRAMMC2PLB4_BESRx_ALy_n) )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR0_AL2_e,      16 )                                            /**< BEAR Address Lock for master 2 */
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR0_AL2_i,      IBM_BIT_INDEX(32, SRAMMC2PLB4_BESR0_AL2_e) )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR0_AL2_mask,   FIELD_MASK32(SRAMMC2PLB4_BESR0_AL2_i, SRAMMC2PLB4_BESRx_ALy_n) )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR0_AL3_e,      22 )                                            /**< BEAR Address Lock for master 3 */
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR0_AL3_i,      IBM_BIT_INDEX(32, SRAMMC2PLB4_BESR0_AL3_e) )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR0_AL3_mask,   FIELD_MASK32(SRAMMC2PLB4_BESR0_AL3_i, SRAMMC2PLB4_BESRx_ALy_n) )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR1_AL4_e,      SRAMMC2PLB4_BESR0_AL0_e )                       /**< BEAR Address Lock for master 4 */
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR1_AL4_i,      SRAMMC2PLB4_BESR0_AL0_i )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR1_AL4_mask,   SRAMMC2PLB4_BESR0_AL0_mask )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR1_AL5_e,      SRAMMC2PLB4_BESR0_AL1_e )                       /**< BEAR Address Lock for master 5 */
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR1_AL5_i,      SRAMMC2PLB4_BESR0_AL1_i )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR1_AL5_mask,   SRAMMC2PLB4_BESR0_AL1_mask )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR1_AL6_e,      SRAMMC2PLB4_BESR0_AL2_e )                       /**< BEAR Address Lock for master 6 */
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR1_AL6_i,      SRAMMC2PLB4_BESR0_AL2_i )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR1_AL6_mask,   SRAMMC2PLB4_BESR0_AL2_mask )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR1_AL7_e,      SRAMMC2PLB4_BESR0_AL3_e )                       /**< BEAR Address Lock for master 7 */
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR1_AL7_i,      SRAMMC2PLB4_BESR0_AL3_i )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR1_AL7_mask,   SRAMMC2PLB4_BESR0_AL3_mask )
END_ENUM( SRAMMC2PLB4_BESRx_FIELD )
/** Error Type for master */
BEGIN_ENUM( srammc2plb4_et )
    DECLARE_ENUM_VAL( srammc2plb4_et_no_error,          0b000 )
    DECLARE_ENUM_VAL( srammc2plb4_et_parity_error,      0b010 )
    DECLARE_ENUM_VAL( srammc2plb4_et_protection_error,  0b100 )
END_ENUM( srammc2plb4_et )
/** Read/Write Status for master */
BEGIN_ENUM( srammc2plb4_rws )
    DECLARE_ENUM_VAL( srammc2plb4_rws_write_error,  0b0 )
    DECLARE_ENUM_VAL( srammc2plb4_rws_read_error,   0b1 )
END_ENUM( srammc2plb4_rws )
/** Field Lock for master */
BEGIN_ENUM( srammc2plb4_fl )
    DECLARE_ENUM_VAL( srammc2plb4_fl_unlocked,  0b0 )   /**< ET, RWS, and AL fields are unlocked */
    DECLARE_ENUM_VAL( srammc2plb4_fl_locked,    0b1 )   /**< ET, RWS, and AL fields are locked */
END_ENUM( srammc2plb4_fl )
/** BEAR Address Lock for master */
BEGIN_ENUM( srammc2plb4_al )
    DECLARE_ENUM_VAL( srammc2plb4_al_not_correspond, 0b0 )  /**< BEAR address does not correspond to master */
    DECLARE_ENUM_VAL( srammc2plb4_al_correspond,     0b1 )  /**< BEAR address corresponds to master */
END_ENUM( srammc2plb4_al )

BEGIN_ENUM( SRAMMC2PLB4_PMEG_FIELD )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_PMEG_PM_EN_e,         0 ) /**< Power Management enable */
    DECLARE_ENUM_VAL( SRAMMC2PLB4_PMEG_PM_EN_n,         1 )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_PMEG_PM_EN_i,         IBM_BIT_INDEX(32, SRAMMC2PLB4_PMEG_PM_EN_e) )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_PMEG_PM_EN_mask,      FIELD_MASK32(SRAMMC2PLB4_PMEG_PM_EN_i, SRAMMC2PLB4_PMEG_PM_EN_n) )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_PMEG_PM_CNT_e,        6 ) /**< Power Management Counter */
    DECLARE_ENUM_VAL( SRAMMC2PLB4_PMEG_PM_CNT_n,        6 )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_PMEG_PM_CNT_i,        IBM_BIT_INDEX(32, SRAMMC2PLB4_PMEG_PM_CNT_e) )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_PMEG_PM_CNT_mask,     FIELD_MASK32(SRAMMC2PLB4_PMEG_PM_CNT_i, SRAMMC2PLB4_PMEG_PM_CNT_n) )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_PMEG_PM_DFLT_e,       10 )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_PMEG_PM_DFLT_n,       4 ) /**< Power Management Default Wait Interval. Hardwired to a binary value of ?1111? (decimal 15). Read-only. */
    DECLARE_ENUM_VAL( SRAMMC2PLB4_PMEG_PM_DFLT_i,       IBM_BIT_INDEX(32, SRAMMC2PLB4_PMEG_PM_DFLT_e) )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_PMEG_PM_DFLT_mask,    FIELD_MASK32(SRAMMC2PLB4_PMEG_PM_DFLT_i, SRAMMC2PLB4_PMEG_PM_DFLT_n) )
END_ENUM( SRAMMC2PLB4_PMEG_FIELD )

BEGIN_ENUM( SRAMMC2PLB4_CID_FIELD )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_CID_e,    31 )    /**< Core ID */
    DECLARE_ENUM_VAL( SRAMMC2PLB4_CID_n,    32 )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_CID_i,    IBM_BIT_INDEX(32, SRAMMC2PLB4_CID_e) )
END_ENUM( SRAMMC2PLB4_CID_FIELD )

BEGIN_ENUM( SRAMMC2PLB4_REVID_FIELD )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_REVID_REV_e,          23 )    /**< Revision Number */
    DECLARE_ENUM_VAL( SRAMMC2PLB4_REVID_REV_n,          12 )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_REVID_REV_i,          IBM_BIT_INDEX(32, SRAMMC2PLB4_REVID_REV_e) )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_REVID_REV_mask,       FIELD_MASK32(SRAMMC2PLB4_REVID_REV_i, SRAMMC2PLB4_REVID_REV_n) )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_REVID_BRANCH_e,       31 )    /**< Branch Revision Number */
    DECLARE_ENUM_VAL( SRAMMC2PLB4_REVID_BRANCH_n,       8 )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_REVID_BRANCH_i,       IBM_BIT_INDEX(32, SRAMMC2PLB4_REVID_BRANCH_e) )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_REVID_BRANCH_mask,    FIELD_MASK32(SRAMMC2PLB4_REVID_BRANCH_i, SRAMMC2PLB4_REVID_BRANCH_n) )
END_ENUM( SRAMMC2PLB4_REVID_FIELD )

BEGIN_ENUM( SRAMMC2PLB4_DPC_FIELD )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_DPC_e,    0 )     /**< Data Parity Check Enable */
    DECLARE_ENUM_VAL( SRAMMC2PLB4_DPC_n,    1 )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_DPC_i,    IBM_BIT_INDEX(32, SRAMMC2PLB4_DPC_e) )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_DPC_mask, FIELD_MASK32(SRAMMC2PLB4_DPC_i, SRAMMC2PLB4_DPC_n) )
END_ENUM( SRAMMC2PLB4_DPC_FIELD )

/** SRAMMC2PLB4 registers */
BEGIN_ENUM( SRAMMC2PLB4_REG )
    #define           SRAMMC2PLB4_SBxCR( x )    ( 0x00 + (x) )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_SB0CR,        SRAMMC2PLB4_SBxCR( 0 ) )    /**< SRAM Bank 0 Configuration Register */
    DECLARE_ENUM_VAL( SRAMMC2PLB4_SB1CR,        SRAMMC2PLB4_SBxCR( 1 ) )    /**< SRAM Bank 1 Configuration Register */
    DECLARE_ENUM_VAL( SRAMMC2PLB4_SB2CR,        SRAMMC2PLB4_SBxCR( 2 ) )    /**< SRAM Bank 2 Configuration Register */
    DECLARE_ENUM_VAL( SRAMMC2PLB4_SB3CR,        SRAMMC2PLB4_SBxCR( 3 ) )    /**< SRAM Bank 3 Configuration Register */
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BEAR,         0x04 )                      /**< Bus Error Address Register */
    #define           SRAMMC2PLB4_BESRx( x )    ( 0x05 + (x) )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR0,        SRAMMC2PLB4_BESRx( 0 ) )    /**< Bus Error Status Register 0 */
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR1,        SRAMMC2PLB4_BESRx( 1 ) )    /**< Bus Error Status Register 1 */
    DECLARE_ENUM_VAL( SRAMMC2PLB4_PMEG,         0x07 )                      /**< Power Management Register */
    DECLARE_ENUM_VAL( SRAMMC2PLB4_CID,          0x08 )                      /**< Core Identification Register */
    DECLARE_ENUM_VAL( SRAMMC2PLB4_REVID,        0x09 )                      /**< Core Revision ID Register */
    DECLARE_ENUM_VAL( SRAMMC2PLB4_DPC,          0x0A )                      /**< Data Parity Checking Register */
END_ENUM( SRAMMC2PLB4_REG )

/** SRAMMC2PLB4 register reset values */
BEGIN_ENUM( SRAMMC2PLB4_REG_DFLT )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_SBxCR_DFLT,   0x00000000 )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BEAR_DFLT,    0x00000000 )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESRx_DFLT,   0x00000000 )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_PMEG_DFLT,    (0b1111 << SRAMMC2PLB4_PMEG_PM_DFLT_i) )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_CID_DFLT,     0x322B0000 )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_REVID_DFLT,   0x00000160 )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_DPC_DFLT,     0x00000000 )
END_ENUM( SRAMMC2PLB4_REG_DFLT )

/** SRAMMC2PLB4 register masks */
BEGIN_ENUM( SRAMMC2PLB4_REG_MASK )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_SBxCR_MASK,   SRAMMC2PLB4_SBxCR_BAS_mask
                                              | SRAMMC2PLB4_SBxCR_BS_mask
                                              | SRAMMC2PLB4_SBxCR_BU_mask
                                              | SRAMMC2PLB4_SBxCR_UA_mask )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BEAR_MASK,    0xFFFFFFFF )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR0_MASK,   SRAMMC2PLB4_BESR0_ET0_mask
                                              | SRAMMC2PLB4_BESR0_RWS0_mask
                                              | SRAMMC2PLB4_BESR0_FL0_mask
                                              | SRAMMC2PLB4_BESR0_AL0_mask
                                              | SRAMMC2PLB4_BESR0_ET1_mask
                                              | SRAMMC2PLB4_BESR0_RWS1_mask
                                              | SRAMMC2PLB4_BESR0_FL1_mask
                                              | SRAMMC2PLB4_BESR0_AL1_mask
                                              | SRAMMC2PLB4_BESR0_ET2_mask
                                              | SRAMMC2PLB4_BESR0_RWS2_mask
                                              | SRAMMC2PLB4_BESR0_FL2_mask
                                              | SRAMMC2PLB4_BESR0_AL2_mask
                                              | SRAMMC2PLB4_BESR0_ET3_mask
                                              | SRAMMC2PLB4_BESR0_RWS3_mask
                                              | SRAMMC2PLB4_BESR0_FL3_mask
                                              | SRAMMC2PLB4_BESR0_AL3_mask )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_BESR1_MASK,   SRAMMC2PLB4_BESR1_ET4_mask
                                              | SRAMMC2PLB4_BESR1_RWS4_mask
                                              | SRAMMC2PLB4_BESR1_FL4_mask
                                              | SRAMMC2PLB4_BESR1_AL4_mask
                                              | SRAMMC2PLB4_BESR1_ET5_mask
                                              | SRAMMC2PLB4_BESR1_RWS5_mask
                                              | SRAMMC2PLB4_BESR1_FL5_mask
                                              | SRAMMC2PLB4_BESR1_AL5_mask
                                              | SRAMMC2PLB4_BESR1_ET6_mask
                                              | SRAMMC2PLB4_BESR1_RWS6_mask
                                              | SRAMMC2PLB4_BESR1_FL6_mask
                                              | SRAMMC2PLB4_BESR1_AL6_mask
                                              | SRAMMC2PLB4_BESR1_ET7_mask
                                              | SRAMMC2PLB4_BESR1_RWS7_mask
                                              | SRAMMC2PLB4_BESR1_FL7_mask
                                              | SRAMMC2PLB4_BESR1_AL7_mask )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_PMEG_MASK,    SRAMMC2PLB4_PMEG_PM_EN_mask
                                              | SRAMMC2PLB4_PMEG_PM_CNT_mask
                                              | SRAMMC2PLB4_PMEG_PM_DFLT_mask )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_CID_MASK,     0xFFFFFFFF )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_REVID_MASK,   SRAMMC2PLB4_REVID_REV_mask
                                              | SRAMMC2PLB4_REVID_BRANCH_mask )
    DECLARE_ENUM_VAL( SRAMMC2PLB4_DPC_MASK,     SRAMMC2PLB4_DPC_mask )
END_ENUM( SRAMMC2PLB4_REG_MASK )


#endif /* REGS_SRAMMC2PLB4_H_ */
