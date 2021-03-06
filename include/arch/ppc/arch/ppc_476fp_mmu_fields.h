/**
 * ppc_476fp_mmu_fields.h
 */

#ifndef PPC_476FP_MMU_FIELDS_H
#define PPC_476FP_MMU_FIELDS_H


#include <rumboot/macros.h>
#include <arch/ibm_bit_ordering_macros.h>
#include <arch/mem_window.h>


BEGIN_ENUM( MMU_MMUCR_FIELD )
    DECLARE_ENUM_VAL( MMU_MMUCR_REALE_e,    0 )
    DECLARE_ENUM_VAL( MMU_MMUCR_REALE_n,    1 )
    DECLARE_ENUM_VAL( MMU_MMUCR_REALE_i,    IBM_BIT_INDEX( 32, MMU_MMUCR_REALE_e ) )

    DECLARE_ENUM_VAL( MMU_MMUCR_LWAY_e,     2 )
    DECLARE_ENUM_VAL( MMU_MMUCR_LWAY_n,     2 )
    DECLARE_ENUM_VAL( MMU_MMUCR_LWAY_i,     IBM_BIT_INDEX( 32, MMU_MMUCR_LWAY_e ) )

    DECLARE_ENUM_VAL( MMU_MMUCR_LVALID_e,   3 )
    DECLARE_ENUM_VAL( MMU_MMUCR_LVALID_n,   1 )
    DECLARE_ENUM_VAL( MMU_MMUCR_LVALID_i,   IBM_BIT_INDEX( 32, MMU_MMUCR_LVALID_e ) )

    DECLARE_ENUM_VAL( MMU_MMUCR_DULXE_e,    4 )
    DECLARE_ENUM_VAL( MMU_MMUCR_DULXE_n,    1 )
    DECLARE_ENUM_VAL( MMU_MMUCR_DULXE_i,    IBM_BIT_INDEX( 32, MMU_MMUCR_DULXE_e ) )

    DECLARE_ENUM_VAL( MMU_MMUCR_IULXE_e,    5 )
    DECLARE_ENUM_VAL( MMU_MMUCR_IULXE_n,    1 )
    DECLARE_ENUM_VAL( MMU_MMUCR_IULXE_i,    IBM_BIT_INDEX( 32, MMU_MMUCR_IULXE_e ) )

    DECLARE_ENUM_VAL( MMU_MMUCR_LINDEX_e,   14 )
    DECLARE_ENUM_VAL( MMU_MMUCR_LINDEX_n,   8 )
    DECLARE_ENUM_VAL( MMU_MMUCR_LINDEX_i,   IBM_BIT_INDEX( 32, MMU_MMUCR_LINDEX_e ) )

    DECLARE_ENUM_VAL( MMU_MMUCR_STS_e,      15 )
    DECLARE_ENUM_VAL( MMU_MMUCR_STS_n,      1 )
    DECLARE_ENUM_VAL( MMU_MMUCR_STS_i,      IBM_BIT_INDEX( 32, MMU_MMUCR_STS_e ) )

    DECLARE_ENUM_VAL( MMU_MMUCR_STID_e,     31 )
    DECLARE_ENUM_VAL( MMU_MMUCR_STID_n,     16 )
    DECLARE_ENUM_VAL( MMU_MMUCR_STID_i,     IBM_BIT_INDEX( 32, MMU_MMUCR_STID_e ) )
END_ENUM( MMU_MMUCR_FIELD )

#define MMU_MMUCR( REALE,     DULXE, IULXE,   TS,     TID ) (\
    (REALE << MMU_MMUCR_REALE_i)\
|   (DULXE << MMU_MMUCR_DULXE_i)\
|   (IULXE << MMU_MMUCR_IULXE_i)\
|   (TS << MMU_MMUCR_STS_i)\
|   (TID << MMU_MMUCR_STID_i)\
)


