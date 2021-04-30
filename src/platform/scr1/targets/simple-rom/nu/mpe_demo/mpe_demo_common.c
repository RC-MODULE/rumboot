
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
  uint32_t start_time;
  uint32_t inst_masked = 0x141 & 0xffffffc1 ;
  uint32_t result;
  ptr = (uint32_t*) cmd;
  num_cmds = (cmd_metrics->s / 8);
  rumboot_printf("num_cmds=%x addr = %x\n", num_cmds, offset);
  for(int i=0;i<num_cmds;i++) {
    data = *ptr;
    ptr++;
    offset = *ptr;

	if (offset == inst_masked)	
    {start_time = rumboot_platform_get_uptime();
	rumboot_printf("start_time = %d addr = %x n", start_time, offset);}		
    ptr++;
    rumboot_printf("Writing data=%x addr = %x  num_cmds=%x\n", data, offset, num_cmds);
	iowrite32(data,base + (offset*4));
  }
  result = start_time;
  return result;
}
int  nu_mpe_cmd(uintptr_t base, void* cmd, MPECmdMetrics* cmd_metrics) {
  uint32_t offset;
  uint32_t data;
  int num_cmds;
  uint32_t* ptr;
  uint32_t start_instr;
  uint32_t inst_masked = 0x141 & 0xffffffc1;
  uint32_t result;
  
  ptr = (uint32_t*) cmd;
  num_cmds = (cmd_metrics->s / 8);
  rumboot_printf("inst_masked=%d addr = %x\n", inst_masked, offset);
  for(int i=0;i<num_cmds;i++) {
    ptr++;
    offset = *ptr;
	if (offset == inst_masked)
    {start_instr = num_cmds -(i +1);  //number of the executed instructions
 	rumboot_printf("start_instr = %x addr = %x n", i, offset);}		
    ptr++;

  }
  result = start_instr;
  return result;
  }


int main() {
  int heap_id;
  uint32_t in_buffer_warr_offset;
  
  uint32_t start;
  uint32_t end;
  uint32_t delta;
  uint32_t instr_number;

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

  start_inst = nu_mpe_cmd(NU_MPE_STANDALONE_BASE,cmd,cmd_metrics);
  start = nu_run_mpe_cmd(NU_MPE_STANDALONE_BASE,cmd,cmd_metrics);

  
  nu_mpe_wait_wr_main_channel_complete(NU_CPDMAC_ASM_BASE);
  end = rumboot_platform_get_uptime();
  rumboot_platform_dump_region("res_data.bin",(uint32_t)res_data,res_metrics->s);
  
  delta = (end -start); // number of cycles in microseconds, cycle_APB_timer =10ns;
  instr_number = start_inst *128 * 1024; // 8 MAC istructions,128 - number of repeat,1024 - min number of cycles 
 
  cycle_num_inst = (delta * 1000)/650 ;    //650 ns - one cycle period

  vc = instr_number/( cycle_num_inst );
  
  rumboot_printf(" \n");
  rumboot_printf( "performance=%d",vc);
  rumboot_printf(" MAC/cycle \n");
  rumboot_printf(" \n");
  rumboot_printf("Comparing..\n");
  
  if(nu_bitwise_compare(res_data,etalon,res_metrics->s) == 0)
    rumboot_printf("Test PASSED\n");
  else {
    rumboot_printf("Test FAILED\n");
    return 1;
  }
  
  return 0;
  
}
