/*
 * ppc_476fp_l1c_fields.h
 *
 *  Created on: Jan 28, 2019
 *      Author: v.strukov
 */

#ifndef RUMBOOT_INCLUDE_PLATFORM_OI10_PLATFORM_ARCH_PPC_PPC_476FP_L1C_FIELDS_H_
#define RUMBOOT_INCLUDE_PLATFORM_OI10_PLATFORM_ARCH_PPC_PPC_476FP_L1C_FIELDS_H_

BEGIN_ENUM(XCREAD_EA_FIELD)
    DECLARE_ENUM_VAL(   XCREAD_EA_WORD_ADDR_e,        29    )   /**< Word address within L1 Cache line*/
    DECLARE_ENUM_VAL(   XCREAD_EA_WORD_ADDR_n,         3    )
    DECLARE_ENUM_VAL(   XCREAD_EA_WORD_ADDR_i,        IBM_BIT_INDEX(32, XCREAD_EA_WORD_ADDR_e) )
    DECLARE_ENUM_VAL(   XCREAD_EA_WORD_ADDR_mask,     FIELD_MASK32( XCREAD_EA_WORD_ADDR_i, XCREAD_EA_WORD_ADDR_n ) )

    DECLARE_ENUM_VAL(   XCREAD_EA_L1I_INDEX_e,        26    )   /**< Instruction cache index */
    DECLARE_ENUM_VAL(   XCREAD_EA_L1I_INDEX_n,         8    )
    DECLARE_ENUM_VAL(   XCREAD_EA_L1I_INDEX_i,        IBM_BIT_INDEX(32, XCREAD_EA_L1I_INDEX_e) )
    DECLARE_ENUM_VAL(   XCREAD_EA_L1I_INDEX_mask,     FIELD_MASK32( XCREAD_EA_L1I_INDEX_i, XCREAD_EA_L1I_INDEX_n ) )

    DECLARE_ENUM_VAL(   XCREAD_EA_L1I_WAY_e,        18    )   /**< Instruction Cache way */
    DECLARE_ENUM_VAL(   XCREAD_EA_L1I_WAY_n,         2    )
    DECLARE_ENUM_VAL(   XCREAD_EA_L1I_WAY_i,        IBM_BIT_INDEX(32, XCREAD_EA_L1I_WAY_e) )
    DECLARE_ENUM_VAL(   XCREAD_EA_L1I_WAY_mask,     FIELD_MASK32( XCREAD_EA_L1I_WAY_i, XCREAD_EA_L1I_WAY_n ) )
END_ENUM(XCREAD_EA_FIELD)

BEGIN_ENUM(ICDBTRH_FIELD)
    DECLARE_ENUM_VAL(   XCDBTRH_TAG_EXT_ADDR_e,        31    )   /**< Tag external address */
    DECLARE_ENUM_VAL(   XCDBTRH_TAG_EXT_ADDR_n,        10    )
    DECLARE_ENUM_VAL(   XCDBTRH_TAG_EXT_ADDR_i,        IBM_BIT_INDEX(32, XCDBTRH_TAG_EXT_ADDR_e) )
    DECLARE_ENUM_VAL(   XCDBTRH_TAG_EXT_ADDR_mask,     FIELD_MASK32( XCDBTRH_TAG_EXT_ADDR_i, XCDBTRH_TAG_EXT_ADDR_n ) )

    DECLARE_ENUM_VAL(   XCDBTRH_TAG_PAR_e,        21    )   /**< TAG parity */
    DECLARE_ENUM_VAL(   XCDBTRH_TAG_PAR_n,         2    )
    DECLARE_ENUM_VAL(   XCDBTRH_TAG_PAR_i,        IBM_BIT_INDEX(32, XCDBTRH_TAG_PAR_e) )
    DECLARE_ENUM_VAL(   XCDBTRH_TAG_PAR_mask,     FIELD_MASK32( XCDBTRH_TAG_PAR_i, XCDBTRH_TAG_PAR_n ) )

    DECLARE_ENUM_VAL(   XCDBTRH_VALID_e,        19    )   /**< Valid bit */
    DECLARE_ENUM_VAL(   XCDBTRH_VALID_n,         1    )
    DECLARE_ENUM_VAL(   XCDBTRH_VALID_i,        IBM_BIT_INDEX(32, XCDBTRH_VALID_e) )
    DECLARE_ENUM_VAL(   XCDBTRH_VALID_mask,     FIELD_MASK32( XCDBTRH_VALID_i, XCDBTRH_VALID_n ) )

    DECLARE_ENUM_VAL(   XCDBTRH_TAG_ADDR_e,        18    )   /**< Tag address */
    DECLARE_ENUM_VAL(   XCDBTRH_TAG_ADDR_n,        19    )
    DECLARE_ENUM_VAL(   XCDBTRH_TAG_ADDR_i,        IBM_BIT_INDEX(32, XCDBTRH_TAG_ADDR_e) )
    DECLARE_ENUM_VAL(   XCDBTRH_TAG_ADDR_mask,     FIELD_MASK32( XCDBTRH_TAG_ADDR_i, XCDBTRH_TAG_ADDR_n ) )
END_ENUM(ICDBTRH_FIELD)

