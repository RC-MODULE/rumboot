#ifndef L2C_H
#define L2C_H

#include <stdbool.h>
#include <platform/regs/regs_l2c_l2.h>
#include <platform/regs/regs_l2c_pmu.h>
#include <platform/regs/fields/l2c_l2.h>


/*
 * TODO: need to change constants definition in common project manner
 */
//L2INTEN bits
typedef enum
{
    L2INTEN_EXTINT      = (1 << 11),
    L2INTEN_PLBINT0     = (1 << 10),
    L2INTEN_PLBINT1     = (1 << 9),
    L2INTEN_L2AINT0     = (1 << 8),
    L2INTEN_L2AINT1     = (1 << 7),
    L2INTEN_L2AINT2     = (1 << 6),
    L2INTEN_L1CINT0     = (1 << 5),
    L2INTEN_RACINT0     = (1 << 4),
    L2INTEN_WACINT0     = (1 << 3),
    L2INTEN_WACINT1     = (1 << 2),
    L2INTEN_WACINT2     = (1 << 1),
} L2INTEN_bits_t;

//L2MCKEN bits
BEGIN_ENUM( L2MCKEN_bits_t )
    DECLARE_ENUM_VAL( L2MCKEN_EXTMCK_e, 20 )
    DECLARE_ENUM_VAL( L2MCKEN_EXTMCK_i, IBM_BIT_INDEX( 32, L2MCKEN_EXTMCK_e ) )

    DECLARE_ENUM_VAL( L2MCKEN_PLBMCK0_e, 21 )
    DECLARE_ENUM_VAL( L2MCKEN_PLBMCK0_i, IBM_BIT_INDEX( 32, L2MCKEN_PLBMCK0_e ) )

    DECLARE_ENUM_VAL( L2MCKEN_PLBMCK1_e, 22 )
    DECLARE_ENUM_VAL( L2MCKEN_PLBMCK1_i, IBM_BIT_INDEX( 32, L2MCKEN_PLBMCK1_e ) )

    DECLARE_ENUM_VAL( L2MCKEN_L2AMCK0_e, 23 )
    DECLARE_ENUM_VAL( L2MCKEN_L2AMCK0_i, IBM_BIT_INDEX( 32, L2MCKEN_L2AMCK0_e ) )

    DECLARE_ENUM_VAL( L2MCKEN_L2AMCK1_e, 24 )
    DECLARE_ENUM_VAL( L2MCKEN_L2AMCK1_i, IBM_BIT_INDEX( 32, L2MCKEN_L2AMCK1_e ) )

    DECLARE_ENUM_VAL( L2MCKEN_L2AMCK2_e, 25 )
    DECLARE_ENUM_VAL( L2MCKEN_L2AMCK2_i, IBM_BIT_INDEX( 32, L2MCKEN_L2AMCK2_e ) )

    DECLARE_ENUM_VAL( L2MCKEN_L1CMCK0_e, 26 )
    DECLARE_ENUM_VAL( L2MCKEN_L1CMCK0_i, IBM_BIT_INDEX( 32, L2MCKEN_L1CMCK0_e ) )

    DECLARE_ENUM_VAL( L2MCKEN_RACMCK0_e, 27 )
    DECLARE_ENUM_VAL( L2MCKEN_RACMCK0_i, IBM_BIT_INDEX( 32, L2MCKEN_RACMCK0_e ) )

    DECLARE_ENUM_VAL( L2MCKEN_WACMCK0_e, 28 )
    DECLARE_ENUM_VAL( L2MCKEN_WACMCK0_i, IBM_BIT_INDEX( 32, L2MCKEN_WACMCK0_e ) )

    DECLARE_ENUM_VAL( L2MCKEN_WACMCK1_e, 29 )
    DECLARE_ENUM_VAL( L2MCKEN_WACMCK1_i, IBM_BIT_INDEX( 32, L2MCKEN_WACMCK1_e ) )

    DECLARE_ENUM_VAL( L2MCKEN_WACMCK2_e, 30 )
    DECLARE_ENUM_VAL( L2MCKEN_WACMCK2_i, IBM_BIT_INDEX( 32, L2MCKEN_WACMCK2_e ) )
END_ENUM( L2MCKEN_bits_t )

//L2PLBINTEN0 bits
typedef enum
{
    L2PLBINTEN0_IntvnDataPE0    = (1 << 31),
    L2PLBINTEN0_IntvnDataPE1    = (1 << 30),
    L2PLBINTEN0_IntvnDataPE2    = (1 << 29),
    L2PLBINTEN0_IntvnDataPE3    = (1 << 28),
    L2PLBINTEN0_IntvnDataPE4    = (1 << 27),
    L2PLBINTEN0_IntvnDataPE5    = (1 << 26),
    L2PLBINTEN0_IntvnDataPE6    = (1 << 25),
    L2PLBINTEN0_IntvnDataPE7    = (1 << 24),
    L2PLBINTEN0_IntvnDataPE8    = (1 << 23),
    L2PLBINTEN0_IntvnDataPE9    = (1 << 22),
    L2PLBINTEN0_IntvnDataPE10   = (1 << 21),
    L2PLBINTEN0_IntvnDataPE11   = (1 << 20),
    L2PLBINTEN0_IntvnDataPE12   = (1 << 19),
    L2PLBINTEN0_IntvnDataPE13   = (1 << 18),
    L2PLBINTEN0_IntvnDataPE14   = (1 << 17),
    L2PLBINTEN0_IntvnDataPE15   = (1 << 16),
    L2PLBINTEN0_WrDataPE0       = (1 << 15),
    L2PLBINTEN0_WrDataPE1       = (1 << 14),
    L2PLBINTEN0_WrDataPE2       = (1 << 13),
    L2PLBINTEN0_WrDataPE3       = (1 << 12),
    L2PLBINTEN0_WrDataPE4       = (1 << 11),
    L2PLBINTEN0_WrDataPE5       = (1 << 10),
    L2PLBINTEN0_WrDataPE6       = (1 << 9),
    L2PLBINTEN0_WrDataPE7       = (1 << 8),
    L2PLBINTEN0_WrDataPE8       = (1 << 7),
    L2PLBINTEN0_WrDataPE9       = (1 << 6),
    L2PLBINTEN0_WrDataPE10      = (1 << 5),
    L2PLBINTEN0_WrDataPE11      = (1 << 4),
    L2PLBINTEN0_WrDataPE12      = (1 << 3),
    L2PLBINTEN0_WrDataPE13      = (1 << 2),
    L2PLBINTEN0_WrDataPE14      = (1 << 1),
    L2PLBINTEN0_WrDataPE15      = (1)
} L2PLBINTEN0_bits_t;

