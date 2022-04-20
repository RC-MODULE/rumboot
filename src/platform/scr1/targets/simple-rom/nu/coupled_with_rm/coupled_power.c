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

  cfg->src_flying = cfg->in_data_type == DataTypeExt_Int32 || cfg->in_data_type == DataTypeExt_Fp32 ? Enable_En : Enable_NotEn;
  cfg->dst_flying = Enable_NotEn;

#ifdef VPE_TraceMode_PPE
  cfg->trace_mode = TraceMode_PPE;
#else
  cfg->trace_mode = TraceMode_MPE;
#endif

  cfg->src_rdma_config.dma_bsize=0;
  cfg->src_rdma_config.dma_bstride=0;
  cfg->src_rdma_config.dma_bstride=0;
  cfg->op0_rdma_config.dma_bsize=0;
  cfg->op0_rdma_config.dma_bstride=0;
  cfg->op1_rdma_config.dma_bsize=0;
  cfg->op1_rdma_config.dma_bstride=0;
  cfg->op2_rdma_config.dma_bsize=0;
  cfg->op2_rdma_config.dma_bstride=0;
  cfg->wdma_config.dma_bsize=0;
  cfg->wdma_config.dma_bstride=0;
  cfg->depend_mask=0;

  nu_vpe_decide_dma_config_trivial(cfg,in_metrics);

  cfg->src_rdma_config.dma_baddr = nu_virt_to_dma(in_data);
  cfg->op0_rdma_config.dma_baddr = nu_virt_to_dma(op0);
  cfg->op1_rdma_config.dma_baddr = nu_virt_to_dma(op1);
  cfg->op2_rdma_config.dma_baddr = nu_virt_to_dma(op2);
  cfg->wdma_config.dma_baddr     = nu_virt_to_dma(res_data);

  cfg->src_rdma_config.dma_axi_len = axi_len;
  cfg->op0_rdma_config.dma_axi_len = axi_len;
  cfg->op1_rdma_config.dma_axi_len = axi_len;
  cfg->op2_rdma_config.dma_axi_len = axi_len;
  cfg->wdma_config.dma_axi_len     = axi_len;

}

CubeMetrics* in_mpe_metrics;
WarrMetrics* warr_metrics;
//CubeMetrics* res_mpe_metrics;
ConfigMPE cfg_mpe;

CubeMetrics* in_ppe_metrics;
CubeMetrics* res_ppe_metrics;
ConfigPPE    cfg_ppe;
ConfigREGPPE cfg_reg_ppe = {0};

ConfigVPE cfg_vpe;
CubeMetrics* in_vpe_metrics;
CubeMetrics* res_vpe_metrics;

VectorMetrics* lut1_metrics;
VectorMetrics* lut2_metrics;

StatusRegs status_regs_etalon;

void* in_mpe_data = NULL;
void* in_warr = NULL;

void* in_ppe_data = NULL;
void* res_ppe_data = NULL;
void* ppe_etalon = NULL;

void* in_vpe_data = NULL;
void* res_vpe_data = NULL;
void* vpe_etalon = NULL;
void* op0 = NULL;
void* op1 = NULL;
void* op2 = NULL;
void* lut1 = NULL;
void* lut2 = NULL;

