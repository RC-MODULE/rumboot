/**
 * ppc_476fp_debug_fields.h
 */

#ifndef PPC_476FP_DEBUG_FIELDS_H
#define PPC_476FP_DEBUG_FIELDS_H


#include <platform/common_macros/common_macros.h>
#include <platform/arch/ppc/ibm_bit_ordering_macros.h>


#define DEBUG_RESET_No      0b00
#define DEBUG_RESET_Core    0b01
#define DEBUG_RESET_Chip    0b10
#define DEBUG_RESET_System  0b11


#define DEBUG_DBCR0_EDM_e   32
#define DEBUG_DBCR0_EDM_n   1
#define DEBUG_DBCR0_EDM_i   IBM_BIT_INDEX( 64, DEBUG_DBCR0_EDM_e )

#define DEBUG_DBCR0_IDM_e   33
#define DEBUG_DBCR0_IDM_n   1
#define DEBUG_DBCR0_IDM_i   IBM_BIT_INDEX( 64, DEBUG_DBCR0_IDM_e )

#define DEBUG_DBCR0_RST_e   35
#define DEBUG_DBCR0_RST_n   2
#define DEBUG_DBCR0_RST_i   IBM_BIT_INDEX( 64, DEBUG_DBCR0_RST_e )

#define DEBUG_DBCR0_ICMP_e  36
#define DEBUG_DBCR0_ICMP_n  1
#define DEBUG_DBCR0_ICMP_i  IBM_BIT_INDEX( 64, DEBUG_DBCR0_ICMP_e )

#define DEBUG_DBCR0_BRT_e   37
#define DEBUG_DBCR0_BRT_n   1
#define DEBUG_DBCR0_BRT_i   IBM_BIT_INDEX( 64, DEBUG_DBCR0_BRT_e )

#define DEBUG_DBCR0_IRPT_e  38
#define DEBUG_DBCR0_IRPT_n  1
#define DEBUG_DBCR0_IRPT_i  IBM_BIT_INDEX( 64, DEBUG_DBCR0_IRPT_e )

#define DEBUG_DBCR0_TRAP_e  39
#define DEBUG_DBCR0_TRAP_n  1
#define DEBUG_DBCR0_TRAP_i  IBM_BIT_INDEX( 64, DEBUG_DBCR0_TRAP_e )

#define DEBUG_DBCR0_IACx_n  1
#define DEBUG_DBCR0_IAC1_e  40
#define DEBUG_DBCR0_IAC1_i  IBM_BIT_INDEX( 64, DEBUG_DBCR0_IAC1_e )
#define DEBUG_DBCR0_IAC2_e  41
#define DEBUG_DBCR0_IAC2_i  IBM_BIT_INDEX( 64, DEBUG_DBCR0_IAC2_e )
#define DEBUG_DBCR0_IAC3_e  42
#define DEBUG_DBCR0_IAC3_i  IBM_BIT_INDEX( 64, DEBUG_DBCR0_IAC3_e )
#define DEBUG_DBCR0_IAC4_e  43
#define DEBUG_DBCR0_IAC4_i  IBM_BIT_INDEX( 64, DEBUG_DBCR0_IAC4_e )

#define DEBUG_DBCR0_DAC1R_e 44
#define DEBUG_DBCR0_DAC1R_n 1
#define DEBUG_DBCR0_DAC1R_i IBM_BIT_INDEX( 64, DEBUG_DBCR0_DAC1R_e )

#define DEBUG_DBCR0_DAC1W_e 45
#define DEBUG_DBCR0_DAC1W_n 1
#define DEBUG_DBCR0_DAC1W_i IBM_BIT_INDEX( 64, DEBUG_DBCR0_DAC1W_e )

#define DEBUG_DBCR0_DAC2R_e 46
#define DEBUG_DBCR0_DAC2R_n 1
#define DEBUG_DBCR0_DAC2R_i IBM_BIT_INDEX( 64, DEBUG_DBCR0_DAC2R_e )

#define DEBUG_DBCR0_DAC2W_e 47
#define DEBUG_DBCR0_DAC2W_n 1
#define DEBUG_DBCR0_DAC2W_i IBM_BIT_INDEX( 64, DEBUG_DBCR0_DAC2W_e )

