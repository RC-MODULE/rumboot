#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>

#include <platform/devices.h> 

#include <devices/rcm_na/nu_lib.h> 
#include <devices/rcm_na/nu_test_lib.h> 
#include <devices/rcm_na/nu_test_macro.h> 

int nu_mpe_decide_dma_config(
  ConfigMPE* cfg, 
  CubeMetrics* cube_metrics, 
  WarrMetrics* warr_metrics,
  void* cube,
  void* warr,
  void* cfg_lut
) {
  cfg->dma_d_config.rdma.BFCA  = nu_virt_to_dma(cube); // Data Base Address

  cfg->dma_w_config.rdma.BFCA  = nu_virt_to_dma(warr); // Weights Base Address
  cfg->dma_d_config.depend_mask = 0; // Does Not Depend On Any Other Channel Run
  cfg->dma_w_config.depend_mask = 0;
  if(nu_mpe_look_up_dma_config(cfg,cfg_lut)!=0) return -1; // Search For The MPE DMA Configuration (That Is Predefined In cfg_lut Table)
  if(nu_mpe_decide_dma_config_trivial(cfg, cube_metrics, warr_metrics)!=0) return -1; // Fill Other Fields
  return 0;
}

void nu_ppe_decide_dma_config(
  ConfigPPE* cfg, 
  CubeMetrics* res_metrics, 
  ConfigREGPPE* cfg_reg,
  uint32_t flying_mode,
  void* in_data,
  void* res_data
) {
  cfg_reg->rBALi = nu_virt_to_dma(in_data);
  cfg_reg->wBALo = nu_virt_to_dma(res_data);

  cfg_reg->wOpM = flying_mode << 8;
  nu_ppe_decide_dma_config_trivial(cfg, res_metrics, cfg_reg);
}

CubeMetrics* in_mpe_metrics;
WarrMetrics* warr_metrics;
//CubeMetrics* res_mpe_metrics;
ConfigMPE cfg_mpe;

CubeMetrics* in_ppe_metrics;
CubeMetrics* res_ppe_metrics;
ConfigPPE    cfg_ppe;
ConfigREGPPE  cfg_reg_ppe = {0};

void* in_mpe_data = NULL;
void* in_warr = NULL;

void* in_ppe_data = NULL;
void* res_ppe_data = NULL;
void* ppe_etalon = NULL;

int main() {
  int res = 0;
  // iteration 0 is for mpe and vpe
  // iteration 1 is for ppe

  char* fn_in_base          = "in_file_tag_"        ;
  char* fn_warr_base        = "warr_file_tag_"      ;
  char* fn_cfg_mpe_base     = "cfg_mpe_file_tag_"   ;
  char* fn_cfg_ppe_base     = "cfg_ppe_file_tag_"   ;
  char* fn_in_metrics_base  = "metrics_in_tag_"     ;
  char* fn_warr_metrics_base= "metrics_warr_tag_"   ;
  char* fn_res_metrics_base = "metrics_etalon_tag_" ;
  char* fn_etalon_base      = "etalon_file_tag_"    ;

  char  fn_in[32];
  char  fn_warr[32];
  char  fn_cfg_mpe[32];
  char  fn_cfg_ppe[32];
  char  fn_in_metrics[32];
  char  fn_warr_metrics[32];
  char  fn_res_metrics[32];
  char  fn_etalon[32];

  int heap_id = nu_get_add_heap_id();

// Iteration 1
  fn_base_it_nmb(fn_in, fn_in_base, 1);
  fn_base_it_nmb(fn_cfg_ppe, fn_cfg_ppe_base, 1);
  fn_base_it_nmb(fn_in_metrics, fn_in_metrics_base, 1);
  fn_base_it_nmb(fn_res_metrics, fn_res_metrics_base, 1);
  fn_base_it_nmb(fn_etalon, fn_etalon_base, 1);

  nu_ppe_load_cfg_by_tag(heap_id, &cfg_ppe, fn_cfg_ppe);

  in_ppe_metrics  = nu_load_cube_metrics(heap_id, fn_in_metrics);
  res_ppe_metrics = nu_load_cube_metrics(heap_id, fn_res_metrics);

  in_ppe_data = nu_load_cube(heap_id, fn_in, in_ppe_metrics);
  res_ppe_data = nu_ppe_malloc_res(heap_id, res_ppe_metrics);;

  ppe_etalon  = nu_load_cube (heap_id, fn_etalon, res_ppe_metrics);

  uint32_t lbs = 0x0;
  #ifdef LIN
    lbs = 0x0;
  #endif
  #ifdef BOX
    lbs = 0x1;
  #endif
  #ifdef SPL
    lbs = 0x2;
  #endif

  uint32_t mv = 0x0; // MEMtoPPE

  uint32_t flying_mode = ((lbs&0x3)<<1) | (mv&0x1);

  nu_ppe_decide_dma_config(
      &cfg_ppe,
      res_ppe_metrics,
      &cfg_reg_ppe,
      flying_mode,
      in_ppe_data,
      res_ppe_data);

  nu_ppe_setup_reg(MY_PPE_RDMA_BASE, MY_PPE_REGS_BASE, &cfg_reg_ppe);
    
  cfg_reg_ppe.wOpEn  = 0x1;
  nu_ppe_wdma_run(MY_PPE_REGS_BASE, &cfg_reg_ppe); // wdma start
  
  cfg_reg_ppe.rOpEn  = 0x1;
  nu_ppe_rdma_run(MY_PPE_RDMA_BASE, &cfg_reg_ppe); // rdma start

// Iteration 0
  fn_base_it_nmb(fn_in, fn_in_base, 0);
  fn_base_it_nmb(fn_warr, fn_warr_base, 0);
  fn_base_it_nmb(fn_cfg_mpe, fn_cfg_mpe_base, 0);
  fn_base_it_nmb(fn_in_metrics, fn_in_metrics_base, 0);
  fn_base_it_nmb(fn_warr_metrics, fn_warr_metrics_base, 0);
  fn_base_it_nmb(fn_res_metrics, fn_res_metrics_base, 0);
  
  nu_mpe_load_cfg_by_tag(heap_id, &cfg_mpe, fn_cfg_mpe);

  in_mpe_metrics  = nu_load_cube_metrics(heap_id, fn_in_metrics);
  warr_metrics = nu_load_warr_metrics (heap_id, fn_warr_metrics);

  in_mpe_data = nu_load_cube(heap_id, fn_in, in_mpe_metrics);
  in_warr = nu_load_warr(heap_id, fn_warr, warr_metrics);

  void* mpe_cfg_lut = nu_mpe_load_cfg_lut(heap_id);
  if(mpe_cfg_lut == NULL) return -1;

  if(nu_mpe_decide_dma_config(
      &cfg_mpe,
      in_mpe_metrics,
      warr_metrics,
      in_mpe_data,
      in_warr,
      mpe_cfg_lut )!=0) return -1;
  
  //nu_mpe_print_config(&cfg_mpe);
  nu_mpe_setup(MY_MPE_REGS_BASE, &cfg_mpe);
  nu_mpe_run(MY_MPE_REGS_BASE, &cfg_mpe);

  nu_ppe_wait_complete(MY_PPE_REGS_BASE);
  return 0;
  while (1) {};
}
