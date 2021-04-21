#ifndef RCM_CP
#define RCM_CP
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>

struct rcm_cp_instance {
    uintptr_t base;
    bool      swap_endian;
    void      *txbuf;
    void      *rxbuf;
    void      *rxtargetbuf;
    size_t    buflen;
};

#endif // !RCM_CP