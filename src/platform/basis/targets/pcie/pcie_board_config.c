
//-----------------------------------------------------------------------------
//  This program is for testing PCIe setup functions
//  It includes:
//    - 
//    
//    Test duration (RTL): < 
//-----------------------------------------------------------------------------

#include <rumboot/pcie_lib.h>
#include <rumboot/printf.h>

int main ()
{
    rumboot_printf ("\npcie_board_test_config_with_file start\n");
    
    if (pcie_init (
            1       ,  //  uint32_t speed            
            0x5813  ,  //  uint32_t device_id        
            0xABDC  ,  //  uint32_t revision_id      
            0xFF    ,  //  uint32_t subclass_code    
            0xFF    ,  //  uint32_t class_code       
            1          //  uint32_t state_auto_ack
        ))
        return -1;
    
    //            bar_number, bar_axi_start, bar_aperture, bar_type
    pcie_add_bar (         0,    0x00000000,         0x13,      0x4);
    // pcie_add_bar (         1,    0x12300000,         0x13,      0x1);
    // pcie_add_bar (         2,    0x20000000,         0x13,      0x5);
    // pcie_add_bar (         3,    0x30000000,         0x13,      0x6);
    // pcie_add_bar (         4,    0x40000000,         0x13,      0x7);
    // pcie_add_bar (         5,    0x50000000,         0x13,      0x0);
    
    if (pcie_link_up ())
        return -1;

    rumboot_printf ("\npcie_board_test_config_with_file pass\n");
    return 0;
}

