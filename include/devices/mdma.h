#include <stdint.h>

enum ERR_CODE {
        OK = 0,
        NOT_AVAILABLE = -1,

};

enum DESC_TYPE {
        NORMAL = 0b00,
        LONG = 0b10,
        PITCH = 0b11,
};

enum EVENT_TYPE {
    DIS_DONE = 0,
    CANCEL_DONE ,
    INT_DESC_DONE,
    BAD_DESC,
    STOP_DESC_DONE,
    DISCARD_DESC,
    RAXI_ERR,
    WAXI_ERR,
    AXI_ERR,
    IGNORE_EVENT
};

struct settings {
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

};

struct table_cfg {
        enum DESC_TYPE type;
        uint32_t desc_gap;
        uint32_t table_addr;
};

struct desc_cfg {
        struct settings* set;
        uint32_t data_addr;
        uint64_t free_run_val;//DEFINE THIS FIELD AS NULL IF YOU WORK WITH NORMAL DSCRIPTORS!
};

void mdma_init(uint32_t base);
void mdma_set_rxtable(uint32_t base, struct table_cfg* cfg);
void mdma_set_wxtable(uint32_t base, struct table_cfg* cfg);
enum ERR_CODE mdma_set_desc(uint32_t desc_addr, struct desc_cfg *cfg);
enum ERR_CODE mdma_get_desc(uint32_t desc_addr, struct desc_cfg *cfg);
bool mdma_wait_r(uint32_t base, enum EVENT_TYPE event);
bool mdma_wait_w(uint32_t base, enum EVENT_TYPE event);
void mdma_dump(uint32_t base);
