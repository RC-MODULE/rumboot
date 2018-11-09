#include <stdint.h>
#include <platform/devices.h>
#include <rumboot/io.h>
#include <regs/regs_gpio_rcm.h>
#include <rumboot/boot.h>
#include <rumboot/platform.h>
#include <rumboot/bootsrc/spiflash.h>
#include <rumboot/bootsrc/eeprom.h>
#include <rumboot/bootsrc/boilerplate.h>
#include <rumboot/bootsrc/sdio.h>
#include <platform/regs/sctl.h>
#include <rumboot/regpoker.h>
#include <rumboot/testsuite.h>
#include <rumboot/printf.h>
#include <rumboot/timer.h>
#include <devices/bisr.h>
#include <regs/regs_pcie.h>

#define BISR_TIMEOUT 50000

static bool check_gp_started(uint32_t arg)
{
        iowrite32(1, GLOBAL_TIMERS + 0x20);
        uint64_t v = ioread32(GLOBAL_TIMERS + 0xc);
        v = v << 32;
        v |= ioread32(GLOBAL_TIMERS + 0x8);
        return v != 0;
}


static void st_bisr_start(uintptr_t base)
{
        uint32_t sctlreg = ioread32(base);

        iowrite32(sctlreg | (1 << SCTL_START_i), base);
}

static void st_bisr_stop(uintptr_t base)
{
        uint32_t sctlreg = ioread32(base);

        iowrite32(sctlreg & ~(1 << SCTL_START_i), base);
}

static bool st_bisr_wait(uintptr_t base, uint32_t timeout)
{
        uint32_t start = rumboot_platform_get_uptime();

        while (!(ioread32(base) & (1 << SCTL_FINISH_i))) {
                if (rumboot_platform_get_uptime() - start > timeout) {
                        rumboot_printf("Error: Timeout waiting for BISR to complete.\n");
                        return false;
                }
        }
        return true;
}

int st_bisr_analyze(uintptr_t base)
{
        uint32_t l2cbisr = ioread32(base);

        bool rslt = l2cbisr & (1 << SCTL_RSLT_i);
        bool redok = l2cbisr & (1 << SCTL_REDOK_i);

        if (!redok && !rslt) {
                return BISR_MEM_FAIL;
        } else {
                if (!redok && rslt) {
                        return BISR_MEM_GOOD;
                } else {
                        if (redok && rslt) {
                                return BISR_MEM_PERFECT;
                        } else {
                                return BISR_TEST_TIMEOUT;
                        }
                }
        }
}


static bool check_pcie_mbist(uint32_t base)
{
        //-----------------------------------------------------------------
        //  PCIe Reset Off - start PLL
        //  And wait PLL is locked
        //-----------------------------------------------------------------
        iowrite32(0x1, SCTL_BASE + SCTL_PCIE_RST);

        int done = 0;
        LOOP_UNTIL_TIMEOUT(100000) {
                if ((ioread32(SCTL_BASE + SCTL_PCIE_RST) & 0x1) == 0x1) {
                        done = 1;
                        break;
                }
        }
        if (!done) {
                rumboot_printf("Error: Timeout waiting for PLL to lock.\n");
                return false;
        }

        rumboot_printf("MBSIT: PLL now locked\n");

        //-----------------------------------------------------------------
        //  Enable MBIST functionality
        //-----------------------------------------------------------------
        iowrite32(0x04000039, SCTL_BASE + SCTL_PCIE_REG_0);

        //-----------------------------------------------------------------
        //  Check, MBIST enabled
        //    This "if" is used like delay function
        //-----------------------------------------------------------------
        if ((ioread32(SCTL_BASE + SCTL_PCIE_REG_0) & 0x04000000) != 0x04000000) {
        }

        //-----------------------------------------------------------------
        //  Start MBIST
        //-----------------------------------------------------------------
        iowrite32(0x04800039, SCTL_BASE + SCTL_PCIE_REG_0);

        //-----------------------------------------------------------------
        //  Wait MBIST done
        //-----------------------------------------------------------------
        done = 0;
        LOOP_UNTIL_TIMEOUT(100000) {
                if ((ioread32(SCTL_BASE + SCTL_PCIE_REG_0) & 0x02000000) == 0x02000000) {
                        done = 1;
                        break;
                }
        }

        if (!done) {
                rumboot_printf("Error: Timeout waiting for MBIST to finish.\n");
                return false;
        }

        rumboot_printf("MBIST: completed\n");
        //-----------------------------------------------------------------
        //  Check, if MBIST failed
        //-----------------------------------------------------------------
        if ((ioread32(SCTL_BASE + SCTL_PCIE_REG_0) & 0x01000000) == 0x01000000) {
                return false;
        }
        return true;
}

static bool check_bisr(uint32_t base)
{
        iowrite32(0x1, SCTL_BASE + SCTL_BISR_CLK_EN);
        st_bisr_start(base);
        udelay(1);
        st_bisr_stop(base);
        st_bisr_wait(base, BISR_TIMEOUT);

        int ret = st_bisr_analyze(base);
        rumboot_printf("Memory redundancy check: ");
        switch (ret) {
        case BISR_MEM_FAIL:
                rumboot_printf("Junk\n");
                break;
        case BISR_TEST_TIMEOUT:
                rumboot_printf("Timeout\n");
                break;
        case BISR_MEM_PERFECT:
                rumboot_printf("Perfect\n");
                break;
        case BISR_MEM_GOOD:
                rumboot_printf("Good\n");
                break;
        }
        iowrite32(0x0, SCTL_BASE + SCTL_BISR_CLK_EN);
        return !((ret == BISR_MEM_FAIL) || (ret == BISR_TEST_TIMEOUT));
}


TEST_SUITE_BEGIN(selftest, "Selftest")
TEST_ENTRY("Global Timers", check_gp_started, GLOBAL_TIMERS),
TEST_ENTRY("PCIe MBIST", check_pcie_mbist, SCTL_BASE + SCTL_BISR_L2C),
TEST_ENTRY("L2C BISR", check_bisr, SCTL_BASE + SCTL_BISR_L2C),
TEST_SUITE_END();

void rumboot_platform_selftest(struct rumboot_config *conf)
{
        /* Execute selftest routines */
        int ret = test_suite_run(NULL, &selftest);

        rumboot_printf("selftest: %d test from suite failed\n", ret);
}
