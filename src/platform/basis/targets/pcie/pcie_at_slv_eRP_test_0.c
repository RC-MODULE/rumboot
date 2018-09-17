
//-----------------------------------------------------------------------------
//  This program is for PCIe outbound Address Translator testing
//    
//    
//    Test includes:
//    - turn PCIe On
//    - turn DDR0 On
//    - repeat for few regions
//      - repeat for few AT settings:
//        - setting outbound Address Translator (one region)
//        - write data (ARM -> outb AT -> PCIe -> inb AT -> DDR0)
//        - read and check data (ARM <- outb AT <- PCIe <- inb AT <- DDR0)
//        - read and check data (ARM <- DDR0)
//        - clear data in DDR0
//      - clear settings of current region
//      - 
//      - 
//    - 
//    
//    Better check all regions, while testing without short_test_for_sim
//    
//    Test duration (RTL, short_test_for_sim): < 1.6ms
//    Test duration (RTL): < 
//-----------------------------------------------------------------------------

#include <rumboot/pcie_test_lib.h>
#include <rumboot/ddr_test_lib.h>
#include <rumboot/io.h>
#include <regs/regs_pcie.h>
#include <platform/devices.h>

#define array_depth 29

#if short_test_for_sim == 1
#define regions_for_test_number 4

    const uint32_t regions_for_test [regions_for_test_number] =
    {
         0 ,
         2 ,
         8 ,
        32
    };
#else
#define regions_for_test_number 8

    const uint32_t regions_for_test [regions_for_test_number] =
    {
         0 ,
         1 ,
         2 ,
         4 ,
         8 ,
        16 ,
        32 ,
        64
    };
#endif
    
    //---------------------------------------------------------
    //  This array contains:
    //    base address of region
    //---------------------------------------------------------
    const uint32_t base_addr_array [array_depth] =
    {
        0x40000000 ,
        0x40000008 ,
        0x40000010 ,
        0x40000020 ,
        0x40000040 ,
        0x40000080 ,
        0x40000100 ,
        0x40000200 ,
        0x40000400 ,
        0x40000800 ,
        0x40001000 ,
        0x40002000 ,
        0x40004000 ,
        0x40008000 ,
        0x40010000 ,
        0x40020000 ,
        0x40040000 ,
        0x40080000 ,
        0x40100000 ,
        0x40200000 ,
        0x40400000 ,
        0x40800000 ,
        0x41000000 ,
        0x42000000 ,
        0x44000000 ,
        0x48000000 ,
        0x50000000 ,
        0x60000000 ,
        0x6FFFFFF0
    };
    //---------------------------------------------------------
    //  This array contains:
    //    end address of region
    //---------------------------------------------------------
    const uint32_t end_addr_array [array_depth] =
    {
        0x40000008 ,
        0x40000010 ,
        0x40000020 ,
        0x40000040 ,
        0x40000080 ,
        0x40000100 ,
        0x40000200 ,
        0x40000400 ,
        0x40000800 ,
        0x40001000 ,
        0x40002000 ,
        0x40004000 ,
        0x40008000 ,
        0x40010000 ,
        0x40020000 ,
        0x40040000 ,
        0x40080000 ,
        0x40100000 ,
        0x40200000 ,
        0x40400000 ,
        0x40800000 ,
        0x41000000 ,
        0x42000000 ,
        0x44000000 ,
        0x48000000 ,
        0x50000000 ,
        0x60000000 ,
        0x6FFFFFF0 ,
        0x6FFFFFF8
    };
    //---------------------------------------------------------
    //  This array contains:
    //    address shift (to DDR0)
    //---------------------------------------------------------
    const uint32_t tran_addr_array [array_depth] =
    {
        0x40000008 ,
        0x40000010 ,
        0x40000020 ,
        0x40000040 ,
        0x40000080 ,
        0x40000100 ,
        0x40000200 ,
        0x40000400 ,
        0x40000800 ,
        0x40001000 ,
        0x40002000 ,
        0x40004000 ,
        0x40008000 ,
        0x40010000 ,
        0x40020000 ,
        0x40040000 ,
        0x40080000 ,
        0x40100000 ,
        0x40200000 ,
        0x40400000 ,
        0x40800000 ,
        0x41000000 ,
        0x42000000 ,
        0x44000000 ,
        0x48000000 ,
        0x49000000 ,
        0x41234568 ,
        0x4FFFFFF0 ,
        0x50000000
    };
    
