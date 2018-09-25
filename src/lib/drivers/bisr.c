#include <stdint.h>
#include <stdbool.h>

#include <platform/regs/sctl.h>
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

void rb_init()
{
    rumboot_printf("Set RB=0 for all arrays.\n");
  for (int a=0; a<NUM_ARRAYS; a++)
	iowrite32(0, BISR_L2C + BISR_REDUNDANCY_BUS_ARR0 + 0x10*a);
}

uint32_t fca1_gen(uint32_t err_vec){
    uint32_t fca1;
    switch(err_vec){
     case 0x00000000: fca1 = 0x00; break;
     case 0x00000001: fca1 = 0x1f; break;
     case 0x00000002: fca1 = 0x1e; break;
     case 0x00000004: fca1 = 0x1d; break;
     case 0x00000008: fca1 = 0x1c; break;
     case 0x00000010: fca1 = 0x1b; break;
     case 0x00000020: fca1 = 0x1a; break;
     case 0x00000040: fca1 = 0x19; break;
     case 0x00000080: fca1 = 0x18; break;
     case 0x00000100: fca1 = 0x17; break;
     case 0x00000200: fca1 = 0x16; break;
     case 0x00000400: fca1 = 0x15; break;
     case 0x00000800: fca1 = 0x14; break;
     case 0x00001000: fca1 = 0x13; break;
     case 0x00002000: fca1 = 0x12; break;
     case 0x00004000: fca1 = 0x11; break;
     case 0x00008000: fca1 = 0x10; break;
     case 0x00010000: fca1 = 0x0f; break;
     case 0x00020000: fca1 = 0x0e; break;
     case 0x00040000: fca1 = 0x0d; break;
     case 0x00080000: fca1 = 0x0c; break;
     case 0x00100000: fca1 = 0x0b; break;
     case 0x00200000: fca1 = 0x0a; break;
     case 0x00400000: fca1 = 0x09; break;
     case 0x00800000: fca1 = 0x08; break;
     case 0x01000000: fca1 = 0x07; break;
     case 0x02000000: fca1 = 0x06; break;
     case 0x04000000: fca1 = 0x05; break;
     case 0x08000000: fca1 = 0x04; break;
     case 0x10000000: fca1 = 0x03; break;
     case 0x20000000: fca1 = 0x02; break;
     case 0x40000000: fca1 = 0x01; break;
     case 0x80000000: fca1 = 0x00; break;
     default:         fca1 = 0x00;
    }
     return fca1;
}

uint32_t fca2_gen(uint32_t err_vec){
    uint32_t fca2;
    switch(err_vec){
     case 0x00000000: fca2 = 0x00; break;
     case 0x00000001: fca2 = 0x00; break;
     case 0x00000002: fca2 = 0x01; break;
     case 0x00000004: fca2 = 0x02; break;
     case 0x00000008: fca2 = 0x03; break;
     case 0x00000010: fca2 = 0x04; break;
     case 0x00000020: fca2 = 0x05; break;
     case 0x00000040: fca2 = 0x06; break;
     case 0x00000080: fca2 = 0x07; break;
     case 0x00000100: fca2 = 0x08; break;
     case 0x00000200: fca2 = 0x09; break;
     case 0x00000400: fca2 = 0x0a; break;
     case 0x00000800: fca2 = 0x0b; break;
     case 0x00001000: fca2 = 0x0c; break;
     case 0x00002000: fca2 = 0x0d; break;
     case 0x00004000: fca2 = 0x0e; break;
     case 0x00008000: fca2 = 0x0f; break;
     case 0x00010000: fca2 = 0x10; break;
     case 0x00020000: fca2 = 0x11; break;
     case 0x00040000: fca2 = 0x12; break;
     case 0x00080000: fca2 = 0x13; break;
     case 0x00100000: fca2 = 0x14; break;
     case 0x00200000: fca2 = 0x15; break;
     case 0x00400000: fca2 = 0x16; break;
     case 0x00800000: fca2 = 0x17; break;
     case 0x01000000: fca2 = 0x18; break;
     case 0x02000000: fca2 = 0x19; break;
     case 0x04000000: fca2 = 0x1a; break;
     case 0x08000000: fca2 = 0x1b; break;
     case 0x10000000: fca2 = 0x1c; break;
     case 0x20000000: fca2 = 0x1d; break;
     case 0x40000000: fca2 = 0x1e; break;
     case 0x80000000: fca2 = 0x1f; break;
     default:         fca2 = 0x00;
    }
     return fca2;
}


