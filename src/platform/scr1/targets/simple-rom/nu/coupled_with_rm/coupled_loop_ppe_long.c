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
  if (max_red) cfg_reg->wOpM = cfg_reg->wOpM | max_red << 28;

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

  uint32_t flying_mode, lbs, vpe_mem, max_red, ppe_10_clk_t;
  uint32_t ppe_swrst_en, setup_reg, mark_cube, resperr;

  flying_mode = 0x0;
  lbs         = 0x0;
  vpe_mem     = 0x0;
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
  #ifdef SPL
    lbs = 0x2;
  #endif

  #ifdef MEMtoPPE
    vpe_mem = 0x0;
  #endif
  #ifdef VPEtoPPE
    vpe_mem = 0x1;
  #endif

  #ifdef MAX_RED_0
    max_red = 0x1<<1 | 0x0;
  #endif
  #ifdef MAX_RED_1
    max_red = 0x1<<1 | 0x1;
  #endif

  heap_id = nu_get_heap_id();

  rumboot_platform_request_file_ex("num_iterations_file_tag", (uintptr_t) &it_nmb,sizeof(it_nmb));
  rumboot_printf("it_nmb is %d\n", it_nmb);

  #if DUT_IS_NPE
    na_cu_set_units_direct_mode(NPE_BASE+NA_CU_REGS_BASE, NA_CU_PPE_UNIT_MODE);
  #endif

  nu_ppe_init_test_desc(&test_desc);

  res = nu_ppe_place_arrays(heap_id,&test_desc,it_nmb);

  if (!res) nu_ppe_init_iteration_desc(&test_desc,&iteration_desc);

  #ifdef LIN_SPL
    int Wout = iteration_desc.res_metrics->W;
    int Wout_splt_1 = Wout_splt_calc(iteration_desc.cfg->Kh, iteration_desc.cfg->Sh, 0x1);
    int Wout_splt_2 = Wout_splt_calc(iteration_desc.cfg->Kh, iteration_desc.cfg->Sh, 0x2);

    //perf 128b/clk; rdata 128b
    //8*rdctn*16b; 16*rdctn*8b
    //iteration_desc.cfg->C

    if (Wout > Wout_splt_1) { // SPL
      lbs = 0x2;
      max_red = 0x1<<1 | 0x0; // rdctn 1 || rdctn 2

      rumboot_printf("LIN_SPL SPL rdctn 1\n");
    }
    else if (Wout > Wout_splt_2) {  // LIN
      lbs = 0x0;
      max_red = 0x1<<1 | 0x0; // rdctn 1

      rumboot_printf("LIN_SPL LIN rdctn 1\n");
    }
    else {  // LIN
      lbs = 0x0;
      max_red = 0x1<<1 | 0x1; // rdctn 1 || rdctn 2

      rumboot_printf("LIN_SPL LIN rdctn 2\n");
    }

    vpe_mem = 0x0;
  #endif

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

  #ifdef ShowPerf
    ppe_10_clk_t = get_nmb_10_clk_t();

    rumboot_printf("ppe_10_clk_t %d\n", ppe_10_clk_t);
  #else
    ppe_10_clk_t = 1;
  #endif

  #ifdef RESP_ERR
    resperr = 1;
  #else
    resperr = 0;
  #endif

  nu_ppe_wdma_err_mask(MY_PPE_REGS_BASE);
  nu_ppe_rdma_err_mask(MY_PPE_REGS_BASE);

  flying_mode = ((lbs&0x3)<<1) | (vpe_mem&0x1);

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

