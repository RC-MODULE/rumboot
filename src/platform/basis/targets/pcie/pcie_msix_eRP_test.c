
//-----------------------------------------------------------------------------
//  This program is for PCIe EXT_IRQ_GEN MSIX testing
//    It uses external PCIe module with mirror AXI buses.
//    Interruptions are generated by incorrect writes to Address Translator
//    Test includes:
//    - turn PCIe On
//    - setting EXT_IRQ_GEN
//    - setting Address Translator
//    - repeat few times:
//      - clear interruption effects
//      - generating interruptions with incorrect AXI writes to AT
//      - wait some time
//      - check msix vectors
//    - 
//    
//    Test duration (RTL): < 300us
//-----------------------------------------------------------------------------

#include <stdint.h>
#include <stdlib.h>

#include <rumboot/io.h>
#include <rumboot/pcie_test_lib.h>
#include <regs/regs_pcie.h>
#include <platform/devices.h>

#define interrupt_turnaround_duration 4

//------------------------------------------------------------------------
//  Send AXI transaction to Address Translator, missing all regions
//  That will cause interruption, that will create MSIX transasction
//  Data have no meaning
//  Address must miss all regions (correspond addr_trans_slv_config)
//------------------------------------------------------------------------
void create_error_transaction ()
{
    *(volatile uint32_t*) (0x7FFFF000) = 0x12345678;
}

//------------------------------------------------------------------------
//  Clear all interruption effects:
//    - status in ADDR_TRANS_SLV
//    - status in EXT_IRQ_GEN
//    - space in eSRAM
//  
//  Usefull for repeating interruptions and repeating program
//------------------------------------------------------------------------
void clear_msix_int ()
{
    uint32_t rdata;
    rdata = ioread32 (ADDR_TRANS_SLV_BASE + ADDR_TRANS_SLV_ctrl) | 0x1;
    iowrite32 (rdata, ADDR_TRANS_SLV_BASE + ADDR_TRANS_SLV_ctrl);
    iowrite32 (0x20000000, EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Status_l);
    *(volatile uint32_t*) (0x00050000 + (29 << 2)) = 0;
}

//------------------------------------------------------------------------
//  Required data - MSIX vector data, that corresponds Address
//    Translator interruption.
//  Required address - MSIX vector address, that corresponds Address
//    Translator interruption, plus address translation.
//------------------------------------------------------------------------
uint32_t check_msix_interrupt ()
{
    if (*(volatile uint32_t*) (0x00050000 + (29 << 2)) != 29)
        return -1;
    return 0;
}

uint32_t main ()
{
    rumboot_printf ("  pcie_msix_eRP_test start\n");
    
    if (pcie_turn_on_with_options_ep (1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) != 0)
    {
        rumboot_printf ("    pcie_turn_on_with_options_ep FAILED\n");
        return -1;
    }
    ext_irq_gen_config (0x2, 0x00000000, 0x00000000);
    addr_trans_slv_config (2);
    
    for (volatile uint32_t j = 0; j < 18; j++)
    {
        clear_msix_int ();
        create_error_transaction ();
        for (volatile uint32_t i = 0; i < interrupt_turnaround_duration; i++)
            ;
        if (check_msix_interrupt () != 0)
        {
            rumboot_printf ("    check_msix_interrupt FAILED\n");
            return -2;
        }
    }
        
    rumboot_printf ("  pcie_msix_eRP_test finish\n");
    return 0;
}
