
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>

#include <platform/devices.h> 

#include <platform/devices/nu_lib.h> 
#include <platform/devices/nu_test_lib.h> 

#include "platform/devices/nu_vpe_file_tags.h"

void *in_data;
void *etalon;
void *res_data;
void *op0;
void *op1;
void *op2;


void nu_vpe_decide_dma_config (
  ConfigVPE* cfg,
  CubeMetrics* in_metrics,
  void*in_data,
  void*op0,
  void*op1,
  void*op2,
  CubeMetrics*res_metrics,
  void*res_data, 
  ConfigDMAVPE* cfg_dma
) {
  cfg->op0_rdma_config.dma_data_mode = cfg->op0_config.mux_mode; // Init Them
  cfg->op1_rdma_config.dma_data_mode = cfg->op1_config.mux_mode;
  cfg->op2_rdma_config.dma_data_mode = cfg->op2_config.mux_mode;
  cfg->wdma_config.dma_data_mode      = 1; // Copypaste From src_rdma_config in nu_vpe_decide_dma_config_trivial
  cfg->wdma_config.dma_data_use=DmaDUse_Off;
  
  nu_vpe_decide_dma_config_trivial(cfg,in_metrics,cfg_dma);
  cfg_dma->dma_dst_en = Enable_NotEn; // Волюнтари3м
  
  cfg->src_rdma_config.dma_baddr = (uint32_t) in_data;
  cfg->op0_rdma_config.dma_baddr = (uint32_t) op0;
  cfg->op1_rdma_config.dma_baddr = (uint32_t) op1;
  cfg->op2_rdma_config.dma_baddr = (uint32_t) op2;
  cfg->wdma_config.dma_baddr = (uint32_t) res_data;
  
  cfg->src_flying = Enable_En;
  cfg->dst_flying = Enable_En; // Everything Through Main Channel
}

ConfigVPE cfg;
ConfigDMAVPE cfg_dma;

CubeMetrics* in_metrics;
CubeMetrics* res_metrics;

int main() {
  int heap_id;
  int i;
  int iterations;
  
  rumboot_printf("Hello\n");
  
  heap_id = nu_get_heap_id();
  
  rumboot_platform_request_file("num_iterations_file_tag",(uintptr_t) &iterations);
  rumboot_printf("Number of iterations %d\n",iterations);
  
  for(i=0;i<iterations;i++) {
    rumboot_printf("Starting iteration %d\n",i);
    
    if(nu_vpe_load_cfg_by_tag(heap_id, &cfg, cfg_file_tag[i]) != 0) return -1;
    
    in_metrics = nu_load_cube_metrics(heap_id,metrics_in_tag[i]);
    if(in_metrics == NULL) return -1;
    
    res_metrics= nu_load_cube_metrics(heap_id,metrics_etalon_tag[i]);
    if(res_metrics == NULL) return -1;
    
    in_data = nu_load_cube(heap_id,in_file_tag[i],in_metrics);
    if(in_data == NULL) return -1;
    
    res_data = nu_vpe_malloc_res(heap_id, res_metrics);
    if(res_data == NULL) return -1;
    
      // Load OP0-OP2 Operands If Needed
    if(cfg.op0_en==Enable_En) {
      op0 = nu_vpe_load_op01_by_tags(heap_id,&cfg.op0_config,metrics_op0_cube_tag[i],metrics_op0_vec_tag[i],op0_cube_file_tag[i],op0_vec_file_tag[i]);
    }
    else op0 = NULL;
    if(cfg.op1_en==Enable_En) {
      op1 = nu_vpe_load_op01_by_tags(heap_id,&cfg.op1_config,metrics_op1_cube_tag[i],metrics_op1_vec_tag[i],op1_cube_file_tag[i],op1_vec_file_tag[i]);
    }
    else op1 = NULL;
    if(cfg.op2_en==Enable_En) {
      op2 = nu_vpe_load_op2_by_tags(heap_id,&cfg.op2_config,metrics_op2_cube_tag[i],metrics_op2_vec_tag[i],op2_cube_file_tag[i],op2_vec_file_tag[i]);
    }
    else op2 = NULL;
    
    etalon = nu_load_cube(heap_id,etalon_file_tag[i],res_metrics);
    if(etalon == NULL) return -1;
    
    //print_in_data(in_data,in_size);
    
    nu_vpe_print_config(&cfg);
    nu_vpe_decide_dma_config(&cfg,in_metrics,in_data,op0,op1,op2,res_metrics,res_data,&cfg_dma);
    nu_vpe_print_config_dma(&cfg_dma);
    nu_print_config_dma(&cfg.src_rdma_config,"src_rdma_config");
    nu_print_config_dma(&cfg.op0_rdma_config,"op0_rdma_config");
    nu_print_config_dma(&cfg.op1_rdma_config,"op1_rdma_config");
    nu_print_config_dma(&cfg.op2_rdma_config,"op2_rdma_config");
    nu_print_config_dma(&cfg.wdma_config,"wdma_config");
    
    nu_vpe_setup(NU_VPE_STANDALONE_BASE, &cfg, &cfg_dma);
    
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
    nu_vpe_run(NU_VPE_STANDALONE_BASE, &cfg);     // To Invoke Or Not To Invoke Internal DMA Channel - Decide inside nu_vpe_run
    
      // Finalize Required DMA Channels
    if(cfg_dma.dma_src_en == Enable_NotEn)
      nu_vpe_wait_rd_main_channel_complete(NU_CPDMAC_ASM_BASE);
    if(cfg_dma.dma_dst_en == Enable_NotEn)
      nu_vpe_wait_wr_main_channel_complete(NU_CPDMAC_ASM_BASE);
    nu_vpe_wait(NU_VPE_STANDALONE_BASE, &cfg);
    
    
    rumboot_printf("Comparing..\n");
    
    if(nu_bitwise_compare(res_data,etalon,res_metrics->s) == 0)
      rumboot_printf("Iteration %d PASSED\n",i);
    else {
      rumboot_printf("Test FAILED at iteration %d\n",i);
      return 1;
    }
  }
  
  return 0;
  
}
