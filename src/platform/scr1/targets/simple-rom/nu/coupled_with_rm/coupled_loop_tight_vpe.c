
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>

#include <platform/devices.h> 

#include <devices/rcm_na/nu_lib.h> 
#include <devices/rcm_na/nu_test_lib.h> 
#include <devices/rcm_na/nu_test_macro.h> 

#include "devices/rcm_na/nu_vpe_file_tags.h"

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
  bool last_turn
) {
  uint8_t axi_len;
  
  rumboot_printf("nu_vpe_decide_dma_config \n");
  
  cfg->src_flying = cfg->in_data_type == DataTypeExt_Int32 || cfg->in_data_type == DataTypeExt_Fp32 ? Enable_En : Enable_NotEn;
#ifdef FORCE_VPE_WDMA_EN
  cfg->dst_flying = Enable_NotEn;
#else
  cfg->dst_flying = cfg->out_data_type == DataType_Int8 ? Enable_NotEn : Enable_En;
#endif
  cfg->trace_mode = TraceMode_MPE;

  cfg->src_rdma_config.dma_bsize=0;
  cfg->src_rdma_config.dma_bstride=0;
  cfg->wdma_config.dma_bsize=0;
  cfg->wdma_config.dma_bstride=0;
  
#ifdef AXI_LEN
    axi_len = AxiLen;
#else 
    axi_len = 15;
#endif

  nu_vpe_decide_dma_config_trivial(cfg,in_metrics);
  
  if(last_turn) 
    cfg->mark = Enable_En;
  
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
   char* fn_in_base          			= "in_file_tag_"        ;
  char* fn_cfg_base         			= "cfg_file_tag_"   ;
  char* fn_in_metrics_base  			= "metrics_in_tag_"     ;
  char* fn_res_metrics_base 			= "metrics_etalon_tag_" ;
  char* fn_etalon_base      			= "etalon_file_tag_"    ;
  
  char* fn_metrics_op0_cube_base    	= "metrics_op0_cube_tag_" ;
  char* fn_metrics_op0_vec_base     	= "metrics_op0_vec_tag_" ;
  char* fn_op0_cube_file_base   		= "op0_cube_file_tag_" ;
  char* fn_op0_vec_file_base    		= "op0_vec_file_tag_" ;
  
  char* fn_metrics_op1_cube_base    	= "metrics_op1_cube_tag_" ;
  char* fn_metrics_op1_vec_base     	= "metrics_op1_vec_tag_" ;
  char* fn_op1_cube_file_base   		= "op1_cube_file_tag_" ;
  char* fn_op1_vec_file_base    		= "op1_vec_file_tag_" ;
  
  char* fn_metrics_op2_cube_base    	= "metrics_op2_cube_tag_" ;
  char* fn_metrics_op2_vec_base     	= "metrics_op2_vec_tag_" ;
  char* fn_op2_cube_file_base   		= "op2_cube_file_tag_" ;
  char* fn_op2_vec_file_base    		= "op2_vec_file_tag_" ;
  
  char* fn_metrics_lut1_file_base   	= "metrics_lut1_file_tag_" ; 
  char* fn_metrics_lut2_file_base   	= "metrics_lut2_file_tag_" ;
  char* fn_lut1_file_base   			= "lut1_file_tag_" ; 
  char* fn_lut2_file_base   			= "lut2_file_tag_" ;
 
  char* fn_status_regs_file_base   		= "status_regs_file_tag_" ;
 
  char  fn_in[32];
  char  fn_cfg[32];
  char  fn_in_metrics[32];
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

  rumboot_printf("Hello \n");
  
  heap_id = nu_get_heap_id();
  
  rumboot_platform_request_file("num_iterations_file_tag",(uintptr_t) &iterations);
  rumboot_printf("Number of iterations %d\n",iterations);
  
#if DUT_IS_NPE
  na_cu_set_units_direct_mode(NPE_BASE+NA_CU_REGS_BASE, NA_CU_VPE_UNIT_MODE);
#endif
  
  for(i=0;i<iterations;i++) {
      rumboot_printf("Starting iteration %d\n",i);
	fn_base_it_nmb(fn_in, fn_in_base, i);
    fn_base_it_nmb(fn_cfg, fn_cfg_base, i);
    fn_base_it_nmb(fn_in_metrics, fn_in_metrics_base, i);
    fn_base_it_nmb(fn_res_metrics, fn_res_metrics_base, i);
    fn_base_it_nmb(fn_etalon, fn_etalon_base, i);
	
	
	fn_base_it_nmb(fn_metrics_op0_cube, fn_metrics_op0_cube_base, i);
	fn_base_it_nmb(fn_metrics_op0_vec, fn_metrics_op0_vec_base, i);
	fn_base_it_nmb(fn_op0_cube_file, fn_op0_cube_file_base, i);
	fn_base_it_nmb(fn_op0_vec_file, fn_op0_vec_file_base, i);	
	
	
	fn_base_it_nmb(fn_metrics_op1_cube, fn_metrics_op1_cube_base, i);
	fn_base_it_nmb(fn_metrics_op1_vec, fn_metrics_op1_vec_base, i);
	fn_base_it_nmb(fn_op1_cube_file, fn_op1_cube_file_base, i);
	fn_base_it_nmb(fn_op1_vec_file, fn_op1_vec_file_base, i);	

	fn_base_it_nmb(fn_metrics_op2_cube, fn_metrics_op2_cube_base, i);
	fn_base_it_nmb(fn_metrics_op2_vec, fn_metrics_op2_vec_base, i);
	fn_base_it_nmb(fn_op2_cube_file, fn_op2_cube_file_base, i);
	fn_base_it_nmb(fn_op2_vec_file, fn_op2_vec_file_base, i);	
	
	fn_base_it_nmb(fn_metrics_lut1_file, fn_metrics_lut1_file_base, i);
	fn_base_it_nmb(fn_metrics_lut2_file, fn_metrics_lut2_file_base, i);
	fn_base_it_nmb(fn_lut1_file, fn_lut1_file_base, i);
	fn_base_it_nmb(fn_lut2_file, fn_lut2_file_base, i);
	
	fn_base_it_nmb(fn_status_regs_file, fn_status_regs_file_base, i); 
	
     if(nu_vpe_load_cfg_by_tag(heap_id, &cfg, fn_cfg) != 0) return -1;
    
    in_metrics = nu_load_cube_metrics(heap_id,fn_in_metrics);
    if(in_metrics == NULL) return -1;
    
    res_metrics= nu_load_cube_metrics(heap_id,fn_res_metrics);
    if(res_metrics == NULL) return -1;

    in_data = nu_load_cube_misaligned(heap_id,fn_in,in_metrics,IntMisalign);
    if(in_data == NULL) return -1;
    
    res_data = nu_vpe_malloc_res(heap_id, res_metrics);
    if(res_data == NULL) return -1;
      
        // Load OP0-OP2 Operands If Needed
    if(cfg.op0_en==Enable_En) {  
  op0 = nu_vpe_load_op01_misaligned_by_tags(heap_id,&cfg.op0_config,fn_metrics_op0_cube,fn_metrics_op0_vec,fn_op0_cube_file,fn_op0_vec_file,IntMisalign);
}
    else op0 = NULL;
    if(cfg.op1_en==Enable_En) {
	   op1 = nu_vpe_load_op01_misaligned_by_tags(heap_id,&cfg.op1_config,fn_metrics_op1_cube,fn_metrics_op1_vec,fn_op1_cube_file,fn_op1_vec_file,IntMisalign);
 }
    else op1 = NULL;
    if(cfg.op2_en==Enable_En) {
		op2 = nu_vpe_load_op2_misaligned_by_tags(heap_id,&cfg.op2_config,fn_metrics_op2_cube,fn_metrics_op2_vec,fn_op2_cube_file,fn_op2_vec_file,IntMisalign);
 }
    else op2 = NULL;
    
      // Load LUTs If Needed
    if(cfg.op2_config.lut_en == Enable_En) {
      lut1_metrics = nu_load_vec_metrics(heap_id,fn_metrics_lut1_file);
      lut2_metrics = nu_load_vec_metrics(heap_id,fn_metrics_lut2_file);
      lut1 = nu_load_vec(heap_id,fn_lut1_file,lut1_metrics);
      lut2 = nu_load_vec(heap_id,fn_lut2_file,lut2_metrics);
      nu_vpe_load_lut(MY_VPE_REGS_BASE,lut1,lut2);
    }
      
      etalon = nu_load_cube(heap_id,fn_etalon,res_metrics);
      if(etalon == NULL) return -1;
      
      //print_in_data(in_data,in_size);
      
      // if(nu_vpe_load_status_regs_by_tag(heap_id,&status_regs_etalon,status_regs_file_tag[i]) != 0) return -1;
      
      nu_vpe_decide_dma_config(&cfg,in_metrics,in_data,op0,op1,op2,res_metrics,res_data,i==iterations-1);
      
      // nu_vpe_print_config(&cfg);
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
