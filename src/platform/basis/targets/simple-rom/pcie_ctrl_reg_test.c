
//-----------------------------------------------------------------------------
//  This program is for testing PCIe  Ctrl registers, placed in SCTL.
//  It must show, that APB bus works correctly.
//  It includes:
//    - send PCIe to reset
//    - Chech, that RW register work correctly
//
//-----------------------------------------------------------------------------
#include <basis/defs_c.h>

void pcie_reset ()
{
    rgSCTL_PCIE_RST = 0x0;
}

int check_registers ()
{
    if ((rgSCTL_PCIE_REG_0 != 0x00000039) | (rgSCTL_PCIE_REG_1 != 0x00000000))
        return -1;
        
    rgSCTL_PCIE_REG_0 = 0x00000000 ;
    rgSCTL_PCIE_REG_1 = 0x00000000 ;
    if ((rgSCTL_PCIE_REG_0 != 0x00000000) | (rgSCTL_PCIE_REG_1 != 0x00000000))
        return -1;
        
    rgSCTL_PCIE_REG_1 = 0xFFFFFFFF ;
    rgSCTL_PCIE_REG_0 = 0xFFFFFFFF ;
    if ((rgSCTL_PCIE_REG_0 != 0x04C03EFF) | (rgSCTL_PCIE_REG_1 != 0x00000004))
        return -1;
        
    rgSCTL_PCIE_REG_1 = 0x55555555 ;
    rgSCTL_PCIE_REG_0 = 0x55555555 ;
    if ((rgSCTL_PCIE_REG_1 != 0x00000004) | (rgSCTL_PCIE_REG_0 != 0x04401455))
        return -1;
        
    rgSCTL_PCIE_REG_0 = 0xAAAAAAAA ;
    rgSCTL_PCIE_REG_1 = 0xAAAAAAAA ;
    if ((rgSCTL_PCIE_REG_1 != 0x00000000) | (rgSCTL_PCIE_REG_0 != 0x00802AAA))
        return -1;
        
    return 0;
}

int main ()
{
    pcie_reset ();
    if (check_registers () != 0)
        return -1;
        
    return 0;
}
