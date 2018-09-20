/*
 * emi.h
 *
 *  Created on: Jul 26, 2018
 *      Author: s.chernousov
 */

#ifndef EMI_H_FIELD
#define EMI_H_FIELD

#include <platform/common_macros/common_macros.h>

BEGIN_ENUM(ssx_btyp_t)
DECLARE_ENUM_VAL( BTYP_SRAM,    0b000 )
DECLARE_ENUM_VAL( BTYP_NOR,     0b001 )
DECLARE_ENUM_VAL( BTYP_SSRAM,   0b010 )
DECLARE_ENUM_VAL( BTYP_PIPERDY, 0b100 )
DECLARE_ENUM_VAL( BTYP_SDRAM,   0b110 )
END_ENUM( ssx_btyp_t );

BEGIN_ENUM( ssx_ptyp_t )
DECLARE_ENUM_VAL( PTYP_NO_PAGES,  0b0)
DECLARE_ENUM_VAL( PTYP_USE_PAGES, 0b1)
END_ENUM( ssx_ptyp_t );

BEGIN_ENUM( ssx_srdy_t )
DECLARE_ENUM_VAL( SRDY_EXT_RDY_NOT_USE, 0b0)
DECLARE_ENUM_VAL( SRDY_EXT_RDY_USE,     0b1)
END_ENUM( ssx_srdy_t );

BEGIN_ENUM( ssx_twr_t )
DECLARE_ENUM_VAL( TWR_0, 0b0)
DECLARE_ENUM_VAL( TWR_1, 0b1)
END_ENUM( ssx_twr_t );

BEGIN_ENUM( ssx_sst_t )
DECLARE_ENUM_VAL( SST_Flow_Through, 0b0)
DECLARE_ENUM_VAL( SST_Pipelined,    0b1)
END_ENUM( ssx_sst_t );

BEGIN_ENUM( ssx_tssoe_t )
DECLARE_ENUM_VAL( TSSOE_1, 0b0)
DECLARE_ENUM_VAL( TSSOE_2, 0b1)
END_ENUM( ssx_tssoe_t );

BEGIN_ENUM( ssx_tsoe_t )
DECLARE_ENUM_VAL( TSOE_1, 0b0)
DECLARE_ENUM_VAL( TSOE_2, 0b1)
END_ENUM( ssx_tsoe_t );

BEGIN_ENUM( ssx_tcyc_t )
DECLARE_ENUM_VAL( TCYC_2,  0b1111)
DECLARE_ENUM_VAL( TCYC_3,  0b1110)
DECLARE_ENUM_VAL( TCYC_4,  0b1101)
DECLARE_ENUM_VAL( TCYC_5,  0b1100)
DECLARE_ENUM_VAL( TCYC_6,  0b1011)
DECLARE_ENUM_VAL( TCYC_7,  0b1010)
DECLARE_ENUM_VAL( TCYC_8,  0b1001)
DECLARE_ENUM_VAL( TCYC_9,  0b1000)
DECLARE_ENUM_VAL( TCYC_10, 0b0111)
DECLARE_ENUM_VAL( TCYC_11, 0b0110)
DECLARE_ENUM_VAL( TCYC_12, 0b0101)
DECLARE_ENUM_VAL( TCYC_13, 0b0100)
DECLARE_ENUM_VAL( TCYC_14, 0b0011)
DECLARE_ENUM_VAL( TCYC_15, 0b0010)
DECLARE_ENUM_VAL( TCYC_16, 0b0001)
DECLARE_ENUM_VAL( TCYC_17, 0b0000)
END_ENUM( ssx_tcyc_t );

BEGIN_ENUM( ssx_tdel_t )
DECLARE_ENUM_VAL( TDEL_0, 0b00)
DECLARE_ENUM_VAL( TDEL_3, 0b01)
DECLARE_ENUM_VAL( TDEL_2, 0b10)
DECLARE_ENUM_VAL( TDEL_1, 0b11)
END_ENUM( ssx_tdel_t );

BEGIN_ENUM( sdx_csp_t )
DECLARE_ENUM_VAL( CSP_256,  0b000)
DECLARE_ENUM_VAL( CSP_512,  0b001)
DECLARE_ENUM_VAL( CSP_1024, 0b011)
DECLARE_ENUM_VAL( CSP_2048, 0b101)
DECLARE_ENUM_VAL( CSP_4096, 0b111)
END_ENUM( sdx_csp_t );

BEGIN_ENUM( sdx_sds_t )
DECLARE_ENUM_VAL( SDS_2M,   0b000)
DECLARE_ENUM_VAL( SDS_4M,   0b001)
DECLARE_ENUM_VAL( SDS_8M,   0b010)
DECLARE_ENUM_VAL( SDS_16M,  0b011)
DECLARE_ENUM_VAL( SDS_32M,  0b100)
DECLARE_ENUM_VAL( SDS_64M,  0b101)
DECLARE_ENUM_VAL( SDS_128M, 0b110)
DECLARE_ENUM_VAL( SDS_256M, 0b111)
END_ENUM( sdx_sds_t );

