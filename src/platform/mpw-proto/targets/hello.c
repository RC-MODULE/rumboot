#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>


static void msr_write( uint32_t const wval )
{
  __asm volatile
  (
      "mtmsr %0 \n\t"
      ::"r"(wval)
  );
}

static uint32_t msr_read()
{
  uint32_t rval=0;
  __asm volatile
  (
      "mfmsr %0 \n\t"
      :"=r"(rval)
  );
  return rval;
}

#define IBM_BIT_INDEX( size, index )    ( ((size)-1) - ((index)%(size)) )
#define ITRPT_XSR_FP_e  50
#define ITRPT_XSR_FP_i IBM_BIT_INDEX( 64, ITRPT_XSR_FP_e )

static void enable_fpu()
{
    msr_write(msr_read() | (1<<ITRPT_XSR_FP_i));
}


int main()
{
	rumboot_print_logo();
	rumboot_printf("rumboot: Yarr! I need moar rum!\n\n");
	enable_fpu();
	double a = 1.0;
	double b = 2.0;
	rumboot_printf("rumboot: %f\n", a + b);

	/* Return 0, test passed */
	return 0;
}
