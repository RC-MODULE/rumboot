#ifndef __SCTL_REGS_H__
#define __SCTL_REGS_H__


#include <common_macros/common_macros.h>
#include <arch/ppc/ibm_bit_ordering_macros.h>
#include <platform/oi10/platform/sctl/sctl_reg_fields.h>
#include <platform/oi10/platform/boost/preprocessor/cat.hpp>
#include <platform/oi10/platform/boost/preprocessor/iteration/local.hpp>


#define SCTLx_IFSYS_IO_CONF_REGS_COUNT    36
#define SCTLx_MLT_IO_CONF_REGS_COUNT      34

// register addresses
//SYS
#define SCTLx_BOOT_CFG              0x000
#define SCTLx_PLL_STATE             0x004
#define SCTLx_SYS_CONF0             0x008
#define SCTLx_RST_MON               0x010
#define SCTLx_RST_CTRL              0x014
#define SCTLx_PPC_SLP_CTRL          0x018
#define SCTLx_SRAM_NOR_CTRL         0x01C
//BISR
#define SCTLx_BISR_IM0              0x020
#define SCTLx_BISR_L2C_0            0x024
#define SCTLx_BISR_L2C_1            0x028
#define SCTLx_BISR_DSP0_IM1_0       0x02C
#define SCTLx_BISR_DSP0_IM1_1       0x030
#define SCTLx_BISR_DSP0_L2C         0x034
#define SCTLx_BISR_DSP1_IM1_0       0x038
#define SCTLx_BISR_DSP1_IM1_1       0x03C
#define SCTLx_BISR_DSP1_L2C         0x040
//IFSYS
#define SCTLx_IFSYS_IO_CONFx(x)     (0x060+(0x4*x))

BEGIN_ENUM( SCTLx_IFSYS_IO_CONF )
#define BOOST_PP_LOCAL_MACRO( i )\
DECLARE_ENUM_VAL( BOOST_PP_CAT( SCTLx_IFSYS_IO_CONF,i ),   0x060+(0x4*i) )
#define BOOST_PP_LOCAL_LIMITS (0, 35)
#include BOOST_PP_LOCAL_ITERATE()
END_ENUM( SCTLx_IFSYS_IO_CONF )
//MLT
#define SCTLx_MLT_IO_CONFx(x)       (0x100+(0x4*x))

BEGIN_ENUM( SCTLx_MLT_IO_CONF )
#define BOOST_PP_LOCAL_MACRO( i )\
DECLARE_ENUM_VAL( BOOST_PP_CAT( SCTLx_MLT_IO_CONF,i ),   0x100+(0x4*i) )
#define BOOST_PP_LOCAL_LIMITS (0, 33)
#include BOOST_PP_LOCAL_ITERATE()
END_ENUM( SCTLx_MLT_IO_CONF )

#define SCTLx_MISC_IO_CONF0			0x190
//BIST
#define SCTLx_USB_BIST_CNTRL        0x1E0
#define SCTLx_EM0_BIST_CNTRL        0x1E4
#define SCTLx_EM1_BIST_CNTRL        0x1E8
//DSP
#define SCTLx_INTSTA_DSP2CPU        0x240
#define SCTLx_INTCLR_DSP2CPU        0x244
#define SCTLx_INTREQ_CPU2DSP        0x248
//misc
#define SCTLx_MLT_ADR_MSB0          0x290
#define SCTLx_MLT_ADR_MSB1          0x294

