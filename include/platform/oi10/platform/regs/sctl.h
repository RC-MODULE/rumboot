#ifndef REGS_SCTL_H
#define REGS_SCTL_H


#include <rumboot/macros.h>

/* TODO: remove this */
#define SCTL_BOOTM 0x0


BEGIN_ENUM( SCTL_REG )
    DECLARE_ENUM_VAL(   SCTL_SYS_BOOT_CFG,      0x000 )
    DECLARE_ENUM_VAL(   SCTL_PPC_SYS_CONF,      0x028 )
    DECLARE_ENUM_VAL(   SCTL_PPC_SLP_CTRL,      0x02C )
    DECLARE_ENUM_VAL(   SCTL_PPC_PMU_CTRL,      0x030 )
    DECLARE_ENUM_VAL(   SCTL_IFSYS_ETH_CTRL,    0x034 )
    DECLARE_ENUM_VAL(   SCTL_IFSYS_ETH_HPROT,   0x038 )
END_ENUM( SCTL_REG )


#endif /* end of include guard: REGS_SCTL_H */