//L2PLBMCKEN0 bits
typedef enum
{
    L2PLBMCKEN0_IntvnDataPE0    = (1 << 31),
    L2PLBMCKEN0_IntvnDataPE1    = (1 << 30),
    L2PLBMCKEN0_IntvnDataPE2    = (1 << 29),
    L2PLBMCKEN0_IntvnDataPE3    = (1 << 28),
    L2PLBMCKEN0_IntvnDataPE4    = (1 << 27),
    L2PLBMCKEN0_IntvnDataPE5    = (1 << 26),
    L2PLBMCKEN0_IntvnDataPE6    = (1 << 25),
    L2PLBMCKEN0_IntvnDataPE7    = (1 << 24),
    L2PLBMCKEN0_IntvnDataPE8    = (1 << 23),
    L2PLBMCKEN0_IntvnDataPE9    = (1 << 22),
    L2PLBMCKEN0_IntvnDataPE10   = (1 << 21),
    L2PLBMCKEN0_IntvnDataPE11   = (1 << 20),
    L2PLBMCKEN0_IntvnDataPE12   = (1 << 19),
    L2PLBMCKEN0_IntvnDataPE13   = (1 << 18),
    L2PLBMCKEN0_IntvnDataPE14   = (1 << 17),
    L2PLBMCKEN0_IntvnDataPE15   = (1 << 16),
    L2PLBMCKEN0_WrDataPE0       = (1 << 15),
    L2PLBMCKEN0_WrDataPE1       = (1 << 14),
    L2PLBMCKEN0_WrDataPE2       = (1 << 13),
    L2PLBMCKEN0_WrDataPE3       = (1 << 12),
    L2PLBMCKEN0_WrDataPE4       = (1 << 11),
    L2PLBMCKEN0_WrDataPE5       = (1 << 10),
    L2PLBMCKEN0_WrDataPE6       = (1 << 9),
    L2PLBMCKEN0_WrDataPE7       = (1 << 8),
    L2PLBMCKEN0_WrDataPE8       = (1 << 7),
    L2PLBMCKEN0_WrDataPE9       = (1 << 6),
    L2PLBMCKEN0_WrDataPE10      = (1 << 5),
    L2PLBMCKEN0_WrDataPE11      = (1 << 4),
    L2PLBMCKEN0_WrDataPE12      = (1 << 3),
    L2PLBMCKEN0_WrDataPE13      = (1 << 2),
    L2PLBMCKEN0_WrDataPE14      = (1 << 1),
    L2PLBMCKEN0_WrDataPE15      = (1)
} L2PLBMCKEN0_bits_t;

//L2PLBFRC0 bits
typedef enum
{
    L2PLBFRC0_IntvnDataPE0      = (1 << 31),
    L2PLBFRC0_IntvnDataPE1      = (1 << 30),
    L2PLBFRC0_IntvnDataPE2      = (1 << 29),
    L2PLBFRC0_IntvnDataPE3      = (1 << 28),
    L2PLBFRC0_IntvnDataPE4      = (1 << 27),
    L2PLBFRC0_IntvnDataPE5      = (1 << 26),
    L2PLBFRC0_IntvnDataPE6      = (1 << 25),
    L2PLBFRC0_IntvnDataPE7      = (1 << 24),
    L2PLBFRC0_IntvnDataPE8      = (1 << 23),
    L2PLBFRC0_IntvnDataPE9      = (1 << 22),
    L2PLBFRC0_IntvnDataPE10     = (1 << 21),
    L2PLBFRC0_IntvnDataPE11     = (1 << 20),
    L2PLBFRC0_IntvnDataPE12     = (1 << 19),
    L2PLBFRC0_IntvnDataPE13     = (1 << 18),
    L2PLBFRC0_IntvnDataPE14     = (1 << 17),
    L2PLBFRC0_IntvnDataPE15     = (1 << 16),
    L2PLBFRC0_WrDataPE0         = (1 << 15),
    L2PLBFRC0_WrDataPE1         = (1 << 14),
    L2PLBFRC0_WrDataPE2         = (1 << 13),
    L2PLBFRC0_WrDataPE3         = (1 << 12),
    L2PLBFRC0_WrDataPE4         = (1 << 11),
    L2PLBFRC0_WrDataPE5         = (1 << 10),
    L2PLBFRC0_WrDataPE6         = (1 << 9),
    L2PLBFRC0_WrDataPE7         = (1 << 8),
    L2PLBFRC0_WrDataPE8         = (1 << 7),
    L2PLBFRC0_WrDataPE9         = (1 << 6),
    L2PLBFRC0_WrDataPE10        = (1 << 5),
    L2PLBFRC0_WrDataPE11        = (1 << 4),
    L2PLBFRC0_WrDataPE12        = (1 << 3),
    L2PLBFRC0_WrDataPE13        = (1 << 2),
    L2PLBFRC0_WrDataPE14        = (1 << 1),
    L2PLBFRC0_WrDataPE15        = (1)
} L2PLBFRC0_bits_t;

