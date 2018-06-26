
//-----------------------------------------------------------------------------
//  This program is for PCIe interruptions testing
//  It also tests software reset feature of PCIe. (Dont work)
//    It uses external PCIe module with mirror AXI buses.
//    Test includes:
//    - setting GIC
//    - turn PCIe On
//    - setting EXT_IRQ_GEN
//    - repeat few times
//      - clear msix irq effects
//      - send data by DMA            (must cause GIC and MSIX irq)
//      - wait until GIC irq handled
//      - wait a bit and check MSIX irq received
//    - check GIC irq was handled neccessary amount of times
//
//    Test duration (RTL): < 300us
//-----------------------------------------------------------------------------

#include <rumboot/pcie_test_lib.h>

#define interrupt_turnaround_duration 4

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <rumboot/irq.h>

#include <platform/devices.h>
#include <devices/sp804.h>

#define irq_cntr_ptr 0x00050000
#define irq_flag_ptr 0x00050008


#define pcie_dma_int_gic             55
#define pcie_dma_int_ext_irq_gen     23

#define repeat_number 5

#define data_dst_ptr 0x00051000
const long long data_dst [256];

const long long data_src [256] = {
    (long long) 0x0000000000000000l,(long long) 0x0101010101010101l,(long long) 0x0202020202020202l,(long long) 0x0303030303030303l,
    (long long) 0x0404040404040404l,(long long) 0x0505050505050505l,(long long) 0x0606060606060606l,(long long) 0x0707070707070707l,
    (long long) 0x0808080808080808l,(long long) 0x0909090909090909l,(long long) 0x0A0A0A0A0A0A0A0Al,(long long) 0x0B0B0B0B0B0B0B0Bl,
    (long long) 0x0C0C0C0C0C0C0C0Cl,(long long) 0x0D0D0D0D0D0D0D0Dl,(long long) 0x0E0E0E0E0E0E0E0El,(long long) 0x0F0F0F0F0F0F0F0Fl,
    (long long) 0x1010101010101010l,(long long) 0x1111111111111111l,(long long) 0x1212121212121212l,(long long) 0x1313131313131313l,
    (long long) 0x1414141414141414l,(long long) 0x1515151515151515l,(long long) 0x1616161616161616l,(long long) 0x1717171717171717l,
    (long long) 0x1818181818181818l,(long long) 0x1919191919191919l,(long long) 0x1A1A1A1A1A1A1A1Al,(long long) 0x1B1B1B1B1B1B1B1Bl,
    (long long) 0x1C1C1C1C1C1C1C1Cl,(long long) 0x1D1D1D1D1D1D1D1Dl,(long long) 0x1E1E1E1E1E1E1E1El,(long long) 0x1F1F1F1F1F1F1F1Fl,
    (long long) 0x2020202020202020l,(long long) 0x2121212121212121l,(long long) 0x2222222222222222l,(long long) 0x2323232323232323l,
    (long long) 0x2424242424242424l,(long long) 0x2525252525252525l,(long long) 0x2626262626262626l,(long long) 0x2727272727272727l,
    (long long) 0x2828282828282828l,(long long) 0x2929292929292929l,(long long) 0x2A2A2A2A2A2A2A2Al,(long long) 0x2B2B2B2B2B2B2B2Bl,
    (long long) 0x2C2C2C2C2C2C2C2Cl,(long long) 0x2D2D2D2D2D2D2D2Dl,(long long) 0x2E2E2E2E2E2E2E2El,(long long) 0x2F2F2F2F2F2F2F2Fl,
    (long long) 0x3030303030303030l,(long long) 0x3131313131313131l,(long long) 0x3232323232323232l,(long long) 0x3333333333333333l,
    (long long) 0x3434343434343434l,(long long) 0x3535353535353535l,(long long) 0x3636363636363636l,(long long) 0x3737373737373737l,
    (long long) 0x3838383838383838l,(long long) 0x3939393939393939l,(long long) 0x3A3A3A3A3A3A3A3Al,(long long) 0x3B3B3B3B3B3B3B3Bl,
    (long long) 0x3C3C3C3C3C3C3C3Cl,(long long) 0x3D3D3D3D3D3D3D3Dl,(long long) 0x3E3E3E3E3E3E3E3El,(long long) 0x3F3F3F3F3F3F3F3Fl,
    (long long) 0x4040404040404040l,(long long) 0x4141414141414141l,(long long) 0x4242424242424242l,(long long) 0x4343434343434343l,
    (long long) 0x4444444444444444l,(long long) 0x4545454545454545l,(long long) 0x4646464646464646l,(long long) 0x4747474747474747l,
    (long long) 0x4848484848484848l,(long long) 0x4949494949494949l,(long long) 0x4A4A4A4A4A4A4A4Al,(long long) 0x4B4B4B4B4B4B4B4Bl,
    (long long) 0x4C4C4C4C4C4C4C4Cl,(long long) 0x4D4D4D4D4D4D4D4Dl,(long long) 0x4E4E4E4E4E4E4E4El,(long long) 0x4F4F4F4F4F4F4F4Fl,
    (long long) 0x5050505050505050l,(long long) 0x5151515151515151l,(long long) 0x5252525252525252l,(long long) 0x5353535353535353l,
    (long long) 0x5454545454545454l,(long long) 0x5555555555555555l,(long long) 0x5656565656565656l,(long long) 0x5757575757575757l,
    (long long) 0x5858585858585858l,(long long) 0x5959595959595959l,(long long) 0x5A5A5A5A5A5A5A5Al,(long long) 0x5B5B5B5B5B5B5B5Bl,
    (long long) 0x5C5C5C5C5C5C5C5Cl,(long long) 0x5D5D5D5D5D5D5D5Dl,(long long) 0x5E5E5E5E5E5E5E5El,(long long) 0x5F5F5F5F5F5F5F5Fl,
    (long long) 0x6060606060606060l,(long long) 0x6161616161616161l,(long long) 0x6262626262626262l,(long long) 0x6363636363636363l,
    (long long) 0x6464646464646464l,(long long) 0x6565656565656565l,(long long) 0x6666666666666666l,(long long) 0x6767676767676767l,
    (long long) 0x6868686868686868l,(long long) 0x6969696969696969l,(long long) 0x6A6A6A6A6A6A6A6Al,(long long) 0x6B6B6B6B6B6B6B6Bl,
    (long long) 0x6C6C6C6C6C6C6C6Cl,(long long) 0x6D6D6D6D6D6D6D6Dl,(long long) 0x6E6E6E6E6E6E6E6El,(long long) 0x6F6F6F6F6F6F6F6Fl,
    (long long) 0x7070707070707070l,(long long) 0x7171717171717171l,(long long) 0x7272727272727272l,(long long) 0x7373737373737373l,
    (long long) 0x7474747474747474l,(long long) 0x7575757575757575l,(long long) 0x7676767676767676l,(long long) 0x7777777777777777l,
    (long long) 0x7878787878787878l,(long long) 0x7979797979797979l,(long long) 0x7A7A7A7A7A7A7A7Al,(long long) 0x7B7B7B7B7B7B7B7Bl,
    (long long) 0x7C7C7C7C7C7C7C7Cl,(long long) 0x7D7D7D7D7D7D7D7Dl,(long long) 0x7E7E7E7E7E7E7E7El,(long long) 0x7F7F7F7F7F7F7F7Fl,
    (long long) 0x8080808080808080l,(long long) 0x8181818181818181l,(long long) 0x8282828282828282l,(long long) 0x8383838383838383l,
    (long long) 0x8484848484848484l,(long long) 0x8585858585858585l,(long long) 0x8686868686868686l,(long long) 0x8787878787878787l,
    (long long) 0x8888888888888888l,(long long) 0x8989898989898989l,(long long) 0x8A8A8A8A8A8A8A8Al,(long long) 0x8B8B8B8B8B8B8B8Bl,
    (long long) 0x8C8C8C8C8C8C8C8Cl,(long long) 0x8D8D8D8D8D8D8D8Dl,(long long) 0x8E8E8E8E8E8E8E8El,(long long) 0x8F8F8F8F8F8F8F8Fl,
    (long long) 0x9090909090909090l,(long long) 0x9191919191919191l,(long long) 0x9292929292929292l,(long long) 0x9393939393939393l,
    (long long) 0x9494949494949494l,(long long) 0x9595959595959595l,(long long) 0x9696969696969696l,(long long) 0x9797979797979797l,
    (long long) 0x9898989898989898l,(long long) 0x9999999999999999l,(long long) 0x9A9A9A9A9A9A9A9Al,(long long) 0x9B9B9B9B9B9B9B9Bl,
    (long long) 0x9C9C9C9C9C9C9C9Cl,(long long) 0x9D9D9D9D9D9D9D9Dl,(long long) 0x9E9E9E9E9E9E9E9El,(long long) 0x9F9F9F9F9F9F9F9Fl,
    (long long) 0xA0A0A0A0A0A0A0A0l,(long long) 0xA1A1A1A1A1A1A1A1l,(long long) 0xA2A2A2A2A2A2A2A2l,(long long) 0xA3A3A3A3A3A3A3A3l,
    (long long) 0xA4A4A4A4A4A4A4A4l,(long long) 0xA5A5A5A5A5A5A5A5l,(long long) 0xA6A6A6A6A6A6A6A6l,(long long) 0xA7A7A7A7A7A7A7A7l,
    (long long) 0xA8A8A8A8A8A8A8A8l,(long long) 0xA9A9A9A9A9A9A9A9l,(long long) 0xAAAAAAAAAAAAAAAAl,(long long) 0xABABABABABABABABl,
    (long long) 0xACACACACACACACACl,(long long) 0xADADADADADADADADl,(long long) 0xAEAEAEAEAEAEAEAEl,(long long) 0xAFAFAFAFAFAFAFAFl,
    (long long) 0xB0B0B0B0B0B0B0B0l,(long long) 0xB1B1B1B1B1B1B1B1l,(long long) 0xB2B2B2B2B2B2B2B2l,(long long) 0xB3B3B3B3B3B3B3B3l,
    (long long) 0xB4B4B4B4B4B4B4B4l,(long long) 0xB5B5B5B5B5B5B5B5l,(long long) 0xB6B6B6B6B6B6B6B6l,(long long) 0xB7B7B7B7B7B7B7B7l,
    (long long) 0xB8B8B8B8B8B8B8B8l,(long long) 0xB9B9B9B9B9B9B9B9l,(long long) 0xBABABABABABABABAl,(long long) 0xBBBBBBBBBBBBBBBBl,
    (long long) 0xBCBCBCBCBCBCBCBCl,(long long) 0xBDBDBDBDBDBDBDBDl,(long long) 0xBEBEBEBEBEBEBEBEl,(long long) 0xBFBFBFBFBFBFBFBFl,
    (long long) 0xC0C0C0C0C0C0C0C0l,(long long) 0xC1C1C1C1C1C1C1C1l,(long long) 0xC2C2C2C2C2C2C2C2l,(long long) 0xC3C3C3C3C3C3C3C3l,
    (long long) 0xC4C4C4C4C4C4C4C4l,(long long) 0xC5C5C5C5C5C5C5C5l,(long long) 0xC6C6C6C6C6C6C6C6l,(long long) 0xC7C7C7C7C7C7C7C7l,
    (long long) 0xC8C8C8C8C8C8C8C8l,(long long) 0xC9C9C9C9C9C9C9C9l,(long long) 0xCACACACACACACACAl,(long long) 0xCBCBCBCBCBCBCBCBl,
    (long long) 0xCCCCCCCCCCCCCCCCl,(long long) 0xCDCDCDCDCDCDCDCDl,(long long) 0xCECECECECECECECEl,(long long) 0xCFCFCFCFCFCFCFCFl,
    (long long) 0xD0D0D0D0D0D0D0D0l,(long long) 0xD1D1D1D1D1D1D1D1l,(long long) 0xD2D2D2D2D2D2D2D2l,(long long) 0xD3D3D3D3D3D3D3D3l,
    (long long) 0xD4D4D4D4D4D4D4D4l,(long long) 0xD5D5D5D5D5D5D5D5l,(long long) 0xD6D6D6D6D6D6D6D6l,(long long) 0xD7D7D7D7D7D7D7D7l,
    (long long) 0xD8D8D8D8D8D8D8D8l,(long long) 0xD9D9D9D9D9D9D9D9l,(long long) 0xDADADADADADADADAl,(long long) 0xDBDBDBDBDBDBDBDBl,
    (long long) 0xDCDCDCDCDCDCDCDCl,(long long) 0xDDDDDDDDDDDDDDDDl,(long long) 0xDEDEDEDEDEDEDEDEl,(long long) 0xDFDFDFDFDFDFDFDFl,
    (long long) 0xE0E0E0E0E0E0E0E0l,(long long) 0xE1E1E1E1E1E1E1E1l,(long long) 0xE2E2E2E2E2E2E2E2l,(long long) 0xE3E3E3E3E3E3E3E3l,
    (long long) 0xE4E4E4E4E4E4E4E4l,(long long) 0xE5E5E5E5E5E5E5E5l,(long long) 0xE6E6E6E6E6E6E6E6l,(long long) 0xE7E7E7E7E7E7E7E7l,
    (long long) 0xE8E8E8E8E8E8E8E8l,(long long) 0xE9E9E9E9E9E9E9E9l,(long long) 0xEAEAEAEAEAEAEAEAl,(long long) 0xEBEBEBEBEBEBEBEBl,
    (long long) 0xECECECECECECECECl,(long long) 0xEDEDEDEDEDEDEDEDl,(long long) 0xEEEEEEEEEEEEEEEEl,(long long) 0xEFEFEFEFEFEFEFEFl,
    (long long) 0xF0F0F0F0F0F0F0F0l,(long long) 0xF1F1F1F1F1F1F1F1l,(long long) 0xF2F2F2F2F2F2F2F2l,(long long) 0xF3F3F3F3F3F3F3F3l,
    (long long) 0xF4F4F4F4F4F4F4F4l,(long long) 0xF5F5F5F5F5F5F5F5l,(long long) 0xF6F6F6F6F6F6F6F6l,(long long) 0xF7F7F7F7F7F7F7F7l,
    (long long) 0xF8F8F8F8F8F8F8F8l,(long long) 0xF9F9F9F9F9F9F9F9l,(long long) 0xFAFAFAFAFAFAFAFAl,(long long) 0xFBFBFBFBFBFBFBFBl,
    (long long) 0xFCFCFCFCFCFCFCFCl,(long long) 0xFDFDFDFDFDFDFDFDl,(long long) 0xFEFEFEFEFEFEFEFEl,(long long) 0xFFFFFFFFFFFFFFFFl
};

