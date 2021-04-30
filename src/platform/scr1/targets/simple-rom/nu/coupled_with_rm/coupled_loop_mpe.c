
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>

#include <platform/devices.h>

#include <platform/devices/nu_lib.h>
#include <platform/devices/nu_test_lib.h>
#include <regs/regs_nu_mpe.h>

#include "platform/devices/nu_mpe_file_tags.h"

void *in_data;
void *warr;
void *etalon;
void *res_data;
void *cmd;

CubeMetrics* in_metrics;
WarrMetrics* warr_metrics;
CubeMetrics* res_metrics;
MPECmdMetrics* cmd_metrics;

int main() {
  int heap_id;
  int i;
  uint32_t in_buffer_warr_offset;
  int iterations;
  uint32_t start;
  uint32_t end;
  uint32_t delta;
  uint32_t instr_number;
  uint32_t v;
  uint32_t vc;
  rumboot_printf("Hello\n");

  heap_id = nu_get_heap_id();

  rumboot_platform_request_file("num_iterations_file_tag",(uintptr_t) &iterations);
  rumboot_printf("Number of iterations %d\n",iterations);

  for (i = 0; i < iterations; i++) {
    rumboot_printf("Starting iteration %d\n",i);

    in_metrics = nu_load_cube_metrics(heap_id, metrics_in_tag[i]);
    if(in_metrics == NULL) return -1;

    res_metrics = nu_load_cube_metrics(heap_id, metrics_etalon_tag[i]);
    if(res_metrics == NULL) return -1;

    in_data = nu_load_cube(heap_id, in_file_tag[i],in_metrics);
    if(in_data == NULL) return -1;

    res_data = nu_mpe_malloc_res(heap_id, res_metrics);
    if(res_data == NULL) return -1;

    // Load Warr
    warr_metrics = nu_load_warr_metrics(heap_id, metrics_warr_tag[i]);
    if(warr_metrics == NULL) return -1;

    warr = nu_load_warr(heap_id, warr_file_tag[i], warr_metrics);
    if(warr == NULL) return -1;
    //

    cmd_metrics = nu_mpe_load_cmd_metrics_by_tag(heap_id, metrics_mpe_cmd_tag[i]);
    if(cmd_metrics == NULL) return -1;

    cmd = nu_mpe_load_cmd_by_tag(heap_id, mpe_cmd_file_tag[i], cmd_metrics);
    if(cmd == NULL) return -1;

    in_buffer_warr_offset = nu_mpe_get_warr_offset(cmd,cmd_metrics);

    rumboot_printf("Loading input buffer\n");
    nu_mpe_load_buf(NU_MPE_BUFFER_BASE+NU_MPE_BUF00,               in_data,in_metrics->s);
    nu_mpe_load_buf(NU_MPE_BUFFER_BASE+NU_MPE_BUF00+in_buffer_warr_offset, warr,warr_metrics->s);


    etalon = nu_load_cube(heap_id, etalon_file_tag[i], res_metrics);
    if(etalon == NULL) return -1;

    nu_mpe_config_wr_main_channel(NU_CPDMAC_ASM_BASE,res_data,res_metrics->s);
    nu_mpe_run_wr_main_channel(NU_CPDMAC_ASM_BASE);

    rumboot_printf("Runnung MPE commands\n");
    start = rumboot_platform_get_uptime();
    if( nu_mpe_run_cmd(NU_MPE_STANDALONE_BASE,cmd,cmd_metrics) != 0 ) return -1;

    nu_mpe_wait_wr_main_channel_complete(NU_CPDMAC_ASM_BASE);
    end = rumboot_platform_get_uptime();
    rumboot_platform_dump_region("res_data.bin",(uint32_t)res_data,res_metrics->s);

    delta = (end -start); // number of cycles in microseconds(ns), cycle =10ns;
    instr_number = 8 *128 * 1024; // 8 MAC istructions,128 - number of repeat,1024 -matrix weghts and data
    //rumboot_printf( " performance  by cycle\n");
    vc = (instr_number) / ((delta * 1000)/850);
    //rumboot_printf( "vc=%d\n",vc);

    //rumboot_printf( " performance  by sec\n");
    // v = (instr_number/delta) * 1000000;

    //rumboot_printf( "v=%d\n",v);

    //rumboot_printf( "time(mcsec)=%d\n",(end-start));
    //rumboot_printf( "start=%d\n",start);
    //rumboot_printf( "end=%d\n",end);

    //rumboot_printf( "instr_number=%d\n",instr_number);


    rumboot_printf("Comparing..\n");

    if(nu_bitwise_compare(res_data,etalon,res_metrics->s) == 0)
      rumboot_printf("Test PASSED\n");
    else {
      rumboot_printf("Test FAILED\n");
      return 1;
    }
  }
  return 0;

}
