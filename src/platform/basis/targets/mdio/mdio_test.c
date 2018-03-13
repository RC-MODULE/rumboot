#include <devices/mdio.h>

#include <rumboot/printf.h>

uint32_t main()
{
  rumboot_printf("MDIO Test\n");
  bool result = mdio_test(MDIO_NUM);
  
  if (~result)
      rumboot_printf("Test OK!\n");
  else
      rumboot_printf("Test FAIL!\n");
      
  
  return result ? 1 : 0;
 
}
