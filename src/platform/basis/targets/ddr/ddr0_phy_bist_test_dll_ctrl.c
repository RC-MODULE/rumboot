//-----------------------------------------------------------------------------
//  This program is for checking DDR0 Phy address/control group DLLs
//  
//  Test includes:
//    - clear log space
//    - DDR0 initialisation with BIST parametes
//    - DDR0 BIST (part 9.13.2 in Phy documentation)
//    
//    Warning! Test is unstable - probably it can show wrong results.
//    It is better to run it few times.
//    Now best "conditions" are set:
//      - maximum error window
//      - maximum nuber of oscillations
// 
//    Watch 0x60000 address for test results.
//    
//    
//    Test duration (RTL): < 
//-----------------------------------------------------------------------------

#include <stdint.h>

#include <regs/regs_ddr.h>
#include <regs/regs_crg.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/ddr_test_lib.h>

#include <platform/devices.h>

#define    ERROR_LOG_HEAD    0x60000

void delay_cycle (volatile uint32_t delay)
{
    while (delay != 0)
        delay--;
}

void ddr_set_phy_update_mask (uint32_t DDRx_BASE)
{
    uint32_t rdata;
    
    rdata = ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_451);
    iowrite32 (rdata | 0x1, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_451);
}

void ddr_phy_dll_ac_test_init (uint32_t DDRx_BASE)
{
    //  Set:
    //     [10] mode_tap_0_size - normal
    //    [9:8] mode_obs_src_select - 0b01 to see "done" bit
    //    [7:3] mode_loop_size - maximum
    //    [2:0] mode_win_size - maximum
    //                                -++-----+++
    iowrite32 (0b00000000000000000000000111111111, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_447);  //  PHY_DDL_AC_MODE:RW:0:11:=0x0000
    iowrite32 (0xFF, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_446);  //  PHY_DDL_AC_ENABLE:RW:0:32:=0x00000000
}

void ddr_phy_dll_ac_test_wait_done (uint32_t DDRx_BASE)
{
    while ((ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_443) & 0x1) == 0);
}

uint32_t check_results (uint32_t DDRx_BASE)
{
    iowrite32 (ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_443), ERROR_LOG_HEAD + 0x10);
    if ((ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_443) & 0x00FFFFFF) != 0x1)
    {
        iowrite32 (0xDEAD0002, ERROR_LOG_HEAD + 0x08);
    }
    
    //  Set another observation mode
    //                                -++-----+++
    iowrite32 (0b00000000000000000000001111111111, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_447);  //  PHY_DDL_AC_MODE:RW:0:11:=0x0000
    iowrite32 (ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_443), ERROR_LOG_HEAD + 0x14);
    if ((ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_443) & 0x000100FF) != 0x1)
    {
        iowrite32 (0xDEAD0003, ERROR_LOG_HEAD + 0x0C);
    }
    
    return 0;
}

uint32_t ddr_bist (uint32_t DDRx_BASE)
{
    ddr_set_phy_update_mask         (DDRx_BASE);
    ddr_phy_dll_ac_test_init        (DDRx_BASE);
    ddr_phy_dll_ac_test_wait_done   (DDRx_BASE);
    check_results                   (DDRx_BASE);
    
    return 0;
}

void error_clr ()
{
    for (uint32_t i = 0; i < 512; i+=4)
        iowrite32 (0x00000000, ERROR_LOG_HEAD + i);
}

int main ()
{
    error_clr ();
    //  Clear "test pass" flag
    iowrite32 (0xDEADBEEF, ERROR_LOG_HEAD + 0x0);
    
    if (ddr_init (DDR0_BASE) != 0)
    {
        iowrite32 (0xDEAD0001, ERROR_LOG_HEAD + 0x4);
        return -1;
    }
    
    if (ddr_bist (DDR0_BASE) != 0)
        return -1;
    
    //  Set "test pass" flag
    iowrite32 (0x7A557A55, ERROR_LOG_HEAD + 0x0);
    return 0;
}

