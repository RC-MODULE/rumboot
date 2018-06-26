
//-----------------------------------------------------------------------------
//  This program is for testing PCIe AXI_master to PCIe AXI_slave access
//    It uses external PCIe module with mirror AXI buses.
//    Test includes:
//    - turn DDR0 On
//    - clear space in DDR0
//    - turn PCIe On
//    - setting Address Translator Master
//    - send transaction, check results
//    
//    Test duration (RTL): < 250us
//
//-----------------------------------------------------------------------------
#include <rumboot/ddr_test_lib.h>
#include <rumboot/pcie_test_lib.h>
#include <rumboot/io.h>

void clear_ddr0_space ()
{
    iowrite32 (0, EMI0_BASE + 0x0);
    iowrite32 (0, EMI0_BASE + 0x4);
    iowrite32 (0, EMI0_BASE + 0x8);
    iowrite32 (0, EMI0_BASE + 0xC);
}

//------------------------------------------------------------------------
//  Configure address translator so, that
//    transaction address += 0x2000_0000
//  Then, address is: 0x4000_0000 -> 0x6000_0000 -> 0x8000_0000
//  Transaction goes from PCIe_AXI_master port to PCIe_AXI_slave port and
//    then to DDR0, where from it could be read
//------------------------------------------------------------------------
void set_pcie_to_pcie_access ()
{
    iowrite32 (0x40000000 | 0x1, ADDR_TRANS_MST_BASE + ((2 * 3 + 1) << 2)) ;  //  base address, enable
    iowrite32 (0x7FFFFFFF      , ADDR_TRANS_MST_BASE + ((2 * 3 + 2) << 2)) ;  //  end address
    iowrite32 (0x20000000      , ADDR_TRANS_MST_BASE + ((2 * 3 + 3) << 2)) ;  //  translate address
    iowrite32 (0, ADDR_TRANS_MST_BASE);
}

uint32_t check_access ()
{
    iowrite32 (0x11235813, PCIE_BASE);
    
    for (volatile uint32_t i = 0; i < 10; i++)
        ;
    
    if (ioread32 (EMI0_BASE) != 0x11235813)
        return -1;
    
    return 0;
}

int main ()
{
    if (ddr_init (DDR0_BASE) != 0)
        return -1;
    
    clear_ddr0_space ();
    
    if (pcie_turn_on_with_options_ep (1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) != 0)
        return -2;
    
    set_pcie_to_pcie_access ();
        
    if (check_access () != 0)
        return -3;
    
    return 0;
}
