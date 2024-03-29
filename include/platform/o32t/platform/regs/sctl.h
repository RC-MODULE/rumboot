#ifndef REGS_SCTL_H
#define REGS_SCTL_H


#include <rumboot/macros.h>
#include <platform/regs/fields/sctl.h>

/* TODO: remove this */
#define SCTL_BOOTM 0x0

BEGIN_ENUM( SCTL_PPC_SLP_TYPE )
DECLARE_ENUM_VAL( SCTL_PPC_SLP_CPU_DOZE,       0b00 )
DECLARE_ENUM_VAL( SCTL_PPC_SLP_CPU_SLP,        0b01 )
DECLARE_ENUM_VAL( SCTL_PPC_SLP_CPU_L2C_SLP,    0b10 )
END_ENUM( SCTL_PPC_SLP_TYPE )

BEGIN_ENUM( SCTL_REG )
    DECLARE_ENUM_VAL(   SCTL_SYS_BOOT_CFG,      0x000 )
    DECLARE_ENUM_VAL(   KMBIST_CHAIN_SF_0,      0x004 )
    DECLARE_ENUM_VAL(   KMBIST_CHAIN_SF_1,      0x008 )
    DECLARE_ENUM_VAL(   KMBIST_CHAIN_SF_2,      0x00C )
    DECLARE_ENUM_VAL(   KMBIST_CHAIN_SF_3,      0x010 )
    DECLARE_ENUM_VAL(   KMBIST_CHAIN_SF_4,      0x014 )
    DECLARE_ENUM_VAL(   KMBIST_CHAIN_SF_5,      0x018 )
    DECLARE_ENUM_VAL(   KMBIST_CHAIN_SF_6,      0x01C )
    DECLARE_ENUM_VAL(   KMBIST_CHAIN_SF_6_IM1,      KMBIST_CHAIN_SF_6 )
    DECLARE_ENUM_VAL(   KMBIST_CHAIN_SF_7,      0x020 )
    DECLARE_ENUM_VAL(   KMBIST_CHAIN_SF_8,      0x024 )
    DECLARE_ENUM_VAL(   KMBIST_CHAIN_SF_8_IM0,      KMBIST_CHAIN_SF_8 )

    DECLARE_ENUM_VAL(   SCTL_PPC_SYS_CONF,      0x028 )
    DECLARE_ENUM_VAL(   SCTL_PPC_SLP_CTRL,      0x02C )
    DECLARE_ENUM_VAL(   SCTL_PPC_PMU_CTRL,      0x030 )
    
    DECLARE_ENUM_VAL(   SCTL_IFSYS_ETH_HPROT,   0x038 )
    DECLARE_ENUM_VAL(   SCTL_NMPU_PRST,         0x03C )
    DECLARE_ENUM_VAL(   SCTL_NMPU_DBG_PRST,     0x040 )
    DECLARE_ENUM_VAL(   SCTL_NMPU_NMI,          0x044 )
    DECLARE_ENUM_VAL(   KMBIST_CHAIN_SF_9,      0x048 )
    DECLARE_ENUM_VAL(   KMBIST_CHAIN_SF_10,     0x04C )
END_ENUM( SCTL_REG )


#endif /* end of include guard: REGS_SCTL_H */