//PMUIE0 bits
typedef enum
{
    PMUIE0_IE0      = (1 << 31),
    PMUIE0_IE1      = (1 << 30),
    PMUIE0_IE2      = (1 << 29),
    PMUIE0_IE3      = (1 << 28),
    PMUIE0_IE4      = (1 << 27),
    PMUIE0_IE5      = (1 << 26),
    PMUIE0_IE6      = (1 << 25),
    PMUIE0_IE7      = (1 << 24),
    PMUIE0_IE8      = (1 << 23),
    PMUIE0_IE9      = (1 << 22),
    PMUIE0_IE10     = (1 << 21),
    PMUIE0_IE11     = (1 << 20),
    PMUIE0_IE12     = (1 << 19),
    PMUIE0_IE13     = (1 << 18),
    PMUIE0_IE14     = (1 << 17),
    PMUIE0_IE15     = (1 << 16)
} PMUIE0_bits_t;

//PMULCX bits
typedef enum
{
    PMULCX_CMODE    = (1 << 24),
    PMULCX_FC0      = (1 << 21),
    PMULCX_FCS      = (1 << 20),
    PMULCX_FCU      = (1 << 19),
    PMULCX_FCM1     = (1 << 18),
    PMULCX_FCM0     = (1 << 17),
    PMULCX_CE       = (1 << 16)
} PMULCX_bits_t;

//L2ARRSTAT1 bits
BEGIN_ENUM( L2ARRSTAT1_bits_t )
    DECLARE_ENUM_VAL( L2ARRSTAT1_MultiHit_e, 19 ) //Multiple hits have been detected in the L2 tag array.
    DECLARE_ENUM_VAL( L2ARRSTAT1_MultiHit_i, IBM_BIT_INDEX( 32, L2ARRSTAT1_MultiHit_e ) )

    DECLARE_ENUM_VAL( L2ARRSTAT1_TagUE0_e, 20 ) //L2 cache tag array uncorrectable error way 0 (includes data and check bits).
    DECLARE_ENUM_VAL( L2ARRSTAT1_TagUE0_i, IBM_BIT_INDEX( 32, L2ARRSTAT1_TagUE0_e ) )

    DECLARE_ENUM_VAL( L2ARRSTAT1_TagUE1_e, 21 ) //L2 cache tag array uncorrectable error way 1 (includes data and check bits).
    DECLARE_ENUM_VAL( L2ARRSTAT1_TagUE1_i, IBM_BIT_INDEX( 32, L2ARRSTAT1_TagUE1_e ) )

    DECLARE_ENUM_VAL( L2ARRSTAT1_TagUE2_e, 22 ) //L2 cache tag array uncorrectable error way 2 (includes data and check bits).
    DECLARE_ENUM_VAL( L2ARRSTAT1_TagUE2_i, IBM_BIT_INDEX( 32, L2ARRSTAT1_TagUE2_e ) )

    DECLARE_ENUM_VAL( L2ARRSTAT1_TagUE3_e, 23 ) //L2 cache tag array uncorrectable error way 3 (includes data and check bits).
    DECLARE_ENUM_VAL( L2ARRSTAT1_TagUE3_i, IBM_BIT_INDEX( 32, L2ARRSTAT1_TagUE3_e ) )

    DECLARE_ENUM_VAL( L2ARRSTAT1_DataUE0_e, 24 ) //L2 cache data array uncorrectable error doubleword 0 (includes data and check bits).
    DECLARE_ENUM_VAL( L2ARRSTAT1_DataUE0_i, IBM_BIT_INDEX( 32, L2ARRSTAT1_DataUE0_e ) )

    DECLARE_ENUM_VAL( L2ARRSTAT1_DataUE1_e, 25 ) //L2 cache data array uncorrectable error doubleword 1 (includes data and check bits).
    DECLARE_ENUM_VAL( L2ARRSTAT1_DataUE1_i, IBM_BIT_INDEX( 32, L2ARRSTAT1_DataUE1_e ) )

    DECLARE_ENUM_VAL( L2ARRSTAT1_DataUE2_e, 26 ) //L2 cache data array uncorrectable error doubleword 2 (includes data and check bits).
    DECLARE_ENUM_VAL( L2ARRSTAT1_DataUE2_i, IBM_BIT_INDEX( 32, L2ARRSTAT1_DataUE2_e ) )

    DECLARE_ENUM_VAL( L2ARRSTAT1_DataUE3_e, 27 ) //L2 cache data array uncorrectable error doubleword 3 (includes data and check bits).
    DECLARE_ENUM_VAL( L2ARRSTAT1_DataUE3_i, IBM_BIT_INDEX( 32, L2ARRSTAT1_DataUE3_e ) )

    DECLARE_ENUM_VAL( L2ARRSTAT1_DataUE4_e, 28 ) //L2 cache data array uncorrectable error doubleword 4 (includes data and check bits).
    DECLARE_ENUM_VAL( L2ARRSTAT1_DataUE4_i, IBM_BIT_INDEX( 32, L2ARRSTAT1_DataUE4_e ) )

    DECLARE_ENUM_VAL( L2ARRSTAT1_DataUE5_e, 29 ) //L2 cache data array uncorrectable error doubleword 5 (includes data and check bits).
    DECLARE_ENUM_VAL( L2ARRSTAT1_DataUE5_i, IBM_BIT_INDEX( 32, L2ARRSTAT1_DataUE5_e ) )

    DECLARE_ENUM_VAL( L2ARRSTAT1_DataUE6_e, 30 ) //L2 cache data array uncorrectable error doubleword 6 (includes data and check bits).
    DECLARE_ENUM_VAL( L2ARRSTAT1_DataUE6_i, IBM_BIT_INDEX( 32, L2ARRSTAT1_DataUE6_e ) )

    DECLARE_ENUM_VAL( L2ARRSTAT1_DataUE7_e, 31 ) //L2 cache data array uncorrectable error doubleword 7 (includes data and check bits).
    DECLARE_ENUM_VAL( L2ARRSTAT1_DataUE7_i, IBM_BIT_INDEX( 32, L2ARRSTAT1_DataUE7_e ) )
