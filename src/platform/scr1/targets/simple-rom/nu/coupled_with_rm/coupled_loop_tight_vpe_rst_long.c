#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>

#include <platform/devices.h> 

#include <devices/rcm_na/nu_lib.h> 
#include <devices/rcm_na/nu_test_lib.h> 
#include <devices/rcm_na/nu_test_macro.h> 



void nu_vpe_decide_dma_config (
  ConfigVPE* cfg,
  CubeMetrics* in_metrics,
  uint8_t axi_len,
  void*in_data,
  void*op0,
  void*op1,
  void*op2,
  CubeMetrics*res_metrics,
  void*res_data,
  bool last_turn
) {
  
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
  cfg->src_rdma_config.dma_bstride=0;
  cfg->op0_rdma_config.dma_bsize=0;
  cfg->op0_rdma_config.dma_bstride=0;
  cfg->op1_rdma_config.dma_bsize=0;
  cfg->op1_rdma_config.dma_bstride=0;
  cfg->op2_rdma_config.dma_bsize=0;
  cfg->op2_rdma_config.dma_bstride=0;
  cfg->wdma_config.dma_bsize=0;
  cfg->wdma_config.dma_bstride=0;
  
  nu_vpe_decide_dma_config_trivial(cfg,in_metrics);
  
  if(last_turn) 
    cfg->mark = Enable_En;
  
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

VPETestDescriptor test_desc;
VPEIterationDescriptor iteration_desc;

int main() {
  int heap_id;
  int i;
  int iterations;
  LUTLoadDecision* lut_decision;
  void* lut1_prev;
  void* lut2_prev;
  //~ ConfigVPE* cfg_prev;
  
  uint8_t  axi_len;
  

#ifdef AXI_LEN
  axi_len = AxiLen;
#else 
  axi_len = 15;
#endif

 
  rumboot_printf("Hello\n");

  heap_id = nu_get_heap_id();
  
  rumboot_platform_request_file_ex("num_iterations_file_tag",(uintptr_t) &iterations,sizeof(iterations));
  rumboot_printf("Number of iterations %d\n",iterations);
  
  nu_vpe_init_test_desc(&test_desc);
  if(nu_vpe_place_arrays(heap_id,&test_desc,iterations) !=0) return -1;
  
  nu_vpe_init_iteration_desc(&test_desc,&iteration_desc);
  
#if DUT_IS_NPE
  na_cu_set_units_direct_mode(NPE_BASE+NA_CU_REGS_BASE, NA_CU_VPE_UNIT_MODE);
#endif
 #ifdef VPE_CUBE_CMPL
  nu_vpe_set_int_mask(MY_VPE_REGS_BASE, &iteration_desc); 	
#endif      
  lut_decision = rumboot_malloc_from_heap(heap_id,sizeof(LUTLoadDecision)*iterations);
  lut1_prev=NULL;lut2_prev=NULL;
  //~ cfg_prev=NULL;
  
    // Fill The Recent cfg Fields Now Because We Can
  for(i=0;i<iterations;i++) {
    rumboot_printf("Deciding DMA Configuration, Iteration %d\n",i);
    nu_vpe_iteration_start(&iteration_desc);
    // nu_vpe_print_iteration_desc(&iteration_desc);
    nu_vpe_decide_dma_config(
      iteration_desc.cfg,
      iteration_desc.in_metrics,
      axi_len,
      iteration_desc.in_data,
      iteration_desc.op0,
      iteration_desc.op1,
      iteration_desc.op2,
      iteration_desc.res_metrics,
      iteration_desc.res_data,
      i==iterations-1
    );
    // nu_vpe_print_config(iteration_desc.cfg);
    // nu_vpe_print_status_regs_etalon(iteration_desc.status_regs_etalon);
    
      // Prepare The Decision What To Do With LUT On Each Iteration
    if(iteration_desc.cfg->op2_config.lut_en==Enable_En) {
      if(lut1_prev==NULL) {
        lut_decision[i]=LUTLoadDecision_LoadNow;
        lut1_prev = iteration_desc.lut1; lut2_prev = iteration_desc.lut2;
      }
      else {
        if(nu_vpe_compare_luts(iteration_desc.lut1, iteration_desc.lut2, lut1_prev, lut2_prev) == 0) {
          lut_decision[i]=LUTLoadDecision_DontLoad;
        }
        else {
          lut_decision[i]=LUTLoadDecision_BlockThenLoad;
          lut1_prev = iteration_desc.lut1; lut2_prev = iteration_desc.lut2;
        }
      }
    }
    else {
      lut_decision[i]=LUTLoadDecision_DontLoad;
      lut1_prev = NULL; lut2_prev = NULL;
    }
    
    //~ cfg_prev = iteration_desc.cfg;
    nu_vpe_iterate_desc(&iteration_desc);
  }
  
    // Once More Init - This Time For The Main Working Loop
  nu_vpe_init_iteration_desc(&test_desc,&iteration_desc);
  
  for(i=0;i<iterations;i++) {
    rumboot_printf("Starting iteration %d\n",i);
	
    nu_vpe_iteration_start(&iteration_desc);
    
      // Load LUTs If Needed
    if(lut_decision[i]==LUTLoadDecision_BlockThenLoad) {
      rumboot_printf("Blocked To Load LUT\n");
      while (nu_vpe_busy(MY_VPE_REGS_BASE)) {};
      rumboot_printf("OK\n");
    }
    if(lut_decision[i]!=LUTLoadDecision_DontLoad) {
      nu_vpe_load_lut(MY_VPE_REGS_BASE,iteration_desc.lut1,iteration_desc.lut2);
    }
    
    nu_vpe_setup(MY_VPE_REGS_BASE, iteration_desc.cfg);    
	nu_vpe_run(MY_VPE_REGS_BASE, iteration_desc.cfg);
if ((i== (iterations-1))  | (i == 0)){
	 
		nu_vpe_pause_next_cntx(MY_VPE_REGS_BASE, iteration_desc.cfg);		
		nu_vpe_soft_reset(MY_VPE_REGS_BASE, iteration_desc.cfg);	
		nu_vpe_dev_pause_resume(MY_VPE_REGS_BASE,iteration_desc.cfg);			
	if	(nu_vpe_regs_check((MY_VPE_REGS_BASE + NU_VPE),2,64) != 0){
		rumboot_printf("Test FAILED at iteration %d\n",i);
      return 1;
	}
	
	if	(nu_regs_check((MY_VPE_REGS_BASE + NU_VPE_SRC_RDMA),0,31) != 0){
		 rumboot_printf("Test FAILED at iteration %d\n",i);
      return 1;
	}	
	if	(nu_regs_check((MY_VPE_REGS_BASE + NU_VPE_OP0_RDMA),0,31) != 0){
		 rumboot_printf("Test FAILED at iteration %d\n",i);
      return 1;
	}
	if	(nu_regs_check((MY_VPE_REGS_BASE + NU_VPE_OP1_RDMA),0,31) != 0){
		 rumboot_printf("Test FAILED at iteration %d\n",i);
      return 1;
	}
	if	(nu_regs_check((MY_VPE_REGS_BASE + NU_VPE_OP2_RDMA),0,31) != 0){
		 rumboot_printf("Test FAILED at iteration %d\n",i);
      return 1;
	}
	
	if	(nu_regs_check((MY_VPE_REGS_BASE + NU_VPE_DST_WDMA),0,31) != 0){
		 rumboot_printf("Test FAILED at iteration %d\n",i);
      return 1;
	}
		
	nu_vpe_wait_int_pause_cntx_appl(MY_VPE_REGS_BASE, iteration_desc.cfg);

	} 	
	else
	{
	nu_vpe_dev_resume(MY_VPE_REGS_BASE, iteration_desc.cfg);
	nu_vpe_wait_int_cntx_appl(MY_VPE_REGS_BASE, iteration_desc.cfg);}	

    if(i!=iterations-1)
      nu_vpe_iterate_desc(&iteration_desc);
  }
  
    // And Third Turn - Check The Data
  rumboot_printf("Comparing..\n");
  nu_vpe_init_iteration_desc(&test_desc,&iteration_desc);
  	   nu_vpe_iterate_desc(&iteration_desc);
 for(i=1;i<(iterations-1);i++ ){
	   nu_vpe_iteration_start(&iteration_desc);
    if(nu_bitwise_compare(iteration_desc.res_data,iteration_desc.etalon,iteration_desc.res_metrics->s) == 0)
		rumboot_printf("Iteration %d PASSED\n",i);
    else {
      rumboot_printf("Test FAILED at iteration %d\n",i);	  	  
      rumboot_printf("Addresses: res_data=0x%x, etalon=0x%x\n", (uintptr_t) iteration_desc.res_data, (uintptr_t) iteration_desc.etalon);
      nu_vpe_print_config(iteration_desc.cfg);
      nu_vpe_interpret_mismatch(
        iteration_desc.cfg,
        iteration_desc.res_data,
        iteration_desc.etalon,
        iteration_desc.in_data,
        iteration_desc.op0,
        iteration_desc.op1,
        iteration_desc.op2,
        iteration_desc.res_metrics->s,
        iteration_desc.res_metrics->C
      );
      return 1;
    }
    
    nu_vpe_iterate_desc(&iteration_desc);
  }
  
  rumboot_printf("Test Passed\n");
  return 0;
  
}

