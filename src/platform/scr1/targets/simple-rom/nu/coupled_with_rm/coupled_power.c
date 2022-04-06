#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>

#include <platform/devices.h> 

#include <devices/rcm_na/nu_lib.h> 
#include <devices/rcm_na/nu_test_lib.h> 
#include <devices/rcm_na/nu_test_macro.h> 

NPETestDescriptor test_desc;

int nu_mpe_decide_dma_config(
  ConfigMPE* cfg, 
  CubeMetrics* cube_metrics, 
  WarrMetrics* warr_metrics,
  void* cube,
  void* warr,
  void* cfg_lut
) {
  cfg->dma_d_config.rdma.BFCA  = nu_virt_to_dma(cube); // Data Base Address
  rumboot_printf("start1.1\n");

  cfg->dma_w_config.rdma.BFCA  = nu_virt_to_dma(warr); // Weights Base Address
  rumboot_printf("start1.2\n");
  //if(nu_mpe_look_up_dma_config(cfg,cfg_lut)!=0) return -1; // Search For The MPE DMA Configuration (That Is Predefined In cfg_lut Table)
  rumboot_printf("start1.3\n");
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
ConfigMPE* cfg_mpe;

CubeMetrics* in_ppe_metrics;
CubeMetrics* res_ppe_metrics;
ConfigPPE    cfg_ppe;


void* in_mpe_data = NULL;
void* in_warr  = NULL;



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

// Iteration 0
  rumboot_printf("start00\n");
  fn_base_it_nmb(fn_in, fn_in_base, 0);
  rumboot_printf("start01\n");
  fn_base_it_nmb(fn_warr, fn_warr_base, 0);
  rumboot_printf("start02\n");
  fn_base_it_nmb(fn_cfg_mpe, fn_cfg_mpe_base, 0);
  rumboot_printf("start03\n");
  fn_base_it_nmb(fn_in_metrics, fn_in_metrics_base, 0);
  rumboot_printf("start04\n");
  fn_base_it_nmb(fn_warr_metrics, fn_warr_metrics_base, 0);
  rumboot_printf("start05\n");
  fn_base_it_nmb(fn_res_metrics, fn_res_metrics_base, 0);
  rumboot_printf("start06\n");
  
  nu_mpe_load_cfg_by_tag(heap_id, cfg_mpe, fn_cfg_mpe);
  rumboot_printf("start07\n");

  in_mpe_metrics  = nu_load_cube_metrics(heap_id, fn_in_metrics);
  rumboot_printf("start08\n");
  warr_metrics = nu_load_warr_metrics (heap_id, fn_warr_metrics);
  rumboot_printf("start09\n");

  in_mpe_data = nu_load_cube(heap_id, fn_in, in_mpe_metrics);
  rumboot_printf("start010\n");
  in_warr = nu_load_warr(heap_id, fn_warr, warr_metrics);
  rumboot_printf("start011\n");
  
  rumboot_printf("start0\n");

  nu_npe_init_test_desc(&test_desc);
  
  rumboot_printf("start1\n");

  if(nu_mpe_decide_dma_config(
      cfg_mpe,
      in_mpe_metrics,
      warr_metrics,
      in_mpe_data,
      in_warr,
      test_desc.mpe_cfg_lut )!=0) return -1;
  
  rumboot_printf("start2\n");
  nu_mpe_print_config(cfg_mpe);
  nu_mpe_setup(MY_MPE_REGS_BASE, cfg_mpe);
  nu_mpe_run(MY_MPE_REGS_BASE, cfg_mpe);
}
//  nu_ppe_load_cfg_by_tag(heap_id, &cfg, fn_cfg);





//  #ifdef LIN
//    lbs = 0x0;
//  #endif
//  #ifdef BOX
//    lbs = 0x1;
//  #endif
//  #ifdef SPL
//    lbs = 0x2;
//  #endif

//  mv = 0x0; // MEMtoPPE


//  flying_mode = ((lbs&0x3)<<1) | (mv&0x1);

//  nu_ppe_decide_dma_config(
//    cfg_ppe,
//    ppe_res_metrics,
//    cfg_reg_ppe,
//    flying_mode,
//    ppe_in_data,
//    ppe_res_data
//  );
//  
//  iteration_desc.cfg_reg->rOpEn  = 0x0;
//  iteration_desc.cfg_reg->wOpEn  = 0x0;
//  nu_ppe_setup_reg(MY_PPE_RDMA_BASE, MY_PPE_REGS_BASE, cfg_ppe);
//  
//  iteration_desc.cfg_reg->wOpEn  = 0x1;
//  nu_ppe_wdma_run(MY_PPE_REGS_BASE, cfg_reg_ppe); // wdma start
//  
//  iteration_desc.cfg_reg->rOpEn  = 0x1;
//  nu_ppe_rdma_run(MY_PPE_RDMA_BASE, cfg_reg_ppe); // rdma start
//}


