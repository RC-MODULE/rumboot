#include <stdio.h>
#include <stdlib.h>
#include <rumboot/testsuite.h>
#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <basis/defs.h>


extern int CCAN_LOOPBACK(uint32_t base);

/* The actual workhorse */
bool run_can_test(uint32_t base)
{
    int ret = CCAN_LOOPBACK(base);
    rumboot_printf("%d tests failed\n", ret);
    return (ret == 0);
}

/* Declare the testsuite structure */
TEST_SUITE_BEGIN(can_test, "CAN TEST")
TEST_ENTRY("CAN 0", run_can_test, 0x1048000),
TEST_SUITE_END();

/* Finally, call the whole test suite */
int main()
{
    int ret = test_suite_run(NULL, &can_test);
    rumboot_printf("%d tests from suite failed\n", ret);
    return ret;
}
