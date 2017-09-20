#include <stdio.h>
#include <stdlib.h>
#include <rumboot/testsuite.h>
#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <platform/defs.h>

extern int CCAN_REQUEST(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3);

/* The actual workhorse */
bool run_can_request(uint32_t can_base_addr) {
    int test_state = 2;
    int ret = CCAN_REQUEST(can_base_addr, 0, 0, test_state);
    if (ret) { rumboot_printf("ret %08x test failed\n", ret); }
    return 0;
}

int main()
{
    unsigned sctl_base_addr = 0x01028000;
    unsigned sctl_can0 = 0x030;
    unsigned can0_base_addr = 0x0104E000;   // 0x01048000+{3'b110,12'b0}
    // unsigned sctl_can1 = 0x034;

    // set 'b111 in sctl
    (*(volatile unsigned*) (sctl_base_addr + sctl_can0)) = 0x7;

    int ret = run_can_request(can0_base_addr);  // 0xcan_base_addr
    rumboot_printf("main run_can_request end\n");

    return ret;
}

///* Declare the testsuite structure */
//TEST_SUITE_BEGIN(can_test, "CAN TEST")
//TEST_ENTRY("CAN_0", run_can_test, 0x1048000),
//TEST_SUITE_END();
//
///* Finally, call the whole test suite */
//int main()
//{
//    int ret = test_suite_run(NULL, &can_test);
//    rumboot_printf("%d tests from suite failed\n", ret);
//    return ret;
//}
