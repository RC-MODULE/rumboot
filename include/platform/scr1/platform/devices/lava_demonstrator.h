// This .h is ok!
#ifndef LAVA_DEMONSTRATOR_H
#define LAVA_DEMONSTRATOR_H

#include <stdint.h>

int load_demonstrator_data    (int heap_id, void *addr_src,  uint32_t data_size);
int load_demonstrator_weights (int heap_id, void *addr_src,  uint32_t data_size);
int load_demonstrator_result  (int heap_id, void *addr_src,  uint32_t data_size);
int run_matrix();
int demonstrator_compare_data (uintptr_t src_etalon[], uintptr_t addr_ext_r[],  uint32_t len);
int load_array ( uint32_t dump_size, uintptr_t dump_ptr[], uint32_t data );
int load_array_w ( uint32_t dump_size, uintptr_t dump_ptr[] );
int load_etalon_array ( uint32_t dump_size, uintptr_t dump_ptr[], uint32_t data ); 

int load_demonstrator_coefficients (int heap_id, void *addr_src, uint32_t data_size);
int unload_demonstrator_results (int heap_id, void *addr_dst, uint32_t data_size);

#endif // LAVA_DEMONSTRATOR_H

