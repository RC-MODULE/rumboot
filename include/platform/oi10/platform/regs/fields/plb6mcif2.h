#ifndef __DDR_PLB6MCIF2_REG_FIELD_H__
#define __DDR_PLB6MCIF2_REG_FIELD_H__

#include <platform/common_macros/common_macros.h>
#include <platform/arch/ppc/ibm_bit_ordering_macros.h>

BEGIN_ENUM( PLB6MCIF2_BESR_FIELD )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_MxRW_e,        0 )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_MxRW_i,        IBM_BIT_INDEX(32, PLB6MCIF2_BESR_MxRW_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_MxRW_n,        1 )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_MxID_e,        5 )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_MxID_i,        IBM_BIT_INDEX(32, PLB6MCIF2_BESR_MxID_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_MxID_n,        5 )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_MTAG_e,        10 )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_MTAG_i,        IBM_BIT_INDEX(32, PLB6MCIF2_BESR_MTAG_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_MTAG_n,        5 )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_BufferID_e,    14 )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_BufferID_i,    IBM_BIT_INDEX(32, PLB6MCIF2_BESR_BufferID_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_BufferID_n,    4 )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_RS_Err_0_e,    15 )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_RS_Err_0_i,    IBM_BIT_INDEX(32, PLB6MCIF2_BESR_RS_Err_0_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_RS_Err_0_n,    1 )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_MxET1_1_5_e,   20 )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_MxET1_1_5_i,   IBM_BIT_INDEX(32, PLB6MCIF2_BESR_MxET1_1_5_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_MxET1_1_5_n,   5 )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_RS_Err_1_2_e,  22 )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_RS_Err_1_2_i,  IBM_BIT_INDEX(32, PLB6MCIF2_BESR_RS_Err_1_2_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_RS_Err_1_2_n,  2 )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_MxET2_e,       25 )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_MxET2_i,       IBM_BIT_INDEX(32, PLB6MCIF2_BESR_MxET2_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_MxET2_n,       3 )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_MxET1_0_e,     26 )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_MxET1_0_i,     IBM_BIT_INDEX(32, PLB6MCIF2_BESR_MxET1_0_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_MxET1_0_n,     1 )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_MxET3_e,       31 )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_MxET3_i,       IBM_BIT_INDEX(32, PLB6MCIF2_BESR_MxET3_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_BESR_MxET3_n,       4 )
END_ENUM( PLB6MCIF2_BESR_FIELD )

DECLARE_CONST( PLB6MCIF2_BESR_MxET1_n,  PLB6MCIF2_BESR_MxET1_0_n+PLB6MCIF2_BESR_MxET1_1_5_n )
DECLARE_CONST( PLB6MCIF2_BESR_RS_Err_n, PLB6MCIF2_BESR_RS_Err_0_n+PLB6MCIF2_BESR_RS_Err_1_2_n )

BEGIN_ENUM( PLB6MCIF2_ERROR_TYPE1 )
DECLARE_ENUM_VAL( PLB6MCIF2_ET1_No_Error,                               0 )
DECLARE_ENUM_VAL( PLB6MCIF2_ET1_PLB6_Bus_Master_Access_Violation,       (1 << 5) )
DECLARE_ENUM_VAL( PLB6MCIF2_ET1_Address_Decode_Miss,                    (1 << 4) )
DECLARE_ENUM_VAL( PLB6MCIF2_ET1_Address_or_Byte_Enable_Parity_Error,    (1 << 3) )
DECLARE_ENUM_VAL( PLB6MCIF2_ET1_PLB_Write_Data_Error,                   (1 << 2) )
DECLARE_ENUM_VAL( PLB6MCIF2_ET1_MCIF_Read_Data_Error,                   (1 << 1) )
DECLARE_ENUM_VAL( PLB6MCIF2_ET1_MCIF_Read_Parity_Error,                 (1 << 0) )
END_ENUM( PLB6MCIF2_ERROR_TYPE1 )