//    #if SKIP_ITERATIONS != 0
//      nu_ppe_print_config(iteration_desc.cfg);
//      nu_ppe_print_config_reg(iteration_desc.cfg_reg);
//    #endif

    if (setup_reg) nu_ppe_setup_reg(MY_PPE_RDMA_BASE, MY_PPE_REGS_BASE, iteration_desc.cfg_reg);

    #if PPE_MARK_CUBE
      if (mark_cube) {
        // MARKED_CNTX
        iteration_desc.cfg_reg->wOpEn = iteration_desc.cfg_reg->wOpEn | 0x1<<8;
      }
    #endif

    iteration_desc.cfg_reg->wOpEn  = iteration_desc.cfg_reg->wOpEn | 0x1;
    nu_ppe_wdma_run(MY_PPE_REGS_BASE, iteration_desc.cfg_reg); // wdma start

    clk_cnt = nu_get_uptime_ns();
    rumboot_printf("clk_cnt %d\n", clk_cnt);

    if (!vpe_mem) {
      iteration_desc.cfg_reg->rOpEn  = iteration_desc.cfg_reg->rOpEn | 0x1;
      nu_ppe_rdma_run(MY_PPE_RDMA_BASE, iteration_desc.cfg_reg); // rdma start
    }

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

      res = nu_ppe_rdma_regs_swrst_check(MY_PPE_RDMA_BASE);
      if (!res) res = nu_ppe_regs_swrst_check(MY_PPE_REGS_BASE);

      nu_na_ppe_pause_clr(NPE_BASE+NA_CU_REGS_BASE);

      ppe_swrst_en = 0;

      nu_ppe_iterate_desc(&iteration_desc);
    }
    else {
      while (nu_ppe_status_done(MY_PPE_REGS_BASE) == 0x0) {}

      clk_cnt = nu_get_uptime_ns() - clk_cnt;
      rumboot_printf("diff clk_cnt %d\n", clk_cnt);

      if (!vpe_mem) nu_ppe_rdma_wait_complete(MY_PPE_RDMA_BASE);  // rdma finish

      nu_ppe_wait_complete(MY_PPE_REGS_BASE);

      while (!nu_ppe_page_cmpl_status(MY_PPE_REGS_BASE)) {};
      nu_ppe_page_cmpl_reset(MY_PPE_REGS_BASE);

      res = nu_ppe_wdma_err_status(MY_PPE_REGS_BASE) ||
            nu_ppe_rdma_err_status(MY_PPE_RDMA_BASE)
      ;
      if (res) rumboot_printf("There is an error on AXI bus at iteration %d\n", i);

      if (resperr) res = 0;

      if (!res) {
        rumboot_printf("Comparing...\n");
        res = nu_bitwise_compare(iteration_desc.res_data,iteration_desc.etalon,iteration_desc.res_metrics->s);
      }

      if (!res) {
        #if PPE_NAN_INF
          res = nu_ppe_status_regs_check(MY_PPE_REGS_BASE, iteration_desc.status_regs_etalon);
        #endif
      }

      if (!res) {
        #if PPE_MARK_CUBE
          if (mark_cube) {
            mark_cube = 0;

            res = !(nu_ppe_marked_page_cmpl_status(MY_PPE_REGS_BASE) == 0x1);

            if (res) rumboot_printf("No WDMA_MARKED_PAGE_CMPL status iteration %d\n", i);
            else nu_ppe_marked_page_cmpl_reset(MY_PPE_REGS_BASE);
          }
          else mark_cube = 1;
        #endif
      }

      if (!res) {
        rumboot_printf("Iteration %d PASSED\n", i);

        #ifdef ShowPerf
          dtB = (iteration_desc.cfg_reg->wOpM >> 16 & 0x3) ? 0x2 : 0x1;

          if (clk_cnt > 0 && ppe_10_clk_t > 0) {
            clk_cnt = (iteration_desc.in_metrics->H * iteration_desc.in_metrics->W * iteration_desc.in_metrics->C * dtB)/clk_cnt;

            clk_cnt = (clk_cnt*100*10)/ppe_10_clk_t;
            perf_avg += clk_cnt;

            rumboot_printf("PPE perfomance of iteration # %d is %d.%d bytes per cycle\n", i, clk_cnt/100, clk_cnt-(clk_cnt/100)*100);
          }
          else rumboot_printf("Unable to determine PPE perfomance\n");

        #endif

        #ifdef PPE_CFG_CONST
          if (setup_reg) {
            memset(iteration_desc.res_data, 0xA5, iteration_desc.res_metrics->s);
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
      }
      else {
        nu_ppe_print_config(iteration_desc.cfg);
        nu_ppe_print_config_reg(iteration_desc.cfg_reg);

        // rumboot_platform_dump_region("res_data.bin",(uint32_t)res_data,res_metrics->s);
        // rumboot_platform_dump_region("cfg_reg.bin", cfg_reg, NU_PPE_REG_CFG_PARAMS_NUM*sizeof(uint32_t));

        rumboot_printf("Test FAILED at iteration %d\n", i);
      }

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