END_ENUM( L2ARRSTAT1_bits_t )

//L2ARRSTAT2 bits
BEGIN_ENUM( L2ARRSTAT2_bits_t )
    DECLARE_ENUM_VAL( L2ARRSTAT2_LruPE0_e, 8 ) //L2 cache LRU array parity error 0.
    DECLARE_ENUM_VAL( L2ARRSTAT2_LruPE0_i, IBM_BIT_INDEX( 32, L2ARRSTAT2_LruPE0_e ) )

    DECLARE_ENUM_VAL( L2ARRSTAT2_LruPE1_e, 9 ) //L2 cache LRU array parity error 1.
    DECLARE_ENUM_VAL( L2ARRSTAT2_LruPE1_i, IBM_BIT_INDEX( 32, L2ARRSTAT2_LruPE1_e ) )

    DECLARE_ENUM_VAL( L2ARRSTAT2_LruPE2_e, 10 ) //L2 cache LRU array parity error 2.
    DECLARE_ENUM_VAL( L2ARRSTAT2_LruPE2_i, IBM_BIT_INDEX( 32, L2ARRSTAT2_LruPE2_e ) )

    DECLARE_ENUM_VAL( L2ARRSTAT2_LruPE3_e, 11 ) //L2 cache LRU array parity error 3.
    DECLARE_ENUM_VAL( L2ARRSTAT2_LruPE3_i, IBM_BIT_INDEX( 32, L2ARRSTAT2_LruPE3_e ) )

    DECLARE_ENUM_VAL( L2ARRSTAT2_LruPE4_e, 12 ) //L2 cache LRU array parity error 4.
    DECLARE_ENUM_VAL( L2ARRSTAT2_LruPE4_i, IBM_BIT_INDEX( 32, L2ARRSTAT2_LruPE4_e ) )

    DECLARE_ENUM_VAL( L2ARRSTAT2_LruPE5_e, 13 ) //L2 cache LRU array parity error 5.
    DECLARE_ENUM_VAL( L2ARRSTAT2_LruPE5_i, IBM_BIT_INDEX( 32, L2ARRSTAT2_LruPE5_e ) )

    DECLARE_ENUM_VAL( L2ARRSTAT2_LruPE6_e, 14 ) //L2 cache LRU array parity error 6.
    DECLARE_ENUM_VAL( L2ARRSTAT2_LruPE6_i, IBM_BIT_INDEX( 32, L2ARRSTAT2_LruPE6_e ) )

    DECLARE_ENUM_VAL( L2ARRSTAT2_LruPE7_e, 15 ) //L2 cache LRU array parity error 7.
    DECLARE_ENUM_VAL( L2ARRSTAT2_LruPE7_i, IBM_BIT_INDEX( 32, L2ARRSTAT2_LruPE7_e ) )

    DECLARE_ENUM_VAL( L2ARRSTAT2_LruPE8_e, 16 ) //L2 cache LRU array parity error 8.
    DECLARE_ENUM_VAL( L2ARRSTAT2_LruPE8_i, IBM_BIT_INDEX( 32, L2ARRSTAT2_LruPE8_e ) )

    DECLARE_ENUM_VAL( L2ARRSTAT2_LruPE9_e, 17 ) //L2 cache LRU array parity error 9.
    DECLARE_ENUM_VAL( L2ARRSTAT2_LruPE9_i, IBM_BIT_INDEX( 32, L2ARRSTAT2_LruPE9_e ) )

    DECLARE_ENUM_VAL( L2ARRSTAT2_LruPE10_e, 18 ) //L2 cache LRU array parity error 10.
    DECLARE_ENUM_VAL( L2ARRSTAT2_LruPE10_i, IBM_BIT_INDEX( 32, L2ARRSTAT2_LruPE10_e ) )

    DECLARE_ENUM_VAL( L2ARRSTAT2_LruPE11_e, 19 ) //L2 cache LRU array parity error 11.
    DECLARE_ENUM_VAL( L2ARRSTAT2_LruPE11_i, IBM_BIT_INDEX( 32, L2ARRSTAT2_LruPE11_e ) )

    DECLARE_ENUM_VAL( L2ARRSTAT2_TagCE0_e, 20 ) //L2 cache tag array correctable error way 0 (includes data and check bits).
    DECLARE_ENUM_VAL( L2ARRSTAT2_TagCE0_i, IBM_BIT_INDEX( 32, L2ARRSTAT2_TagCE0_e ) )

    DECLARE_ENUM_VAL( L2ARRSTAT2_TagCE1_e, 21 ) //L2 cache tag array correctable error way 1 (includes data and check bits).
    DECLARE_ENUM_VAL( L2ARRSTAT2_TagCE1_i, IBM_BIT_INDEX( 32, L2ARRSTAT2_TagCE1_e ) )

    DECLARE_ENUM_VAL( L2ARRSTAT2_TagCE2_e, 22 ) //L2 cache tag array correctable error way 2 (includes data and check bits).
    DECLARE_ENUM_VAL( L2ARRSTAT2_TagCE2_i, IBM_BIT_INDEX( 32, L2ARRSTAT2_TagCE2_e ) )

    DECLARE_ENUM_VAL( L2ARRSTAT2_TagCE3_e, 23 ) //L2 cache tag array correctable error way 3 (includes data and check bits).
    DECLARE_ENUM_VAL( L2ARRSTAT2_TagCE3_i, IBM_BIT_INDEX( 32, L2ARRSTAT2_TagCE3_e ) )

    DECLARE_ENUM_VAL( L2ARRSTAT2_DataCE0_e, 24 ) //L2 cache data array correctable error doubleword 0 (includes data and check bits).
    DECLARE_ENUM_VAL( L2ARRSTAT2_DataCE0_i, IBM_BIT_INDEX( 32, L2ARRSTAT2_DataCE0_e ) )

    DECLARE_ENUM_VAL( L2ARRSTAT2_DataCE1_e, 25 ) //L2 cache data array correctable error doubleword 1 (includes data and check bits).
    DECLARE_ENUM_VAL( L2ARRSTAT2_DataCE1_i, IBM_BIT_INDEX( 32, L2ARRSTAT2_DataCE1_e ) )

    DECLARE_ENUM_VAL( L2ARRSTAT2_DataCE2_e, 26 ) //L2 cache data array correctable error doubleword 2 (includes data and check bits).
    DECLARE_ENUM_VAL( L2ARRSTAT2_DataCE2_i, IBM_BIT_INDEX( 32, L2ARRSTAT2_DataCE2_e ) )

    DECLARE_ENUM_VAL( L2ARRSTAT2_DataCE3_e, 27 ) //L2 cache data array correctable error doubleword 3 (includes data and check bits).
    DECLARE_ENUM_VAL( L2ARRSTAT2_DataCE3_i, IBM_BIT_INDEX( 32, L2ARRSTAT2_DataCE3_e ) )

    DECLARE_ENUM_VAL( L2ARRSTAT2_DataCE4_e, 28 ) //L2 cache data array correctable error doubleword 4 (includes data and check bits).
    DECLARE_ENUM_VAL( L2ARRSTAT2_DataCE4_i, IBM_BIT_INDEX( 32, L2ARRSTAT2_DataCE4_e ) )

    DECLARE_ENUM_VAL( L2ARRSTAT2_DataCE5_e, 29 ) //L2 cache data array correctable error doubleword 5 (includes data and check bits).
    DECLARE_ENUM_VAL( L2ARRSTAT2_DataCE5_i, IBM_BIT_INDEX( 32, L2ARRSTAT2_DataCE5_e ) )

    DECLARE_ENUM_VAL( L2ARRSTAT2_DataCE6_e, 30 ) //L2 cache data array correctable error doubleword 6 (includes data and check bits).
    DECLARE_ENUM_VAL( L2ARRSTAT2_DataCE6_i, IBM_BIT_INDEX( 32, L2ARRSTAT2_DataCE6_e ) )

    DECLARE_ENUM_VAL( L2ARRSTAT2_DataCE7_e, 31 ) //L2 cache data array correctable error doubleword 7 (includes data and check bits).
    DECLARE_ENUM_VAL( L2ARRSTAT2_DataCE7_i, IBM_BIT_INDEX( 32, L2ARRSTAT2_DataCE7_e ) )