BEGIN_ENUM( MMU_MMUBE0_FIELD )
    DECLARE_ENUM_VAL( MMU_MMUBE0_IBE0_e,    7 )
    DECLARE_ENUM_VAL( MMU_MMUBE0_IBE0_n,    8 )
    DECLARE_ENUM_VAL( MMU_MMUBE0_IBE0_i,    IBM_BIT_INDEX( 32, MMU_MMUBE0_IBE0_e ) )

    DECLARE_ENUM_VAL( MMU_MMUBE0_IBE1_e,    15 )
    DECLARE_ENUM_VAL( MMU_MMUBE0_IBE1_n,    8 )
    DECLARE_ENUM_VAL( MMU_MMUBE0_IBE1_i,    IBM_BIT_INDEX( 32, MMU_MMUBE0_IBE1_e ) )

    DECLARE_ENUM_VAL( MMU_MMUBE0_IBE2_e,    23 )
    DECLARE_ENUM_VAL( MMU_MMUBE0_IBE2_n,    8 )
    DECLARE_ENUM_VAL( MMU_MMUBE0_IBE2_i,    IBM_BIT_INDEX( 32, MMU_MMUBE0_IBE2_e ) )

    DECLARE_ENUM_VAL( MMU_MMUBE0_VBE0_e,    29 )
    DECLARE_ENUM_VAL( MMU_MMUBE0_VBE0_n,    1 )
    DECLARE_ENUM_VAL( MMU_MMUBE0_VBE0_i,    IBM_BIT_INDEX( 32, MMU_MMUBE0_VBE0_e ) )

    DECLARE_ENUM_VAL( MMU_MMUBE0_VBE1_e,    30 )
    DECLARE_ENUM_VAL( MMU_MMUBE0_VBE1_n,    1 )
    DECLARE_ENUM_VAL( MMU_MMUBE0_VBE1_i,    IBM_BIT_INDEX( 32, MMU_MMUBE0_VBE1_e ) )

    DECLARE_ENUM_VAL( MMU_MMUBE0_VBE2_e,    31 )
    DECLARE_ENUM_VAL( MMU_MMUBE0_VBE2_n,    1 )
    DECLARE_ENUM_VAL( MMU_MMUBE0_VBE2_i,    IBM_BIT_INDEX( 32, MMU_MMUBE0_VBE2_e ) )
END_ENUM( MMU_MMUBE0_FIELD )


BEGIN_ENUM( MMU_MMUBE1_FIELD )
    DECLARE_ENUM_VAL( MMU_MMUBE1_IBE3_e,    7 )
    DECLARE_ENUM_VAL( MMU_MMUBE1_IBE3_n,    8 )
    DECLARE_ENUM_VAL( MMU_MMUBE1_IBE3_i,    IBM_BIT_INDEX( 32, MMU_MMUBE1_IBE3_e ) )

    DECLARE_ENUM_VAL( MMU_MMUBE1_IBE4_e,    15 )
    DECLARE_ENUM_VAL( MMU_MMUBE1_IBE4_n,    8 )
    DECLARE_ENUM_VAL( MMU_MMUBE1_IBE4_i,    IBM_BIT_INDEX( 32, MMU_MMUBE1_IBE4_e ) )

    DECLARE_ENUM_VAL( MMU_MMUBE1_IBE5_e,    23 )
    DECLARE_ENUM_VAL( MMU_MMUBE1_IBE5_n,    8 )
    DECLARE_ENUM_VAL( MMU_MMUBE1_IBE5_i,    IBM_BIT_INDEX( 32, MMU_MMUBE1_IBE5_e ) )

    DECLARE_ENUM_VAL( MMU_MMUBE1_VBE3_e,    29 )
    DECLARE_ENUM_VAL( MMU_MMUBE1_VBE3_n,    1 )
    DECLARE_ENUM_VAL( MMU_MMUBE1_VBE3_i,    IBM_BIT_INDEX( 32, MMU_MMUBE1_VBE3_e ) )

    DECLARE_ENUM_VAL( MMU_MMUBE1_VBE4_e,    30 )
    DECLARE_ENUM_VAL( MMU_MMUBE1_VBE4_n,    1 )
    DECLARE_ENUM_VAL( MMU_MMUBE1_VBE4_i,    IBM_BIT_INDEX( 32, MMU_MMUBE1_VBE4_e ) )

    DECLARE_ENUM_VAL( MMU_MMUBE1_VBE5_e,    31 )
    DECLARE_ENUM_VAL( MMU_MMUBE1_VBE5_n,    1 )
    DECLARE_ENUM_VAL( MMU_MMUBE1_VBE5_i,    IBM_BIT_INDEX( 32, MMU_MMUBE1_VBE5_e ) )
END_ENUM( MMU_MMUBE1_FIELD )


