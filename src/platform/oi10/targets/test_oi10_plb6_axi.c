
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <rumboot/irq.h>
#include <rumboot/timer.h>
#include <rumboot/regpoker.h>
#include <rumboot/irq.h>

#include <platform/interrupts.h>
#include <platform/regs/fields/mpic128.h>
#include <platform/devices/emi.h>
#include <platform/test_assert.h>

#include <platform/devices/greth.h>

/*
 * Registers access checks
 */
struct regpoker_checker greth_check_array[] = {
    { "CTRL              ",  REGPOKER_READ32,  CTRL              , 0x98000090, 0xFE007CFF },
    { "MDIO_CTRL         ",  REGPOKER_READ32,  MDIO_CTRL         , 0x01E10140, 0xFFFFFFCF },
    { "TRANSMIT_DESCR_PTR",  REGPOKER_READ32,  TRANSMIT_DESCR_PTR, 0x0, 0x3F8 },
    { "RECEIVER_DESCR_PTR",  REGPOKER_READ32,  RECEIVER_DESCR_PTR, 0x0, 0x3F8 },
    { "EDCL_IP           ",  REGPOKER_READ32,  EDCL_IP           , EDCLIP0, ~0 },
    { "EDCL_MAC_MSB      ",  REGPOKER_READ32,  EDCL_MAC_MSB      , EDCLMAC_MSB, 0xffff },
    { "EDCL_MAC_LSB      ",  REGPOKER_READ32,  EDCL_MAC_LSB      , EDCLMAC_LSB, ~0 },
    { "EDCL_IP           ",  REGPOKER_WRITE32, EDCL_IP           , 0, ~0 },
    { "EDCL_MAC_MSB      ",  REGPOKER_WRITE32, EDCL_MAC_MSB      , 0, 0xffff },
    { "EDCL_MAC_LSB      ",  REGPOKER_WRITE32, EDCL_MAC_LSB      , 0, ~0 }
};

void regs_check(uint32_t base_addr)
{
    if (base_addr==GRETH_1_BASE)
    {
        rumboot_printf("Checking access to GRETH%d(0x%x) registers\n", 1, base_addr);
        greth_check_array[4].expected = EDCLIP1;
        greth_check_array[6].expected |= EDCLADDRL1;
    }
    else
    {
        rumboot_printf("Checking access to GRETH%d(0x%x) registers\n", 0, base_addr);
    }
    TEST_ASSERT(rumboot_regpoker_check_array(greth_check_array, base_addr)==0, "Failed to check GRETH registers\n");
}

int main(void) 
{

#ifdef CHECK_PLB6_AXI_SINGLE
    rumboot_putstring("Start test_plb6_axi_greth. Apply access to GRETH0 regs for checking PLB6->AXI convertion for single transactions\n");
    regs_check(GRETH_0_BASE);
#endif

    return 0;
}
