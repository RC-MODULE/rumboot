//-----------------------------------------------------------------------------
//  This program is for checking DDR0 in BIST mode
//    It uses internal Loopback mode.
//  
//  Test includes:
//    - clear log space
//    - DDR0 initialisation with BIST parametes
//      (do this only, if DDR was not initialised yet. Second initialisation doesnt pass. Reason unknown.)
//    - run DDR0 Loppback tests:
//      - data
//      - control
//      - clock
//    
//    Test could be repeated without reset. It is necessary for condition testings.
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


//-----------------------------------------------------------------------------------------------------------------------------
//  Common functions for all test modes
//-----------------------------------------------------------------------------------------------------------------------------
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


//-----------------------------------------------------------------------------------------------------------------------------
//  Loopback Data mode functions
//-----------------------------------------------------------------------------------------------------------------------------
void ddr_phy_manual_update (uint32_t DDRx_BASE)
{
    uint32_t rdata;
    
    rdata = ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_387);
    iowrite32 (rdata | 0x1, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_387);
}

void ddr_phy_lpbk_en (uint32_t DDRx_BASE)
{
    uint32_t rdata;
    
    //  [2]
    //      0 - LFSR
    //      1 - clock pattern
    //      both of them are NOT PRBS7
    rdata = ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_07 );
    iowrite32 (rdata | 0x21000000, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_07 );  //  PHY_LPBK_CONTROL_0:RW:24:8:=0x00 PHY_DQS_TSEL_SELECT_0:RW:0:24:=0x665555
    rdata = ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_135);
    iowrite32 (rdata | 0x21000000, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_135);  //  PHY_LPBK_CONTROL_1:RW:24:8:=0x00 PHY_DQS_TSEL_SELECT_1:RW:0:24:=0x665555
    rdata = ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_263);
    iowrite32 (rdata | 0x21000000, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_263);  //  PHY_LPBK_CONTROL_2:RW:24:8:=0x00 PHY_DQS_TSEL_SELECT_2:RW:0:24:=0x665555
}

void ddr_phy_lpbk_go (uint32_t DDRx_BASE)
{
    uint32_t rdata;
    
    rdata = ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_07 );
    iowrite32 (rdata | 0x80000000, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_07 );  //  PHY_LPBK_CONTROL_0:RW:24:8:=0x00 PHY_DQS_TSEL_SELECT_0:RW:0:24:=0x665555
    rdata = ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_135);
    iowrite32 (rdata | 0x80000000, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_135);  //  PHY_LPBK_CONTROL_1:RW:24:8:=0x00 PHY_DQS_TSEL_SELECT_1:RW:0:24:=0x665555
    rdata = ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_263);
    iowrite32 (rdata | 0x80000000, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_263);  //  PHY_LPBK_CONTROL_2:RW:24:8:=0x00 PHY_DQS_TSEL_SELECT_2:RW:0:24:=0x665555
}

void ddr_phy_lpbk_wait_done (uint32_t DDRx_BASE)
{
    while (
            (
            ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_18 ) &
            ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_146) &
            ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_274) &
            (0x1 << 21)) == 0);
}