BEGIN_ENUM( MMU_SSPCR_FIELD )
    DECLARE_ENUM_VAL( MMU_SSPCR_ORDn_n,                                 4 )

    DECLARE_ENUM_VAL( MMU_SSPCR_ORD1_e, 3 )
    DECLARE_ENUM_VAL( MMU_SSPCR_ORD1_i, IBM_BIT_INDEX( 32, MMU_SSPCR_ORD1_e ) )

    DECLARE_ENUM_VAL( MMU_SSPCR_ORD2_e, 7 )
    DECLARE_ENUM_VAL( MMU_SSPCR_ORD2_i, IBM_BIT_INDEX( 32, MMU_SSPCR_ORD2_e ) )

    DECLARE_ENUM_VAL( MMU_SSPCR_ORD3_e, 11 )
    DECLARE_ENUM_VAL( MMU_SSPCR_ORD3_i, IBM_BIT_INDEX( 32, MMU_SSPCR_ORD3_e ) )

    DECLARE_ENUM_VAL( MMU_SSPCR_ORD4_e, 15 )
    DECLARE_ENUM_VAL( MMU_SSPCR_ORD4_i, IBM_BIT_INDEX( 32, MMU_SSPCR_ORD4_e ) )

    DECLARE_ENUM_VAL( MMU_SSPCR_ORD5_e, 19 )
    DECLARE_ENUM_VAL( MMU_SSPCR_ORD5_i, IBM_BIT_INDEX( 32, MMU_SSPCR_ORD5_e ) )

    DECLARE_ENUM_VAL( MMU_SSPCR_ORD6_e, 23 )
    DECLARE_ENUM_VAL( MMU_SSPCR_ORD6_i, IBM_BIT_INDEX( 32, MMU_SSPCR_ORD6_e ) )

    DECLARE_ENUM_VAL( MMU_SSPCR_ORD7_e, 27 )
    DECLARE_ENUM_VAL( MMU_SSPCR_ORD7_i, IBM_BIT_INDEX( 32, MMU_SSPCR_ORD7_e ) )
END_ENUM( MMU_SSPCR_FIELD )

BEGIN_ENUM( MMU_ISPCR_FIELD )
    DECLARE_ENUM_VAL( MMU_ISPCR_ORDn_n,                                 3 )

    DECLARE_ENUM_VAL( MMU_ISPCR_ORD1_e, 3 )
    DECLARE_ENUM_VAL( MMU_ISPCR_ORD1_i, IBM_BIT_INDEX( 32, MMU_ISPCR_ORD1_e ) )

    DECLARE_ENUM_VAL( MMU_ISPCR_ORD2_e, 7 )
    DECLARE_ENUM_VAL( MMU_ISPCR_ORD2_i, IBM_BIT_INDEX( 32, MMU_ISPCR_ORD2_e ) )

    DECLARE_ENUM_VAL( MMU_ISPCR_ORD3_e, 11 )
    DECLARE_ENUM_VAL( MMU_ISPCR_ORD3_i, IBM_BIT_INDEX( 32, MMU_ISPCR_ORD3_e ) )

    DECLARE_ENUM_VAL( MMU_ISPCR_ORD4_e, 15 )
    DECLARE_ENUM_VAL( MMU_ISPCR_ORD4_i, IBM_BIT_INDEX( 32, MMU_ISPCR_ORD4_e ) )

    DECLARE_ENUM_VAL( MMU_ISPCR_ORD5_e, 19 )
    DECLARE_ENUM_VAL( MMU_ISPCR_ORD5_i, IBM_BIT_INDEX( 32, MMU_ISPCR_ORD5_e ) )

    DECLARE_ENUM_VAL( MMU_ISPCR_ORD6_e, 23 )
    DECLARE_ENUM_VAL( MMU_ISPCR_ORD6_i, IBM_BIT_INDEX( 32, MMU_ISPCR_ORD6_e ) )

    DECLARE_ENUM_VAL( MMU_ISPCR_ORD7_e, 27 )
    DECLARE_ENUM_VAL( MMU_ISPCR_ORD7_i, IBM_BIT_INDEX( 32, MMU_ISPCR_ORD7_e ) )
END_ENUM( MMU_ISPCR_FIELD )

