#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/timer.h>

#include <platform/common_macros/common_macros.h>
#include <platform/devices.h>
#include <platform/devices/emi.h>

#define RUMBOOT_ASSERT_WARN_ONLY
#include <platform/devices/nor_1636RR4.h>
#undef RUMBOOT_ASSERT_WARN_ONLY

#include <platform/regs/regs_emi.h>
#include <platform/test_event_c.h>
#include <platform/test_event_codes.h>


#define EVENT_INJECT_SRAM0_0                    TEST_EVENT_CODE_MIN + 0
#define EVENT_INJECT_SRAM0_7                    TEST_EVENT_CODE_MIN + 1

#define EVENT_INJECT_SSRAM_0                    TEST_EVENT_CODE_MIN + 2
#define EVENT_INJECT_SSRAM_7                    TEST_EVENT_CODE_MIN + 3

#define EVENT_INJECT_SDRAM_0                    TEST_EVENT_CODE_MIN + 4
#define EVENT_INJECT_SDRAM_7                    TEST_EVENT_CODE_MIN + 5

#define EVENT_INJECT_NOR_0                      TEST_EVENT_CODE_MIN + 6
#define EVENT_INJECT_NOR_7                      TEST_EVENT_CODE_MIN + 7


#define ADDR_SRAM0_SE                           SRAM0_BASE
#define ADDR_SRAM0_DE                           SRAM0_BASE + 0x100

#define ADDR_SSRAM_SE                           SSRAM_BASE
#define ADDR_SSRAM_DE                           SSRAM_BASE + 0x100

#define ADDR_SDRAM_SE                           SDRAM_BASE
#define ADDR_SDRAM_DE                           SDRAM_BASE + 0x100

#define ADDR_NOR_SE                             NOR_BASE + 0x10000
#define ADDR_NOR_DE                             NOR_BASE + 0x10100


bool check_data()
{
    uint32_t reg = 0, stat = 0;

    dcr_write(DCR_EM2_EMI_BASE + EMI_ECNT20, 0x00); //clear reg
    dcr_write(DCR_EM2_EMI_BASE + EMI_ECNT53, 0x00); //clear reg


    reg = ioread32(ADDR_SRAM0_SE);
    if (reg != 0xBABA0001)
    {
        rumboot_putstring("ECC ERROR SRAM0 (Single error)\n");
        rumboot_printf("ADDR = 0x%x\n", ADDR_SRAM0_SE);
        rumboot_printf("Expected = 0x%x\n", 0xBABA0001);
        rumboot_printf("Actual = 0x%x\n",reg);
        return false;
    }
    stat = dcr_read(DCR_EM2_EMI_BASE + EMI_ECNT20);
    rumboot_printf("stat = 0x%x\n", stat);
    TEST_ASSERT(stat == 0x000001, "Invalid value in STATUS_IRQ (SRAM0)");

    reg = ioread32(ADDR_SRAM0_DE);
    if (reg != 0xBABA0007) //see .svh
    {
        rumboot_putstring("ECC ERROR SRAM0 (Double error)\n");
        rumboot_printf("ADDR = 0x%x\n", ADDR_SRAM0_DE);
        rumboot_printf("Expected = 0x%x\n", 0xBABA0007);
        rumboot_printf("Actual = 0x%x\n", reg);
        return false;
    }
    stat = dcr_read(DCR_EM2_EMI_BASE + EMI_ECNT20);
    rumboot_printf("stat = 0x%x\n", stat);
    TEST_ASSERT(stat == 0x000001, "Invalid value in STATUS_IRQ (SRAM)");


    reg = ioread32(ADDR_SSRAM_SE);
    if (reg != 0xBABA0001)
    {
        rumboot_putstring("ECC ERROR SSRAM (Single error)\n");
        rumboot_printf("ADDR = 0x%x\n", ADDR_SSRAM_SE);
        rumboot_printf("Expected = 0x%x\n", 0xBABA0001);
        rumboot_printf("Actual = 0x%x\n",reg);
        return false;
    }
    stat = dcr_read(DCR_EM2_EMI_BASE + EMI_ECNT20);
    TEST_ASSERT(stat == 0x100, "Invalid value in STATUS_IRQ (SSRAM)");

    reg = ioread32(ADDR_SSRAM_DE);
    if (reg != 0xBABA0007) //see .svh
    {
        rumboot_putstring("ECC ERROR SSRAM (Double error)\n");
        rumboot_printf("ADDR = 0x%x\n", ADDR_SSRAM_DE);
        rumboot_printf("Expected = 0x%x\n", 0xBABA0007);
        rumboot_printf("Actual = 0x%x\n", reg);
        return false;
    }
    stat = dcr_read(DCR_EM2_EMI_BASE + EMI_ECNT20);
    TEST_ASSERT(stat == 0x200, "Invalid value in STATUS_IRQ (SSRAM)");


    reg = ioread32(ADDR_SDRAM_SE);
    if (reg != 0xBABA0001)
    {
        rumboot_putstring("ECC ERROR SDRAM (Single error)\n");
        rumboot_printf("ADDR = 0x%x\n", ADDR_SDRAM_SE);
        rumboot_printf("Expected = 0x%x\n", 0xBABA0001);
        rumboot_printf("Actual = 0x%x\n",reg);
        return false;
    }
    stat = dcr_read(DCR_EM2_EMI_BASE + EMI_ECNT20);
    TEST_ASSERT(stat == 0x010, "Invalid value in STATUS_IRQ (SDRAM)");

    reg = ioread32(ADDR_SDRAM_DE);
    if (reg != 0xBABA0007) //see .svh
    {
        rumboot_putstring("ECC ERROR SDRAM (Double error)\n");
        rumboot_printf("ADDR = 0x%x\n", ADDR_SDRAM_DE);
        rumboot_printf("Expected = 0x%x\n", 0xBABA0007);
        rumboot_printf("Actual = 0x%x\n", reg);
        return false;
    }
    stat = dcr_read(DCR_EM2_EMI_BASE + EMI_ECNT20);
    TEST_ASSERT(stat == 0x020, "Invalid value in STATUS_IRQ (SDRAM)");


    reg = ioread32(ADDR_NOR_SE);
    if (reg != 0xBABA0001)
    {
        rumboot_putstring("ECC ERROR NOR (Single error)\n");
        rumboot_printf("ADDR = 0x%x\n", ADDR_NOR_SE);
        rumboot_printf("Expected = 0x%x\n", 0xBABA0001);
        rumboot_printf("Actual = 0x%x\n", reg);
        return false;
    }
    stat = dcr_read(DCR_EM2_EMI_BASE + EMI_ECNT53);
    rumboot_printf("stat = 0x%x\n", stat);
    TEST_ASSERT(stat == 0x010000, "Invalid value in STATUS_IRQ (NOR)");

    reg = ioread32(ADDR_NOR_DE);
    if (reg != 0xBABA0007) //see .svh
    {
        rumboot_putstring("ECC ERROR NOR (Double error)\n");
        rumboot_printf("ADDR = 0x%x\n", ADDR_NOR_DE);
        rumboot_printf("Expected = 0x%x\n", 0xBABA0007);
        rumboot_printf("Actual = 0x%x\n", reg);
        return false;
    }
    stat = dcr_read(DCR_EM2_EMI_BASE + EMI_ECNT53);
    rumboot_printf("stat = 0x%x\n", stat);
    TEST_ASSERT(stat == 0x010000, "Invalid value in STATUS_IRQ (NOR)");

    return true;
}


