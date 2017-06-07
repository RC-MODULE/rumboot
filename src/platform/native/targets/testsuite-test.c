#include <stdio.h>
#include <stdlib.h>
#include <rumboot/testsuite.h>


bool do_shit(uint32_t arg)
{
    return (rand() % 2) ? true : false;
}

TEST_SUITE_BEGIN(shit, "dummies")
TEST_ENTRY("shit-o-test-1", do_shit, 0),
TEST_ENTRY("shit-o-test-2", do_shit, 0),
TEST_ENTRY("shit-o-test-3", do_shit, 0),
TEST_ENTRY("shit-o-test-4", do_shit, 0),
TEST_ENTRY("shit-o-test-5", do_shit, 0),
TEST_ENTRY("shit-o-test-6", do_shit, 0),
TEST_ENTRY("shit-o-test-7", do_shit, 0),
TEST_ENTRY("shit-o-test-8", do_shit, 0),
TEST_ENTRY("shit-o-test-9", do_shit, 0),
TEST_ENTRY("shit-o-test-10", do_shit, 0),
TEST_ENTRY("shit-o-test-11", do_shit, 0),
TEST_ENTRY("shit-o-test-12", do_shit, 0),
TEST_ENTRY("shit-o-test-13", do_shit, 0),
TEST_ENTRY("shit-o-test-14", do_shit, 0),
TEST_ENTRY("shit-o-test-15", do_shit, 0),
TEST_ENTRY("shit-o-test-16", do_shit, 0),
TEST_ENTRY("shit-o-test-17", do_shit, 0),
TEST_ENTRY("shit-o-test-18", do_shit, 0),
TEST_ENTRY("shit-o-test-19", do_shit, 0),
TEST_ENTRY("shit-o-test-20", do_shit, 0),
TEST_ENTRY("shit-o-test-21", do_shit, 0),
TEST_ENTRY("shit-o-test-22", do_shit, 0),
TEST_ENTRY("shit-o-test-23", do_shit, 0),
TEST_ENTRY("shit-o-test-24", do_shit, 0),
TEST_ENTRY("shit-o-test-25", do_shit, 0),
TEST_SUITE_END();

char logstruct[8192];

int main()
{
    test_suite_run((void *) logstruct, &shit);
    return 0;
}
