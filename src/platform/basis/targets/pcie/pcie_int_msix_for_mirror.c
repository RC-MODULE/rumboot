
//-----------------------------------------------------------------------------
//  This program shows, how to use Ext_IRQ_Gen for PCIe mirror tests
//    It uses external PCIe module with mirror AXI buses.
//    Interruptions are generated by incorrect writes to Address Translator
//    Test includes:
//    - turn PCIe On
//    - setting EXT_IRQ_GEN
//    
//    Test duration (RTL): < TODO
//-----------------------------------------------------------------------------

#include <stdint.h>
#include <stdlib.h>

#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <rumboot/printf.h>
#include <rumboot/pcie_test_lib.h>
#include <regs/regs_pcie.h>
#include <platform/devices.h>

#include <platform/interrupts.h>

#include <regs/regs_gic.h>


#define ADTRANS_SLV_IRQ_SPI_SHORT (ADTRANS_SLV_IRQ_SPI - 32)

#define INTERRUPT_TURNAROUND_DURATION 100

//-------------------------------------------------------------------------
//  Send AXI transaction to Address Translator, missing all regions
//  That will cause interruption, that will create MSIX transasction
//  Data have no meaning
//  Address must miss all regions (correspond addr_trans_slv_config)
//-------------------------------------------------------------------------
static inline void create_error_transaction ()
{
    iowrite32 (0x12345678, 0x7FFFF000);
}

//-------------------------------------------------------------------------
//  Clear interruption effects in initial source:
//    - status in ADDR_TRANS_SLV
//  
//  Usefull for repeating interruptions and repeating program
//-------------------------------------------------------------------------
static inline void clear_int_ADDR_TRANS_SLV ()
{
    iowrite32 (0x1, ADDR_TRANS_SLV_BASE + ADDR_TRANS_SLV_ctrl);
}

static inline void setup_ADDR_TRANS_SLV ()
{
    iowrite32 (0, ADDR_TRANS_SLV_BASE + ADDR_TRANS_SLV_ctrl);
    iowrite32 (0x40000001, ADDR_TRANS_SLV_BASE + ADDR_TRANS_SLV_region0_base);
    iowrite32 (0x7FFFE000, ADDR_TRANS_SLV_BASE + ADDR_TRANS_SLV_region0_end );
    iowrite32 (0x00000000, ADDR_TRANS_SLV_BASE + ADDR_TRANS_SLV_region0_tran);
}





//-------------------------------------------------------------------------
//  These functions control exact type of interruptions
//
//  In the start of program disable all interruptions and then enable
//    neccessary type
//
//-------------------------------------------------------------------------
static inline void Common_Enable_MSIX ()
{
    iowrite32 (0x80000000, PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_msix_ctrl);
}

static inline void Common_Enable_Legacy ()
{
    uint32_t rdata;
    rdata = ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Ctrl);
    iowrite32 (rdata | 0x8, EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Ctrl);
}

static inline void Common_Enable_External ()
{
    uint32_t rdata;
    rdata = ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Ctrl);
    iowrite32 (rdata | 0x1, EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Ctrl);
}

static inline void Disable_all_int ()
{
    iowrite32 (0x00000000, PCIE_CORE_BASE + PCIe_Core_FuncRPConfig + PCIe_EP_i_msix_ctrl);
    iowrite32 (0x00000000, EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Ctrl);
}

//-------------------------------------------------------------------------
//  Function enables selected input interruption to be elaborated by
//    Ext_IRQ_Gen
//
//  Dont forget, that exact type of interruptions (External, Legacy, MSIX)
//    must be enabled separately
//
//  Also, dont forget, that each MSIX vector has its own mask
//
//-------------------------------------------------------------------------
void Enable_IRQ (uint32_t IRQ_number)
{
    uint32_t rdata;
    if (IRQ_number <= 31)
    {
        rdata = ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Mask_l);
        iowrite32 (rdata & (~(1 << IRQ_number)), EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Mask_l);
    }
    else
    {
        rdata = ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Mask_h);
        iowrite32 (rdata & (~(1 << (IRQ_number - 32))), EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Mask_h);
    }
}

//-------------------------------------------------------------------------
//  Function disables selected input interruption to be elaborated by
//    Ext_IRQ_Gen
//
//  Interruption, disabled in such way, wont set Pending status in PCIe
//    configuration space registers
//-------------------------------------------------------------------------
void Disable_IRQ (uint32_t IRQ_number)
{
    uint32_t rdata;
    if (IRQ_number <= 31)
    {
        rdata = ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Mask_l);
        iowrite32 (rdata | (1 << IRQ_number), EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Mask_l);
    }
    else
    {
        rdata = ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Mask_h);
        iowrite32 (rdata | (1 << (IRQ_number - 32)), EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Mask_h);
    }
}

