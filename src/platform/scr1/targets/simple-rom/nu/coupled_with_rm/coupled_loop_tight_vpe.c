
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>

#include <platform/devices.h> 

#include <platform/devices/nu_lib.h> 
#include <platform/devices/nu_test_lib.h> 
#include <platform/devices/nu_test_macro.h> 

#include "platform/devices/nu_vpe_file_tags.h"

void *in_data;
void *etalon;
void *res_data;
void *op0;
void *op1;
void *op2;
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
  uint8_t axi_len;
  
  rumboot_printf("nu_vpe_decide_dma_config \n");
  cfg->op0_rdma_config.dma_data_mode = cfg->op0_config.mux_mode; // Init Them
  cfg->op1_rdma_config.dma_data_mode = cfg->op1_config.mux_mode;
  cfg->op2_rdma_config.dma_data_mode = cfg->op2_config.mux_mode;
  cfg->wdma_config.dma_data_mode     = Mode_Element; // Copypaste From src_rdma_config in nu_vpe_decide_dma_config_trivial
  cfg->wdma_config.dma_data_use      = DmaDUse_Off;
  
  cfg->src_flying = cfg->in_data_type == DataTypeExt_Int32 || cfg->in_data_type == DataTypeExt_Fp32 ? Enable_En : Enable_NotEn;
#ifdef FORCE_VPE_WDMA_EN
  cfg->dst_flying = Enable_NotEn;
#else
  cfg->dst_flying = cfg->out_data_type == DataType_Int8 ? Enable_NotEn : Enable_En;
#endif
#ifdef AXI_LEN
    axi_len = AxiLen;
#else 
    axi_len = 15;
#endif

  
  nu_vpe_decide_dma_config_trivial(cfg,in_metrics);
  cfg_dma->H = in_metrics->H;
  cfg_dma->W = in_metrics->W;
  cfg_dma->C = in_metrics->C;
  cfg_dma->dma_dst_en = cfg->dst_flying ? Enable_NotEn : Enable_NotEn;
  
  cfg->src_rdma_config.dma_baddr = (uint32_t) in_data ;
  cfg->op0_rdma_config.dma_baddr = (uint32_t) op0     ;
  cfg->op1_rdma_config.dma_baddr = (uint32_t) op1     ;
  cfg->op2_rdma_config.dma_baddr = (uint32_t) op2     ;
  cfg->wdma_config.dma_baddr     = (uint32_t) res_data;
  
  cfg->src_rdma_config.dma_axi_len = axi_len;
  cfg->op0_rdma_config.dma_axi_len = axi_len;
  cfg->op1_rdma_config.dma_axi_len = axi_len;
  cfg->op2_rdma_config.dma_axi_len = axi_len;
  cfg->wdma_config.dma_axi_len     = axi_len;
  
}

ConfigVPE cfg;
ConfigDMAVPE cfg_dma;

CubeMetrics* in_metrics;
CubeMetrics* res_metrics;

VectorMetrics* lut1_metrics;
VectorMetrics* lut2_metrics;

  // Iteration Tail Vars
ConfigVPE tail_cfg;
void* etalon_ptr[32];
void* res_ptr[32];
int res_size[32];
  //

