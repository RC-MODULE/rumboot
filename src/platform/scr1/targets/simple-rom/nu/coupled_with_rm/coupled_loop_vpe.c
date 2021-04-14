
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>

#include <platform/devices.h> 

#include <platform/devices/nu_lib.h> 
#include <platform/devices/nu_test_lib.h> 

void *in_data;
void *etalon;
void *res_data;
void *op0;
void *op1;
void *op2;


void nu_vpe_decide_dma_config (ConfigVPE* cfg, CubeMetrics* metrics, ConfigDMAVPE* cfg_dma) {
  nu_vpe_decide_dma_config_trivial(cfg,metrics,cfg_dma);
  cfg_dma->dma_dst_en = Enable_NotEn; // Волюнтари3м
}


char* cfg_file_tag[] = {
  "cfg_file_tag_0_",
  "cfg_file_tag_1_",
  "cfg_file_tag_2_",
  "cfg_file_tag_3_",
  "cfg_file_tag_4_",
  "cfg_file_tag_5_",
  "cfg_file_tag_6_",
  "cfg_file_tag_7_",
  "cfg_file_tag_8_",
  "cfg_file_tag_9_",
  "cfg_file_tag_10_",
  "cfg_file_tag_11_",
  "cfg_file_tag_12_",
  "cfg_file_tag_13_",
  "cfg_file_tag_14_",
  "cfg_file_tag_15_"
};
char* metrics_in_tag[] = {
  "metrics_in_tag_0_",
  "metrics_in_tag_1_",
  "metrics_in_tag_2_",
  "metrics_in_tag_3_",
  "metrics_in_tag_4_",
  "metrics_in_tag_5_",
  "metrics_in_tag_6_",
  "metrics_in_tag_7_",
  "metrics_in_tag_8_",
  "metrics_in_tag_9_",
  "metrics_in_tag_10_",
  "metrics_in_tag_11_",
  "metrics_in_tag_12_",
  "metrics_in_tag_13_",
  "metrics_in_tag_14_",
  "metrics_in_tag_15_"
};
char* metrics_etalon_tag[] = {
  "metrics_etalon_tag_0_",
  "metrics_etalon_tag_1_",
  "metrics_etalon_tag_2_",
  "metrics_etalon_tag_3_",
  "metrics_etalon_tag_4_",
  "metrics_etalon_tag_5_",
  "metrics_etalon_tag_6_",
  "metrics_etalon_tag_7_",
  "metrics_etalon_tag_8_",
  "metrics_etalon_tag_9_",
  "metrics_etalon_tag_10_",
  "metrics_etalon_tag_11_",
  "metrics_etalon_tag_12_",
  "metrics_etalon_tag_13_",
  "metrics_etalon_tag_14_",
  "metrics_etalon_tag_15_"
};
char* in_file_tag[] = {
  "in_file_tag_0_",
  "in_file_tag_1_",
  "in_file_tag_2_",
  "in_file_tag_3_",
  "in_file_tag_4_",
  "in_file_tag_5_",
  "in_file_tag_6_",
  "in_file_tag_7_",
  "in_file_tag_8_",
  "in_file_tag_9_",
  "in_file_tag_10_",
  "in_file_tag_11_",
  "in_file_tag_12_",
  "in_file_tag_13_",
  "in_file_tag_14_",
  "in_file_tag_15_"
};
char* metrics_op0_cube_tag[] = {
  "metrics_op0_cube_tag_0_",
  "metrics_op0_cube_tag_1_",
  "metrics_op0_cube_tag_2_",
  "metrics_op0_cube_tag_3_",
  "metrics_op0_cube_tag_4_",
  "metrics_op0_cube_tag_5_",
  "metrics_op0_cube_tag_6_",
  "metrics_op0_cube_tag_7_",
  "metrics_op0_cube_tag_8_",
  "metrics_op0_cube_tag_9_",
  "metrics_op0_cube_tag_10_",
  "metrics_op0_cube_tag_11_",
  "metrics_op0_cube_tag_12_",
  "metrics_op0_cube_tag_13_",
  "metrics_op0_cube_tag_14_",
  "metrics_op0_cube_tag_15_"
};
char* metrics_op0_vec_tag[] = {
  "metrics_op0_vec_tag_0_",
  "metrics_op0_vec_tag_1_",
  "metrics_op0_vec_tag_2_",
  "metrics_op0_vec_tag_3_",
  "metrics_op0_vec_tag_4_",
  "metrics_op0_vec_tag_5_",
  "metrics_op0_vec_tag_6_",
  "metrics_op0_vec_tag_7_",
  "metrics_op0_vec_tag_8_",
  "metrics_op0_vec_tag_9_",
  "metrics_op0_vec_tag_10_",
  "metrics_op0_vec_tag_11_",
  "metrics_op0_vec_tag_12_",
  "metrics_op0_vec_tag_13_",
  "metrics_op0_vec_tag_14_",
  "metrics_op0_vec_tag_15_"
};
char* op0_cube_file_tag[] = {
  "op0_cube_file_tag_0_",
  "op0_cube_file_tag_1_",
  "op0_cube_file_tag_2_",
  "op0_cube_file_tag_3_",
  "op0_cube_file_tag_4_",
  "op0_cube_file_tag_5_",
  "op0_cube_file_tag_6_",
  "op0_cube_file_tag_7_",
  "op0_cube_file_tag_8_",
  "op0_cube_file_tag_9_",
  "op0_cube_file_tag_10_",
  "op0_cube_file_tag_11_",
  "op0_cube_file_tag_12_",
  "op0_cube_file_tag_13_",
  "op0_cube_file_tag_14_",
  "op0_cube_file_tag_15_"
};
char* op0_vec_file_tag[] = {
  "op0_vec_file_tag_0_",
  "op0_vec_file_tag_1_",
  "op0_vec_file_tag_2_",
  "op0_vec_file_tag_3_",
  "op0_vec_file_tag_4_",
  "op0_vec_file_tag_5_",
  "op0_vec_file_tag_6_",
  "op0_vec_file_tag_7_",
  "op0_vec_file_tag_8_",
  "op0_vec_file_tag_9_",
  "op0_vec_file_tag_10_",
  "op0_vec_file_tag_11_",
  "op0_vec_file_tag_12_",
  "op0_vec_file_tag_13_",
  "op0_vec_file_tag_14_",
  "op0_vec_file_tag_15_"
};

