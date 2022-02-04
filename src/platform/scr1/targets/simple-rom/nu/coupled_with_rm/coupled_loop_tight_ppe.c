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
  int i, it_nmb;
  int res;

  uint32_t flying_mode, lbs, mv;

  rumboot_printf("coupled_loop_ppe\n");


  lbs = 0x0;
  mv = 0x0;

  flying_mode = ((lbs&0x3)<<1) | (mv&0x1);
  
  heap_id = nu_get_heap_id();
  
  rumboot_platform_request_file("num_iterations_file_tag", (uintptr_t) &it_nmb);
  rumboot_printf("it_nmb is %d\n", it_nmb);

  #if DUT_IS_NPE
    na_cu_set_units_direct_mode(NPE_BASE+NA_CU_REGS_BASE, NA_CU_PPE_UNIT_MODE);
  #endif

  nu_ppe_init_test_desc(&test_desc);
  
  if(nu_ppe_place_arrays(heap_id,&test_desc,it_nmb) !=0) return -1;
  
    // Prepare cfg_reg
  nu_ppe_init_iteration_desc(&test_desc,&iteration_desc);
  for (i=0; i<it_nmb; i++) {
    rumboot_printf("Deciding ConfigREGPPE For Iteration %d\n",i);
    
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
    
    nu_ppe_iterate_desc(&iteration_desc);
  }

    // Main Loop
  nu_ppe_init_iteration_desc(&test_desc,&iteration_desc);
  for (i=0; i<it_nmb; i++) {
    rumboot_printf("Starting Iteration %d\n",i);
    
    nu_ppe_setup_reg(MY_PPE_RDMA_BASE, MY_PPE_REGS_BASE, iteration_desc.cfg_reg);
    
    nu_ppe_dma_wait_ready_and_run(MY_PPE_RDMA_BASE);
    nu_ppe_dma_wait_ready_and_run(MY_PPE_REGS_BASE); // WDMA
    
    nu_ppe_iterate_desc(&iteration_desc);
  }
  
  nu_ppe_wait_complete(MY_PPE_REGS_BASE);
  
    // Data Compare Loop
  nu_ppe_init_iteration_desc(&test_desc,&iteration_desc);
  for (i=0; i<it_nmb; i++) {
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

    }
    else {
      rumboot_printf("Test FAILED at iteration %d\n", i);
      return 1;
    };
    
      //
    nu_ppe_iterate_desc(&iteration_desc);
  }
  
  return 0;
}


