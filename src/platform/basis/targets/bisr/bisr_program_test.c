#include <devices/bisr.h>

#include <rumboot/printf.h>

uint32_t main()
{
  rumboot_printf("Test program BISR\n");
  rumboot_printf("Possible results:\n");
  rumboot_printf("BISR_TEST_TIMEOUT %x\n", BISR_TEST_TIMEOUT);
  rumboot_printf("BISR_MEM_PERFECT %x\n", BISR_MEM_PERFECT);
  rumboot_printf("BISR_MEM_GOOD %x\n", BISR_MEM_GOOD);
  rumboot_printf("BISR_MEM_FAIL %x\n", BISR_MEM_FAIL);
  rumboot_printf("Expecting result: %d\n", BISR_TEST_EXPECTED);
  
  int result = (bisr_program_test(BISR_TIMEOUT) == BISR_TEST_EXPECTED);
  if (result)
      rumboot_printf("Test OK!\n");
  else
      rumboot_printf("Test FAIL!\n");
      
  
  return result ? 0 : 1;
 
}
