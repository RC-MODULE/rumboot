#include <rumboot/bootsrc/spiflash.h>
#include <rumboot/bootsource.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/timer.h>

#include <regs/regs_spi.h>

#include <stddef.h>

struct spiflash_private_data {
	const struct rumboot_bootsource *src;
};

static uint32_t calc_div(const uint32_t freq)
{
	uint32_t scr = 0;
	uint32_t div = freq / (1 + scr);

	return div;
}

static void set_data_size(uint32_t base, enum Data_size data_size)
{
	size_t i;
	uint32_t value = 0b0011;

	for (i = 0; i < data_size; i++)
		value &= 0b1;

	iowrite32(value, base + PL022_CR0);
}

static void clear_rx_buf(uint32_t base)
{
	while (ioread32(base + PL022_SR) & (1 << PL022_SR__RNE_i))
		ioread32(base + PL022_DR);
}

bool spiflash_init(const struct rumboot_bootsource *src, void *pdata)
{
	struct spiflash_private_data *spi_flash = (struct spiflash_private_data *)pdata;

	uint32_t div = calc_div(src->freq_khz);

	iowrite32(div, src->base + PL022_CPSR); //100MHz/8 = 12.5MHz if SCR = 0

	set_data_size(src->base, _8_BIT);

	iowrite32((1 << PL022_CR1__SOD_i) | (1 << PL022_CR1__SSE_i), src->base + PL022_CR1);//enable PL022 SSP

	spi_flash->src = src;

	return true;
}

void spiflash_deinit(void *pdata)
{
	/*TO DO!*/
	struct spiflash_private_data *spiflash_pdata = (struct spiflash_private_data *)pdata;
	const struct rumboot_bootsource *src = spiflash_pdata->src;
	uint32_t cr1 = ioread32(src->base + PL022_CR1);

	iowrite32(cr1 & ~(1 << PL022_CR1__SSE_i), src->base + PL022_CR1); //disable PL022 SSP
}

bool spiflash_read(void *pdata)
{
	/*TO DO!*/
	struct spiflash_private_data *spiflash_pdata = (struct spiflash_private_data *)pdata;
	const struct rumboot_bootsource *src = spiflash_pdata->src;

	clear_rx_buf(src->base);

	iowrite8(0x03, src->base + PL022_DR);  // Is it command?
	iowrite8(((src->base + src->offset) >> 16) & 0xff, src->base + PL022_DR);
	iowrite8(((src->base + src->offset) >> 8) & 0xff, src->base + PL022_DR);
	iowrite8(((src->base + src->offset) >> 0) & 0xff, src->base + PL022_DR);

	uint32_t start;
	start = rumboot_platform_get_uptime();

	while (!(ioread32(src->base + PL022_SR) & (1 << PL022_SR__TFE_i))) {
		if ((rumboot_platform_get_uptime() - start) > SPI_READ_CMD_TIMEOUT) {
			rumboot_printf("SPI flash read cmd timeout");

			return false;
		}
	}

	clear_rx_buf(src->base);

	start = rumboot_platform_get_uptime();

	//UGLY HACK: WRITE REAL LENGTH
	size_t tx_len = 512;
	size_t rx_len = 512;
#define IM0_BASE 0x0
	uint32_t *dst = IM0_BASE;

	while (rx_len-- && tx_len--) {
		if ((rumboot_platform_get_uptime() - start) > SPI_READ_TIMEOUT)
			return false;


		if (ioread32(src->base + PL022_SR) & (1 << PL022_SR__TNF_i))
			iowrite8(0xff, src->base + PL022_DR);


		if (ioread32(src->base + PL022_SR) & (1 << PL022_SR__RNE_i)) {
			uint8_t byte = ioread8(src->base + PL022_DR);
			rumboot_printf("%x ", byte);
			*dst++ = byte;
		}
	}

	return true;
}

bool spiflash_are_load_attempts(void *pdata)
{
	return false;
}
