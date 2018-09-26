#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <platform/common_macros/common_macros.h>
#include <platform/test_event_codes.h>
#include <platform/devices.h>
#include <rumboot/irq.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <platform/interrupts.h>
#include <platform/test_assert.h>
#include <platform/regs/fields/mpic128.h>

#define NON_EXISTENT_ADDR   0xC0403000
#define TEST_VALUE          0xBABADEDA
#define SEAR_upper          0x1
#define SEAR_lower          0x2
#define SESR                0x3

//REG_WRITE_DCR( PLB4AHB, SEAR_upper, 32 )
//REG_WRITE_DCR( PLB4AHB, SEAR_lower, 32 )
//REG_WRITE_DCR( PLB4AHB, SESR, 32 )


typedef enum test_event_code
{
    TEC_START_CHECK_HRESP   = TEST_EVENT_CODE_MIN,
    TEC_START_CHECK_CELL1,
    TEC_START_CHECK_CELL2
} test_event_code;

int main()
{
    test_event_send_test_id( "test_oi10_sys_005" );

    rumboot_printf("Checking access to existing devices in AXI\n");
    iowrite32(TEST_VALUE, IM1_BASE);
    rumboot_printf("Read value = 0x%\n", ioread32(IM1_BASE));
    test_event(TEC_START_CHECK_CELL1);
    rumboot_printf("Checking: OK\n");

//    GMII_write_MAC_LSB(GMII1_BASE, 0x0);

    rumboot_printf("Checking access to non-existent devices in AXI\n");
    iowrite32(TEST_VALUE, NON_EXISTENT_ADDR);
    test_event(TEC_START_CHECK_HRESP);
    rumboot_printf("Checking: OK\n");

    rumboot_printf("Cleaning of registers SEAR, SESR\n");
//    PLB4AHB_dcr_write_SEAR_upper(PLB4AHB_BASE,0xF);
//    PLB4AHB_dcr_write_SEAR_lower(PLB4AHB_BASE,0xF);
//    PLB4AHB_dcr_write_SESR(PLB4AHB_BASE,0xF);

    rumboot_printf("Checking access to existing devices in AXI\n");
    iowrite32(TEST_VALUE, IM1_BASE + 0x20);
    rumboot_printf("Read value = 0x%\n", ioread32(IM1_BASE + 0x20));
    test_event(TEC_START_CHECK_CELL2);
    rumboot_printf("Checking: OK\n");

    return 0;
}


