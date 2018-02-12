#include <devices/bisr.h>

#include <rumboot/printf.h>

uint32_t main()
{
  rumboot_printf("Test BISR.\n");

  if(bisr_hard_test())
    rumboot_printf("Test OK!\n");

  rumboot_printf("Test failed\n");

  return 0;
}