BEGIN_ENUM( sdx_cl_t )
DECLARE_ENUM_VAL( CL_3, 0b00)
DECLARE_ENUM_VAL( CL_2, 0b01)
DECLARE_ENUM_VAL( CL_1, 0b10)
END_ENUM( sdx_cl_t );

BEGIN_ENUM( sdx_trdl_t )
DECLARE_ENUM_VAL( TRDL_1, 0b0)
DECLARE_ENUM_VAL( TRDL_2, 0b1)
END_ENUM( sdx_trdl_t );

BEGIN_ENUM( sdx_si_t )
DECLARE_ENUM_VAL( SI_EXT_INIT, 0b0)
DECLARE_ENUM_VAL( SI_CPU_INIT, 0b1)
END_ENUM( sdx_si_t );

BEGIN_ENUM( sdx_trcd_t )
DECLARE_ENUM_VAL( TRCD_2, 0b11)
DECLARE_ENUM_VAL( TRCD_3, 0b10)
DECLARE_ENUM_VAL( TRCD_4, 0b01)
DECLARE_ENUM_VAL( TRCD_5, 0b00)
END_ENUM( sdx_trcd_t );

BEGIN_ENUM( sdx_tras_t )
DECLARE_ENUM_VAL( TRAS_4,  0b101)
DECLARE_ENUM_VAL( TRAS_5,  0b100)
DECLARE_ENUM_VAL( TRAS_6,  0b011)
DECLARE_ENUM_VAL( TRAS_7,  0b010)
DECLARE_ENUM_VAL( TRAS_8,  0b001)
DECLARE_ENUM_VAL( TRAS_9,  0b000)
DECLARE_ENUM_VAL( TRAS_10, 0b111)
DECLARE_ENUM_VAL( TRAS_11, 0b110)
END_ENUM( sdx_tras_t );


BEGIN_ENUM( EMI_SSx_FIELD )
DECLARE_ENUM_VAL( EMI_SSx_BTYP_i,       0 )
DECLARE_ENUM_VAL( EMI_SSx_PTYP_i,       3 )
DECLARE_ENUM_VAL( EMI_SSx_SRDY_i,       4 )
DECLARE_ENUM_VAL( EMI_SSx_TWR_i,        5 )
DECLARE_ENUM_VAL( EMI_SSx_SST_i,        6 )
DECLARE_ENUM_VAL( EMI_SSx_T_SSOE_i,     7 )
DECLARE_ENUM_VAL( EMI_SSx_T_SOE_i,      8 )
DECLARE_ENUM_VAL( EMI_SSx_T_CYC_i,      9 )
DECLARE_ENUM_VAL( EMI_SSx_T_RDY_i,      13 )
DECLARE_ENUM_VAL( EMI_SSx_T_DEL_i,      23 )
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

BEGIN_ENUM( rfc_trfc_t )
DECLARE_ENUM_VAL( TRFC_6,  0b011)
DECLARE_ENUM_VAL( TRFC_7,  0b010)
DECLARE_ENUM_VAL( TRFC_8,  0b001)
DECLARE_ENUM_VAL( TRFC_9,  0b000)
DECLARE_ENUM_VAL( TRFC_10, 0b111)
DECLARE_ENUM_VAL( TRFC_11, 0b110)
DECLARE_ENUM_VAL( TRFC_12, 0b101)
DECLARE_ENUM_VAL( TRFC_13, 0b100)
END_ENUM( rfc_trfc_t );

BEGIN_ENUM( EMI_RFC_FIELD )
DECLARE_ENUM_VAL( EMI_RFC_TRFC_i,  0 )
DECLARE_ENUM_VAL( EMI_RFC_RP_i,    3 )
END_ENUM( EMI_RFC_FIELD )

BEGIN_ENUM( EMI_WECR_FIELD )
DECLARE_ENUM_VAL( EMI_WECR_BWE0_i,   0 )
DECLARE_ENUM_VAL( EMI_WECR_BWE1_i,   1 )
DECLARE_ENUM_VAL( EMI_WECR_BWE2_i,   2 )
DECLARE_ENUM_VAL( EMI_WECR_BWE3_i,   3 )
DECLARE_ENUM_VAL( EMI_WECR_BWE4_i,   4 )
DECLARE_ENUM_VAL( EMI_WECR_HSH_i,    5 )
DECLARE_ENUM_VAL( EMI_WECR_FB_i,     6 )
DECLARE_ENUM_VAL( EMI_WECR_OE_DIS_i, 7 )
END_ENUM( EMI_WECR_FIELD )