void clear_PCIe_DMA_IRQ_effects ()
{
    rgPCIe_DMA_common_udma_int = 0x1;
}

//------------------------------------------------------------------------
//  Clear all interruption effects:
//    - status in EXT_IRQ_GEN
//    - space in eSRAM
//
//  Usefull for repeating interruptions and repeating program
//------------------------------------------------------------------------
void clear_msix_PCIe_DMA_IRQ_effects ()
{
   iowrite32 (1 << pcie_dma_int_ext_irq_gen, EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Status_l);
   iowrite32 (0, 0x00050000 + (pcie_dma_int_ext_irq_gen << 2));
}

//------------------------------------------------------------------------
//  Required data - MSIX vector data, that corresponds irq.
//  Required address - MSIX vector address, that corresponds irq
//------------------------------------------------------------------------
uint32_t check_msix_PCIe_DMA_IRQ ()
{
   if (ioread32 (0x00050000 + (pcie_dma_int_ext_irq_gen << 2)) != pcie_dma_int_ext_irq_gen)
       return -1;
   return 0;
}

//------------------------------------------------------------------------
//  PCIe DMA IRQ handler function.
//    Increase irq_cntr_ptr to count irq events number
//    Set irq_flag_ptr main program to acknowledge irq recieving
//------------------------------------------------------------------------
static void PCIe_DMA_IRQ_handler (int irq, void *arg)
{
	rumboot_printf("PCIe DMA IRQ happened \n");
    iowrite32 (ioread32 (irq_cntr_ptr) + 1, irq_cntr_ptr);
    iowrite32 (1, irq_flag_ptr);
    clear_PCIe_DMA_IRQ_effects ();
}

