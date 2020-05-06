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
#include <devices/pl022.h>
#include <regs/regs_gpio_pl061.h>
#include <string.h>


#define	SPIFLASH_MANID         0x90
#define	SPIFLASH_MANID2        0x90
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
  cmd[0] = 0xD7;
  cmd[1] = 0x0;
  spi_cs(src, (void *) pdata, 0);
  pl022_xfer(src->base, (uint8_t *) &cmd, (uint8_t *) &cmd, 2);
  spi_cs(src, (void *) pdata, 1);	
  return cmd[1];
}

void spiflash_wait_busy(const struct rumboot_bootsource *src, void *pdata)
{
	while ((spiflash_status(src,pdata) & (1<<7)) == 0);;
}

//3Dh + 2Ah + 80h + A6h

void spiflash_st45_set_256(const struct rumboot_bootsource *src, void *pdata)
{
	uint8_t wcmd[128 + 4];
	wcmd[0] = 0x3D;
	wcmd[1] = 0x2A;
	wcmd[2] = 0x80;
	wcmd[3] = 0xA6;

	spi_cs(src, (void *) pdata, 0);
	pl022_xfer(src->base, wcmd, wcmd, ARRAY_SIZE(wcmd));
    spi_cs(src, (void *) pdata, 1);

	spiflash_wait_busy(src, pdata);
}

void spiflash_erase_at45(const struct rumboot_bootsource *src, void *pdata)
{
	uint8_t wcmd[128 + 4];
	wcmd[0] = 0xC7;
	wcmd[1] = 0x94;
	wcmd[2] = 0x80;
	wcmd[3] = 0x9A;

	spi_cs(src, (void *) pdata, 0);
	pl022_xfer(src->base, wcmd, wcmd, ARRAY_SIZE(wcmd));
    spi_cs(src, (void *) pdata, 1);

	spiflash_wait_busy(src, pdata);
}
static void spiflash_write_page(const struct rumboot_bootsource *src, void *pdata, uint32_t offset, void *data)
{
	uint8_t wcmd[128 + 4];
	wcmd[0] = SPIFLASH_PAGEPROG;
	wcmd[1] = (offset >> 16) & 0xff;
	wcmd[2] = (offset >> 8) & 0xff;
	wcmd[3] = offset & 0xff;
	memcpy(&wcmd[4], data, 128);

    spi_cs(src, (void *) pdata, 0);
	pl022_xfer(src->base, wcmd, wcmd, ARRAY_SIZE(wcmd));
    spi_cs(src, (void *) pdata, 1);

	spiflash_wait_busy(src, pdata);
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

#ifndef BOOT_ID
#error "Updater needs a BOOT_ID defined"
#endif

//static char sectorbuf[256];
void packet_rx_cb(size_t curpos, void *ptr, size_t length, void *src)
{
	char pdata[512];

//	if (curpos % 256) {
//		memcpy(&sectorbuf[128], ptr, 128);
//	} else {
		spiflash_cmd(src, pdata, SPIFLASH_WRITEENABLE);			
		spiflash_write_page(src, (void *) pdata, curpos, ptr);
		spiflash_cmd(src, pdata, SPIFLASH_WRITEDISABLE);
//		memcpy(sectorbuf, ptr, 128);		
//	}
}

void adjust_speed(int baudrate)
{
	struct rumboot_config conf;
    rumboot_platform_read_config(&conf);
	conf.baudrate = baudrate;
    rumboot_platform_init_loader(&conf);
}

#include <platform/devices.h>
#include <devices/uart_pl011.h>
#include <regs/regs_uart_pl011.h>


#if 0
struct flasher_plugin {
	struct bootsrc *src;
	size_t chunksize;
	size_t offset;
	int (*prepare)(struct flasher_plugin *flp);
	int (*write_chunk)(struct flasher_plugin *flp, size_t offset, void *data);
};

int flashapp_execute(struct flasher_plugin *flp)
{
	char pdata[64];
	rumboot_platform_runtime_info->silent = false;

}

int main()
{
	char pdata[64];
	rumboot_platform_runtime_info->silent = false;
	while (1) {
		int i = 0;
		scanf("HELLO: %d\n", &i);
		rumboot_printf("%d i\n");
	}
}
#endif
#if 1
int main()
{
	char pdata[64];
	rumboot_platform_runtime_info->silent = false;
    const struct rumboot_bootsource *src = &rumboot_platform_get_bootsources()[BOOT_ID];
	bootsource_init(src, (void *) pdata);
	//adjust_speed(115200);
	//iowrite32(ioread32(UART0_BASE + UARTLCR_H) | 1<<3, UART0_BASE + UARTLCR_H);

	spiflash_st45_set_256(src, pdata);


	rumboot_printf("Erasing whole flash...\n");
	spiflash_cmd(src, pdata, SPIFLASH_WRITEENABLE);
	spiflash_cmd(src, pdata, SPIFLASH_CHIPERASE);
	spiflash_wait_busy(src, pdata);
	rumboot_printf("Done\n");	
	char tmp[4096];

    if (!spiflash_is_blank(src, pdata)) {
            rumboot_printf("Trying alternate chip erase...\n");
	    spiflash_cmd(src, pdata, SPIFLASH_WRITEENABLE);
	    spiflash_cmd(src, pdata, SPIFLASH_ALT_CHIPERASE);
	    spiflash_wait_busy(src, pdata);
    }

	uint8_t st = spiflash_status(src, pdata) & 1;
	if (!st) {
		rumboot_printf("WARNING: 264b page size ;(\n");
	}


    if (!spiflash_is_blank(src, pdata)) {
            rumboot_printf("Trying yet another alternate chip erase...\n");
	    spiflash_cmd(src, pdata, SPIFLASH_WRITEENABLE);
		spiflash_erase_at45(src, pdata);
	    spiflash_wait_busy(src, pdata);
    }


    rumboot_printf("boot: Press 'X' and send me the image\n");
    while ('X' != rumboot_getchar(1000));;

	size_t bytes = xmodem_get_async(512 * 1024 * 1024, packet_rx_cb, (void *) src);
	rumboot_printf("Operation completed, %d bytes written\n", bytes);

	bootsource_read(src, (void *) pdata, tmp, 0, 4096);
	rumboot_hexdump(tmp, 1024);
	return 0;
}
#endif