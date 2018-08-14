#include <rumboot/bootsrc/spiflash.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/timer.h>

#include <regs/regs_spi.h>

#include <stddef.h>

#define DEBUG
#define SPI_READ_TIMEOUT 500

enum Data_size {
  _4_BIT = 0,
  _5_BIT,
  _6_BIT,
  _7_BIT,
  _8_BIT,
  _9_BIT,
  _10_BIT,
  _11_BIT,
  _12_BIT,
  _13_BIT,
  _14_BIT,
  _15_BIT,
  _16_BIT,
};


struct spiflash_private_data {
	const struct rumboot_bootsource *src;
};

/*FIX THIS FUNC!*/
#if 0
static uint32_t calc_div(const uint32_t freq)
{
	uint32_t scr = 0;
	//uint32_t bit_rate;
	uint32_t div = freq / (12500 * (1 + scr));
	rumboot_printf("DIV: %d\n", div);

	return div;
}
#endif

static void set_data_size(uint32_t base, enum Data_size data_size)
{
	size_t i;
	uint32_t value = 0b0011;

	for (i = 0; i < data_size; i++)
		value += 0b1;

	iowrite32(value, base + PL022_CR0);
}

static void clear_rx_buf(uint32_t base)
{
	while (ioread32(base + PL022_SR) & (1 << PL022_SR__RNE_i))
		ioread32(base + PL022_DR);
}

static bool spiflash_init(const struct rumboot_bootsource *src, void *pdata)
{
	struct spiflash_private_data *spi_flash = (struct spiflash_private_data *)pdata;

	//uint32_t div = calc_div(src->freq_khz);

	iowrite32(8, src->base + PL022_CPSR); //100MHz/8 = 12.5MHz if SCR = 0

	set_data_size(src->base, _8_BIT);

	iowrite32((1 << PL022_CR1__SOD_i) | (1 << PL022_CR1__SSE_i), src->base + PL022_CR1);//enable PL022 SSP

	spi_flash->src = src;

	return true;
}

static inline struct spiflash_private_data *to_spiflash_pdata(void *pdata)
{
	return (struct spiflash_private_data *)pdata;
}

static void spiflash_deinit(void *pdata)
{
	/*TO DO!*/
	struct spiflash_private_data *spiflash_pdata = to_spiflash_pdata(pdata);
	const struct rumboot_bootsource *src = spiflash_pdata->src;
	uint32_t cr1 = ioread32(src->base + PL022_CR1);

	iowrite32(cr1 & ~(1 << PL022_CR1__SSE_i), src->base + PL022_CR1); //disable PL022 SSP
}

static int spiflash_read(void *pdata, void *ram, void *spi_addr)
{
	/*TO DO!*/
	struct spiflash_private_data *spiflash_pdata = to_spiflash_pdata(pdata);
	const struct rumboot_bootsource *src = spiflash_pdata->src;
	uint8_t *dst = ram;

	clear_rx_buf(src->base);

	iowrite8(0x03, src->base + PL022_DR);
	iowrite8(( (uint32_t) spi_addr >> 16) & 0xff, src->base + PL022_DR);
	iowrite8(( (uint32_t) spi_addr >> 8) & 0xff, src->base + PL022_DR);
	iowrite8(( (uint32_t) spi_addr >> 0) & 0xff, src->base + PL022_DR);

	uint32_t start;
	start = rumboot_platform_get_uptime();

	while (!(ioread32(src->base + PL022_SR) & (1 << PL022_SR__TFE_i))) {
		if ((rumboot_platform_get_uptime() - start) > SPI_READ_CMD_TIMEOUT) {
			rumboot_printf("SPI flash read cmd timeout");

			return -1;
		}
	}

	clear_rx_buf(src->base);

	//UGLY HACK: WRITE REAL LENGTH
	size_t tx_len = 512;
	size_t rx_len = 512;
	size_t data_size = 0;

	start = rumboot_platform_get_uptime();

	while (rx_len-- && tx_len--) {

#if 0
		if ((rumboot_platform_get_uptime() - start) > SPI_READ_TIMEOUT) {
			rumboot_printf("SPI flash read timeout.\n");

			return -2;
		}
#endif

		if (ioread32(src->base + PL022_SR) & (1 << PL022_SR__TNF_i)) {
#ifdef DEBUG
			rumboot_printf("write 0x%x ", 0xff);
#endif
			iowrite16(0xff, src->base + PL022_DR);
		}

//		udelay(100);

		if (ioread32(src->base + PL022_SR) & (1 << PL022_SR__RNE_i)) {
			uint8_t byte = ioread16(src->base + PL022_DR);
			rumboot_printf("read 0x%x\n", ioread16(src->base + PL022_DR));

			*dst++ = byte;
			//dst++;
		}
	}

	dst = ram;
	rumboot_printf("\n");

	int i;
	for (i = 0; i < 512; i++) {
		rumboot_printf("%x ", *dst);
		dst++;
	}
	rumboot_printf("\n");

	return data_size;
}

const struct rumboot_bootmodule g_bootmodule_spiflash = {
		.init = spiflash_init,
		.deinit = spiflash_deinit,
		.read = spiflash_read,
		.privdatalen = sizeof(struct spiflash_private_data)
};
