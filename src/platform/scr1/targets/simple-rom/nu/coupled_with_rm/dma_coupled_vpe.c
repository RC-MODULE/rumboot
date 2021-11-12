
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>

#include <platform/devices.h> 

#include <devices/rcm_na/nu_lib.h> 
#include <devices/rcm_na/nu_test_lib.h> 

int32_t *in_data;
int16_t *etalon;
int16_t *res_data;
void *op0;
void *op1;
void *op2;

void print_in_data(int32_t * in_data, int in_size){
  int lines;
  int ptr;
  
  lines = in_size / 16;
  ptr=0;
  
  for(int i=0;i<lines;i++) {
    rumboot_printf("%x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x \n",
                   in_data[ptr+0],in_data[ptr+1],in_data[ptr+2],in_data[ptr+3],
                   in_data[ptr+4],in_data[ptr+5],in_data[ptr+6],in_data[ptr+7],
                   in_data[ptr+8],in_data[ptr+9],in_data[ptr+10],in_data[ptr+11],
                   in_data[ptr+12],in_data[ptr+13],in_data[ptr+14],in_data[ptr+15]);
    ptr = ptr + 16;
  }
}

void nu_vpe_decide_dma_config (ConfigVPE* cfg, CubeMetrics* metrics, ConfigDMAVPE* cfg_dma) {
  nu_vpe_decide_dma_config_trivial(cfg,metrics);
  cfg_dma->H = metrics->H;
  cfg_dma->W = metrics->W;
  cfg_dma->C = metrics->C;
  cfg_dma->dma_dst_en = Enable_NotEn; // Волюнтари3м
}



ConfigVPE cfg;
ConfigDMAVPE cfg_dma;

CubeMetrics* in_metrics;
CubeMetrics* res_metrics;

int main() {
  int heap_id;
  
  rumboot_printf("Hello\n");
  
  heap_id = nu_get_heap_id();
  
  if(nu_vpe_load_cfg(heap_id, &cfg) != 0) return -1;
  
  in_metrics = nu_vpe_load_in_metrics(heap_id);
  if(in_metrics == NULL) return -1;
  
  res_metrics= nu_vpe_load_res_metrics(heap_id);
  if(res_metrics == NULL) return -1;
  
  // load source data cubes to memory
  in_data = nu_vpe_load_in_data(heap_id,in_metrics);
  if(in_data == NULL) return -1;
  
  // reserve space for result data cubes in memory
  res_data = nu_vpe_malloc_res(heap_id, res_metrics);
  if(res_data == NULL) return -1;
  
  // Load OP0-OP2 Operands to memory If Needed
  if(cfg.op0_en==Enable_En) {
    op0 = nu_vpe_load_op01(heap_id,&cfg.op0_config,0);
  }
  if(cfg.op1_en==Enable_En) {
    op1 = nu_vpe_load_op01(heap_id,&cfg.op1_config,1);
  }
  if(cfg.op2_en==Enable_En) {
    op2 = nu_vpe_load_op2(heap_id,&cfg.op2_config);
  }
  
  // load ethalon result data to memory
  etalon = nu_vpe_load_etalon(heap_id,res_metrics);
  if(etalon == NULL) return -1;
  
  // print_in_data(in_data,in_size);
  
  // define addidtional parameters in VPE config
  cfg.src_rdma_config.dma_baddr = (uint32_t)in_data  ; 
  cfg.op0_rdma_config.dma_baddr = (uint32_t)op0      ; 
  cfg.op1_rdma_config.dma_baddr = (uint32_t)op1      ; 
  cfg.op2_rdma_config.dma_baddr = (uint32_t)op2      ; 
  cfg.wdma_config.dma_baddr     = (uint32_t)res_data ;
  
  
  nu_vpe_print_config(&cfg);
  nu_vpe_decide_dma_config(&cfg,in_metrics,&cfg_dma);
  nu_vpe_print_config_dma(&cfg_dma);
  
  nu_vpe_setup(NU_VPE_STANDALONE_BASE, &cfg);
  
    // Setup Main Channel DMAs if Required
  if(cfg_dma.dma_src_en == Enable_NotEn)
    nu_vpe_config_rd_main_channel(NU_CPDMAC_ASM_BASE,in_data,in_metrics->s);
  if(cfg_dma.dma_dst_en == Enable_NotEn)
    nu_vpe_config_wr_main_channel(NU_CPDMAC_ASM_BASE,res_data,res_metrics->s);
  
  rumboot_printf("Running DMA..\n");
  
    // Invoke Required DMA Channels
  if(cfg_dma.dma_src_en == Enable_NotEn)
    nu_vpe_run_rd_main_channel(NU_CPDMAC_ASM_BASE);
  if(cfg_dma.dma_dst_en == Enable_NotEn)
    nu_vpe_run_wr_main_channel(NU_CPDMAC_ASM_BASE);

  // Wait VPE context apply enable
  nu_vpe_run(NU_VPE_STANDALONE_BASE, &cfg);     // To Invoke Or Not To Invoke Internal DMA Channel - Decide inside nu_vpe_run
  
    // Finalize Required DMA Channels
  if(cfg_dma.dma_src_en == Enable_NotEn)
    nu_vpe_wait_rd_main_channel_complete(NU_CPDMAC_ASM_BASE);
  if(cfg_dma.dma_dst_en == Enable_NotEn)
    nu_vpe_wait_wr_main_channel_complete(NU_CPDMAC_ASM_BASE);
  
  nu_vpe_wait(NU_VPE_STANDALONE_BASE, &cfg); // Wait complite VPE WDMA
  

  
  rumboot_platform_dump_region("res_data.bin",(uint32_t)res_data,res_metrics->s);
  
  rumboot_printf("Comparing..\n");
  
  if(nu_bitwise_compare(res_data,etalon,res_metrics->s) == 0)
    rumboot_printf("Test PASSED\n");
  else {
    rumboot_printf("Test FAILED\n");
    return 1;
  }
  
  
  
  
  
  
  
  return 0;
  
}
