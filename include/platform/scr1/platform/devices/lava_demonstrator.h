#ifndef LAVA_DEMONSTRATOR_H
#define LAVA_DEMONSTRATOR_H

#include <stdint.h>

int load_demonstrator_data    (int heap_id, void* addr_src, uint32_t data_size);
int load_demonstrator_weights (int heap_id, void* addr_src, uint32_t data_size);

#endif // LAVA_DEMONSTRATOR_H

