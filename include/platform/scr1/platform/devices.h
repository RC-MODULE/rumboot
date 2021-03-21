#ifndef DEVICES_H
#define DEVICES_H

#define DATA_BASE               0xFFF00000
#define COEFFICIENT_BASE        0xFFF40000
#define WEIGHT_BASE             0xFFF80000
#define RESULT_BASE             0xFFFC0000
#define EXT_MEMORY_BASE         0x01000000

#define SCR1_TIMER_BASE         0x00490000
#define DUT_BASE                0x00800000
#define DUT_PORT1_BASE          (DUT_BASE + 0x00200000)
#define EXT_MEMORY_BASE         0x01000000

#define SPI_CONTROL_BASE        DUT_BASE
#define SPI_FIFO_BASE           DUT_PORT1_BASE

#define MDMA_BASE               DUT_BASE

#define DEMONSTRATOR_MDMA_BASE  DUT_BASE
#define DEMONSTRATOR_APB_BASE   DUT_PORT1_BASE

#define NU_CPDMAC_ASM_BASE      DUT_PORT1_BASE
#define NU_VPE_STANDALONE_BASE  DUT_BASE
#define NU_MPE_STANDALONE_BASE  DUT_BASE
#define NU_PPE_STANDALONE_BASE  DUT_BASE

#endif /* end of include guard: DEVICES_H */