int bisr_prog_mbist1_analyze() {
  bool unrep_mem = 0;
  bool rep_mem   = 0;
  
  //Analysis for all arrays
  for (int a=0; a<NUM_ARRAYS; a++){
      uint32_t rb                      = 0x0;
      uint32_t err_vec_low_detect      = 0x0;   
      uint32_t err_vec_high_detect     = 0x0;
      bool unrepairable_arr            =   0;
      bool repairable_arr              =   0;
      bool cre1                        =   0;
      bool cre2                        =   0;

      int ones_low_cnt  = 0x0;  
      int ones_high_cnt = 0x0;       
      
      uint32_t err_vector_low  = ioread32(BISR_L2C + BISR_ERROR_VECTOR_LOW_ARR0  + 0x10*a);
      uint32_t err_vector_high = ioread32(BISR_L2C + BISR_ERROR_VECTOR_HIGH_ARR0 + 0x10*a);      
       
      uint32_t err_vector_low_shift  = err_vector_low  ;
      uint32_t err_vector_high_shift = err_vector_high ;      
      
      for (int b=0; b<ERR_VEC_WIDTH; b++){
          
          err_vec_low_detect  = err_vector_low_shift  & 0x00000001;
          err_vec_high_detect = err_vector_high_shift & 0x00000001;
          
          ones_low_cnt  += err_vec_low_detect ;
          ones_high_cnt += err_vec_high_detect;         
          
          err_vector_low_shift  >>= 1;
          err_vector_high_shift >>= 1;         
 
      }
      
      unrepairable_arr    = ones_low_cnt  > 1 || ones_high_cnt  > 1;
      repairable_arr      = ones_low_cnt == 1 || ones_high_cnt == 1;
      
      cre1 = ones_low_cnt  == 1;
      cre2 = ones_high_cnt == 1;
      
            
      if (unrepairable_arr) {
          rumboot_printf("ARR%x has unrepairable number of errors!\n", a);
          rumboot_printf("Error vector Low ARR%x: %x\n", a, err_vector_low);
          rumboot_printf("Error vector High ARR%x: %x\n", a, err_vector_high);
      }
          
      else{
          if (repairable_arr) {
              rumboot_printf("ARR%x has repairable number of errors! RB is generated\n", a);
              rumboot_printf("Error vector low ARR%x: %x\n", a, err_vector_low);
              rumboot_printf("Error vector high ARR%x: %x\n", a, err_vector_high);
              rumboot_printf("value for cre2: %x, CRE2: %x\n", cre2, CRE2);
              rumboot_printf("fca2_gen(err_vector_high): %x, FCA2: %x\n", fca2_gen(err_vector_high), FCA2);
              rumboot_printf("value for cre1: %x, CRE1: %x\n", cre1, CRE1);
              rumboot_printf("fca1_gen(err_vector_low): %x, FCA1: %x\n", fca1_gen(err_vector_low), FCA1);
              rb = cre2 << CRE2 | fca2_gen(err_vector_high) << FCA2 | cre1 << CRE1 | fca1_gen(err_vector_low) << FCA1;
              iowrite32(rb, BISR_L2C + BISR_REDUNDANCY_BUS_ARR0  + 0x10*a);
              rumboot_printf("For ARR%x RB = %x\n", a, rb);              
          }
          else{
              rumboot_printf("ARR%x hasn't errors! It's error-free\n", a);
              rumboot_printf("Let's check redundant elements!\n");
              rb = 1 << CRE2 | 1 << CRE1;
              iowrite32(rb, BISR_L2C + BISR_REDUNDANCY_BUS_ARR0  + 0x10*a);
              rumboot_printf("For ARR%x RB = %x\n", a, rb);
          }              
        }
    unrep_mem = unrep_mem || unrepairable_arr;
    rep_mem   = rep_mem   || repairable_arr  ;
  }//End of analysis for all arrays
  
  if(unrep_mem){
      rumboot_printf("Memory has unrepairable(FATAL) number of errors!\n");
      return MAIN_MEM_NONREP;
  }
  else{
      if(rep_mem){
          rumboot_printf("Memory has repairable number of errors! 2nd MBIST is required to check the correction possibility.\n");
          return MAIN_MEM_REP;
      }
      else {
          rumboot_printf("Main memory elements are error-free!\n");
          rumboot_printf("2nd MBIST will check redunant elements.\n");          
          return MAIN_MEM_CLEAR;
      }
  }  
}