BEGIN_ENUM( MMU_USPCR_FIELD )
    DECLARE_ENUM_VAL( MMU_USPCR_ORDn_n,                                 4 )

    DECLARE_ENUM_VAL( MMU_USPCR_ORD1_e, 3 )
    DECLARE_ENUM_VAL( MMU_USPCR_ORD1_i, IBM_BIT_INDEX( 32, MMU_USPCR_ORD1_e ) )

    DECLARE_ENUM_VAL( MMU_USPCR_ORD2_e, 7 )
    DECLARE_ENUM_VAL( MMU_USPCR_ORD2_i, IBM_BIT_INDEX( 32, MMU_USPCR_ORD2_e ) )

    DECLARE_ENUM_VAL( MMU_USPCR_ORD3_e, 11 )
    DECLARE_ENUM_VAL( MMU_USPCR_ORD3_i, IBM_BIT_INDEX( 32, MMU_USPCR_ORD3_e ) )

    DECLARE_ENUM_VAL( MMU_USPCR_ORD4_e, 15 )
    DECLARE_ENUM_VAL( MMU_USPCR_ORD4_i, IBM_BIT_INDEX( 32, MMU_USPCR_ORD4_e ) )

    DECLARE_ENUM_VAL( MMU_USPCR_ORD5_e, 19 )
    DECLARE_ENUM_VAL( MMU_USPCR_ORD5_i, IBM_BIT_INDEX( 32, MMU_USPCR_ORD5_e ) )

    DECLARE_ENUM_VAL( MMU_USPCR_ORD6_e, 23 )
    DECLARE_ENUM_VAL( MMU_USPCR_ORD6_i, IBM_BIT_INDEX( 32, MMU_USPCR_ORD6_e ) )

    DECLARE_ENUM_VAL( MMU_USPCR_ORD7_e, 27 )
    DECLARE_ENUM_VAL( MMU_USPCR_ORD7_i, IBM_BIT_INDEX( 32, MMU_USPCR_ORD7_e ) )
END_ENUM( MMU_USPCR_FIELD )

DECLARE_CONST( MMU_XSPCR_DEFAULT,   0x00000000 )

DECLARE_CONST( MMU_XSPCR_ORD_END,   0 )

BEGIN_ENUM( MMU_XSPCR_ORD_SIZE )
    DECLARE_ENUM_VAL( MMU_XSPCR_ORD_4KB,    0b001 )
    DECLARE_ENUM_VAL( MMU_XSPCR_ORD_16KB,   0b010 )
    DECLARE_ENUM_VAL( MMU_XSPCR_ORD_64KB,   0b011 )
    DECLARE_ENUM_VAL( MMU_XSPCR_ORD_1MB,    0b100 )
    DECLARE_ENUM_VAL( MMU_XSPCR_ORD_16MB,   0b101 )
    DECLARE_ENUM_VAL( MMU_XSPCR_ORD_256MB,  0b110 )
    DECLARE_ENUM_VAL( MMU_XSPCR_ORD_1GB,    0b111 )
END_ENUM( MMU_XSPCR_ORD_SIZE )

BEGIN_ENUM( MMU_XSPCR_ORD_TYPE )
    DECLARE_ENUM_VAL( MMU_SUSPCR_ORD_PID_ONLY,  0b0000 )
    DECLARE_ENUM_VAL( MMU_SUSPCR_ORD_SHARED,    0b1000 )
END_ENUM( MMU_XSPCR_ORD_TYPE )


BEGIN_ENUM( MMU_PID_FIELD )
    DECLARE_ENUM_VAL( MMU_PID_e,    31 )
    DECLARE_ENUM_VAL( MMU_PID_n,    16 )
    DECLARE_ENUM_VAL( MMU_PID_i,    IBM_BIT_INDEX( 32, MMU_PID_e ) )
END_ENUM( MMU_PID_FIELD )


BEGIN_ENUM( MMU_TLBE_RA_FIELD )
    DECLARE_ENUM_VAL( MMU_TLBE_RA_WAY_e,    34 )
    DECLARE_ENUM_VAL( MMU_TLBE_RA_WAY_n,    3 )
    DECLARE_ENUM_VAL( MMU_TLBE_RA_WAY_i,    IBM_BIT_INDEX( 64, MMU_TLBE_RA_WAY_e ) )

    DECLARE_ENUM_VAL( MMU_TLBE_RA_BE_e,     39 )
    DECLARE_ENUM_VAL( MMU_TLBE_RA_BE_n,     4 )
    DECLARE_ENUM_VAL( MMU_TLBE_RA_BE_i,     IBM_BIT_INDEX( 64, MMU_TLBE_RA_BE_e ) )

    DECLARE_ENUM_VAL( MMU_TLBE_RA_INDEX_e,  47 )
    DECLARE_ENUM_VAL( MMU_TLBE_RA_INDEX_n,  8 )
    DECLARE_ENUM_VAL( MMU_TLBE_RA_INDEX_i,  IBM_BIT_INDEX( 64, MMU_TLBE_RA_INDEX_e ) )
END_ENUM( MMU_TLBE_RA_FIELD )

BEGIN_ENUM( MMU_TLBWE_WAY )
    DECLARE_ENUM_VAL( MMU_TLBWE_WAY_UND,    0b000 )
    DECLARE_ENUM_VAL( MMU_TLBWE_WAY_0,      0b100 )
    DECLARE_ENUM_VAL( MMU_TLBWE_WAY_1,      0b101 )
    DECLARE_ENUM_VAL( MMU_TLBWE_WAY_2,      0b110 )
    DECLARE_ENUM_VAL( MMU_TLBWE_WAY_3,      0b111 )
