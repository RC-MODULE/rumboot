
//-----------------------------------------------------------------------------
//  This program is for PCIe DMA simple testing
//    It uses external PCIe module with mirror AXI buses.
//    Test includes:
//    - creation of etalon array in esram0
//    - clearing data_mid and data_dst space (you can rerun program without SoC reset)
//    - PCIe turning ON
//    - creation of PCIe internal DMA descriptors for write
//    - write data data_src -> ext PCIe -> data_mid
//    - creation of PCIe internal DMA descriptors for read
//    - reading data data_mid -> ext PCIe -> data_dst
//    - check that data_src == data_dst
//    
//    Test duration (RTL): < 300us
//-----------------------------------------------------------------------------

#include <stdint.h>
#include <stdlib.h>

#include <rumboot/io.h>
#include <rumboot/pcie_test_lib.h>
#include <regs/regs_pcie.h>
#include <platform/devices.h>


#ifndef increase_test_duration
#define increase_test_duration 0
#endif


#if increase_test_duration == 1
#define write_duration 13*2
#define read_duration 20*2
#else
#define write_duration 13
#define read_duration 20
#endif


    uint32_t data_dst [256];
    uint32_t data_mid [256];
    uint32_t data_src [256];

//------------------------------------------------------------------------
//  Create reference data array for next transactions
//  4K bytes
//------------------------------------------------------------------------
void create_etalon_array ()
{
    for (uint32_t i = 0; i < 256; i++)
    {
        data_src [i] = i + (i << 8) + (i << 16) + (i << 24);
    }
}

//------------------------------------------------------------------------
//  Clear space for destination array
//  It may be helpfull, if you cannot reset system between tests call
//------------------------------------------------------------------------
void clear_space (uint32_t* this_addr_first, uint32_t* this_addr_last)
{
    uint32_t* address;
    for (address = this_addr_first; address <= this_addr_last; address++)
    {
        *address = 0x00000000;
    }
}


