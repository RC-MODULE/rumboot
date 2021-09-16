
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>

#include <platform/devices.h> 

#include <platform/devices/nu_lib.h> 
#include <platform/devices/nu_test_lib.h> 
#include <platform/devices/nu_test_macro.h> 

#include "platform/devices/nu_ppe_file_tags.h"


ConfigPPE     cfg;
ConfigREGPPE  cfg_reg = {0};

CubeMetrics*  in_metrics  = NULL;
CubeMetrics*  res_metrics = NULL;

void* in_data = NULL;
void* etalon  = NULL;
void* res_data= NULL;

int ppe_clk_f = 25; // MHz ppe clk frequency

int main() {
  int res = 0;
  int i, it_nmb, dtB;
  int clk_cnt;
  int perf_avg;

  int heap_id = nu_get_heap_id();

  rumboot_printf("coupled_loop_ppe_new\n");

  rumboot_platform_request_file("num_iterations_file_tag", (uintptr_t) &it_nmb);
  rumboot_printf("it_nmb is %d\n", it_nmb);

  //#if DUT_IS_NPE
  //  na_cu_set_units_direct_mode(NPE_BASE+NA_CU_REGS_BASE, NA_CU_PPE_UNIT_MODE);
  //#endif

  iowrite32(0xFACE2021, MY_PPE_REGS_BASE + NU_PPE_WDMA_BASE_ADDR);

  //uint32_t tmp;
  //tmp = ioread32(MY_PPE_REGS_BASE + NU_PPE_WDMA_BASE_ADDR);
  //rumboot_printf("%x\n", tmp);
  //
  //iowrite32(0xFACE2022, MY_PPE_REGS_BASE + NU_PPE_WDMA_BASE_ADDR);
  //tmp = ioread32(MY_PPE_REGS_BASE + NU_PPE_WDMA_BASE_ADDR);
  //rumboot_printf("%x\n", tmp);

  perf_avg = 0;
  for (i=0; i<it_nmb && !res; i++) {
    rumboot_malloc_update_heaps(1);

    nu_ppe_load_cfg_by_tag(heap_id, &cfg, cfg_file_tag[i]);

    #ifdef MEMtoMEM
    in_metrics  = nu_load_cube_metrics(heap_id, metrics_in_tag[i]);
    res_metrics = nu_load_cube_metrics (heap_id, metrics_etalon_tag[i]);

    if (in_metrics == NULL || res_metrics == NULL) res = 1;

    if (!res) {
      in_data     = nu_load_cube(heap_id, in_file_tag[i], in_metrics);
      etalon      = nu_load_cube (heap_id, etalon_file_tag[i], res_metrics);
      res_data    = nu_ppe_malloc_res(heap_id, res_metrics);
    }

    if (in_data == NULL || etalon == NULL || res_data == NULL) res = 1;

//    if (!res) {
//      cfg_reg.rBALs = (uintptr_t)in_data;
//      cfg_reg.wBALd = (uintptr_t)res_data;
//
//      cfg_reg.rOpEn = 0x1;  // is needed to set memory linear all-in-one
//      cfg_reg.wOpM = max_red << 28 | flying_mode << 8;
//      res = nu_ppe_decide_dma_config_trivial(&cfg, res_metrics, &cfg_reg);
//    }
//
//    if(!res){
//      nu_ppe_setup_reg(MY_PPE_RDMA_BASE, MY_PPE_REGS_BASE, &cfg_reg);
//
//      cfg_reg.rOpEn  = 0x1; // Set start of PPE RDMA field to active value
//      cfg_reg.wOpEn  = 0x1; // Set start of PPE+WDMA field to active value
//
//      clk_cnt = rumboot_platform_get_uptime();
//
//      // Start RDMA then PPE+WDMA
//      nu_ppe_rdma_run(MY_PPE_RDMA_BASE, &cfg_reg);
//      nu_ppe_run(MY_PPE_REGS_BASE, &cfg_reg);
//
//      while (nu_ppe_status_done_rd(MY_PPE_REGS_BASE) == 0x0) {} // set timeout
//      clk_cnt = rumboot_platform_get_uptime() - clk_cnt;
//
//      // Wait RDMA then PPE+WDMA
//      nu_ppe_rdma_wait_complete(MY_PPE_RDMA_BASE);
//      nu_ppe_wait_complete(MY_PPE_REGS_BASE);
//
//      // Sizeof(DataCube)/(time*frequency); time measure is us, frequency measure is MHz
//      // clk_cnt will be devided by frequency later
//      dtB = (cfg_reg.wOpM & 0x600000) == 0x0 ? 1 : 2; // sizeof(DataType) in bytes
//      clk_cnt = (in_metrics->H * in_metrics->W * in_metrics->C * dtB)/clk_cnt;
//    }
    #endif

    #if DUT_IS_PPE
    #ifdef VPEtoPPE
    in_metrics  = nu_load_cube_metrics(heap_id, metrics_in_tag[i]);
    res_metrics = nu_load_cube_metrics (heap_id, metrics_etalon_tag[i]);

    if (in_metrics == NULL || res_metrics == NULL) res = 1;

    if (!res) {
      in_data     = nu_load_cube(heap_id, in_ameba_file_tag[i], in_metrics);
      etalon      = nu_load_cube (heap_id, etalon_file_tag[i], res_metrics);
      res_data    = nu_ppe_malloc_res(heap_id, res_metrics);
    }

    if (in_data == NULL || etalon == NULL || res_data == NULL) res = 1;

    if (!res) {
      cfg_reg.rBALs = (uintptr_t)in_data;
      cfg_reg.wBALd = (uintptr_t)res_data;
    
      cfg_reg.rOpEn = 0x0; // to set wOpM to VPE Linear mode
      res = nu_ppe_decide_dma_config_trivial(&cfg, res_metrics, &cfg_reg);
    }

    if(!res){
      cfg_reg.wOpEn  = 0x1; // Set start of PPE+WDMA field to active value
      nu_ppe_setup_reg(MY_PPE_RDMA_BASE, MY_PPE_REGS_BASE, &cfg_reg);

      nu_cpdmac_trn256_config(NU_CPDMAC_ASM_BASE,in_data,in_metrics->s);

      nu_ppe_run(MY_PPE_REGS_BASE, &cfg_reg); 
      clk_cnt = rumboot_platform_get_uptime();

      nu_cpdmac_trn256_run(NU_CPDMAC_ASM_BASE);

      while (nu_ppe_status_done_rd(MY_PPE_REGS_BASE) == 0x0) {} // set timeout
      clk_cnt = rumboot_platform_get_uptime() - clk_cnt;

      nu_cpdmac_trn256_wait_complete(NU_CPDMAC_ASM_BASE);
      nu_ppe_wait_complete(MY_PPE_REGS_BASE);

      // Sizeof(DataCube)/(time*frequency); time measure is us, frequency measure is MHz
      // clk_cnt will be devided by frequency later
      dtB = (cfg_reg.wOpM & 0x600000) == 0x0 ? 1 : 2;
      clk_cnt = (in_metrics->H * in_metrics->W * in_metrics->C * dtB)/clk_cnt;
    }
    #endif
    #endif

//    if (!res) {
//      rumboot_printf("Comparing...\n");
//
//      res = nu_bitwise_compare(res_data,etalon,res_metrics->s);
//
//      if (res) {
//        nu_ppe_print_config(&cfg);
//        nu_ppe_print_config_reg(&cfg_reg);
//
//        rumboot_platform_dump_region("res_data.bin",(uint32_t)res_data,res_metrics->s);
//        rumboot_platform_dump_region("cfg_reg.bin", &cfg_reg, NU_PPE_REG_CFG_PARAMS_NUM*sizeof(uint32_t));
//      }
//    }

    if (!res) {
      rumboot_printf("Iteration %d PASSED\n", i);

      #ifdef ShowPerf
      perf_avg += clk_cnt;

      //rumboot_printf("clk_cnt is %d\n", clk_cnt);

      clk_cnt = (clk_cnt*100)/ppe_clk_f;

      rumboot_printf("PPE perfomance of iteration # %d is %d.%d bytes per cycle\n", i, clk_cnt/100, clk_cnt-(clk_cnt/100)*100);
      #endif
    }
    else rumboot_printf("Test FAILED at iteration %d\n", i);

    rumboot_malloc_update_heaps(0);
  }

  #ifdef ShowPerf
  if (it_nmb == 0) rumboot_printf("ShowPerf failed\n");
  else {
    perf_avg = (perf_avg*100)/it_nmb;

    rumboot_printf("PPE average perfomance of %d iterations is %d.%d bytes per cycle\n", it_nmb, perf_avg/100, perf_avg-(perf_avg/100)*100);
  }
  #endif

  return res;
}
