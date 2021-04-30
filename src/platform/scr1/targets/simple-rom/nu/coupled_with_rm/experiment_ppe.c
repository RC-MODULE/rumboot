
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>

#include <platform/devices.h> 

#include <platform/devices/nu_lib.h> 
#include <platform/devices/nu_test_lib.h> 

#include "platform/devices/nu_ppe_file_tags.h"

ConfigREGPPE  cfg_reg = {0};

CubeMetrics*  in_metrics  = NULL;
CubeMetrics*  res_metrics = NULL;

void* in_data = NULL;
void* etalon  = NULL;
void* res_data= NULL;

int main() {
  int res = 0;
  int i, it_nmb, dtB;
  int clk_cnt;

  int heap_id = nu_get_heap_id();;

  int FM; // 0 FLYING_BOXED 1 FLYING_LINEAR 2 OFFLY_LINEAR

  rumboot_printf("coupled_pytorch_ppe\n");

  rumboot_platform_request_file("num_iterations_file_tag", (uintptr_t) &it_nmb);
  rumboot_printf("it_nmb is %d\n", it_nmb);

  for (i=0; i<it_nmb && !res; i++) {
    rumboot_malloc_update_heaps(1);

    res = nu_ppe_load_cfg_reg_by_tag(heap_id, &cfg_reg, cfg_file_tag[i]);

    if (!res) {
      FM = (cfg_reg.wOpM & 0x30) >> 4;
      //rumboot_printf("FM is %d\n", FM);

      in_metrics =
        (FM == 0x0) ? nu_load_cube_metrics(heap_id, metrics_in_ameba_tag[i]) :
        (FM == 0x1) ? nu_load_cube_metrics(heap_id, metrics_in_tag[i]) :
        (FM == 0x2) ? nu_load_cube_metrics(heap_id, metrics_in_tag[i]) : NULL;
        //(FM == 0x2) ? nu_load_cube_metrics(heap_id, metrics_in_with_unused_tag[i]) : NULL;

      res_metrics =
        (FM == 0x0) ?
          nu_load_cube_metrics (heap_id, metrics_etalon_ameba_tag[i]) :
        (FM == 0x1 || FM == 0x2) ?
          nu_load_cube_metrics (heap_id, metrics_etalon_tag[i]) : NULL;
          //nu_load_cube_metrics (heap_id, metrics_etalon_with_unused_tag[i]) : NULL;

      if (in_metrics == NULL || res_metrics == NULL) res = 1;
    }

    if (!res) {

      in_data =
        (FM == 0x0) ? nu_load_cube(heap_id, in_ameba_file_tag[i], in_metrics) :
        (FM == 0x1) ? nu_load_cube(heap_id, in_file_tag[i], in_metrics) :
        (FM == 0x2) ? nu_load_cube(heap_id, in_file_tag[i], in_metrics) : NULL;
        //(FM == 0x2) ? : nu_load_cube(heap_id, in_with_unused_file_tag[i], in_metrics) : NULL;

      etalon =
        (FM == 0x0) ?
          nu_load_cube(heap_id, etalon_ameba_file_tag[i], in_metrics) :
        (FM == 0x1 || FM == 0x2) ?
          nu_load_cube(heap_id, etalon_file_tag[i], res_metrics) : NULL;
          //nu_load_cube(heap_id, etalon_with_unused_file_tag[i], res_metrics) : NULL;

      res_data = nu_ppe_malloc_res(heap_id, res_metrics);
    }

    if (in_data == NULL || etalon == NULL || res_data == NULL) res = 1;

    if(!res){
      cfg_reg.rBALs = (uintptr_t)in_data;
      cfg_reg.wBALd = (uintptr_t)res_data;

      nu_ppe_setup_reg(NU_PPE_RDMA_BASE, NU_PPE_STANDALONE_BASE, &cfg_reg);

      if (FM == 0x2) {
        cfg_reg.rOpEn  = 0x1; // Set start of PPE RDMA field to active value
        cfg_reg.wOpEn  = 0x1; // Set start of PPE+WDMA field to active value

        clk_cnt = rumboot_platform_get_uptime();

        // Start RDMA then PPE+WDMA
        nu_ppe_rdma_run(NU_PPE_RDMA_BASE, &cfg_reg);
        nu_ppe_run(NU_PPE_STANDALONE_BASE, &cfg_reg);

        while (nu_ppe_status_done_rd(NU_PPE_STANDALONE_BASE) == 0x0) {} // set timeout
        clk_cnt = rumboot_platform_get_uptime() - clk_cnt;

        // Wait RDMA then PPE+WDMA
        nu_ppe_rdma_wait_complete(NU_PPE_RDMA_BASE);
        nu_ppe_wait_complete(NU_PPE_STANDALONE_BASE);

        dtB = cfg_reg.wOpM & 0x600000 == 0x0 ? 1 : 2; // sizeof(DataType) in bytes

        // (cube size in byte) / (clk numbers * 16B/clk); 16B - 128-bit wdata bus
        //clk_cnt = (in_metrics->H * in_metrics->W * in_metrics->C * dtB) / (clk_cnt*16);

        clk_cnt = (in_metrics->H * in_metrics->W * in_metrics->C * dtB)/(clk_cnt*100);
      }
    }

    if (!res) {
      rumboot_printf("Comparing...\n");

      res = nu_bitwise_compare(res_data,etalon,res_metrics->s);

      if (res) {
        nu_ppe_print_config_reg(&cfg_reg);
        rumboot_platform_dump_region("res_data.bin",(uint32_t)res_data,res_metrics->s);
      }
    }

    if (!res) {
      rumboot_printf("Iteration %d PASSED\n", i);

      #ifdef ShowPerf
      rumboot_printf("PPE perfomance is %d bytes per cycle\n", clk_cnt);
      #endif
    }
    else rumboot_printf("Test FAILED at iteration %d\n", i);

    rumboot_malloc_update_heaps(0);
  }

  return res;
}
