#include <rumboot/bootsrc/spiflash.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/timer.h>
#include <rumboot/macros.h>
#include <regs/regs_spi.h>
#include <stddef.h>
#include <devices/pl022.h>

#define DEBUG
#define SPI_READ_TIMEOUT 500

struct spiflash_private_data {
	const struct rumboot_bootsource *src;
};

static void spi_cs(const struct rumboot_bootsource *src, void *pdata, int select)
{
	if (src->chipselect) {
		pl022_internal_cs(src->base, 1); /* Make sure internal CS is '1' */
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


static void spiflash_read_flash(uint32_t base, uint32_t offset, unsigned char *dest, int len) {
   unsigned char cmd[4];
   cmd[0] = SPIFLASH_READDATA;
   cmd[1] = (unsigned char) ((offset >> 16) & 0xff);
   cmd[2] = (unsigned char) ((offset >> 8)  & 0xff);
   cmd[3] = (unsigned char) ((offset >> 0)  & 0xff);

   uint32_t timeout = 100 + len*4;
   pl022_xfer_timeout(base, cmd, cmd, ARRAY_SIZE(cmd), timeout);
   pl022_xfer_timeout(base, NULL, dest, len, timeout);

}

static bool spiflash_init(const struct rumboot_bootsource *src, void *pdata)
{
	struct spiflash_private_data *spi_flash = (struct spiflash_private_data *)pdata;

   struct pl022_config conf;
   conf.ssp_clk = src->freq_khz * 1000;
   conf.spi_clk = 12500000;
   conf.data_size = 8;
   conf.soft_cs = 1;
   pl022_init(src->base, &conf);

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
