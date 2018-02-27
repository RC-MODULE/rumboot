#ifndef BISR
#define BISR

#define BISR_TIMEOUT 50000
#define NUM_ARRAYS 4

#define BISR_TEST_TIMEOUT 0
#define BISR_MEM_PERFECT  1
#define BISR_MEM_GOOD     2
#define BISR_MEM_FAIL     3

#define MAIN_MEM_CLEAR    1
#define MAIN_MEM_REP      2
#define MAIN_MEM_NONREP   3

#define BISR_STATUS_ARR0            0x00
#define BISR_REDUNDANCY_BUS_ARR0    0x04
#define BISR_ERROR_VECTOR_LOW_ARR0  0x08
#define BISR_ERROR_VECTOR_HIGH_ARR0 0x0c

#define FCA1  0
#define CRE1  5
#define FCA2  6
#define CRE2 11


#include <stdbool.h>

int bisr_program_test();
int bisr_hard_test();

#endif