END_ENUM( L2ARRSTAT2_bits_t )

//L2ARRMCKEN1 bits
BEGIN_ENUM( L2ARRMCKEN1_bits_t )
    DECLARE_ENUM_VAL( L2ARRMCKEN1_MultiHit_e, 19 ) //Multiple hits have been detected in the L2 tag array.
    DECLARE_ENUM_VAL( L2ARRMCKEN1_MultiHit_i, IBM_BIT_INDEX( 32, L2ARRMCKEN1_MultiHit_e ) )

    DECLARE_ENUM_VAL( L2ARRMCKEN1_TagUE0_e, 20 ) //L2 cache tag array uncorrectable error way 0
    DECLARE_ENUM_VAL( L2ARRMCKEN1_TagUE0_i, IBM_BIT_INDEX( 32, L2ARRMCKEN1_TagUE0_e ) )

    DECLARE_ENUM_VAL( L2ARRMCKEN1_TagUE1_e, 21 ) //L2 cache tag array uncorrectable error way 1
    DECLARE_ENUM_VAL( L2ARRMCKEN1_TagUE1_i, IBM_BIT_INDEX( 32, L2ARRMCKEN1_TagUE1_e ) )

    DECLARE_ENUM_VAL( L2ARRMCKEN1_TagUE2_e, 22 ) //L2 cache tag array uncorrectable error way 2
    DECLARE_ENUM_VAL( L2ARRMCKEN1_TagUE2_i, IBM_BIT_INDEX( 32, L2ARRMCKEN1_TagUE2_e ) )

    DECLARE_ENUM_VAL( L2ARRMCKEN1_TagUE3_e, 23 ) //L2 cache tag array uncorrectable error way 3
    DECLARE_ENUM_VAL( L2ARRMCKEN1_TagUE3_i, IBM_BIT_INDEX( 32, L2ARRMCKEN1_TagUE3_e ) )

    DECLARE_ENUM_VAL( L2ARRMCKEN1_DataUE0_e, 24 ) //L2 cache data array uncorrectable error doubleword 0
    DECLARE_ENUM_VAL( L2ARRMCKEN1_DataUE0_i, IBM_BIT_INDEX( 32, L2ARRMCKEN1_DataUE0_e ) )

    DECLARE_ENUM_VAL( L2ARRMCKEN1_DataUE1_e, 25 ) //L2 cache data array uncorrectable error doubleword 1
    DECLARE_ENUM_VAL( L2ARRMCKEN1_DataUE1_i, IBM_BIT_INDEX( 32, L2ARRMCKEN1_DataUE1_e ) )

    DECLARE_ENUM_VAL( L2ARRMCKEN1_DataUE2_e, 26 ) //L2 cache data array uncorrectable error doubleword 2
    DECLARE_ENUM_VAL( L2ARRMCKEN1_DataUE2_i, IBM_BIT_INDEX( 32, L2ARRMCKEN1_DataUE2_e ) )

    DECLARE_ENUM_VAL( L2ARRMCKEN1_DataUE3_e, 27 ) //L2 cache data array uncorrectable error doubleword 3
    DECLARE_ENUM_VAL( L2ARRMCKEN1_DataUE3_i, IBM_BIT_INDEX( 32, L2ARRMCKEN1_DataUE3_e ) )

    DECLARE_ENUM_VAL( L2ARRMCKEN1_DataUE4_e, 28 ) //L2 cache data array uncorrectable error doubleword 4
    DECLARE_ENUM_VAL( L2ARRMCKEN1_DataUE4_i, IBM_BIT_INDEX( 32, L2ARRMCKEN1_DataUE4_e ) )

    DECLARE_ENUM_VAL( L2ARRMCKEN1_DataUE5_e, 29 ) //L2 cache data array uncorrectable error doubleword 5
    DECLARE_ENUM_VAL( L2ARRMCKEN1_DataUE5_i, IBM_BIT_INDEX( 32, L2ARRMCKEN1_DataUE5_e ) )

    DECLARE_ENUM_VAL( L2ARRMCKEN1_DataUE6_e, 30 ) //L2 cache data array uncorrectable error doubleword 6
    DECLARE_ENUM_VAL( L2ARRMCKEN1_DataUE6_i, IBM_BIT_INDEX( 32, L2ARRMCKEN1_DataUE6_e ) )

    DECLARE_ENUM_VAL( L2ARRMCKEN1_DataUE7_e, 31 ) //L2 cache data array uncorrectable error doubleword 7
    DECLARE_ENUM_VAL( L2ARRMCKEN1_DataUE7_i, IBM_BIT_INDEX( 32, L2ARRMCKEN1_DataUE7_e ) )