int main() {
  // iteration 0 is for mpe
  // iteration 1 is for ppe
  // iteration 2 is for vpe

  char* fn_in_base           = "in_file_tag_"        ;
  char* fn_warr_base         = "warr_file_tag_"      ;
  char* fn_cfg_mpe_base      = "cfg_mpe_file_tag_"   ;
  char* fn_cfg_vpe_base      = "cfg_file_tag_"       ;
  char* fn_cfg_ppe_base      = "cfg_ppe_file_tag_"   ;
  char* fn_in_metrics_base   = "metrics_in_tag_"     ;
  char* fn_warr_metrics_base = "metrics_warr_tag_"   ;
  char* fn_res_metrics_base  = "metrics_etalon_tag_" ;
  char* fn_etalon_base       = "etalon_file_tag_"    ;

  char* fn_metrics_op0_cube_base = "metrics_op0_cube_tag_" ;
  char* fn_metrics_op0_vec_base  = "metrics_op0_vec_tag_"  ;
  char* fn_op0_cube_file_base    = "op0_cube_file_tag_"    ;
  char* fn_op0_vec_file_base     = "op0_vec_file_tag_"     ;

  char* fn_metrics_op1_cube_base = "metrics_op1_cube_tag_" ;
  char* fn_metrics_op1_vec_base  = "metrics_op1_vec_tag_"  ;
  char* fn_op1_cube_file_base    = "op1_cube_file_tag_"    ;
  char* fn_op1_vec_file_base     = "op1_vec_file_tag_"     ;

  char* fn_metrics_op2_cube_base = "metrics_op2_cube_tag_" ;
  char* fn_metrics_op2_vec_base  = "metrics_op2_vec_tag_"  ;
  char* fn_op2_cube_file_base    = "op2_cube_file_tag_"    ;
  char* fn_op2_vec_file_base     = "op2_vec_file_tag_"     ;

  char* fn_metrics_lut1_file_base = "metrics_lut1_file_tag_" ;
  char* fn_metrics_lut2_file_base = "metrics_lut2_file_tag_" ;
  char* fn_lut1_file_base         = "lut1_file_tag_" ;
  char* fn_lut2_file_base         = "lut2_file_tag_" ;

  char* fn_status_regs_file_base = "status_regs_file_tag_" ;

  char  fn_in[32];
  char  fn_warr[32];
  char  fn_cfg_mpe[32];
  char  fn_cfg_vpe[32];
  char  fn_cfg_ppe[32];
  char  fn_in_metrics[32];
  char  fn_warr_metrics[32];
  char  fn_res_metrics[32];
  char  fn_etalon[32];

  char  fn_metrics_op0_cube[32];
  char  fn_metrics_op0_vec[32];
  char  fn_op0_cube_file[32];
  char  fn_op0_vec_file[32];

  char  fn_metrics_op1_cube[32];
  char  fn_metrics_op1_vec[32];
  char  fn_op1_cube_file[32];
  char  fn_op1_vec_file[32];

  char  fn_metrics_op2_cube[32];
  char  fn_metrics_op2_vec[32];
  char  fn_op2_cube_file[32];
  char  fn_op2_vec_file[32];

  char  fn_metrics_lut1_file[32];
  char  fn_metrics_lut2_file[32];
  char  fn_lut1_file[32];
  char  fn_lut2_file[32];

  char  fn_status_regs_file[32];

  int heap0_id = nu_get_heap_id();
  int heap1_id = nu_get_add_heap_id();

// Iteration 1 (Setup only)
  fn_base_it_nmb(fn_in, fn_in_base, 1);
  fn_base_it_nmb(fn_cfg_ppe, fn_cfg_ppe_base, 1);
  fn_base_it_nmb(fn_in_metrics, fn_in_metrics_base, 1);
  fn_base_it_nmb(fn_res_metrics, fn_res_metrics_base, 1);
  fn_base_it_nmb(fn_etalon, fn_etalon_base, 1);

  nu_ppe_load_cfg_by_tag(heap0_id, &cfg_ppe, fn_cfg_ppe);

  in_ppe_metrics  = nu_load_cube_metrics(heap0_id, fn_in_metrics);
  res_ppe_metrics = nu_load_cube_metrics(heap0_id, fn_res_metrics);

  in_ppe_data = nu_load_cube(heap0_id, fn_in, in_ppe_metrics);
  res_ppe_data = nu_ppe_malloc_res(heap0_id, res_ppe_metrics);;

  ppe_etalon  = nu_load_cube (heap0_id, fn_etalon, res_ppe_metrics);

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
  cfg_reg_ppe.rOpEn  = 0x1;

// Iteration 0 (Setup only)
  fn_base_it_nmb(fn_in, fn_in_base, 0);
  fn_base_it_nmb(fn_warr, fn_warr_base, 0);
  fn_base_it_nmb(fn_cfg_mpe, fn_cfg_mpe_base, 0);
  fn_base_it_nmb(fn_in_metrics, fn_in_metrics_base, 0);
  fn_base_it_nmb(fn_warr_metrics, fn_warr_metrics_base, 0);
  fn_base_it_nmb(fn_res_metrics, fn_res_metrics_base, 0);
  
  nu_mpe_load_cfg_by_tag(heap0_id, &cfg_mpe, fn_cfg_mpe);

  in_mpe_metrics  = nu_load_cube_metrics(heap0_id, fn_in_metrics);
  warr_metrics = nu_load_warr_metrics (heap0_id, fn_warr_metrics);

  in_mpe_data = nu_load_cube(heap0_id, fn_in, in_mpe_metrics);
  in_warr = nu_load_warr(heap0_id, fn_warr, warr_metrics);

  void* mpe_cfg_lut = nu_mpe_load_cfg_lut(heap0_id);
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

// Iteration 2
  fn_base_it_nmb(fn_in, fn_in_base, 2);
  fn_base_it_nmb(fn_cfg_vpe, fn_cfg_vpe_base, 2);
  fn_base_it_nmb(fn_in_metrics, fn_in_metrics_base, 2);
  fn_base_it_nmb(fn_res_metrics, fn_res_metrics_base, 2);
  fn_base_it_nmb(fn_etalon, fn_etalon_base, 2);

  fn_base_it_nmb(fn_metrics_op0_cube, fn_metrics_op0_cube_base, 2);
  fn_base_it_nmb(fn_metrics_op0_vec, fn_metrics_op0_vec_base, 2);
  fn_base_it_nmb(fn_op0_cube_file, fn_op0_cube_file_base, 2);
  fn_base_it_nmb(fn_op0_vec_file, fn_op0_vec_file_base, 2);

  fn_base_it_nmb(fn_metrics_op1_cube, fn_metrics_op1_cube_base, 2);
  fn_base_it_nmb(fn_metrics_op1_vec, fn_metrics_op1_vec_base, 2);
  fn_base_it_nmb(fn_op1_cube_file, fn_op1_cube_file_base, 2);
  fn_base_it_nmb(fn_op1_vec_file, fn_op1_vec_file_base, 2);

  fn_base_it_nmb(fn_metrics_op2_cube, fn_metrics_op2_cube_base, 2);
  fn_base_it_nmb(fn_metrics_op2_vec, fn_metrics_op2_vec_base, 2);
  fn_base_it_nmb(fn_op2_cube_file, fn_op2_cube_file_base, 2);
  fn_base_it_nmb(fn_op2_vec_file, fn_op2_vec_file_base, 2);

  fn_base_it_nmb(fn_metrics_lut1_file, fn_metrics_lut1_file_base, 2);
  fn_base_it_nmb(fn_metrics_lut2_file, fn_metrics_lut2_file_base, 2);
  fn_base_it_nmb(fn_lut1_file, fn_lut1_file_base, 2);
  fn_base_it_nmb(fn_lut2_file, fn_lut2_file_base, 2);

  fn_base_it_nmb(fn_status_regs_file, fn_status_regs_file_base, 2);

  if(nu_vpe_load_cfg_by_tag(heap1_id, &cfg_vpe, fn_cfg_vpe) != 0) return -1;

  in_vpe_metrics = nu_load_cube_metrics(heap1_id,fn_in_metrics);
  if(in_vpe_metrics == NULL) return -1;

  res_vpe_metrics= nu_load_cube_metrics(heap1_id,fn_res_metrics);
  if(res_vpe_metrics == NULL) return -1;

  in_vpe_data = nu_load_cube_misaligned(heap1_id,fn_in,in_vpe_metrics,0);
  if(in_vpe_data == NULL) return -1;

  res_vpe_data = nu_vpe_malloc_res(heap1_id, res_vpe_metrics);
  if(res_vpe_data == NULL) return -1;

  // Load OP0-OP2 Operands If Needed
  if(cfg_vpe.op0_en==Enable_En) op0 = nu_vpe_load_op01_misaligned_by_tags(heap1_id,&cfg_vpe.op0_config,fn_metrics_op0_cube,fn_metrics_op0_vec,fn_op0_cube_file,fn_op0_vec_file,0);
  else                          op0 = NULL;

  if(cfg_vpe.op1_en==Enable_En) op1 = nu_vpe_load_op01_misaligned_by_tags(heap1_id,&cfg_vpe.op1_config,fn_metrics_op1_cube,fn_metrics_op1_vec,fn_op1_cube_file,fn_op1_vec_file,0);
  else                          op1 = NULL;

  if(cfg_vpe.op2_en==Enable_En) op2 = nu_vpe_load_op2_misaligned_by_tags(heap1_id,&cfg_vpe.op2_config,fn_metrics_op2_cube,fn_metrics_op2_vec,fn_op2_cube_file,fn_op2_vec_file,0);
  else                          op2 = NULL;

  // Load LUTs If Needed
  if(cfg_vpe.op2_config.lut_en == Enable_En) {
    lut1_metrics = nu_load_vec_metrics(heap1_id,fn_metrics_lut1_file);
    lut2_metrics = nu_load_vec_metrics(heap1_id,fn_metrics_lut2_file);
    lut1 = nu_load_vec(heap1_id,fn_lut1_file,lut1_metrics);
    lut2 = nu_load_vec(heap1_id,fn_lut2_file,lut2_metrics);
    nu_vpe_load_lut(MY_VPE_REGS_BASE,lut1,lut2);
  }

  vpe_etalon = nu_load_cube(heap1_id,fn_etalon,res_vpe_metrics);
  if(vpe_etalon == NULL) return -1;

  int axi_len = 15;

  if(nu_vpe_load_status_regs_by_tag(heap1_id,&status_regs_etalon,fn_status_regs_file/*status_regs_file_tag[i]*/) != 0) return -1;

  nu_vpe_decide_dma_config(&cfg_vpe,in_vpe_metrics,axi_len,in_vpe_data,op0,op1,op2,res_vpe_metrics,res_vpe_data);

  //nu_vpe_print_config(&cfg_vpe);
  //nu_vpe_print_status_regs_etalon(&status_regs_etalon);

  nu_vpe_setup(MY_VPE_REGS_BASE, &cfg_vpe);

  // Setup Main Channel DMAs if Required
  if(cfg_vpe.src_rdma_config.dma_en == Enable_NotEn)
    nu_vpe_config_rd_main_channel(NU_CPDMAC_ASM_BASE,in_vpe_data,in_vpe_metrics->s);
  if(cfg_vpe.wdma_config.dma_en == Enable_NotEn)
    nu_vpe_config_wr_main_channel(NU_CPDMAC_ASM_BASE,res_vpe_data,res_vpe_metrics->s);

  rumboot_printf("Running VPE..\n");

  // Invoke Required DMA Channels
  if(cfg_vpe.src_rdma_config.dma_en == Enable_NotEn)
    nu_vpe_run_rd_main_channel(NU_CPDMAC_ASM_BASE);
  if(cfg_vpe.wdma_config.dma_en == Enable_NotEn)
    nu_vpe_run_wr_main_channel(NU_CPDMAC_ASM_BASE);

// Iteration 1 start
  nu_ppe_wdma_run(MY_PPE_REGS_BASE, &cfg_reg_ppe); // wdma start
  nu_ppe_rdma_run(MY_PPE_RDMA_BASE, &cfg_reg_ppe); // rdma start

// Iteration 0 start
  nu_mpe_run(MY_MPE_REGS_BASE, &cfg_mpe);

// Iteration 3 start
  nu_vpe_run(MY_VPE_REGS_BASE, &cfg_vpe);

  nu_vpe_wait(MY_VPE_REGS_BASE, &cfg_vpe);
  nu_ppe_wait_complete(MY_PPE_REGS_BASE);

  return 0;
  while (1) {};
}
