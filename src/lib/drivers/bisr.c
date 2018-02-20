#include <stdint.h>
#include <stdbool.h>

#include <devices/sctl.h>
#include <devices/bisr.h>

#include <rumboot/io.h>
#include <rumboot/timer.h>
#include <rumboot/printf.h>

#include <platform/devices.h>

#define BISR_TIMEOUT 50000

void bisr_prog_start()
{
	uint32_t sctlreg = ioread32(SCTL_BASE + SCTL_BISR_L2C);

	iowrite32(sctlreg | (1 << SCTL_PROG_START_i), SCTL_BASE + SCTL_BISR_L2C);
}

void bisr_prog_stop()
{
	uint32_t sctlreg = ioread32(SCTL_BASE + SCTL_BISR_L2C);

	iowrite32(sctlreg & ~(1 << SCTL_PROG_START_i), SCTL_BASE + SCTL_BISR_L2C);
}

void bisr_start()
{
  uint32_t sctlreg = ioread32(SCTL_BASE + SCTL_BISR_L2C);

  iowrite32(sctlreg | (1 << SCTL_START_i), SCTL_BASE + SCTL_BISR_L2C);
}

void bisr_stop()
{
	uint32_t sctlreg = ioread32(SCTL_BASE + SCTL_BISR_L2C);

	iowrite32(sctlreg & ~(1 << SCTL_START_i), SCTL_BASE + SCTL_BISR_L2C);
}

bool bisr_wait(uint32_t timeout)
{
	uint32_t start = rumboot_platform_get_uptime();
	while (!(ioread32(SCTL_BASE + SCTL_BISR_L2C) & (1 << SCTL_FINISH_i))) {
    if (rumboot_platform_get_uptime() - start > timeout) {
      rumboot_printf("Timeout ERROR.\n");
      return false;
    }
  }
	return true;
}

int fca1_gen(int err_vec){
    int fca1;
    switch(err_vec){
     case 0x00000000: fca1 = 0x00;
     case 0x00000001: fca1 = 0x1f;
     case 0x00000002: fca1 = 0x1e;
     case 0x00000004: fca1 = 0x1d;
     case 0x00000008: fca1 = 0x1c;
     case 0x00000010: fca1 = 0x1b;
     case 0x00000020: fca1 = 0x1a;
     case 0x00000040: fca1 = 0x19;
     case 0x00000080: fca1 = 0x18;
     case 0x00000100: fca1 = 0x17;
     case 0x00000200: fca1 = 0x16;
     case 0x00000400: fca1 = 0x15;
     case 0x00000800: fca1 = 0x14;
     case 0x00001000: fca1 = 0x13;
     case 0x00002000: fca1 = 0x12;
     case 0x00004000: fca1 = 0x11;
     case 0x00008000: fca1 = 0x10;
     case 0x00010000: fca1 = 0x0f;
     case 0x00020000: fca1 = 0x0e;
     case 0x00040000: fca1 = 0x0d;
     case 0x00080000: fca1 = 0x0c;
     case 0x00100000: fca1 = 0x0b;
     case 0x00200000: fca1 = 0x0a;
     case 0x00400000: fca1 = 0x09;
     case 0x00800000: fca1 = 0x08;
     case 0x01000000: fca1 = 0x07;
     case 0x02000000: fca1 = 0x06;
     case 0x04000000: fca1 = 0x05;
     case 0x08000000: fca1 = 0x04;
     case 0x10000000: fca1 = 0x03;
     case 0x20000000: fca1 = 0x02;
     case 0x40000000: fca1 = 0x01;
     case 0x80000000: fca1 = 0x00;
     default:         fca1 = 0x00;
    }
     return fca1;
}

