

#include <stdint.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>

#include <platform/devices/nu_cpdmac_lib.h> 


void nu_vpe_config_rd_main_channel(uintptr_t dma_base, void *addr, int size) {
  nu_cpdmac_trn512_config(dma_base,addr,size);
}
void nu_vpe_run_rd_main_channel(uintptr_t dma_base) {
  nu_cpdmac_trn512_run(dma_base);
}
void nu_vpe_wait_rd_main_channel_complete(uintptr_t dma_base){
  nu_cpdmac_trn512_wait_complete(dma_base);
}

void nu_vpe_config_wr_main_channel(uintptr_t dma_base, void *addr, int size){
  nu_cpdmac_rcv256_config(dma_base,addr,size);
}

void nu_vpe_run_wr_main_channel(uintptr_t dma_base) {
  nu_cpdmac_rcv256_run(dma_base);
}
void nu_vpe_wait_wr_main_channel_complete(uintptr_t dma_base) {
  nu_cpdmac_rcv256_wait_complete(dma_base);
}