void pcie_dma_transaction ()
{
    PCIe_DMA_desc PCIe_DMA_descriptor;

    //-------------------------------------------------------------
    //  Create descriptor for PCIe internal DMA controller
    //-------------------------------------------------------------
    PCIe_DMA_descriptor.axi_base_addr       = (uint32_t) (&data_src);
    PCIe_DMA_descriptor.axi_addr_phase      = 0;
    PCIe_DMA_descriptor.pcie_base_addr      = (uint64_t) (uint32_t) (&data_dst);
    PCIe_DMA_descriptor.tlp_header_att      = 0;
    PCIe_DMA_descriptor.transfer_length     = 8*256;
    PCIe_DMA_descriptor.control_byte        = 1;
    PCIe_DMA_descriptor.next_desc_pointer   = 0;
    //-------------------------------------------------------------
    //  Set parameters for reading descriptor
    //-------------------------------------------------------------
    rgPCIe_DMA_channel_0_ctrl = 0x2;
    rgPCIe_DMA_channel_0_sp_l = (uint32_t) (&PCIe_DMA_descriptor);
    rgPCIe_DMA_channel_0_sp_u = 0;
    rgPCIe_DMA_channel_0_attr_l = 0;
    rgPCIe_DMA_channel_0_attr_u = 0;
    //  ... and run transaction for writing
    rgPCIe_DMA_channel_0_ctrl = 0x3;
}

