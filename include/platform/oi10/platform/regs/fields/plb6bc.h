/**
 *
 */

#ifndef __PLB6BC_REG_FIELD_H__
#define __PLB6BC_REG_FIELD_H__


#include <rumboot/macros.h>
#include <platform/arch/ppc/ibm_bit_ordering_macros.h>


#define PLB6BC_MASTER_N         16
#define PLB6BC_SEGMENT_N        8
#define PLB6BC_SEGMENT_SLAVE_N  4


BEGIN_ENUM( PLB6BC_SEGMENT_SLAVE_FIELD )
DECLARE_ENUM_VAL( PLB6BC_SEGMENT_SLAVE0_i,  IBM_BIT_INDEX(4, 0) )
DECLARE_ENUM_VAL( PLB6BC_SEGMENT_SLAVE1_i,  IBM_BIT_INDEX(4, 1) )
DECLARE_ENUM_VAL( PLB6BC_SEGMENT_SLAVE2_i,  IBM_BIT_INDEX(4, 2) )
DECLARE_ENUM_VAL( PLB6BC_SEGMENT_SLAVE3_i,  IBM_BIT_INDEX(4, 3) )
END_ENUM( PLB6BC_SEGMENT_SLAVE_FIELD )


BEGIN_ENUM( PLB6BC_CR0_FIELD )
DECLARE_ENUM_VAL( PLB6BC_CR0_BC_DSBS_e,         0 )                                             // Disable segment boot strap.
DECLARE_ENUM_VAL( PLB6BC_CR0_BC_DSBS_i,         IBM_BIT_INDEX(32, PLB6BC_CR0_BC_DSBS_e) )       // Defaults to ‘0’ to allow the processors to boot from ROM, based on the external boot ROM segment decode bus, I_EBRSD[0:2].
DECLARE_ENUM_VAL( PLB6BC_CR0_BC_DSBS_n,         1 )                                             // Setting this bit to ‘1’ provides flexibility. For example, an external device can set up the boot code in main memory and boot from that location.
DECLARE_ENUM_VAL( PLB6BC_CR0_BC_MASK_MSRSP_e,   1 )                                             // Mask for external interrupt reporting of an error on BC_MSRSP
DECLARE_ENUM_VAL( PLB6BC_CR0_BC_MASK_MSRSP_i,   IBM_BIT_INDEX(32, PLB6BC_CR0_BC_MASK_MSRSP_e) )
DECLARE_ENUM_VAL( PLB6BC_CR0_BC_MASK_MSRSP_n,   1 )
DECLARE_ENUM_VAL( PLB6BC_CR0_BC_MASK_SRSNZ_e,   2 )                                             // Mask for external interrupt reporting of an error on BC_SRSNZ
DECLARE_ENUM_VAL( PLB6BC_CR0_BC_MASK_SRSNZ_i,   IBM_BIT_INDEX(32, PLB6BC_CR0_BC_MASK_SRSNZ_e) )
DECLARE_ENUM_VAL( PLB6BC_CR0_BC_MASK_SRSNZ_n,   1 )
END_ENUM( PLB6BC_CR0_FIELD )


