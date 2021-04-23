
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

int  nu_run_mpe_cmd(uintptr_t base, void* cmd, MPECmdMetrics* cmd_metrics) {
  uint32_t offset;
  uint32_t data;
  int num_cmds;
  uint32_t* ptr;

  ptr = (uint32_t*) cmd;
  num_cmds = (cmd_metrics->s / 8) - 8;
  rumboot_printf("num_cmds=%x addr = %x\n", num_cmds, offset);
  for(int i=0;i<num_cmds;i++) {
    data = *ptr;
    ptr++;
    offset = *ptr;
    ptr++;
    rumboot_printf("Writing data=%x addr = %x  num_cmds=%x\n", data, offset, num_cmds);
	iowrite32(data,base + (offset*4));
  }
  return 0;
}
int main() {
  int heap_id;
  uint32_t in_buffer_warr_offset;
  
  uint32_t start;
  uint32_t end;
  uint32_t delta;
  uint32_t instr_number;
  uint32_t k; 
  uint32_t vc;
  uint32_t start_inst;
  uint32_t cycle_num_inst;
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
  
  cmd_metrics = nu_mpe_load_cmd_metrics(heap_id);
  if(cmd_metrics == NULL) return -1;
  
  cmd = nu_mpe_load_cmd(heap_id,cmd_metrics);
  if(cmd == NULL) return -1;
  
  in_buffer_warr_offset = nu_mpe_get_warr_offset(cmd,cmd_metrics);
  
  rumboot_printf("Loading input buffer\n");
  nu_mpe_load_buf(NU_MPE_BUFFER_BASE+NU_MPE_BUF00,               in_data,in_metrics->s);
  nu_mpe_load_buf(NU_MPE_BUFFER_BASE+NU_MPE_BUF00+in_buffer_warr_offset, warr,warr_metrics->s);
  
  
  etalon = nu_mpe_load_etalon(heap_id,res_metrics);
  if(etalon == NULL) return -1;
  
  nu_mpe_config_wr_main_channel(NU_CPDMAC_ASM_BASE,res_data,res_metrics->s);
  nu_mpe_run_wr_main_channel(NU_CPDMAC_ASM_BASE);
    
  rumboot_printf("Runnung MPE commands\n");
 start_inst = rumboot_platform_get_uptime(); 
 if( nu_run_mpe_cmd(NU_MPE_STANDALONE_BASE,cmd,cmd_metrics) != 0 ) return -1;
  
  start = rumboot_platform_get_uptime();
  if( nu_mpe_run_cmd(NU_MPE_STANDALONE_BASE,cmd,cmd_metrics) != 0 ) return -1;
  
  nu_mpe_wait_wr_main_channel_complete(NU_CPDMAC_ASM_BASE);
  end = rumboot_platform_get_uptime();
  rumboot_platform_dump_region("res_data.bin",(uint32_t)res_data,res_metrics->s);
  
  delta = (end -start); // number of cycles in microseconds(ns), cycle_APB_timer =10ns;
  instr_number = 8 *128 * 1024; // 8 MAC istructions,128 - number of repeat,1024 -matrix weghts and data 

  rumboot_printf( " performance  by cycle\n");
  cycle_num_inst = (((delta - (start-start_inst))) * 1000)/650;
 
  rumboot_printf( "time(mcsec)=%d\n",(end-start));
  rumboot_printf( "start=%d\n",start);
  rumboot_printf( "end=%d\n",end);
  rumboot_printf( "start_inst=%d\n",start_inst);
  rumboot_printf( "time_inst=%d\n",(start-start_inst));
  rumboot_printf( "instr_number=%d\n",instr_number);
  rumboot_printf( "cycle_num_inst=%d\n",cycle_num_inst);
  
  vc = instr_number/( cycle_num_inst );
 // rumboot_printf( "vc=%d\n",vc);

  vc = (vc * 660)/650; //consider with coeff of apb timer cycle count correct
  rumboot_printf( "vc_final=%d\n",vc);
  rumboot_printf("Comparing..\n");
  
  if(nu_bitwise_compare(res_data,etalon,res_metrics->s) == 0)
    rumboot_printf("Test PASSED\n");
  else {
    rumboot_printf("Test FAILED\n");
    return 1;
  }
  
  return 0;
  
}
