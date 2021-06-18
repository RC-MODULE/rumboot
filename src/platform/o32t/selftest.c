#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <rumboot/platform.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/rumboot.h>
#include <rumboot/boot.h>
#include <rumboot/regpoker.h>
#include <rumboot/testsuite.h>
#include <platform/devices.h>
#include <rumboot/timer.h>

#define     UARTPeriphID0_MSK   0xFF
#define     UARTPeriphID1_MSK   0xFF
#define     UARTPeriphID2_MSK   0x0F
#define     UARTPeriphID3_MSK   0xFF
#define     UARTPCellID0_MSK    0xFF
#define     UARTPCellID1_MSK    0xFF
#define     UARTPCellID2_MSK    0xFF
#define     UARTPCellID3_MSK    0xFF
#define     UARTPeriphID0_DEFAULT   0x11
#define     UARTPeriphID1_DEFAULT   0x10
#define     UARTPeriphID2_DEFAULT   0x4
#define     UARTPeriphID3_DEFAULT   0x0
#define     UARTPCellID0_DEFAULT    0x0D
#define     UARTPCellID1_DEFAULT    0xF0
#define     UARTPCellID2_DEFAULT    0x05
#define     UARTPCellID3_DEFAULT    0xB1

#include <regs/regs_uart_pl011.h>

const struct regpoker_checker check_uart_regs[] = {
 { "UARTPeriphID0", REGPOKER_READ32, UARTPeriphID0, UARTPeriphID0_DEFAULT,  UARTPeriphID0_MSK },
 { "UARTPeriphID1", REGPOKER_READ32, UARTPeriphID1, UARTPeriphID1_DEFAULT,  UARTPeriphID1_MSK },
 { "UARTPeriphID2", REGPOKER_READ32, UARTPeriphID2, UARTPeriphID2_DEFAULT,  UARTPeriphID2_MSK },
 { "UARTPeriphID3", REGPOKER_READ32, UARTPeriphID3, UARTPeriphID3_DEFAULT,  UARTPeriphID3_MSK },
 { "UARTPCellID0",  REGPOKER_READ32, UARTPCellID0,  UARTPCellID0_DEFAULT,   UARTPCellID0_MSK },
 { "UARTPCellID1",  REGPOKER_READ32, UARTPCellID1,  UARTPCellID1_DEFAULT,   UARTPCellID1_MSK },
 { "UARTPCellID2",  REGPOKER_READ32, UARTPCellID2,  UARTPCellID2_DEFAULT,   UARTPCellID2_MSK },
 { "UARTPCellID3",  REGPOKER_READ32, UARTPCellID3,  UARTPCellID3_DEFAULT,   UARTPCellID3_MSK },
 { /* Sentinel */ }
};

static bool check_pl011_regs(uint32_t arg)
{
    /* Return true if test passed, false otherwise */
    return (rumboot_regpoker_check_array(check_uart_regs, arg) == 0);
}

static bool check_kmbist(uint32_t arg)
{
    uint32_t timeout = 20000; /* us */
    dcr_write(arg, 0x16);
    LOOP_UNTIL_TIMEOUT(timeout) {
        if ((dcr_read(arg) & (1<<6) ))
            break;
    }
    bool result = ! (dcr_read(arg) & (1<<5) );
    dcr_write(arg, 0);
    return result;
}

static bool check_kmbist_im0(uint32_t arg)
{
    uint32_t oldstack;
    uint32_t tempstack;
    bool result = false;
    uint32_t stacksize = ((&rumboot_platform_stack_area_end) - (&rumboot_platform_stack_area_start));
    asm("msync");
    asm("mr %0, %%r1" : "=r" (oldstack));
    tempstack = IM1_BASE + (oldstack - IM0_BASE);
    rumboot_printf("Relocating stack: 0x%x -> 0x%x\n", oldstack, tempstack);
    memcpy(IM1_BASE, &rumboot_platform_stack_area_start, stacksize);
    asm("mr %%r1, %0" :: "r" (tempstack));
    #ifdef CMAKE_BUILD_TYPE_DEBUG
        rumboot_printf("Debug: wiping old stack to simulate KMBIST effects\n");    
        memset(&rumboot_platform_stack_area_start, 0x0, stacksize);
    #endif
    result = check_kmbist(arg);
    asm("msync");
    asm("mr %0, %%r1" :: "r" (tempstack));
    rumboot_printf("Restoring stack: 0x%x -> 0x%x\n", tempstack, oldstack);
    memcpy(&rumboot_platform_stack_area_start, IM1_BASE, stacksize);
    asm("mr %0, %%r1" : "=r" (oldstack));    
    return true;
}

TEST_SUITE_BEGIN(selftest, "Selftest")
TEST_ENTRY("UART0:      ID Registers", check_pl011_regs, UART0_BASE),
TEST_ENTRY("UART1:      ID Registers", check_pl011_regs, UART1_BASE),
TEST_ENTRY("KMBIST #0:  PPC L1C", check_kmbist, DCR_SCTL_BASE + 0x4),
TEST_ENTRY("KMBIST #1:  i-trace/l-trace", check_kmbist, DCR_SCTL_BASE + 0x8),
TEST_ENTRY("KMBIST #2:  PPC L2C", check_kmbist, DCR_SCTL_BASE + 0xC),
TEST_ENTRY("KMBIST #3:  SpaceWire", check_kmbist, DCR_SCTL_BASE + 0x10),
TEST_ENTRY("KMBIST #4:  Ethernet", check_kmbist, DCR_SCTL_BASE + 0x14),
TEST_ENTRY("KMBIST #5:  SPI/SDIO", check_kmbist, DCR_SCTL_BASE + 0x18),
TEST_ENTRY("KMBIST #6:  IM0 Memory", check_kmbist_im0, DCR_SCTL_BASE + 0x1C),
TEST_ENTRY("KMBIST #7:  IM1 Memory", check_kmbist, DCR_SCTL_BASE + 0x20),
TEST_ENTRY("KMBIST #8:  IM2 Memory", check_kmbist, DCR_SCTL_BASE + 0x24),
TEST_ENTRY("KMBIST #9:  IM3 Memory", check_kmbist, DCR_SCTL_BASE + 0x48),
TEST_ENTRY("KMBIST #10: NMPU", check_kmbist, DCR_SCTL_BASE + 0x4C),
TEST_ENTRY("KMBIST #11: AXI2MCIF Bridge", check_kmbist, DCR_SCTL_BASE + 0x50),
TEST_SUITE_END();

int rumboot_platform_selftest(struct rumboot_config *conf)
{
        /* Execute selftest routines */
        int ret = test_suite_run(NULL, &selftest);
        return ret;
}
