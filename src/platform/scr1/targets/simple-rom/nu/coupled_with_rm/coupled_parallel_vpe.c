
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>

#include <platform/devices.h> 

#include <platform/devices/nu_lib.h> 
#include <platform/devices/nu_test_lib.h> 

#include "platform/devices/nu_vpe_file_tags.h"

void *in_data[2];
void *etalon[2];
void *res_data[2];
void *op0[2];
void *op1[2];
void *op2[2];
void *lut1;
void *lut2;


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
  rumboot_printf("nu_vpe_decide_dma_config \n");
  cfg->op0_rdma_config.dma_data_mode = cfg->op0_config.mux_mode; // Init Them
  cfg->op1_rdma_config.dma_data_mode = cfg->op1_config.mux_mode;
  cfg->op2_rdma_config.dma_data_mode = cfg->op2_config.mux_mode;
  cfg->wdma_config.dma_data_mode      = 1; // Copypaste From src_rdma_config in nu_vpe_decide_dma_config_trivial
  cfg->wdma_config.dma_data_use=DmaDUse_Off;
  
  cfg->src_flying = cfg->in_data_type == DataTypeExt_Int32 || cfg->in_data_type == DataTypeExt_Fp32 ? Enable_En : Enable_NotEn;
#ifdef FORCE_VPE_WDMA_EN
  cfg->dst_flying = Enable_NotEn;
#else
  cfg->dst_flying = cfg->out_data_type == DataType_Int8 ? Enable_NotEn : Enable_En;
#endif
  
  nu_vpe_decide_dma_config_trivial(cfg,in_metrics,cfg_dma);
  cfg_dma->dma_dst_en = cfg->dst_flying ? Enable_NotEn : Enable_NotEn;
  
  cfg->src_rdma_config.dma_baddr = (uint32_t) in_data;
  cfg->op0_rdma_config.dma_baddr = (uint32_t) op0;
  cfg->op1_rdma_config.dma_baddr = (uint32_t) op1;
  cfg->op2_rdma_config.dma_baddr = (uint32_t) op2;
  cfg->wdma_config.dma_baddr = (uint32_t) res_data;
  
  
}

ConfigVPE cfg;
ConfigDMAVPE cfg_dma;

CubeMetrics* in_metrics[2];
CubeMetrics* res_metrics[2];

VectorMetrics* lut1_metrics;
VectorMetrics* lut2_metrics;