#define DEBUG_DBCR0_RET_e   48
#define DEBUG_DBCR0_RET_n   1
#define DEBUG_DBCR0_RET_i   IBM_BIT_INDEX( 64, DEBUG_DBCR0_RET_e )

#define DEBUG_DBCR0_FT_e    63
#define DEBUG_DBCR0_FT_n    1
#define DEBUG_DBCR0_FT_i    IBM_BIT_INDEX( 64, DEBUG_DBCR0_FT_e )


#define DEBUG_DBCR1_IAC1US_e    33
#define DEBUG_DBCR1_IAC1US_n    2
#define DEBUG_DBCR1_IAC1US_i    IBM_BIT_INDEX( 64, DEBUG_DBCR1_IAC1US_e )

#define DEBUG_DBCR1_IAC1ER_e    35
#define DEBUG_DBCR1_IAC1ER_n    2
#define DEBUG_DBCR1_IAC1ER_i    IBM_BIT_INDEX( 64, DEBUG_DBCR1_IAC1ER_e )

#define DEBUG_DBCR1_IAC2US_e    37
#define DEBUG_DBCR1_IAC2US_n    2
#define DEBUG_DBCR1_IAC2US_i    IBM_BIT_INDEX( 64, DEBUG_DBCR1_IAC2US_e )

#define DEBUG_DBCR1_IAC2ER_e    39
#define DEBUG_DBCR1_IAC2ER_n    2
#define DEBUG_DBCR1_IAC2ER_i    IBM_BIT_INDEX( 64, DEBUG_DBCR1_IAC2ER_e )

#define DEBUG_DBCR1_IAC12M_e    41
#define DEBUG_DBCR1_IAC12M_n    2
#define DEBUG_DBCR1_IAC12M_i    IBM_BIT_INDEX( 64, DEBUG_DBCR1_IAC12M_e )

#define DEBUG_DBCR1_IAC12AT_e   47
#define DEBUG_DBCR1_IAC12AT_n   1
#define DEBUG_DBCR1_IAC12AT_i   IBM_BIT_INDEX( 64, DEBUG_DBCR1_IAC12AT_e )

#define DEBUG_DBCR1_IAC3US_e    49
#define DEBUG_DBCR1_IAC3US_n    2
#define DEBUG_DBCR1_IAC3US_i    IBM_BIT_INDEX( 64, DEBUG_DBCR1_IAC3US_e )

#define DEBUG_DBCR1_IAC3ER_e    51
#define DEBUG_DBCR1_IAC3ER_n    2
#define DEBUG_DBCR1_IAC3ER_i    IBM_BIT_INDEX( 64, DEBUG_DBCR1_IAC3ER_e )

#define DEBUG_DBCR1_IAC4US_e    53
#define DEBUG_DBCR1_IAC4US_n    2
#define DEBUG_DBCR1_IAC4US_i    IBM_BIT_INDEX( 64, DEBUG_DBCR1_IAC4US_e )

#define DEBUG_DBCR1_IAC4ER_e    55
#define DEBUG_DBCR1_IAC4ER_n    2
#define DEBUG_DBCR1_IAC4ER_i    IBM_BIT_INDEX( 64, DEBUG_DBCR1_IAC4ER_e )

#define DEBUG_DBCR1_IAC34M_e    57
#define DEBUG_DBCR1_IAC34M_n    2
#define DEBUG_DBCR1_IAC34M_i    IBM_BIT_INDEX( 64, DEBUG_DBCR1_IAC34M_e )

#define DEBUG_DBCR1_IAC34AT_e   63
#define DEBUG_DBCR1_IAC34AT_n   1
#define DEBUG_DBCR1_IAC34AT_i   IBM_BIT_INDEX( 64, DEBUG_DBCR1_IAC34AT_e )

#define DEBUG_DBCR1_IACxUS_Both         0b00
#define DEBUG_DBCR1_IACxUS_Supervisor   0b10
#define DEBUG_DBCR1_IACxUS_User         0b11

#define DEBUG_DBCR1_IACxER_E_IS_Both    0b00
#define DEBUG_DBCR1_IACxER_E_IS_0       0b10
#define DEBUG_DBCR1_IACxER_E_IS_1       0b11