int main() {
  int heap_id;
  int i;
  int iterations;
  
  rumboot_printf("Hello\n");
  
  heap_id = nu_get_heap_id();
  
  rumboot_platform_request_file("num_iterations_file_tag",(uintptr_t) &iterations);
  rumboot_printf("Number of iterations %d\n",iterations);
  
#if DUT_IS_NPE
  na_cu_set_units_direct_mode(NPE_BASE+NA_CU_REGS_BASE, NA_CU_VPE_UNIT_MODE);
#endif
  
  for(i=0;i<iterations;i++) {
      rumboot_printf("Starting iteration %d\n",i);
      
      if(nu_vpe_load_cfg_by_tag(heap_id, &cfg, cfg_file_tag[i]) != 0) return -1;
      
      in_metrics = nu_load_cube_metrics(heap_id,metrics_in_tag[i]);
      if(in_metrics == NULL) return -1;
      
      res_metrics= nu_load_cube_metrics(heap_id,metrics_etalon_tag[i]);
      if(res_metrics == NULL) return -1;

      in_data = nu_load_cube_misaligned(heap_id,in_file_tag[i],in_metrics,IntMisalign);
      if(in_data == NULL) return -1;
      
      res_data = nu_vpe_malloc_res(heap_id, res_metrics);
      if(res_data == NULL) return -1;
      
        // Load OP0-OP2 Operands If Needed
      if(cfg.op0_en==Enable_En) {
        op0 = nu_vpe_load_op01_misaligned_by_tags(heap_id,&cfg.op0_config,metrics_op0_cube_tag[i],metrics_op0_vec_tag[i],op0_cube_file_tag[i],op0_vec_file_tag[i],IntMisalign);
      }
      else op0 = NULL;
      if(cfg.op1_en==Enable_En) {
        op1 = nu_vpe_load_op01_misaligned_by_tags(heap_id,&cfg.op1_config,metrics_op1_cube_tag[i],metrics_op1_vec_tag[i],op1_cube_file_tag[i],op1_vec_file_tag[i],IntMisalign);
      }
      else op1 = NULL;
      if(cfg.op2_en==Enable_En) {
        op2 = nu_vpe_load_op2_misaligned_by_tags(heap_id,&cfg.op2_config,metrics_op2_cube_tag[i],metrics_op2_vec_tag[i],op2_cube_file_tag[i],op2_vec_file_tag[i],IntMisalign);
      }
      else op2 = NULL;
      
        // Load LUTs If Needed
      if(cfg.op2_config.lut_en == Enable_En) {
        lut1_metrics = nu_load_vec_metrics(heap_id,metrics_lut1_file_tag[i]);
        lut2_metrics = nu_load_vec_metrics(heap_id,metrics_lut2_file_tag[i]);
        lut1 = nu_load_vec(heap_id,lut1_file_tag[i],lut1_metrics);
        lut2 = nu_load_vec(heap_id,lut2_file_tag[i],lut2_metrics);
        nu_vpe_load_lut(MY_VPE_REGS_BASE,lut1,lut2);
      }
      
      etalon = nu_load_cube(heap_id,etalon_file_tag[i],res_metrics);
      if(etalon == NULL) return -1;
      
      //print_in_data(in_data,in_size);
      
      // if(nu_vpe_load_status_regs_by_tag(heap_id,&status_regs_etalon,status_regs_file_tag[i]) != 0) return -1;

      cfg.trace_mode = TraceMode_MPE;

      // nu_vpe_print_config(&cfg);
      nu_vpe_decide_dma_config(&cfg,in_metrics,in_data,op0,op1,op2,res_metrics,res_data,&cfg_dma);
      if(i==iterations-1) 
        cfg.mark = Enable_En;
      else
        cfg.mark = Enable_NotEn;
      
      // nu_print_config_dma(&cfg.src_rdma_config,"src_rdma_config");
      // nu_print_config_dma(&cfg.op0_rdma_config,"op0_rdma_config");
      // nu_print_config_dma(&cfg.op1_rdma_config,"op1_rdma_config");
      // nu_print_config_dma(&cfg.op2_rdma_config,"op2_rdma_config");
      // nu_print_config_dma(&cfg.wdma_config,"wdma_config");
      // nu_vpe_print_status_regs_etalon(&status_regs_etalon);
      
      nu_vpe_setup(MY_VPE_REGS_BASE, &cfg);
      
      rumboot_printf("Running DMA..\n");
      
      nu_vpe_run(MY_VPE_REGS_BASE, &cfg);     // To Invoke Or Not To Invoke Internal DMA Channel - Decide inside nu_vpe_run
      nu_vpe_wait_cntx_appl(MY_VPE_REGS_BASE, &cfg);
    
      res_ptr[i] = res_data;
      etalon_ptr[i]=etalon;
      res_size[i]=res_metrics->s;
  }
    
    
  nu_vpe_wait(MY_VPE_REGS_BASE, &cfg); // Wait For The Last Iteration
  
  rumboot_printf("Comparing..\n");
  
  
  for(i=0;i<iterations;i++) {
    if(nu_bitwise_compare(res_ptr[i],etalon_ptr[i],res_size[i]) == 0)
      rumboot_printf("Iteration %d PASSED\n",i);
    else {
      rumboot_printf("Test FAILED at iteration %d\n",i);
      return 1;
    }
    
  }
  
  return 0;
  
}
