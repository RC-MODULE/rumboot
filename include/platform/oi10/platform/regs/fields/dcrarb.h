/*
 * dcrarb.h
 *
 *  Created on: Feb 22, 2019
 *      Author: m.smolina
 */

#ifndef REGS_FIELDS_DCRARB_H__
#define REGS_FIELDS_DCRARB_H__

#include <rumboot/macros.h>
#include <arch/ibm_bit_ordering_macros.h>

BEGIN_ENUM( DCRARB_DACR_FIELD )
    DECLARE_ENUM_VAL( DCRARB_DACR_DPM_e,                                              0 )
    DECLARE_ENUM_VAL( DCRARB_DACR_DPM_n,                                              1 )
    DECLARE_ENUM_VAL( DCRARB_DACR_DPM_i,     IBM_BIT_INDEX( 32, DCRARB_DACR_DPM_e     ) )

    DECLARE_ENUM_VAL( DCRARB_DACR_LP_e,                                               1 )
    DECLARE_ENUM_VAL( DCRARB_DACR_LP_n,                                               1 )
    DECLARE_ENUM_VAL( DCRARB_DACR_LP_i,      IBM_BIT_INDEX( 32, DCRARB_DACR_LP_e      ) )

    DECLARE_ENUM_VAL( DCRARB_DACR_DPO_e,                                              7 )
    DECLARE_ENUM_VAL( DCRARB_DACR_DPO_n,                                              2 )
    DECLARE_ENUM_VAL( DCRARB_DACR_DPO_i,     IBM_BIT_INDEX( 32, DCRARB_DACR_DPO_e     ) )

    DECLARE_ENUM_VAL( DCRARB_DACR_TOCNT_e,                                           27 )
    DECLARE_ENUM_VAL( DCRARB_DACR_TOCNT_n,                                            8 )
    DECLARE_ENUM_VAL( DCRARB_DACR_TOCNT_i,   IBM_BIT_INDEX( 32, DCRARB_DACR_TOCNT_e   ) )

END_ENUM( DCRARB_DACR_FIELD )

BEGIN_ENUM( DCRARB_DAESR_FIELD )
    DECLARE_ENUM_VAL( DCRARB_DAESR_EV_e,                                              0 )
    DECLARE_ENUM_VAL( DCRARB_DAESR_EV_n,                                              1 )
    DECLARE_ENUM_VAL( DCRARB_DAESR_EV_i,     IBM_BIT_INDEX( 32, DCRARB_DAESR_EV_e     ) )

    DECLARE_ENUM_VAL( DCRARB_DAESR_TE_e,                                              1 )
    DECLARE_ENUM_VAL( DCRARB_DAESR_TE_n,                                              1 )
    DECLARE_ENUM_VAL( DCRARB_DAESR_TE_i,     IBM_BIT_INDEX( 32, DCRARB_DAESR_TE_e     ) )

    DECLARE_ENUM_VAL( DCRARB_DAESR_LPE_e,                                             2 )
    DECLARE_ENUM_VAL( DCRARB_DAESR_LPE_n,                                             1 )
    DECLARE_ENUM_VAL( DCRARB_DAESR_LPE_i,    IBM_BIT_INDEX( 32, DCRARB_DAESR_LPE_e    ) )

    DECLARE_ENUM_VAL( DCRARB_DAESR_MID_e,                                             7 )
    DECLARE_ENUM_VAL( DCRARB_DAESR_MID_n,                                             4 )
    DECLARE_ENUM_VAL( DCRARB_DAESR_MID_i,    IBM_BIT_INDEX( 32, DCRARB_DAESR_MID_e    ) )

    DECLARE_ENUM_VAL( DCRARB_DAESR_RW_e,                                              8 )
    DECLARE_ENUM_VAL( DCRARB_DAESR_RW_n,                                              1 )
    DECLARE_ENUM_VAL( DCRARB_DAESR_RW_i,     IBM_BIT_INDEX( 32, DCRARB_DAESR_RW_e     ) )

    DECLARE_ENUM_VAL( DCRARB_DAESR_PT_e,                                              9 )
    DECLARE_ENUM_VAL( DCRARB_DAESR_PT_n,                                              1 )
    DECLARE_ENUM_VAL( DCRARB_DAESR_PT_i,     IBM_BIT_INDEX( 32, DCRARB_DAESR_PT_e     ) )

    DECLARE_ENUM_VAL( DCRARB_DAESR_M0_e,                                             28 )
    DECLARE_ENUM_VAL( DCRARB_DAESR_M0_n,                                              1 )
    DECLARE_ENUM_VAL( DCRARB_DAESR_M0_i,     IBM_BIT_INDEX( 32, DCRARB_DAESR_M0_e     ) )

    DECLARE_ENUM_VAL( DCRARB_DAESR_M1_e,                                             29 )
    DECLARE_ENUM_VAL( DCRARB_DAESR_M1_n,                                              1 )
    DECLARE_ENUM_VAL( DCRARB_DAESR_M1_i,     IBM_BIT_INDEX( 32, DCRARB_DAESR_M1_e     ) )

    DECLARE_ENUM_VAL( DCRARB_DAESR_M2_e,                                             30 )
    DECLARE_ENUM_VAL( DCRARB_DAESR_M2_n,                                              1 )
    DECLARE_ENUM_VAL( DCRARB_DAESR_M2_i,     IBM_BIT_INDEX( 32, DCRARB_DAESR_M2_e     ) )

    DECLARE_ENUM_VAL( DCRARB_DAESR_M3_e,                                             31 )
    DECLARE_ENUM_VAL( DCRARB_DAESR_M3_n,                                              1 )
    DECLARE_ENUM_VAL( DCRARB_DAESR_M3_i,     IBM_BIT_INDEX( 32, DCRARB_DAESR_M3_e     ) )

END_ENUM( DCRARB_DAESR_FIELD )


#endif // REGS_FIELDS_DCRARB_H__

