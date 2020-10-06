// Test: test on vector operations in demonstrator
// input: <input_data>.bin, <input_coef>.bin
// test do: 
//   1) run demonstrator; 
//   2) calculate etalon; 
//   3) compare etalon with demonstrator's result
// const data length = 16 * 8 bytes


#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>

#include <platform/devices.h> // memory map scr1
#include <regs/regs_demonstrator.h> // regs names demonstrator
#include <regs/regs_mdma.h> // regs names demonstrator

#include <devices/mdma_simple.h> // lib for work with mdma
#include <platform/devices/lava_demonstrator.h> // lib for work with demonstrator

#include <assert.h>
#include <stdlib.h>

// #define DEBUG
#ifdef DEBUG
  #define dbg(x, ...) rumboot_printf("test_dem: " x, ##__VA_ARGS__) 
#else
  #define dbg(x, ...)
#endif


typedef struct Vec_data {
  int32_t D;
  int32_t Zero;
} Vec_data;

typedef struct Vec_coef {
  int32_t C1;
  int16_t C2;
  int16_t Zero;
} Vec_coef;

typedef struct Vec_result {
  int32_t R0;
  int32_t Zero;
} Vec_result;


Vec_result rm_vec_op(Vec_data *data, Vec_coef *coefficient, Vec_op op, Relu_op relu_on) {
  // do operation
  int64_t D = data->D;
  int64_t C1 = coefficient->C1;
  int64_t C2 = coefficient->C2;
  int64_t result_data = (op == PLUS)
                      ? (D + C1)
                      : ((op == MIN)
                        ? ((D > C1) 
                          ? C1
                          : D)
                        : ((D < C1) 
                          ? C1
                          : D));

  result_data *= C2;

  Vec_result result;
  result.Zero = 0x0;
  dbg("data->D=%x, coefficient->C1=%x, coefficient->C2=%x\n", data->D, coefficient->C1, coefficient->C2);

  // normalization
  if(result_data > 0) {
      dbg("result_data>0\n");
    dbg("data->D=%x, coefficient->C1=%x, (result_data >> 40)=%x, (result_data >> 9)=%x\n", data->D, coefficient->C1, (int32_t)(result_data >> 40), (int32_t)(result_data >> 9));
    result.R0 = ((result_data >> 40) > 0) 
              ? INT32_MAX 
              : (result_data >> 9);
  } else {
    if(result_data < 0) {
      dbg("result_data<0\n");
      result.R0 = ((((result_data << 1) >> 41) & INT8_MIN) != INT8_MIN) 
                ? (0x1 << 31) 
                : (0x1 << 31) | ((result_data << 9) >> 18);
    } else {
      dbg("result_data=0\n");
      result.R0 = 0x0;
    }
  }

  // relu
  if((relu_on == RELU_ON) && (result.R0 < 0)) {
    result.R0 = 0x0;
  }

  return result;
}
 


int main() {
  rumboot_printf("TEST test_on_vec_op start\n");

  uint32_t count_vectors = 16;
  uint32_t data_size = count_vectors * sizeof(Vec_data);
  uint32_t coefficient_size = count_vectors * sizeof(Vec_coef);
  uint32_t result_size = count_vectors * sizeof(Vec_result);
  int i;

  // load coefficients in Coefficients mem 
  Vec_coef *coefficients = rumboot_malloc_from_heap_aligned(1, coefficient_size, 16);
  assert (coefficients != NULL);
  rumboot_platform_request_file("input_coef", (uint64_t)coefficients);
  assert (!demonstrator_load_coefficients(1, coefficients, coefficient_size));

  // load data in Data mem
  Vec_data *data = rumboot_malloc_from_heap_aligned(1, data_size, 16);
  assert (data != NULL);
  rumboot_platform_request_file("input_data", (uint32_t)data);
  assert (!load_demonstrator_data(1, data, data_size));

  // Start demonstrator
  srand(0);
  Vec_op current_op = rand() % 3;
  Relu_op current_relu = rand() % 2;
  dbg("current_op=%d, current_relu=%d", current_op, current_relu);
  assert (!demonstrator_run_vec_flow(DEMONSTRATOR_APB_BASE, data_size, current_op, current_relu, 0x0, 0x0));

  // Unload result
  Vec_result *result = rumboot_malloc_from_heap_aligned(1, result_size, 16);
  assert (result != NULL);
  assert (!unload_demonstrator_results(1, result, result_size));

  // Load etalon
  Vec_result *etalon = rumboot_malloc_from_heap_aligned(1, result_size, 16);
  assert (etalon != NULL);

  for(i = 0; i < count_vectors; ++i) {
    dbg("i=%d\n", i);
    etalon[i] = rm_vec_op(&data[i], &coefficients[i], current_op, current_relu);
    // Compare result and etalon
    if(result[i].R0 != etalon[i].R0) {
      rumboot_printf("NOT_EQUAL, D=%x, C1=%x, C2=%x, result[%d].R0=%x, etalon[%d].R0=%x, op=%d, relu=%d\n", data[i].D, coefficients[i].C1, coefficients[i].C2, i, result[i].R0, i, etalon[i].R0, current_op, current_relu);
      dbg("coef[i]=%x ", coefficients[i].C1);
      dbg("data[i]=%x\n", data[i].D);
      return 1;
    }
  }

  // Compare result and etalon
  // assert (memcmp(result, etalon, result_size) == 0);
  assert (compare_demonstrator_result_64bit((int64_t *)result, (int64_t *)etalon, result_size / sizeof(int64_t)));

  return 0;
}
