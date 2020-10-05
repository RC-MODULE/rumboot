// This .h is ok!
#ifndef LAVA_DEMONSTRATOR_H
#define LAVA_DEMONSTRATOR_H

#include <stdint.h>

int load_demonstrator_data    (int heap_id, void *addr_src,  uint32_t data_size);
int load_demonstrator_weights (int heap_id, void *addr_src,  uint32_t data_size);
int load_demonstrator_result  (int heap_id, void *addr_src,  uint32_t data_size);
int demonstrator_run_matrix_flow(uintptr_t matrix_base, uint32_t data_size, uint32_t weight_size);
int get_demonstrator_matrix_last_duration(uintptr_t matrix_base);
int read_demonstrator_matrix_etalon(void * etalon, uint32_t etalon_size);
bool compare_demonstrator_result_64bit(int64_t* res, int64_t* etalon, uint32_t size);

int demonstrator_load_coefficients (int heap_id, void *addr_src, uint32_t data_size);
int unload_demonstrator_results (int heap_id, void *addr_dst, uint32_t data_size);

typedef enum Vec_op {PLUS, MIN, MAX} Vec_op;
typedef enum Relu_op {RELU_OFF, RELU_ON} Relu_op;

int demonstrator_run_vec_flow(
  uintptr_t matrix_base, 
  uint32_t data_size, 
  Vec_op op,
  Relu_op relu_on,
  uint32_t data_offset,
  uint32_t result_offset
);

typedef enum Pool_op {POOL_AVE, POOL_MAX} Pool_op;

int demonstrator_run_pooling_flow(uintptr_t matrix_base, uint32_t data_size, Pool_op pooling_mode);

#endif // LAVA_DEMONSTRATOR_H

