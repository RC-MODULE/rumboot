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

static void msr_write(uint32_t const wval)
{
	__asm volatile
	(
		"mtmsr %0 \n\t"
		::"r" (wval)
	);
}

static uint32_t msr_read()
{
	uint32_t rval = 0;
	__asm volatile
	(
		"mfmsr %0 \n\t"
		: "=r" (rval)
	);

	return rval;
}

#define IBM_BIT_INDEX(size, index)    (((size) - 1) - ((index) % (size)))
#define ITRPT_XSR_FP_e  50
#define ITRPT_XSR_FP_i IBM_BIT_INDEX(64, ITRPT_XSR_FP_e)

static void enable_fpu()
{
	msr_write(msr_read() | (1 << ITRPT_XSR_FP_i));
}

#define PL061_AFSEL 0x420
static bool enable_gpio_for_SDIO()
{
	uint32_t afsel = ioread32(LSIF1_GPIO7__ + PL061_AFSEL) | 0b11110;

	iowrite32(afsel, LSIF1_GPIO7__ + PL061_AFSEL);

	return true;
}

static void disable_gpio_for_SDIO()
{
	uint32_t afsel = ioread32(LSIF1_GPIO7__ + PL061_AFSEL) & ~(0b11110);

	iowrite32(afsel, LSIF1_GPIO7__ + PL061_AFSEL);
}

static bool spiflash_init_gpio_mux()
{
	return true;
}

static void spiflash_deinit_gpio_mux()
{

}

#define SDIO_CLK_FREQ 100000
#define SPI_CLK_FREQ 10000
const struct rumboot_bootsource arr[] = {

#if 0
  {
		.name = "SDIO",
		.base = SDIO0_BASE,
		.offset = 8192,//Because at the begining we have descriptors table
		.freq_khz = SDIO_CLK_FREQ,
		.privdatalen = 128,
		.init = sd_init,
		.deinit = sd_deinit,
		.read = sd_read,
		.init_gpio_mux = enable_gpio_for_SDIO,
		.deinit_gpio_mux = disable_gpio_for_SDIO,
		.load_again = sd_load_again,
	},
#endif

#if 1
	{
		.name = "SPI",
		.base = SPI_CTRL1_BASE,
		.offset = 0x0,
		.freq_khz = SPI_CLK_FREQ,
		.privdatalen = 128,
		.init = spiflash_init,
		.deinit = spiflash_deinit,
		.read = spiflash_read,
		.init_gpio_mux = spiflash_init_gpio_mux,
		.deinit_gpio_mux = spiflash_deinit_gpio_mux,
		.load_again = spiflash_load_again,
	},
#endif
	{ /*Sentinel*/ }
};

#define PDATA_SIZE 128
int main()
{
	enable_fpu();

	rumboot_print_logo();

	rumboot_printf("rumboot: Yarr! I need moar rum!\n\n");

	char pdata[PDATA_SIZE];

	struct rumboot_bootsource *ptr = arr;
	while (ptr->name) {
		if (!arr->init_gpio_mux(&pdata))
			return false;

		ptr++;
	}

	bootsource_try_chain(arr, (void *)&pdata);

	while (1);

	return 0;
}
