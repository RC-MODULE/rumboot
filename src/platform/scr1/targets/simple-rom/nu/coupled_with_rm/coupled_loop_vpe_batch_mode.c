
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>
#include <stdbool.h>

#include <platform/devices.h> 

#include <devices/rcm_na/nu_lib.h> 
#include <devices/rcm_na/nu_test_lib.h> 
#include <devices/rcm_na/nu_test_macro.h> 

#include "devices/rcm_na/nu_vpe_file_tags.h"

void *in_data;
// void *etalon;
void *res_data;
void *op0;
void *op1;
void *op2;
void *lut1;
void *lut2;

void** etalon; // Need To Store Many etalon Arrays

void nu_vpe_set_op_pointers (
  ConfigVPE* cfg,
  void*op0,
  void*op1,
  void*op2
){
  cfg->op0_rdma_config.dma_baddr = nu_virt_to_dma(op0);
  cfg->op1_rdma_config.dma_baddr = nu_virt_to_dma(op1);
  cfg->op2_rdma_config.dma_baddr = nu_virt_to_dma(op2);
}

void nu_vpe_decide_dma_config (
  ConfigVPE* cfg,
  CubeMetrics* in_metrics,
  uint8_t axi_len,
  void*in_data,
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
  cfg->wdma_config.dma_bsize=0;
  cfg->wdma_config.dma_bstride=0;
  cfg->depend_mask=0; // Does Not Depend On Any Other Channel Run
  
  cfg->wdma_config.dma_en = Enable_En;
  
  nu_vpe_decide_dma_config_trivial(cfg,in_metrics);
  
  cfg->src_rdma_config.dma_baddr = nu_virt_to_dma(in_data);
  cfg->wdma_config.dma_baddr     = nu_virt_to_dma(res_data);

  cfg->src_rdma_config.dma_axi_len = axi_len;
  cfg->op0_rdma_config.dma_axi_len = axi_len;
  cfg->op1_rdma_config.dma_axi_len = axi_len;
  cfg->op2_rdma_config.dma_axi_len = axi_len;
  cfg->wdma_config.dma_axi_len     = axi_len;
  
}

int nu_vpe_update_batch_parameters(ConfigVPE* cfg, bool second_turn, bool last_turn, void* in_prev, void* in_curr, void* res_prev, void* res_curr) {
  uint32_t in_prev_uint;
  uint32_t in_curr_uint;
  uint32_t in_diff;
  uint32_t res_prev_uint;
  uint32_t res_curr_uint;
  uint32_t res_diff;
  
  in_prev_uint = (uint32_t) in_prev;
  in_curr_uint = (uint32_t) in_curr;
  in_diff = in_curr_uint - in_prev_uint;
  
  res_prev_uint = (uint32_t) res_prev;
  res_curr_uint = (uint32_t) res_curr;
  res_diff = res_curr_uint - res_prev_uint;
  
  if(second_turn) {
    cfg->src_rdma_config.dma_bstride = in_diff;
    cfg->    wdma_config.dma_bstride = res_diff;
  }
  else {
    if(cfg->src_rdma_config.dma_bstride != in_diff) {
      rumboot_printf("Bad Allocation of Batch In items: dma_bstride=0x%x, curr=0x%x, prev=0x%x\n",
        cfg->src_rdma_config.dma_bstride,in_curr_uint,in_prev_uint
      );
      return -1;
    }
    if(cfg->    wdma_config.dma_bstride != res_diff) {
      rumboot_printf("Bad Allocation of Batch Res items: dma_bstride=0x%x, curr=0x%x, prev=0x%x\n",
        cfg->    wdma_config.dma_bstride,res_curr_uint,res_prev_uint
      );
      return -1;
    }
  }
  
  cfg->src_rdma_config.dma_bsize++;
  cfg->    wdma_config.dma_bsize++;
  
  if(last_turn) {
    cfg->cube_size = (cfg->cube_size+1) /** (cfg->wdma_config.dma_bsize+1)*/ - 1;  // IMPROVE IT!!!
  }
  
  return 0;
}

ConfigVPE cfg;

CubeMetrics* in_metrics;
CubeMetrics* res_metrics;

VectorMetrics* lut1_metrics;
VectorMetrics* lut2_metrics;

StatusRegs status_regs_etalon;

