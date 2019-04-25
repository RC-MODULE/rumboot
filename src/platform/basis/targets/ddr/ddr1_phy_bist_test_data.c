//-----------------------------------------------------------------------------
//  This program is for checking DDR1 Phy data slices
//    It uses internal Loopback mode.
//  
//  Test includes:
//    - clear log space
//    - DDR1 initialisation with BIST parametes
//    - DDR1 BIST (part 9.12.1 in Phy documentation)
//    
//    
//    Parameters (PHY_CLK_WRDQ1_SLAVE_DELAY_0, ...) values are unproven, but works
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
    // iowrite32 ( 0x00000000, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_46 );  //  PHY_RDDQ0_SLAVE_DELAY_0:RW:8:10:=0x0000 PHY_WRLVL_THRESHOLD_ADJUST_0:RW+:0:2:=0x00
    // iowrite32 ( 0x00000000, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_47 );  //  PHY_RDDQ2_SLAVE_DELAY_0:RW:16:10:=0x0000 PHY_RDDQ1_SLAVE_DELAY_0:RW:0:10:=0x0000
    // iowrite32 ( 0x00000000, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_48 );  //  PHY_RDDQ4_SLAVE_DELAY_0:RW:16:10:=0x0000 PHY_RDDQ3_SLAVE_DELAY_0:RW:0:10:=0x0000
    // iowrite32 ( 0x00000000, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_49 );  //  PHY_RDDQ6_SLAVE_DELAY_0:RW:16:10:=0x0000 PHY_RDDQ5_SLAVE_DELAY_0:RW:0:10:=0x0000
    // iowrite32 ( 0x00000000, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_50 );  //  PHY_RDDM_SLAVE_DELAY_0:RW:16:10:=0x0000 PHY_RDDQ7_SLAVE_DELAY_0:RW:0:10:=0x0000
    // iowrite32 ( 0x00800080, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_51 );  //  PHY_RDDQS_DQ0_FALL_SLAVE_DELAY_0:RW+:16:10:=0x0080 PHY_RDDQS_DQ0_RISE_SLAVE_DELAY_0:RW+:0:10:=0x0080
    // iowrite32 ( 0x00800080, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_52 );  //  PHY_RDDQS_DQ1_FALL_SLAVE_DELAY_0:RW+:16:10:=0x0080 PHY_RDDQS_DQ1_RISE_SLAVE_DELAY_0:RW+:0:10:=0x0080
    // iowrite32 ( 0x00800080, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_53 );  //  PHY_RDDQS_DQ2_FALL_SLAVE_DELAY_0:RW+:16:10:=0x0080 PHY_RDDQS_DQ2_RISE_SLAVE_DELAY_0:RW+:0:10:=0x0080
    // iowrite32 ( 0x00800080, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_54 );  //  PHY_RDDQS_DQ3_FALL_SLAVE_DELAY_0:RW+:16:10:=0x0080 PHY_RDDQS_DQ3_RISE_SLAVE_DELAY_0:RW+:0:10:=0x0080
    // iowrite32 ( 0x00800080, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_55 );  //  PHY_RDDQS_DQ4_FALL_SLAVE_DELAY_0:RW+:16:10:=0x0080 PHY_RDDQS_DQ4_RISE_SLAVE_DELAY_0:RW+:0:10:=0x0080
    // iowrite32 ( 0x00800080, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_56 );  //  PHY_RDDQS_DQ5_FALL_SLAVE_DELAY_0:RW+:16:10:=0x0080 PHY_RDDQS_DQ5_RISE_SLAVE_DELAY_0:RW+:0:10:=0x0080
    // iowrite32 ( 0x00800080, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_57 );  //  PHY_RDDQS_DQ6_FALL_SLAVE_DELAY_0:RW+:16:10:=0x0080 PHY_RDDQS_DQ6_RISE_SLAVE_DELAY_0:RW+:0:10:=0x0080
    // iowrite32 ( 0x00800080, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_58 );  //  PHY_RDDQS_DQ7_FALL_SLAVE_DELAY_0:RW+:16:10:=0x0080 PHY_RDDQS_DQ7_RISE_SLAVE_DELAY_0:RW+:0:10:=0x0080
    // iowrite32 ( 0x00800080, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_59 );  //  PHY_RDDQS_DM_FALL_SLAVE_DELAY_0:RW+:16:10:=0x0080 PHY_RDDQS_DM_RISE_SLAVE_DELAY_0:RW+:0:10:=0x0080


    iowrite32 ( 0x03000300, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_169);  //  PHY_CLK_WRDQ1_SLAVE_DELAY_1:RW+:16:11:=0x0280 PHY_CLK_WRDQ0_SLAVE_DELAY_1:RW+:0:11:=0x0280
    iowrite32 ( 0x03000300, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_170);  //  PHY_CLK_WRDQ3_SLAVE_DELAY_1:RW+:16:11:=0x0280 PHY_CLK_WRDQ2_SLAVE_DELAY_1:RW+:0:11:=0x0280
    iowrite32 ( 0x03000300, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_171);  //  PHY_CLK_WRDQ5_SLAVE_DELAY_1:RW+:16:11:=0x0280 PHY_CLK_WRDQ4_SLAVE_DELAY_1:RW+:0:11:=0x0280
    iowrite32 ( 0x03000300, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_172);  //  PHY_CLK_WRDQ7_SLAVE_DELAY_1:RW+:16:11:=0x0280 PHY_CLK_WRDQ6_SLAVE_DELAY_1:RW+:0:11:=0x0280
    iowrite32 ( 0x00000300, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_173);  //  PHY_CLK_WRDQS_SLAVE_DELAY_1:RW+:16:10:=0x0000 PHY_CLK_WRDM_SLAVE_DELAY_1:RW+:0:11:=0x0280
    // iowrite32 ( 0x00000000, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_174);  //  PHY_RDDQ0_SLAVE_DELAY_1:RW:8:10:=0x0000 PHY_WRLVL_THRESHOLD_ADJUST_1:RW+:0:2:=0x00
    // iowrite32 ( 0x00000000, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_175);  //  PHY_RDDQ2_SLAVE_DELAY_1:RW:16:10:=0x0000 PHY_RDDQ1_SLAVE_DELAY_1:RW:0:10:=0x0000
    // iowrite32 ( 0x00000000, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_176);  //  PHY_RDDQ4_SLAVE_DELAY_1:RW:16:10:=0x0000 PHY_RDDQ3_SLAVE_DELAY_1:RW:0:10:=0x0000
    // iowrite32 ( 0x00000000, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_177);  //  PHY_RDDQ6_SLAVE_DELAY_1:RW:16:10:=0x0000 PHY_RDDQ5_SLAVE_DELAY_1:RW:0:10:=0x0000
    // iowrite32 ( 0x00000000, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_178);  //  PHY_RDDM_SLAVE_DELAY_1:RW:16:10:=0x0000 PHY_RDDQ7_SLAVE_DELAY_1:RW:0:10:=0x0000
    // iowrite32 ( 0x00800080, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_179);  //  PHY_RDDQS_DQ0_FALL_SLAVE_DELAY_1:RW+:16:10:=0x0080 PHY_RDDQS_DQ0_RISE_SLAVE_DELAY_1:RW+:0:10:=0x0080
    // iowrite32 ( 0x00800080, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_180);  //  PHY_RDDQS_DQ1_FALL_SLAVE_DELAY_1:RW+:16:10:=0x0080 PHY_RDDQS_DQ1_RISE_SLAVE_DELAY_1:RW+:0:10:=0x0080
    // iowrite32 ( 0x00800080, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_181);  //  PHY_RDDQS_DQ2_FALL_SLAVE_DELAY_1:RW+:16:10:=0x0080 PHY_RDDQS_DQ2_RISE_SLAVE_DELAY_1:RW+:0:10:=0x0080
    // iowrite32 ( 0x00800080, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_182);  //  PHY_RDDQS_DQ3_FALL_SLAVE_DELAY_1:RW+:16:10:=0x0080 PHY_RDDQS_DQ3_RISE_SLAVE_DELAY_1:RW+:0:10:=0x0080
    // iowrite32 ( 0x00800080, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_183);  //  PHY_RDDQS_DQ4_FALL_SLAVE_DELAY_1:RW+:16:10:=0x0080 PHY_RDDQS_DQ4_RISE_SLAVE_DELAY_1:RW+:0:10:=0x0080
    // iowrite32 ( 0x00800080, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_184);  //  PHY_RDDQS_DQ5_FALL_SLAVE_DELAY_1:RW+:16:10:=0x0080 PHY_RDDQS_DQ5_RISE_SLAVE_DELAY_1:RW+:0:10:=0x0080
    // iowrite32 ( 0x00800080, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_185);  //  PHY_RDDQS_DQ6_FALL_SLAVE_DELAY_1:RW+:16:10:=0x0080 PHY_RDDQS_DQ6_RISE_SLAVE_DELAY_1:RW+:0:10:=0x0080
    // iowrite32 ( 0x00800080, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_186);  //  PHY_RDDQS_DQ7_FALL_SLAVE_DELAY_1:RW+:16:10:=0x0080 PHY_RDDQS_DQ7_RISE_SLAVE_DELAY_1:RW+:0:10:=0x0080
    // iowrite32 ( 0x00800080, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_187);  //  PHY_RDDQS_DM_FALL_SLAVE_DELAY_1:RW+:16:10:=0x0080 PHY_RDDQS_DM_RISE_SLAVE_DELAY_1:RW+:0:10:=0x0080


    iowrite32 ( 0x03000300, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_297);  //  PHY_CLK_WRDQ1_SLAVE_DELAY_2:RW+:16:11:=0x0280 PHY_CLK_WRDQ0_SLAVE_DELAY_2:RW+:0:11:=0x0280
    iowrite32 ( 0x03000300, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_298);  //  PHY_CLK_WRDQ3_SLAVE_DELAY_2:RW+:16:11:=0x0280 PHY_CLK_WRDQ2_SLAVE_DELAY_2:RW+:0:11:=0x0280
    iowrite32 ( 0x03000300, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_299);  //  PHY_CLK_WRDQ5_SLAVE_DELAY_2:RW+:16:11:=0x0280 PHY_CLK_WRDQ4_SLAVE_DELAY_2:RW+:0:11:=0x0280
    iowrite32 ( 0x03000300, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_300);  //  PHY_CLK_WRDQ7_SLAVE_DELAY_2:RW+:16:11:=0x0280 PHY_CLK_WRDQ6_SLAVE_DELAY_2:RW+:0:11:=0x0280
    iowrite32 ( 0x00000300, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_301);  //  PHY_CLK_WRDQS_SLAVE_DELAY_2:RW+:16:10:=0x0000 PHY_CLK_WRDM_SLAVE_DELAY_2:RW+:0:11:=0x0280
    // iowrite32 ( 0x00000000, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_302);  //  PHY_RDDQ0_SLAVE_DELAY_2:RW:8:10:=0x0000 PHY_WRLVL_THRESHOLD_ADJUST_2:RW+:0:2:=0x00
    // iowrite32 ( 0x00000000, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_303);  //  PHY_RDDQ2_SLAVE_DELAY_2:RW:16:10:=0x0000 PHY_RDDQ1_SLAVE_DELAY_2:RW:0:10:=0x0000
    // iowrite32 ( 0x00000000, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_304);  //  PHY_RDDQ4_SLAVE_DELAY_2:RW:16:10:=0x0000 PHY_RDDQ3_SLAVE_DELAY_2:RW:0:10:=0x0000
    // iowrite32 ( 0x00000000, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_305);  //  PHY_RDDQ6_SLAVE_DELAY_2:RW:16:10:=0x0000 PHY_RDDQ5_SLAVE_DELAY_2:RW:0:10:=0x0000
    // iowrite32 ( 0x00000000, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_306);  //  PHY_RDDM_SLAVE_DELAY_2:RW:16:10:=0x0000 PHY_RDDQ7_SLAVE_DELAY_2:RW:0:10:=0x0000
    // iowrite32 ( 0x00800080, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_307);  //  PHY_RDDQS_DQ0_FALL_SLAVE_DELAY_2:RW+:16:10:=0x0080 PHY_RDDQS_DQ0_RISE_SLAVE_DELAY_2:RW+:0:10:=0x0080
    // iowrite32 ( 0x00800080, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_308);  //  PHY_RDDQS_DQ1_FALL_SLAVE_DELAY_2:RW+:16:10:=0x0080 PHY_RDDQS_DQ1_RISE_SLAVE_DELAY_2:RW+:0:10:=0x0080
    // iowrite32 ( 0x00800080, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_309);  //  PHY_RDDQS_DQ2_FALL_SLAVE_DELAY_2:RW+:16:10:=0x0080 PHY_RDDQS_DQ2_RISE_SLAVE_DELAY_2:RW+:0:10:=0x0080
    // iowrite32 ( 0x00800080, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_310);  //  PHY_RDDQS_DQ3_FALL_SLAVE_DELAY_2:RW+:16:10:=0x0080 PHY_RDDQS_DQ3_RISE_SLAVE_DELAY_2:RW+:0:10:=0x0080
    // iowrite32 ( 0x00800080, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_311);  //  PHY_RDDQS_DQ4_FALL_SLAVE_DELAY_2:RW+:16:10:=0x0080 PHY_RDDQS_DQ4_RISE_SLAVE_DELAY_2:RW+:0:10:=0x0080
    // iowrite32 ( 0x00800080, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_312);  //  PHY_RDDQS_DQ5_FALL_SLAVE_DELAY_2:RW+:16:10:=0x0080 PHY_RDDQS_DQ5_RISE_SLAVE_DELAY_2:RW+:0:10:=0x0080
    // iowrite32 ( 0x00800080, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_313);  //  PHY_RDDQS_DQ6_FALL_SLAVE_DELAY_2:RW+:16:10:=0x0080 PHY_RDDQS_DQ6_RISE_SLAVE_DELAY_2:RW+:0:10:=0x0080
    // iowrite32 ( 0x00800080, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_314);  //  PHY_RDDQS_DQ7_FALL_SLAVE_DELAY_2:RW+:16:10:=0x0080 PHY_RDDQS_DQ7_RISE_SLAVE_DELAY_2:RW+:0:10:=0x0080
    // iowrite32 ( 0x00800080, DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_315);  //  PHY_RDDQS_DM_FALL_SLAVE_DELAY_2:RW+:16:10:=0x0080 PHY_RDDQS_DM_RISE_SLAVE_DELAY_2:RW+:0:10:=0x0080
}

uint32_t check_results (uint32_t DDRx_BASE)
{
    iowrite32 (ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_18 ), ERROR_LOG_HEAD + 0x10);
    iowrite32 (ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_146), ERROR_LOG_HEAD + 0x14);
    iowrite32 (ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_274), ERROR_LOG_HEAD + 0x18);
    if ((ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_18 ) != (0x1 << 21)) |
        (ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_146) != (0x1 << 21)) |
        (ioread32 (DDRx_BASE + DENALI_PHY_BASE + DENALI_PHY_274) != (0x1 << 21)))
        {
            iowrite32 (0xDEAD0002, ERROR_LOG_HEAD + 0x08);
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
    
    if (ddr_init (DDR1_BASE) != 0)
    {
        iowrite32 (0xDEAD0001, ERROR_LOG_HEAD + 0x4);
        return -1;
    }
    
    if (ddr_bist (DDR1_BASE) != 0)
        return -1;
    
    //  Set "test pass" flag
    iowrite32 (0x7A557A55, ERROR_LOG_HEAD + 0x0);
    return 0;
}