int main(void) 
{
    test_event_send_test_id("test_oi10_em2_205");

    emi_init();
    emi_set_ecc(DCR_EM2_EMI_BASE, emi_bank_all, emi_ecc_on);



//    rumboot_putstring_fast("WRITE SRAM0");
//    iowrite32(0xBABA0001, ADDR_SRAM0_SE);
//    rumboot_printf("ADDR_SRAM0_SE = 0x%x\n", ioread32(ADDR_SRAM0_SE));
//
////    rumboot_putstring_fast("WRITE SSRAM");
////    iowrite32(0xBABA0001, ADDR_SSRAM_SE);
////    rumboot_printf("ADDR_SSRAM_SE = 0x%x\n", ioread32(ADDR_SSRAM_SE));
//
//    rumboot_putstring_fast("WRITE SDRAM");
//    iowrite32(0xBABA0001, ADDR_SDRAM_SE);
//    rumboot_printf("ADDR_SDRAM_SE = 0x%x\n", ioread32(ADDR_SDRAM_SE));
//
//    rumboot_putstring_fast("WRITE NOR");
//    nor_write32(0xBABA0001, ADDR_NOR_SE);
//    rumboot_printf("ADDR_NOR_SE = 0x%x\n", ioread32(ADDR_NOR_SE));
//
//    return 0;


/*
    uint32_t mask_reg = 0;
    mask_reg = sram_nor_dcr_read_SRAM_NOR_DCR_MASK_IRQ(EM2_SRAM_DCRAPB_DCR_BASE);
    sram_nor_dcr_write_SRAM_NOR_DCR_MASK_IRQ(EM2_SRAM_DCRAPB_DCR_BASE,
            (mask_reg |
            (0x01 << SRAM_NOR_DCR_MASK_IRQ_S_ERR_i))
    );

    mask_reg = sram_nor_dcr_read_SRAM_NOR_DCR_MASK_IRQ(EM3_NOR_DCRAPB_DCR_BASE);
    sram_nor_dcr_write_SRAM_NOR_DCR_MASK_IRQ(EM3_NOR_DCRAPB_DCR_BASE,
            (mask_reg |
            (0x01 << SRAM_NOR_DCR_MASK_IRQ_S_ERR_i))
    );
*/
    rumboot_putstring("WRITE SRAM0\n");
    test_event(EVENT_INJECT_SRAM0_0); //DATA_SE = 0xBABA0000
    iowrite32(0xBABA0001, ADDR_SRAM0_SE);

    test_event(EVENT_INJECT_SRAM0_7); //DATA_DE = 0xBABA0007
    iowrite32(0xBABA0001, ADDR_SRAM0_DE);


    rumboot_putstring("WRITE SSRAM\n");
    iowrite32(0xBABA0001, ADDR_SSRAM_SE);
    test_event(EVENT_INJECT_SSRAM_0); //DATA_SE = 0xBABA0000

    iowrite32(0xBABA0001, ADDR_SSRAM_DE);
    test_event(EVENT_INJECT_SSRAM_7); //DATA_SE = 0xBABA0007


    rumboot_putstring("WRITE SDRAM\n");
    iowrite32(0xBABA0001, ADDR_SDRAM_SE);
    test_event(EVENT_INJECT_SDRAM_0); //DATA_SE = 0xBABA0000

    iowrite32(0xBABA0001, ADDR_SDRAM_DE);
    test_event(EVENT_INJECT_SDRAM_7); //DATA_SE = 0xBABA0007


    rumboot_putstring("WRITE NOR\n");

    test_event(EVENT_INJECT_NOR_0); //DATA_SE = 0xBABA0000
    nor_write32(0xBABA0001, ADDR_NOR_SE);

    test_event(EVENT_INJECT_NOR_7); //DATA_DE = 0xBABA0007
    nor_write32(0xBABA0001, ADDR_NOR_DE);

    udelay(5);

    rumboot_putstring("CHECK\n");
    if (!check_data())
        return 1;

    return 0;
}
