#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>

#include <platform/devices.h> 

#include <devices/rcm_na/nu_lib.h> 
#include <devices/rcm_na/nu_test_lib.h> 
#include <devices/rcm_na/nu_test_macro.h> 

void nu_vpe_ppe_decide_dma_config(
  ConfigVPE* cfg_vpe,
  ConfigPPE* cfg_ppe,
  ConfigREGPPE* cfg_ppe_reg,
  CubeMetrics* in_metrics,
  CubeMetrics* res_metrics,
  void*in_data,
  void*op0,
  void*op1,
  void*op2,
  void*res_data
) {
  uint8_t  axi_len;
  uint32_t flying_mode, lbs, mv;
  axi_len = 15; // 15 - Most Performance-Friendly Option

  #ifdef LIN
    lbs = 0x0;
  #endif
  #ifdef BOX
    lbs = 0x1;
  #endif

  mv = 0x1;

  flying_mode = ((lbs&0x3)<<1) | (mv&0x1);

  if      (lbs == 0x0)  cfg_vpe->trace_mode = TraceMode_PPE;
  else if (lbs == 0x1)  cfg_vpe->trace_mode = TraceMode_MPE;

  cfg_vpe->src_rdma_config.dma_bsize=0;   // Batch Parameters Are Default In This Program (Do Not Use Batch)
  cfg_vpe->src_rdma_config.dma_bstride=0;
  cfg_vpe->op0_rdma_config.dma_bsize=0;
  cfg_vpe->op0_rdma_config.dma_bstride=0;
  cfg_vpe->op1_rdma_config.dma_bsize=0;
  cfg_vpe->op1_rdma_config.dma_bstride=0;
  cfg_vpe->op2_rdma_config.dma_bsize=0;
  cfg_vpe->op2_rdma_config.dma_bstride=0;
  cfg_vpe->wdma_config.dma_bsize=0;
  cfg_vpe->wdma_config.dma_bstride=0;
  
  cfg_vpe->src_rdma_config.dma_axi_len = axi_len;
  cfg_vpe->op0_rdma_config.dma_axi_len = axi_len;
  cfg_vpe->op1_rdma_config.dma_axi_len = axi_len;
  cfg_vpe->op2_rdma_config.dma_axi_len = axi_len;
  cfg_vpe->wdma_config.dma_axi_len     = axi_len;
  
  
  cfg_vpe->src_flying = Enable_NotEn;
  cfg_vpe->dst_flying = Enable_En;
  
  cfg_ppe_reg->wOpM = flying_mode << 8;
  
  nu_vpe_decide_dma_config_trivial(cfg_vpe,in_metrics); // Fill Other cfg Fields
  nu_ppe_decide_dma_config_trivial(cfg_ppe,res_metrics,cfg_ppe_reg);
  
  cfg_ppe_reg->wOpEn  = 0x0; // This Is Needed Because Some Other Setup Functions Write Into DMA Start Reg
  
    // Data Base Addresses
  cfg_vpe->src_rdma_config.dma_baddr = nu_virt_to_dma(in_data);
  cfg_vpe->op0_rdma_config.dma_baddr = nu_virt_to_dma(op0);
  cfg_vpe->op1_rdma_config.dma_baddr = nu_virt_to_dma(op1);
  cfg_vpe->op2_rdma_config.dma_baddr = nu_virt_to_dma(op2);
  cfg_vpe->wdma_config.dma_baddr     = (uint32_t) 0xDEADBEEF; // Should Be Not Used
  cfg_ppe_reg->wBALo = nu_virt_to_dma(res_data);
  cfg_ppe_reg->rBALi = (uintptr_t)0xDEADBEEF; 

}

NPETestDescriptor test_desc;
NPEIterationDescriptor iteration_desc;