//-------------------------------------------------------------------------
//  Function enables all input interruptions to be elaborated by
//    Ext_IRQ_Gen
//
//  Dont forget, that exact type of interruptions (External, Legacy, MSIX)
//    must be enabled separately
//
//  Also, dont forget, that each MSIX vector has its own mask
//
//-------------------------------------------------------------------------
static inline void Enable_all_IRQs ()
{
    iowrite32 (0, EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Mask_l);
    iowrite32 (0, EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Mask_h);
}

//-------------------------------------------------------------------------
//  Function configures one MSIX vector
//-------------------------------------------------------------------------
void Set_MSIX_vector (uint32_t vector_number, uint32_t addr, uint32_t data, uint32_t mask)
{
    uint32_t addr_pointer = EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_v0_Message_Address;
    addr_pointer += (vector_number << 4);
    iowrite32 (addr, addr_pointer);
    addr_pointer += 8;
    iowrite32 (data, addr_pointer);
    addr_pointer += 4;
    iowrite32 (mask, addr_pointer);
}

//-------------------------------------------------------------------------
//  Function returns number of first interruption from Ext_IRQ_Gen
//    Compare it with Basis IRQ map to find interruption source device
//-------------------------------------------------------------------------
uint32_t Get_IRQ_number_lowest ()
{
    uint32_t rdata;
    uint32_t IRQ_number;
    
    rdata = ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Status_l);
    if (rdata != 0)
    {
        IRQ_number = 0;
        while ((rdata & 0x1) != 1)
        {
            rdata >>= 1;
            IRQ_number++;
        }
    }
    else
    {
        rdata = ioread32 (EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Status_h);
        IRQ_number = 32;
        while ((rdata & 0x1) != 1)
        {
            rdata >>= 1;
            IRQ_number++;
        }
    }
    
    return IRQ_number;
}

//-------------------------------------------------------------------------
//  Function to clear selected IRQ in Ext_IRQ_Gen
//-------------------------------------------------------------------------
void Clear_IRQ_status (uint32_t IRQ_number)
{
    if (IRQ_number <= 31)
        iowrite32 (1 << IRQ_number, EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Status_l);
    else
        iowrite32 (1 << (IRQ_number - 32), EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Status_h);
}

//-------------------------------------------------------------------------
//  Function to clear all IRQ status in Ext_IRQ_Gen
//-------------------------------------------------------------------------
static inline void Clear_all_IRQ_status ()
{
    iowrite32 (0xFFFFFFFF, EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Status_l);
    iowrite32 (0xFFFFFFFF, EXT_IRQ_GEN_BASE + EXT_IRQ_GEN_Global_IRQ_Status_h);
}

//-------------------------------------------------------------------------
//  Configuration of Ext_IRQ_Gen
//  
//  Set required vectors here
//  Now only one vector set
//-------------------------------------------------------------------------
void Setup_Ext_IRQ_Gen_for_mirror ()
{
    Disable_all_int ();
    Common_Enable_MSIX ();
    Enable_all_IRQs ();
    
    Set_MSIX_vector (ADTRANS_SLV_IRQ_SPI_SHORT, PCIE_BASE + GIC_DIST_BASE + GICD_REG_SGIR, GIC_GENSWINT0, 0);
}

static void IRQN_SW0_IRQ_handler (int irq, void *arg)
{
    rumboot_printf("IRQN_SW0 IRQ happened \n");
    clear_int_ADDR_TRANS_SLV ();
    Clear_IRQ_status (ADTRANS_SLV_IRQ_SPI_SHORT);
	// rumboot_irq_free(tbl);  TODO
}

//-------------------------------------------------------------------------
//  Test sequence
//-------------------------------------------------------------------------
uint32_t main ()
{
    //-----------------------------------------------------------------
    //  GIC configuration
    //-----------------------------------------------------------------
    rumboot_irq_cli();
    struct rumboot_irq_entry *tbl = rumboot_irq_create(NULL);
    rumboot_irq_set_handler(tbl, IRQN_SW0, 0, IRQN_SW0_IRQ_handler, NULL);
    /* Activate the table */
    rumboot_irq_table_activate(tbl);
    rumboot_irq_enable(IRQN_SW0);
    rumboot_irq_sei();
    //-----------------------------------------------------------------
    
    if (pcie_mirror_tests_setup () != 0)
        return -1;
    
    
    setup_ADDR_TRANS_SLV ();
    Setup_Ext_IRQ_Gen_for_mirror ();
    
    
    //-----------------------------------------------------------------
    //  Create few interrupts
    //-----------------------------------------------------------------
    for (uint32_t j = 0; j <= 3; j++)
    {
        create_error_transaction ();
        for (volatile uint32_t i = 0; i < INTERRUPT_TURNAROUND_DURATION; i++)
            ;
    }
        
    return 0;
}

