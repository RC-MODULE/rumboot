#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>

#include <platform/devices.h> 

#include <devices/rcm_na/nu_lib.h> 
#include <devices/rcm_na/nu_test_lib.h> 
#include <devices/rcm_na/nu_test_macro.h> 


void nu_ppe_decide_dma_config(
  ConfigPPE* cfg, 
  CubeMetrics* res_metrics, 
  ConfigREGPPE* cfg_reg,
  uint32_t flying_mode,
  void* in_data,
  void* res_data
) {
  cfg_reg->rBALi = (uintptr_t)in_data;
  cfg_reg->wBALo = (uintptr_t)res_data;

  cfg_reg->wOpM = flying_mode << 8;
  nu_ppe_decide_dma_config_trivial(cfg, res_metrics, cfg_reg);
  
}

PPETestDescriptor test_desc;
PPEIterationDescriptor iteration_desc;

int main() {
  int heap_id ;
  int i, it_nmb, dtB;
  int clk_cnt, perf_avg;
  int res;

  uint32_t flying_mode, lbs, mv;

  rumboot_printf("coupled_loop_ppe\n");


  #ifdef LIN
    lbs = 0x0;
  #endif
  #ifdef BOX
    lbs = 0x1;
  #endif
  #ifdef SPL
    lbs = 0x2;
  #endif

  #ifdef MEMtoPPE
    mv = 0x0;
  #endif
  #ifdef VPEtoPPE
    mv = 0x1;
  #endif

  flying_mode = ((lbs&0x3)<<1) | (mv&0x1);
  perf_avg = 0;
  
  heap_id = nu_get_heap_id();
  
  rumboot_platform_request_file("num_iterations_file_tag", (uintptr_t) &it_nmb);
  rumboot_printf("it_nmb is %d\n", it_nmb);

  #if DUT_IS_NPE
    na_cu_set_units_direct_mode(NPE_BASE+NA_CU_REGS_BASE, NA_CU_PPE_UNIT_MODE);
  #endif

  nu_ppe_init_test_desc(&test_desc);
  
    // CHECK If This Is Actually For All Tests In DUT=PPE Environment
  #if DUT_IS_PPE
    test_desc.in_file_tag = "in_ameba_file_tag";
  #endif
  
  
  if(nu_ppe_place_arrays(heap_id,&test_desc,it_nmb) !=0) return -1;
  
  nu_ppe_init_iteration_desc(&test_desc,&iteration_desc);
  

  for (i=0; i<it_nmb; i++) {
    rumboot_printf("Starting Iteration %d\n",i);
    
    nu_ppe_decide_dma_config(
      iteration_desc.cfg,
      iteration_desc.res_metrics,
      iteration_desc.cfg_reg,
      flying_mode,
      iteration_desc.in_data,
      iteration_desc.res_data
    );
    
    iteration_desc.cfg_reg->rOpEn  = 0x0;
    iteration_desc.cfg_reg->wOpEn  = 0x0;
    nu_ppe_setup_reg(MY_PPE_RDMA_BASE, MY_PPE_REGS_BASE, iteration_desc.cfg_reg);
    
    iteration_desc.cfg_reg->wOpEn  = 0x1;
    nu_ppe_wdma_run(MY_PPE_REGS_BASE, iteration_desc.cfg_reg); // wdma start
  
    #ifdef MEMtoPPE
    iteration_desc.cfg_reg->rOpEn  = 0x1;
    nu_ppe_rdma_run(MY_PPE_RDMA_BASE, iteration_desc.cfg_reg); // rdma start
    #endif
  
    #ifdef VPEtoPPE
    #if DUT_IS_PPE
    nu_cpdmac_trn256_config(NU_CPDMAC_ASM_BASE,iteration_desc.in_data,iteration_desc.in_metrics->s);
    nu_cpdmac_trn256_run(NU_CPDMAC_ASM_BASE);
    #endif
    #endif
  
    clk_cnt = rumboot_platform_get_uptime();
  
    while (nu_ppe_status_done(MY_PPE_REGS_BASE) == 0x0) {} // set timeout
    clk_cnt = rumboot_platform_get_uptime() - clk_cnt;
  
    #ifdef MEMtoPPE
    nu_ppe_rdma_wait_complete(MY_PPE_RDMA_BASE);  // wait rdma complete
    #endif
  
    #ifdef VPEtoPPE
    #if DUT_IS_PPE
    nu_cpdmac_trn256_wait_complete(NU_CPDMAC_ASM_BASE);
    #endif
    #endif
  
    nu_ppe_wait_complete(MY_PPE_REGS_BASE);
    
    // Sizeof(DataCube)/(times*frequency); time measure is us, frequency measure is MHz
    // clk_cnt will be devided by frequency later
    dtB = (iteration_desc.cfg_reg->wOpM >> 16 & 0x3) ? 0x2 : 0x1;
    clk_cnt = (iteration_desc.in_metrics->H * iteration_desc.in_metrics->W * iteration_desc.in_metrics->C * dtB)/clk_cnt;
  
    rumboot_printf("Comparing...\n");
  
    res = nu_bitwise_compare(iteration_desc.res_data,iteration_desc.etalon,iteration_desc.res_metrics->s);
  
    if (res) {
      nu_ppe_print_config(iteration_desc.cfg);
      nu_ppe_print_config_reg(iteration_desc.cfg_reg);
    
      //~ rumboot_platform_dump_region("res_data.bin",(uint32_t)res_data,res_metrics->s);
      //~ rumboot_platform_dump_region("cfg_reg.bin", cfg_reg, NU_PPE_REG_CFG_PARAMS_NUM*sizeof(uint32_t));
    }
  
    if (!res) {
      rumboot_printf("Iteration %d PASSED\n", i);

      #ifdef ShowPerf
      clk_cnt = (clk_cnt*100)/ppe_clk_f;
      perf_avg += clk_cnt;

      rumboot_printf("PPE perfomance of iteration # %d is %d.%d bytes per cycle\n", i, clk_cnt/100, clk_cnt-(clk_cnt/100)*100);
      #endif
    }
    else {
      rumboot_printf("Test FAILED at iteration %d\n", i);
      return 1;
    };
    
      //
    nu_ppe_iterate_desc(&iteration_desc);
  }
  
  #ifdef ShowPerf
  if (it_nmb == 0) rumboot_printf("ShowPerf failed\n");
  else {
    perf_avg = perf_avg/it_nmb;

    rumboot_printf("PPE average perfomance of %d iterations is %d.%d bytes per cycle\n", it_nmb, perf_avg/100, perf_avg-(perf_avg/100)*100);
  }
  #endif

  return 0;
}

