
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
  void*res_data
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
 
//#ifdef AXI_LEN
//    axi_len = AxiLen;
//#else 
 //   axi_len = 15;
//#endif
  
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

ConfigVPE cfg;

CubeMetrics* in_metrics;
CubeMetrics* res_metrics;

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
  uint32_t num_cycles;
  uint32_t num_vectors;
  uint32_t productivity_x1000;
  uint32_t productivity_x100;
  uint32_t productivity_frac;  
  
  char* fn_in_base          			= "in_file_tag_"        ;
  char* fn_cfg_base         			= "cfg_file_tag_"   	;
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

  rumboot_printf("Hello\n");
  
  heap_id = nu_get_heap_id();
  
  rumboot_platform_request_file_ex("num_iterations_file_tag",(uintptr_t) &iterations,sizeof(iterations));
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
    
    in_data = nu_load_cube(heap_id,fn_in,in_metrics);
    if(in_data == NULL) return -1;
    res_data = nu_vpe_malloc_res(heap_id, res_metrics);
    if(res_data == NULL) return -1;
    
      // Load OP0-OP2 Operands If Needed
    if(cfg.op0_en==Enable_En) {
   op0 = nu_vpe_load_op01_by_tags(heap_id,&cfg.op0_config,fn_metrics_op0_cube,fn_metrics_op0_vec,fn_op0_cube_file,fn_op0_vec_file);
    }
    else op0 = NULL;
    if(cfg.op1_en==Enable_En) {
 	   op1 = nu_vpe_load_op01_by_tags(heap_id,&cfg.op1_config,fn_metrics_op1_cube,fn_metrics_op1_vec,fn_op1_cube_file,fn_op1_vec_file);
    }
    else op1 = NULL;
    if(cfg.op2_en==Enable_En) {
 	   op2 = nu_vpe_load_op2_by_tags(heap_id,&cfg.op2_config,fn_metrics_op2_cube,fn_metrics_op2_vec,fn_op2_cube_file,fn_op2_vec_file);
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
    
    nu_vpe_decide_dma_config(&cfg,in_metrics,in_data,op0,op1,op2,res_metrics,res_data);
    //nu_vpe_print_config(&cfg);
    //nu_print_config_dma(&cfg.src_rdma_config,"src_rdma_config");
    //nu_print_config_dma(&cfg.op0_rdma_config,"op0_rdma_config");
    //nu_print_config_dma(&cfg.op1_rdma_config,"op1_rdma_config");
    //nu_print_config_dma(&cfg.op2_rdma_config,"op2_rdma_config");
    //nu_print_config_dma(&cfg.wdma_config,"wdma_config");
    
    start[i] = rumboot_platform_get_uptime();
    
    nu_vpe_setup(MY_VPE_REGS_BASE, &cfg);
    
    
    rumboot_printf("Running DMA..\n");
    
    
    nu_vpe_run(MY_VPE_REGS_BASE, &cfg);     // To Invoke Or Not To Invoke Internal DMA Channel - Decide inside nu_vpe_run
    
    nu_vpe_wait(MY_VPE_REGS_BASE, &cfg);
    
    end[i] = rumboot_platform_get_uptime();
    delta[i] = end[i] - start[0];
   // num_cycles = delta * 1000 / 650; // delta in microseconds, 1 DUT cycle is 650 nanoseconds
   // num_vectors = in_metrics->H * in_metrics->W * in_metrics->C / 16;
   // productivity_x1000 =  num_vectors*1000 / num_cycles;
   // productivity_x100 = productivity_x1000 / 10;
   // productivity_frac = productivity_x1000 % 10;
    
    rumboot_printf("Comparing..\n");
    
    if(nu_bitwise_compare(res_data,etalon,res_metrics->s) == 0)
      rumboot_printf("Iteration %d PASSED\n",i);
    else {
      rumboot_printf("Test FAILED at iteration %d\n",i);
      return 1;
    }
    
//    rumboot_printf("Productivity: %d.%d Percent  num_vectors = %d, num_cycles = %d \n",
//                   productivity_x100,productivity_frac,num_vectors,num_cycles);
  }
  rumboot_printf("First cube time %d us, both cube time %d us \n",delta[0],delta[1]);
  
  return 0;
  
}
