#ifndef BISR
#define BISR

#define BISR_STATUS_ARR0 0x00
#define BISR_REDUNDANCY_BUS_ARR0 0x04
#define BISR_ERROR_VECTOR_LOW_ARR0 0x08
#define BISR_ERROR_VECTOR_HIGH_ARR0 0x0c

#define FCA1 0x00
#define CRE1 0x05
#define FCA2 0x06
#define CRE2 0x0b


#include <stdbool.h>

bool bisr_program_test();
bool bisr_hard_test();

#endif
