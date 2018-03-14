#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <devices/mdio.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <rumboot/bootheader.h>
#include <platform/devices.h> 
#include <platform/interrupts.h>


uint32_t main()
{
  rumboot_printf("MDIO%x PHY intrp Test\n", MDIO_NUM);
  bool result = mdio_phy_intrp_test(MDIO_NUM);
  
  if (result)
      rumboot_printf("Test OK!\n");
  else
      rumboot_printf("Test FAIL!\n");
      
  
  return result ? 0 : 1;
 
}