// default values
//SYS
#define SCTLx_SYS_CONF0_DEFAULT             0x00000400
#define SCTLx_RST_MON_DEFAULT               0x00000000
#define SCTLx_RST_CTRL_DEFAULT              0x01061A80
#define SCTLx_PPC_SLP_CTRL_DEFAULT          0x00000000
#define SCTLx_SRAM_NOR_CTRL_DEFAULT         0x00000000
//BISR
#define SCTLx_BISR_IM0_DEFAULT              0x00000000
#define SCTLx_BISR_L2C_0_DEFAULT            0x00000000
#define SCTLx_BISR_L2C_1_DEFAULT            0x00000000
#define SCTLx_BISR_DSP0_IM1_0_DEFAULT       0x00000000
#define SCTLx_BISR_DSP0_IM1_1_DEFAULT       0x00000000
#define SCTLx_BISR_DSP0_L2C_DEFAULT         0x00000000
#define SCTLx_BISR_DSP1_IM1_0_DEFAULT       0x00000000
#define SCTLx_BISR_DSP1_IM1_1_DEFAULT       0x00000000
#define SCTLx_BISR_DSP1_L2C_DEFAULT         0x00000000
//IFSYS
#define SCTLx_IFSYS_IO_CONF0_1__15_35_DEFAULT   0x00000000
#define SCTLx_IFSYS_IO_CONF2_13_DEFAULT         0x12121212
#define SCTLx_IFSYS_IO_CONF14_DEFAULT           0x00001212
//MLT
#define SCTLx_MLT_IO_CONF0_6__8_11__13_14__16_22__24_30__32_DEFAULT         0x12121212
#define SCTLx_MLT_IO_CONF7__15_DEFAULT                                      0x00000000
#define SCTLx_MLT_IO_CONF12_DEFAULT                                         SCTLx_MLT_IO_CONF0_6__8_11__13_14__16_22__24_30__32_DEFAULT//0x00001212
#define SCTLx_MLT_IO_CONF23__31_DEFAULT                                     0x00000012
#define SCTLx_MLT_IO_CONF33_DEFAULT                                         0x00121212
#define SCTLx_MISC_IO_CONF0_DEFAULT											0x00001212
//BIST
#define SCTLx_USB_BIST_CNTRL_DEFAULT        0x00000004
#define SCTLx_EM0_BIST_CNTRL_DEFAULT        0x00000008
#define SCTLx_EM1_BIST_CNTRL_DEFAULT        0x00000008
//DSP
#define SCTLx_INTSTA_DSP2CPU_DEFAULT        0x00000000
#define SCTLx_INTCLR_DSP2CPU_DEFAULT        0x00000000
#define SCTLx_INTREQ_CPU2DSP_DEFAULT        0x00000000
//misc
#define SCTLx_MLT_ADR_MSB0_DEFAULT          0x00000000
#define SCTLx_MLT_ADR_MSB1_DEFAULT          0x00000000

