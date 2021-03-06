/**
 * ppc_476fp_ctrl_fields.h
 */

#ifndef PPC_476FP_CTRL_FIELDS_H
#define PPC_476FP_CTRL_FIELDS_H


#include <rumboot/macros.h>
#include <arch/ibm_bit_ordering_macros.h>


BEGIN_ENUM( CTRL_PVR_FIELD )
    DECLARE_ENUM_VAL( CTRL_PVR_OWN_e,  43 )
    DECLARE_ENUM_VAL( CTRL_PVR_OWN_n,  12 )
    DECLARE_ENUM_VAL( CTRL_PVR_OWN_i,  IBM_BIT_INDEX( 64, CTRL_PVR_OWN_e ) )

    DECLARE_ENUM_VAL( CTRL_PVR_PVN_e,  63 )
    DECLARE_ENUM_VAL( CTRL_PVR_PVN_n,  20 )
    DECLARE_ENUM_VAL( CTRL_PVR_PVN_i,  IBM_BIT_INDEX( 64, CTRL_PVR_PVN_e ) )
END_ENUM( CTRL_PVR_FIELD )


BEGIN_ENUM( CTRL_PIR_FIELD )
    DECLARE_ENUM_VAL( CTRL_PIR_PIN_e,  63 )
    DECLARE_ENUM_VAL( CTRL_PIR_PIN_n,  4 )
    DECLARE_ENUM_VAL( CTRL_PIR_PIN_i,  IBM_BIT_INDEX( 64, CTRL_PIR_PIN_e ) )
END_ENUM( CTRL_PIR_FIELD )


BEGIN_ENUM( CTRL_CCR0_FIELD )
    DECLARE_ENUM_VAL( CTRL_CCR0_ITE_e,      0 )
    DECLARE_ENUM_VAL( CTRL_CCR0_ITE_n,      1 )
    DECLARE_ENUM_VAL( CTRL_CCR0_ITE_i,      IBM_BIT_INDEX( 32, CTRL_CCR0_ITE_e ) )

    DECLARE_ENUM_VAL( CTRL_CCR0_PRE_e,      1 )
    DECLARE_ENUM_VAL( CTRL_CCR0_PRE_n,      1 )
    DECLARE_ENUM_VAL( CTRL_CCR0_PRE_i,      IBM_BIT_INDEX( 32, CTRL_CCR0_PRE_e ) )

    DECLARE_ENUM_VAL( CTRL_CCR0_CRPE_e,     4 )
    DECLARE_ENUM_VAL( CTRL_CCR0_CRPE_n,     1 )
    DECLARE_ENUM_VAL( CTRL_CCR0_CRPE_i,     IBM_BIT_INDEX( 32, CTRL_CCR0_CRPE_e ) )

    DECLARE_ENUM_VAL( CTRL_CCR0_ICS_e,      10 )
    DECLARE_ENUM_VAL( CTRL_CCR0_ICS_n,      1 )
    DECLARE_ENUM_VAL( CTRL_CCR0_ICS_i,      IBM_BIT_INDEX( 32, CTRL_CCR0_ICS_e ) )

    DECLARE_ENUM_VAL( CTRL_CCR0_DAPUIB_e,   11 )
    DECLARE_ENUM_VAL( CTRL_CCR0_DAPUIB_n,   1 )
    DECLARE_ENUM_VAL( CTRL_CCR0_DAPUIB_i,   IBM_BIT_INDEX( 32, CTRL_CCR0_DAPUIB_e ) )

    DECLARE_ENUM_VAL( CTRL_CCR0_ICWRIDX_e,  15 )
    DECLARE_ENUM_VAL( CTRL_CCR0_ICWRIDX_n,  4 )
    DECLARE_ENUM_VAL( CTRL_CCR0_ICWRIDX_i,  IBM_BIT_INDEX( 32, CTRL_CCR0_ICWRIDX_e ) )

    DECLARE_ENUM_VAL( CTRL_CCR0_DTB_e,      16 )
    DECLARE_ENUM_VAL( CTRL_CCR0_DTB_n,      1 )
    DECLARE_ENUM_VAL( CTRL_CCR0_DTB_i,      IBM_BIT_INDEX( 32, CTRL_CCR0_DTB_e ) )

    DECLARE_ENUM_VAL( CTRL_CCR0_FLSTA_e,    23 )
    DECLARE_ENUM_VAL( CTRL_CCR0_FLSTA_n,    1 )
    DECLARE_ENUM_VAL( CTRL_CCR0_FLSTA_i,    IBM_BIT_INDEX( 32, CTRL_CCR0_FLSTA_e ) )

    DECLARE_ENUM_VAL( CTRL_CCR0_DBTAC_e,    25 )
    DECLARE_ENUM_VAL( CTRL_CCR0_DBTAC_n,    1 )
    DECLARE_ENUM_VAL( CTRL_CCR0_DBTAC_i,    IBM_BIT_INDEX( 32, CTRL_CCR0_DBTAC_e ) )

    DECLARE_ENUM_VAL( CTRL_CCR0_DQWPM_e,    29 )
    DECLARE_ENUM_VAL( CTRL_CCR0_DQWPM_n,    2 )
    DECLARE_ENUM_VAL( CTRL_CCR0_DQWPM_i,    IBM_BIT_INDEX( 32, CTRL_CCR0_DQWPM_e ) )

    DECLARE_ENUM_VAL( CTRL_CCR0_IQWPM_e,    30 )
    DECLARE_ENUM_VAL( CTRL_CCR0_IQWPM_n,    1 )
    DECLARE_ENUM_VAL( CTRL_CCR0_IQWPM_i,    IBM_BIT_INDEX( 32, CTRL_CCR0_IQWPM_e ) )