int main() {
  int heap_id;
  int i;
  int iterations;
  bool last_turn;
  int turn_index;
  int invocation_index;
  
  uint32_t misalign; // Misaligns Operands Only
  uint8_t  axi_len;
    
  char* fn_in_base          			= "in_file_tag_"        ;
  char* fn_cfg_base         			= "cfg_file_tag_"   	;
  char* fn_in_metrics_base  			= "metrics_in_tag_"     ;
  char* fn_res_metrics_base 			= "metrics_etalon_tag_" ;
  char* fn_etalon_base      			= "etalon_file_tag_"    ;
  
  char* fn_metrics_op0_cube_base		= "metrics_op0_cube_tag_" ;
  char* fn_metrics_op0_vec_base 		= "metrics_op0_vec_tag_" ;
  char* fn_op0_cube_file_base   		= "op0_cube_file_tag_" ;
  char* fn_op0_vec_file_base    		= "op0_vec_file_tag_" ;
  
  char* fn_metrics_op1_cube_base		= "metrics_op1_cube_tag_" ;
  char* fn_metrics_op1_vec_base 		= "metrics_op1_vec_tag_" ;
  char* fn_op1_cube_file_base   		= "op1_cube_file_tag_" ;
  char* fn_op1_vec_file_base    		= "op1_vec_file_tag_" ;
  
  char* fn_metrics_op2_cube_base		= "metrics_op2_cube_tag_" ;
  char* fn_metrics_op2_vec_base 		= "metrics_op2_vec_tag_" ;
  char* fn_op2_cube_file_base   		= "op2_cube_file_tag_" ;
  char* fn_op2_vec_file_base    		= "op2_vec_file_tag_" ;
  
  char* fn_metrics_lut1_file_base   	= "metrics_lut1_file_tag_" ; 
  char* fn_metrics_lut2_file_base   	= "metrics_lut2_file_tag_" ;
  char* fn_lut1_file_base   			= "lut1_file_tag_" ; 
  char* fn_lut2_file_base   			= "lut2_file_tag_" ;
 
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
  
  void* in_data_prev;
  void* res_data_prev;
  
  void* in_data_first;
  void* res_data_first;
  
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

 
  rumboot_printf("Hello Batch Mode\n");

  heap_id = nu_get_heap_id();
  
  rumboot_platform_request_file_ex("num_iterations_file_tag",(uintptr_t) &iterations,sizeof(iterations));
  rumboot_printf("Number of iterations %d\n",iterations);
  
  etalon = rumboot_malloc_from_heap(0, sizeof(void*) * iterations); // Allocate The Array Of Pointers To Etalon Data
  
#if DUT_IS_NPE
  na_cu_set_units_direct_mode(NPE_BASE+NA_CU_REGS_BASE, NA_CU_VPE_UNIT_MODE);
#endif


  turn_index=0;
  invocation_index=0;
  for(i=0;i<iterations;i++) {
    
    if(turn_index==0)
      rumboot_printf("Starting invocation %d\n",invocation_index);
    
    rumboot_printf("Loading Data for invocation %d, Cube %d (Iteration %d)\n",invocation_index,turn_index,i);
    
	fn_base_it_nmb(fn_in, fn_in_base, i);
    fn_base_it_nmb(fn_cfg, fn_cfg_base, i);
    fn_base_it_nmb(fn_in_metrics, fn_in_metrics_base, i);
    fn_base_it_nmb(fn_res_metrics, fn_res_metrics_base, i);
    fn_base_it_nmb(fn_etalon, fn_etalon_base, i);
	
	fn_base_it_nmb(fn_metrics_op0_cube, fn_metrics_op0_cube_base, (i-turn_index));
	fn_base_it_nmb(fn_metrics_op0_vec, fn_metrics_op0_vec_base, (i-turn_index));
	fn_base_it_nmb(fn_op0_cube_file, fn_op0_cube_file_base, (i-turn_index));
	fn_base_it_nmb(fn_op0_vec_file, fn_op0_vec_file_base, (i-turn_index));	
	
	fn_base_it_nmb(fn_metrics_op1_cube, fn_metrics_op1_cube_base, (i-turn_index));
	fn_base_it_nmb(fn_metrics_op1_vec, fn_metrics_op1_vec_base, (i-turn_index));
	fn_base_it_nmb(fn_op1_cube_file, fn_op1_cube_file_base, (i-turn_index));
	fn_base_it_nmb(fn_op1_vec_file, fn_op1_vec_file_base, (i-turn_index));	

	fn_base_it_nmb(fn_metrics_op2_cube, fn_metrics_op2_cube_base, (i-turn_index));
	fn_base_it_nmb(fn_metrics_op2_vec, fn_metrics_op2_vec_base, (i-turn_index));
	fn_base_it_nmb(fn_op2_cube_file, fn_op2_cube_file_base, (i-turn_index));
	fn_base_it_nmb(fn_op2_vec_file, fn_op2_vec_file_base, (i-turn_index));	
	
	fn_base_it_nmb(fn_metrics_lut1_file, fn_metrics_lut1_file_base, (i-turn_index));
	fn_base_it_nmb(fn_metrics_lut2_file, fn_metrics_lut2_file_base, (i-turn_index));
	fn_base_it_nmb(fn_lut1_file, fn_lut1_file_base, (i-turn_index));
	fn_base_it_nmb(fn_lut2_file, fn_lut2_file_base, (i-turn_index));

    if(turn_index==0) {
      if(nu_vpe_load_cfg_by_tag(heap_id, &cfg, fn_cfg) != 0) return -1;
    }
    
    in_metrics = nu_load_cube_metrics(heap_id,fn_in_metrics);
    if(in_metrics == NULL) return -1;
    
    last_turn = in_metrics->role == CubeRole_LastInBatch;
    
    res_metrics= nu_load_cube_metrics(heap_id,fn_res_metrics);
    if(res_metrics == NULL) return -1;

    in_data = nu_load_cube(heap_id,fn_in,in_metrics);
    if(in_data == NULL) return -1;
    
    res_data = nu_vpe_malloc_res(heap_id, res_metrics);
    if(res_data == NULL) return -1;
    
    etalon[i] = nu_load_cube(heap_id,fn_etalon,res_metrics);
    if(etalon[i] == NULL) return -1;
    
    if(turn_index==0) {
      nu_vpe_decide_dma_config(&cfg,in_metrics,axi_len,in_data,res_metrics,res_data);
      
        // Store The In And Out Batch Bases
      res_data_first = res_data;
      in_data_first = in_data;
      
    }
    else {  // turn_index != 0 // All Other Turns
      if(nu_vpe_update_batch_parameters(&cfg,turn_index==1,last_turn,in_data_prev,in_data,res_data_prev,res_data) != 0) {
        return -1;
      }
    }
       
    if(last_turn) {
      char* res_ptr;
      char* in_data_ptr;
      int k;
 
      k=i-turn_index; // Iteration Which If The First For This Turn
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
      
      nu_vpe_set_op_pointers(&cfg,op0,op1,op2);
      
    //~ if(..) {
      //~ if(nu_vpe_load_status_regs_by_tag(heap_id,&status_regs_etalon,status_regs_file_tag[i]) != 0) return -1;
    //~ }

      nu_vpe_print_config(&cfg);
      // nu_vpe_print_status_regs_etalon(&status_regs_etalon);
      
      nu_vpe_setup(MY_VPE_REGS_BASE, &cfg);
      
        // Setup Main Channel DMAs if Required
      if(cfg.src_rdma_config.dma_en == Enable_NotEn)
        nu_vpe_config_rd_main_channel(NU_CPDMAC_ASM_BASE,in_data,in_metrics->s);
      //~ if(cfg.wdma_config.dma_en == Enable_NotEn)
        //~ nu_vpe_config_wr_main_channel(NU_CPDMAC_ASM_BASE,res_data,res_metrics->s);
      
      rumboot_printf("Running DMA..\n");
      
        // Invoke Required DMA Channels
      if(cfg.src_rdma_config.dma_en == Enable_NotEn)
        nu_vpe_run_rd_main_channel(NU_CPDMAC_ASM_BASE);
      //~ if(cfg.wdma_config.dma_en == Enable_NotEn)
        //~ nu_vpe_run_wr_main_channel(NU_CPDMAC_ASM_BASE);
      
      nu_vpe_run(MY_VPE_REGS_BASE, &cfg);     // To Invoke Or Not To Invoke Internal DMA Channel - Decide inside nu_vpe_run
      
      nu_vpe_wait(MY_VPE_REGS_BASE, &cfg);
      
        // Finalize Required DMA Channels
      if(cfg.src_rdma_config.dma_en == Enable_NotEn)
        nu_vpe_wait_rd_main_channel_complete(NU_CPDMAC_ASM_BASE);
      //~ if(cfg.wdma_config.dma_en == Enable_NotEn)
        //~ nu_vpe_wait_wr_main_channel_complete(NU_CPDMAC_ASM_BASE);
      
      rumboot_printf("Comparing..\n");
      
      res_ptr = res_data_first;
      in_data_ptr = in_data_first;
      for(int j=k; j<=i; j++) { // Iterate From The Batch Start To The Current Iteration
        rumboot_printf("Comparing invocation %d, Cube %d (Iteration %d): res=0x%x, etalon=0x%x, %d bytes\n",invocation_index,j-k,j,
                                                                (uint32_t)res_ptr,(uint32_t)etalon[j], res_metrics->s);
        
        if(nu_bitwise_compare(res_ptr,etalon[j],res_metrics->s) == 0)
          rumboot_printf("Invocation %d, Cube %d (Iteration %d) PASSED\n",invocation_index,j-k,j);
        else {
          rumboot_printf("Test FAILED at invocation %d, Cube %0d (Iteration %d)\n",invocation_index,j-k,j);
          nu_vpe_interpret_mismatch(&cfg,res_ptr,etalon[j],in_data_ptr,op0,op1,op2,res_metrics->s,res_metrics->C);
          return 1;
        }
        
        res_ptr     += cfg.    wdma_config.dma_bstride;
        in_data_ptr += cfg.src_rdma_config.dma_bstride;
      }
    }
    
    
      // Iteration Info Switch
    in_data_prev = in_data;
    res_data_prev = res_data;
    
    turn_index = last_turn ? 0 : turn_index+1;
    
    if(last_turn)
      invocation_index++;
  }
  
  return 0;
  
}
