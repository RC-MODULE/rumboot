#ifndef RCM_CP
#define RCM_CP
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>

struct rcm_cp_instance {
    uintptr_t base;
    bool      swap_endian;
    bool      two_dimentional;
    void      *txbuf;
    void      *rxbuf;
    void      *rxtargetbuf;
    size_t    buflen;
};

enum cp_status {
    CP_IDLE = 0, 
    CP_WORKING = 1,
};


void cp_instance_init(struct rcm_cp_instance *inst, uintptr_t base);
void cp_attach_buffers(struct rcm_cp_instance *inst, void *rxbuf, void *txbuf, size_t buflen);
void cp_detach_buffers(struct rcm_cp_instance *inst);
void cp_attach_buffers_from_heap(struct rcm_cp_instance *inst, const char *heapname, size_t buflen);
void cp_start_tx(struct rcm_cp_instance *inst, const void *buf, size_t len);
enum cp_status cp_tx_status(struct rcm_cp_instance *inst);
void cp_start_rx(struct rcm_cp_instance *inst, void *buf, size_t len);
enum cp_status cp_rx_status(struct rcm_cp_instance *inst);
int cp_wait(struct rcm_cp_instance *inst, bool rx, bool tx, uint32_t timeout_us);

#endif // !RCM_CP