
//-----------------------------------------------------------------------------
//  This program is for simple PCIe subsystem testing
//  PCIe consider turned on, if link training complete.
//    so it needs: response device in Root Port mode.
//  It includes:
//    - turning ON function without setting any parameters
//    
//    Test duration (RTL): < 120us
//-----------------------------------------------------------------------------

#include <rumboot/pcie_test_lib.h>

int main ()
{
    if (pcie_simple_turn_on () != 0)
        return -1;
    
    return 0;
}

