//-----------------------------------------------------------------------------
//  This program is for checking DDR0 Phy clock slice
//    It uses internal Loopback mode.
//  
//  Test includes:
//    - clear log space
//    - DDR0 initialisation with BIST parametes
//    - DDR0 BIST (part 9.12.3 in Phy documentation)
//    
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

void ddr_phy_ac_clk_lpbk_en (uint32_t DDRx_BASE)
{
    uint32_t rdata;
    
    //  Set:
    //    [5:4] observation by default
    //      [3] dont go yet
    //    [2:1] 1024 pulses
    //      [0] Internal
    //  
    rdata = ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_444 );
    iowrite32 (rdata | ( 0x02 << 16), DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_444 );  //  PHY_AC_CLK_LPBK_CONTROL:RW:16:6:=0x00 PHY_AC_CLK_LPBK_ENABLE:RW:8:1:=0x00 PHY_AC_CLK_LPBK_OBS_SELECT:RW:0:1:=0x00
    //  Enable loopback. Separated point in documantation, so separated here
    rdata = ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_444 );
    iowrite32 (rdata | ( 0x1 << 8), DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_444 );  //  PHY_AC_CLK_LPBK_CONTROL:RW:16:6:=0x00 PHY_AC_CLK_LPBK_ENABLE:RW:8:1:=0x00 PHY_AC_CLK_LPBK_OBS_SELECT:RW:0:1:=0x00
}

void ddr_phy_ac_clk_lpbk_go (uint32_t DDRx_BASE)
{
    uint32_t rdata;
    
    rdata = ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_444 );
    iowrite32 (rdata | ( 0x08 << 16), DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_444 );  //  PHY_AC_CLK_LPBK_CONTROL:RW:16:6:=0x00 PHY_AC_CLK_LPBK_ENABLE:RW:8:1:=0x00 PHY_AC_CLK_LPBK_OBS_SELECT:RW:0:1:=0x00
}

void ddr_phy_ac_clk_lpbk_wait_done (uint32_t DDRx_BASE)
{
    while ((ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_445) & (0x1 << 1)) == 0);
}

uint32_t check_results (uint32_t DDRx_BASE)
{
    iowrite32 (ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_445), ERROR_LOG_HEAD + 0x10);
    if (ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_445) != (0x1 << 1))
        {
            iowrite32 (0xDEAD0002, ERROR_LOG_HEAD + 0x08);
            return -1;
        }
    return 0;
}

uint32_t ddr_bist (uint32_t DDRx_BASE)
{
    ddr_set_phy_update_mask         (DDRx_BASE);
    ddr_phy_ac_clk_lpbk_en          (DDRx_BASE);
    delay_cycle (10);
    ddr_phy_ac_clk_lpbk_go          (DDRx_BASE);
    ddr_phy_ac_clk_lpbk_wait_done   (DDRx_BASE);
    
    return check_results (DDRx_BASE);
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

