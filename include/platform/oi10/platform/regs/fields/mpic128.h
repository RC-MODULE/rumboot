/*
 * mpic128.h
 *
 *  Created on: Jul 9, 2018
 *      Author: r.galiulin
 */

#ifndef RUMBOOT_REGS_FIELDS_MPIC128_H
#define RUMBOOT_REGS_FIELDS_MPIC128_H


#include <platform/common_macros/common_macros.h>
#include <platform/arch/ppc/ibm_bit_ordering_macros.h>


BEGIN_ENUM( MPIC128_GCF0_FIELD )
DECLARE_ENUM_VAL( MPIC128_GCF0_BAS_e,       31 )                                        /** Base Address Relocation Bits IBM */
DECLARE_ENUM_VAL( MPIC128_GCF0_BAS_i,       IBM_BIT_INDEX(32, MPIC128_GCF0_BAS_e) )
DECLARE_ENUM_VAL( MPIC128_GCF0_BAS_n,       4 )
DECLARE_ENUM_VAL( MPIC128_GCF0_P_e,         2 )                                         /** 8259 Pass Through Disable */
DECLARE_ENUM_VAL( MPIC128_GCF0_P_i,         IBM_BIT_INDEX(32, MPIC128_GCF0_P_e) )
DECLARE_ENUM_VAL( MPIC128_GCF0_P_n,         1 )
DECLARE_ENUM_VAL( MPIC128_GCF0_R_e,         0 )                                         /** Reset MPIC */
DECLARE_ENUM_VAL( MPIC128_GCF0_R_i,         IBM_BIT_INDEX(32, MPIC128_GCF0_R_e) )
DECLARE_ENUM_VAL( MPIC128_GCF0_R_n,         1 )
END_ENUM( MPIC128_GCF0_FIELD )

BEGIN_ENUM( MPIC128_VITC_FIELD )
DECLARE_ENUM_VAL( MPIC128_VITC_CRB_e,       31 )                                        /** Lower Critical Interrupt Border. Valid entries 0 to 16. */
DECLARE_ENUM_VAL( MPIC128_VITC_CRB_i,       IBM_BIT_INDEX(32, MPIC128_VITC_CRB_e) )
DECLARE_ENUM_VAL( MPIC128_VITC_CRB_n,       5 )
DECLARE_ENUM_VAL( MPIC128_VITC_MCB_e,       23 )                                        /** Lower Machine Check Border. Valid entries 0 to 16, and MCB entry >= CRB entry. */
DECLARE_ENUM_VAL( MPIC128_VITC_MCB_i,       IBM_BIT_INDEX(32, MPIC128_VITC_MCB_e) )
DECLARE_ENUM_VAL( MPIC128_VITC_MCB_n,       5 )
END_ENUM( MPIC128_VITC_FIELD )

BEGIN_ENUM( MPIC128_VP_FIELD )
DECLARE_ENUM_VAL( MPIC128_VP_VEC_e,         31 )                                        /** Interrupt Vector */
DECLARE_ENUM_VAL( MPIC128_VP_VEC_i,         IBM_BIT_INDEX(32, MPIC128_VP_VEC_e) )
DECLARE_ENUM_VAL( MPIC128_VP_VEC_n,         8 )
DECLARE_ENUM_VAL( MPIC128_VP_PRI_e,         15 )                                        /** Interrupt Priority: 0b0000 (0) is the lowest priority, 0b1111 (15) is the highest priority */
DECLARE_ENUM_VAL( MPIC128_VP_PRI_i,         IBM_BIT_INDEX(32, MPIC128_VP_PRI_e) )
DECLARE_ENUM_VAL( MPIC128_VP_PRI_n,         4 )
DECLARE_ENUM_VAL( MPIC128_VP_S_e,           9 )                                        /** Sense. Reset to 0 */
DECLARE_ENUM_VAL( MPIC128_VP_S_i,           IBM_BIT_INDEX(32, MPIC128_VP_S_e) )
DECLARE_ENUM_VAL( MPIC128_VP_S_n,           1 )
DECLARE_ENUM_VAL( MPIC128_VP_POL_e,         8 )                                        /** Polarity. Reset to 1 */
DECLARE_ENUM_VAL( MPIC128_VP_POL_i,         IBM_BIT_INDEX(32, MPIC128_VP_POL_e) )
DECLARE_ENUM_VAL( MPIC128_VP_POL_n,         1 )
DECLARE_ENUM_VAL( MPIC128_VP_ACT_e,         1 )                                        /** Activity bit: 0 Okay to modify contents of this register, 1 The Vector, Priority and Destination information are in use and should not be changed */
DECLARE_ENUM_VAL( MPIC128_VP_ACT_i,         IBM_BIT_INDEX(32, MPIC128_VP_ACT_e) )
DECLARE_ENUM_VAL( MPIC128_VP_ACT_n,         1 )
DECLARE_ENUM_VAL( MPIC128_VP_MSK_e,         0 )                                        /** Mask bit: 0 Interrupt enabled, 1 Disable any further interrupts from this source. Reset to 1 */
DECLARE_ENUM_VAL( MPIC128_VP_MSK_i,         IBM_BIT_INDEX(32, MPIC128_VP_MSK_e) )
DECLARE_ENUM_VAL( MPIC128_VP_MSK_n,         1 )
END_ENUM( MPIC128_VP_FIELD )

