#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>

#include <rumboot/rumboot.h>
#include <stdlib.h>
#include <rumboot/boot.h>
#include <rumboot/bootsrc/eeprom.h>
#include <platform/devices.h>
#include <rumboot/timer.h>
#include <regs/regs_i2c.h>
#include <rumboot/io.h>
#include <rumboot/hexdump.h>
#include <rumboot/xmodem.h>

struct eeprom_private_data {
	uint32_t prescaler;
};

static const struct rumboot_bootsource src[] = {
        {
                .name = "I2C EEPROM",
                .base = I2C_BASE,
                .slave_addr = EEPROM_ADDRESS,
                .base_freq_khz = 100000,
				.iface_freq_khz = 400,
                .plugin = &g_bootmodule_eeprom,
        },
};



uint32_t get_i2c_clk(uint32_t clk_apb, uint32_t pr)
{
    return clk_apb / (5 * (pr + 1));
}

static uint32_t calc_best_pr(uint32_t clk_apb, uint32_t clk_i2c)
{
	return clk_apb / (5 * clk_i2c) - 1;
}

static void i2c_stat_rst(uint32_t base)
{
	iowrite32(0x0, base + I2C_STAT_RST);
	iowrite32(0x1, base + I2C_STAT_RST);
	iowrite32(0x0, base + I2C_STAT_RST);
}

static void i2c_reset(uint32_t base)
{
	iowrite32(1, base + I2C_SOFTR);
	iowrite32(0, base + I2C_SOFTR);
}

static bool i2c_is_nack(uint32_t base)
{
	return ioread32(base + I2C_STATUS) & (1<<9);
}

static bool i2c_is_arblost(uint32_t base)
{
	return ioread32(base + I2C_STATUS) & (1<<1);
}

static bool i2c_is_busy(uint32_t base)
{
	return ioread32(base + I2C_STATUS) & (1<<0);
}

static void i2c_resetinit(uint32_t base, uint32_t prescaler)
{
	i2c_reset(base);
	iowrite32(prescaler, base + I2C_PRESCALE);
}

static size_t eeprom_write_chunk(const struct rumboot_bootsource *src, struct eeprom_private_data *epd, uint8_t devaddr, uint16_t offset, uint8_t *data, int len)
{
	size_t ret = 0;
	uint32_t base = src->base;
	iowrite32(0x00,   				 base + I2C_STATUS);
	iowrite32(devaddr << 1,          base + I2C_TRANSMIT);
	iowrite32(offset >> 8,           base + I2C_TRANSMIT);
	iowrite32(offset & 0xff,         base + I2C_TRANSMIT);

	int i=0; 
	while(len--) {
		iowrite32(data[i++],           base + I2C_TRANSMIT);		
	}
	iowrite32(len, 					 base + I2C_NUMBER);	
	iowrite32(1<<0 | 1<<1 | 1<<4,    base + I2C_CTRL);

	bool timeout = true;

	LOOP_UNTIL_TIMEOUT(100 * len) {
		if (ioread32(base + I2C_STATUS) & 0x10) {
			timeout = false;
			break;
		}

		if (i2c_is_nack(base)) {
			dbg_boot(src, "error: nack while writing eeprom offset");
			goto bailout;
		}
		if (i2c_is_arblost(base)) {
			dbg_boot(src, "error: arbitration lost while writing eeprom offset");
			goto bailout;
		}
	};

	if (timeout) {
		dbg_boot(src, "error: timeout while writing eeprom offset (%d)", offset);
		goto bailout;
	}

	/** Issue stop */
    uint32_t rdata;
    rdata = ioread32 (base + I2C_CTRL);
    iowrite32 (rdata | 0x40, I2C0_BASE + I2C_CTRL);
	
	LOOP_UNTIL_TIMEOUT(400) {
        /* Wait for transaction to finish */
		i2c_stat_rst(base);
		if (!i2c_is_busy(base))
			break;
	}

	if (i2c_is_busy(base)) {
		dbg_boot(src, "WARN: Timeout waiting for bus release");
	}

	/* Hardware weirdness: We have to do softreset to get rid of
	*  NACK from STATUS register
	*/
	i2c_resetinit(base, epd->prescaler);

bailout:
	i2c_stat_rst(base);
	return ret;
}

static char pdata[64];

void packet_rx_cb(size_t curpos, void *ptr, size_t length, void *arg)
{
	int i;
	char *buf = ptr;
	int ret;
	for (i=0; i<length; i=i+16) {
		ret = eeprom_write_chunk(src, (void *) pdata, src->slave_addr, curpos + i, &buf[i], 16);
		mdelay(5);		
	}
}

#define TRANSFER_CHUNK 4096
int main()
{

	char buf[TRANSFER_CHUNK];
	rumboot_platform_runtime_info->silent = false;
	bootsource_init(src, (void *) pdata);
	size_t ret = bootsource_read(src, (void *) pdata, buf, 0, 512);
	rumboot_printf("Read %d bytes\n", ret);
	rumboot_hexdump(buf, 512);

	size_t transfer = TRANSFER_CHUNK;
	int pos=0;


    rumboot_printf("boot: Press 'X' and send me the image\n");
    while ('X' != rumboot_getchar(1000));;

	size_t bytes = xmodem_get_async(512 * 1024 * 1024, packet_rx_cb, (void *) src);
	rumboot_printf("Operation completed, %d bytes written\n", bytes);

	
	rumboot_printf("Operation completed\n");
	return 0;
}
