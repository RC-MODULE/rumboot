
//-----------------------------------------------------------------------------
//  This program is for testing PCIe RG, placed in SCTL.
//  It shows, that PCIe can be put into reset and waked up several times.
//  PCIe consider turned on, if link training complete,
//    so it needs: response device in Root Port mode.
//  It includes:
//      - reset off, wait until PCIe link up
//      - reset on
//    - repeat several times
//
//-----------------------------------------------------------------------------

#include <rumboot/pcie_test_lib.h>

int main ()
{
    if (pcie_simple_turn_on () != 0)
        return -1;
    pcie_soft_reset ();
    
    if (pcie_simple_turn_on () != 0)
        return -1;
    pcie_soft_reset ();
    
    if (pcie_simple_turn_on () != 0)
        return -1;
    pcie_soft_reset ();
    
    return 0;
}
