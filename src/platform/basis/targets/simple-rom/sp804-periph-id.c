#include <stdio.h>
#include <stdlib.h>
#include <rumboot/testsuite.h>
#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <basis/defs.h>


/* The actual workhorse */
bool test_dit_periph_id_regs(uint32_t base)
{
    uint32_t tmp = ioread32(base + 0xFE0) & 0xf;
    if (tmp != 0x4) {
        rumboot_printf("FAIL: expected: %x got %x\n", 0x4, tmp);
        return false;
    }
    return true;
}

/* Declare the testsuite structure */
TEST_SUITE_BEGIN(dit_reg_test, "SP804 ID Regs")
TEST_ENTRY("SP804_0", test_dit_periph_id_regs, DIT0_BASE),
TEST_ENTRY("SP804_1", test_dit_periph_id_regs, DIT1_BASE),
TEST_ENTRY("SP804_2", test_dit_periph_id_regs, DIT2_BASE),
TEST_ENTRY("SP804_3", test_dit_periph_id_regs, DIT3_BASE),
TEST_SUITE_END();

/* Finally, call the whole test suite */
int main()
{
    int ret = test_suite_run(NULL, &dit_reg_test);
    rumboot_printf("%d tests from suite failed\n", ret);
    return ret;
}
