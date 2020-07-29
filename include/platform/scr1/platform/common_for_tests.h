#include <stdint.h> // for uint32_t
// #include <assert.h>

// #define assert(condition) \
//   if(!(condition)) { \
//     rumboot_printf("Assertion '%s' failed.\n", #condition); \
//     return 1; \
//   }


static inline uint32_t bit_in_reg(uint32_t reg, int bit) {
  return (reg & (1 << bit)) >> bit;
}


static inline uint32_t bits_in_reg(uint32_t reg, int msb, int lsb) {
  uint32_t mask = 1 << lsb;

  for(int k = 1; k <= msb - lsb; ++k) {
    mask = mask | (mask << k);
  }

  return (reg & mask) >> lsb;
}

// int randomize(int min, int max);
// int summa(int count, ...);
