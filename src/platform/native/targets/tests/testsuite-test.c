#include <stdio.h>
#include <stdlib.h>
#include <rumboot/testsuite.h>
#include <rumboot/printf.h>

static bool mytest(uint32_t arg)
{
    /* Return true if test passed, false otherwise */
    return (rand() % 2) ? true : false;
}

static bool myskip_func(uint32_t arg)
{
    /* return true to skip test */
    return true;
}

TEST_SUITE_BEGIN(shit, "dummies")
TEST_ENTRY("shit-o-test-1", mytest, 0),
TEST_ENTRY_COND("shit-o-test-cond", mytest, 0, myskip_func),
TEST_ENTRY("shit-o-test-2", mytest, 0),
TEST_ENTRY("shit-o-test-3", mytest, 0),
TEST_ENTRY("shit-o-test-4", mytest, 0),
TEST_ENTRY("shit-o-test-5", mytest, 0),
TEST_ENTRY("shit-o-test-6", mytest, 0),
TEST_SUITE_END();

char logstruct[8192];

int main()
{
    int ret = test_suite_run((void *) logstruct, &shit);
    rumboot_printf("%d tests failed\n", ret);

    /* Always return 0 here */
    return 0;
}
