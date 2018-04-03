/**
 *
 */

#ifndef __PLB6BC_REG_H__
#define __PLB6BC_REG_H__


#include <platform/oi10/platform/plb6bc/reg_field.h>
#include <common_macros/common_macros.h>
#include <arch/ppc/ibm_bit_ordering_macros.h>
#include <platform/oi10/platform/boost/preprocessor/iteration/local.hpp>
#include <platform/oi10/platform/boost/preprocessor/arithmetic/dec.hpp>
#include <platform/oi10/platform/boost/preprocessor/cat.hpp>


BEGIN_ENUM( PLB6BC_REG )
DECLARE_ENUM_VAL( PLB6BC_CR0,                   0x00 )          // Control Register 0           0x00000000
DECLARE_ENUM_VAL( PLB6BC_PRI,                   0x01 )          // Master Priority Level        0x00000000      All masters default to the lowest priority.
DECLARE_ENUM_VAL( PLB6BC_TSNOOP,                0x02 )          // TSNOOP                       NA              The value of this register is hard coded during configuration and does not change.
DECLARE_ENUM_VAL( PLB6BC_PAAM_WIN_EXT,          0x03 )          // PAAM Window Extension        0xC0000000      This register defaults to 3 to represent three clock cycles.
#define BOOST_PP_LOCAL_MACRO( x )\
DECLARE_ENUM_VAL( BOOST_PP_CAT( PLB6BC_SGD,x ), 0x04+(x-1) )    // Segment i+1 Decode           0x(i+1)0000000  If the segment does not exist during configuration, the register reset value is hard coded to 0xFFFFFFF0 and is read only.
#define BOOST_PP_LOCAL_LIMITS (1, BOOST_PP_DEC(PLB6BC_SEGMENT_N))
#include BOOST_PP_LOCAL_ITERATE()
DECLARE_ENUM_VAL( PLB6BC_ERR,                   0x0B )          // Error                        0x00000000      Write a ‘1’ to an individual bit to clear the individual bit, or write all ‘1’s to clear all bits.
DECLARE_ENUM_VAL( PLB6BC_MSRSP,                 0x0C )          // Multiple Slaves Responded    0x00000000      Write a ‘1’ to an individual bit to clear the individual bit or write all ‘1’s to clear all bits.
DECLARE_ENUM_VAL( PLB6BC_HCPP,                  0x0D )          // Hang Count Pulse Period      0x03FFFFFF
DECLARE_ENUM_VAL( PLB6BC_HD,                    0x0E )          // Hang Detected Status         0x00000000      Write a ‘1’ to an individual bit to clear the individual bit or write all ‘1’s to clear all bits.
DECLARE_ENUM_VAL( PLB6BC_QD,                    0x0F )          // Quiesce Detected Status      0x00000000      Write a ‘1’ to an individual bit to clear the individual bit or write all ‘1’s to clear all bits.
DECLARE_ENUM_VAL( PLB6BC_SHD,                   0x10 )          // System Hung Detected Status  0x00000000      Write a ‘1’ to an individual bit to clear the individual bit or write all ‘1’s to clear all bits.
DECLARE_ENUM_VAL( PLB6BC_REVID,                 0x11 )          // CoreConnectTM Revision ID    0x00000101      The value of this register is hard coded during configuration and does not change.
END_ENUM( PLB6BC_REG )

BEGIN_ENUM( PLB6BC_REG_DFLT )
DECLARE_ENUM_VAL( PLB6BC_CR0_DFLT,                                  0x00000000 )
DECLARE_ENUM_VAL( PLB6BC_PRI_DFLT,                                  0x00000000 )
DECLARE_ENUM_VAL( PLB6BC_PAAM_WIN_EXT_DFLT,                         0xC0000000 )
#define BOOST_PP_LOCAL_MACRO( x )\
DECLARE_ENUM_VAL( BOOST_PP_CAT(BOOST_PP_CAT( PLB6BC_SGD,x),_DFLT ), (x * 0x10000000) )
#define BOOST_PP_LOCAL_LIMITS (1, 2)
#include BOOST_PP_LOCAL_ITERATE()
#define BOOST_PP_LOCAL_MACRO( x )\
DECLARE_ENUM_VAL( BOOST_PP_CAT(BOOST_PP_CAT( PLB6BC_SGD,x),_DFLT ), 0xFFFFFFF0 )
#define BOOST_PP_LOCAL_LIMITS (3, BOOST_PP_DEC(PLB6BC_SEGMENT_N))
#include BOOST_PP_LOCAL_ITERATE()
DECLARE_ENUM_VAL( PLB6BC_ERR_DFLT,                                  0x00000000 )
DECLARE_ENUM_VAL( PLB6BC_MSRSP_DFLT,                                0x00000000 )
DECLARE_ENUM_VAL( PLB6BC_HCPP_DFLT,                                 0x03FFFFFF )
DECLARE_ENUM_VAL( PLB6BC_HD_DFLT,                                   0x00000000 )
DECLARE_ENUM_VAL( PLB6BC_QD_DFLT,                                   0x00000000 )
DECLARE_ENUM_VAL( PLB6BC_SHD_DFLT,                                  0x00000000 )
DECLARE_ENUM_VAL( PLB6BC_REVID_DFLT,                                0x00000101 )
END_ENUM( PLB6BC_REG_DFLT )

BEGIN_ENUM( PLB6BC_REG_MSK )
DECLARE_ENUM_VAL( PLB6BC_CR0_MSK,           0xE0000000 )
DECLARE_ENUM_VAL( PLB6BC_PRI_MSK,           0xFFFF0000 )
DECLARE_ENUM_VAL( PLB6BC_TSNOOP_MSK,        0xF0000000 )
DECLARE_ENUM_VAL( PLB6BC_PAAM_WIN_EXT_MSK,  0xC0000000 )
DECLARE_ENUM_VAL( PLB6BC_SGDx_MSK,          0xFFFFFFF0 )
DECLARE_ENUM_VAL( PLB6BC_ERR_MSK,           0xFFFFF800 )
DECLARE_ENUM_VAL( PLB6BC_HCPP_MSK,          0x03FFFFFF )
DECLARE_ENUM_VAL( PLB6BC_HD_MSK,            0xFFFF0000 )
DECLARE_ENUM_VAL( PLB6BC_QD_MSK,            0xFFFF0000 )
DECLARE_ENUM_VAL( PLB6BC_SHD_MSK,           0xFFFF0000 )
END_ENUM( PLB6BC_REG_MSK )


#endif // __PLB6BC_REG_H__
