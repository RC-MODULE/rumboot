
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>

#include <platform/devices.h> 

#include <platform/devices/nu_lib.h> 
#include <platform/devices/nu_test_lib.h> 
#include <regs/regs_nu_mpe.h>

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
  
  rumboot_printf("Hello\n");
  
  heap_id = nu_get_heap_id();
  
  in_metrics = nu_mpe_load_in_metrics(heap_id);
  if(in_metrics == NULL) return -1;
  
  res_metrics= nu_mpe_load_res_metrics(heap_id);
  if(res_metrics == NULL) return -1;
  
  in_data = nu_mpe_load_in_data(heap_id,in_metrics);
  if(in_data == NULL) return -1;
  
  res_data = nu_mpe_malloc_res(heap_id, res_metrics);
  if(res_data == NULL) return -1;
  
    // Load Warr
  warr_metrics = nu_mpe_load_warr_metrics(heap_id);
  if(warr_metrics == NULL) return -1;
  
  warr = nu_mpe_load_warr(heap_id,warr_metrics);
  if(warr == NULL) return -1;
   //
  
  nu_mpe_load_buf(NU_MPE_BUFFER_BASE+NU_MPE_BUF00,               in_data,in_metrics->s);
  nu_mpe_load_buf(NU_MPE_BUFFER_BASE+NU_MPE_BUF00+in_metrics->s, warr   ,warr_metrics->s);  // Подряд
  
  
  etalon = nu_mpe_load_etalon(heap_id,res_metrics);
  if(etalon == NULL) return -1;
  
  cmd_metrics = nu_mpe_load_cmd_metrics(heap_id);
  if(cmd_metrics == NULL) return -1;
  
  cmd = nu_mpe_load_cmd(heap_id,cmd_metrics);
  if(cmd == NULL) return -1;
  
  nu_mpe_config_wr_main_channel(NU_CPDMAC_ASM_BASE,res_data,res_metrics->s);
  nu_mpe_run_wr_main_channel(NU_CPDMAC_ASM_BASE);
  
  if( nu_mpe_run_cmd(NU_MPE_STANDALONE_BASE,cmd,cmd_metrics) != 0 ) return -1;
  
  nu_mpe_wait_wr_main_channel_complete(NU_CPDMAC_ASM_BASE);
  
  rumboot_platform_dump_region("res_data.bin",(uint32_t)res_data,res_metrics->s);
  
  rumboot_printf("Comparing..\n");
  
  if(nu_bitwise_compare(res_data,etalon,res_metrics->s) == 0)
    rumboot_printf("Test PASSED\n");
  else {
    rumboot_printf("Test FAILED\n");
    return 1;
  }
  
  return 0;
  
}
