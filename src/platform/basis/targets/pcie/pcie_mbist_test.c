
//-----------------------------------------------------------------------------
//  This program is for PCIe Memory BIST testing
//  MBIST controller is inserted after synthesis, so, on RTL this program
//    need a model, that connected to internal points of SOC
//
//  !!!  Dont forget to delete this model on Netlist  !!!
//
//    Test includes:
//    - PCIe reset Off
//    - start MBIST
//    - wait until MBIST finish and check
//    
//    Test duration (RTL): < 
//-----------------------------------------------------------------------------

#include <rumboot/pcie_test_lib.h>
#include <regs/regs_pcie.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <rumboot/printf.h>
#include <rumboot/io.h>

#include <platform/devices.h>

uint32_t main ()
{
    //-----------------------------------------------------------------
    //  PCIe Reset Off - start PLL
    //  And wait PLL is locked
    //-----------------------------------------------------------------
    iowrite32 (0x1, SCTL_BASE + SCTL_PCIE_RST);
    while ((ioread32 (SCTL_BASE + SCTL_PCIE_RST) & 0x1) != 0x1)
        ;
        
    //-----------------------------------------------------------------
    //  Enable MBIST functionality
    //-----------------------------------------------------------------
    iowrite32 (0x04000039, SCTL_BASE + SCTL_PCIE_REG_0);
    
    //-----------------------------------------------------------------
    //  Check, MBIST enabled
    //    This "if" is used like delay function
    //-----------------------------------------------------------------
    if ((ioread32 (SCTL_BASE + SCTL_PCIE_REG_0) & 0x04000000) != 0x04000000)
    {
    }
    
    //-----------------------------------------------------------------
    //  Start MBIST
    //-----------------------------------------------------------------
    iowrite32 (0x04800039, SCTL_BASE + SCTL_PCIE_REG_0);
    
    //-----------------------------------------------------------------
    //  Wait MBIST done
    //-----------------------------------------------------------------
    while ((ioread32 (SCTL_BASE + SCTL_PCIE_REG_0) & 0x02000000) != 0x02000000)
        ;
    
    //-----------------------------------------------------------------
    //  Check, if MBIST failed
    //-----------------------------------------------------------------
    if ((ioread32 (SCTL_BASE + SCTL_PCIE_REG_0) & 0x01000000) == 0x01000000)
        return -1;
    
    return 0;
}