END_ENUM( CTRL_CCR0_FIELD )

BEGIN_ENUM( CTRL_CCR0_ICS )
    DECLARE_ENUM_VAL( CTRL_CCR0_ICS_32byte,     0b0 )
    DECLARE_ENUM_VAL( CTRL_CCR0_ICS_128byte,    0b1 )
END_ENUM( CTRL_CCR0_ICS )

BEGIN_ENUM( CTRL_CCR0_DQWPM )
    DECLARE_ENUM_VAL( CTRL_CCR0_DQWPM_No_prediction,    0b00 )
    DECLARE_ENUM_VAL( CTRL_CCR0_DQWPM_Use_EA,           0b01 )
    DECLARE_ENUM_VAL( CTRL_CCR0_DQWPM_Use_last_qw_EA,   0b10 )
    DECLARE_ENUM_VAL( CTRL_CCR0_DQWPM_Use_NOT_EA,       0b11 )
END_ENUM( CTRL_CCR0_DQWPM )

BEGIN_ENUM( CTRL_CCR0_IQWPM )
    DECLARE_ENUM_VAL( CTRL_CCR0_IQWPM_Use_last_qw_EA,   0b0 )
    DECLARE_ENUM_VAL( CTRL_CCR0_IQWPM_Use_EA,           0b1 )
END_ENUM( CTRL_CCR0_IQWPM )


