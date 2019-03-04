#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/rumboot.h>
#include <platform/test_event_codes.h>
#include <platform/devices.h>
#include <platform/interrupts.h>
#include <platform/test_assert.h>
#include <platform/regs/regs_plb4ahb.h>

#ifndef NON_EXISTENT_AXI_ADDR
#define NON_EXISTENT_AXI_ADDR   0xC0025000
#endif

#ifndef AXI_HEAP
#define AXI_HEAP                "IM1"
#endif

#define TEST_VALUE              0xBABADEDA

typedef enum test_event_code {
    TEC_START_CHECK_HRESP = TEST_EVENT_CODE_MIN,
    TEC_START_CHECK_CELL1,
    TEC_START_CHECK_CELL2
} test_event_code;

int main() {
    test_event_send_test_id( "test_oi10_sys_005" );

    void * const test_addr = rumboot_malloc_from_named_heap( AXI_HEAP, 4 );
    uint32_t const test_axi_addr = rumboot_virt_to_dma( test_addr );

    rumboot_printf( "Checking access to existing devices in AXI\n" );
    uint32_t const event_start_check_cell1 [] = { TEC_START_CHECK_CELL1, test_axi_addr };
    rumboot_platform_event_raise( EVENT_TESTEVENT, event_start_check_cell1, ARRAY_SIZE(event_start_check_cell1) );
    iowrite32( TEST_VALUE, (uint32_t)test_addr );
    rumboot_printf( "Checking: OK\n" );

    rumboot_printf( "Checking access to non-existent devices in AXI\n" );
    uint32_t const event_start_check_hresp [] = { TEC_START_CHECK_HRESP, rumboot_virt_to_dma( (void*)NON_EXISTENT_AXI_ADDR ) };
    rumboot_platform_event_raise( EVENT_TESTEVENT, event_start_check_hresp, ARRAY_SIZE(event_start_check_hresp) );
    iowrite32( TEST_VALUE, NON_EXISTENT_AXI_ADDR );
    rumboot_printf( "Checking: OK\n" );

    rumboot_printf( "Cleaning of registers SEAR, SESR\n" );
    dcr_write( DCR_PLB4AHB_0_BASE + SEAR_ADDRUPPER, 0xF );
    dcr_write( DCR_PLB4AHB_0_BASE + SEAR_ADDRLOWER, 0xF );
    dcr_write( DCR_PLB4AHB_0_BASE + SESR, 0xF );

    rumboot_printf( "Checking access to existing devices in AXI\n" );
    test_event( TEC_START_CHECK_CELL2 );
    uint32_t const event_start_check_cell2 [] = { TEC_START_CHECK_CELL2, test_axi_addr };
    rumboot_platform_event_raise( EVENT_TESTEVENT, event_start_check_cell2, ARRAY_SIZE(event_start_check_cell2) );
    ioread32( (uint32_t)test_addr );
    rumboot_printf( "Checking: OK\n" );

    return 0;
}