int fca2_gen(int err_vec){
    int fca2;
    switch(err_vec){
     case 0x00000000: fca2 = 0x00;
     case 0x00000001: fca2 = 0x00;
     case 0x00000002: fca2 = 0x01;
     case 0x00000004: fca2 = 0x02;
     case 0x00000008: fca2 = 0x03;
     case 0x00000010: fca2 = 0x04;
     case 0x00000020: fca2 = 0x05;
     case 0x00000040: fca2 = 0x06;
     case 0x00000080: fca2 = 0x07;
     case 0x00000100: fca2 = 0x08;
     case 0x00000200: fca2 = 0x09;
     case 0x00000400: fca2 = 0x0a;
     case 0x00000800: fca2 = 0x0b;
     case 0x00001000: fca2 = 0x0c;
     case 0x00002000: fca2 = 0x0d;
     case 0x00004000: fca2 = 0x0e;
     case 0x00008000: fca2 = 0x0f;
     case 0x00010000: fca2 = 0x10;
     case 0x00020000: fca2 = 0x11;
     case 0x00040000: fca2 = 0x12;
     case 0x00080000: fca2 = 0x13;
     case 0x00100000: fca2 = 0x14;
     case 0x00200000: fca2 = 0x15;
     case 0x00400000: fca2 = 0x16;
     case 0x00800000: fca2 = 0x17;
     case 0x01000000: fca2 = 0x18;
     case 0x02000000: fca2 = 0x19;
     case 0x04000000: fca2 = 0x1a;
     case 0x08000000: fca2 = 0x1b;
     case 0x10000000: fca2 = 0x1c;
     case 0x20000000: fca2 = 0x1d;
     case 0x40000000: fca2 = 0x1e;
     case 0x80000000: fca2 = 0x1f;
     default:         fca2 = 0x00;
    }
     return fca2;
}


static bool bisr_prog_mbist1_analyze() {
  bool unrep_mem = 0;
  for (int a=0; a<4; a++){
      uint32_t err_vector_low  = ioread32(BISR_L2C + BISR_ERROR_VECTOR_LOW_ARR0  + 0x10*a);
      uint32_t err_vector_high = ioread32(BISR_L2C + BISR_ERROR_VECTOR_HIGH_ARR0 + 0x10*a);
      
      uint32_t err_vector_low_buff  = err_vector_low ;
      uint32_t err_vector_high_buff = err_vector_high;
      
      uint32_t err_vector_low_shift ;
      uint32_t err_vector_high_shift;
      
      //rumboot_printf("Error vector Low ARR %x: %x\n", a, err_vector_low);
      //rumboot_printf("Error vector High ARR %x: %x\n", a, err_vector_high);
      
      for (int b=0; b<30; b++){
          err_vector_low_shift  = err_vector_low  >> 1;
          err_vector_high_shift = err_vector_high >> 1;
          
          err_vector_low_buff  = err_vector_low_buff  | err_vector_low_shift ;
          err_vector_high_buff = err_vector_high_buff | err_vector_high_shift;       
      }
      
      uint32_t err_vector_analyze_low  = err_vector_low_buff  & 0x00000003;
      uint32_t err_vector_analyze_high = err_vector_high_buff & 0x00000003;
      
      bool unrepairable    = err_vector_analyze_low  == 0x3 || err_vector_analyze_high == 0x3 ;
      bool repairable_low  = err_vector_analyze_low  == 0x1 || err_vector_analyze_low  == 0x2 ;
      bool repairable_high = err_vector_analyze_high == 0x1 || err_vector_analyze_high == 0x2 ;
      bool repairable      = repairable_low                 || repairable_high                ;
            
      if (unrepairable) 
          rumboot_printf("Array %x has unrepairable number of errors!\n", a);
      else{
          if (repairable) {
              rumboot_printf("Array %x has repairable number of errors! RB is generated\n", a);
              iowrite32(repairable_high << CRE2 | fca2_gen(err_vector_high) << FCA2 | repairable_low << CRE1 | fca1_gen(err_vector_low) << FCA1, BISR_L2C + BISR_REDUNDANCY_BUS_ARR0  + 0x10*a);
          }
          else
              rumboot_printf("Array %x hasn't errors! It's error-free\n", a);
      
        }
    unrep_mem = unrep_mem || unrepairable;
  }
  return (unrep_mem) ? false : true;
}

