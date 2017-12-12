#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rumboot/printf.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/io.h>
#include <rumboot/bootsrc/sdio.h>
#include <rumboot/bootsrc/spiflash.h>
#include <rumboot/bootsource.h>

#include <platform/devices.h>



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

void test(int n) {

  while(n--) {

    //Before call this func ENABLE FPU!
    test_fpu();

    //test_cpu();

  }
}

#endif

static bool enable_gpio_for_SDIO()
{
    #define PL061_AFSEL 0x420
    uint8_t afsel = ioread32(LSIF1_GPIO7__ + PL061_AFSEL) | 0b11110;

    iowrite32(afsel, LSIF1_GPIO7__ + PL061_AFSEL);
}

static void disable_gpio_for_SDIO() {

  #define PL061_AFSEL 0x420
  uint8_t afsel = ioread32(LSIF1_GPIO7__ + PL061_AFSEL) & ~(0b11110);

  iowrite32(afsel, LSIF1_GPIO7__ + PL061_AFSEL);
}

static bool spiflash_init_gpio_mux() {

  return true;
}

static void spiflash_deinit_gpio_mux() {

}

#define SDIO_CLK_FREQ 100000
#define SPI_CLK_FREQ 100000
const struct rumboot_bootsource arr[] = {
	{
	    .name = "SDIO",
			.base = SDIO0_BASE,
			.freq_khz = SDIO_CLK_FREQ,
	    .privdatalen = 128,
	    .init = sd_init,
	    .deinit = sd_deinit ,
	    .load_img = sd_read,
			.init_gpio_mux = enable_gpio_for_SDIO,
			.deinit_gpio_mux = disable_gpio_for_SDIO,
	    .are_load_attempts = sd_are_load_attempts,
	},

	{
			.name = "SPI",
			.base = SPI_CTRL0_BASE,
			.freq_khz = SPI_CLK_FREQ,
			.privdatalen = 128,
			.init = spiflash_init,
			.deinit = spiflash_deinit ,
			.load_img = spiflash_read,
			.init_gpio_mux = spiflash_init_gpio_mux,
			.deinit_gpio_mux = spiflash_deinit_gpio_mux,
			.are_load_attempts = spiflash_are_load_attempts,
	},
  {/*Sentinel*/}
};

#define PDATA_SIZE 128
bool test_sdio(uint32_t base_addr)
{
	rumboot_printf("TEST SDIO\n");

  char pdata[PDATA_SIZE];

	while(arr->name)
    if(!arr->init_gpio_mux(&pdata))
      return false;

	bootsource_try_chain(arr);

	return true;
}

int main()
{
  enable_fpu();

	rumboot_print_logo();

	rumboot_printf("rumboot: Yarr! I need moar rum!\n\n");

  #if 0
	double a = 1.0;
	double b = 2.0;
	rumboot_printf("rumboot: %f\n", a + asin(b));

  test(5);
  perform_mpw();

  #endif

  enable_gpio_for_SDIO();
	test_sdio(SDIO0_BASE);

	while (1);
	/* Return 0, test passed */
	return 0;
}