BEGIN_ENUM( CTRL_CCR1_FIELD )
    DECLARE_ENUM_VAL( CTRL_CCR1_GPRPEI_e,   1 )
    DECLARE_ENUM_VAL( CTRL_CCR1_GPRPEI_n,   2 )
    DECLARE_ENUM_VAL( CTRL_CCR1_GPRPEI_i,   IBM_BIT_INDEX( 32, CTRL_CCR1_GPRPEI_e ) )

    DECLARE_ENUM_VAL( CTRL_CCR1_FPRPEI_e,   3 )
    DECLARE_ENUM_VAL( CTRL_CCR1_FPRPEI_n,   2 )
    DECLARE_ENUM_VAL( CTRL_CCR1_FPRPEI_i,   IBM_BIT_INDEX( 32, CTRL_CCR1_FPRPEI_e ) )

    DECLARE_ENUM_VAL( CTRL_CCR1_ICDPEI_e,   5 )
    DECLARE_ENUM_VAL( CTRL_CCR1_ICDPEI_n,   2 )
    DECLARE_ENUM_VAL( CTRL_CCR1_ICDPEI_i,   IBM_BIT_INDEX( 32, CTRL_CCR1_ICDPEI_e ) )

    DECLARE_ENUM_VAL( CTRL_CCR1_ICLPEI_e,   7 )
    DECLARE_ENUM_VAL( CTRL_CCR1_ICLPEI_n,   2 )
    DECLARE_ENUM_VAL( CTRL_CCR1_ICLPEI_i,   IBM_BIT_INDEX( 32, CTRL_CCR1_ICLPEI_e ) )

    DECLARE_ENUM_VAL( CTRL_CCR1_ICTPEI_e,   9 )
    DECLARE_ENUM_VAL( CTRL_CCR1_ICTPEI_n,   2 )
    DECLARE_ENUM_VAL( CTRL_CCR1_ICTPEI_i,   IBM_BIT_INDEX( 32, CTRL_CCR1_ICTPEI_e ) )

    DECLARE_ENUM_VAL( CTRL_CCR1_DCDPEI_e,   11 )
    DECLARE_ENUM_VAL( CTRL_CCR1_DCDPEI_n,   2 )
    DECLARE_ENUM_VAL( CTRL_CCR1_DCDPEI_i,   IBM_BIT_INDEX( 32, CTRL_CCR1_DCDPEI_e ) )

    DECLARE_ENUM_VAL( CTRL_CCR1_DCLPEI_e,   13 )
    DECLARE_ENUM_VAL( CTRL_CCR1_DCLPEI_n,   2 )
    DECLARE_ENUM_VAL( CTRL_CCR1_DCLPEI_i,   IBM_BIT_INDEX( 32, CTRL_CCR1_DCLPEI_e ) )

    DECLARE_ENUM_VAL( CTRL_CCR1_DCTPEI_e,   15 )
    DECLARE_ENUM_VAL( CTRL_CCR1_DCTPEI_n,   2 )
    DECLARE_ENUM_VAL( CTRL_CCR1_DCTPEI_i,   IBM_BIT_INDEX( 32, CTRL_CCR1_DCTPEI_e ) )

    DECLARE_ENUM_VAL( CTRL_CCR1_MMUTPEI_e,  16 )
    DECLARE_ENUM_VAL( CTRL_CCR1_MMUTPEI_n,  1 )
    DECLARE_ENUM_VAL( CTRL_CCR1_MMUTPEI_i,  IBM_BIT_INDEX( 32, CTRL_CCR1_MMUTPEI_e ) )

    DECLARE_ENUM_VAL( CTRL_CCR1_MMUDPEI_e,  17 )
    DECLARE_ENUM_VAL( CTRL_CCR1_MMUDPEI_n,  1 )
    DECLARE_ENUM_VAL( CTRL_CCR1_MMUDPEI_i,  IBM_BIT_INDEX( 32, CTRL_CCR1_MMUDPEI_e ) )

    DECLARE_ENUM_VAL( CTRL_CCR1_TSS_e,      19 )
    DECLARE_ENUM_VAL( CTRL_CCR1_TSS_n,      1 )
    DECLARE_ENUM_VAL( CTRL_CCR1_TSS_i,      IBM_BIT_INDEX( 32, CTRL_CCR1_TSS_e ) )

    DECLARE_ENUM_VAL( CTRL_CCR1_DPC_e,      21 )
    DECLARE_ENUM_VAL( CTRL_CCR1_DPC_n,      1 )
    DECLARE_ENUM_VAL( CTRL_CCR1_DPC_i,      IBM_BIT_INDEX( 32, CTRL_CCR1_DPC_e ) )

    DECLARE_ENUM_VAL( CTRL_CCR1_TCS_e,      23 )
    DECLARE_ENUM_VAL( CTRL_CCR1_TCS_n,      2 )
    DECLARE_ENUM_VAL( CTRL_CCR1_TCS_i,      IBM_BIT_INDEX( 32, CTRL_CCR1_TCS_e ) )
END_ENUM( CTRL_CCR1_FIELD )

BEGIN_ENUM( CTRL_CCR1_TSS )
    DECLARE_ENUM_VAL( CTRL_CCR1_TSS_CPU_clock,  0b0 )
    DECLARE_ENUM_VAL( CTRL_CCR1_TSS_alt_clock,  0b1 )
END_ENUM( CTRL_CCR1_TSS )

BEGIN_ENUM( CTRL_CCR1_TCS )
    DECLARE_ENUM_VAL( CTRL_CCR1_TCS_div1,   0b00 )
    DECLARE_ENUM_VAL( CTRL_CCR1_TCS_div4,   0b01 )
    DECLARE_ENUM_VAL( CTRL_CCR1_TCS_div8,   0b10 )
    DECLARE_ENUM_VAL( CTRL_CCR1_TCS_div16,  0b11 )
END_ENUM( CTRL_CCR1_TCS )


