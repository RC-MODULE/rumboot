#ifndef TEST_ASSERT_S_H_
#define TEST_ASSERT_S_H_

#include <platform/trace.S.h>

.macro test_assert suffix, crfield, file, line, text, tmp_reg_nop_or_addr=r5, \
        tmp_reg_opcode_testevent=r6, tmp_reg_event_code=r7, crAccErr=cr6 errLabel=test_error
    crand \crAccErr*4+\suffix, \crAccErr*4+\suffix, \crfield*4+\suffix
    b\suffix+ \crfield, 5f
    rumboot_putstring "PROGRAM ASSERTION FAILED: \"\suffix\"\n\file:\line: \"\text\"\n", \tmp_reg_nop_or_addr, \tmp_reg_opcode_testevent, \tmp_reg_event_code
#ifndef DEBUG_CONTINUE_ON_ASSERT
    b   \errLabel
#endif
5: //take care of label number with nested macros! 1, 2, 3, 4 labels already declared in rumboot_putstring
.endm

#define _TEST_ASSERT(condition, crfield, text, tmp_reg_nop_or_addr, tmp_reg_opcode_testevent, tmp_reg_event_code) \
    test_assert condition, crfield, __FILE__, __LINE__, text, tmp_reg_nop_or_addr, tmp_reg_opcode_testevent, tmp_reg_event_code

#define TEST_ASSERT(condition, crfield, text) \
    test_assert condition, crfield, __FILE__, __LINE__, text

#endif /* TEST_ASSERT_S_H_ */
