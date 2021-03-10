#include <stdint.h> // for uintptr_t, uint32_t

typedef enum ndma_ams {ONE_DIMENSION, TWO_DIMENSION} ndma_am_t;

#define NDMA_IRQ_MASK_COMPLETE 0x1
#define NDMA_IRQ_MASK_ERROR    0x2

#define NDMA_CONTROL_DFLT  0x0
#define NDMA_CONTROL_EN    0x1
#define NDMA_CONTROL_CPL   0x2

typedef struct ndma_cfg {
  uint32_t  RD_Address      ;
  uint32_t  RD_Bias         ;
  uint16_t  RD_RowCounter   ;
  ndma_am_t RD_AddressMode  ;
  uint32_t  WR_Address      ;
  uint32_t  WR_Bias         ;
  uint16_t  WR_RowCounter   ;
  ndma_am_t WR_AddressMode  ;
  uint16_t  MainCounter     ;
  uint8_t   InterruptMask   ;
} ndma_cfg_t;


int ndma_simple_run(uintptr_t base, ndma_cfg_t * cfg);
int ndma_simple_wait_complete(uintptr_t base);
int ndma_simple_memcpy(uintptr_t base, ndma_cfg_t * cfg);
int ndma_apb_check(uintptr_t base, ndma_cfg_t * cfg);
int ndma_apb_running_one(uintptr_t base);