#define DEBUG_DBCR1_IACxxM_Exact_match  0b00
#define DEBUG_DBCR1_IACxxM_Range_incl   0b10
#define DEBUG_DBCR1_IACxxM_Range_excl   0b11


#define DEBUG_DBCR2_DAC1US_e    33
#define DEBUG_DBCR2_DAC1US_n    2
#define DEBUG_DBCR2_DAC1US_i    IBM_BIT_INDEX( 64, DEBUG_DBCR2_DAC1US_e )

#define DEBUG_DBCR2_DAC1ER_e    35
#define DEBUG_DBCR2_DAC1ER_n    2
#define DEBUG_DBCR2_DAC1ER_i    IBM_BIT_INDEX( 64, DEBUG_DBCR2_DAC1ER_e )

#define DEBUG_DBCR2_DAC2US_e    37
#define DEBUG_DBCR2_DAC2US_n    2
#define DEBUG_DBCR2_DAC2US_i    IBM_BIT_INDEX( 64, DEBUG_DBCR2_DAC2US_e )

#define DEBUG_DBCR2_DAC2ER_e    39
#define DEBUG_DBCR2_DAC2ER_n    2
#define DEBUG_DBCR2_DAC2ER_i    IBM_BIT_INDEX( 64, DEBUG_DBCR2_DAC2ER_e )

#define DEBUG_DBCR2_DAC12M_e    41
#define DEBUG_DBCR2_DAC12M_n    2
#define DEBUG_DBCR2_DAC12M_i    IBM_BIT_INDEX( 64, DEBUG_DBCR2_DAC12M_e )

#define DEBUG_DBCR2_DACxUS_Both         0b00
#define DEBUG_DBCR2_DACxUS_Supervisor   0b10
#define DEBUG_DBCR2_DACxUS_User         0b11

#define DEBUG_DBCR2_DACxER_E_DS_Both    0b00
#define DEBUG_DBCR2_DACxER_E_DS_0       0b10
#define DEBUG_DBCR2_DACxER_E_DS_1       0b11

#define DEBUG_DBCR2_DACxxM_Exact_match  0b00
#define DEBUG_DBCR2_DACxxM_Addr_mask    0b01
#define DEBUG_DBCR2_DACxxM_Range_incl   0b10
#define DEBUG_DBCR2_DACxxM_Range_excl   0b11

#define DEBUG_DBCR2_DVC1M_e     45
#define DEBUG_DBCR2_DVC1M_n     2
#define DEBUG_DBCR2_DVC1M_i     IBM_BIT_INDEX( 64, DEBUG_DBCR2_DVC1M_e )

#define DEBUG_DBCR2_DVC2M_e     47
#define DEBUG_DBCR2_DVC2M_n     2
#define DEBUG_DBCR2_DVC2M_i     IBM_BIT_INDEX( 64, DEBUG_DBCR2_DVC2M_e )

#define DEBUG_DBCR2_DVCxM_AND       0b01
#define DEBUG_DBCR2_DVCxM_OR        0b10
#define DEBUG_DBCR2_DVCxM_AND_OR    0b11

#define DEBUG_DBCR2_DVC1BE_e    55
#define DEBUG_DBCR2_DVC1BE_n    4
#define DEBUG_DBCR2_DVC1BE_i    IBM_BIT_INDEX( 64, DEBUG_DBCR2_DVC1BE_e )

#define DEBUG_DBCR2_DVC2BE_e    63
#define DEBUG_DBCR2_DVC2BE_n    4
#define DEBUG_DBCR2_DVC2BE_i    IBM_BIT_INDEX( 64, DEBUG_DBCR2_DVC2BE_e )


#define DEBUG_DBSR_IDE_e        32
#define DEBUG_DBSR_IDE_n        1
#define DEBUG_DBSR_IDE_i        IBM_BIT_INDEX( 64, DEBUG_DBSR_IDE_e )

#define DEBUG_DBSR_UDE_e        33
#define DEBUG_DBSR_UDE_n        1
#define DEBUG_DBSR_UDE_i        IBM_BIT_INDEX( 64, DEBUG_DBSR_UDE_e )

