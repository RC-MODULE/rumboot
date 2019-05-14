#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <rumboot/macros.h>
#include <rumboot/rumboot.h>
#include <stdlib.h>
#include <rumboot/boot.h>
#include <rumboot/bootsrc/spiflash.h>
#include <platform/devices.h>
#include <rumboot/timer.h>
#include <rumboot/io.h>
#include <rumboot/hexdump.h>
#include <rumboot/xmodem.h>
#include <regs/regs_gpio_pl061.h>

#ifdef BASIS_SPI0_GP5
#include <regs/regs_gpio_rcm.h>
static bool spi0_1_enable(const struct rumboot_bootsource *src, void *pdata)
{
        uint32_t v;

        v = ioread32(GPIO0_BASE + GPIO_PAD_DIR);
        v |= 1 << 5;
        iowrite32(v, GPIO0_BASE + GPIO_PAD_DIR);
        iowrite32((1 << 5), GPIO0_BASE + GPIO_WR_DATA_SET1);
        return true;
}

static void spi0_1_disable(const struct rumboot_bootsource *src, void *pdata)
{
        uint32_t v;

        v = ioread32(GPIO0_BASE + GPIO_PAD_DIR);
        v &= ~(1 << 5);
        iowrite32(v, GPIO0_BASE + GPIO_PAD_DIR);
}

static void spi0_1_cs(const struct rumboot_bootsource *src, void *pdata, int select)
{
        if (!select) {
                iowrite32(~(1 << 5), GPIO0_BASE + GPIO_WR_DATA_SET0);
        } else {
                iowrite32((1 << 5), GPIO0_BASE + GPIO_WR_DATA_SET1);
        }
}

static const struct rumboot_bootsource src[] = {
        {
                .name = "SPI FLASH (GPIO CS)",
                .base = SPI_BASE,
                .base_freq_khz = 100000,
                .iface_freq_khz = 12500,
                .plugin = &g_bootmodule_spiflash,
                .enable = spi0_1_enable,
                .disable = spi0_1_disable,
                .chipselect = spi0_1_cs,
        }
};

#elif defined(MM7705_SPI)


#define BOOT_SPI_BASE SPI_CTRL0__
#define BOOT_GPIO_FOR_SPI_BASE  GPIO1_BASE
#define BOOT_GPIO_FOR_SPI_PIN  2
#define SPI_CS0 (1 << BOOT_GPIO_FOR_SPI_PIN)

static void mm7705_cs(const struct rumboot_bootsource *src, void *pdata, int select)
{
    if (!select) {
            iowrite32(0, BOOT_GPIO_FOR_SPI_BASE + GPIO_DATA + (SPI_CS0 << 2));
    } else {
            iowrite32(SPI_CS0, BOOT_GPIO_FOR_SPI_BASE + GPIO_DATA + (SPI_CS0 << 2));
    }
}

static bool mm7705_spi_enable(const struct rumboot_bootsource *src, void *pdata)
{
    uint8_t afsel;
    iowrite32(0xff, LSIF1_MGPIO3_BASE + 0x420 );

    afsel = ioread32(BOOT_GPIO_FOR_SPI_BASE + 0x420);
    afsel &= ~(1 << BOOT_GPIO_FOR_SPI_PIN);
    iowrite32(afsel, BOOT_GPIO_FOR_SPI_BASE + 0x420 );

    uint8_t dir = ioread32(BOOT_GPIO_FOR_SPI_BASE + GPIO_DIR);
    dir |= (1 << BOOT_GPIO_FOR_SPI_PIN);
    iowrite32(dir, BOOT_GPIO_FOR_SPI_BASE + GPIO_DIR);

}

static bool mm7705_spi_disable(const struct rumboot_bootsource *src, void *pdata)
{

    uint8_t afsel;
    afsel = ioread32(LSIF1_MGPIO3_BASE + 0x420);
    afsel &= ~0b01110000;
    iowrite32(afsel, LSIF0_MGPIO3_BASE + 0x420);

    uint8_t dir = ioread32(BOOT_GPIO_FOR_SPI_BASE + GPIO_DIR);
    dir &= ~(1 << BOOT_GPIO_FOR_SPI_PIN);
    iowrite32(dir, BOOT_GPIO_FOR_SPI_BASE + GPIO_DIR);

}


static const struct rumboot_bootsource src[] = {
        {
                .name = "SPI FLASH",
                .base = SPI_BASE,
                .base_freq_khz = 30000,
                .plugin = &g_bootmodule_spiflash,
		.enable = mm7705_spi_enable,
		.disable = mm7705_spi_disable,
		.chipselect = mm7705_cs
        },
};
#else
static const struct rumboot_bootsource src[] = {
        {
                .name = "SPI FLASH",
                .base = SPI_BASE,
                .base_freq_khz = 100000,
                .plugin = &g_bootmodule_spiflash,
        },
};
#endif


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
#define SPIFLASH_UNIQUEID      0x4B

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
	mdelay(1);
}

