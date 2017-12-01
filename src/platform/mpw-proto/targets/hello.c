#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>

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

#if 1

#define CRGCPU__ 0x38006000
int freq_apply_mpw(uint32_t fbdiv, uint32_t prediv, uint32_t postdiv)
{

  uint32_t ckdiv = 1;

  if( (fbdiv > 511) || (fbdiv < 8) ||
      (prediv > 31) || (prediv < 1) ||
      ((postdiv%2 != 0) && (postdiv != 1)) ) {

        rumboot_printf("Wrong values for div.\n");
        rumboot_printf("fbdiv %d\n", fbdiv);
        rumboot_printf("prediv %d\n", prediv);
        rumboot_printf("postdiv %d\n", postdiv);
        return -1;
  }


  uint32_t freq = 25 * fbdiv / prediv / postdiv / (ckdiv + 1);
  rumboot_printf("Switching to %d Mhz ... ", freq);
  iowrite32(0x1ACCE551, CRGCPU__ + 0xc);
  iowrite32((fbdiv << 0) | (prediv<<16) | (postdiv<<24), CRGCPU__ + 0x8);
  iowrite32(0x1, CRGCPU__ + 0x4);
  rumboot_printf("OK");

  return 0;
}

void perform_mpw(/*struct freq_entry *frq*/)
{
  uint32_t prediv = 2;
  uint32_t postdiv = 1;
  uint32_t fbdiv;

  for (fbdiv = 8; fbdiv < 511; fbdiv++) {

    if(freq_apply_mpw(fbdiv, prediv, postdiv) < 0)
      break;

    test(5);
  }
}

#endif

void test(int n) {

  while(n--) {

    //Before call this func ENABLE FPU!
    test_fpu();

    //test_cpu();

  }
}

int main()
{
	rumboot_print_logo();
  enable_fpu();
	rumboot_printf("rumboot: Yarr! I need moar rum!\n\n");
	double a = 1.0;
	double b = 2.0;
	rumboot_printf("rumboot: %f\n", a + asin(b));

  //test(5);
  perform_mpw();

	printf("WOOOT\n");

	while (1);;
	/* Return 0, test passed */
	return 0;
}
