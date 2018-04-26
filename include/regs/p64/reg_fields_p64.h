/**
 *
 */

#ifndef P64_REGS_FIELDS_H
#define P64_REGS_FIELDS_H

#include <platform/common_macros/common_macros.h>
#include <arch/ppc/ibm_bit_ordering_macros.h>

typedef enum P64_P64CR_FIELD
{
    P64CR_PRI       =   IBM_BIT_INDEX(32, 31),
    P64CR_SPEN      =   IBM_BIT_INDEX(32, 23),
    P64CR_OWEN      =   IBM_BIT_INDEX(32, 22),
    P64CR_IPC       =   IBM_BIT_INDEX(32, 21),
    P64CR_IFMID0    =   IBM_BIT_INDEX(32, 15),
    P64CR_IFMID1    =   IBM_BIT_INDEX(32, 10),
    P64CR_IFE0      =   IBM_BIT_INDEX(32,  5),
    P64CR_IFE1      =   IBM_BIT_INDEX(32,  4)
} P64_P64CR_FIELD;

#define P64CR_PRI_MASK      0b11
#define P64CR_SPEN_MASK     0b1
#define P64CR_OWEN_MASK     0b1
#define P64CR_IPC_MASK      0b1
#define P64CR_IFMIDi_MASK   0x1F
#define P64CR_IFEi_MASK     0b1

typedef enum P64CR_PRI_N
{
    P64CR_PRI_0    =   0b00,
    P64CR_PRI_1    =   0b01,
    P64CR_PRI_2    =   0b10,
    P64CR_PRI_3    =   0b11
} P64CR_PRI_N;

BEGIN_ENUM( P64_ESR_TESR_FIELD )

    DECLARE_ENUM_VAL( ESR_TESR_P6APE_e,                                                0 )
    DECLARE_ENUM_VAL( ESR_TESR_P6APE_n,                                                1 )
    DECLARE_ENUM_VAL( ESR_TESR_P6APE_i,     IBM_BIT_INDEX( 32, ESR_TESR_P6APE_e        ) )

    DECLARE_ENUM_VAL( ESR_TESR_P6BPE_e,                                                1 )
    DECLARE_ENUM_VAL( ESR_TESR_P6BPE_n,                                                1 )
    DECLARE_ENUM_VAL( ESR_TESR_P6BPE_i,     IBM_BIT_INDEX( 32, ESR_TESR_P6BPE_e        ) )

    DECLARE_ENUM_VAL( ESR_TESR_P4RPE_e,                                                2 )
    DECLARE_ENUM_VAL( ESR_TESR_P4RPE_n,                                                1 )
    DECLARE_ENUM_VAL( ESR_TESR_P4RPE_i,     IBM_BIT_INDEX( 32, ESR_TESR_P4RPE_e        ) )

    DECLARE_ENUM_VAL( ESR_TESR_P6WPE_e,                                                3 )
    DECLARE_ENUM_VAL( ESR_TESR_P6WPE_n,                                                1 )
    DECLARE_ENUM_VAL( ESR_TESR_P6WPE_i,     IBM_BIT_INDEX( 32, ESR_TESR_P6WPE_e        ) )

    DECLARE_ENUM_VAL( ESR_TESR_P4MRE_e,                                                4 )
    DECLARE_ENUM_VAL( ESR_TESR_P4MRE_n,                                                1 )
    DECLARE_ENUM_VAL( ESR_TESR_P4MRE_i,     IBM_BIT_INDEX( 32, ESR_TESR_P4MRE_e        ) )

    DECLARE_ENUM_VAL( ESR_TESR_P4MWE_e,                                                5 )
    DECLARE_ENUM_VAL( ESR_TESR_P4MWE_n,                                                1 )
    DECLARE_ENUM_VAL( ESR_TESR_P4MWE_i,     IBM_BIT_INDEX( 32, ESR_TESR_P4MWE_e        ) )

    DECLARE_ENUM_VAL( ESR_TESR_P6SWE_e,                                                6 )
    DECLARE_ENUM_VAL( ESR_TESR_P6SWE_n,                                                1 )
    DECLARE_ENUM_VAL( ESR_TESR_P6SWE_i,     IBM_BIT_INDEX( 32, ESR_TESR_P6SWE_e        ) )

    DECLARE_ENUM_VAL( ESR_TESR_P4WTO_e,                                                7 )
    DECLARE_ENUM_VAL( ESR_TESR_P4WTO_n,                                                1 )
    DECLARE_ENUM_VAL( ESR_TESR_P4WTO_i,     IBM_BIT_INDEX( 32, ESR_TESR_P4WTO_e        ) )

    DECLARE_ENUM_VAL( ESR_TESR_P4RTO_e,                                                8 )
    DECLARE_ENUM_VAL( ESR_TESR_P4RTO_n,                                                1 )
    DECLARE_ENUM_VAL( ESR_TESR_P4RTO_i,     IBM_BIT_INDEX( 32, ESR_TESR_P4RTO_e        ) )

    DECLARE_ENUM_VAL( ESR_TESR_P6RERR_e,                                               9 )
    DECLARE_ENUM_VAL( ESR_TESR_P6RERR_n,                                               1 )
    DECLARE_ENUM_VAL( ESR_TESR_P6RERR_i,    IBM_BIT_INDEX( 32, ESR_TESR_P6RERR_e       ) )

    DECLARE_ENUM_VAL( ESR_TESR_P6MID_e,                                               14 )
    DECLARE_ENUM_VAL( ESR_TESR_P6MID_n,                                                5 )
    DECLARE_ENUM_VAL( ESR_TESR_P6MID_i,     IBM_BIT_INDEX( 32, ESR_TESR_P6MID_e        ) )

    DECLARE_ENUM_VAL( ESR_TESR_P6MTAG_e,                                              19 )
    DECLARE_ENUM_VAL( ESR_TESR_P6MTAG_n,                                               5 )
    DECLARE_ENUM_VAL( ESR_TESR_P6MTAG_i,    IBM_BIT_INDEX( 32, ESR_TESR_P6MTAG_e       ) )

    DECLARE_ENUM_VAL( ESR_TESR_P6MIRQ_e,                                               31 )
    DECLARE_ENUM_VAL( ESR_TESR_P6MIRQ_n,                                               1 )
    DECLARE_ENUM_VAL( ESR_TESR_P6MIRQ_i,    IBM_BIT_INDEX( 32, ESR_TESR_P6MIRQ_e       ) )

END_ENUM( P64_ESR_TESR_FIELD )


#endif // P64_REGS_FIELDS_H