#define DEBUG_DBSR_MRR_e        35
#define DEBUG_DBSR_MRR_n        2
#define DEBUG_DBSR_MRR_i        IBM_BIT_INDEX( 64, DEBUG_DBSR_MRR_e )

#define DEBUG_DBSR_ICMP_e       36
#define DEBUG_DBSR_ICMP_n       1
#define DEBUG_DBSR_ICMP_i       IBM_BIT_INDEX( 64, DEBUG_DBSR_ICMP_e )

#define DEBUG_DBSR_BRT_e        37
#define DEBUG_DBSR_BRT_n        1
#define DEBUG_DBSR_BRT_i        IBM_BIT_INDEX( 64, DEBUG_DBSR_BRT_e )

#define DEBUG_DBSR_IRPT_e       38
#define DEBUG_DBSR_IRPT_n       1
#define DEBUG_DBSR_IRPT_i       IBM_BIT_INDEX( 64, DEBUG_DBSR_IRPT_e )

#define DEBUG_DBSR_TRAP_e       39
#define DEBUG_DBSR_TRAP_n       1
#define DEBUG_DBSR_TRAP_i       IBM_BIT_INDEX( 64, DEBUG_DBSR_TRAP_e )

#define DEBUG_DBSR_IACx_n       1
#define DEBUG_DBSR_IAC1_e       40
#define DEBUG_DBSR_IAC1_i       IBM_BIT_INDEX( 64, DEBUG_DBSR_IAC1_e )
#define DEBUG_DBSR_IAC2_e       41
#define DEBUG_DBSR_IAC2_i       IBM_BIT_INDEX( 64, DEBUG_DBSR_IAC2_e )
#define DEBUG_DBSR_IAC3_e       42
#define DEBUG_DBSR_IAC3_i       IBM_BIT_INDEX( 64, DEBUG_DBSR_IAC3_e )
#define DEBUG_DBSR_IAC4_e       43
#define DEBUG_DBSR_IAC4_i       IBM_BIT_INDEX( 64, DEBUG_DBSR_IAC4_e )

#define DEBUG_DBSR_DAC1R_e      44
#define DEBUG_DBSR_DAC1R_n      1
#define DEBUG_DBSR_DAC1R_i      IBM_BIT_INDEX( 64, DEBUG_DBSR_DAC1R_e )

#define DEBUG_DBSR_DAC1W_e      45
#define DEBUG_DBSR_DAC1W_n      1
#define DEBUG_DBSR_DAC1W_i      IBM_BIT_INDEX( 64, DEBUG_DBSR_DAC1W_e )

#define DEBUG_DBSR_DAC2R_e      46
#define DEBUG_DBSR_DAC2R_n      1
#define DEBUG_DBSR_DAC2R_i      IBM_BIT_INDEX( 64, DEBUG_DBSR_DAC2R_e )

#define DEBUG_DBSR_DAC2W_e      47
#define DEBUG_DBSR_DAC2W_n      1
#define DEBUG_DBSR_DAC2W_i      IBM_BIT_INDEX( 64, DEBUG_DBSR_DAC2W_e )

#define DEBUG_DBSR_RET_e        48
#define DEBUG_DBSR_RET_n        1
#define DEBUG_DBSR_RET_i        IBM_BIT_INDEX( 64, DEBUG_DBSR_RET_e )

#define DEBUG_DBSR_IAC12ATS_e   62
#define DEBUG_DBSR_IAC12ATS_n   1
#define DEBUG_DBSR_IAC12ATS_i   IBM_BIT_INDEX( 64, DEBUG_DBSR_IAC12ATS_e )

#define DEBUG_DBSR_IAC34ATS_e   63
#define DEBUG_DBSR_IAC34ATS_n   1
#define DEBUG_DBSR_IAC34ATS_i   IBM_BIT_INDEX( 64, DEBUG_DBSR_IAC34ATS_e )


#define DEBUG_IACx_ADDR_e   61
#define DEBUG_IACx_ADDR_n   30
#define DEBUG_IACx_ADDR_i   IBM_BIT_INDEX( 64, DEBUG_DBSR_IACx_ADDR_e )


#endif // PPC_476FP_DEBUG_FIELDS_H