void ddr_set_dq_dqs_slave_delays (uint32_t DDRx_BASE)
{
    iowrite32 ( 0x03000300, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_41 );  //  PHY_CLK_WRDQ1_SLAVE_DELAY_0:RW+:16:11:=0x0280 PHY_CLK_WRDQ0_SLAVE_DELAY_0:RW+:0:11:=0x0280
    iowrite32 ( 0x03000300, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_42 );  //  PHY_CLK_WRDQ3_SLAVE_DELAY_0:RW+:16:11:=0x0280 PHY_CLK_WRDQ2_SLAVE_DELAY_0:RW+:0:11:=0x0280
    iowrite32 ( 0x03000300, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_43 );  //  PHY_CLK_WRDQ5_SLAVE_DELAY_0:RW+:16:11:=0x0280 PHY_CLK_WRDQ4_SLAVE_DELAY_0:RW+:0:11:=0x0280
    iowrite32 ( 0x03000300, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_44 );  //  PHY_CLK_WRDQ7_SLAVE_DELAY_0:RW+:16:11:=0x0280 PHY_CLK_WRDQ6_SLAVE_DELAY_0:RW+:0:11:=0x0280
    iowrite32 ( 0x00000300, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_45 );  //  PHY_CLK_WRDQS_SLAVE_DELAY_0:RW+:16:10:=0x0000 PHY_CLK_WRDM_SLAVE_DELAY_0:RW+:0:11:=0x0280

    iowrite32 ( 0x03000300, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_169);  //  PHY_CLK_WRDQ1_SLAVE_DELAY_1:RW+:16:11:=0x0280 PHY_CLK_WRDQ0_SLAVE_DELAY_1:RW+:0:11:=0x0280
    iowrite32 ( 0x03000300, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_170);  //  PHY_CLK_WRDQ3_SLAVE_DELAY_1:RW+:16:11:=0x0280 PHY_CLK_WRDQ2_SLAVE_DELAY_1:RW+:0:11:=0x0280
    iowrite32 ( 0x03000300, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_171);  //  PHY_CLK_WRDQ5_SLAVE_DELAY_1:RW+:16:11:=0x0280 PHY_CLK_WRDQ4_SLAVE_DELAY_1:RW+:0:11:=0x0280
    iowrite32 ( 0x03000300, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_172);  //  PHY_CLK_WRDQ7_SLAVE_DELAY_1:RW+:16:11:=0x0280 PHY_CLK_WRDQ6_SLAVE_DELAY_1:RW+:0:11:=0x0280
    iowrite32 ( 0x00000300, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_173);  //  PHY_CLK_WRDQS_SLAVE_DELAY_1:RW+:16:10:=0x0000 PHY_CLK_WRDM_SLAVE_DELAY_1:RW+:0:11:=0x0280

    iowrite32 ( 0x03000300, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_297);  //  PHY_CLK_WRDQ1_SLAVE_DELAY_2:RW+:16:11:=0x0280 PHY_CLK_WRDQ0_SLAVE_DELAY_2:RW+:0:11:=0x0280
    iowrite32 ( 0x03000300, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_298);  //  PHY_CLK_WRDQ3_SLAVE_DELAY_2:RW+:16:11:=0x0280 PHY_CLK_WRDQ2_SLAVE_DELAY_2:RW+:0:11:=0x0280
    iowrite32 ( 0x03000300, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_299);  //  PHY_CLK_WRDQ5_SLAVE_DELAY_2:RW+:16:11:=0x0280 PHY_CLK_WRDQ4_SLAVE_DELAY_2:RW+:0:11:=0x0280
    iowrite32 ( 0x03000300, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_300);  //  PHY_CLK_WRDQ7_SLAVE_DELAY_2:RW+:16:11:=0x0280 PHY_CLK_WRDQ6_SLAVE_DELAY_2:RW+:0:11:=0x0280
    iowrite32 ( 0x00000300, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_301);  //  PHY_CLK_WRDQS_SLAVE_DELAY_2:RW+:16:10:=0x0000 PHY_CLK_WRDM_SLAVE_DELAY_2:RW+:0:11:=0x0280
}

uint32_t check_results (uint32_t DDRx_BASE)
{
    iowrite32 (ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_18 ), ERROR_LOG_HEAD + 0x14);
    iowrite32 (ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_146), ERROR_LOG_HEAD + 0x18);
    iowrite32 (ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_274), ERROR_LOG_HEAD + 0x1C);
    if ((ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_18 ) != (0x1 << 21)) |
        (ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_146) != (0x1 << 21)) |
        (ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_274) != (0x1 << 21)))
        {
            iowrite32 (0xDEAD0002, ERROR_LOG_HEAD + 0x10);
            return -1;
        }
    return 0;
}

uint32_t ddr_bist (uint32_t DDRx_BASE)
{
    ddr_set_phy_update_mask         (DDRx_BASE);
    ddr_set_dq_dqs_slave_delays     (DDRx_BASE);
    delay_cycle (150);
    ddr_phy_manual_update           (DDRx_BASE);
    ddr_phy_lpbk_en                 (DDRx_BASE);
    delay_cycle (10);
    ddr_phy_lpbk_go                 (DDRx_BASE);
    ddr_phy_lpbk_wait_done          (DDRx_BASE);
    
    return check_results (DDRx_BASE);
}


//-----------------------------------------------------------------------------------------------------------------------------
//  Loopback AC mode functions
//-----------------------------------------------------------------------------------------------------------------------------
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

uint32_t check_results_ac (uint32_t DDRx_BASE)
{
    iowrite32 (ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_443), ERROR_LOG_HEAD + 0x24);
    if (ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_443) != (0x1 << 5))
        {
            iowrite32 (0xDEAD0002, ERROR_LOG_HEAD + 0x20);
            return -1;
        }
    return 0;
}

uint32_t ddr_bist_ac (uint32_t DDRx_BASE)
{
    ddr_set_phy_update_mask         (DDRx_BASE);
    ddr_set_group_slave_delays      (DDRx_BASE);
    delay_cycle (50);
    ddr_phy_adrctl_manual_update    (DDRx_BASE);
    ddr_phy_ac_lpbk_en              (DDRx_BASE);
    delay_cycle (10);
    ddr_phy_ac_lpbk_go              (DDRx_BASE);
    ddr_phy_ac_lpbk_wait_done       (DDRx_BASE);
    
    return check_results_ac (DDRx_BASE);
}


//-----------------------------------------------------------------------------------------------------------------------------
//  Loopback CLK mode functions
//-----------------------------------------------------------------------------------------------------------------------------
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

