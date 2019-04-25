//-----------------------------------------------------------------------------
//  This program is for checking DDR0 Phy control slice
//    It uses internal Loopback mode.
//  
//  Test includes:
//    - clear log space
//    - DDR0 initialisation with BIST parametes
//    - DDR0 BIST (part 9.12.2 in Phy documentation)
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

void ddr_phy_adrctl_manual_update (uint32_t DDRx_BASE)
{
    uint32_t rdata;
    
    rdata = ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_440);
    iowrite32 (rdata | (0x1 << 16), DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_440);  //  PHY_AC_LPBK_ERR_CLEAR:WR:24:1:=0x00 PHY_ADRCTL_MANUAL_UPDATE:WR:16:1:=0x00 PHY_PAD_ATB_CTRL:RW:0:16:=0x0000
}

void ddr_phy_ac_lpbk_en (uint32_t DDRx_BASE)
{
    uint32_t rdata;
    
    //  Set:
    //    [8:6] observation by default
    //      [5] dont go yet
    //    [4:3] 1024 pulses
    //    [2:1] PRBS7 pattern
    //      [0] Internal
    //  
    rdata = ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_441 );
    iowrite32 (rdata | ( 0x00C << 16), DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_441 );  //  PHY_AC_LPBK_CONTROL:RW:16:9:=0x0000 PHY_AC_LPBK_ENABLE:RW:8:7:=0x00 PHY_AC_LPBK_OBS_SELECT:RW:0:3:=0x00
    //  Enable loopback. Separated point in documantation, so separated here
    rdata = ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_441 );
    iowrite32 (rdata | ( 0x7F << 8), DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_441 );  //  PHY_AC_LPBK_CONTROL:RW:16:9:=0x0000 PHY_AC_LPBK_ENABLE:RW:8:7:=0x00 PHY_AC_LPBK_OBS_SELECT:RW:0:3:=0x00
}

void ddr_phy_ac_lpbk_go (uint32_t DDRx_BASE)
{
    uint32_t rdata;
    
    rdata = ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_441 );
    iowrite32 (rdata | (0x020 << 16), DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_441 );  //  PHY_AC_LPBK_CONTROL:RW:16:9:=0x0000 PHY_AC_LPBK_ENABLE:RW:8:7:=0x00 PHY_AC_LPBK_OBS_SELECT:RW:0:3:=0x00
}

void ddr_phy_ac_lpbk_wait_done (uint32_t DDRx_BASE)
{
    while ((ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_443) & (0x1 << 5)) == 0);
}

void ddr_set_group_slave_delays (uint32_t DDRx_BASE)
{
}

uint32_t check_results (uint32_t DDRx_BASE)
{
    iowrite32 (ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_443), ERROR_LOG_HEAD + 0x10);
    if (ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_443) != (0x1 << 5))
        {
            iowrite32 (0xDEAD0002, ERROR_LOG_HEAD + 0x08);
            return -1;
        }
    return 0;
}

uint32_t ddr_bist (uint32_t DDRx_BASE)
{
    ddr_set_phy_update_mask         (DDRx_BASE);
    ddr_set_group_slave_delays      (DDRx_BASE);
    delay_cycle (50);
    ddr_phy_adrctl_manual_update    (DDRx_BASE);
    ddr_phy_ac_lpbk_en              (DDRx_BASE);
    delay_cycle (10);
    ddr_phy_ac_lpbk_go              (DDRx_BASE);
    ddr_phy_ac_lpbk_wait_done       (DDRx_BASE);
    
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

