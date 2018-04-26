#ifndef TEST_ASSERT_S_H_
#define TEST_ASSERT_S_H_

#include <platform/trace.S.h>

.macro test_assert suffix, crfield, file, line, text
    b\suffix+ \crfield, 4f
    rumboot_putstring "PROGRAM ASSERTION FAILED: \"\suffix\"\n\file:\line: \"\text\"\n"
    test_event EVENT_ASSERT
5: //take care of label number with nested macros! 1, 2, 3, 4 labels already decalred in rumboot_putstring
.endm

#define TEST_ASSERT(condition, crfield, text) \
    test_assert condition, crfield, __FILE__, __LINE__, text

#endif /* TEST_ASSERT_S_H_ */
