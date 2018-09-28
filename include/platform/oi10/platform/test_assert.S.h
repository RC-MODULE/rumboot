#ifndef TEST_ASSERT_S_H_
#define TEST_ASSERT_S_H_

#include <platform/trace.S.h>

.macro test_assert suffix, crfield, file, text, tmp_reg=r5, tmp_reg_msr=r6, crAccErr=cr6 errLabel=test_error
    crand \crAccErr*4+\suffix, \crAccErr*4+\suffix, \crfield*4+\suffix
    b\suffix+ \crfield, 5f
    rumboot_putstring "SOFT ASSERTION FAILED: \"\suffix\"\n\file: \"\text\"\n", \tmp_reg, \tmp_reg_msr
#ifndef RUMBOOT_ASSERT_WARN_ONLY
    b   \errLabel
#endif
5: //take care of label number with nested macros! 1, 2, 3, 4 labels already declared in rumboot_putstring
.endm

#define _TEST_ASSERT(condition, crfield, text, tmp_reg, tmp_reg_msr) \
    test_assert condition, crfield, __FILENAME__, text, tmp_reg, tmp_reg_msr

#define TEST_ASSERT(condition, crfield, text) \
    test_assert condition, crfield, __FILENAME__, text

#endif /* TEST_ASSERT_S_H_ */
