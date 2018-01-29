#include <stdint.h>

enum ERR_CODE {
  OK = 0,
  NOT_AVAILABLE = -1,

};

struct settings{
   uint32_t ownership : 1;
   uint32_t link : 1;
   uint32_t interrupt : 1;
   uint32_t stop : 1;
   uint32_t increment : 1;
   uint32_t oer : 1;
   uint32_t ber : 1;
   uint32_t per : 1;
   uint32_t fer : 1;
   uint32_t length : 14;

} ;

struct desc_cfg {

    struct settings* set;
    uint32_t data_addr;
    uint64_t free_run_val;
};


enum ERR_CODE set_desc(uint32_t desc_addr, struct desc_cfg *cfg);
enum ERR_CODE get_desc(uint32_t desc_addr, struct desc_cfg *cfg);
