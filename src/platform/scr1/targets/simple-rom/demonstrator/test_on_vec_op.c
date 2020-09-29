// Test: test on vector operations in demonstrator

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
int j;
for(j = 1; j < 33; ++j){ 
  rumboot_printf("ITERATION %d\n", j);
  uint32_t count_vectors = j;
  uint32_t count_elements_in_vector = 16;
  uint32_t data_size = count_vectors * count_elements_in_vector * sizeof(Vec_data);
  uint32_t coefficient_size = count_vectors * count_elements_in_vector * sizeof(Vec_coef);
  uint32_t result_size = count_vectors * count_elements_in_vector * sizeof(Vec_result);
  int i;


  // load coefficients in Coefficients mem 
  Vec_coef *coefficients = rumboot_malloc_from_heap_aligned(1, coefficient_size, 16);
  assert (coefficients != NULL);

  // rumboot_platform_request_file("input_coef", (uint32_t)addr_ext_coef);
  srand(0);

  Vec_coef elem_coef;
  // Vec_coef elem_coef = {0x2, 0x1, 0};
  elem_coef.Zero = 0x0;
  elem_coef.C2 = 0x1;
  for(i = 0; i < count_vectors * count_elements_in_vector; ++i){
    int32_t temp;
    // do {
      temp = rand();
    // } while(temp < 0);
    elem_coef.C1 = temp;
    coefficients[i] = elem_coef;
  }

  assert (!demonstrator_load_coefficients(1, coefficients, coefficient_size));

  dbg("coefficients[0] 0=%x, C2=%x, C1=%x\n", coefficients[0].Zero, coefficients[0].C2, coefficients[0].C1);
  dbg("coefficients[1] 0=%x, C2=%x, C1=%x\n", coefficients[1].Zero, coefficients[1].C2, coefficients[1].C1);

  // load data in Data mem
  Vec_data *data = rumboot_malloc_from_heap_aligned(1, data_size, 16);
  assert (data != NULL);

  Vec_data elem_data;
  elem_data.Zero = 0x0;
  // Vec_data elem_data = {-0x200, 0x0};
  for(i = 0; i < count_vectors * count_elements_in_vector; ++i) {
    int32_t temp;
    // do {
      temp = rand();
    // } while(temp < 0);
    elem_data.D = temp;
    data[i] = elem_data;
  }

  assert (!load_demonstrator_data(1, data, data_size));

  dbg("data[0] 0=%x, D=%x\n", data[0].Zero, data[0].D);
  dbg("data[1] 0=%x, D=%x\n", data[1].Zero, data[1].D);

  // Start demonstrator
  Vec_op current_op = rand() % 3;
  Relu_op current_relu = rand() % 2;
  rumboot_printf("current_op=%d, current_relu=%d", current_op, current_relu);
  assert (!demonstrator_run_vec_flow(DEMONSTRATOR_APB_BASE, data_size, current_op, current_relu, 0x0, 0x0));

  // Unload result
  Vec_result *result = rumboot_malloc_from_heap_aligned(1, result_size, 16);
  assert (result != NULL);

  assert (!unload_demonstrator_results(1, result, result_size));
  dbg("result[0] 0=%x, R0=%x\n", result[0].Zero, result[0].R0);
  dbg("result[1] 0=%x, R0=%x\n", result[1].Zero, result[1].R0);

  // Load etalon
  Vec_result *etalon = rumboot_malloc_from_heap_aligned(1, result_size, 16);
  assert (etalon != NULL);

  for(i = 0; i < count_vectors * count_elements_in_vector; ++i) {
    dbg("i=%d\n", i);
    etalon[i] = rm_vec_op(&data[i], &coefficients[i], current_op, current_relu);
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

  rumboot_printf("ITERATION %d passed\n", j);

}

  return 0;
}
