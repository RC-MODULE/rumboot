#ifndef __TEST_ASSERT_H__
#define __TEST_ASSERT_H__


#include <stdlib.h>
#include <platform/trace.h>


#ifdef RUMBOOT_ASSERT_WARN_ONLY
#define __test_assert_failure_do()  exit( 1 )
#else
#define __test_assert_failure_do()  (void)0
#endif

#define TEST_ASSERT( expr, message )\
MACRO_BEGIN\
    if( !(expr) ) {\
        rumboot_putstring( "PROGRAM ASSERTION FAILED: \"" STRINGIZE(expr) "\"\n" __FILE__ ":" STRINGIZE(__LINE__) ": \"" message "\"\n" );\
        __test_assert_failure_do();\
    }\
MACRO_END


#define TEST_WAIT( condition, timeout )\
MACRO_BEGIN\
    unsigned int time = timeout;\
    while( !(condition) && time-- );\
    TEST_ASSERT( condition, "Condition timeout (" STRINGIZE(timeout) ")" );\
MACRO_END


#define TEST_WAIT_ASSERT( condition, timeout, message )\
MACRO_BEGIN\
    unsigned int time = timeout;\
    while( !(condition) && time-- );\
    TEST_ASSERT( condition, message ". Condition timeout (" STRINGIZE(timeout) ")"  );\
MACRO_END


#endif // __TEST_ASSERT_H__