BEGIN_ENUM( PLB6BC_PRI_FIELD )
DECLARE_ENUM_VAL( PLB6BC_PRI_MiP_n,                                 1 )
DECLARE_ENUM_VAL( PLB6BC_PRI_M0P_i, IBM_BIT_INDEX(32,  0) )
DECLARE_ENUM_VAL( PLB6BC_PRI_M1P_i, IBM_BIT_INDEX(32,  1) )
DECLARE_ENUM_VAL( PLB6BC_PRI_M2P_i, IBM_BIT_INDEX(32,  2) )
DECLARE_ENUM_VAL( PLB6BC_PRI_M3P_i, IBM_BIT_INDEX(32,  3) )
DECLARE_ENUM_VAL( PLB6BC_PRI_M4P_i, IBM_BIT_INDEX(32,  4) )
DECLARE_ENUM_VAL( PLB6BC_PRI_M5P_i, IBM_BIT_INDEX(32,  5) )
DECLARE_ENUM_VAL( PLB6BC_PRI_M6P_i, IBM_BIT_INDEX(32,  6) )
DECLARE_ENUM_VAL( PLB6BC_PRI_M7P_i, IBM_BIT_INDEX(32,  7) )
DECLARE_ENUM_VAL( PLB6BC_PRI_M8P_i, IBM_BIT_INDEX(32,  8) )
DECLARE_ENUM_VAL( PLB6BC_PRI_M9P_i, IBM_BIT_INDEX(32,  9) )
DECLARE_ENUM_VAL( PLB6BC_PRI_M10P_i,IBM_BIT_INDEX(32, 10) )
DECLARE_ENUM_VAL( PLB6BC_PRI_M11P_i,IBM_BIT_INDEX(32, 11) )
DECLARE_ENUM_VAL( PLB6BC_PRI_M12P_i,IBM_BIT_INDEX(32, 12) )
DECLARE_ENUM_VAL( PLB6BC_PRI_M13P_i,IBM_BIT_INDEX(32, 13) )
DECLARE_ENUM_VAL( PLB6BC_PRI_M14P_i,IBM_BIT_INDEX(32, 14) )
DECLARE_ENUM_VAL( PLB6BC_PRI_M15P_i,IBM_BIT_INDEX(32, 15) )
END_ENUM( PLB6BC_PRI_FIELD )

BEGIN_ENUM( PLB6BC_MiP )
DECLARE_ENUM_VAL( PLB6BC_MiP_Low,   0b0 )
DECLARE_ENUM_VAL( PLB6BC_MiP_High,  0b1 )
END_ENUM( PLB6BC_MiP )


BEGIN_ENUM( PLB6BC_TSNOOP_FIELD )
DECLARE_ENUM_VAL( PLB6BC_TSNOOP_e,  3 )                                     // Bus Controller TSNOOP clock count.
DECLARE_ENUM_VAL( PLB6BC_TSNOOP_i,  IBM_BIT_INDEX(32, PLB6BC_TSNOOP_e) )
DECLARE_ENUM_VAL( PLB6BC_TSNOOP_n,  4 )
END_ENUM( PLB6BC_TSNOOP_FIELD )

BEGIN_ENUM( PLB6BC_TSNOOP_N )               // Valid settings range from 4 (‘0100’) to 8 (‘1000’), and are set during synthesis.
DECLARE_ENUM_VAL( PLB6BC_TSNOOP_4, 0b0100 )
DECLARE_ENUM_VAL( PLB6BC_TSNOOP_5, 0b0101 )
DECLARE_ENUM_VAL( PLB6BC_TSNOOP_6, 0b0110 )
DECLARE_ENUM_VAL( PLB6BC_TSNOOP_7, 0b0111 )
DECLARE_ENUM_VAL( PLB6BC_TSNOOP_8, 0b1000 )
END_ENUM( PLB6BC_TSNOOP_N )


BEGIN_ENUM( PLB6BC_PAAM_WIN_EXT_FIELD )
DECLARE_ENUM_VAL( PLB6BC_PAAM_WIN_EXT_e,    1 )                                         // Bus Controller PAAM Window Extension register.
DECLARE_ENUM_VAL( PLB6BC_PAAM_WIN_EXT_i,    IBM_BIT_INDEX(32, PLB6BC_PAAM_WIN_EXT_e) )
DECLARE_ENUM_VAL( PLB6BC_PAAM_WIN_EXT_n,    2 )
END_ENUM( PLB6BC_PAAM_WIN_EXT_FIELD )

BEGIN_ENUM( PLB6BC_PAAM_WIN_EXT_N )             // Valid settings range from 1 - 3 (‘01’ - ‘11’) to represent 1 - 3 clock cycles respectively. If an invalid number is input, it defaults to three clocks.
DECLARE_ENUM_VAL( PLB6BC_PAAM_WIN_EXT_1, 0b01 )
DECLARE_ENUM_VAL( PLB6BC_PAAM_WIN_EXT_2, 0b10 )
DECLARE_ENUM_VAL( PLB6BC_PAAM_WIN_EXT_3, 0b11 )
END_ENUM( PLB6BC_PAAM_WIN_EXT_N )


