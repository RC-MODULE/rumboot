#include <devices/bisr.h>

#include <rumboot/printf.h>

uint32_t main()
{
  rumboot_printf("Test program BISR.\n");

  if(bisr_program_test())
    rumboot_printf("Test OK!\n");
  else
    rumboot_printf("Test failed\n");

  return 0;
}
