#include <devices/bisr.h>

#include <rumboot/printf.h>

uint32_t main()
{
  rumboot_printf("Test hard BISR with repairable number of errors.\n");

  if(bisr_hard_test())
    rumboot_printf("Test OK!\n");
  else
    rumboot_printf("Test failed\n");

  return 0;
}
