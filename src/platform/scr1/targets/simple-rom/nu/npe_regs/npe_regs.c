
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/rumboot.h>
#include <rumboot/io.h>
#include <string.h>

#include <platform/devices/nu_lib.h>
#include <regs/regs_na.h>
#include <platform/devices.h>


int main() {
  uint32_t temp;
  
  rumboot_printf("Hello npe_regs\n");
  temp = ioread32(NPE_BASE + NA_VPE_BASE + NU_VPE + NU_VPE_DEV_ID);
  if(temp != 0xabcd2021) {
    rumboot_printf("Unexpected NU_VPE_DEV_ID=0x%x\n",temp);
    return 1;
  }
  rumboot_printf("Test Passed\n");
  return 0;
}
