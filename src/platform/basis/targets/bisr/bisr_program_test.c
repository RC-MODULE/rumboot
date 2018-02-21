#include <devices/bisr.h>

#include <rumboot/printf.h>

uint32_t main()
{
  rumboot_printf("Test program BISR\n");
  rumboot_printf("Possible results:\n");
  rumboot_printf("BISR_TEST_TIMEOUT 0\n");
  rumboot_printf("BISR_MEM_PERFECT 1\n");
  rumboot_printf("BISR_MEM_GOOD 2\n");
  rumboot_printf("BISR_MEM_FAIL 3\n");
  rumboot_printf("Expecting result: %d\n", BISR_TEST_EXPECTED);
  
  int result = (bisr_program_test(BISR_TIMEOUT) == BISR_TEST_EXPECTED);
  if (result)
      rumboot_printf("Test OK!\n");
  else
      rumboot_printf("Test FAIL!\n");
      
  
  return result ? 0 : 1;
 
}
