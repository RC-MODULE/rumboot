
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>

#include <platform/devices.h> 

#include <platform/devices/nu_lib.h> 
#include <platform/devices/nu_test_lib.h> 
#include <platform/devices/nu_test_macro.h> 

#include "platform/devices/nu_npe_file_tags.h"

void *in_data;
void *warr;
void *etalon;
void *res_data;
void *op0;
void *op1;
void *op2;
void *lut1;
void *lut2;

void *mpe_cfg_lut;

ConfigVPE cfg_vpe;
ConfigMPE cfg_mpe;
ConfigPPE cfg_ppe;
ConfigREGPPE  cfg_reg_ppe = {0};

CubeMetrics* in_metrics;
WarrMetrics* warr_metrics;
CubeMetrics* res_metrics;

VectorMetrics* lut1_metrics;
VectorMetrics* lut2_metrics;

VPEStatusRegs status_regs_etalon;

int nu_mpe_decide_dma_config(
  ConfigMPE* cfg, 
  CubeMetrics* cube_metrics, 
  WarrMetrics* warr_metrics,
  void* cube,
  void* warr,
  void* cfg_lut
) {
  cfg->dma_d_config.rdma.BFCA  = (uint32_t) cube;
  cfg->dma_w_config.rdma.BFCA  = (uint32_t) warr;
  if(nu_mpe_look_up_dma_config(cfg,cfg_lut)!=0) return -1;
  if(nu_mpe_decide_dma_config_trivial(cfg, cube_metrics, warr_metrics)!=0) return -1;
  return 0;
}

void nu_vpe_decide_dma_config (
  ConfigVPE* cfg,
  CubeMetrics* in_metrics,
  uint8_t axi_len,
  void*in_data,
  void*op0,
  void*op1,
  void*op2,
  CubeMetrics*res_metrics,
  void*res_data
) {
  rumboot_printf("nu_vpe_decide_dma_config \n");
  cfg->trace_mode = TraceMode_MPE;  // FIRST!!!! Because nu_vpe_decide_dma_config_trivial depends On It
  
  cfg->op0_rdma_config.dma_data_mode = cfg->op0_config.mux_mode; // Init Them
  cfg->op1_rdma_config.dma_data_mode = cfg->op1_config.mux_mode;
  cfg->op2_rdma_config.dma_data_mode = cfg->op2_config.mux_mode;
  cfg->wdma_config.dma_data_mode     = Mode_Element; // Copypaste From src_rdma_config in nu_vpe_decide_dma_config_trivial
  cfg->wdma_config.dma_data_use=DmaDUse_Off;
  
  cfg->src_flying = Enable_En;
  cfg->dst_flying = Enable_En;
  
  nu_vpe_decide_dma_config_trivial(cfg,in_metrics);
  
  cfg->src_rdma_config.dma_baddr = (uint32_t) 0xDEADBEEF; // Should Be Not Used
  cfg->op0_rdma_config.dma_baddr = (uint32_t) op0;
  cfg->op1_rdma_config.dma_baddr = (uint32_t) op1;
  cfg->op2_rdma_config.dma_baddr = (uint32_t) op2;
  cfg->wdma_config.dma_baddr     = (uint32_t) 0xDEADBEEF;  //

  cfg->src_rdma_config.dma_axi_len = axi_len;
  cfg->op0_rdma_config.dma_axi_len = axi_len;
  cfg->op1_rdma_config.dma_axi_len = axi_len;
  cfg->op2_rdma_config.dma_axi_len = axi_len;
  cfg->wdma_config.dma_axi_len     = axi_len;
  
}

void nu_ppe_decide_dma_config(
  ConfigPPE* cfg,
  ConfigREGPPE* cfg_reg,
  CubeMetrics* res_metrics,
  void*in_data,
  void*res_data
){
  cfg_reg->rBALi = (uintptr_t)0xDEADBEEF;
  cfg_reg->wBALo = (uintptr_t)res_data;

  cfg_reg->wOpM = 3 << 8; // FLYING_BOXED 
                         // Other Fields Of wOpM Will Be Appended By nu_ppe_decide_dma_config_trivial
  nu_ppe_decide_dma_config_trivial(cfg, res_metrics, cfg_reg);
}

