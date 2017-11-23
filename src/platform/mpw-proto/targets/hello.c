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

#if 0
struct freq_entry {
  uint32_t freq_mhz;
  uint32_t dvalue;
};

struct freq_entry freq_table_mpw[] = {
  { 250, 0x03010050 },
  { 500, 0x02010050 },
  { /* Sentinel */ }
};

struct freq_entry freq_table_fullchip[] = {
  { 250, 0x03010050 },
  { 500, 0x02010050 },
  { /* Sentinel */ }
};


void freq_apply_mpw(uint32_t value)
{
  uint32_t freq = 25 * (value & 0xff) / ( (value & (0xff << 24)) >> 24);
  rumboot_printf("Switching to %d Mhz ... ", frq);
  iowrite32(0x1ACCE551, CRGCPU__ + 0xc);
  iowrite32(value, CRGCPU__ + 0x8);
  iowrite32(0x1, CRGCPU__ + 0x4);
  rumboot_printf("OK");
}


void perform_mpw(struct freq_entry *frq)
{
  uint32_t previv;
  for (fbdiv = 8; fbdiv < 511; fbdiv++) {
    freq_apply_mpw(fbdiv | (1<<16) | );
  }


  while(frq->dvalue) {

    if (!run_stability_tests(100)) {
      rumboot_printf_mpw("FAIL\n");
      while(1);;
    }
    rumboot_printf("PASS\n");
    frq++;
  }
}

#endif

int main()
{
	rumboot_print_logo();
  enable_fpu();
	rumboot_printf("rumboot: Yarr! I need moar rum!\n\n");
	double a = 1.0;
	double b = 2.0;
	rumboot_printf("rumboot: %f\n", a + asin(b));
  rumboot_platform_putchar('X');
  rumboot_platform_putchar('Y');
  printf("WOOOT\n");
	while (1);;
	/* Return 0, test passed */
	return 0;
}