END_ENUM( L2ARRMCKEN1_bits_t )

//L2ARRMCKEN2 bits
BEGIN_ENUM( L2ARRMCKEN2_bits_t )
    DECLARE_ENUM_VAL( L2ARRMCKEN2_LruPE0_e, 8 ) //L2 cache LRU array parity error 0.
    DECLARE_ENUM_VAL( L2ARRMCKEN2_LruPE0_i, IBM_BIT_INDEX( 32, L2ARRMCKEN2_LruPE0_e ) )

    DECLARE_ENUM_VAL( L2ARRMCKEN2_LruPE1_e, 9 ) //L2 cache LRU array parity error 1.
    DECLARE_ENUM_VAL( L2ARRMCKEN2_LruPE1_i, IBM_BIT_INDEX( 32, L2ARRMCKEN2_LruPE1_e ) )

    DECLARE_ENUM_VAL( L2ARRMCKEN2_LruPE2_e, 10 ) //L2 cache LRU array parity error 2.
    DECLARE_ENUM_VAL( L2ARRMCKEN2_LruPE2_i, IBM_BIT_INDEX( 32, L2ARRMCKEN2_LruPE2_e ) )

    DECLARE_ENUM_VAL( L2ARRMCKEN2_LruPE3_e, 11 ) //L2 cache LRU array parity error 3.
    DECLARE_ENUM_VAL( L2ARRMCKEN2_LruPE3_i, IBM_BIT_INDEX( 32, L2ARRMCKEN2_LruPE3_e ) )

    DECLARE_ENUM_VAL( L2ARRMCKEN2_LruPE4_e, 12 ) //L2 cache LRU array parity error 4.
    DECLARE_ENUM_VAL( L2ARRMCKEN2_LruPE4_i, IBM_BIT_INDEX( 32, L2ARRMCKEN2_LruPE4_e ) )

    DECLARE_ENUM_VAL( L2ARRMCKEN2_LruPE5_e, 13 ) //L2 cache LRU array parity error 5.
    DECLARE_ENUM_VAL( L2ARRMCKEN2_LruPE5_i, IBM_BIT_INDEX( 32, L2ARRMCKEN2_LruPE5_e ) )

    DECLARE_ENUM_VAL( L2ARRMCKEN2_LruPE6_e, 14 ) //L2 cache LRU array parity error 6.
    DECLARE_ENUM_VAL( L2ARRMCKEN2_LruPE6_i, IBM_BIT_INDEX( 32, L2ARRMCKEN2_LruPE6_e ) )

    DECLARE_ENUM_VAL( L2ARRMCKEN2_LruPE7_e, 15 ) //L2 cache LRU array parity error 7.
    DECLARE_ENUM_VAL( L2ARRMCKEN2_LruPE7_i, IBM_BIT_INDEX( 32, L2ARRMCKEN2_LruPE7_e ) )

    DECLARE_ENUM_VAL( L2ARRMCKEN2_LruPE8_e, 16 ) //L2 cache LRU array parity error 8.
    DECLARE_ENUM_VAL( L2ARRMCKEN2_LruPE8_i, IBM_BIT_INDEX( 32, L2ARRMCKEN2_LruPE8_e ) )

    DECLARE_ENUM_VAL( L2ARRMCKEN2_LruPE9_e, 17 ) //L2 cache LRU array parity error 9.
    DECLARE_ENUM_VAL( L2ARRMCKEN2_LruPE9_i, IBM_BIT_INDEX( 32, L2ARRMCKEN2_LruPE9_e ) )

    DECLARE_ENUM_VAL( L2ARRMCKEN2_LruPE10_e, 18 ) //L2 cache LRU array parity error 10.
    DECLARE_ENUM_VAL( L2ARRMCKEN2_LruPE10_i, IBM_BIT_INDEX( 32, L2ARRMCKEN2_LruPE10_e ) )

    DECLARE_ENUM_VAL( L2ARRMCKEN2_LruPE11_e, 19 ) //L2 cache LRU array parity error 11.
    DECLARE_ENUM_VAL( L2ARRMCKEN2_LruPE11_i, IBM_BIT_INDEX( 32, L2ARRMCKEN2_LruPE11_e ) )

    DECLARE_ENUM_VAL( L2ARRMCKEN2_TagCE0_e, 20 ) //L2 cache tag array correctable error way 0
    DECLARE_ENUM_VAL( L2ARRMCKEN2_TagCE0_i, IBM_BIT_INDEX( 32, L2ARRMCKEN2_TagCE0_e ) )

    DECLARE_ENUM_VAL( L2ARRMCKEN2_TagCE1_e, 21 ) //L2 cache tag array correctable error way 1
    DECLARE_ENUM_VAL( L2ARRMCKEN2_TagCE1_i, IBM_BIT_INDEX( 32, L2ARRMCKEN2_TagCE1_e ) )

    DECLARE_ENUM_VAL( L2ARRMCKEN2_TagCE2_e, 22 ) //L2 cache tag array correctable error way 2
    DECLARE_ENUM_VAL( L2ARRMCKEN2_TagCE2_i, IBM_BIT_INDEX( 32, L2ARRMCKEN2_TagCE2_e ) )

    DECLARE_ENUM_VAL( L2ARRMCKEN2_TagCE3_e, 23 ) //L2 cache tag array correctable error way 3
    DECLARE_ENUM_VAL( L2ARRMCKEN2_TagCE3_i, IBM_BIT_INDEX( 32, L2ARRMCKEN2_TagCE3_e ) )

    DECLARE_ENUM_VAL( L2ARRMCKEN2_DataCE0_e, 24 ) //L2 cache data array correctable error doubleword 0
    DECLARE_ENUM_VAL( L2ARRMCKEN2_DataCE0_i, IBM_BIT_INDEX( 32, L2ARRMCKEN2_DataCE0_e ) )

    DECLARE_ENUM_VAL( L2ARRMCKEN2_DataCE1_e, 25 ) //L2 cache data array correctable error doubleword 1
    DECLARE_ENUM_VAL( L2ARRMCKEN2_DataCE1_i, IBM_BIT_INDEX( 32, L2ARRMCKEN2_DataCE1_e ) )

    DECLARE_ENUM_VAL( L2ARRMCKEN2_DataCE2_e, 26 ) //L2 cache data array correctable error doubleword 2
    DECLARE_ENUM_VAL( L2ARRMCKEN2_DataCE2_i, IBM_BIT_INDEX( 32, L2ARRMCKEN2_DataCE2_e ) )

    DECLARE_ENUM_VAL( L2ARRMCKEN2_DataCE3_e, 27 ) //L2 cache data array correctable error doubleword 3
    DECLARE_ENUM_VAL( L2ARRMCKEN2_DataCE3_i, IBM_BIT_INDEX( 32, L2ARRMCKEN2_DataCE3_e ) )

    DECLARE_ENUM_VAL( L2ARRMCKEN2_DataCE4_e, 28 ) //L2 cache data array correctable error doubleword 4
    DECLARE_ENUM_VAL( L2ARRMCKEN2_DataCE4_i, IBM_BIT_INDEX( 32, L2ARRMCKEN2_DataCE4_e ) )

    DECLARE_ENUM_VAL( L2ARRMCKEN2_DataCE5_e, 29 ) //L2 cache data array correctable error doubleword 5
    DECLARE_ENUM_VAL( L2ARRMCKEN2_DataCE5_i, IBM_BIT_INDEX( 32, L2ARRMCKEN2_DataCE5_e ) )

    DECLARE_ENUM_VAL( L2ARRMCKEN2_DataCE6_e, 30 ) //L2 cache data array correctable error doubleword 6
    DECLARE_ENUM_VAL( L2ARRMCKEN2_DataCE6_i, IBM_BIT_INDEX( 32, L2ARRMCKEN2_DataCE6_e ) )

    DECLARE_ENUM_VAL( L2ARRMCKEN2_DataCE7_e, 31 ) //L2 cache data array correctable error doubleword 7
    DECLARE_ENUM_VAL( L2ARRMCKEN2_DataCE7_i, IBM_BIT_INDEX( 32, L2ARRMCKEN2_DataCE7_e ) )