uint32_t at_slv_test_0 ()
{
    
    iowrite32 (0x0, ADDR_TRANS_SLV_BASE + ADDR_TRANS_SLV_ctrl);
    
    for (volatile uint32_t j = 0; j < regions_for_test_number; j++)
    {
        for (volatile uint32_t i = 0; i < array_depth; i++)
        {
            volatile uint32_t base_addr ;
            volatile uint32_t end_addr  ;
            volatile uint32_t mid_addr  ;
            
            volatile uint32_t direct_base_addr ;
            volatile uint32_t direct_end_addr  ;
            volatile uint32_t direct_mid_addr  ;
            
            iowrite32(base_addr_array [i] | 0x1, ADDR_TRANS_SLV_BASE + ((regions_for_test [j] * 3 + 1) << 2)) ;
            iowrite32(end_addr_array  [i]      , ADDR_TRANS_SLV_BASE + ((regions_for_test [j] * 3 + 2) << 2)) ;
            iowrite32(tran_addr_array [i]      , ADDR_TRANS_SLV_BASE + ((regions_for_test [j] * 3 + 3) << 2)) ;
            // iowrite32(base_addr_array [i] | 0x1, ADDR_TRANS_SLV_BASE + 0x4) ;
            // iowrite32(end_addr_array  [i]      , ADDR_TRANS_SLV_BASE + 0x8) ;
            // iowrite32(tran_addr_array [i]      , ADDR_TRANS_SLV_BASE + 0xC) ;
            
            base_addr = base_addr_array [i];
            end_addr  = end_addr_array  [i];
            mid_addr  = ((base_addr >> 1) + (end_addr >> 1)) & (~0x7);
            //-------------------------------------------------------------
            //  Write data near bounds of region and in middle
            //  through PCIe, external PCIe and address translators.
            //-------------------------------------------------------------
            *((uint32_t*) base_addr) = base_addr ;
            *((uint32_t*) end_addr ) = end_addr  ;
            *((uint32_t*) mid_addr ) = mid_addr  ;
            
            *((uint32_t*) base_addr + 0x1) = 0x0 ;
            *((uint32_t*) end_addr  + 0x1) = 0x0 ;
            *((uint32_t*) mid_addr  + 0x1) = 0x0 ;
            //-------------------------------------------------------------
            //  Read data near bounds of region and in middle
            //  through PCIe, external PCIe and address translators.
            //  And check if it is correct.
            //-------------------------------------------------------------
            if ((*((uint32_t*) base_addr) != base_addr) ||
                (*((uint32_t*) end_addr ) != end_addr ) ||
                (*((uint32_t*) mid_addr ) != mid_addr ))
                return -1;
            //-------------------------------------------------------------
            //  Read data near bounds of region and in middle
            //  directly from DDR0.
            //  And check if it is correct.
            //-------------------------------------------------------------
            direct_base_addr = (base_addr_array [i] & (~0xFFF)) + (tran_addr_array [i] & (~0xFFF)) + (base_addr_array [i] & 0xFFF);
            direct_end_addr  = (end_addr_array  [i] & (~0xFFF)) + (tran_addr_array [i] & (~0xFFF)) + (end_addr_array  [i] & 0xFFF);
            direct_mid_addr  = ((direct_base_addr >> 1) + (direct_end_addr >> 1)) & (~0x7);
            if ((*((uint32_t*) direct_base_addr) != base_addr) ||
                (*((uint32_t*) direct_end_addr ) != end_addr ) ||
                (*((uint32_t*) direct_mid_addr ) != mid_addr ))
                    return -1;
            //-------------------------------------------------------------
            //  Clear data in DDR
            //-------------------------------------------------------------
            iowrite32(0, direct_base_addr) ;
            iowrite32(0, direct_end_addr ) ;
            iowrite32(0, direct_mid_addr ) ;

        }
        iowrite32(0, ADDR_TRANS_SLV_BASE + ((regions_for_test [j] * 3 + 1) << 2)) ;
        iowrite32(0, ADDR_TRANS_SLV_BASE + ((regions_for_test [j] * 3 + 2) << 2)) ;
        iowrite32(0, ADDR_TRANS_SLV_BASE + ((regions_for_test [j] * 3 + 3) << 2)) ;
    }

    return 0;
}

uint32_t main ()
{
    crg_ddr_init (0x63 ,0x0);
    if (ddr_init (DDR0_BASE) != 0)
        return -1;
    if (pcie_turn_on_with_options_ep (1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) != 0)
        return -2;
    
    if (at_slv_test_0 () != 0)
        return -3;
    
    
        
    return 0;
}
