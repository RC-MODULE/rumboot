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
#include <platform/regs/regs_plb4ahb.h>

#define NON_EXISTENT_ADDR   0xC0025000
#define TEST_VALUE          0xBABADEDA

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
    test_event(TEC_START_CHECK_CELL1);
    iowrite32(TEST_VALUE, IM1_BASE);
    rumboot_printf("Checking: OK\n");


    rumboot_printf("Checking access to non-existent devices in AXI\n");
    test_event(TEC_START_CHECK_HRESP);
    iowrite32(TEST_VALUE, NON_EXISTENT_ADDR);
    rumboot_printf("Checking: OK\n");

    rumboot_printf("Cleaning of registers SEAR, SESR\n");
    dcr_write(DCR_PLB4AHB_0_BASE + SEAR_ADDRUPPER, 0xF);
    dcr_write(DCR_PLB4AHB_0_BASE + SEAR_ADDRLOWER, 0xF);
    dcr_write(DCR_PLB4AHB_0_BASE + SESR, 0xF);

    rumboot_printf("Checking access to existing devices in AXI\n");
    test_event(TEC_START_CHECK_CELL2);
    ioread32(IM1_BASE + 0x20);
    rumboot_printf("Checking: OK\n");

    return 0;
}


