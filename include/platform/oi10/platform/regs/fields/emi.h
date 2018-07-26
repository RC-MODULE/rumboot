/*
 * emi.h
 *
 *  Created on: Jul 26, 2018
 *      Author: s.chernousov
 */

#ifndef EMI_H_FIELD
#define EMI_H_FIELD

#include <platform/common_macros/common_macros.h>

BEGIN_ENUM( EMI_SSx_FIELD )
DECLARE_ENUM_VAL( EMI_SSx_BTYP_i,       0 )
DECLARE_ENUM_VAL( EMI_SSx_PTYP_i,       2 )
DECLARE_ENUM_VAL( EMI_SSx_SRDY_i,       3 )
DECLARE_ENUM_VAL( EMI_SSx_TWR_i,        4 )
DECLARE_ENUM_VAL( EMI_SSx_SST_i,        5 )
DECLARE_ENUM_VAL( EMI_SSx_T_SSOE_i,     6 )
DECLARE_ENUM_VAL( EMI_SSx_T_SOE_i,      7 )
DECLARE_ENUM_VAL( EMI_SSx_T_CYC_i,      8 )
DECLARE_ENUM_VAL( EMI_SSx_T_RDY_i,      12 )
DECLARE_ENUM_VAL( EMI_SSx_T_DEL_i,      22 )
DECLARE_ENUM_VAL( EMI_SSx_UWEB_i,       24 )
END_ENUM( EMI_SSx_FIELD )

BEGIN_ENUM( EMI_SDx_FIELD )
DECLARE_ENUM_VAL( EMI_SDx_CSP_i,        0 )
DECLARE_ENUM_VAL( EMI_SDx_SDS_i,        3 )
DECLARE_ENUM_VAL( EMI_SDx_CL_i,         6 )
DECLARE_ENUM_VAL( EMI_SDx_T_RDL_i,      8 )
DECLARE_ENUM_VAL( EMI_SDx_SI_i,         9 )
DECLARE_ENUM_VAL( EMI_SDx_T_RCD_i,      10 )
DECLARE_ENUM_VAL( EMI_SDx_T_RAS_i,      12 )
END_ENUM( EMI_SDx_FIELD )

#endif /* EMI_H_ */
