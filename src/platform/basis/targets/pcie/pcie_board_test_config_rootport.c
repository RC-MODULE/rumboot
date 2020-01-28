
//-----------------------------------------------------------------------------
//  This program is for setting Basis PCIe for testing through external host
//  It includes:
//    - turning ON function, enable access to all 4GB
//    
//    Test duration (RTL): < 
//-----------------------------------------------------------------------------

#include <rumboot/pcie_test_lib.h>

int main ()
{
    rumboot_printf ("\npcie_board_test_config_rootport start\n");
    if (pcie_turn_on_with_options_rc (1, 1, 0, 0, 0, 0, 0, 0, 0, 0) != 0)
        return -1;
    
    rumboot_printf ("\npcie_board_test_config_rootport pass\n");
    return 0;
}

