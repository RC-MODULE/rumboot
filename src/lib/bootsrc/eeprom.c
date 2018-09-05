#include <rumboot/bootsrc/eeprom.h>
#include <rumboot/printf.h>
#include <stdlib.h>
#include <regs/regs_i2c.h>
#include <rumboot/io.h>


#define EEPROM_PAGE_SIZE 32
struct eeprom_private_data {
	const struct rumboot_bootsource *src;
};

#define CMD_WRITE_START ((1 << I2C_EN_i) | (1 << WR_i) | (1 << START_i))
#define CMD_WRITE ((1 << I2C_EN_i) | (1 << WR_i))
#define CMD_READ_REPEAT_START ((1 << I2C_EN_i) | (1 << RD_i) | (1 << START_i) | (1 << REPEAT_i))
//#define CMD_READ_REPEAT_START ((1 << I2C_EN_i) | (1 << RD_i) | (1 << START_i) | (1 << REPEAT_i))
#define CMD_STOP ((1 << I2C_EN_i) | (1 << STOP_i))
#define CMD_READ_REPEAT_STOP ((1 << I2C_EN_i) | (1 << RD_i) | (1 << START_i) | (1 << REPEAT_i) | (1 << STOP_i))


uint32_t get_i2c_clk(uint32_t clk_apb, uint32_t pr)
{
    return clk_apb / (5 * pr);
}

static uint32_t calc_best_pr(uint32_t clk_apb, uint32_t clk_i2c)
{
#if 1
	return 51;
#else
	uint32_t clk_real = 0;
	uint32_t pr = 1;
	do {
		clk_real = get_i2c_clk(clk_apb, pr++);
	} while ((clk_real > clk_i2c));
	rumboot_printf("pr %d clk %d ? %d\n", pr, clk_real, clk_i2c);
	return pr - 1;
#endif
}

static void i2c_stat_rst(uint32_t base)
{
	iowrite32(0x0, base + I2C_STAT_RST); //reset STATRST reg
	iowrite32(0x1, base + I2C_STAT_RST); //reset STATUS reg
	iowrite32(0x0, base + I2C_STAT_RST); //reset STATRST reg
}


static bool i2c_has_errors(const struct rumboot_bootsource *src)
{
	if (ioread32(src->base + I2C_STATUS) & (1<<9)) {
		dbg_boot(src, "error: nack");
		return true;
	}
	if (ioread32(src->base + I2C_STATUS) & (1<<9)) {
		dbg_boot(src, "error: arbitration lost");
		return true;
	}

	return false;
}

static size_t eeprom_read_chunk(const struct rumboot_bootsource *src, uint8_t devaddr, uint16_t offset, uint8_t *dst, int len)
{
	size_t ret = 0;
	uint32_t base = src->base;
	iowrite32(0x00,   				 base + I2C_STATUS);
	iowrite32(devaddr << 1,          base + I2C_TRANSMIT);
	iowrite32(offset >> 8,           base + I2C_TRANSMIT);
	iowrite32(offset & 0xff,         base + I2C_TRANSMIT);

	iowrite32(1<<0 | 1<<1 | 1<<4,    		         base + I2C_CTRL);

	while (! (ioread32(base + I2C_STATUS) & 0x10)) {
		if (i2c_has_errors(src))
			goto bailout;
	};;

	i2c_stat_rst(base);
	iowrite32(len, base + I2C_NUMBER);
	iowrite32((devaddr << 1) | 1, base + I2C_TRANSMIT);
	iowrite32(CMD_READ_REPEAT_STOP,( base + I2C_CTRL));

	while (len--) {
		while (ioread32(base + I2C_STATUS) & (1 << 7)) {
			if (i2c_has_errors(src))
				goto bailout;
		}
		*dst++ = ioread32(base + I2C_RECEIVE);
		ret++;
	}
bailout:
	i2c_stat_rst(base);
	return ret;
}

const uint32_t get_eeprom_numb()
{
	uint32_t eeprom_numb = 0;

	return (const uint32_t)eeprom_numb;
}

bool eeprom_init(const struct rumboot_bootsource *src, void *pdata)
{
	uint32_t base = src->base;
	int sda, scl;

	uint32_t tmp = ioread32(base + I2C_ID);
	if (tmp != I2C_DEVICE_ID_VALUE) {
		dbg_boot(src, "Invalid device id, bootrom bug?");
		return false;
	}

	i2c_stat_rst(base);
	tmp = ioread32(base + I2C_STATUS);
	dbg_boot(src, "%x", tmp);
	scl = (tmp & (1 << 23)) ? 1 : 0;
	sda = (tmp & (1 << 25)) ? 1 : 0;
	if ((!sda) || (!scl)) {
		dbg_boot(src, "Invalid I2C line state: SDA: %d SCL: %d", sda, scl);
		return false;
	}

	uint32_t pr = calc_best_pr(src->freq_khz * 1000, 400000);
	iowrite32(1, base + I2C_SOFTR);
	iowrite32(0, base + I2C_SOFTR);
	iowrite32(pr, base + I2C_PRESCALE);
	return true;
}

static void eeprom_deinit(const struct rumboot_bootsource *src, void *pdata)
{
	/* Just soft-reset this bitch */
	iowrite32(1, src->base + I2C_SOFTR);
	iowrite32(0, src->base + I2C_SOFTR);
}

static size_t eeprom_read(const struct rumboot_bootsource* src, void* pdata, void* to, size_t offset, size_t length)
{
	uint8_t *dest = to;
	size_t transferred = 0;
	while (length) {
		size_t toread = (length < EEPROM_PAGE_SIZE) ? length : EEPROM_PAGE_SIZE;
		size_t actual = eeprom_read_chunk(src, src->slave_addr, offset, dest, toread);
		transferred += actual;
		if (actual != toread) /* short read, return what we've got */
			return transferred;
		length -= toread;
		offset += toread;
		dest = &dest[toread];
	}
	return transferred;
}

const struct rumboot_bootmodule g_bootmodule_eeprom = {
		.init = eeprom_init,
		.deinit = eeprom_deinit,
		.read = eeprom_read,
		.privdatalen = sizeof(struct eeprom_private_data)
};
