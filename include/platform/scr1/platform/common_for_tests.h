#include <stdint.h> // for uint32_t

#define assert(condition) \
  if(!(condition)) { \
    rumboot_printf("Assertion '%s' failed.\n", #condition); \
    return 1; \
  }


uint32_t bit_in_reg(uint32_t reg, int bit);
uint32_t bits_in_reg(uint32_t reg, int msb, int lsb);
int randomize(int min, int max);

int summa(int count, ...);