BEGIN_ENUM(ICDBTRL_FIELD)
    DECLARE_ENUM_VAL(   ICDBTRL_CONF_e,        28    )   /**< Way conflict bit */
    DECLARE_ENUM_VAL(   ICDBTRL_CONF_n,         1    )
    DECLARE_ENUM_VAL(   ICDBTRL_CONF_i,        IBM_BIT_INDEX(32, ICDBTRL_CONF_e) )
    DECLARE_ENUM_VAL(   ICDBTRL_CONF_mask,     FIELD_MASK32( ICDBTRL_CONF_i, ICDBTRL_CONF_n ) )

    DECLARE_ENUM_VAL(   ICDBTRL_LRUP_e,        18    )   /**< Way conflict bit */
    DECLARE_ENUM_VAL(   ICDBTRL_LRUP_n,         3    )
    DECLARE_ENUM_VAL(   ICDBTRL_LRUP_i,        IBM_BIT_INDEX(32, ICDBTRL_LRUP_e) )
    DECLARE_ENUM_VAL(   ICDBTRL_LRUP_mask,     FIELD_MASK32( ICDBTRL_LRUP_i, ICDBTRL_LRUP_n ) )

    DECLARE_ENUM_VAL(   ICDBTRL_LOCK_e,        13    )   /**< Way conflict bit */
    DECLARE_ENUM_VAL(   ICDBTRL_LOCK_n,         4    )
    DECLARE_ENUM_VAL(   ICDBTRL_LOCK_i,        IBM_BIT_INDEX(32, ICDBTRL_LOCK_e) )
    DECLARE_ENUM_VAL(   ICDBTRL_LOCK_mask,     FIELD_MASK32( ICDBTRL_LOCK_i, ICDBTRL_LOCK_n ) )

    DECLARE_ENUM_VAL(   ICDBTRL_LRU_e,        9    )   /**< Way conflict bit */
    DECLARE_ENUM_VAL(   ICDBTRL_LRU_n,        6    )
    DECLARE_ENUM_VAL(   ICDBTRL_LRU_i,        IBM_BIT_INDEX(32, ICDBTRL_LRU_e) )
    DECLARE_ENUM_VAL(   ICDBTRL_LRU_mask,     FIELD_MASK32( ICDBTRL_LRU_i, ICDBTRL_LRU_n ) )

    DECLARE_ENUM_VAL(   ICDBTRL_LRUV_e,        3    )   /**< Way conflict bit */
    DECLARE_ENUM_VAL(   ICDBTRL_LRUV_n,        4    )
    DECLARE_ENUM_VAL(   ICDBTRL_LRUV_i,        IBM_BIT_INDEX(32, ICDBTRL_LRUV_e) )
    DECLARE_ENUM_VAL(   ICDBTRL_LRUV_mask,     FIELD_MASK32( ICDBTRL_LRUV_i, ICDBTRL_LRUV_n ) )
END_ENUM(ICDBTRL_FIELD)

BEGIN_ENUM(DCDBTRL_FIELD)
    DECLARE_ENUM_VAL(   DCDBTRL_DATAP_e,        28    )   /**< Way conflict bit */
    DECLARE_ENUM_VAL(   DCDBTRL_DATAP_n,         1    )
    DECLARE_ENUM_VAL(   DCDBTRL_DATAP_i,        IBM_BIT_INDEX(32, DCDBTRL_DATAP_e) )
    DECLARE_ENUM_VAL(   DCDBTRL_DATAP_mask,     FIELD_MASK32( DCDBTRL_DATAP_i, DCDBTRL_DATAP_n ) )

    DECLARE_ENUM_VAL(   DCDBTRL_LRUP_e,        17    )   /**< Way conflict bit */
    DECLARE_ENUM_VAL(   DCDBTRL_LRUP_n,         2    )
    DECLARE_ENUM_VAL(   DCDBTRL_LRUP_i,        IBM_BIT_INDEX(32, DCDBTRL_LRUP_e) )
    DECLARE_ENUM_VAL(   DCDBTRL_LRUP_mask,     FIELD_MASK32( DCDBTRL_LRUP_i, DCDBTRL_LRUP_n ) )

    DECLARE_ENUM_VAL(   DCDBTRL_LOCK_e,        13    )   /**< Way conflict bit */
    DECLARE_ENUM_VAL(   DCDBTRL_LOCK_n,         4    )
    DECLARE_ENUM_VAL(   DCDBTRL_LOCK_i,        IBM_BIT_INDEX(32, DCDBTRL_LOCK_e) )
    DECLARE_ENUM_VAL(   DCDBTRL_LOCK_mask,     FIELD_MASK32( DCDBTRL_LOCK_i, DCDBTRL_LOCK_n ) )

    DECLARE_ENUM_VAL(   DCDBTRL_LRU_e,        9    )   /**< Way conflict bit */
    DECLARE_ENUM_VAL(   DCDBTRL_LRU_n,        6    )
    DECLARE_ENUM_VAL(   DCDBTRL_LRU_i,        IBM_BIT_INDEX(32, DCDBTRL_LRU_e) )
    DECLARE_ENUM_VAL(   DCDBTRL_LRU_mask,     FIELD_MASK32( DCDBTRL_LRU_i, DCDBTRL_LRU_n ) )

    DECLARE_ENUM_VAL(   DCDBTRL_LRUV_e,        3    )   /**< Way conflict bit */
    DECLARE_ENUM_VAL(   DCDBTRL_LRUV_n,        4    )
    DECLARE_ENUM_VAL(   DCDBTRL_LRUV_i,        IBM_BIT_INDEX(32, DCDBTRL_LRUV_e) )
    DECLARE_ENUM_VAL(   DCDBTRL_LRUV_mask,     FIELD_MASK32( DCDBTRL_LRUV_i, DCDBTRL_LRUV_n ) )
END_ENUM(DCDBTRL_FIELD)

#endif /* RUMBOOT_INCLUDE_PLATFORM_OI10_PLATFORM_ARCH_PPC_PPC_476FP_L1C_FIELDS_H_ */