END_ENUM( MMU_TLBWE_WAY )

BEGIN_ENUM( MMU_TLBWE_BE )
    DECLARE_ENUM_VAL( MMU_TLBWE_BE_UND, 0b0000 )
    DECLARE_ENUM_VAL( MMU_TLBWE_BE_0,   0b1000 )
    DECLARE_ENUM_VAL( MMU_TLBWE_BE_1,   0b1001 )
    DECLARE_ENUM_VAL( MMU_TLBWE_BE_2,   0b1010 )
    DECLARE_ENUM_VAL( MMU_TLBWE_BE_3,   0b1011 )
    DECLARE_ENUM_VAL( MMU_TLBWE_BE_4,   0b1100 )
    DECLARE_ENUM_VAL( MMU_TLBWE_BE_5,   0b1101 )
END_ENUM( MMU_TLBWE_BE )


BEGIN_ENUM( MMU_TLBE_TAG_FIELD )
    DECLARE_ENUM_VAL( MMU_TLBE_TAG_EPN_e,       51 )
    DECLARE_ENUM_VAL( MMU_TLBE_TAG_EPN_n,       20 )
    DECLARE_ENUM_VAL( MMU_TLBE_TAG_EPN_i,       IBM_BIT_INDEX( 64, MMU_TLBE_TAG_EPN_e ) )

    DECLARE_ENUM_VAL( MMU_TLBE_TAG_V_e,         52 )
    DECLARE_ENUM_VAL( MMU_TLBE_TAG_V_n,         1 )
    DECLARE_ENUM_VAL( MMU_TLBE_TAG_V_i,         IBM_BIT_INDEX( 64, MMU_TLBE_TAG_V_e ) )

    DECLARE_ENUM_VAL( MMU_TLBE_TAG_TS_e,        53 )
    DECLARE_ENUM_VAL( MMU_TLBE_TAG_TS_n,        1 )
    DECLARE_ENUM_VAL( MMU_TLBE_TAG_TS_i,        IBM_BIT_INDEX( 64, MMU_TLBE_TAG_TS_e ) )

    DECLARE_ENUM_VAL( MMU_TLBE_TAG_DSIZ_e,      59 )
    DECLARE_ENUM_VAL( MMU_TLBE_TAG_DSIZ_n,      6 )
    DECLARE_ENUM_VAL( MMU_TLBE_TAG_DSIZ_i,      IBM_BIT_INDEX( 64, MMU_TLBE_TAG_DSIZ_e ) )

    DECLARE_ENUM_VAL( MMU_TLBE_TAG_BLTD_e,      60 )
    DECLARE_ENUM_VAL( MMU_TLBE_TAG_BLTD_n,      1 )
    DECLARE_ENUM_VAL( MMU_TLBE_TAG_BLTD_i,      IBM_BIT_INDEX( 64, MMU_TLBE_TAG_BLTD_e ) )

    DECLARE_ENUM_VAL( MMU_TLBE_TAG_EPNPar_e,    61 )
    DECLARE_ENUM_VAL( MMU_TLBE_TAG_EPNPar_n,    1 )
    DECLARE_ENUM_VAL( MMU_TLBE_TAG_EPNPar_i,    IBM_BIT_INDEX( 64, MMU_TLBE_TAG_EPNPar_e ) )

    DECLARE_ENUM_VAL( MMU_TLBE_TAG_DSIZPar_e,   62 )
    DECLARE_ENUM_VAL( MMU_TLBE_TAG_DSIZPar_n,   1 )
    DECLARE_ENUM_VAL( MMU_TLBE_TAG_DSIZPar_i,   IBM_BIT_INDEX( 64, MMU_TLBE_TAG_DSIZPar_e ) )

    DECLARE_ENUM_VAL( MMU_TLBE_TAG_TIDPar_e,    63 )
    DECLARE_ENUM_VAL( MMU_TLBE_TAG_TIDPar_n,    1 )
    DECLARE_ENUM_VAL( MMU_TLBE_TAG_TIDPar_i,    IBM_BIT_INDEX( 64, MMU_TLBE_TAG_TIDPar_e ) )
END_ENUM( MMU_TLBE_TAG_FIELD )

