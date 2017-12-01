#include "heatshrink_encoder.h"
#include "heatshrink_decoder.h"
#include "heatshrink_common.h"
#include "heatshrink_config.h"


#if HEATSHRINK_DYNAMIC_ALLOC
#error HEATSHRINK_DYNAMIC_ALLOC must be false for static allocation test suite.
#endif

uint32_t shrink(uint8_t* inbuf, const uint32_t input_size, uint8_t* compbuf, const uint32_t comp_sz);
void unshrink(uint8_t* compbuf, const uint32_t compressed_size, uint8_t* decompbuf, const uint32_t decomp_sz,
     const uint32_t etalon_sz);
bool check_buf_with_random_seq(uint8_t* buf, uint32_t size);
void fill_buff_with_random_seq(uint8_t* buf, uint32_t size);
void dump_buf(uint8_t* buf, uint32_t size);
