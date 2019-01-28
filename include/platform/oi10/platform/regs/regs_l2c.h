/*
 * regs_l2c.h
 *
 *  Created on: May 4, 2018
 *      Author: d.arsentiev
 */

#ifndef REGS_L2C_H
#define REGS_L2C_H


#include <platform/common_macros/common_macros.h>
#include <platform/arch/ppc/ibm_bit_ordering_macros.h>


/** SRAMMC2PLB4 registers */
BEGIN_ENUM( L2C_REG )
    /* L2 Cache Core Architected DCR */
    DECLARE_ENUM_VAL(   L2C_L2CDCRAI,   0x00    )   /**< D-Cache DCR Address Indirect Register */
    DECLARE_ENUM_VAL(   L2C_L2CDCRDI,   0x04    )   /**< D-Cache DCR Data Indirect Register */
    /* PMU Architected DCR */
    DECLARE_ENUM_VAL(   L2C_PMUDCRAI,   0x80    )   /**< PMU DCR Address Indirect */
    DECLARE_ENUM_VAL(   L2C_PMUDCRDI,   0x84    )   /**< PMU DCR Data Indirect */
END_ENUM( L2C_REG )

BEGIN_ENUM( L2C_L2CDCRAI_FIELD )
    DECLARE_ENUM_VAL(   L2C_L2CDCRAI_INDEX_e,       31    )   /**< Index of indirect DCR to access */
    DECLARE_ENUM_VAL(   L2C_L2CDCRAI_INDEX_n,       12    )
    DECLARE_ENUM_VAL(   L2C_L2CDCRAI_INDEX_i,       IBM_BIT_INDEX(32, L2C_L2CDCRAI_INDEX_e) )
    DECLARE_ENUM_VAL(   L2C_L2CDCRAI_INDEX_mask,    FIELD_MASK32(L2C_L2CDCRAI_INDEX_i, L2C_L2CDCRAI_INDEX_n) )
END_ENUM( L2C_L2CDCRAI_FIELD )

BEGIN_ENUM( L2C_L2CDCRDI_FIELD )
    DECLARE_ENUM_VAL(   L2C_L2CDCRDI_DATA_e,        31    )   /**< Data for indirect DCR access */
    DECLARE_ENUM_VAL(   L2C_L2CDCRDI_DATA_n,        32    )
    DECLARE_ENUM_VAL(   L2C_L2CDCRDI_DATA_i,        IBM_BIT_INDEX(32, L2C_L2CDCRDI_DATA_e) )
END_ENUM( L2C_L2CDCRDI_FIELD )

BEGIN_ENUM( L2C_PMUDCRAI_FIELD )
    DECLARE_ENUM_VAL(   L2C_PMUDCRAI_INDEX_e,       31    )   /**< Index of indirect DCR to access */
    DECLARE_ENUM_VAL(   L2C_PMUDCRAI_INDEX_n,       12    )
    DECLARE_ENUM_VAL(   L2C_PMUDCRAI_INDEX_i,       IBM_BIT_INDEX(32, L2C_PMUDCRAI_INDEX_e) )
    DECLARE_ENUM_VAL(   L2C_PMUDCRAI_INDEX_mask,    FIELD_MASK32(L2C_PMUDCRAI_INDEX_i, L2C_PMUDCRAI_INDEX_n) )
END_ENUM( L2C_PMUDCRAI_FIELD )

BEGIN_ENUM( L2C_PMUDCRDI_FIELD )
    DECLARE_ENUM_VAL(   L2C_PMUDCRDI_DATA_e,        31    )   /**< Data for indirect DCR access */
    DECLARE_ENUM_VAL(   L2C_PMUDCRDI_DATA_n,        32    )
    DECLARE_ENUM_VAL(   L2C_PMUDCRDI_DATA_i,        IBM_BIT_INDEX(32, L2C_PMUDCRDI_DATA_e) )
END_ENUM( L2C_PMUDCRDI_FIELD )


/** SRAMMC2PLB4 registers defaults */
BEGIN_ENUM( L2C_REG_DFLT )
    /* L2 Cache Core Architected DCR */
    DECLARE_ENUM_VAL(   L2C_L2CDCRAI_DFLT,   0x00000000 )
    DECLARE_ENUM_VAL(   L2C_L2CDCRDI_DFLT,   0x00000000 )
    /* PMU Architected DCR */
    DECLARE_ENUM_VAL(   L2C_PMUDCRAI_DFLT,   0x00000000 )
    DECLARE_ENUM_VAL(   L2C_PMUDCRDI_DFLT,   0x00000000 )
END_ENUM( L2C_REG_DFLT )

DECLARE_CONST(L2C_COUNT_WAYS, 4)


#endif /* REGS_L2C_H */
