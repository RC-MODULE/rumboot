#ifndef REGS_SCTL_H
#define REGS_SCTL_H


#include <rumboot/macros.h>

/* TODO: remove this */
#define SCTL_BOOTM 0x0


BEGIN_ENUM( SCTL_REG )
    DECLARE_ENUM_VAL(   SCTL_SYS_BOOT_CFG,      0x000 )
    DECLARE_ENUM_VAL(   SCTL_PPC_SYS_CONF,      0x004 )
    DECLARE_ENUM_VAL(   SCTL_PPC_SLP_CTRL,      0x008 )
    DECLARE_ENUM_VAL(   SCTL_PPC_PMU_CTRL,      0x00C )
    DECLARE_ENUM_VAL(   SCTL_PPC_KMBIST,        0x010 )
    DECLARE_ENUM_VAL(   SCTL_IFSYS_KMBIST,      0x018 )
    DECLARE_ENUM_VAL(   SCTL_IFSYS_ETH_CTRL,    0x01C )
END_ENUM( SCTL_REG )


#endif /* end of include guard: REGS_SCTL_H */
