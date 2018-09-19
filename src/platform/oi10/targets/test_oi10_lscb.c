
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/regpoker.h>
#include <rumboot/macros.h>

#include <platform/devices.h>
#include <platform/interrupts.h>
#include <platform/test_assert.h>

#include <regs/regs_mkio.h>
#include <devices/mkio.h>

/*
 * Registers access checks
 */
struct regpoker_checker mkio_check_array[] =
{
    { "IRQ    ", REGPOKER_READ32,  IRQ   , 0x00000000, 0xFFFFFFFF},
    { "IRQE   ", REGPOKER_READ32,  IRQE  , 0x00000000, 0xFFFFFFFF},
    { "HWCFG  ", REGPOKER_READ32,  HWCFG , 0x00000000, 0xFFFFFFFF},
    { "BCSL   ", REGPOKER_READ32,  BCSL  , 0xF0000000, 0xFFFFFFFF},
    { "BCA    ", REGPOKER_READ32,  BCA   , 0x00000000, 0xFFFFFFFF},
    { "BCTNP  ", REGPOKER_READ32,  BCTNP , 0x00000000, 0xFFFFFFFF},
    { "BCANP  ", REGPOKER_READ32,  BCANP , 0x00000000, 0xFFFFFFFF},
    { "BCT    ", REGPOKER_READ32,  BCT   , 0x00000000, 0xFFFFFFFF},
    { "BCTW   ", REGPOKER_READ32,  BCTW  , 0x00000000, 0xFFFFFFFF},
    { "BCRD   ", REGPOKER_READ32,  BCRD  , 0x00000000, 0xFFFFFFFF},
    { "BCBS   ", REGPOKER_READ32,  BCBS  , 0x00000000, 0xFFFFFFFF},
    { "BCTCP  ", REGPOKER_READ32,  BCTCP , 0x00000000, 0xFFFFFFFF},
    { "BCACP  ", REGPOKER_READ32,  BCACP , 0x00000000, 0xFFFFFFFF},
    { "RTS    ", REGPOKER_READ32,  RTS   , 0x80000000, 0xFFFFFFFF},
    { "RTC    ", REGPOKER_READ32,  RTC   , 0x0000E03E, 0xFFFFFFFF},
    { "RTBS   ", REGPOKER_READ32,  RTBS  , 0x00000000, 0xFFFFFFFF},
    { "RTSW   ", REGPOKER_READ32,  RTSW  , 0x00000000, 0xFFFFFFFF},
    { "RTSY   ", REGPOKER_READ32,  RTSY  , 0x00000000, 0xFFFFFFFF},
    { "RTSTBA ", REGPOKER_READ32,  RTSTBA, 0x00000000, 0xFFFFFFFF},
    { "RTMCC  ", REGPOKER_READ32,  RTMCC , 0x00000555, 0xFFFFFFFF},
    { "RTTTC  ", REGPOKER_READ32,  RTTTC , 0x00000000, 0xFFFFFFFF},
    { "RTELM  ", REGPOKER_READ32,  RTELM , 0xFFFFFFFC, 0xFFFFFFFF},
    { "RTELP  ", REGPOKER_READ32,  RTELP , 0x00000000, 0xFFFFFFFF},
    { "RTELIP ", REGPOKER_READ32,  RTELIP, 0x00000000, 0xFFFFFFFF},
    { "BMS    ", REGPOKER_READ32,  BMS   , 0x80000000, 0xFFFFFFFF},
    { "BMC    ", REGPOKER_READ32,  BMC   , 0x00000000, 0xFFFFFFFF},
    { "BMRTAF ", REGPOKER_READ32,  BMRTAF, 0xFFFFFFFF, 0xFFFFFFFF},
    { "BMRTSF ", REGPOKER_READ32,  BMRTSF, 0xFFFFFFFF, 0xFFFFFFFF},
    { "BMRTMC ", REGPOKER_READ32,  BMRTMC, 0xFFFFFFFF, 0xFFFFFFFF},
    { "BMLBS  ", REGPOKER_READ32,  BMLBS , 0x00000000, 0xFFFFFFFF},
    { "BMLBE  ", REGPOKER_READ32,  BMLBE , 0x00000007, 0xFFFFFFFF},
    { "BMLBP  ", REGPOKER_READ32,  BMLBP , 0x00000000, 0xFFFFFFFF},
    { "BMTTC  ", REGPOKER_READ32,  BMTTC , 0x00000000, 0xFFFFFFFF},
    { "BCRD   ", REGPOKER_WRITE32, BCRD  , 0x00000000, 0xFFFFFFFC},
    { "BCBS   ", REGPOKER_WRITE32, BCBS  , 0x00000000, 0xFFFFFFFF},
    { "RTSTBA ", REGPOKER_WRITE32, RTSTBA, 0x00000000, 0xFFFFFE00},
    { "RTTTC  ", REGPOKER_WRITE32, RTTTC , 0x00000000, 0xFFFFFFFF},
    { "RTELP  ", REGPOKER_WRITE32, RTELP , 0x00000000, 0xFFFFFFFC},
    { "BMRTAF ", REGPOKER_WRITE32, BMRTAF, 0xFFFFFFFF, 0xFFFFFFFF},
    { "BMRTSF ", REGPOKER_WRITE32, BMRTSF, 0xFFFFFFFF, 0xFFFFFFFF},
    { "BMLBS  ", REGPOKER_WRITE32, BMLBS , 0x00000000, 0xFFFFFFF8},
    { "BMLBE  ", REGPOKER_WRITE32, BMLBE , 0x00000007, 0x003FFFF8},
    { "BMLBP  ", REGPOKER_WRITE32, BMLBP , 0x00000000, 0x003FFFF8},
    { "BMTTC  ", REGPOKER_WRITE32, BMTTC , 0x00000000, 0xFFFFFFFF},
};

void regs_check(uint32_t base_address)
{
    TEST_ASSERT(rumboot_regpoker_check_array(mkio_check_array, base_address)==0, "Failed to check MKIO registers\n");
}

int main(void)
{
#ifdef CHECK_MKIO_REGS
    rumboot_printf("Start test_oi10_lscb. Registers access checks for MKIO module (0x%X)\n", MKIO_BASE);
    regs_check(MKIO_BASE);
#endif
    return 0;
}