BEGIN_ENUM( PLB6BC_SGDi_FIELD )
DECLARE_ENUM_VAL( PLB6BC_SGDi_e,    27 )                                // The value of this register determines the start address for the segment.
DECLARE_ENUM_VAL( PLB6BC_SGDi_i,    IBM_BIT_INDEX(32, PLB6BC_SGDi_e) )  // Note: There are seven BC_SGD registers, one for each segment from 1 to 7, with segment 0 set to a default value of 0x0000000.
DECLARE_ENUM_VAL( PLB6BC_SGDi_n,    28 )
END_ENUM( PLB6BC_SGDi_FIELD )


BEGIN_ENUM( PLB6BC_ERR_FIELD )
DECLARE_ENUM_VAL( PLB6BC_ERR_BC_NSRSP_e,    0 )                                         // No slave response per segment. When this bit is set to ‘1’, a slave device did not respond.
DECLARE_ENUM_VAL( PLB6BC_ERR_BC_NSRSP_i,    IBM_BIT_INDEX(32, PLB6BC_ERR_BC_NSRSP_e) )
DECLARE_ENUM_VAL( PLB6BC_ERR_BC_NSRSP_n,    1 )
DECLARE_ENUM_VAL( PLB6BC_ERR_BC_MSRSP_e,    1 )                                         // Multiple slaves responded per segment. When this bit is set, multiple slaves responded to the same address, which was not an addr_only command.
DECLARE_ENUM_VAL( PLB6BC_ERR_BC_MSRSP_i,    IBM_BIT_INDEX(32, PLB6BC_ERR_BC_MSRSP_e) )
DECLARE_ENUM_VAL( PLB6BC_ERR_BC_MSRSP_n,    1 )
DECLARE_ENUM_VAL( PLB6BC_ERR_BC_SRSNZ_e,    2 )                                         // Snoopable request to a segment other than zero. When this bit is set to ‘1’, a slave device other than zero was snooped.
DECLARE_ENUM_VAL( PLB6BC_ERR_BC_SRSNZ_i,    IBM_BIT_INDEX(32, PLB6BC_ERR_BC_SRSNZ_e) )
DECLARE_ENUM_VAL( PLB6BC_ERR_BC_SRSNZ_n,    1 )
DECLARE_ENUM_VAL( PLB6BC_ERR_BC_SREMID_e,   7 )                                         // Slave response error master ID. This bus stores the MID (O_Sn_MAST_ID[1:4]).
DECLARE_ENUM_VAL( PLB6BC_ERR_BC_SREMID_i,   IBM_BIT_INDEX(32, PLB6BC_ERR_BC_SREMID_e) )
DECLARE_ENUM_VAL( PLB6BC_ERR_BC_SREMID_n,   4 )
DECLARE_ENUM_VAL( PLB6BC_ERR_BC_SREMT_e,    12 )                                        // Slave response error Mtag. This bus stores the Mtag (O_Sn_MTAG[0:4]) bus.
DECLARE_ENUM_VAL( PLB6BC_ERR_BC_SREMT_i,    IBM_BIT_INDEX(32, PLB6BC_ERR_BC_SREMT_e) )
DECLARE_ENUM_VAL( PLB6BC_ERR_BC_SREMT_n,    5 )
DECLARE_ENUM_VAL( PLB6BC_ERR_BC_SRERD_e,    13 )                                        // Slave response error read. This bus stores the Rd/Wr (O_Sn_READ) bit.
DECLARE_ENUM_VAL( PLB6BC_ERR_BC_SRERD_i,    IBM_BIT_INDEX(32, PLB6BC_ERR_BC_SRERD_e) )
DECLARE_ENUM_VAL( PLB6BC_ERR_BC_SRERD_n,    1 )
DECLARE_ENUM_VAL( PLB6BC_ERR_BC_SEGID_e,    16 )                                        // Slave response error read. This bus stores the slave segment ID.
DECLARE_ENUM_VAL( PLB6BC_ERR_BC_SEGID_i,    IBM_BIT_INDEX(32, PLB6BC_ERR_BC_SEGID_e) )
DECLARE_ENUM_VAL( PLB6BC_ERR_BC_SEGID_n,    3 )
DECLARE_ENUM_VAL( PLB6BC_ERR_BC_SPVAL_e,    20 )                                        // Slave segment. Each bit [0:3] defines which of the slave devices responded to the same address, based on slave output signals,
DECLARE_ENUM_VAL( PLB6BC_ERR_BC_SPVAL_i,    IBM_BIT_INDEX(32, PLB6BC_ERR_BC_SPVAL_e) )
DECLARE_ENUM_VAL( PLB6BC_ERR_BC_SPVAL_n,    PLB6BC_SEGMENT_SLAVE_N )    // I_Snq_PVAL (where “n” represents the segment number and “q” represents the slave device number).
END_ENUM( PLB6BC_ERR_FIELD )