BEGIN_ENUM( CTRL_CCR2_FIELD )
    DECLARE_ENUM_VAL( CTRL_CCR2_DSTG_e,     1 )
    DECLARE_ENUM_VAL( CTRL_CCR2_DSTG_n,     2 )
    DECLARE_ENUM_VAL( CTRL_CCR2_DSTG_i,     IBM_BIT_INDEX( 32, CTRL_CCR2_DSTG_e ) )

    DECLARE_ENUM_VAL( CTRL_CCR2_DLFPD_e,    2 )
    DECLARE_ENUM_VAL( CTRL_CCR2_DLFPD_n,    1 )
    DECLARE_ENUM_VAL( CTRL_CCR2_DLFPD_i,    IBM_BIT_INDEX( 32, CTRL_CCR2_DLFPD_e ) )

    DECLARE_ENUM_VAL( CTRL_CCR2_DSTI_e,     4 )
    DECLARE_ENUM_VAL( CTRL_CCR2_DSTI_n,     1 )
    DECLARE_ENUM_VAL( CTRL_CCR2_DSTI_i,     IBM_BIT_INDEX( 32, CTRL_CCR2_DSTI_e ) )

    DECLARE_ENUM_VAL( CTRL_CCR2_PMUD_e,     9 )
    DECLARE_ENUM_VAL( CTRL_CCR2_PMUD_n,     1 )
    DECLARE_ENUM_VAL( CTRL_CCR2_PMUD_i,     IBM_BIT_INDEX( 32, CTRL_CCR2_PMUD_e ) )

    DECLARE_ENUM_VAL( CTRL_CCR2_DCSTGW_e,   11 )
    DECLARE_ENUM_VAL( CTRL_CCR2_DCSTGW_n,   1 )
    DECLARE_ENUM_VAL( CTRL_CCR2_DCSTGW_i,   IBM_BIT_INDEX( 32, CTRL_CCR2_DCSTGW_e ) )

    DECLARE_ENUM_VAL( CTRL_CCR2_STGCTR_e,   15 )
    DECLARE_ENUM_VAL( CTRL_CCR2_STGCTR_n,   4 )
    DECLARE_ENUM_VAL( CTRL_CCR2_STGCTR_i,   IBM_BIT_INDEX( 32, CTRL_CCR2_STGCTR_e ) )

    DECLARE_ENUM_VAL( CTRL_CCR2_DISTG_e,    16 )
    DECLARE_ENUM_VAL( CTRL_CCR2_DISTG_n,    1 )
    DECLARE_ENUM_VAL( CTRL_CCR2_DISTG_i,    IBM_BIT_INDEX( 32, CTRL_CCR2_DISTG_e ) )

    DECLARE_ENUM_VAL( CTRL_CCR2_SPC5C1_e,   20 )
    DECLARE_ENUM_VAL( CTRL_CCR2_SPC5C1_n,   1 )
    DECLARE_ENUM_VAL( CTRL_CCR2_SPC5C1_i,   IBM_BIT_INDEX( 32, CTRL_CCR2_SPC5C1_e ) )

    DECLARE_ENUM_VAL( CTRL_CCR2_MCDTO_e,    21 )
    DECLARE_ENUM_VAL( CTRL_CCR2_MCDTO_n,    1 )
    DECLARE_ENUM_VAL( CTRL_CCR2_MCDTO_i,    IBM_BIT_INDEX( 32, CTRL_CCR2_MCDTO_e ) )
END_ENUM( CTRL_CCR2_FIELD )

BEGIN_ENUM( CTRL_CCR2_DSTG )
    DECLARE_ENUM_VAL( CTRL_CCR2_DSTG_enabled,       0b00 )
    DECLARE_ENUM_VAL( CTRL_CCR2_DSTG_contiguous,    0b01 )
    DECLARE_ENUM_VAL( CTRL_CCR2_DSTG_disabled,      0b11 )
END_ENUM( CTRL_CCR2_DSTG )


BEGIN_ENUM( CTRL_RSTCFG_FIELD )
    DECLARE_ENUM_VAL( CTRL_RSTCFG_ERPN_e,   11 )
    DECLARE_ENUM_VAL( CTRL_RSTCFG_ERPN_n,   10 )
    DECLARE_ENUM_VAL( CTRL_RSTCFG_ERPN_i,   IBM_BIT_INDEX( 32, CTRL_RSTCFG_ERPN_e ) )

    DECLARE_ENUM_VAL( CTRL_RSTCFG_E_e,      17 )
    DECLARE_ENUM_VAL( CTRL_RSTCFG_E_n,      1 )
    DECLARE_ENUM_VAL( CTRL_RSTCFG_E_i,      IBM_BIT_INDEX( 32, CTRL_RSTCFG_E_e ) )

    DECLARE_ENUM_VAL( CTRL_RSTCFG_U_e,      31 )
    DECLARE_ENUM_VAL( CTRL_RSTCFG_U_n,      4 )
    DECLARE_ENUM_VAL( CTRL_RSTCFG_U_i,      IBM_BIT_INDEX( 32, CTRL_RSTCFG_U_e ) )
END_ENUM( CTRL_RSTCFG_FIELD )


BEGIN_ENUM( CTRL_DCRIPR_FIELD )
    DECLARE_ENUM_VAL( CTRL_DCRIPR_UOA_e,    21 )
    DECLARE_ENUM_VAL( CTRL_DCRIPR_UOA_n,    22 )
    DECLARE_ENUM_VAL( CTRL_DCRIPR_UOA_i,    IBM_BIT_INDEX( 32, CTRL_DCRIPR_UOA_e ) )
END_ENUM( CTRL_DCRIPR_FIELD )


#endif // PPC_476FP_CTRL_FIELDS_H