BEGIN_ENUM( MMU_TLBE_DSIZ )
    DECLARE_ENUM_VAL( MMU_TLBE_DSIZ_4KB,    0b000000 )
    DECLARE_ENUM_VAL( MMU_TLBE_DSIZ_16KB,   0b000001 )
    DECLARE_ENUM_VAL( MMU_TLBE_DSIZ_64KB,   0b000011 )
    DECLARE_ENUM_VAL( MMU_TLBE_DSIZ_1MB,    0b000111 )
    DECLARE_ENUM_VAL( MMU_TLBE_DSIZ_16MB,   0b001111 )
    DECLARE_ENUM_VAL( MMU_TLBE_DSIZ_256MB,  0b011111 )
    DECLARE_ENUM_VAL( MMU_TLBE_DSIZ_1GB,    0b111111 )
END_ENUM( MMU_TLBE_DSIZ )


BEGIN_ENUM( MMU_TLBE_DATA_FIELD )
    DECLARE_ENUM_VAL( MMU_TLBE_DATA_RPN_e,      51 )
    DECLARE_ENUM_VAL( MMU_TLBE_DATA_RPN_n,      20 )
    DECLARE_ENUM_VAL( MMU_TLBE_DATA_RPN_i,      IBM_BIT_INDEX( 64, MMU_TLBE_DATA_RPN_e ) )

    DECLARE_ENUM_VAL( MMU_TLBE_DATA_RPNPar_e,   52 )
    DECLARE_ENUM_VAL( MMU_TLBE_DATA_RPNPar_n,   1 )
    DECLARE_ENUM_VAL( MMU_TLBE_DATA_RPNPar_i,   IBM_BIT_INDEX( 64, MMU_TLBE_DATA_RPNPar_e ) )

    DECLARE_ENUM_VAL( MMU_TLBE_DATA_ERPNPar_e,  53 )
    DECLARE_ENUM_VAL( MMU_TLBE_DATA_ERPNPar_n,  1 )
    DECLARE_ENUM_VAL( MMU_TLBE_DATA_ERPNPar_i,  IBM_BIT_INDEX( 64, MMU_TLBE_DATA_ERPNPar_e ) )

    DECLARE_ENUM_VAL( MMU_TLBE_DATA_ERPN_e,     63 )
    DECLARE_ENUM_VAL( MMU_TLBE_DATA_ERPN_n,     10 )
    DECLARE_ENUM_VAL( MMU_TLBE_DATA_ERPN_i,     IBM_BIT_INDEX( 64, MMU_TLBE_DATA_ERPN_e ) )
END_ENUM( MMU_TLBE_DATA_FIELD )

