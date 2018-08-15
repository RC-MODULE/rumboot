#include <rumboot/bootsrc/spiflash.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/timer.h>
#include <rumboot/macros.h>
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



void pl022_internal_cs(uint32_t base, int select)
{
	/* TODO: .... */
}

bool pl022_tx_empty(uint32_t base)
{
	return ioread32(base + PL022_SR) & (1 << PL022_SR__TFE_i);
}

bool pl022_tx_avail(uint32_t base)
{
	return ioread32(base + PL022_SR) & (1 << PL022_SR__TNF_i);
}

bool pl022_rx_empty(uint32_t base)
{
	return !(ioread32(base + PL022_SR) & (1 << PL022_SR__RNE_i));
}

void pl022_dump_fifo_state(uint32_t base)
{
	rumboot_printf("FIFO: rx: %s tx %s\n",
		pl022_rx_empty(base) ? "empty" : "non-empty",
		pl022_tx_empty(base) ? "empty" : "non-empty");
}

void pl022_xfer(uint32_t base, unsigned char *wrbuf, unsigned char *rdbuf, size_t len)
{
	int written = 0;
	int read = 0;

	while ((read < len) || (written < len)) {
		if ((read < len) && (!pl022_rx_empty(base))) {
			*rdbuf++ = (unsigned char)(ioread32(base + PL022_DR) & 0xff);
			read++;
		}
		if ((written < len) && (pl022_tx_avail(base))) {
			iowrite32(*wrbuf++, base + PL022_DR);
			written++;
		}
	}
}

void pl022_clear_rx_buf(uint32_t base)
{
	while (ioread32(base + PL022_SR) & (1 << PL022_SR__RNE_i))
		ioread32(base + PL022_DR);
}

static void spi_cs(const struct rumboot_bootsource *src, void *pdata, int select)
{
	if (src->chipselect) {
		src->chipselect(src, pdata, select);
	} else {
		pl022_internal_cs(src->base, select);
	}
}


#define	SPIFLASH_MANID         0x90
#define SPIFLASH_PAGEPROG      0x02
#define SPIFLASH_READDATA      0x03
#define SPIFLASH_FASTREAD      0x0B
#define SPIFLASH_WRITEDISABLE  0x04
#define SPIFLASH_READSTAT1     0x05
#define SPIFLASH_READSTAT2     0x35
#define SPIFLASH_READSTAT3     0x15
#define SPIFLASH_WRITESTATEN   0x50
#define SPIFLASH_WRITESTAT1    0x01
#define SPIFLASH_WRITESTAT2    0x31
#define SPIFLASH_WRITESTAT3    0x11
#define SPIFLASH_WRITEENABLE   0x06
#define SPIFLASH_ADDR4BYTE_EN  0xB7
#define SPIFLASH_ADDR4BYTE_DIS 0xE9
#define SPIFLASH_SECTORERASE   0x20
#define SPIFLASH_BLOCK32ERASE  0x52
#define SPIFLASH_BLOCK64ERASE  0xD8
#define SPIFLASH_CHIPERASE     0x60
#define SPIFLASH_ALT_CHIPERASE 0xC7    // Some flash chips use a different chip erase command
#define SPIFLASH_SUSPEND       0x75
#define SPIFLASH_ID            0x90
#define SPIFLASH_RESUME        0x7A
#define SPIFLASH_JEDECID       0x9F
#define SPIFLASH_POWERDOWN     0xB9
#define SPIFLASH_RELEASE       0xAB
#define SPIFLASH_READSFDP      0x5A
#define SPIFLASH_UNIQUEID 	   0x4B


void spiflash_read_id(uint32_t base) {
   unsigned char wrbuf[5];
   unsigned char rdbuf[5];

   wrbuf[0] = SPIFLASH_MANID;

   pl022_xfer(base, wrbuf, rdbuf, 5);
   int i;

   for (i=0; i<5; i++) {
	   rumboot_printf("0x%x\n", rdbuf[i]);
   }
}

void spiflash_read_flash(uint32_t base, uint32_t offset, unsigned char *dest, int len) {
   unsigned char cmd[4];
   cmd[0] = SPIFLASH_READDATA;
   cmd[1] = (unsigned char) ((offset >> 16) & 0xff);
   cmd[2] = (unsigned char) ((offset >> 8)  & 0xff);
   cmd[3] = (unsigned char) ((offset >> 0)  & 0xff);

   pl022_xfer(base, cmd, cmd, ARRAY_SIZE(cmd));
   pl022_xfer(base, dest, dest, len);

}


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

static void spiflash_deinit(const struct rumboot_bootsource *src, void *pdata)
{

	uint32_t cr1 = ioread32(src->base + PL022_CR1);
	iowrite32(cr1 & ~(1 << PL022_CR1__SSE_i), src->base + PL022_CR1); //disable PL022 SSP
}

static size_t spiflash_read(const struct rumboot_bootsource* src, void* pdata, void* to, size_t offset, size_t length)
{
	spi_cs(src, pdata, 0);
	rumboot_printf("boot: %s: Reading %d bytes from offset 0x%x\n", src->name, length, offset);
	spiflash_read_flash(src->base, offset, to, length);
	spi_cs(src, pdata, 1);
	return length;
}

const struct rumboot_bootmodule g_bootmodule_spiflash = {
		.init = spiflash_init,
		.deinit = spiflash_deinit,
		.read = spiflash_read,
		.privdatalen = sizeof(struct spiflash_private_data)
};
