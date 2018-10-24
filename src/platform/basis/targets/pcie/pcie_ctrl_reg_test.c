
//-----------------------------------------------------------------------------
//  This program is for testing PCIe  Ctrl registers, placed in SCTL.
//  It must show, that APB bus works correctly.
//  It includes:
//    - send PCIe to reset
//    - Chech, that RW register work correctly
//
//-----------------------------------------------------------------------------
#include <stdint.h>
#include <stdlib.h>

#include <rumboot/io.h>
#include <rumboot/pcie_test_lib.h>
#include <regs/regs_pcie.h>
#include <platform/devices.h>

void pcie_reset ()
{
    iowrite32 (0x0, SCTL_BASE + SCTL_PCIE_RST);
}

int check_registers ()
{
    if ((ioread32 (SCTL_BASE + SCTL_PCIE_REG_0) != 0x00000039) | (ioread32 (SCTL_BASE + SCTL_PCIE_REG_1) != 0x00000000))
        return -1;

    iowrite32 (0x00000000, SCTL_BASE + SCTL_PCIE_REG_0);
    iowrite32 (0x00000000, SCTL_BASE + SCTL_PCIE_REG_1);
    if ((ioread32 (SCTL_BASE + SCTL_PCIE_REG_0) != 0x00000000) | (ioread32 (SCTL_BASE + SCTL_PCIE_REG_1) != 0x00000000))
        return -1;

    iowrite32 (0xFFFFFFFF, SCTL_BASE + SCTL_PCIE_REG_1);
    iowrite32 (0xFFFFFFFF, SCTL_BASE + SCTL_PCIE_REG_0);
    if ((ioread32 (SCTL_BASE + SCTL_PCIE_REG_0) != 0x04C03EFF) | (ioread32 (SCTL_BASE + SCTL_PCIE_REG_1) != 0x00000004))
        return -1;

    iowrite32 (0x55555555, SCTL_BASE + SCTL_PCIE_REG_1);
    iowrite32 (0x55555555, SCTL_BASE + SCTL_PCIE_REG_0);
    if ((ioread32 (SCTL_BASE + SCTL_PCIE_REG_1) != 0x00000004) | (ioread32 (SCTL_BASE + SCTL_PCIE_REG_0) != 0x04401455))
        return -1;

    iowrite32 (0xAAAAAAAA, SCTL_BASE + SCTL_PCIE_REG_0);
    iowrite32 (0xAAAAAAAA, SCTL_BASE + SCTL_PCIE_REG_1);
    if ((ioread32 (SCTL_BASE + SCTL_PCIE_REG_1) != 0x00000000) | (ioread32 (SCTL_BASE + SCTL_PCIE_REG_0) != 0x00802AAA))
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