BEGIN_ENUM( MMU_TLBE_ATTR_FIELD )
    DECLARE_ENUM_VAL( MMU_TLBE_ATTR_SPPar_e,    32 )
    DECLARE_ENUM_VAL( MMU_TLBE_ATTR_SPPar_n,    1 )
    DECLARE_ENUM_VAL( MMU_TLBE_ATTR_SPPar_i,    IBM_BIT_INDEX( 64, MMU_TLBE_ATTR_SPPar_e ) )

    DECLARE_ENUM_VAL( MMU_TLBE_ATTR_IL1I_e,     46 )
    DECLARE_ENUM_VAL( MMU_TLBE_ATTR_IL1I_n,     1 )
    DECLARE_ENUM_VAL( MMU_TLBE_ATTR_IL1I_i,     IBM_BIT_INDEX( 64, MMU_TLBE_ATTR_IL1I_e ) )

    DECLARE_ENUM_VAL( MMU_TLBE_ATTR_IL1D_e,     47 )
    DECLARE_ENUM_VAL( MMU_TLBE_ATTR_IL1D_n,     1 )
    DECLARE_ENUM_VAL( MMU_TLBE_ATTR_IL1D_i,     IBM_BIT_INDEX( 64, MMU_TLBE_ATTR_IL1D_e ) )

    DECLARE_ENUM_VAL( MMU_TLBE_ATTR_U_e,        51 )
    DECLARE_ENUM_VAL( MMU_TLBE_ATTR_U_n,        4 )
    DECLARE_ENUM_VAL( MMU_TLBE_ATTR_U_i,        IBM_BIT_INDEX( 64, MMU_TLBE_ATTR_U_e ) )

    DECLARE_ENUM_VAL( MMU_TLBE_ATTR_W_e,        52 )
    DECLARE_ENUM_VAL( MMU_TLBE_ATTR_W_n,        1 )
    DECLARE_ENUM_VAL( MMU_TLBE_ATTR_W_i,        IBM_BIT_INDEX( 64, MMU_TLBE_ATTR_W_e ) )

    DECLARE_ENUM_VAL( MMU_TLBE_ATTR_I_e,        53 )
    DECLARE_ENUM_VAL( MMU_TLBE_ATTR_I_n,        1 )
    DECLARE_ENUM_VAL( MMU_TLBE_ATTR_I_i,        IBM_BIT_INDEX( 64, MMU_TLBE_ATTR_I_e ) )

    DECLARE_ENUM_VAL( MMU_TLBE_ATTR_M_e,        54 )
    DECLARE_ENUM_VAL( MMU_TLBE_ATTR_M_n,        1 )
    DECLARE_ENUM_VAL( MMU_TLBE_ATTR_M_i,        IBM_BIT_INDEX( 64, MMU_TLBE_ATTR_M_e ) )

    DECLARE_ENUM_VAL( MMU_TLBE_ATTR_G_e,        55 )
    DECLARE_ENUM_VAL( MMU_TLBE_ATTR_G_n,        1 )
    DECLARE_ENUM_VAL( MMU_TLBE_ATTR_G_i,        IBM_BIT_INDEX( 64, MMU_TLBE_ATTR_G_e ) )

    DECLARE_ENUM_VAL( MMU_TLBE_ATTR_E_e,        56 )
    DECLARE_ENUM_VAL( MMU_TLBE_ATTR_E_n,        1 )
    DECLARE_ENUM_VAL( MMU_TLBE_ATTR_E_i,        IBM_BIT_INDEX( 64, MMU_TLBE_ATTR_E_e ) )

    DECLARE_ENUM_VAL( MMU_TLBE_ATTR_UX_e,       58 )
    DECLARE_ENUM_VAL( MMU_TLBE_ATTR_UX_n,       1 )
    DECLARE_ENUM_VAL( MMU_TLBE_ATTR_UX_i,       IBM_BIT_INDEX( 64, MMU_TLBE_ATTR_UX_e ) )

    DECLARE_ENUM_VAL( MMU_TLBE_ATTR_UW_e,       59 )
    DECLARE_ENUM_VAL( MMU_TLBE_ATTR_UW_n,       1 )
    DECLARE_ENUM_VAL( MMU_TLBE_ATTR_UW_i,       IBM_BIT_INDEX( 64, MMU_TLBE_ATTR_UW_e ) )

    DECLARE_ENUM_VAL( MMU_TLBE_ATTR_UR_e,       60 )
    DECLARE_ENUM_VAL( MMU_TLBE_ATTR_UR_n,       1 )
    DECLARE_ENUM_VAL( MMU_TLBE_ATTR_UR_i,       IBM_BIT_INDEX( 64, MMU_TLBE_ATTR_UR_e ) )

    DECLARE_ENUM_VAL( MMU_TLBE_ATTR_SX_e,       61 )
    DECLARE_ENUM_VAL( MMU_TLBE_ATTR_SX_n,       1 )
    DECLARE_ENUM_VAL( MMU_TLBE_ATTR_SX_i,       IBM_BIT_INDEX( 64, MMU_TLBE_ATTR_SX_e ) )

    DECLARE_ENUM_VAL( MMU_TLBE_ATTR_SW_e,       62 )
    DECLARE_ENUM_VAL( MMU_TLBE_ATTR_SW_n,       1 )
    DECLARE_ENUM_VAL( MMU_TLBE_ATTR_SW_i,       IBM_BIT_INDEX( 64, MMU_TLBE_ATTR_SW_e ) )

    DECLARE_ENUM_VAL( MMU_TLBE_ATTR_SR_e,       63 )
    DECLARE_ENUM_VAL( MMU_TLBE_ATTR_SR_n,       1 )
    DECLARE_ENUM_VAL( MMU_TLBE_ATTR_SR_i,       IBM_BIT_INDEX( 64, MMU_TLBE_ATTR_SR_e ) )
END_ENUM( MMU_TLBE_ATTR_FIELD )

BEGIN_ENUM( MMU_TLBE_E )
    DECLARE_ENUM_VAL( MMU_TLBE_E_BIG_END,       0b0 )
    DECLARE_ENUM_VAL( MMU_TLBE_E_LITTLE_END,    0b1 )
END_ENUM( MMU_TLBE_E )


BEGIN_ENUM( MMU_TLB_ENTRY_WORD )
    DECLARE_ENUM_VAL( MMU_TLB_ENTRY_TAG,    0 )
    DECLARE_ENUM_VAL( MMU_TLB_ENTRY_DATA,   1 )
    DECLARE_ENUM_VAL( MMU_TLB_ENTRY_ATTR,   2 )