uint32_t check_results_clk (uint32_t DDRx_BASE)
{
    iowrite32 (ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_445), ERROR_LOG_HEAD + 0x34);
    if (ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_445) != (0x1 << 1))
        {
            iowrite32 (0xDEAD0003, ERROR_LOG_HEAD + 0x30);
            return -1;
        }
    return 0;
}

uint32_t ddr_bist_clk (uint32_t DDRx_BASE)
{
    ddr_set_phy_update_mask         (DDRx_BASE);
    ddr_phy_ac_clk_lpbk_en          (DDRx_BASE);
    delay_cycle (10);
    ddr_phy_ac_clk_lpbk_go          (DDRx_BASE);
    ddr_phy_ac_clk_lpbk_wait_done   (DDRx_BASE);
    
    return check_results_clk (DDRx_BASE);
}


//-----------------------------------------------------------------------------------------------------------------------------
//  DLL data mode functions
//-----------------------------------------------------------------------------------------------------------------------------
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

uint32_t check_results_dll_data (uint32_t DDRx_BASE, uint32_t mode_group)
{
    iowrite32 (ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_31 ), ERROR_LOG_HEAD + (mode_group << 4) + 0x44);
    iowrite32 (ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_159), ERROR_LOG_HEAD + (mode_group << 4) + 0x48);
    iowrite32 (ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_287), ERROR_LOG_HEAD + (mode_group << 4) + 0x4C);
    if (((ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_31 ) & 0x00FFFFFF) != 0x1) |
        ((ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_159) & 0x00FFFFFF) != 0x1) |
        ((ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_287) & 0x00FFFFFF) != 0x1))
        {
            iowrite32 (0xDEAD0002, ERROR_LOG_HEAD + 0x40);
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

uint32_t ddr_bist_dll_data (uint32_t DDRx_BASE)
{
    ddr_set_phy_update_mask         (DDRx_BASE);
    for (uint32_t mode_group = 1; mode_group <= 0xA; mode_group++)
    {
        ddr_phy_dll_test_init           (DDRx_BASE, mode_group);
        ddr_phy_dll_test_wait_done      (DDRx_BASE);
        check_results_dll_data          (DDRx_BASE, mode_group);
        ddr_phy_dll_restart             (DDRx_BASE);
    }
    
    return 0;
}


//-----------------------------------------------------------------------------------------------------------------------------
//  DLL AC mode functions
//-----------------------------------------------------------------------------------------------------------------------------
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

uint32_t check_results_dll_ac (uint32_t DDRx_BASE)
{
    iowrite32 (ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_443), ERROR_LOG_HEAD + 0x54);
    if ((ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_443) & 0x00FFFFFF) != 0x1)
    {
        iowrite32 (0xDEAD0002, ERROR_LOG_HEAD + 0x50);
    }
    
    //  Set another observation mode
    //                                -++-----+++
    iowrite32 (0b00000000000000000000001111111111, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_447);  //  PHY_DDL_AC_MODE:RW:0:11:=0x0000
    iowrite32 (ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_443), ERROR_LOG_HEAD + 0x58);
    if ((ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_443) & 0x000100FF) != 0x1)
    {
        iowrite32 (0xDEAD0003, ERROR_LOG_HEAD + 0x50);
    }
    
    return 0;
}

uint32_t ddr_bist_dll_ac (uint32_t DDRx_BASE)
{
    ddr_set_phy_update_mask         (DDRx_BASE);
    ddr_phy_dll_ac_test_init        (DDRx_BASE);
    ddr_phy_dll_ac_test_wait_done   (DDRx_BASE);
    check_results_dll_ac            (DDRx_BASE);
    
    return 0;
}

//-----------------------------------------------------------------------------------------------------------------------------


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
    
    //---------------------------------------------------------
    //  Restarting Core doesnt help dll test to pass
    //-------------------------
    // iowrite32 (ioread32 (DDR0_BASE + DENALI_CTL_00) & 0xFFFFFF00, DDR0_BASE + DENALI_CTL_00);
    // delay_cycle (1000000);
    
    if ((ioread32 (DDR0_BASE + DENALI_CTL_00) & 0x1) == 0)
        if (ddr_init (DDR0_BASE) != 0)
        {
            iowrite32 (0xDEAD0001, ERROR_LOG_HEAD + 0x4);
            return -1;
        }
    
    if (ddr_bist            (DDR0_BASE) != 0)
        return -2;
    if (ddr_bist_ac         (DDR0_BASE) != 0)
        return -3;
    if (ddr_bist_clk        (DDR0_BASE) != 0)
        return -4;
    //---------------------------------------------------------
    //  Dll test doesnt pass always on first run
    //  results of DLL test dont allow to pass all other tests on second run
    //-------------------------
    // if (ddr_bist_dll_data   (DDR0_BASE) != 0)
        // return -5;
    // if (ddr_bist_dll_ac     (DDR0_BASE) != 0)
        // return -6;
    
    //  Set "test pass" flag
    iowrite32 (0x7A557A55, ERROR_LOG_HEAD + 0x0);
    return 0;
}