char* metrics_op1_cube_tag[] = {
  "metrics_op1_cube_tag_0_",
  "metrics_op1_cube_tag_1_",
  "metrics_op1_cube_tag_2_",
  "metrics_op1_cube_tag_3_",
  "metrics_op1_cube_tag_4_",
  "metrics_op1_cube_tag_5_",
  "metrics_op1_cube_tag_6_",
  "metrics_op1_cube_tag_7_",
  "metrics_op1_cube_tag_8_",
  "metrics_op1_cube_tag_9_",
  "metrics_op1_cube_tag_10_",
  "metrics_op1_cube_tag_11_",
  "metrics_op1_cube_tag_12_",
  "metrics_op1_cube_tag_13_",
  "metrics_op1_cube_tag_14_",
  "metrics_op1_cube_tag_15_"
};
char* metrics_op1_vec_tag[] = {
  "metrics_op1_vec_tag_0_",
  "metrics_op1_vec_tag_1_",
  "metrics_op1_vec_tag_2_",
  "metrics_op1_vec_tag_3_",
  "metrics_op1_vec_tag_4_",
  "metrics_op1_vec_tag_5_",
  "metrics_op1_vec_tag_6_",
  "metrics_op1_vec_tag_7_",
  "metrics_op1_vec_tag_8_",
  "metrics_op1_vec_tag_9_",
  "metrics_op1_vec_tag_10_",
  "metrics_op1_vec_tag_11_",
  "metrics_op1_vec_tag_12_",
  "metrics_op1_vec_tag_13_",
  "metrics_op1_vec_tag_14_",
  "metrics_op1_vec_tag_15_"
};
char* op1_cube_file_tag[] = {
  "op1_cube_file_tag_0_",
  "op1_cube_file_tag_1_",
  "op1_cube_file_tag_2_",
  "op1_cube_file_tag_3_",
  "op1_cube_file_tag_4_",
  "op1_cube_file_tag_5_",
  "op1_cube_file_tag_6_",
  "op1_cube_file_tag_7_",
  "op1_cube_file_tag_8_",
  "op1_cube_file_tag_9_",
  "op1_cube_file_tag_10_",
  "op1_cube_file_tag_11_",
  "op1_cube_file_tag_12_",
  "op1_cube_file_tag_13_",
  "op1_cube_file_tag_14_",
  "op1_cube_file_tag_15_"
};
char* op1_vec_file_tag[] = {
  "op1_vec_file_tag_0_",
  "op1_vec_file_tag_1_",
  "op1_vec_file_tag_2_",
  "op1_vec_file_tag_3_",
  "op1_vec_file_tag_4_",
  "op1_vec_file_tag_5_",
  "op1_vec_file_tag_6_",
  "op1_vec_file_tag_7_",
  "op1_vec_file_tag_8_",
  "op1_vec_file_tag_9_",
  "op1_vec_file_tag_10_",
  "op1_vec_file_tag_11_",
  "op1_vec_file_tag_12_",
  "op1_vec_file_tag_13_",
  "op1_vec_file_tag_14_",
  "op1_vec_file_tag_15_"
};