BEGIN_ENUM( EMI_IMR_FIELD )
DECLARE_ENUM_VAL( EMI_IMR_ME1B0_i,   0 )
DECLARE_ENUM_VAL( EMI_IMR_ME2B0_i,   1 )
DECLARE_ENUM_VAL( EMI_IMR_ME1B1_i,   2 )
DECLARE_ENUM_VAL( EMI_IMR_ME2B1_i,   3 )
DECLARE_ENUM_VAL( EMI_IMR_ME1B2_i,   4 )
DECLARE_ENUM_VAL( EMI_IMR_ME2B2_i,   5 )
DECLARE_ENUM_VAL( EMI_IMR_ME1B3_i,   6 )
DECLARE_ENUM_VAL( EMI_IMR_ME2B3_i,   7 )
DECLARE_ENUM_VAL( EMI_IMR_ME1B4_i,   8 )
DECLARE_ENUM_VAL( EMI_IMR_ME2B4_i,   9 )
DECLARE_ENUM_VAL( EMI_IMR_ME1B5_i,  10 )
DECLARE_ENUM_VAL( EMI_IMR_ME2B5_i,  11 )
DECLARE_ENUM_VAL( EMI_IMR_MRDYR_i,  12 )
DECLARE_ENUM_VAL( EMI_IMR_MRDYW_i,  13 )
DECLARE_ENUM_VAL( EMI_IMR_MERRAR_i, 14 )
DECLARE_ENUM_VAL( EMI_IMR_MERRAW_i, 15 )
DECLARE_ENUM_VAL( EMI_IMR_MERRDW_i, 16 )
END_ENUM( EMI_IMR_FIELD )

BEGIN_ENUM( EMI_IRR_FIELD )
DECLARE_ENUM_VAL( EMI_IRR_IE1B0_i,   0 )
DECLARE_ENUM_VAL( EMI_IRR_IE2B0_i,   1 )
DECLARE_ENUM_VAL( EMI_IRR_IE1B1_i,   2 )
DECLARE_ENUM_VAL( EMI_IRR_IE2B1_i,   3 )
DECLARE_ENUM_VAL( EMI_IRR_IE1B2_i,   4 )
DECLARE_ENUM_VAL( EMI_IRR_IE2B2_i,   5 )
DECLARE_ENUM_VAL( EMI_IRR_IE1B3_i,   6 )
DECLARE_ENUM_VAL( EMI_IRR_IE2B3_i,   7 )
DECLARE_ENUM_VAL( EMI_IRR_IE1B4_i,   8 )
DECLARE_ENUM_VAL( EMI_IRR_IE2B4_i,   9 )
DECLARE_ENUM_VAL( EMI_IRR_IE1B5_i,  10 )
DECLARE_ENUM_VAL( EMI_IRR_IE2B5_i,  11 )
DECLARE_ENUM_VAL( EMI_IRR_IRDYR_i,  12 )
DECLARE_ENUM_VAL( EMI_IRR_IRDYW_i,  13 )
DECLARE_ENUM_VAL( EMI_IRR_IERRAR_i, 14 )
DECLARE_ENUM_VAL( EMI_IRR_IERRAW_i, 15 )
DECLARE_ENUM_VAL( EMI_IRR_IERRDW_i, 16 )
END_ENUM( EMI_IRR_FIELD )

BEGIN_ENUM( EMI_FLCNTRL_FIELD )
DECLARE_ENUM_VAL( EMI_FLCNTRL_ECC_CTRL_i,   0 )
DECLARE_ENUM_VAL( EMI_FLCNTRL_ECC_CNT_i,    2 )
END_ENUM( EMI_FLCNTRL_FIELD )

BEGIN_ENUM( flcntrl_ecc_cnt_t )
DECLARE_ENUM_VAL( ECC_CNT_0,                0b000 )
DECLARE_ENUM_VAL( ECC_CNT_2,                0b111 )
DECLARE_ENUM_VAL( ECC_CNT_3,                0b110 )
DECLARE_ENUM_VAL( ECC_CNT_4,                0b101 )
DECLARE_ENUM_VAL( ECC_CNT_5,                0b100 )
DECLARE_ENUM_VAL( ECC_CNT_6,                0b011 )
DECLARE_ENUM_VAL( ECC_CNT_7,                0b010 )
DECLARE_ENUM_VAL( ECC_CNT_8,                0b001 )
END_ENUM( flcntrl_ecc_cnt_t );

BEGIN_ENUM( flcntrl_ecc_ctrl_t )
DECLARE_ENUM_VAL( ECC_CTRL_ALWAYS_ECC,      0b00 )
DECLARE_ENUM_VAL( ECC_CTRL_ALWAYS_D7D0,     0b01 )
DECLARE_ENUM_VAL( ECC_CTRL_CNT_ECCWRR,      0b10 )
DECLARE_ENUM_VAL( ECC_CTRL_CNT_ECC,         0b11 )
END_ENUM( flcntrl_ecc_ctrl_t );


#endif /* EMI_H_ */