uint32_t pcie_dma_transaction ()
{
    PCIe_DMA_desc PCIe_DMA_descriptor;
    
    //  Allow DMA interruptions
    // rgPCIe_DMA_common_udma_int_ena = 0x1;
    //-------------------------------------------------------------
    //  Create descriptor for PCIe internal DMA controller
    //-------------------------------------------------------------
    PCIe_DMA_descriptor.axi_base_addr       = (uint32_t) (&data_src);
    PCIe_DMA_descriptor.axi_addr_phase      = 0;
    PCIe_DMA_descriptor.pcie_base_addr      = (uint64_t) (uint32_t) (&data_mid);
    PCIe_DMA_descriptor.tlp_header_att      = 0;
    PCIe_DMA_descriptor.transfer_length     = 1024;
    PCIe_DMA_descriptor.control_byte        = 1;
    PCIe_DMA_descriptor.next_desc_pointer   = 0;
    //-------------------------------------------------------------
    //  Set parameters for reading descriptor
    //-------------------------------------------------------------
    iowrite32 ( 0x2, PCIE_CORE_BASE + PCIe_Core_DMAConfig + PCIe_DMA_channel_0_ctrl  );
    iowrite32 ((uint32_t) (&PCIe_DMA_descriptor), PCIE_CORE_BASE + PCIe_Core_DMAConfig + PCIe_DMA_channel_0_sp_l  );
    iowrite32 ( 0x0, PCIE_CORE_BASE + PCIe_Core_DMAConfig + PCIe_DMA_channel_0_sp_u  );
    iowrite32 ( 0x0, PCIE_CORE_BASE + PCIe_Core_DMAConfig + PCIe_DMA_channel_0_attr_l);
    iowrite32 ( 0x0, PCIE_CORE_BASE + PCIe_Core_DMAConfig + PCIe_DMA_channel_0_attr_u);
    //  ... and run transaction for writing
    iowrite32 (0x3, PCIE_CORE_BASE + PCIe_Core_DMAConfig + PCIe_DMA_channel_0_ctrl );
    //-------------------------------------------------------------
    //  Wait while transaction complete in our side
    //-------------------------------------------------------------
    for (volatile uint32_t i = 0; i <= write_duration; i++)
    {
    }
    if (data_mid [255] != data_src [255])
        return -1;
    
    //-------------------------------------------------------------
    //  Create descriptor for PCIe internal DMA controller
    //-------------------------------------------------------------
    PCIe_DMA_descriptor.axi_base_addr       = (uint32_t) (&data_dst);
    PCIe_DMA_descriptor.axi_addr_phase      = 0;
    PCIe_DMA_descriptor.pcie_base_addr      = (uint64_t) (uint32_t) (&data_mid);
    PCIe_DMA_descriptor.tlp_header_att      = 0;
    PCIe_DMA_descriptor.transfer_length     = 1024;
    PCIe_DMA_descriptor.control_byte        = 1;
    PCIe_DMA_descriptor.next_desc_pointer   = 0;
    //-------------------------------------------------------------
    //  Set parameters for reading descriptor
    //-------------------------------------------------------------
    iowrite32 ( 0x0, PCIE_CORE_BASE + PCIe_Core_DMAConfig + PCIe_DMA_channel_0_ctrl  );
    iowrite32 ((uint32_t) (&PCIe_DMA_descriptor), PCIE_CORE_BASE + PCIe_Core_DMAConfig + PCIe_DMA_channel_0_sp_l  );
    iowrite32 ( 0x0, PCIE_CORE_BASE + PCIe_Core_DMAConfig + PCIe_DMA_channel_0_sp_u  );
    iowrite32 ( 0x0, PCIE_CORE_BASE + PCIe_Core_DMAConfig + PCIe_DMA_channel_0_attr_l);
    iowrite32 ( 0x0, PCIE_CORE_BASE + PCIe_Core_DMAConfig + PCIe_DMA_channel_0_attr_u);
    //  ... and run transaction for writing
    iowrite32 (0x1, PCIE_CORE_BASE + PCIe_Core_DMAConfig + PCIe_DMA_channel_0_ctrl );
    //-------------------------------------------------------------
    //  Wait while transaction complete in our side
    //-------------------------------------------------------------
    for (volatile uint32_t i = 0; i <= read_duration; i++)
    {
    }
    if (data_dst [255] != data_src [255])
        return -1;
    
    return 0;
}

//-----------------------------------------------------------------------------
uint32_t Check_transaction ()
{
    rumboot_printf ("    Check_transaction start\n");
    for (uint32_t i = 0; i < 256; i+=8)
    {
        if (
            (data_dst [i    ] != data_src [i    ]) ||    //    mass
            (data_dst [i + 1] != data_src [i + 1]) ||    //    check
            (data_dst [i + 2] != data_src [i + 2]) ||    //    reduce
            (data_dst [i + 3] != data_src [i + 3]) ||    //    execution
            (data_dst [i + 4] != data_src [i + 4]) ||    //    time
            (data_dst [i + 5] != data_src [i + 5]) ||    //    
            (data_dst [i + 6] != data_src [i + 6]) ||    //    
            (data_dst [i + 7] != data_src [i + 7])
            )
            return -1;
    }
    return 0;
}


uint32_t main ()
{
    rumboot_printf ("  pcie_dma_eRP_test start\n");
    
    create_etalon_array ();
    clear_space         ((uint32_t*) (&data_mid), (uint32_t*) (&data_mid) + 256);
    clear_space         ((uint32_t*) (&data_dst), (uint32_t*) (&data_dst) + 256);
    if (pcie_turn_on_with_options_ep (1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) != 0)
    {
        rumboot_printf ("    pcie_turn_on_with_options_ep FAILED\n");
        return -1;
    }
    if (pcie_dma_transaction () != 0)
    {
        rumboot_printf ("    pcie_dma_transaction FAILED\n");
        return -2;
    }
    if (Check_transaction () != 0)
    {
        rumboot_printf ("    Check_transaction FAILED\n");
        return -3;
    }
    
    rumboot_printf ("  pcie_dma_eRP_test finish\n");
    return 0;
}