static void spiflash_cmd(const struct rumboot_bootsource *src, void *pdata, uint8_t cmd)
{
	spi_cs(src, (void *) pdata, 0);
	pl022_xfer(src->base, &cmd, &cmd, 1);
	spi_cs(src, (void *) pdata, 1);
}

uint8_t spiflash_status(const struct rumboot_bootsource *src, void *pdata)
{
  uint8_t cmd[2];
  cmd[0] = SPIFLASH_READSTAT1;
  cmd[1] = 0x0;
  spi_cs(src, (void *) pdata, 0);
  pl022_xfer(src->base, &cmd, &cmd, 2);
  spi_cs(src, (void *) pdata, 1);	
  return cmd[1];
}

void spiflash_wait_busy(const struct rumboot_bootsource *src, void *pdata)
{
	while (spiflash_status(src,pdata) & 0x1);;
}

static void spiflash_write_page(const struct rumboot_bootsource *src, void *pdata, uint32_t offset, void *data)
{
	uint8_t wcmd[256 + 4];
	wcmd[0] = SPIFLASH_PAGEPROG;
	wcmd[1] = (offset >> 16) & 0xff;
	wcmd[2] = (offset >> 8) & 0xff;
	wcmd[3] = offset & 0xff;
	memcpy(&wcmd[4], data, 256);

  spi_cs(src, (void *) pdata, 0);
	pl022_xfer(src->base, wcmd, wcmd, ARRAY_SIZE(wcmd));
  spi_cs(src, (void *) pdata, 1);

	spiflash_wait_busy(src, pdata);
}

static void spiflash_read_flash(uint32_t base, uint32_t offset, unsigned char *dest, int len) 
{
   unsigned char cmd[4];
   cmd[0] = SPIFLASH_READDATA;
   cmd[1] = (unsigned char) ((offset >> 16) & 0xff);
   cmd[2] = (unsigned char) ((offset >> 8)  & 0xff);
   cmd[3] = (unsigned char) ((offset >> 0)  & 0xff);

   uint32_t timeout = 100 + len*4;
   pl022_xfer_timeout(base, cmd, cmd, ARRAY_SIZE(cmd), timeout);
   pl022_xfer_timeout(base, NULL, dest, len, timeout);

}

int spiflash_is_blank(const struct rumboot_bootsource *src, void *pdata)
{
        int i;
        char buf[256];
	bootsource_read(src, (void *) pdata, buf, 0, 256);
        for (i=0; i<256; i++) {
                if (buf[i]!=0xff) {
                        return 0;
                }
        }
        return 1;
}

int main()
{
	char pdata[64];
	char buf[4096];
	size_t ret;
	int i;
	bootsource_init(src, (void *) pdata);

	size_t transfer = 4096;
	int pos=0;


	rumboot_printf("Erasing whole flash...\n");
	spiflash_cmd(src, pdata, SPIFLASH_WRITEENABLE);
	spiflash_cmd(src, pdata, SPIFLASH_CHIPERASE);
	spiflash_wait_busy(src, pdata);
	rumboot_printf("Done\n");	


        if (!spiflash_is_blank(src, pdata)) {
                rumboot_printf("Trying alternate chip erase...\n");
	        spiflash_cmd(src, pdata, SPIFLASH_WRITEENABLE);
	        spiflash_cmd(src, pdata, SPIFLASH_ALT_CHIPERASE);
	        spiflash_wait_busy(src, pdata);
        }

	while (transfer == 4096) {
		rumboot_printf("boot: Press 'X' and send me the image\n");
	    while ('X' != rumboot_getchar(1000));;

		transfer = xmodem_get(buf, 4096);
		for (i=0; i<4096; i=i+256) {
			char tmp[256];
			spiflash_cmd(src, pdata, SPIFLASH_WRITEENABLE);			
			spiflash_write_page(src, (void *) pdata, pos, &buf[i]);
			spiflash_cmd(src, pdata, SPIFLASH_WRITEDISABLE);			

			bootsource_read(src, (void *) pdata, tmp, pos, 256);
			if (memcmp(&buf[i], tmp, 256)!=0) {
				rumboot_printf("verification failed at offset %d: \nWrote:\n", pos);
				rumboot_hexdump(&buf[i], 256);
				rumboot_printf("But read:\n");
				rumboot_hexdump(tmp, 256);
				rumboot_platform_panic(NULL);
			}
			pos+=256;
		}
	}

	rumboot_printf("Operation completed\n");
	return 0;
}