BEGIN_ENUM( PLB6BC_SRERD )
DECLARE_ENUM_VAL( PLB6BC_SRERD_Write,   0b0 )
DECLARE_ENUM_VAL( PLB6BC_SRERD_Read,    0b1 )
END_ENUM( PLB6BC_SRERD )


BEGIN_ENUM( PLB6BC_MSRSP_FIELD )
DECLARE_ENUM_VAL( PLB6BC_BC_Si_MSRSP_n,                                 PLB6BC_SEGMENT_SLAVE_N )

DECLARE_ENUM_VAL( PLB6BC_BC_S0_MSRSP_e,  3 ) // Multiple slave responded per segment.
DECLARE_ENUM_VAL( PLB6BC_BC_S1_MSRSP_e,  7 )
DECLARE_ENUM_VAL( PLB6BC_BC_S2_MSRSP_e, 11 )
DECLARE_ENUM_VAL( PLB6BC_BC_S3_MSRSP_e, 15 )
DECLARE_ENUM_VAL( PLB6BC_BC_S4_MSRSP_e, 19 )
DECLARE_ENUM_VAL( PLB6BC_BC_S5_MSRSP_e, 23 )
DECLARE_ENUM_VAL( PLB6BC_BC_S6_MSRSP_e, 27 )
DECLARE_ENUM_VAL( PLB6BC_BC_S7_MSRSP_e, 31 )
DECLARE_ENUM_VAL( PLB6BC_BC_S0_MSRSP_i, IBM_BIT_INDEX(32, PLB6BC_BC_S0_MSRSP_e) )
DECLARE_ENUM_VAL( PLB6BC_BC_S1_MSRSP_i, IBM_BIT_INDEX(32, PLB6BC_BC_S1_MSRSP_e) )
DECLARE_ENUM_VAL( PLB6BC_BC_S2_MSRSP_i, IBM_BIT_INDEX(32, PLB6BC_BC_S2_MSRSP_e) )
DECLARE_ENUM_VAL( PLB6BC_BC_S3_MSRSP_i, IBM_BIT_INDEX(32, PLB6BC_BC_S3_MSRSP_e) )
DECLARE_ENUM_VAL( PLB6BC_BC_S4_MSRSP_i, IBM_BIT_INDEX(32, PLB6BC_BC_S4_MSRSP_e) )
DECLARE_ENUM_VAL( PLB6BC_BC_S5_MSRSP_i, IBM_BIT_INDEX(32, PLB6BC_BC_S5_MSRSP_e) )
DECLARE_ENUM_VAL( PLB6BC_BC_S6_MSRSP_i, IBM_BIT_INDEX(32, PLB6BC_BC_S6_MSRSP_e) )
DECLARE_ENUM_VAL( PLB6BC_BC_S7_MSRSP_i, IBM_BIT_INDEX(32, PLB6BC_BC_S7_MSRSP_e) )
END_ENUM( PLB6BC_MSRSP_FIELD )


BEGIN_ENUM( PLB6BC_HCPP_FIELD )
DECLARE_ENUM_VAL( PLB6BC_HCPP_e,    31 )                                // Segment decrement count: 0x0000400 to 0x3FFFFFF. The value of this register determines the clock count pulse interval.
DECLARE_ENUM_VAL( PLB6BC_HCPP_i,    IBM_BIT_INDEX(32, PLB6BC_HCPP_e) )
DECLARE_ENUM_VAL( PLB6BC_HCPP_n,    26 )
END_ENUM( PLB6BC_HCPP_FIELD )

