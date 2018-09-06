#include <rumboot/bootsrc/eeprom.h>
#include <rumboot/printf.h>
#include <stdlib.h>
#include <regs/regs_i2c.h>
#include <rumboot/io.h>
#include <rumboot/timer.h>

#define EEPROM_PAGE_SIZE 32
#define I2C_FREQ 400000

struct eeprom_private_data {

};

#define CMD_WRITE_START ((1 << I2C_EN_i) | (1 << WR_i) | (1 << START_i))
#define CMD_WRITE ((1 << I2C_EN_i) | (1 << WR_i))
#define CMD_READ_REPEAT_START ((1 << I2C_EN_i) | (1 << RD_i) | (1 << START_i) | (1 << REPEAT_i))
#define CMD_STOP ((1 << I2C_EN_i) | (1 << STOP_i))
#define CMD_READ_REPEAT_STOP ((1 << I2C_EN_i) | (1 << RD_i) | (1 << START_i) | (1 << REPEAT_i) | (1 << STOP_i))


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

static size_t eeprom_read_chunk(const struct rumboot_bootsource *src, uint8_t devaddr, uint16_t offset, uint8_t *dst, int len)
{
	size_t ret = 0;
	uint32_t base = src->base;
	iowrite32(0x00,   				 base + I2C_STATUS);
	iowrite32(devaddr << 1,          base + I2C_TRANSMIT);
	iowrite32(offset >> 8,           base + I2C_TRANSMIT);
	iowrite32(offset & 0xff,         base + I2C_TRANSMIT);
	iowrite32(1<<0 | 1<<1 | 1<<4,    base + I2C_CTRL);

	bool timeout = true;

	LOOP_UNTIL_TIMEOUT(100) {
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

	i2c_stat_rst(base);
	iowrite32(len, base + I2C_NUMBER);
	iowrite32((devaddr << 1) | 1, base + I2C_TRANSMIT);
	iowrite32(CMD_READ_REPEAT_STOP,( base + I2C_CTRL));

	while (len--) {
		uint32_t have_bytes = 0;
		LOOP_UNTIL_TIMEOUT(100) {
			have_bytes = !(ioread32(base + I2C_STATUS) & (1 << 7));
			if (have_bytes)
				break;
		}

		if (len && i2c_is_nack(src->base)) {
				dbg_boot(src, "error: unexpected nack at byte %d", offset + ret);
				goto bailout;
			}

		if (i2c_is_arblost(src->base)) {
				dbg_boot(src, "error: arbitration lost at byte %d", offset + ret);
				goto bailout;
			}

		if (!have_bytes) {
			dbg_boot(src, "error: operation timed out at byte %d", offset + ret);
			goto bailout;
		}

		*dst++ = ioread32(base + I2C_RECEIVE);
		ret++;
	}

	/* Wait for hardware to release SDA & SCL */
	LOOP_UNTIL_TIMEOUT(40) {
		if (ioread32(base + I2C_STATUS) & (1 << 10))
			break;
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
	scl = (tmp & (1 << 23)) ? 1 : 0;
	sda = (tmp & (1 << 25)) ? 1 : 0;
	if ((!sda) || (!scl)) {
		dbg_boot(src, "Invalid I2C line state: SDA: %d SCL: %d", sda, scl);
		return false;
	}

	uint32_t pr = calc_best_pr(src->freq_khz * 1000, I2C_FREQ);
    i2c_reset(base);
	iowrite32(pr, base + I2C_PRESCALE);
	dbg_boot(src, "Prescaler: 0x%x (%d Hz)", pr, get_i2c_clk(src->freq_khz * 1000, pr));
	return true;
}

static void eeprom_deinit(const struct rumboot_bootsource *src, void *pdata)
{
	/* Just soft-reset this bitch */
    i2c_reset(src->base);
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