BEGIN_ENUM( PLB6MCIF2_ERROR_TYPE2 )
DECLARE_ENUM_VAL( PLB6MCIF2_ET2_No_Error,                   0 )
DECLARE_ENUM_VAL( PLB6MCIF2_ET2_PLB_Write_Parity_Error,     (1 << 2) )
DECLARE_ENUM_VAL( PLB6MCIF2_ET2_MCIF_Write_Parity_Error,    (1 << 1) )
DECLARE_ENUM_VAL( PLB6MCIF2_ET2_MCIF_Write_RMW_Error,       (1 << 0) )
END_ENUM( PLB6MCIF2_ERROR_TYPE2 )

BEGIN_ENUM( PLB6MCIF2_ERROR_TYPE3 )
DECLARE_ENUM_VAL( PLB6MCIF2_ET3_No_flags,       0 )
DECLARE_ENUM_VAL( PLB6MCIF2_ET3_Decode_flag,    (1 << 3) )
DECLARE_ENUM_VAL( PLB6MCIF2_ET3_MCIFII_flag,    (1 << 2) )
DECLARE_ENUM_VAL( PLB6MCIF2_ET3_Read_flag,      (1 << 1) )
DECLARE_ENUM_VAL( PLB6MCIF2_ET3_Write_flag,     (1 << 0) )
END_ENUM( PLB6MCIF2_ERROR_TYPE3 )


