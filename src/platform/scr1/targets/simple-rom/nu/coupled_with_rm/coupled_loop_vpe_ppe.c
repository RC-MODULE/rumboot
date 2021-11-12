
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>

#include <platform/devices.h> 

#include <devices/rcm_na/nu_lib.h> 
#include <devices/rcm_na/nu_test_lib.h> 
#include <devices/rcm_na/nu_test_macro.h> 

ConfigVPE     cfg_vpe;
ConfigPPE     cfg_ppe;

ConfigREGPPE  cfg_ppe_reg = {0};

CubeMetrics*  in_metrics  = NULL;
CubeMetrics*  res_metrics = NULL;

void* in_data = NULL;
void* etalon  = NULL;
void* res_data= NULL;

int ppe_clk_f = 25; // MHz ppe clk frequency

int main() {
  int res = 0;
  int i, it_nmb, dtB;
  int clk_cnt, perf_avg;

  uint32_t flying_mode, lbs, mv;

  char* fn_in_base          = "in_file_tag_"        ;
  char* fn_cfg_vpe_base     = "cfg_file_tag_"       ;
  char* fn_cfg_ppe_base     = "cfg_ppe_file_tag_"   ;
  char* fn_in_metrics_base  = "metrics_in_tag_"     ;
  char* fn_res_metrics_base = "metrics_etalon_tag_" ;
  char* fn_etalon_base      = "etalon_file_tag_"    ;

  char  fn_in[32];
  char  fn_cfg_vpe[32];
  char  fn_cfg_ppe[32];
  char  fn_in_metrics[32];
  char  fn_res_metrics[32];
  char  fn_etalon[32];

  int heap_id = nu_get_heap_id();

  rumboot_printf("coupled_loop_ppe\n");

  rumboot_platform_request_file("num_iterations_file_tag", (uintptr_t) &it_nmb);
  rumboot_printf("it_nmb is %d\n", it_nmb);

  #if DUT_IS_NPE
    na_cu_set_units_direct_mode(NPE_BASE+NA_CU_REGS_BASE, NA_CU_VPE_UNIT_MODE);
    na_cu_set_units_direct_mode(NPE_BASE+NA_CU_REGS_BASE, NA_CU_PPE_UNIT_MODE);
  #endif

  #ifdef LIN
    lbs = 0x0;
  #endif
  #ifdef BOX
    lbs = 0x1;
  #endif

  mv = 0x1;

  flying_mode = (lbs&0x3)<<1 | mv&0x1;

  perf_avg = 0;
  for (i=0; i<it_nmb && !res; i++) {
    rumboot_malloc_update_heaps(1);

    fn_base_it_nmb(fn_in, fn_in_base, i);
    fn_base_it_nmb(fn_cfg_vpe, fn_cfg_vpe_base, i);
    fn_base_it_nmb(fn_cfg_ppe, fn_cfg_ppe_base, i);
    fn_base_it_nmb(fn_in_metrics, fn_in_metrics_base, i);
    fn_base_it_nmb(fn_res_metrics, fn_res_metrics_base, i);
    fn_base_it_nmb(fn_etalon, fn_etalon_base, i);

    nu_vpe_load_cfg_by_tag(heap_id, &cfg_vpe, fn_cfg_vpe);
    nu_ppe_load_cfg_by_tag(heap_id, &cfg_ppe, fn_cfg_ppe);

    in_metrics  = nu_load_cube_metrics(heap_id, fn_in_metrics);
    res_metrics = nu_load_cube_metrics (heap_id, fn_res_metrics);

    if (in_metrics == NULL || res_metrics == NULL) res = 1;

    if (!res) {
      in_data     = nu_load_cube(heap_id, fn_in, in_metrics);
      etalon      = nu_load_cube (heap_id, fn_etalon, res_metrics);
      res_data    = nu_ppe_malloc_res(heap_id, res_metrics);
    }

    if (in_data == NULL || etalon == NULL || res_data == NULL) res = 1;

    if (!res) {
      cfg_ppe_reg.wBALo = (uintptr_t)res_data;
    
      cfg_ppe_reg.wOpM = flying_mode << 8;
      nu_ppe_decide_dma_config_trivial(&cfg_ppe, res_metrics, &cfg_ppe_reg);
    }

    if(!res){
      //nu_ppe_print_config_reg(&cfg_ppe_reg);

      nu_ppe_setup_reg(MY_PPE_RDMA_BASE, MY_PPE_REGS_BASE, &cfg_ppe_reg);
    
      cfg_ppe_reg.wOpEn  = 0x1;
      nu_ppe_wdma_run(MY_PPE_REGS_BASE, &cfg_ppe_reg); // wdma start

      #if DUT_IS_PPE
      nu_cpdmac_trn256_config(NU_CPDMAC_ASM_BASE,in_data,in_metrics->s);
      nu_cpdmac_trn256_run(NU_CPDMAC_ASM_BASE);
      #endif

      #if DUT_IS_NPE

      cfg_vpe.op0_rdma_config.dma_data_mode = cfg_vpe.op0_config.mux_mode; // Init Them
      cfg_vpe.op1_rdma_config.dma_data_mode = cfg_vpe.op1_config.mux_mode;
      cfg_vpe.op2_rdma_config.dma_data_mode = cfg_vpe.op2_config.mux_mode;
      cfg_vpe.wdma_config.dma_data_mode     = 0x0;
      cfg_vpe.wdma_config.dma_data_use      = 0x0;
      cfg_vpe.op0_rdma_config.dma_baddr     = 0x0;
      cfg_vpe.op1_rdma_config.dma_baddr     = 0x0;
      cfg_vpe.op2_rdma_config.dma_baddr     = 0x0;
      cfg_vpe.wdma_config.dma_baddr         = 0x0;
      cfg_vpe.op0_rdma_config.dma_axi_len   = 0x0;
      cfg_vpe.op1_rdma_config.dma_axi_len   = 0x0;
      cfg_vpe.op2_rdma_config.dma_axi_len   = 0x0;
      cfg_vpe.wdma_config.dma_axi_len       = 0x0;

      cfg_vpe.src_flying = Enable_NotEn;
      cfg_vpe.src_flying = Enable_NotEn;

      if      (lbs == 0x0)  cfg_vpe.trace_mode = TraceMode_PPE;
      else if (lbs == 0x1)  cfg_vpe.trace_mode = TraceMode_MPE;

      cfg_vpe.dst_flying = Enable_En;

      nu_vpe_decide_dma_config_trivial(&cfg_vpe,in_metrics);

      cfg_vpe.src_rdma_config.dma_baddr = (uint32_t)in_data;
      cfg_vpe.src_rdma_config.dma_axi_len = 0xF; // axi_len

      nu_vpe_setup(MY_VPE_REGS_BASE, &cfg_vpe);

      nu_vpe_run(MY_VPE_REGS_BASE, &cfg_vpe);
      #endif

      clk_cnt = rumboot_platform_get_uptime();
    
      while (nu_ppe_status_done(MY_PPE_REGS_BASE) == 0x0) {} // set timeout
      clk_cnt = rumboot_platform_get_uptime() - clk_cnt;

      #if DUT_IS_NPE
      nu_vpe_wait(MY_VPE_REGS_BASE, &cfg_vpe);
      #endif

      #if DUT_IS_PPE
      nu_cpdmac_trn256_wait_complete(NU_CPDMAC_ASM_BASE);
      #endif

      nu_ppe_wait_complete(MY_PPE_REGS_BASE);
    
      // Sizeof(DataCube)/(times*frequency); time measure is us, frequency measure is MHz
      // clk_cnt will be devided by frequency later
      dtB = (cfg_ppe_reg.wOpM >> 16 & 0x3) ? 0x2 : 0x1;
      clk_cnt = (in_metrics->H * in_metrics->W * in_metrics->C * dtB)/clk_cnt;
    
      rumboot_printf("Comparing...\n");
    
      res = nu_bitwise_compare(res_data,etalon,res_metrics->s);
    
      if (res) {
        nu_ppe_print_config(&cfg_ppe);
        nu_ppe_print_config_reg(&cfg_ppe_reg);

        nu_vpe_print_config(&cfg_vpe);
        nu_vpe_print_config_dma(&(cfg_vpe.src_rdma_config));
      
        rumboot_platform_dump_region("res_data.bin",(uint32_t)res_data,res_metrics->s);
        rumboot_platform_dump_region("cfg_ppe_reg.bin", &cfg_ppe_reg, NU_PPE_REG_CFG_PARAMS_NUM*sizeof(uint32_t));
      }
    }

    if (!res) {
      rumboot_printf("Iteration %d PASSED\n", i);

      #ifdef ShowPerf
      clk_cnt = (clk_cnt*100)/ppe_clk_f;
      perf_avg += clk_cnt;

      rumboot_printf("PPE perfomance of iteration # %d is %d.%d bytes per cycle\n", i, clk_cnt/100, clk_cnt-(clk_cnt/100)*100);
      #endif
    }
    else rumboot_printf("Test FAILED at iteration %d\n", i);

    rumboot_malloc_update_heaps(0);
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
