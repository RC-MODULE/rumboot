//-----------------------------------------------------------------------------
//  This program is for checking DDR0 Phy data groups DLLs
//  
//  Test includes:
//    - clear log space
//    - DDR0 initialisation with BIST parametes
//    - DDR0 BIST (part 9.13.1 in Phy documentation)
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

void ddr_phy_dll_test_init (uint32_t DDRx_BASE, uint32_t mode_group)
{
    //  Set:
    //     [18] mode_obs_src_select
    //     [17] mode_tap_0_size
    //  [16:13] mode_sub_group
    //   [12:9] mode_group - select all groups
    //    [8:4] 31*16 loop oscillations               unproven
    //    [3:1] 10 count error                        unproven
    //      [0] Execute the test
    //                        +-++++----+++++---+
    iowrite32 (0b00000000000000000000000111111111 | (mode_group << 9), DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_30 );  //  PHY_DDL_MODE_0:RW:0:18:=0x000000
    iowrite32 (0b00000000000000000000000111111111 | (mode_group << 9), DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_158);  //  PHY_DDL_MODE_1:RW:0:18:=0x000000
    iowrite32 (0b00000000000000000000000111111111 | (mode_group << 9), DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_286);  //  PHY_DDL_MODE_2:RW:0:18:=0x000000
}

void ddr_phy_dll_test_wait_done (uint32_t DDRx_BASE)
{
    while (
            (
            ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_31 ) &
            ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_159) &
            ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_287) &
            0x1) == 0);
}

uint32_t check_results (uint32_t DDRx_BASE, uint32_t mode_group)
{
    iowrite32 (ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_31 ), ERROR_LOG_HEAD + (mode_group << 4) + 0x0);
    iowrite32 (ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_159), ERROR_LOG_HEAD + (mode_group << 4) + 0x4);
    iowrite32 (ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_287), ERROR_LOG_HEAD + (mode_group << 4) + 0x8);
    if (((ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_31 ) & 0x00FFFFFF) != 0x1) |
        ((ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_159) & 0x00FFFFFF) != 0x1) |
        ((ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_287) & 0x00FFFFFF) != 0x1))
        {
            iowrite32 (0xDEAD0002, ERROR_LOG_HEAD + 0x08);
            return -1;
        }
    return 0;
}

void ddr_phy_dll_restart (uint32_t DDRx_BASE)
{
    uint32_t rdata;
    
    rdata = ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_07 );
    iowrite32 (rdata | (1 << 28), DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_07 );  //  PHY_LPBK_CONTROL_0:RW:24:8:=0x00 PHY_DQS_TSEL_SELECT_0:RW:0:24:=0x665555
    rdata = ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_135);
    iowrite32 (rdata | (1 << 28), DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_135);  //  PHY_LPBK_CONTROL_1:RW:24:8:=0x00 PHY_DQS_TSEL_SELECT_1:RW:0:24:=0x665555
    rdata = ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_263);
    iowrite32 (rdata | (1 << 28), DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_263);  //  PHY_LPBK_CONTROL_2:RW:24:8:=0x00 PHY_DQS_TSEL_SELECT_2:RW:0:24:=0x665555
    
    rdata = ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_07 );
    iowrite32 (rdata & (~(1 << 28)), DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_07 );  //  PHY_LPBK_CONTROL_0:RW:24:8:=0x00 PHY_DQS_TSEL_SELECT_0:RW:0:24:=0x665555
    rdata = ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_135);
    iowrite32 (rdata & (~(1 << 28)), DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_135);  //  PHY_LPBK_CONTROL_1:RW:24:8:=0x00 PHY_DQS_TSEL_SELECT_1:RW:0:24:=0x665555
    rdata = ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_263);
    iowrite32 (rdata & (~(1 << 28)), DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_263);  //  PHY_LPBK_CONTROL_2:RW:24:8:=0x00 PHY_DQS_TSEL_SELECT_2:RW:0:24:=0x665555

    iowrite32 (0, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_30 );  //  PHY_DDL_MODE_0:RW:0:18:=0x000000
    iowrite32 (0, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_158);  //  PHY_DDL_MODE_1:RW:0:18:=0x000000
    iowrite32 (0, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_286);  //  PHY_DDL_MODE_2:RW:0:18:=0x000000
}

uint32_t ddr_bist (uint32_t DDRx_BASE)
{
    ddr_set_phy_update_mask         (DDRx_BASE);
    for (uint32_t mode_group = 1; mode_group <= 0xA; mode_group++)
    {
        ddr_phy_dll_test_init           (DDRx_BASE, mode_group);
        ddr_phy_dll_test_wait_done      (DDRx_BASE);
        check_results                   (DDRx_BASE, mode_group);
        ddr_phy_dll_restart             (DDRx_BASE);
    }
    
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

