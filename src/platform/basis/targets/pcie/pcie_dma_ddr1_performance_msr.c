
//-----------------------------------------------------------------------------
//  This program is for measurement PCIe.DMA <-> DDR1 latency and bandwidth
//    It uses external PCIe module with mirror AXI buses.
//    Test includes:
//    - turn GP_TIMER On
//    - DDR1 turning ON
//    - creation of etalon array in esram0
//    - clearing data_mid and data_dst space (you can rerun program without SoC reset)
//    - PCIe turning ON
//    - 
//    - configure PCIe ADT
//    - send few single transactions to measure latency
//    - 
//    - get GP_TIMER free run counter
//    - transmit array with PCIe.DMA
//    - get GP_TIMER free run counter
//    - 
//    - get GP_TIMER free run counter
//    - receive array with PCIe.DMA
//    - get GP_TIMER free run counter
//    - 
//    
//    Test duration (RTL): 
//-----------------------------------------------------------------------------

#include <rumboot/printf.h>
#include <rumboot/io.h>

#include <rumboot/pcie_test_lib.h>
#include <rumboot/ddr_test_lib.h>
#include <regs/regs_pcie.h>

#include <devices/gp_timers.h>

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


#define DATA_SIZE   1024

#define data_mid ((uint32_t*) 0xC0000000)

uint32_t data_dst [DATA_SIZE];
uint32_t data_src [DATA_SIZE];

//------------------------------------------------------------------------
//  Configure address translator so, that
//    transaction address += 0x8000_0000 and transactions go to DDR1
//------------------------------------------------------------------------
void set_adt_slv ()
{
    iowrite32 (0x40000000 | 0x1, ADDR_TRANS_SLV_BASE + ((2 * 3 + 1) << 2)) ;  //  base address, enable
    iowrite32 (0x7FFFFFFF      , ADDR_TRANS_SLV_BASE + ((2 * 3 + 2) << 2)) ;  //  end address
    iowrite32 (0x80000000      , ADDR_TRANS_SLV_BASE + ((2 * 3 + 3) << 2)) ;  //  translate address
    iowrite32 (0x00000000      , ADDR_TRANS_SLV_BASE);
}

static inline void adt_slv_bypass_on ()
{
    iowrite32 (ioread32 (ADDR_TRANS_SLV_BASE) | 0x4, ADDR_TRANS_SLV_BASE);
}

//------------------------------------------------------------------------
//  Create reference data array for next transactions
//  1K bytes
//------------------------------------------------------------------------
void create_etalon_array (uint32_t* address, uint32_t size)
{
    for (volatile uint32_t i = 0; i < size; i++)
    {
        *address = i + (i<<8) + (i<<16) + (i<<24);
        address++;
    }
}

//------------------------------------------------------------------------
//  Nullify destination space.
//  Useful for repeating writes.
//  1K bytes
//------------------------------------------------------------------------
void clear_destination_space (uint32_t* address, uint32_t size)
{
    for (volatile uint32_t i = 0; i < size; i++)
    {
        *address = 0x00000000;
        address++;
    }
}

void pcie_single_transaction ()
{
    iowrite32 (0x11235813, PCIE_BASE);
    
    for (volatile uint32_t i = 0; i < 10; i++)
        ;
    
    ioread32 (PCIE_BASE);
}

void pcie_dma_transaction (uint32_t size_byte)
{
    volatile PCIe_DMA_desc PCIe_DMA_descriptor;
    
    uint32_t rdata;
    
    //  Allow DMA interruptions
    // rgPCIe_DMA_common_udma_int_ena = 0x1;
    //-------------------------------------------------------------
    //  Create descriptor for PCIe internal DMA controller
    //-------------------------------------------------------------
    PCIe_DMA_descriptor.axi_base_addr       = (uint32_t) (data_mid);
    PCIe_DMA_descriptor.axi_addr_phase      = 0;
    PCIe_DMA_descriptor.pcie_base_addr      = (uint64_t) (uint32_t) (&data_src);
    PCIe_DMA_descriptor.tlp_header_att      = 0;
    PCIe_DMA_descriptor.transfer_length     = size_byte;
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
    rdata = ioread32 (PCIE_CORE_BASE + PCIe_Core_DMAConfig + PCIe_DMA_channel_0_ctrl);
    rumboot_printf("Write to DDR    %d DWORD array\n", DATA_SIZE);
    gp_timer_show_free_run ();
    iowrite32 (rdata |= 0x1, PCIE_CORE_BASE + PCIe_Core_DMAConfig + PCIe_DMA_channel_0_ctrl );
    //-------------------------------------------------------------
    //  Wait while transaction complete in our side
    //-------------------------------------------------------------
    while ((ioread32 (PCIE_CORE_BASE + PCIe_Core_DMAConfig + PCIe_DMA_common_udma_int) & 0x1) == 0)
        ;
    gp_timer_show_free_run ();
    iowrite32 (0x1, PCIE_CORE_BASE + PCIe_Core_DMAConfig + PCIe_DMA_common_udma_int);
    
    //-------------------------------------------------------------
    //  Create descriptor for PCIe internal DMA controller
    //-------------------------------------------------------------
    PCIe_DMA_descriptor.axi_base_addr       = (uint32_t) (data_mid);
    PCIe_DMA_descriptor.axi_addr_phase      = 0;
    PCIe_DMA_descriptor.pcie_base_addr      = (uint64_t) (uint32_t) (&data_dst);
    PCIe_DMA_descriptor.tlp_header_att      = 0;
    PCIe_DMA_descriptor.transfer_length     = size_byte;
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
    rdata = ioread32 (PCIE_CORE_BASE + PCIe_Core_DMAConfig + PCIe_DMA_channel_0_ctrl);
    rumboot_printf("Read from DDR    %d DWORD array\n", DATA_SIZE);
    gp_timer_show_free_run ();
    iowrite32 (rdata |= 0x1, PCIE_CORE_BASE + PCIe_Core_DMAConfig + PCIe_DMA_channel_0_ctrl );
    //-------------------------------------------------------------
    //  Wait while transaction complete in our side
    //-------------------------------------------------------------
    while ((ioread32 (PCIE_CORE_BASE + PCIe_Core_DMAConfig + PCIe_DMA_common_udma_int) & 0x1) == 0)
        ;
    gp_timer_show_free_run ();
    iowrite32 (0x1, PCIE_CORE_BASE + PCIe_Core_DMAConfig + PCIe_DMA_common_udma_int);
}

uint32_t main ()
{
    uint32_t dbg_msg_nmbr = 0;
    
    rumboot_printf("This is _pcie_dma_ddr_performance_msr\n");
    rumboot_printf("_dbg %d\n", dbg_msg_nmbr++);
    gp_timer_turn_on ();
    if (ddr_init (DDR1_BASE) != 0)
        return -1;
    create_etalon_array         (data_src, DATA_SIZE);
    clear_destination_space     (data_mid, DATA_SIZE);
    clear_destination_space     (data_dst, DATA_SIZE);
    if (pcie_turn_on_with_options_ep (1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) != 0)
        return -2;
    rumboot_printf("_dbg %d\n", dbg_msg_nmbr++);
    
    
    set_adt_slv ();
    pcie_single_transaction ();
    pcie_single_transaction ();
    pcie_single_transaction ();
    pcie_single_transaction ();
    adt_slv_bypass_on ();
    
    
    rumboot_printf("_dbg %d\n", dbg_msg_nmbr++);
    
    
    if (gp_timer_present () != 0)
        return -3;
    
    pcie_dma_transaction (DATA_SIZE << 2);
    
    return 0;
}