END_ENUM( L2ARRMCKEN2_bits_t )

//L2CPUMCKEN bits
BEGIN_ENUM( L2CPUMCKEN_bits_t )
    DECLARE_ENUM_VAL( L2CPUMCKEN_IrReqPE0_e, 20 ) //Parity error on C476_L2CIRDREQPARITY bit 0.
    DECLARE_ENUM_VAL( L2CPUMCKEN_IrReqPE0_i, IBM_BIT_INDEX( 32, L2CPUMCKEN_IrReqPE0_e ) )

    DECLARE_ENUM_VAL( L2CPUMCKEN_IrReqPE1_e, 21 ) //Parity error on C476_L2CIRDREQPARITY bit 1.
    DECLARE_ENUM_VAL( L2CPUMCKEN_IrReqPE1_i, IBM_BIT_INDEX( 32, L2CPUMCKEN_IrReqPE1_e ) )

    DECLARE_ENUM_VAL( L2CPUMCKEN_DrReqPE0_e, 22 ) //Parity error on C476_L2CDRDREQPARITY bit 0.
    DECLARE_ENUM_VAL( L2CPUMCKEN_DrReqPE0_i, IBM_BIT_INDEX( 32, L2CPUMCKEN_DrReqPE0_e ) )

    DECLARE_ENUM_VAL( L2CPUMCKEN_DrReqPE1_e, 23 ) //Parity error on C476_L2CDRDREQPARITY bit 1.
    DECLARE_ENUM_VAL( L2CPUMCKEN_DrReqPE1_i, IBM_BIT_INDEX( 32, L2CPUMCKEN_DrReqPE1_e ) )

    DECLARE_ENUM_VAL( L2CPUMCKEN_DrReqPE2_e, 24 ) //Parity error on C476_L2CDRDREQPARITY bit 2.
    DECLARE_ENUM_VAL( L2CPUMCKEN_DrReqPE2_i, IBM_BIT_INDEX( 32, L2CPUMCKEN_DrReqPE2_e ) )

    DECLARE_ENUM_VAL( L2CPUMCKEN_DwReqPE0_e, 25 ) //Parity error on C476_L2CDWWREQPARITY bit 0.
    DECLARE_ENUM_VAL( L2CPUMCKEN_DwReqPE0_i, IBM_BIT_INDEX( 32, L2CPUMCKEN_DwReqPE0_e ) )

    DECLARE_ENUM_VAL( L2CPUMCKEN_DwReqPE1_e, 26 ) //Parity error on C476_L2CDWWREQPARITY bit 1.
    DECLARE_ENUM_VAL( L2CPUMCKEN_DwReqPE1_i, IBM_BIT_INDEX( 32, L2CPUMCKEN_DwReqPE1_e ) )

    DECLARE_ENUM_VAL( L2CPUMCKEN_DwReqPE2_e, 27 ) //Parity error on C476_L2CDWWREQPARITY bit 2.
    DECLARE_ENUM_VAL( L2CPUMCKEN_DwReqPE2_i, IBM_BIT_INDEX( 32, L2CPUMCKEN_DwReqPE2_e ) )

    DECLARE_ENUM_VAL( L2CPUMCKEN_DwDataPE0_e, 28 ) //arity error on C476_L2CDWRDATAPARITY bit 0.
    DECLARE_ENUM_VAL( L2CPUMCKEN_DwDataPE0_i, IBM_BIT_INDEX( 32, L2CPUMCKEN_DwDataPE0_e ) )

    DECLARE_ENUM_VAL( L2CPUMCKEN_DwDataPE1_e, 29 ) //arity error on C476_L2CDWRDATAPARITY bit 1.
    DECLARE_ENUM_VAL( L2CPUMCKEN_DwDataPE1_i, IBM_BIT_INDEX( 32, L2CPUMCKEN_DwDataPE1_e ) )

    DECLARE_ENUM_VAL( L2CPUMCKEN_DwDataPE2_e, 30 ) //arity error on C476_L2CDWRDATAPARITY bit 2.
    DECLARE_ENUM_VAL( L2CPUMCKEN_DwDataPE2_i, IBM_BIT_INDEX( 32, L2CPUMCKEN_DwDataPE2_e ) )

    DECLARE_ENUM_VAL( L2CPUMCKEN_DwDataPE3_e, 31 ) //arity error on C476_L2CDWRDATAPARITY bit 3.
    DECLARE_ENUM_VAL( L2CPUMCKEN_DwDataPE3_i, IBM_BIT_INDEX( 32, L2CPUMCKEN_DwDataPE3_e ) )

