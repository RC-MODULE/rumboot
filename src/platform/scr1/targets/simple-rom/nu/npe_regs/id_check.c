
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>

#include <platform/devices.h>

#include <regs/regs_na.h>


int main() {
  int res;
  uint32_t temp;
  
  rumboot_printf("Test Checking ID of NA\n");
  
  temp = ioread32(NPE_BASE + NA_VPE_BASE + NU_VPE_DEV_ID);
  
  rumboot_printf("VPE_ID=%x\n",temp);
  
  if(temp == 0xabcd2021)
  {
    res = 0;
  }
  else 
  {
    res = 1;
  }
    
  
  return res;
}