BEGIN_ENUM( PLB6MCIF2_INTR_EN_FIELD )
DECLARE_ENUM_VAL( PLB6MCIF2_INTR_EN_ERR_EN_e,   15 )
DECLARE_ENUM_VAL( PLB6MCIF2_INTR_EN_ERR_EN_i,   IBM_BIT_INDEX(32, PLB6MCIF2_INTR_EN_ERR_EN_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_INTR_EN_ERR_EN_n,   16 )
DECLARE_ENUM_VAL( PLB6MCIF2_INTR_EN_INTR_EN_e,  31 )
DECLARE_ENUM_VAL( PLB6MCIF2_INTR_EN_INTR_EN_i,  IBM_BIT_INDEX(32, PLB6MCIF2_INTR_EN_INTR_EN_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_INTR_EN_INTR_EN_n,  16 )
END_ENUM( PLB6MCIF2_INTR_EN_FIELD )


BEGIN_ENUM( PLB6MCIF2_PLBASYNC_FIELD )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBASYNC_PLBgtMCIF_e,   0 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBASYNC_PLBgtMCIF_i,   IBM_BIT_INDEX(32, PLB6MCIF2_PLBASYNC_PLBgtMCIF_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBASYNC_PLBgtMCIF_n,   1 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBASYNC_PLBeqMCIF_e,   1 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBASYNC_PLBeqMCIF_i,   IBM_BIT_INDEX(32, PLB6MCIF2_PLBASYNC_PLBeqMCIF_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBASYNC_PLBeqMCIF_n,   1 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBASYNC_PLB6NUM_e,     7 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBASYNC_PLB6NUM_i,     IBM_BIT_INDEX(32, PLB6MCIF2_PLBASYNC_PLB6NUM_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBASYNC_PLB6NUM_n,     4 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBASYNC_MCIFNUM_e,     11 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBASYNC_MCIFNUM_i,     IBM_BIT_INDEX(32, PLB6MCIF2_PLBASYNC_MCIFNUM_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBASYNC_MCIFNUM_n,     4 )
//DECLARE_ENUM_VAL( PLB6MCIF2_PLBASYNC_WRDONE_TUNE_e, 12 )
//DECLARE_ENUM_VAL( PLB6MCIF2_PLBASYNC_WRDONE_TUNE_i, IBM_BIT_INDEX(32, PLB6MCIF2_PLBASYNC_WRDONE_TUNE_e) )
//DECLARE_ENUM_VAL( PLB6MCIF2_PLBASYNC_WRDONE_TUNE_n, 4 )
//DECLARE_ENUM_VAL( PLB6MCIF2_PLBASYNC_WRRDY_TUNE_e,  16 )
//DECLARE_ENUM_VAL( PLB6MCIF2_PLBASYNC_WRRDY_TUNE_i,  IBM_BIT_INDEX(32, PLB6MCIF2_PLBASYNC_WRRDY_TUNE_e) )
//DECLARE_ENUM_VAL( PLB6MCIF2_PLBASYNC_WRRDY_TUNE_n,  4 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBASYNC_RDRDY_TUNE_e,  23 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBASYNC_RDRDY_TUNE_i,  IBM_BIT_INDEX(32, PLB6MCIF2_PLBASYNC_RDRDY_TUNE_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBASYNC_RDRDY_TUNE_n,  4 )
END_ENUM( PLB6MCIF2_PLBASYNC_FIELD )


BEGIN_ENUM( PLB6MCIF2_PLBORD_FIELD )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBORD_RaW_e,           0 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBORD_RaW_i,           IBM_BIT_INDEX(32, PLB6MCIF2_PLBORD_RaW_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBORD_RaW_n,           1 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBORD_Threshold_1_3_e, 3 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBORD_Threshold_1_3_i, IBM_BIT_INDEX(32, PLB6MCIF2_PLBORD_Threshold_1_3_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBORD_Threshold_1_3_n, 3 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBORD_Threshold_0_e,   7 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBORD_Threshold_0_i,   IBM_BIT_INDEX(32, PLB6MCIF2_PLBORD_Threshold_0_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBORD_Threshold_0_n,   1 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBORD_IT_OOO_e,        16 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBORD_IT_OOO_i,        IBM_BIT_INDEX(32, PLB6MCIF2_PLBORD_IT_OOO_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBORD_IT_OOO_n,        1 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBORD_TOM_e,           18 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBORD_TOM_i,           IBM_BIT_INDEX(32, PLB6MCIF2_PLBORD_TOM_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBORD_TOM_n,           2 )
END_ENUM( PLB6MCIF2_PLBORD_FIELD )

DECLARE_CONST( PLB6MCIF2_PLBORD_Threshold_n,    PLB6MCIF2_PLBORD_Threshold_0_n+PLB6MCIF2_PLBORD_Threshold_1_3_n )

BEGIN_ENUM( PLB6MCIF2_PLB_RAW_THRESHOLD )
DECLARE_ENUM_VAL( PLB6MCIF2_RAW_32DEEP,     (1 << 3) )
DECLARE_ENUM_VAL( PLB6MCIF2_RAW_16DEEP,     (1 << 2) )
DECLARE_ENUM_VAL( PLB6MCIF2_RAW_8DEEP,      (1 << 1) )
END_ENUM( PLB6MCIF2_PLB_RAW_THRESHOLD )

BEGIN_ENUM( PLB6MCIF2_MCIF2_TOM )
DECLARE_ENUM_VAL( PLB6MCIF2_MCIF2_STRICT_IN_ORDER,          0x0 )
DECLARE_ENUM_VAL( PLB6MCIF2_MCIF2_PARTIAL_OUT_OF_ORDER,     0x2 )
DECLARE_ENUM_VAL( PLB6MCIF2_MCIF2_Full_OUT_OF_ORDER,        0x3 )
END_ENUM( PLB6MCIF2_MCIF2_TOM )


BEGIN_ENUM( PLB6MCIF2_PLBCFG_FIELD )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBCFG_Non_Coherent_e,      0 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBCFG_Non_Coherent_i,      IBM_BIT_INDEX(32, PLB6MCIF2_PLBCFG_Non_Coherent_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBCFG_Non_Coherent_n,      1 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBCFG_Tsnoop_e,            4 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBCFG_Tsnoop_i,            IBM_BIT_INDEX(32, PLB6MCIF2_PLBCFG_Tsnoop_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBCFG_Tsnoop_n,            4 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBCFG_PmstrID_chk_EN_e,    5 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBCFG_PmstrID_chk_EN_i,    IBM_BIT_INDEX(32, PLB6MCIF2_PLBCFG_PmstrID_chk_EN_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBCFG_PmstrID_chk_EN_n,    1 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBCFG_Parity_Enables_e,    27 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBCFG_Parity_Enables_i,    IBM_BIT_INDEX(32, PLB6MCIF2_PLBCFG_Parity_Enables_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBCFG_Parity_Enables_n,    4 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBCFG_CG_ENABLE_e,         30 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBCFG_CG_ENABLE_i,         IBM_BIT_INDEX(32, PLB6MCIF2_PLBCFG_CG_ENABLE_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBCFG_CG_ENABLE_n,         3 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBCFG_Br_Enable_e,         31 )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBCFG_Br_Enable_i,         IBM_BIT_INDEX(32, PLB6MCIF2_PLBCFG_Br_Enable_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_PLBCFG_Br_Enable_n,         1 )
END_ENUM( PLB6MCIF2_PLBCFG_FIELD )

BEGIN_ENUM( PLB6MCIF2_TSNOOP )
#define BOOST_PP_LOCAL_MACRO( i )\
DECLARE_ENUM_VAL( BOOST_PP_CAT( PLB6MCIF2_PLBCFG_TSNOOP_,i ),   i )
#define BOOST_PP_LOCAL_LIMITS (4, 8)
#include BOOST_PP_LOCAL_ITERATE()
END_ENUM( PLB6MCIF2_TSNOOP )

BEGIN_ENUM( PLB6MCIF2_PARITY )
DECLARE_ENUM_VAL( PLB6MCIF2_ADR_RS_PAR_EN,  (1 << 3) )
DECLARE_ENUM_VAL( PLB6MCIF2_BE_PAR_EN,      (1 << 2) )
DECLARE_ENUM_VAL( PLB6MCIF2_CMD_PAR_EN,     (1 << 1) )
DECLARE_ENUM_VAL( PLB6MCIF2_MCIF_PAR_EN,    (1 << 0) )
END_ENUM( PLB6MCIF2_PARITY )

BEGIN_ENUM( PLB6MCIF2_CLK_GATING )
DECLARE_ENUM_VAL( PLB6MCIF2_CG_CLOCK_DOMAIN3,   (1 < 2) )
DECLARE_ENUM_VAL( PLB6MCIF2_CG_CLOCK_DOMAIN4,   (1 < 1) )
DECLARE_ENUM_VAL( PLB6MCIF2_CG_CLOCK_DOMAIN5,   (1 < 0) )
END_ENUM( PLB6MCIF2_CLK_GATING )


BEGIN_ENUM( PLB6MCIF2_MAXMEM_FIELD )
DECLARE_ENUM_VAL( PLB6MCIF2_MAXMEM_e,   1 )
DECLARE_ENUM_VAL( PLB6MCIF2_MAXMEM_i,   IBM_BIT_INDEX(32, PLB6MCIF2_MAXMEM_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_MAXMEM_n,   2 )
END_ENUM( PLB6MCIF2_MAXMEM_FIELD )

BEGIN_ENUM( PLB6MCIF2_PLB_MAXMEM )
DECLARE_ENUM_VAL( PLB6MCIF2_MAXMEM_64GB,    0x3 )
DECLARE_ENUM_VAL( PLB6MCIF2_MAXMEM_32GB,    0x2 )
DECLARE_ENUM_VAL( PLB6MCIF2_MAXMEM_16GB,    0x1 )
DECLARE_ENUM_VAL( PLB6MCIF2_MAXMEM_8GB,     0x0 )
END_ENUM( PLB6MCIF2_PLB_MAXMEM )


BEGIN_ENUM( PLB6MCIF2_PUABA_FIELD )
DECLARE_ENUM_VAL( PLB6MCIF2_PUABA_e,    31 )
DECLARE_ENUM_VAL( PLB6MCIF2_PUABA_i,    IBM_BIT_INDEX(32, PLB6MCIF2_PUABA_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_PUABA_n,    32 )
END_ENUM( PLB6MCIF2_PUABA_FIELD )


BEGIN_ENUM( PLB6MCIF2_MRxCF_FIELD )
DECLARE_ENUM_VAL( PLB6MCIF2_MRxCF_M_BA_e,   12 )
DECLARE_ENUM_VAL( PLB6MCIF2_MRxCF_M_BA_i,   IBM_BIT_INDEX(32, PLB6MCIF2_MRxCF_M_BA_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_MRxCF_M_BA_n,   13 )
DECLARE_ENUM_VAL( PLB6MCIF2_MRxCF_M_SZ_e,   19 )
DECLARE_ENUM_VAL( PLB6MCIF2_MRxCF_M_SZ_i,   IBM_BIT_INDEX(32, PLB6MCIF2_MRxCF_M_SZ_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_MRxCF_M_SZ_n,   4 )
DECLARE_ENUM_VAL( PLB6MCIF2_MRxCF_M_RE_e,   31 )
DECLARE_ENUM_VAL( PLB6MCIF2_MRxCF_M_RE_i,   IBM_BIT_INDEX(32, PLB6MCIF2_MRxCF_M_RE_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_MRxCF_M_RE_n,   1 )
END_ENUM( PLB6MCIF2_MRxCF_FIELD )

BEGIN_ENUM( PLB6MCIF2_MEMRANK_SZ )
DECLARE_ENUM_VAL( PLB6MCIF2_MEMRANK_8MB,    0x0 )
DECLARE_ENUM_VAL( PLB6MCIF2_MEMRANK_16MB,   0x1 )
DECLARE_ENUM_VAL( PLB6MCIF2_MEMRANK_32MB,   0x2 )
DECLARE_ENUM_VAL( PLB6MCIF2_MEMRANK_64MB,   0x3 )
DECLARE_ENUM_VAL( PLB6MCIF2_MEMRANK_128MB,  0x4 )
DECLARE_ENUM_VAL( PLB6MCIF2_MEMRANK_256MB,  0x5 )
DECLARE_ENUM_VAL( PLB6MCIF2_MEMRANK_512MB,  0x6 )
DECLARE_ENUM_VAL( PLB6MCIF2_MEMRANK_1GB,    0x7 )
DECLARE_ENUM_VAL( PLB6MCIF2_MEMRANK_2GB,    0x8 )
DECLARE_ENUM_VAL( PLB6MCIF2_MEMRANK_4GB,    0x9 )
DECLARE_ENUM_VAL( PLB6MCIF2_MEMRANK_8GB,    0xa )
DECLARE_ENUM_VAL( PLB6MCIF2_MEMRANK_16GB,   0xb )
DECLARE_ENUM_VAL( PLB6MCIF2_MEMRANK_32GB,   0xc )
DECLARE_ENUM_VAL( PLB6MCIF2_MEMRANK_64GB,   0xd )
END_ENUM( PLB6MCIF2_MEMRANK_SZ )


BEGIN_ENUM( PLB6MCIF2_SRxCF_FIELD )
DECLARE_ENUM_VAL( PLB6MCIF2_SRxCF_S_BA_e,   12 )
DECLARE_ENUM_VAL( PLB6MCIF2_SRxCF_S_BA_i,   IBM_BIT_INDEX(32, PLB6MCIF2_SRxCF_S_BA_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_SRxCF_S_BA_n,   13 )
DECLARE_ENUM_VAL( PLB6MCIF2_SRxCF_S_SZ_e,   19 )
DECLARE_ENUM_VAL( PLB6MCIF2_SRxCF_S_SZ_i,   IBM_BIT_INDEX(32, PLB6MCIF2_SRxCF_S_SZ_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_SRxCF_S_SZ_n,   4 )
DECLARE_ENUM_VAL( PLB6MCIF2_SRxCF_S_RE_e,   31 )
DECLARE_ENUM_VAL( PLB6MCIF2_SRxCF_S_RE_i,   IBM_BIT_INDEX(32, PLB6MCIF2_SRxCF_S_RE_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_SRxCF_S_RE_n,   1 )
END_ENUM( PLB6MCIF2_SRxCF_FIELD )


#define PLB6MCIF2_MASTER_N                  16
DECLARE_CONST( PLB6MCIF2_P6BMTAGx_MstxID_n, 5 )

BEGIN_ENUM( PLB6MCIF2_P6BMTAG1_FIELD )
#define BOOST_PP_LOCAL_MACRO( i )\
DECLARE_ENUM_VAL( BOOST_PP_CAT(BOOST_PP_CAT( PLB6MCIF2_P6BMTAG1_Mst,i),ID_e ),  4 + (5 * i) )
#define BOOST_PP_LOCAL_LIMITS (0, 5)
#include BOOST_PP_LOCAL_ITERATE()
#define BOOST_PP_LOCAL_MACRO( i )\
DECLARE_ENUM_VAL( BOOST_PP_CAT(BOOST_PP_CAT( PLB6MCIF2_P6BMTAG1_Mst,i),ID_i ),  IBM_BIT_INDEX(32, BOOST_PP_CAT(BOOST_PP_CAT( PLB6MCIF2_P6BMTAG1_Mst,i),ID_e )) )
#define BOOST_PP_LOCAL_LIMITS (0, 5)
#include BOOST_PP_LOCAL_ITERATE()
END_ENUM( PLB6MCIF2_P6BMTAG1_FIELD )

BEGIN_ENUM( PLB6MCIF2_P6BMTAG2_FIELD )
#define BOOST_PP_LOCAL_MACRO( i )\
DECLARE_ENUM_VAL( BOOST_PP_CAT(BOOST_PP_CAT( PLB6MCIF2_P6BMTAG2_Mst,i),ID_e ),  4 + (5 * (i-6)) )
#define BOOST_PP_LOCAL_LIMITS (6, 11)
#include BOOST_PP_LOCAL_ITERATE()
#define BOOST_PP_LOCAL_MACRO( i )\
DECLARE_ENUM_VAL( BOOST_PP_CAT(BOOST_PP_CAT( PLB6MCIF2_P6BMTAG2_Mst,i),ID_i ),  IBM_BIT_INDEX(32, BOOST_PP_CAT(BOOST_PP_CAT( PLB6MCIF2_P6BMTAG2_Mst,i),ID_e )) )
#define BOOST_PP_LOCAL_LIMITS (6, 11)
#include BOOST_PP_LOCAL_ITERATE()
END_ENUM( PLB6MCIF2_P6BMTAG2_FIELD )

BEGIN_ENUM( PLB6MCIF2_P6BMTAG3_FIELD )
#define BOOST_PP_LOCAL_MACRO( i )\
DECLARE_ENUM_VAL( BOOST_PP_CAT(BOOST_PP_CAT( PLB6MCIF2_P6BMTAG3_Mst,i),ID_e ),  4 + (5 * (i-12)) )
#define BOOST_PP_LOCAL_LIMITS (12, 15)
#include BOOST_PP_LOCAL_ITERATE()
#define BOOST_PP_LOCAL_MACRO( i )\
DECLARE_ENUM_VAL( BOOST_PP_CAT(BOOST_PP_CAT( PLB6MCIF2_P6BMTAG3_Mst,i),ID_i ),  IBM_BIT_INDEX(32, BOOST_PP_CAT(BOOST_PP_CAT( PLB6MCIF2_P6BMTAG3_Mst,i),ID_e )) )
#define BOOST_PP_LOCAL_LIMITS (12, 15)
#include BOOST_PP_LOCAL_ITERATE()
END_ENUM( PLB6MCIF2_P6BMTAG3_FIELD )


BEGIN_ENUM( PLB6MCIF2_MAPxCF_FIELD )
DECLARE_ENUM_VAL( PLB6MCIF2_MAPxCF_MstIDWrAcc_e, 15 )
DECLARE_ENUM_VAL( PLB6MCIF2_MAPxCF_MstIDWrAcc_i, IBM_BIT_INDEX(32, PLB6MCIF2_MAPxCF_MstIDWrAcc_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_MAPxCF_MstIDWrAcc_n, 16 )
DECLARE_ENUM_VAL( PLB6MCIF2_MAPxCF_MstIDRdAcc_e, 31 )
DECLARE_ENUM_VAL( PLB6MCIF2_MAPxCF_MstIDRdAcc_i, IBM_BIT_INDEX(32, PLB6MCIF2_MAPxCF_MstIDRdAcc_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_MAPxCF_MstIDRdAcc_n, 16 )
END_ENUM( PLB6MCIF2_MAPxCF_FIELD )


BEGIN_ENUM( PLB6MCIF2_STATUS_FIELD )
DECLARE_ENUM_VAL( PLB6MCIF2_STATUS_IDLE_e,  1 )
DECLARE_ENUM_VAL( PLB6MCIF2_STATUS_IDLE_i,  IBM_BIT_INDEX(32, PLB6MCIF2_STATUS_IDLE_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_STATUS_IDLE_n,  1 )
END_ENUM( PLB6MCIF2_STATUS_FIELD )


BEGIN_ENUM( PLB6MCIF2_RID_FIELD )
DECLARE_ENUM_VAL( PLB6MCIF2_RID_e,  31 )
DECLARE_ENUM_VAL( PLB6MCIF2_RID_i,  IBM_BIT_INDEX(32, PLB6MCIF2_RID_e) )
DECLARE_ENUM_VAL( PLB6MCIF2_RID_n,  32 )
END_ENUM( PLB6MCIF2_RID_FIELD )

#endif // __DDR_PLB6MCIF2_REG_FIELD_H__