uint32_t main ()
{
    iowrite64 (0, irq_cntr_ptr) ;
    iowrite64 (0, irq_flag_ptr) ;

    //---------------------------------------------------------
    //  GIC configuration
    //---------------------------------------------------------
    rumboot_irq_cli();
    struct rumboot_irq_entry *tbl = rumboot_irq_create(NULL);
    rumboot_irq_set_handler(tbl, pcie_dma_int_gic, 0, PCIe_DMA_IRQ_handler, NULL);
    /* Activate the table */
    rumboot_irq_table_activate(tbl);
    rumboot_irq_enable(pcie_dma_int_gic);
    rumboot_irq_sei();

    //---------------------------------------------------------
    //  Turn PCIe On
    //---------------------------------------------------------
    if (pcie_turn_on_with_options_ep (1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) != 0)
        return -1;
    //---------------------------------------------------------
    //  Configure and enable MSIX
    //---------------------------------------------------------
    ext_irq_gen_config (0x2, 0x00000000, 0x00000000);
    //---------------------------------------------------------
    //  Configure AT Slave, because default MSIX addresses
    //    are (0x40050000 + offset) and must be shifted
    //---------------------------------------------------------
    addr_trans_slv_config (2);
    //---------------------------------------------------------
    //  Enable DMA interruptions
    //---------------------------------------------------------
    iowrite32 (0x1, PCIE_CORE_BASE + PCIe_Core_DMAConfig + PCIe_DMA_common_udma_int_ena);

    for (volatile uint32_t i = 0; i < repeat_number; i++)
    {
        clear_msix_PCIe_DMA_IRQ_effects ();

        //---------------------------------------------------------
        //  Transmit data by internal PCIe DMA
        //---------------------------------------------------------
        pcie_dma_transaction ();

        //---------------------------------------------------------
        //  Wait interrupt handled by GIC
        //    It should be made at the beginning of previous cycle.
        //---------------------------------------------------------
        while (ioread32 (irq_flag_ptr) != 1)
        {
        }
        iowrite32 (0, irq_flag_ptr) ;
        //---------------------------------------------------------
        //  Wait MSIX come back
        //    And check it
        //---------------------------------------------------------
        for (volatile uint32_t i = 0; i < interrupt_turnaround_duration; i++)
            ;
        if (check_msix_PCIe_DMA_IRQ () != 0)
            return -2;
        //---------------------------------------------------------
        //  Not clear why, but PCIe doesn't work after reset.
        //  Investigation is underway.
        //---------------------------------------------------------
        // //---------------------------------------------------------
        // //  Disable and clear DMA interruptions before reset
        // //---------------------------------------------------------
        // rgPCIe_DMA_common_udma_int_dis = 0x1;
        // //---------------------------------------------------------
        // //  PCIe reset
        // //---------------------------------------------------------
        // rgSCTL_PCIE_RST = 0x0;
        // //---------------------------------------------------------
        // //  Turn PCIe On
        // //---------------------------------------------------------
        // if (pcie_turn_on_with_options_ep (1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) != 0)
            // return -1;

    }

    //---------------------------------------------------------
    //  Check, that irq handler was called neccessary times
    //---------------------------------------------------------
    if (ioread32 (irq_cntr_ptr) != repeat_number)
    {
        return ioread32 (irq_cntr_ptr);
    }

    return 0;
}
