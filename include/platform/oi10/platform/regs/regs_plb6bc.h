/**
 *
 */

#ifndef __PLB6BC_REG_H__
#define __PLB6BC_REG_H__


#include <platform/common_macros/common_macros.h>
#include <platform/arch/ppc/ibm_bit_ordering_macros.h>
#include <platform/regs/fields/plb6bc.h>


BEGIN_ENUM( PLB6BC_REG )
DECLARE_ENUM_VAL( PLB6BC_CR0,                   0x00 )          /* Control Register 0           0x00000000*/
DECLARE_ENUM_VAL( PLB6BC_PRI,                   0x01 )          /* Master Priority Level        0x00000000      All masters default to the lowest priority.*/
DECLARE_ENUM_VAL( PLB6BC_TSNOOP,                0x02 )          /* TSNOOP                       NA              The value of this register is hard coded during configuration and does not change.*/
DECLARE_ENUM_VAL( PLB6BC_PAAM_WIN_EXT,          0x03 )          /* PAAM Window Extension        0xC0000000      This register defaults to 3 to represent three clock cycles.*/
DECLARE_ENUM_VAL( PLB6BC_SGD1,                  0x04 )          /* Segment i+1 Decode           0x(i+1)0000000  If the segment does not exist during configuration, the register reset value is hard coded to 0xFFFFFFF0 and is read only.*/
DECLARE_ENUM_VAL( PLB6BC_SGD2,                  0x05 )          /* Segment i+1 Decode           0x(i+1)0000000  If the segment does not exist during configuration, the register reset value is hard coded to 0xFFFFFFF0 and is read only.*/
DECLARE_ENUM_VAL( PLB6BC_SGD3,                  0x06 )          /* Segment i+1 Decode           0x(i+1)0000000  If the segment does not exist during configuration, the register reset value is hard coded to 0xFFFFFFF0 and is read only.*/
DECLARE_ENUM_VAL( PLB6BC_SGD4,                  0x07 )          /* Segment i+1 Decode           0x(i+1)0000000  If the segment does not exist during configuration, the register reset value is hard coded to 0xFFFFFFF0 and is read only.*/
DECLARE_ENUM_VAL( PLB6BC_SGD5,                  0x08 )          /* Segment i+1 Decode           0x(i+1)0000000  If the segment does not exist during configuration, the register reset value is hard coded to 0xFFFFFFF0 and is read only.*/
DECLARE_ENUM_VAL( PLB6BC_SGD6,                  0x09 )          /* Segment i+1 Decode           0x(i+1)0000000  If the segment does not exist during configuration, the register reset value is hard coded to 0xFFFFFFF0 and is read only.*/
DECLARE_ENUM_VAL( PLB6BC_SGD7,                  0x0A )          /* Segment i+1 Decode           0x(i+1)0000000  If the segment does not exist during configuration, the register reset value is hard coded to 0xFFFFFFF0 and is read only.*/
DECLARE_ENUM_VAL( PLB6BC_ERR,                   0x0B )          /* Error                        0x00000000      Write a ‘1’ to an individual bit to clear the individual bit, or write all ‘1’s to clear all bits.*/
DECLARE_ENUM_VAL( PLB6BC_MSRSP,                 0x0C )          /* Multiple Slaves Responded    0x00000000      Write a ‘1’ to an individual bit to clear the individual bit or write all ‘1’s to clear all bits.*/
DECLARE_ENUM_VAL( PLB6BC_HCPP,                  0x0D )          /* Hang Count Pulse Period      0x03FFFFFF*/
DECLARE_ENUM_VAL( PLB6BC_HD,                    0x0E )          /* Hang Detected Status         0x00000000      Write a ‘1’ to an individual bit to clear the individual bit or write all ‘1’s to clear all bits.*/
DECLARE_ENUM_VAL( PLB6BC_QD,                    0x0F )          /* Quiesce Detected Status      0x00000000      Write a ‘1’ to an individual bit to clear the individual bit or write all ‘1’s to clear all bits.*/
DECLARE_ENUM_VAL( PLB6BC_SHD,                   0x10 )          /* System Hung Detected Status  0x00000000      Write a ‘1’ to an individual bit to clear the individual bit or write all ‘1’s to clear all bits.*/
DECLARE_ENUM_VAL( PLB6BC_REVID,                 0x11 )          /* CoreConnectTM Revision ID    0x00000101      The value of this register is hard coded during configuration and does not change.*/
END_ENUM( PLB6BC_REG )

BEGIN_ENUM( PLB6BC_REG_DFLT )
DECLARE_ENUM_VAL( PLB6BC_CR0_DFLT,                                  0x00000000 )
DECLARE_ENUM_VAL( PLB6BC_PRI_DFLT,                                  0x00000000 )
DECLARE_ENUM_VAL( PLB6BC_PAAM_WIN_EXT_DFLT,                         0xC0000000 )
DECLARE_ENUM_VAL( PLB6BC_SGD1_DFLT,                                 0x10000000 )
DECLARE_ENUM_VAL( PLB6BC_SGD2_DFLT,                                 0x20000000 )
DECLARE_ENUM_VAL( PLB6BC_SGD3_DFLT,                                 0x30000000 )
DECLARE_ENUM_VAL( PLB6BC_SGD4_DFLT,                                 0xFFFFFFF0 )
DECLARE_ENUM_VAL( PLB6BC_SGD5_DFLT,                                 0xFFFFFFF0 )
DECLARE_ENUM_VAL( PLB6BC_SGD6_DFLT,                                 0xFFFFFFF0 )
DECLARE_ENUM_VAL( PLB6BC_SGD7_DFLT,                                 0xFFFFFFF0 )
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

#define PLB6_BC_CR0_VALUE			0x00000000
#define PLB6_BC_PRI_VALUE			0x00000000
#define PLB6_BC_TSNOOP_VALUE		reg_field(3,0x8) /* may be changed? */
#define PLB6_BC_PAAM_WIN_EXT_VALUE	0xC0000000
#define PLB6_BC_SGD1_VALUE			0x10000000
#define PLB6_BC_SGD2_VALUE			0x20000000
#define PLB6_BC_SGD3_VALUE			0x30000000
#define PLB6_BC_SGD4_VALUE			0x40000000
#define PLB6_BC_SGD5_VALUE			0x50000000
#define PLB6_BC_SGD6_VALUE			0x60000000
#define PLB6_BC_SGD7_VALUE			0x70000000
#define PLB6_BC_ERR_VALUE			0x00000000
#define PLB6_BC_MSRSP_VALUE			0x00000000
#define PLB6_BC_HCPP_VALUE			PLB6BC_HCPP_4K
#define PLB6_BC_HD_VALUE			0x00000000
#define PLB6_BC_QD_VALUE			0x00000000
#define PLB6_BC_SHD_VALUE			0x00000000
#define PLB6_BC_REVID_VALUE			reg_field(31,0x02)   |\
									reg_field(23, 0x01)  |\
									reg_field(11, 0x000)


#endif /* __PLB6BC_REG_H__*/