int main() {
  int heap_id;
  int i;
  int iterations;
  LUTLoadDecision* lut_decision;
  void* lut1_prev;
  void* lut2_prev;
  
  rumboot_printf("Hellooooooo\n");

  heap_id = nu_get_heap_id();
  
    // Read The Number Of Test Iterations
  rumboot_platform_request_file_ex("num_iterations_file_tag",(uintptr_t) &iterations,sizeof(iterations));
  rumboot_printf("Number of iterations %d\n",iterations);
  
    // Zero The Test Descriptor Fields
  nu_npe_init_test_desc(&test_desc);
  test_desc.MPE_ENABLED=Enable_NotEn;
  
    // Load All The Test Data Into Memory
  if(nu_npe_place_arrays(heap_id,&test_desc,iterations) !=0) return -1;
  
    // Make The Iteration Descriptor To Point At The First Test Data
  nu_npe_init_iteration_desc(&test_desc,&iteration_desc);
  
    // Array Of Decisions What To Do With VPE LUT
  lut_decision = rumboot_malloc_from_heap(heap_id,sizeof(LUTLoadDecision)*iterations);
  lut1_prev=NULL;lut2_prev=NULL;
  
  for(i=0;i<iterations;i++) {
    rumboot_printf("Deciding DMA Configuration for iteration %d\n",i);
    
      // Prepare Stuff
    nu_npe_iteration_start(&iteration_desc);
    
    nu_vpe_ppe_decide_dma_config(
      iteration_desc.cfg_vpe,
      iteration_desc.cfg_ppe,
      iteration_desc.cfg_reg_ppe,
      iteration_desc.in_metrics,
      iteration_desc.res_metrics,
      iteration_desc.in_data,
      iteration_desc.op0,
      iteration_desc.op1,
      iteration_desc.op2,
      iteration_desc.res_data
    );
    
    //~ nu_vpe_print_config(iteration_desc.cfg_vpe);
    //~ nu_ppe_print_config(iteration_desc.cfg_ppe);
    //~ nu_ppe_print_config_reg(iteration_desc.cfg_reg_ppe);
    
      // Prepare The Decision What To Do With LUT On Each Iteration
    if(iteration_desc.cfg_vpe->op2_config.lut_en==Enable_En) {
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
    
    nu_npe_iterate_desc(&iteration_desc);
  }
  
    // Program The CU To Enable Direct Mode 
  na_cu_set_units_direct_mode(NPE_BASE+NA_CU_REGS_BASE, NA_CU_VPE_UNIT_MODE|NA_CU_PPE_UNIT_MODE);
  
  nu_npe_init_iteration_desc(&test_desc,&iteration_desc);
    // Main Loop 
  for(i=0;i<iterations;i++) {
    rumboot_printf("Starting iteration %d\n",i);
	
      // Prepare Iteration Descriptor (Some Modifications Should Be Made Before Each Iteration)
    nu_npe_iteration_start(&iteration_desc);
    
      // Program The VPE And PPE (Single Run)
    nu_ppe_setup_reg(MY_PPE_RDMA_BASE, MY_PPE_REGS_BASE, iteration_desc.cfg_reg_ppe);
    if(i!=0)  // Before Writing VPE Regs - We Should Ensure That VPE Is Ready
              //  Dont Make It On Iteration #0 - Because nu_vpe_wait_cntx_appl Waits For A Status Of Prev Iteration
      nu_vpe_wait_cntx_appl(MY_VPE_REGS_BASE);
    
    nu_vpe_setup(MY_VPE_REGS_BASE, iteration_desc.cfg_vpe);
  
      // Load LUTs If Needed
    if(lut_decision[i]==LUTLoadDecision_BlockThenLoad) { // if We Need To Reload LUT
      rumboot_printf("Blocked To Load LUT\n");  // Here We Should Block And Wait Until VPE Finish
      while (nu_vpe_busy(MY_VPE_REGS_BASE)) {}; // 
      rumboot_printf("OK\n");
    }
    if(lut_decision[i]!=LUTLoadDecision_DontLoad) {
      nu_vpe_load_lut(MY_VPE_REGS_BASE,iteration_desc.lut1,iteration_desc.lut2);
    }
    
    
    nu_vpe_run(MY_VPE_REGS_BASE, iteration_desc.cfg_vpe);
      // PPE Should Be Run After It Finish Prev Operation
    nu_ppe_dma_wait_ready_and_run(MY_PPE_REGS_BASE); // WDMA
    
    nu_npe_iterate_desc(&iteration_desc);
  }
  
  nu_ppe_wait_complete(MY_PPE_REGS_BASE);
  
    // Init It Again - We Start Iterate From The Beginning
  nu_npe_init_iteration_desc(&test_desc,&iteration_desc);
  for(i=0;i<iterations;i++) {
    rumboot_printf("Comparing iteration %d..\n",i);
    nu_npe_iteration_start(&iteration_desc);
    
      // Result vs Etalon Comparision
    if(nu_bitwise_compare(iteration_desc.res_data, iteration_desc.etalon, iteration_desc.res_metrics->s) == 0)
      rumboot_printf("Iteration %d PASSED\n",i);
    else {
      nu_vpe_print_config(iteration_desc.cfg_vpe);
      nu_ppe_print_config(iteration_desc.cfg_ppe);

      rumboot_printf("Test FAILED at iteration %d\n",i);

      return 1;
    }
    
      // Point At The Next Iteration Data
    nu_npe_iterate_desc(&iteration_desc);
  }
  
  rumboot_printf("Test Passed\n");
  return 0;
}