// mask values
//SYS
#define SCTLx_BOOT_CFG_RMSK                 0x000000FF
#define SCTLx_PLL_STATE_RMSK                0x0000001F
#define SCTLx_SYS_CONF0_MSK                 0x00000EFF//0x00003EFF
#define SCTLx_RST_MON_RMSK                  0x00000F00
#define SCTLx_RST_CTRL_MSK                  0xFFFFFFF3
#define SCTLx_PPC_SLP_CTRL_RMSK             0x000000FF
#define SCTLx_PPC_SLP_CTRL_WMSK             0x000000FC
#define SCTLx_SRAM_NOR_CTRL_MSK             0x0000000F
//BISR
#define SCTLx_BISR_IM0_RMSK                 0x0000001F
#define SCTLx_BISR_IM0_WMSK                 0x00000003
#define SCTLx_BISR_L2C_0_RMSK               0x0000001F
#define SCTLx_BISR_L2C_0_WMSK               0x00000003
#define SCTLx_BISR_L2C_1_RMSK               0x0000001F
#define SCTLx_BISR_L2C_1_WMSK               0x00000003
#define SCTLx_BISR_DSP0_IM1_0_RMSK          0x0000001F
#define SCTLx_BISR_DSP0_IM1_0_WMSK          0x00000003
#define SCTLx_BISR_DSP0_IM1_1_RMSK          0x0000001F
#define SCTLx_BISR_DSP0_IM1_1_WMSK          0x00000003
#define SCTLx_BISR_DSP0_L2C_RMSK            0x0000001F
#define SCTLx_BISR_DSP0_L2C_WMSK            0x00000003
#define SCTLx_BISR_DSP1_IM1_0_RMSK          0x0000001F
#define SCTLx_BISR_DSP1_IM1_0_WMSK          0x00000003
#define SCTLx_BISR_DSP1_IM1_1_RMSK          0x0000001F
#define SCTLx_BISR_DSP1_IM1_1_WMSK          0x00000003
#define SCTLx_BISR_DSP1_L2C_RMSK            0x0000001F
#define SCTLx_BISR_DSP1_L2C_WMSK            0x00000003
//IO
#define SCTLx_GPIO_CONF_RD_MASK             0b11110011
//IFSYS
#define SCTLx_IFSYS_IO_CONF2_13_MSK         ((SCTLx_GPIO_CONF_RD_MASK << 24) | (SCTLx_GPIO_CONF_RD_MASK << 16) | (SCTLx_GPIO_CONF_RD_MASK << 8) | (SCTLx_GPIO_CONF_RD_MASK << 0))
#define SCTLx_IFSYS_IO_CONF14_MSK           ((SCTLx_GPIO_CONF_RD_MASK << 8) | (SCTLx_GPIO_CONF_RD_MASK << 0))
#define SCTLx_IFSYS_IO_CONF19_MSK           0x03F3FFFF
#define SCTLx_IFSYS_IO_CONF20_21_MSK        0xFFFFFFFF
#define SCTLx_IFSYS_IO_CONF22_MSK           0x00003FFF
#define SCTLx_IFSYS_IO_CONF35_MSK           0x0000003F
//MLT
#define SCTLx_MLT_IO_CONF0_6__8_11__13_14__16_22__24_30__32_MSK     ((SCTLx_GPIO_CONF_RD_MASK << 24) | (SCTLx_GPIO_CONF_RD_MASK << 16) | (SCTLx_GPIO_CONF_RD_MASK << 8) | (SCTLx_GPIO_CONF_RD_MASK << 0))
#define SCTLx_MLT_IO_CONF12_MSK                                     SCTLx_MLT_IO_CONF0_6__8_11__13_14__16_22__24_30__32_MSK//((SCTLx_GPIO_CONF_RD_MASK << 8) | (SCTLx_GPIO_CONF_RD_MASK << 0))
#define SCTLx_MLT_IO_CONF23__31_MSK                                 (SCTLx_GPIO_CONF_RD_MASK << 0)
#define SCTLx_MLT_IO_CONF33_MSK                                     ((SCTLx_GPIO_CONF_RD_MASK << 16) | (SCTLx_GPIO_CONF_RD_MASK << 8) | (SCTLx_GPIO_CONF_RD_MASK << 0))
#define SCTLx_MISC_IO_CONF0_MSK				0x0000F3F3
//BIST
#define SCTLx_USB_BIST_CNTRL_RMSK           0x000007FF
#define SCTLx_USB_BIST_CNTRL_WMSK           0x000000FF
#define SCTLx_EM0_BIST_CNTRL_RMSK           0x0000007F
#define SCTLx_EM0_BIST_CNTRL_WMSK           0x0000000F
#define SCTLx_EM1_BIST_CNTRL_RMSK           0x0000007F
#define SCTLx_EM1_BIST_CNTRL_WMSK           0x0000000F
//DSP
#define SCTLx_INTSTA_DSP2CPU_RMSK           0x000000FF
#define SCTLx_INTCLR_DSP2CPU_WMSK           0x000000FF
#define SCTLx_INTREQ_CPU2DSP_WMSK           0x00000FFF
//misc
#define SCTLx_MLT_ADR_MSB0_MSK              0xFFFFFFFF
#define SCTLx_MLT_ADR_MSB1_MSK              0x000000FF

#endif // __SCTL_REGS_H__