BEGIN_ENUM( PLB6BC_HCPP_VAL )
DECLARE_ENUM_VAL( PLB6BC_HCPP_MIN,  0x00000400 ) // Valid settings range from 1 K to (2^26 -1).
DECLARE_ENUM_VAL( PLB6BC_HCPP_1K,   0x00000000 ) // If a value less then 1 K (x‘000 0400’) is written to the Hang Count Pulse Period Register (BC_HCPP), it defaults to 1 K clocks (0x400).
DECLARE_ENUM_VAL( PLB6BC_HCPP_4K,   0x00001000 )
DECLARE_ENUM_VAL( PLB6BC_HCPP_16K,  0x00004000 )
DECLARE_ENUM_VAL( PLB6BC_HCPP_64K,  0x00010000 )
DECLARE_ENUM_VAL( PLB6BC_HCPP_256K, 0x00040000 )
DECLARE_ENUM_VAL( PLB6BC_HCPP_1M,   0x00100000 )
DECLARE_ENUM_VAL( PLB6BC_HCPP_4M,   0x00400000 )
DECLARE_ENUM_VAL( PLB6BC_HCPP_16M,  0x01000000 )
DECLARE_ENUM_VAL( PLB6BC_HCPP_MAX,  0x03FFFFFF )
END_ENUM( PLB6BC_HCPP_VAL )


BEGIN_ENUM( PLB6BC_HD_FIELD )
DECLARE_ENUM_VAL( PLB6BC_HD_e,  15 )                                // Hang detected. The appropriate master bit is set if a master generates an I_Mm_HANG_DETECT signal.
DECLARE_ENUM_VAL( PLB6BC_HD_i,  IBM_BIT_INDEX(32, PLB6BC_HD_e) )
DECLARE_ENUM_VAL( PLB6BC_HD_n,  PLB6BC_MASTER_N )
END_ENUM( PLB6BC_HD_FIELD )


BEGIN_ENUM( PLB6BC_QD_FIELD )
DECLARE_ENUM_VAL( PLB6BC_QD_e,  15 )                                // Quiesce detected. If an I_Mm_HANG_DETECT signal remains asserted for J or more clocks,
DECLARE_ENUM_VAL( PLB6BC_QD_i,  IBM_BIT_INDEX(32, PLB6BC_QD_e) )    // where J = H/2 for H < 256 K (2^18) and J = H/8 for all other values of H, the bus controller sets the appropriate
DECLARE_ENUM_VAL( PLB6BC_QD_n,  PLB6BC_MASTER_N )                   // master bit and the bus controller also asserts the O_M_QUIESCE signal
END_ENUM( PLB6BC_QD_FIELD )


BEGIN_ENUM( PLB6BC_SHD_FIELD )
DECLARE_ENUM_VAL( PLB6BC_SHD_e,  15 )                               // System hung detected. If the I_Mm_HANG_DETECT signal is active when the O_M_HANG_CNT_PULSE is asserted,
DECLARE_ENUM_VAL( PLB6BC_SHD_i,  IBM_BIT_INDEX(32, PLB6BC_SHD_e) )  // the appropriate master bit is set and the PLB6 Bus Controller asserts the O_SYSTEM_HUNG signal.
DECLARE_ENUM_VAL( PLB6BC_SHD_n,  PLB6BC_MASTER_N )
END_ENUM( PLB6BC_SHD_FIELD )


BEGIN_ENUM( PLB6BC_REVID_FIELD )
DECLARE_ENUM_VAL( PLB6BC_REVID_VNUMB_e,     23 )                                        // Version number.
DECLARE_ENUM_VAL( PLB6BC_REVID_VNUMB_i,     IBM_BIT_INDEX(32, PLB6BC_REVID_VNUMB_e) )
DECLARE_ENUM_VAL( PLB6BC_REVID_VNUMB_n,     12 )
DECLARE_ENUM_VAL( PLB6BC_REVID_RVNUMB_e,    31 )                                        // Revision number.
DECLARE_ENUM_VAL( PLB6BC_REVID_RVNUMB_i,    IBM_BIT_INDEX(32, PLB6BC_REVID_RVNUMB_e) )
DECLARE_ENUM_VAL( PLB6BC_REVID_RVNUMB_n,    8 )
END_ENUM( PLB6BC_REVID_FIELD )


#endif // __PLB6BC_REG_FIELD_H__