BEGIN_ENUM( int_sense_t )
DECLARE_ENUM_VAL( int_sense_level,  1 )
DECLARE_ENUM_VAL( int_sense_edge,   0 )
END_ENUM( int_sense_t )

BEGIN_ENUM( int_pol_t )
DECLARE_ENUM_VAL( int_pol_pos,  1 )
DECLARE_ENUM_VAL( int_pol_high, 1 )
DECLARE_ENUM_VAL( int_pol_neg,  0 )
DECLARE_ENUM_VAL( int_pol_low,  0 )
END_ENUM( int_pol_t )

BEGIN_ENUM( mpic128_proc_num )
DECLARE_ENUM_VAL( Processor0,   0 )
DECLARE_ENUM_VAL( Processor1,   1 )
DECLARE_ENUM_VAL( Processor2,   2 )
DECLARE_ENUM_VAL( Processor3,   3 )
END_ENUM( mpic128_proc_num )

BEGIN_ENUM( mpic128_dst_t )
DECLARE_ENUM_VAL( MPIC128_DST_P0,   (1 << IBM_BIT_INDEX(32, 31)) )
DECLARE_ENUM_VAL( MPIC128_DST_P1,   (1 << IBM_BIT_INDEX(32, 30)) )
DECLARE_ENUM_VAL( MPIC128_DST_P2,   (1 << IBM_BIT_INDEX(32, 29)) )
DECLARE_ENUM_VAL( MPIC128_DST_P3,   (1 << IBM_BIT_INDEX(32, 28)) )
END_ENUM( mpic128_dst_t )

BEGIN_ENUM( mpic128_prior_t )
DECLARE_ENUM_VAL( MPIC128_PRIOR_0,      0 )
DECLARE_ENUM_VAL( MPIC128_PRIOR_1,      1 )
DECLARE_ENUM_VAL( MPIC128_PRIOR_2,      2 )
DECLARE_ENUM_VAL( MPIC128_PRIOR_3,      3 )
DECLARE_ENUM_VAL( MPIC128_PRIOR_4,      4 )
DECLARE_ENUM_VAL( MPIC128_PRIOR_5,      5 )
DECLARE_ENUM_VAL( MPIC128_PRIOR_6,      6 )
DECLARE_ENUM_VAL( MPIC128_PRIOR_7,      7 )
DECLARE_ENUM_VAL( MPIC128_PRIOR_8,      8 )
DECLARE_ENUM_VAL( MPIC128_PRIOR_9,      9 )
DECLARE_ENUM_VAL( MPIC128_PRIOR_10,     10 )
DECLARE_ENUM_VAL( MPIC128_PRIOR_11,     11 )
DECLARE_ENUM_VAL( MPIC128_PRIOR_12,     12 )
DECLARE_ENUM_VAL( MPIC128_PRIOR_13,     13 )
DECLARE_ENUM_VAL( MPIC128_PRIOR_14,     14 )
DECLARE_ENUM_VAL( MPIC128_PRIOR_15,     15 )
DECLARE_ENUM_VAL( MPIC128_PRIOR_MASK,   MPIC128_PRIOR_15 )
DECLARE_ENUM_VAL( MPIC128_PRIOR_UND,    16 )
END_ENUM( mpic128_prior_t )

BEGIN_ENUM( mpic128_vird_t )
DECLARE_ENUM_VAL( VRID_0_19,                    0b000 )
DECLARE_ENUM_VAL( VRID_20_39,                   0b001 )
DECLARE_ENUM_VAL( VRID_40_59,                   0b010 )
DECLARE_ENUM_VAL( VRID_60_79,                   0b011 )
DECLARE_ENUM_VAL( VRID_80_99,                   0b100 )
DECLARE_ENUM_VAL( VRID_100_119,                 0b101 )
DECLARE_ENUM_VAL( VRID_120_127_Tmr_IPI_0000,    0b110 )
END_ENUM( mpic128_vird_t )


#endif /* RUMBOOT_REGS_FIELDS_MPIC128_H */