END_ENUM( MMU_TLB_ENTRY_WORD )

DECLARE_CONST( MMU_TLB_ENTRY_FIELD_SIZE,    4 )
DECLARE_CONST( MMU_TLB_ENTRY_FIELD_N,       4 )

DECLARE_CONST( MMU_TLB_ENTRY_SIZE,          (MMU_TLB_ENTRY_FIELD_SIZE*MMU_TLB_ENTRY_FIELD_N) )


#define MMU_TLBWE_RA( WAY, BID ) (\
    (WAY << MMU_TLBE_RA_WAY_i)\
|   (BID << MMU_TLBE_RA_BE_i )\
)


#define MMU_TLBRE_RA( index, way ) (\
    (way   << MMU_TLBE_RA_WAY_i  )\
|   (index << MMU_TLBE_RA_INDEX_i)\
)


#define MMU_TLBE_TAG( EPN,  V,  TS,     DSIZ,   BLTD,   EPNPar, DSIZPar, TIDPar ) (\
    (EPN     << MMU_TLBE_TAG_EPN_i    )\
|   (V       << MMU_TLBE_TAG_V_i      )\
|   (TS      << MMU_TLBE_TAG_TS_i     )\
|   (DSIZ    << MMU_TLBE_TAG_DSIZ_i   )\
|   (BLTD    << MMU_TLBE_TAG_BLTD_i   )\
|   (EPNPar  << MMU_TLBE_TAG_EPNPar_i )\
|   (DSIZPar << MMU_TLBE_TAG_DSIZPar_i)\
|   (TIDPar  << MMU_TLBE_TAG_TIDPar_i )\
)


#define MMU_TLBE_DATA( ERPN,RPN,    ERPNPar, RPNPar ) (\
    (RPN     << MMU_TLBE_DATA_RPN_i    )\
|   (RPNPar  << MMU_TLBE_DATA_RPNPar_i )\
|   (ERPNPar << MMU_TLBE_DATA_ERPNPar_i)\
|   (ERPN    << MMU_TLBE_DATA_ERPN_i   )\
)


#define MMU_TLBE_ATTR(  IL1I,IL1D,  W,  I,  M,  G,  E,      UX, UW, UR,     SX, SW, SR,     U,      SPPar ) (\
    (SPPar << MMU_TLBE_ATTR_SPPar_i)\
|   (IL1I  << MMU_TLBE_ATTR_IL1I_i )\
|   (IL1D  << MMU_TLBE_ATTR_IL1D_i )\
|   (U     << MMU_TLBE_ATTR_U_i    )\
|   (W     << MMU_TLBE_ATTR_W_i    )\
|   (I     << MMU_TLBE_ATTR_I_i    )\
|   (M     << MMU_TLBE_ATTR_M_i    )\
|   (G     << MMU_TLBE_ATTR_G_i    )\
|   (E     << MMU_TLBE_ATTR_E_i    )\
|   (UX    << MMU_TLBE_ATTR_UX_i   )\
|   (UW    << MMU_TLBE_ATTR_UW_i   )\
|   (UR    << MMU_TLBE_ATTR_UR_i   )\
|   (SX    << MMU_TLBE_ATTR_SX_i   )\
|   (SW    << MMU_TLBE_ATTR_SW_i   )\
|   (SR    << MMU_TLBE_ATTR_SR_i   )\
)

#define MMU_TLBE_ATTR_ONLY_MASK\
        MMU_TLBE_ATTR(  0b1,0b1,    0b1,0b1,0b1,0b1,0b1,    0b1,0b1,0b1,    0b1,0b1,0b1,    0xF,    0b0 )


#define MMU_TLB_ENTRY( ERPN,RPN,    EPN,    DSIZ,   IL1I,IL1D,      W, I, M, G, E,      UX,UW,UR,   SX,SW,SR,   DULXE,IULXE,    TS,     TID,    WAY,    BID,    V )\
    MMU_MMUCR( 0b0,     DULXE,IULXE,    TS,     TID ),\
\
    MMU_TLBE_TAG( EPN,  V,  TS,     DSIZ,     0b0,    0b0, 0b0, 0b0 ),\
\
    MMU_TLBE_DATA( ERPN,RPN,    0b0, 0b0 ),\
\
(   MMU_TLBWE_RA( WAY,  BID )\
|   MMU_TLBE_ATTR( IL1I,IL1D,   W, I, M, G, E,      UX,UW,UR,   SX,SW,SR,   0b0000,     0b0 )   )


#endif // PPC_476FP_MMU_FIELDS_H
