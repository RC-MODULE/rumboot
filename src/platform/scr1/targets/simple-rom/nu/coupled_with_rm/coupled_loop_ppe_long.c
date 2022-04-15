#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>

#include <platform/devices.h> 

#include <devices/rcm_na/nu_lib.h> 
#include <devices/rcm_na/nu_test_lib.h> 
#include <devices/rcm_na/nu_test_macro.h> 

#define SKIP_ITERATIONS 0

void nu_ppe_decide_dma_config(
  ConfigPPE* cfg, 
  CubeMetrics* res_metrics, 
  ConfigREGPPE* cfg_reg,
  uint32_t flying_mode,
  uint32_t max_red,
  void* in_data,
  void* res_data
) {
  cfg_reg->rBALi = nu_virt_to_dma(in_data);
  cfg_reg->wBALo = nu_virt_to_dma(res_data);

  cfg_reg->wOpM = flying_mode << 8;

  // OP_MODE[29] reserved bit is used
  if (max_red) cfg_reg->wOpM = cfg_reg->wOpM | 1<<29 | max_red << 28;

  nu_ppe_decide_dma_config_trivial(cfg, res_metrics, cfg_reg);
}

PPETestDescriptor test_desc;
PPEIterationDescriptor iteration_desc;

int main() {
  int heap_id ;
  int i, it_nmb, dtB;
  int clk_cnt, perf_avg;
  int res;
  int skip;

  uint32_t flying_mode, lbs, mv, max_red;
  uint32_t ppe_swrst_en, setup_reg, mark_cube;

  flying_mode = 0x0;
  lbs         = 0x0;
  mv          = 0x0;
  max_red     = 0x0;

  ppe_swrst_en= 0;
  setup_reg   = 1;
  mark_cube   = 0;

  rumboot_printf("coupled_loop_ppe_long\n");

  #ifdef LIN
    lbs = 0x0;
  #endif
  #ifdef BOX
    lbs = 0x1;
  #endif
  #ifdef LBS_NOT_SUPPORTED
  rumboot_printf("ERROR: Not supported path mode for this test choosen\n");
  return -1;
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

  #ifdef MAX_RED_0
    max_red = 0x1<<1 | 0x0;
  #endif
  #ifdef MAX_RED_1
    max_red = 0x1<<1 | 0x1;
  #endif

  flying_mode = ((lbs&0x3)<<1) | (mv&0x1);

  heap_id = nu_get_heap_id();

  rumboot_platform_request_file_ex("num_iterations_file_tag", (uintptr_t) &it_nmb,sizeof(it_nmb));
  rumboot_printf("it_nmb is %d\n", it_nmb);

  #if DUT_IS_NPE
    na_cu_set_units_direct_mode(NPE_BASE+NA_CU_REGS_BASE, NA_CU_PPE_UNIT_MODE);
  #endif

  nu_ppe_init_test_desc(&test_desc);

  res = nu_ppe_place_arrays(heap_id,&test_desc,it_nmb);

  if (!res) nu_ppe_init_iteration_desc(&test_desc,&iteration_desc);

  #ifndef SKIP_ITERATIONS
    skip=0;
  #else
    skip = SKIP_ITERATIONS;
  #endif
  
  for(i=0;i<skip;i++) {
    rumboot_printf("Skipping Iteration %d\n",i);
    nu_ppe_iterate_desc(&iteration_desc);
  }

  #ifdef PPE_SWRST
    ppe_swrst_en = 1;
  #else
    ppe_swrst_en = 0;
  #endif

  #if PPE_MARK_CUBE
    mark_cube = 1;
    nu_ppe_marked_page_cmpl_mask(MY_PPE_REGS_BASE);
  #else
    mark_cube = 0;
  #endif

  for (perf_avg=0, i=skip; i<it_nmb && !res; i++) {
    //rumboot_malloc_update_heaps(1);

    rumboot_printf("Starting Iteration %d\n",i);

    nu_ppe_decide_dma_config(
      iteration_desc.cfg,
      iteration_desc.res_metrics,
      iteration_desc.cfg_reg,
      flying_mode,
      max_red,
      iteration_desc.in_data,
      iteration_desc.res_data
    );

    iteration_desc.cfg_reg->rOpEn  = 0x0;
    iteration_desc.cfg_reg->wOpEn  = 0x0;

    #if SKIP_ITERATIONS != 0
      // Print Them When We Debug
      nu_ppe_print_config(iteration_desc.cfg);
      nu_ppe_print_config_reg(iteration_desc.cfg_reg);
    #endif

    if (setup_reg) nu_ppe_setup_reg(MY_PPE_RDMA_BASE, MY_PPE_REGS_BASE, iteration_desc.cfg_reg);

    #if PPE_MARK_CUBE
      if (mark_cube) {
        // MARKED_CNTX
        iteration_desc.cfg_reg->wOpEn = iteration_desc.cfg_reg->wOpEn | 0x1<<8;
      }
    #endif

    iteration_desc.cfg_reg->wOpEn  = iteration_desc.cfg_reg->wOpEn | 0x1;
    nu_ppe_wdma_run(MY_PPE_REGS_BASE, iteration_desc.cfg_reg); // wdma start

    #ifdef MEMtoPPE
      iteration_desc.cfg_reg->rOpEn  = iteration_desc.cfg_reg->rOpEn | 0x1;
      nu_ppe_rdma_run(MY_PPE_RDMA_BASE, iteration_desc.cfg_reg); // rdma start
    #endif

    clk_cnt = rumboot_platform_get_uptime();

    #ifdef PPE_PAUSE
      nu_na_ppe_pause_set(NPE_BASE+NA_CU_REGS_BASE);
      while (!nu_na_ppe_pause_status(NPE_BASE+NA_CU_REGS_BASE)) {}

      nu_na_ppe_pause_clr(NPE_BASE+NA_CU_REGS_BASE);
    #endif

    if (ppe_swrst_en) {

      nu_na_ppe_pause_set(NPE_BASE+NA_CU_REGS_BASE);
      while (!nu_na_ppe_pause_status(NPE_BASE+NA_CU_REGS_BASE)) {}

      nu_na_ppe_soft_reset_set(NPE_BASE+NA_CU_REGS_BASE);

      while (nu_na_ppe_soft_reset_status(NPE_BASE+NA_CU_REGS_BASE)){}

      nu_ppe_rdma_regs_swrst_check(MY_PPE_RDMA_BASE);
      nu_ppe_regs_swrst_check(MY_PPE_REGS_BASE);

      nu_na_ppe_pause_clr(NPE_BASE+NA_CU_REGS_BASE);

      ppe_swrst_en = 0;

      nu_ppe_iterate_desc(&iteration_desc);
    }
    else {
      while (nu_ppe_status_done(MY_PPE_REGS_BASE) == 0x0) {}
      clk_cnt = rumboot_platform_get_uptime() - clk_cnt;

      #ifdef MEMtoPPE
        nu_ppe_rdma_wait_complete(MY_PPE_RDMA_BASE);  // wait rdma complete
      #endif

      nu_ppe_wait_complete(MY_PPE_REGS_BASE);

      while (!nu_ppe_page_cmpl_status(MY_PPE_REGS_BASE)) {};
      nu_ppe_page_cmpl_reset(MY_PPE_REGS_BASE);

      dtB = (iteration_desc.cfg_reg->wOpM >> 16 & 0x3) ? 0x2 : 0x1;
      clk_cnt = (iteration_desc.in_metrics->H * iteration_desc.in_metrics->W * iteration_desc.in_metrics->C * dtB)/clk_cnt;

      rumboot_printf("Comparing...\n");
      res = nu_bitwise_compare(iteration_desc.res_data,iteration_desc.etalon,iteration_desc.res_metrics->s);

      if (res) {
        nu_ppe_print_config(iteration_desc.cfg);
        nu_ppe_print_config_reg(iteration_desc.cfg_reg);

        // rumboot_platform_dump_region("res_data.bin",(uint32_t)res_data,res_metrics->s);
        // rumboot_platform_dump_region("cfg_reg.bin", cfg_reg, NU_PPE_REG_CFG_PARAMS_NUM*sizeof(uint32_t));
      }

      if (!res) {

        #ifdef ShowPerf
          clk_cnt = (clk_cnt*100)/ppe_clk_f;
          perf_avg += clk_cnt;

          rumboot_printf("PPE perfomance of iteration # %d is %d.%d bytes per cycle\n", i, clk_cnt/100, clk_cnt-(clk_cnt/100)*100);
        #endif

        #ifdef PPE_CFG_CONST
          if (setup_reg) {
            memset(iteration_desc.res_data, 0xA5, iteration_desc.res_metrics);
            setup_reg = 0;

            i = i-1;
          }
          else {
            setup_reg = 1;
            nu_ppe_iterate_desc(&iteration_desc);
          }
        #else
          nu_ppe_iterate_desc(&iteration_desc);
        #endif

        #if PPE_MARK_CUBE
          if (mark_cube)  {
            mark_cube = 0;

            res = !(nu_ppe_marked_page_cmpl_status(MY_PPE_REGS_BASE) == 0x1);

            if (res) {
              rumboot_printf("No WDMA_MARKED_PAGE_CMPL status iteration %d\n", i);
              rumboot_printf("Test FAILED at iteration %d\n", i);
            }
            else nu_ppe_marked_page_cmpl_reset(MY_PPE_REGS_BASE);
          }
          else mark_cube = 1;
        #endif

        rumboot_printf("Iteration %d PASSED\n", i);
      }
      else rumboot_printf("Test FAILED at iteration %d\n", i);

      #ifdef PPE_SWRST
      ppe_swrst_en = 1;
      #endif
    }

    //rumboot_malloc_update_heaps(0);
  }

  #ifdef ShowPerf
  if (it_nmb == 0) rumboot_printf("ShowPerf failed\n");
  else {
    perf_avg = perf_avg/it_nmb;

    rumboot_printf("PPE average perfomance of %d iterations is %d.%d bytes per cycle\n", it_nmb, perf_avg/100, perf_avg-(perf_avg/100)*100);
  }
  #endif

  return res;
}
