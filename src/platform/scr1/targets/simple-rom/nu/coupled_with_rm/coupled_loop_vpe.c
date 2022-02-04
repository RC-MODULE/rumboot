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
#ifdef FORCE_VPE_WDMA_EN
  cfg->dst_flying = Enable_NotEn;
#else
  cfg->dst_flying = cfg->out_data_type == DataType_Int8 ? Enable_NotEn : Enable_En;
#endif

#ifdef VPE_TraceMode_PPE
  cfg->trace_mode = TraceMode_PPE;
#else
  cfg->trace_mode = TraceMode_MPE;
#endif

  cfg->src_rdma_config.dma_bsize=0;
  cfg->src_rdma_config.dma_bstride=0;
  cfg->wdma_config.dma_bsize=0;
  cfg->wdma_config.dma_bstride=0;
  
  nu_vpe_decide_dma_config_trivial(cfg,in_metrics);
  
  cfg->src_rdma_config.dma_baddr = rumboot_virt_to_dma(in_data);
  cfg->op0_rdma_config.dma_baddr = rumboot_virt_to_dma(op0);
  cfg->op1_rdma_config.dma_baddr = rumboot_virt_to_dma(op1);
  cfg->op2_rdma_config.dma_baddr = rumboot_virt_to_dma(op2);
  cfg->wdma_config.dma_baddr     = rumboot_virt_to_dma(res_data);

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

VPEStatusRegs status_regs_etalon;

int main() {
  int heap_id;
  int i;
  int iterations;
  
    // Time Measure
  uint32_t start;
  uint32_t end;
  uint32_t delta;
  uint32_t num_cycles;
  uint32_t num_vectors;
  uint32_t productivity_x1000;
  uint32_t misalign;
  uint8_t  axi_len;
  
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

#ifdef MISALIGN_EN
    misalign = IntMisalign;
#else
    misalign = 0;
#endif

#ifdef AXI_LEN
    axi_len = AxiLen;
#else 
    axi_len = 15;
#endif

 
  rumboot_printf("Hello\n");

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

    in_data = nu_load_cube_misaligned(heap_id,fn_in,in_metrics,misalign);
    if(in_data == NULL) return -1;
    
    res_data = nu_vpe_malloc_res(heap_id, res_metrics);
    if(res_data == NULL) return -1;
    
      // Load OP0-OP2 Operands If Needed
    if(cfg.op0_en==Enable_En) {  
  op0 = nu_vpe_load_op01_misaligned_by_tags(heap_id,&cfg.op0_config,fn_metrics_op0_cube,fn_metrics_op0_vec,fn_op0_cube_file,fn_op0_vec_file,misalign);
}
    else op0 = NULL;
    if(cfg.op1_en==Enable_En) {
	   op1 = nu_vpe_load_op01_misaligned_by_tags(heap_id,&cfg.op1_config,fn_metrics_op1_cube,fn_metrics_op1_vec,fn_op1_cube_file,fn_op1_vec_file,misalign);
 }
    else op1 = NULL;
    if(cfg.op2_en==Enable_En) {
		op2 = nu_vpe_load_op2_misaligned_by_tags(heap_id,&cfg.op2_config,fn_metrics_op2_cube,fn_metrics_op2_vec,fn_op2_cube_file,fn_op2_vec_file,misalign);
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
    
    if(nu_vpe_load_status_regs_by_tag(heap_id,&status_regs_etalon,fn_status_regs_file/*status_regs_file_tag[i]*/) != 0) return -1;

    nu_vpe_decide_dma_config(&cfg,in_metrics,axi_len,in_data,op0,op1,op2,res_metrics,res_data);
    nu_vpe_print_config(&cfg);
    nu_vpe_print_status_regs_etalon(&status_regs_etalon);
    
    nu_vpe_setup(MY_VPE_REGS_BASE, &cfg);
    
      // Setup Main Channel DMAs if Required
    if(cfg.src_rdma_config.dma_en == Enable_NotEn)
      nu_vpe_config_rd_main_channel(NU_CPDMAC_ASM_BASE,in_data,in_metrics->s);
    if(cfg.wdma_config.dma_en == Enable_NotEn)
      nu_vpe_config_wr_main_channel(NU_CPDMAC_ASM_BASE,res_data,res_metrics->s);
    
    rumboot_printf("Running DMA..\n");
    
      // Invoke Required DMA Channels
    if(cfg.src_rdma_config.dma_en == Enable_NotEn)
      nu_vpe_run_rd_main_channel(NU_CPDMAC_ASM_BASE);
    if(cfg.wdma_config.dma_en == Enable_NotEn)
      nu_vpe_run_wr_main_channel(NU_CPDMAC_ASM_BASE);
    
    start = rumboot_platform_get_uptime();
    
    nu_vpe_run(MY_VPE_REGS_BASE, &cfg);     // To Invoke Or Not To Invoke Internal DMA Channel - Decide inside nu_vpe_run
    
    nu_vpe_wait(MY_VPE_REGS_BASE, &cfg);
    
    end = rumboot_platform_get_uptime();
    
      // Finalize Required DMA Channels
    if(cfg.src_rdma_config.dma_en == Enable_NotEn)
      nu_vpe_wait_rd_main_channel_complete(NU_CPDMAC_ASM_BASE);
    if(cfg.wdma_config.dma_en == Enable_NotEn)
      nu_vpe_wait_wr_main_channel_complete(NU_CPDMAC_ASM_BASE);
    
    delta = end - start;
    num_cycles = delta * 1000 / 650; // delta in microseconds, 1 DUT cycle is 650 nanoseconds
    num_vectors = in_metrics->H * in_metrics->W * in_metrics->C / 16;
    productivity_x1000 =  num_vectors*1000 / num_cycles;
    
    rumboot_printf("Comparing..\n");
    
    if(nu_bitwise_compare(res_data,etalon,res_metrics->s) == 0)
      rumboot_printf("Iteration %d PASSED\n",i);
    else {
      rumboot_printf("Test FAILED at iteration %d\n",i);
      nu_vpe_interpret_mismatch(&cfg,res_data,etalon,in_data,op0,op1,op2,res_metrics->s,res_metrics->C);
      return 1;
    }
    
//     rumboot_printf("Productivity: %d.%d Percent  num_vectors = %d, start = %d, end = %d, delta = %d, num_cycles = %d \n",
//                    productivity_x100,productivity_frac,num_vectors,start,end,delta,num_cycles);
    rumboot_printf("Performance: 0.%d vectors/cycle num_vectors = %d, num_cycles = %d \n",
                   productivity_x1000,num_vectors,num_cycles);
    
    if(cfg.src_rdma_config.dma_cube_size_c%16 == 0) {
        if(nu_vpe_check_status_regs(MY_VPE_REGS_BASE, &status_regs_etalon) != 0) {
        rumboot_printf("Test FAILED Due to Status Reg Check at iteration %d\n",i);
        return -1;
        }
       else {rumboot_printf("Test OK Due to Status Reg Check at iteration %d\n",i);}		
    }
  }
  
  return 0;
  
}
