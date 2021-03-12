

#ifndef NU_LIB_H
#define NU_LIB_H

#include <stdint.h>

void nu_vpe_config_rd_main_channel(uintptr_t dma_base, void *addr, int size);
void nu_vpe_run_rd_main_channel(uintptr_t dma_base);
void nu_vpe_wait_rd_main_channel_complete(uintptr_t dma_base);

void nu_vpe_config_wr_main_channel(uintptr_t dma_base, void *addr, int size);
void nu_vpe_run_wr_main_channel(uintptr_t dma_base);
void nu_vpe_wait_wr_main_channel_complete(uintptr_t dma_base);


#endif

