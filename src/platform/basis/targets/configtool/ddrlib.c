
//-----------------------------------------------------------------------------
//  This file contain DDR related functions, used for testing.
//  It includes next functions:
//    - standard Phy and controller initialisation
//    - power mode changing
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  Initialisation corresponds to next setings:
//-----------------------------------------------------------------------------
#include <stdint.h>

#include <regs/regs_ddr.h>
#include <regs/regs_crg.h>
#include <stdlib.h>
#include <unistd.h>
#include <platform/devices.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/memtest.h>


#ifndef CMAKE_BUILD_TYPE_POSTPRODUCTION
#define DDR_TEST_LIB_PLL_LOCK_TIMEOUT        0x1000
#define CRG_DDR_TEST_LIB_PLL_LOCK_TIMEOUT    0x1000
#else
#define DDR_TEST_LIB_PLL_LOCK_TIMEOUT        0x1000000
#define CRG_DDR_TEST_LIB_PLL_LOCK_TIMEOUT    0x1000000
#endif


#define DDR_HEADER_FILE "platform/ddr_config/ddr__mt41k256m8_125_8_11_800.h"
#define DDR_INIT_NAME mt41k256m8
#include "ddr.template"
#include "undef.h"

#define DDR_HEADER_FILE "platform/ddr_config/ddr__mt41j512m8_187e_8_8_533.h"
#define DDR_INIT_NAME mt41j512m8
#include "ddr.template"
#include "undef.h"


//-----------------------------------------------------------------------------
//  This function is for changing DDR external CRG frequency
//
//  Arguments:
//
//    - pll_fbdiv    frequency multiplier coefficient
//    - pll_psdiv    frequency post divider coefficient
//
//    FREF = 16MHz
//
//                      DDR3-800      DDR3-1066     DDR3-1333      DDR3-1600
//        pll_fbdiv     0x63 (*99)    0x84 (*132)   0x52 (*82)     0x63 (*99)
//        pll_psdiv     0x1  (/4)     0x1  (/4)     0x0  (/2)      0x0  (/2)
//
//    FOUT =            396MHz        528MHz        656MHz         792MHz
//
//    So, frequency is a bit less, than maximum for corresponding settings.
//    It can be helpfull for simulation and save from freq problems later.
//-----------------------------------------------------------------------------
static uint32_t crg_ddr_init
(
    uint32_t pll_fbdiv ,
    uint32_t pll_psdiv
)
{
    uint32_t timer_cntr = 0;
    
    if (
        (ioread32 (CRG_DDR_BASE + CRG_PLL_FBDIV) != pll_fbdiv) ||
        (ioread32 (CRG_DDR_BASE + CRG_PLL_PSDIV) != pll_psdiv)
        )
    {
        iowrite32 (0x1ACCE551, CRG_DDR_BASE + CRG_WR_LOCK);
        iowrite32 (0x3, CRG_DDR_BASE + CRG_PLL_CTRL);
        iowrite32 (pll_fbdiv, CRG_DDR_BASE + CRG_PLL_FBDIV);
        iowrite32 (pll_psdiv, CRG_DDR_BASE + CRG_PLL_PSDIV);

        while ((ioread32 (CRG_DDR_BASE + CRG_PLL_STAT) & 0x00000010) == 0)
        {
            timer_cntr++;
            if (timer_cntr == CRG_DDR_TEST_LIB_PLL_LOCK_TIMEOUT)
                return -1;
        }

        iowrite32 (0x0, CRG_DDR_BASE + CRG_PLL_CTRL);
        while ((ioread32 (CRG_DDR_BASE + CRG_PLL_STAT) & 0x00000011) == 0)
        {
            timer_cntr++;
            if (timer_cntr == CRG_DDR_TEST_LIB_PLL_LOCK_TIMEOUT)
                return -1;
        }
    }

    return 0;
}

static int crg_init_done;
int ddr_do_init (int slot, const char* memtype)
{
    uintptr_t DDRx_BASE; 
    switch (slot) {
        case 0:
            DDRx_BASE = DDR0_BASE;
            break;
        case 1:
            DDRx_BASE = DDR1_BASE;
            break;
        default:
            return 1;
    }

    if (!crg_init_done) {
        crg_init_done++;
        crg_ddr_init (0x63 ,0x0);
    }

    if (strcmp(memtype, "mt41k256m8")==0) {
        mt41k256m8(DDRx_BASE);
    } else if (strcmp(memtype, "mt41j512m8"))
    {
        mt41j512m8(DDRx_BASE);        
    } else {
        return 1;
    }

    uint32_t timer_cntr = 0;
    while ((ioread32(DDRx_BASE + DENALI_CTL_94) & 0x00000800) == 0)
    {
        timer_cntr++;
        if (timer_cntr == DDR_TEST_LIB_PLL_LOCK_TIMEOUT)
            return 1;
    }
    iowrite32(0b00000000000000000000100000000000, DDRx_BASE + DENALI_CTL_95); // clear interruption flag
    return 0;
}

#if 0
static uint32_t ddr_init (uint32_t DDRx_BASE)
{
    rumboot_printf("DDR: Initializing controller 0x%x with %s\n", DDRx_BASE, DDR_HEADER_FILE);
    uint32_t timer_cntr = 0;

    // crg_ddr_init (0x84 ,0x1);
    crg_ddr_init (0x63 ,0x0);
    ddr_registers_init (DDRx_BASE);
    
    while ((ioread32(DDRx_BASE + DENALI_CTL_94) & 0x00000800) == 0)
    {
        timer_cntr++;
        if (timer_cntr == DDR_TEST_LIB_PLL_LOCK_TIMEOUT)
            return -1;
    }
    iowrite32(0b00000000000000000000100000000000, DDRx_BASE + DENALI_CTL_95); // clear interruption flag

    return 0;
}
#endif

//-----------------------------------------------------------------------------
//  This function is for turning On both DDR subsystems
//  It must take less time, than two consequent ddr_init with different args
//-----------------------------------------------------------------------------

//extern char rumboot_ddr0_heap_start;
//extern char rumboot_ddr1_heap_start;
/* 
int main()
{
    rumboot_printf("DDR: Initializing both controllers with %s\n", DDR_HEADER_FILE);
    uint32_t timer_cntr = 0;

    crg_ddr_init (0x63 ,0x0);
    ddr_registers_init (DDR0_BASE);
    ddr_registers_init (DDR1_BASE);
    
    while (((ioread32(DDR0_BASE + DENALI_CTL_94)) & (ioread32(DDR1_BASE + DENALI_CTL_94)) & 0x00000800) == 0)
    {
        timer_cntr++;
        if (timer_cntr == DDR_TEST_LIB_PLL_LOCK_TIMEOUT) {
            rumboot_printf("Timeout waiting for PLL to lock\n");
            return -1;
        }
    }
    iowrite32(0b00000000000000000000100000000000, DDR0_BASE + DENALI_CTL_95); // clear interruption flag
    iowrite32(0b00000000000000000000100000000000, DDR1_BASE + DENALI_CTL_95); // clear interruption flag

    uint64_t ret = 0; 
    #ifdef TEST_MEMORY
        ret = memtest(&rumboot_ddr0_heap_start, 128*1024*1024);
        if (ret != 0) {
            rumboot_printf("DDR0 init didn't quite work out\n");
            return 1;
        }

        ret = memtest(&rumboot_ddr1_heap_start, 128*1024*1024);
        if (ret != 0) {
            rumboot_printf("DDR1 init didn't quite work out\n");
            return 1;
        }
    #endif


    rumboot_printf("DDR: Initialization completed\n");
    return 0;
}
*/