static bool bisr_prog_mbist2_analyze() {
    
    uint32_t err_vector_mem_low  = 0x0; 
    uint32_t err_vector_mem_high = 0x0;
    
    for (int a=0; a<4; a++){
        uint32_t err_vector_arr_low = ioread32(BISR_L2C + BISR_ERROR_VECTOR_LOW_ARR0 + 0x10*a);
        uint32_t err_vector_arr_high = ioread32(BISR_L2C + BISR_ERROR_VECTOR_HIGH_ARR0 + 0x10*a);
        
        if (err_vector_arr_low || err_vector_arr_high){
            rumboot_printf("ARR %x has error after 2nd MBIST. Memory is unrepairable!", a);
            rumboot_printf("Error vector Low ARR %x: %x\n", a, err_vector_arr_low);
            rumboot_printf("Error vector High ARR %x: %x\n", a, err_vector_arr_high);
        }
        
        err_vector_mem_low  = err_vector_mem_low  | err_vector_arr_low ;
        err_vector_mem_high = err_vector_mem_high | err_vector_arr_high;
        
    }
    return (err_vector_mem_low || err_vector_mem_high) ? false : true;
}

int bisr_hard_analyze()
{
  uint32_t l2cbisr = ioread32(SCTL_BASE + SCTL_BISR_L2C);

  bool rslt = l2cbisr & (1 << SCTL_RSLT_i);
  bool redok = l2cbisr & (1 << SCTL_REDOK_i);

  if(!redok && !rslt){
      rumboot_printf("MEM FAIL.\n");
      return BISR_MEM_FAIL;
  }
  else {
      if(!redok && rslt){
          rumboot_printf("MEM GOOD.\n");
          return BISR_MEM_GOOD;
      }
      else {
          if(redok && rslt){
              rumboot_printf("PERFECT.\n");
              return BISR_MEM_PERFECT;
          }
          else return BISR_TEST_TIMEOUT;
      }
  }
}


bool bisr_program_test(uint32_t timeout)
{
  rumboot_printf("Clock enable.\n");
  iowrite32(0x1 ,SCTL_BASE + SCTL_BISR_CLK_EN);
  rumboot_printf("Set RB=0 for all arrays.\n");
  for (int a=0; a<4; a++)
	iowrite32(0, BISR_L2C + BISR_REDUNDANCY_BUS_ARR0 + 0x10*a);
  //1st MBIST 
  rumboot_printf("Start L2C BISR(Prog mode), 1st MBIST.\n");
	bisr_prog_start();
	udelay(1);
  rumboot_printf("Stop L2C BISR(Prog mode).\n");
	bisr_prog_stop();
  rumboot_printf("Wait L2C BISR finish, 1st MBIST.\n");
	if (!bisr_wait(timeout))
		return false;
  rumboot_printf("L2C BISR(Prog mode) analyze after 1st MBIST.\n");
  bool ret1 = bisr_prog_mbist1_analyze();
   if (!ret1)
       return false; 
  //2nd MBIST
  rumboot_printf("Start L2C BISR(Prog mode), 2nd MBIST.\n");
	bisr_prog_start();
	udelay(1);
  rumboot_printf("Stop L2C BISR(Prog mode).\n");
	bisr_prog_stop();
  rumboot_printf("Wait L2C BISR finish, 2nd MBIST.\n");
	if (!bisr_wait(timeout))
		return false;
  rumboot_printf("L2C BISR(Prog mode) analyze after 2nd MBIST.\n");
  bool ret2 = bisr_prog_mbist2_analyze();
   if (!ret2) 
       return false; 
  rumboot_printf("Clock disable.\n");
  iowrite32(0x0 ,SCTL_BASE + SCTL_BISR_CLK_EN);
  return true;
  //return ret;
}

int bisr_hard_test(uint32_t timeout_us)
{
  rumboot_printf("Clock enable.\n");
  iowrite32(0x1 ,SCTL_BASE + SCTL_BISR_CLK_EN);
  rumboot_printf("Start L2C BISR.\n");
  bisr_start();
  udelay(1);
  rumboot_printf("Stop L2C BISR.\n");
  bisr_stop();
  rumboot_printf("Wait L2C BISR.\n");
	if (!bisr_wait(timeout_us))
		return BISR_TEST_TIMEOUT;
  int bisr_result = bisr_hard_analyze();
  rumboot_printf("Clock disable.\n");
  iowrite32(0x0 ,SCTL_BASE + SCTL_BISR_CLK_EN);
  return bisr_result;
}
