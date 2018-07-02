
//-----------------------------------------------------------------------------
//  This program is for PCIe EXT_IRQ_GEN External interruptions testing
//    Interruptions are generated by incorrect writes to Address Translator
//    Test includes:
//    - setting EXT_IRQ_GEN
//    - setting Address Translator
//    - repeat few times:
//      - clear interruption effects
//      - generating interruptions with incorrect AXI writes to AT
//      - wait some time
//      - check INT value
//    - 
//    
//    Test duration (RTL): < 
//-----------------------------------------------------------------------------

#include <rumboot/pcie_test_lib.h>
#include <platform/defs_c.h>

#define interrupt_turnaround_duration 20
#define interrupt_repeat_number 16

#define interrupt_counter (*(volatile uint32_t*) 0x00050000)

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
void clear_external_int_status ()
{
    rgADDR_TRANS_SLV_ctrl |= 0x1;
    rgEXT_IRQ_GEN_Global_IRQ_Status_l = 0x20000000;
    interrupt_counter = 0;
}

uint32_t check_legacy_interrupt ()
{
    if (interrupt_counter != interrupt_repeat_number)
        return -1;
    return 0;
}

uint32_t main ()
{
    if (pcie_turn_on_with_options_ep (1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) != 0)
        return -1;
    ext_irq_gen_config (0x1, 0x00000000, 0x00000003);
    addr_trans_slv_config (2);
    clear_external_int_status ();
    
    for (volatile uint32_t j = 0; j < interrupt_repeat_number; j++)
    {
        create_error_transaction ();
        for (volatile uint32_t i = 0; i < interrupt_turnaround_duration; i++)
            ;
    }
    
    if (check_legacy_interrupt () != 0)
        return -1;
        
    return 0;
}