int main() {
  int heap_id;
  int i;
  int iterations;
  
    // Time Measure
  uint32_t start[2];
  uint32_t end[2];
  uint32_t delta[2];
  //uint32_t num_cycles;
  //uint32_t num_vectors;
  //uint32_t productivity_x1000;
  //uint32_t productivity_x100;
  //uint32_t productivity_frac;  
  
  rumboot_printf("Hello\n");
  
  heap_id = nu_get_heap_id();
  
  rumboot_platform_request_file("num_iterations_file_tag",(uintptr_t) &iterations);
  rumboot_printf("Number of iterations %d\n",iterations);
  
  for(i=0;i<iterations;i++) {
    rumboot_printf("Starting iteration %d\n",i);
    
    if(nu_vpe_load_cfg_by_tag(heap_id, &cfg, cfg_file_tag[i]) != 0) return -1;
    
    in_metrics[i] = nu_load_cube_metrics(heap_id,metrics_in_tag[i]);
    if(in_metrics[i] == NULL) return -1;
    
    res_metrics[i]= nu_load_cube_metrics(heap_id,metrics_etalon_tag[i]);
    if(res_metrics[i] == NULL) return -1;
    
    in_data[i] = nu_load_cube(heap_id,in_file_tag[i],in_metrics[i]);
    if(in_data[i] == NULL) return -1;
    
    res_data[i] = nu_vpe_malloc_res(heap_id, res_metrics[i]);
    if(res_data[i] == NULL) return -1;
    
      // Load OP0-OP2 Operands If Needed
    if(cfg.op0_en==Enable_En) {
      op0[i] = nu_vpe_load_op01_by_tags(heap_id,&cfg.op0_config,metrics_op0_cube_tag[i],metrics_op0_vec_tag[i],op0_cube_file_tag[i],op0_vec_file_tag[i]);
    }
    else op0[i] = NULL;
    if(cfg.op1_en==Enable_En) {
      op1[i] = nu_vpe_load_op01_by_tags(heap_id,&cfg.op1_config,metrics_op1_cube_tag[i],metrics_op1_vec_tag[i],op1_cube_file_tag[i],op1_vec_file_tag[i]);
    }
    else op1[i] = NULL;
    if(cfg.op2_en==Enable_En) {
      op2[i] = nu_vpe_load_op2_by_tags(heap_id,&cfg.op2_config,metrics_op2_cube_tag[i],metrics_op2_vec_tag[i],op2_cube_file_tag[i],op2_vec_file_tag[i]);
    }
    else op2[i] = NULL;
    
      // Load LUTs If Needed
    if(cfg.op2_config.lut_en == Enable_En) {
      lut1_metrics = nu_load_vec_metrics(heap_id,metrics_lut1_file_tag[i]);
      lut2_metrics = nu_load_vec_metrics(heap_id,metrics_lut2_file_tag[i]);
      lut1 = nu_load_vec(heap_id,lut1_file_tag[i],lut1_metrics);
      lut2 = nu_load_vec(heap_id,lut2_file_tag[i],lut2_metrics);
      nu_vpe_load_lut(NU_VPE_STANDALONE_BASE,lut1,lut2);
    }
    
    etalon[i] = nu_load_cube(heap_id,etalon_file_tag[i],res_metrics[i]);
    if(etalon[i] == NULL) return -1;
    
    //print_in_data(in_data,in_size);
    
    //nu_vpe_print_config(&cfg);
    nu_vpe_decide_dma_config(&cfg,in_metrics[i],in_data[i],op0[i],op1[i],op2[i],res_metrics[i],res_data[i],&cfg_dma);
    //nu_print_config_dma(&cfg.src_rdma_config,"src_rdma_config");
    //nu_print_config_dma(&cfg.op0_rdma_config,"op0_rdma_config");
    //nu_print_config_dma(&cfg.op1_rdma_config,"op1_rdma_config");
    //nu_print_config_dma(&cfg.op2_rdma_config,"op2_rdma_config");
    //nu_print_config_dma(&cfg.wdma_config,"wdma_config");
    
    start[i] = rumboot_platform_get_uptime();
    
    nu_vpe_setup(NU_VPE_STANDALONE_BASE, &cfg, &cfg_dma);
    
      // Setup Main Channel DMAs if Required
    //if(cfg.src_rdma_config.dma_op_en == Enable_NotEn)
    //  nu_vpe_config_rd_main_channel(NU_CPDMAC_ASM_BASE,in_data,in_metrics->s);
    //if(cfg.wdma_config.dma_op_en == Enable_NotEn)
    //  nu_vpe_config_wr_main_channel(NU_CPDMAC_ASM_BASE,res_data,res_metrics->s);
    //
    //rumboot_printf("Running DMA..\n");
    //
    //  // Invoke Required DMA Channels
    //if(cfg.src_rdma_config.dma_op_en == Enable_NotEn)
    //  nu_vpe_run_rd_main_channel(NU_CPDMAC_ASM_BASE);
    //if(cfg.wdma_config.dma_op_en == Enable_NotEn)
    //  nu_vpe_run_wr_main_channel(NU_CPDMAC_ASM_BASE);
    
    nu_vpe_run(NU_VPE_STANDALONE_BASE, &cfg);     // To Invoke Or Not To Invoke Internal DMA Channel - Decide inside nu_vpe_run
    nu_vpe_wait_cntx_appl(NU_VPE_STANDALONE_BASE, &cfg);
    
      // Finalize Required DMA Channels
    //if(cfg.src_rdma_config.dma_op_en == Enable_NotEn)
    //  nu_vpe_wait_rd_main_channel_complete(NU_CPDMAC_ASM_BASE);
    //if(cfg.wdma_config.dma_op_en == Enable_NotEn)
    //  nu_vpe_wait_wr_main_channel_complete(NU_CPDMAC_ASM_BASE);
    //nu_vpe_wait(NU_VPE_STANDALONE_BASE, &cfg);
    
    
    //num_cycles = delta * 1000 / 650; // delta in microseconds, 1 DUT cycle is 650 nanoseconds
    //num_vectors = in_metrics->H * in_metrics->W * in_metrics->C / 16;
    //productivity_x1000 =  num_vectors*1000 / num_cycles;
    //productivity_x100 = productivity_x1000 / 10;
    //productivity_frac = productivity_x1000 % 10;
    //
    //rumboot_printf("Comparing..\n");
    
    
//     rumboot_printf("Productivity: %d.%d Percent  num_vectors = %d, start = %d, end = %d, delta = %d, num_cycles = %d \n",
//                    productivity_x100,productivity_frac,num_vectors,start,end,delta,num_cycles);
//    rumboot_printf("Productivity: %d.%d Percent  num_vectors = %d, num_cycles = %d \n",
//                   productivity_x100,productivity_frac,num_vectors,num_cycles);
  }
  
  for(i=0;i<iterations;i++) {
    nu_vpe_wait(NU_VPE_STANDALONE_BASE, &cfg); //it's better to put cfg[i] here. but it will work good too
    end[i] = rumboot_platform_get_uptime();
    delta[i] = end[i] - start[0];
    if(nu_bitwise_compare(res_data[i],etalon[i],res_metrics[i]->s) == 0)
        rumboot_printf("Iteration %d PASSED\n",i);
        else {
        rumboot_printf("Test FAILED at iteration %d\n",i);
        return 1;
        }
  }
  
  rumboot_printf("First cube time %d us, both cube time %d us \n",delta[0],delta[1]);
  

  return 0;
  
}