char* metrics_op2_cube_tag[] = {
  "metrics_op2_cube_tag_0_",
  "metrics_op2_cube_tag_1_",
  "metrics_op2_cube_tag_2_",
  "metrics_op2_cube_tag_3_",
  "metrics_op2_cube_tag_4_",
  "metrics_op2_cube_tag_5_",
  "metrics_op2_cube_tag_6_",
  "metrics_op2_cube_tag_7_",
  "metrics_op2_cube_tag_8_",
  "metrics_op2_cube_tag_9_",
  "metrics_op2_cube_tag_10_",
  "metrics_op2_cube_tag_11_",
  "metrics_op2_cube_tag_12_",
  "metrics_op2_cube_tag_13_",
  "metrics_op2_cube_tag_14_",
  "metrics_op2_cube_tag_15_"
};
char* metrics_op2_vec_tag[] = {
  "metrics_op2_vec_tag_0_",
  "metrics_op2_vec_tag_1_",
  "metrics_op2_vec_tag_2_",
  "metrics_op2_vec_tag_3_",
  "metrics_op2_vec_tag_4_",
  "metrics_op2_vec_tag_5_",
  "metrics_op2_vec_tag_6_",
  "metrics_op2_vec_tag_7_",
  "metrics_op2_vec_tag_8_",
  "metrics_op2_vec_tag_9_",
  "metrics_op2_vec_tag_10_",
  "metrics_op2_vec_tag_11_",
  "metrics_op2_vec_tag_12_",
  "metrics_op2_vec_tag_13_",
  "metrics_op2_vec_tag_14_",
  "metrics_op2_vec_tag_15_"
};
char* op2_cube_file_tag[] = {
  "op2_cube_file_tag_0_",
  "op2_cube_file_tag_1_",
  "op2_cube_file_tag_2_",
  "op2_cube_file_tag_3_",
  "op2_cube_file_tag_4_",
  "op2_cube_file_tag_5_",
  "op2_cube_file_tag_6_",
  "op2_cube_file_tag_7_",
  "op2_cube_file_tag_8_",
  "op2_cube_file_tag_9_",
  "op2_cube_file_tag_10_",
  "op2_cube_file_tag_11_",
  "op2_cube_file_tag_12_",
  "op2_cube_file_tag_13_",
  "op2_cube_file_tag_14_",
  "op2_cube_file_tag_15_"
};
char* op2_vec_file_tag[] = {
  "op2_vec_file_tag_0_",
  "op2_vec_file_tag_1_",
  "op2_vec_file_tag_2_",
  "op2_vec_file_tag_3_",
  "op2_vec_file_tag_4_",
  "op2_vec_file_tag_5_",
  "op2_vec_file_tag_6_",
  "op2_vec_file_tag_7_",
  "op2_vec_file_tag_8_",
  "op2_vec_file_tag_9_",
  "op2_vec_file_tag_10_",
  "op2_vec_file_tag_11_",
  "op2_vec_file_tag_12_",
  "op2_vec_file_tag_13_",
  "op2_vec_file_tag_14_",
  "op2_vec_file_tag_15_"
};

char* etalon_file_tag[] = {
  "etalon_file_tag_0_",
  "etalon_file_tag_1_",
  "etalon_file_tag_2_",
  "etalon_file_tag_3_",
  "etalon_file_tag_4_",
  "etalon_file_tag_5_",
  "etalon_file_tag_6_",
  "etalon_file_tag_7_",
  "etalon_file_tag_8_",
  "etalon_file_tag_9_",
  "etalon_file_tag_10_",
  "etalon_file_tag_11_",
  "etalon_file_tag_12_",
  "etalon_file_tag_13_",
  "etalon_file_tag_14_",
  "etalon_file_tag_15_"
};



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
    if(cfg.op1_en==Enable_En) {
      op1 = nu_vpe_load_op01_by_tags(heap_id,&cfg.op1_config,metrics_op1_cube_tag[i],metrics_op1_vec_tag[i],op1_cube_file_tag[i],op1_vec_file_tag[i]);
    }
    if(cfg.op2_en==Enable_En) {
      op2 = nu_vpe_load_op2_by_tags(heap_id,&cfg.op2_config,metrics_op2_cube_tag[i],metrics_op2_vec_tag[i],op2_cube_file_tag[i],op2_vec_file_tag[i]);
    }
    
    etalon = nu_load_cube(heap_id,etalon_file_tag[i],res_metrics);
    if(etalon == NULL) return -1;
    
    //print_in_data(in_data,in_size);
    
    nu_vpe_print_config(&cfg);
    nu_vpe_decide_dma_config(&cfg,in_metrics,&cfg_dma);
    nu_vpe_print_config_dma(&cfg_dma);
    
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
    nu_vpe_run(NU_VPE_STANDALONE_BASE, &cfg_dma);     // To Invoke Or Not To Invoke Internal DMA Channel - Decide inside nu_vpe_run
    
      // Finalize Required DMA Channels
    if(cfg_dma.dma_src_en == Enable_NotEn)
      nu_vpe_wait_rd_main_channel_complete(NU_CPDMAC_ASM_BASE);
    if(cfg_dma.dma_dst_en == Enable_NotEn)
      nu_vpe_wait_wr_main_channel_complete(NU_CPDMAC_ASM_BASE);
    nu_vpe_wait(NU_VPE_STANDALONE_BASE, &cfg_dma);
    
    
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
