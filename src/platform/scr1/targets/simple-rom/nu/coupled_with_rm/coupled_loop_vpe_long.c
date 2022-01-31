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
  void*res_data
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
  cfg->wdma_config.dma_bsize=0;
  cfg->wdma_config.dma_bstride=0;
  
  nu_vpe_decide_dma_config_trivial(cfg,in_metrics);
  
  cfg->src_rdma_config.dma_baddr = (uint32_t) in_data;
  cfg->op0_rdma_config.dma_baddr = (uint32_t) op0;
  cfg->op1_rdma_config.dma_baddr = (uint32_t) op1;
  cfg->op2_rdma_config.dma_baddr = (uint32_t) op2;
  cfg->wdma_config.dma_baddr     = (uint32_t) res_data;

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
  
  uint8_t  axi_len;
  

#ifdef AXI_LEN
  axi_len = AxiLen;
#else 
  axi_len = 15;
#endif

 
  rumboot_printf("Hellooooooo\n");

  heap_id = nu_get_heap_id();
  
  rumboot_platform_request_file("num_iterations_file_tag",(uintptr_t) &iterations);
  rumboot_printf("Number of iterations %d\n",iterations);
  
  nu_vpe_init_test_desc(&test_desc);
  if(nu_vpe_place_arrays(heap_id,&test_desc,iterations) !=0) return -1;
  
  nu_vpe_init_iteration_desc(&test_desc,&iteration_desc);
  
#if DUT_IS_NPE
  na_cu_set_units_direct_mode(NPE_BASE+NA_CU_REGS_BASE, NA_CU_VPE_UNIT_MODE);
#endif
  
  for(i=0;i<iterations;i++) {
    rumboot_printf("Starting iteration %d\n",i);
	
    nu_vpe_iteration_start(&iteration_desc);
    
      // Load LUTs If Needed
    if(iteration_desc.cfg->op2_config.lut_en == Enable_En) {
      nu_vpe_load_lut(MY_VPE_REGS_BASE,iteration_desc.lut1,iteration_desc.lut2);
    }
     

    nu_vpe_decide_dma_config(
      iteration_desc.cfg,
      iteration_desc.in_metrics,
      axi_len,
      iteration_desc.in_data,
      iteration_desc.op0,
      iteration_desc.op1,
      iteration_desc.op2,
      iteration_desc.res_metrics,
      iteration_desc.res_data
    );
    // nu_vpe_print_config(iteration_desc.cfg);
    // nu_vpe_print_status_regs_etalon(iteration_desc.status_regs_etalon);
    
    nu_vpe_setup(MY_VPE_REGS_BASE, iteration_desc.cfg);
    
    rumboot_printf("Running DMA..\n");
    
#if DUT_IS_VPE
      // Setup Main Channel DMAs if Required
    if(iteration_desc.cfg->src_rdma_config.dma_en == Enable_NotEn)
      nu_vpe_config_rd_main_channel(NU_CPDMAC_ASM_BASE,iteration_desc.in_data,iteration_desc.in_metrics->s);
    if(iteration_desc.cfg->wdma_config.dma_en == Enable_NotEn)
      nu_vpe_config_wr_main_channel(NU_CPDMAC_ASM_BASE,iteration_desc.res_data,iteration_desc.res_metrics->s);
    
      // Invoke Required DMA Channels
    if(iteration_desc.cfg->src_rdma_config.dma_en == Enable_NotEn)
      nu_vpe_run_rd_main_channel(NU_CPDMAC_ASM_BASE);
    if(iteration_desc.cfg->wdma_config.dma_en == Enable_NotEn)
      nu_vpe_run_wr_main_channel(NU_CPDMAC_ASM_BASE);
#endif
    
    nu_vpe_run(MY_VPE_REGS_BASE, iteration_desc.cfg);     // To Invoke Or Not To Invoke Internal DMA Channel - Decide inside nu_vpe_run
    
    nu_vpe_wait(MY_VPE_REGS_BASE, iteration_desc.cfg);
    
#if DUT_IS_VPE
      // Finalize Required DMA Channels
    if(iteration_desc.cfg->src_rdma_config.dma_en == Enable_NotEn)
      nu_vpe_wait_rd_main_channel_complete(NU_CPDMAC_ASM_BASE);
    if(iteration_desc.cfg->wdma_config.dma_en == Enable_NotEn)
      nu_vpe_wait_wr_main_channel_complete(NU_CPDMAC_ASM_BASE);
#endif
    
    rumboot_printf("Comparing..\n");
    
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
    
    if(nu_vpe_check_status_regs(MY_VPE_REGS_BASE, iteration_desc.status_regs_etalon) != 0) {
      rumboot_printf("Test FAILED Due to Status Reg Check at iteration %d\n",i);
      nu_vpe_print_status_regs_etalon(iteration_desc.status_regs_etalon);
      return -1;
    }
    else {rumboot_printf("Test OK Due to Status Reg Check at iteration %d\n",i);}		
    
    nu_vpe_iterate_desc(&iteration_desc);
  }
  
  return 0;
  
}