static bool bisr_prog_mbist2_analyze() {
    
    bool unrep_mem = 0;
    
    for (int a=0; a<NUM_ARRAYS; a++){
      int ones_low_cnt  = 0x0;  
      int ones_high_cnt = 0x0;       
      
      uint32_t err_vector_low  = ioread32(BISR_L2C + BISR_ERROR_VECTOR_LOW_ARR0  + 0x10*a);
      uint32_t err_vector_high = ioread32(BISR_L2C + BISR_ERROR_VECTOR_HIGH_ARR0 + 0x10*a);      
       
      uint32_t err_vector_low_shift  = err_vector_low  ;
      uint32_t err_vector_high_shift = err_vector_high ;

      uint32_t err_vec_low_detect ;   
      uint32_t err_vec_high_detect;      
      
      for (int b=0; b<ERR_VEC_WIDTH; b++){
          
          err_vec_low_detect  = err_vector_low_shift  & 0x00000001;
          err_vec_high_detect = err_vector_high_shift & 0x00000001;
          
          ones_low_cnt  += err_vec_low_detect ;
          ones_high_cnt += err_vec_high_detect;         
          
          err_vector_low_shift  >>= 1;
          err_vector_high_shift >>= 1;         
 
      }
      
   
        if (ones_high_cnt == 0 && ones_low_cnt == 0){
            rumboot_printf("ARR%x hasn't errors after 2nd MBIST. Array is OK!\n", a);
            rumboot_printf("Error vector Low ARR%x: %x\n", a, err_vector_low);
            rumboot_printf("Error vector High ARR%x: %x\n", a, err_vector_high);
        }
        else {
            rumboot_printf("ARR%x has error after 2nd MBIST. Memory is unrepairable!\n", a);
            rumboot_printf("Error vector Low ARR%x: %x\n", a, err_vector_low);
            rumboot_printf("Error vector High ARR%x: %x\n", a, err_vector_high);
            unrep_mem = 1;
        }        
    }
    return unrep_mem;
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

int bisr_prog_analyze(uint32_t timeout)
{
  rb_init();
  //1st MBIST 
  rumboot_printf("Start L2C BISR(Prog mode), 1st MBIST.\n");
	bisr_prog_start();
	udelay(1);
  rumboot_printf("Stop L2C BISR(Prog mode).\n");
	bisr_prog_stop();
  rumboot_printf("Wait L2C BISR finish, 1st MBIST.\n");
	if (!bisr_wait(timeout)) return BISR_TEST_TIMEOUT;
  rumboot_printf("L2C BISR(Prog mode) analyze after 1st MBIST.\n");
  int mbist_1_result = bisr_prog_mbist1_analyze();
   if (mbist_1_result == MAIN_MEM_NONREP) return BISR_MEM_FAIL;
       
  //2nd MBIST
  rumboot_printf("Start L2C BISR(Prog mode), 2nd MBIST.\n");
	bisr_prog_start();
	udelay(1);
  rumboot_printf("Stop L2C BISR(Prog mode).\n");
	bisr_prog_stop();
  rumboot_printf("Wait L2C BISR finish, 2nd MBIST.\n");
	if (!bisr_wait(timeout))
		return BISR_TEST_TIMEOUT;
  rumboot_printf("L2C BISR(Prog mode) analyze after 2nd MBIST.\n");
  bool mbist_2_result = bisr_prog_mbist2_analyze();
   if (mbist_2_result)
   {
       rb_init();
       return BISR_MEM_FAIL;
   }
   else {
       if (mbist_1_result == MAIN_MEM_REP) {
           rumboot_printf("Successfull Repair!\n");
           return BISR_MEM_GOOD;
       }
       else {
           if (mbist_1_result == MAIN_MEM_CLEAR){
               rumboot_printf("Redundant element are also clear!\n");
               rb_init();
               return BISR_MEM_PERFECT;
           }
           else{
               rb_init();
               return BISR_MEM_FAIL;
           }
       }
   }
}
// End of program algo

int bisr_program_test(uint32_t timeout)
{
  rumboot_printf("Clock enable.\n");
  iowrite32(0x1 ,SCTL_BASE + SCTL_BISR_CLK_EN);
  
   /*uint32_t ev_arg = 0x00000400;
   rumboot_printf("fca1_gen(%x): %x\n", ev_arg, fca1_gen(ev_arg));   
   rumboot_printf("fca2_gen(%x): %x\n", ev_arg, fca2_gen(ev_arg));   
  uint32_t bisr_result = BISR_MEM_GOOD; */
  
  int bisr_result = bisr_prog_analyze(timeout);
  rumboot_printf("Clock disable.\n");
  iowrite32(0x0 ,SCTL_BASE + SCTL_BISR_CLK_EN);
  return bisr_result;
  
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
