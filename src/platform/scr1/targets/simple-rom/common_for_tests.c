// #include <rumboot/printf.h>

#include <platform/common_for_tests.h>

#include <stdlib.h> // for rand()
#include <time.h> // for srand()
#include <stdint.h> // for uint32_t
#include <stdarg.h>

// #include <assert.h>


// uint32_t bit_in_reg(uint32_t reg, int bit) {
//   // rumboot_printf("bit_in_reg\n");

//   return (reg & (1 << bit)) >> bit;
// }


// uint32_t bits_in_reg(uint32_t reg, int msb, int lsb) {
//   uint32_t mask = 1 << lsb;

//   for(int k = 1; k <= msb - lsb; ++k) {
//     mask = mask | (mask << k);
//   }

//   return (reg & mask) >> lsb;
// }


// int randomize(int min, int max) { // random in [min, max]
//   srand(time(NULL) + rand());
  
//   int temp = max - min;
//   temp = temp ? temp : 1;
//   ++temp;

//   return rand() % temp + min;
// }


// int summa(int count, ...) { // это работает
//   int res = 0;
//   va_list args;

//   va_start(args, count);
//   for(int i = 0; i < count; ++i) {
//     // rumboot_printf("i=%d", i);
//     rumboot_printf("i=%d, xi=%d", i, va_arg(args, int));
//     // if(va_arg(args, int) == NULL)  // так писать плохо, знаение не определено, если аргументы законились
//     //   rumboot_printf("NULL\n");
//     // else
//     //   rumboot_printf("not NULL\n");
//   }
//   va_end(args);

//   return res;
// }


// uint32_t bits(uint32_t number, int count_bits, uint32_t left_border, ...) {
//   rumboot_printf("number=%x, count_bits=%d", number, count_bits);
//   uint32_t *p = &left_border;
//   uint32_t left = *p;
//   rumboot_printf("left=%d", left);
//   // ++p;
//   // uint32_t t = p;
//   // rumboot_printf("left=%d", *t);


//   if(count_bits == 1) {
//     return (number & (1 << left)) >> left;
//   } else {
//     rumboot_printf("point1");
//     int right;
//     // p++;
//     rumboot_printf("point2");
//     right = *(++p);
//     // right = *p;
//     rumboot_printf("point3");
//     rumboot_printf("right=%d", right);

//     uint32_t mask = 1 << right;
//     for(int k = 1; k <= left - right; ++k) {
//       mask = mask | (mask << k);
//     }

//     return (number & mask) >> right;
//   }
//   // return 0x10;
// }