END_ENUM( L2CPUMCKEN_bits_t )


void l2c_global_enable_interrupt(uint32_t l2c_dcr_base, L2INTEN_bits_t mask);
void l2c_global_mck_enable(uint32_t l2c_dcr_base, L2MCKEN_bits_t mask);
void l2c_global_mck_disable(uint32_t l2c_dcr_base, L2MCKEN_bits_t mask);
void l2c_enable_interrupt(uint32_t l2c_dcr_base, L2C_L2REG l2c_int_reg_index, uint32_t mask);
uint64_t l2c_arracc_data_read (uint32_t base, uint32_t ext_phys_addr, uint32_t phys_addr);
void l2c_arracc_data_write( uint32_t base, uint32_t ext_phys_addr, uint32_t phys_addr, uint64_t data64);
bool l2c_arracc_tag_info_read_by_way (uint32_t base, uint32_t ext_phys_addr, uint32_t phys_addr, int32_t cache_way, volatile uint32_t* tag_info);
bool l2c_arracc_tag_info_write_by_way_wo_gen_ecc( uint32_t base, uint32_t ext_phys_addr, uint32_t phys_addr, int32_t cache_way, uint32_t cache_data, uint32_t ecc_data);
bool l2c_arracc_data_read_by_way (uint32_t base, uint32_t ext_phys_addr, uint32_t phys_addr, int32_t cache_way, volatile uint64_t* cache_data);
bool l2c_arracc_data_write_by_way (uint32_t base, uint32_t ext_phys_addr, uint32_t phys_addr, int32_t cache_way, uint64_t cache_data);
bool l2c_arracc_data_write_by_way_wo_gen_ecc( uint32_t base, uint32_t ext_phys_addr, uint32_t phys_addr, int32_t cache_way, uint64_t cache_data, uint32_t ecc_data);
bool l2c_arracc_get_way_by_address (uint32_t base, uint32_t ext_phys_addr, uint32_t phys_addr, volatile int32_t* cache_way);
bool l2c_arracc_get_data_by_address (uint32_t base, uint32_t ext_phys_addr, uint32_t phys_addr, volatile uint64_t* data64);
bool l2c_arracc_set_data_by_address (uint32_t base, uint32_t ext_phys_addr, uint32_t phys_addr, uint64_t data64);
bool l2c_arracc_lru_info_read_by_way( uint32_t base, uint32_t ext_phys_addr, uint32_t phys_addr, int32_t cache_way, volatile uint32_t* lru_info );
void l2c_pmu_set_CE_bit(uint32_t pmu_dcr_base, L2C_PMUREG dcr_index);
void l2c_pmu_enable_interrupt(uint32_t pmu_dcr_base, L2C_PMUREG dcr_index, uint32_t mask);
void l2c_pmu_clear_interrupt(uint32_t pmu_dcr_base);
void l2c_pmu_set_cx(uint32_t pmu_dcr_base, L2C_PMUREG dcr_index, uint32_t value);

#endif  /* L2C_H */