int main() {
  int heap_id;
  int i;
  int iterations;
  uint8_t  axi_len;
  
  axi_len = 15;
  
  rumboot_printf("Hello\n");

  heap_id = nu_get_heap_id();
  
  rumboot_platform_request_file("num_iterations_file_tag",(uintptr_t) &iterations);
  rumboot_printf("Number of iterations %d\n",iterations);
  
  mpe_cfg_lut = nu_mpe_load_cfg_lut(heap_id);
  if(mpe_cfg_lut == NULL) return -1;
  
  na_cu_set_units_direct_mode(NPE_BASE+NA_CU_REGS_BASE+NA_UNITS_MODE, NA_CU_MPE_UNIT_MODE|NA_CU_VPE_UNIT_MODE|NA_CU_PPE_UNIT_MODE);
  
  for(i=0;i<iterations;i++) {
    rumboot_printf("Starting iteration %d\n",i);
    
    if(nu_mpe_load_cfg_by_tag(heap_id, &cfg_mpe, cfg_mpe_file_tag[i]) != 0) return -1;
    if(nu_vpe_load_cfg_by_tag(heap_id, &cfg_vpe, cfg_vpe_file_tag[i]) != 0) return -1;
    if(nu_ppe_load_cfg_by_tag(heap_id, &cfg_ppe, cfg_ppe_file_tag[i]) != 0) return -1;
    
      // Load Metrics Files
    in_metrics = nu_load_cube_metrics(heap_id,metrics_in_tag[i]);
    if(in_metrics == NULL) return -1;
    
    warr_metrics = nu_load_warr_metrics(heap_id, metrics_warr_tag[i]);
    if(warr_metrics == NULL) return -1;

    res_metrics= nu_load_cube_metrics(heap_id,metrics_etalon_tag[i]);
    if(res_metrics == NULL) return -1;
    
      // Load Input Data
    in_data = nu_load_cube(heap_id, in_file_tag[i],in_metrics);
    if(in_data == NULL) return -1;

    warr = nu_load_warr(heap_id, warr_file_tag[i], warr_metrics);
    if(warr == NULL) return -1;
    
      // Load OP0-OP2 Operands If Needed
    if(cfg_vpe.op0_en==Enable_En) {
      op0 = nu_vpe_load_op01_by_tags(heap_id,&cfg_vpe.op0_config,metrics_op0_cube_tag[i],metrics_op0_vec_tag[i],op0_cube_file_tag[i],op0_vec_file_tag[i]);
    }
    else op0 = NULL;
    if(cfg_vpe.op1_en==Enable_En) {
      op1 = nu_vpe_load_op01_by_tags(heap_id,&cfg_vpe.op1_config,metrics_op1_cube_tag[i],metrics_op1_vec_tag[i],op1_cube_file_tag[i],op1_vec_file_tag[i]);
    }
    else op1 = NULL;
    if(cfg_vpe.op2_en==Enable_En) {
      op2 = nu_vpe_load_op2_by_tags(heap_id,&cfg_vpe.op2_config,metrics_op2_cube_tag[i],metrics_op2_vec_tag[i],op2_cube_file_tag[i],op2_vec_file_tag[i]);
    }
    else op2 = NULL;
    
      // Load LUTs If Needed
    if(cfg_vpe.op2_config.lut_en == Enable_En) {
      lut1_metrics = nu_load_vec_metrics(heap_id,metrics_lut1_file_tag[i]);
      lut2_metrics = nu_load_vec_metrics(heap_id,metrics_lut2_file_tag[i]);
      lut1 = nu_load_vec(heap_id,lut1_file_tag[i],lut1_metrics);
      lut2 = nu_load_vec(heap_id,lut2_file_tag[i],lut2_metrics);
      nu_vpe_load_lut(MY_VPE_REGS_BASE,lut1,lut2);
    }
    
    
      // Malloc Result
    res_data = nu_mpe_malloc_res(heap_id, res_metrics);
    if(res_data == NULL) return -1;
    
      // Load Etalon
    etalon = nu_load_cube(heap_id, etalon_file_tag[i], res_metrics);
    if(etalon == NULL) return -1;

    // if(nu_vpe_load_status_regs_by_tag(heap_id,&status_regs_etalon,status_regs_file_tag[i]) != 0) return -1;
    
    if(nu_mpe_decide_dma_config(&cfg_mpe,in_metrics,warr_metrics,in_data,warr,mpe_cfg_lut)!=0) return -1;
    nu_vpe_decide_dma_config (&cfg_vpe,res_metrics,axi_len,NULL,op0,op1,op2,res_metrics,res_data);
    nu_ppe_decide_dma_config (&cfg_ppe,&cfg_reg_ppe,res_metrics,NULL,res_data);
    
    nu_mpe_print_config(&cfg_mpe);
    nu_vpe_print_config(&cfg_vpe);
    // nu_vpe_print_status_regs_etalon(&status_regs_etalon);
    nu_ppe_print_config(&cfg_ppe);
    nu_ppe_print_config_reg(&cfg_reg_ppe);
    
    nu_vpe_setup(MY_VPE_REGS_BASE, &cfg_vpe);
    nu_mpe_setup(MY_MPE_REGS_BASE, &cfg_mpe);
    nu_ppe_setup_reg(MY_PPE_RDMA_BASE, MY_PPE_REGS_BASE, &cfg_reg_ppe);
    
    
    cfg_reg_ppe.wOpEn  = 0x1;
    nu_ppe_wdma_run(MY_PPE_REGS_BASE, &cfg_reg_ppe);
    nu_vpe_run(MY_VPE_REGS_BASE, &cfg_vpe);
    nu_mpe_run(MY_MPE_REGS_BASE, &cfg_mpe);
    
    nu_ppe_wait_complete(MY_PPE_REGS_BASE);
    
    rumboot_printf("Comparing..\n");
    
    if(nu_bitwise_compare(res_data,etalon,res_metrics->s) == 0)
      rumboot_printf("Iteration %d PASSED\n",i);
    else {
      rumboot_printf("Test FAILED at iteration %d\n",i);
      return 1;
    }
  }
  
  rumboot_printf("Test Passed\n");
  return 0;
}

