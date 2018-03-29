#include <stdint.h>
#include <stdlib.h>

#include <devices/i2c.h>

#include <regs/regs_i2c.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/timer.h>

static volatile uint32_t irqstat = 0;

#define I2C_TIMEOUT 100
#define I2C_BLOCK_TIMEOUT_US 2500
#define I2C_TIMEOUT_PER_WR_US 9
#define I2C_TIMEOUT_PER_RD_US 9

enum err_code {
	OK = 0,
	NADDR_ERROR = -1,
	NACK_ERROR = -2,
	TX_FIFO_NOT_EMPTY_ERROR = -3,
	RX_FIFO_NOT_FULL = -4
};

#define CMD_WRITE_START ((1 << I2C_EN_i) | (1 << WR_i) | (1 << START_i))
#define CMD_WRITE ((1 << I2C_EN_i) | (1 << WR_i))
#define CMD_READ_REPEAT_START ((1 << I2C_EN_i) | (1 << RD_i) | (1 << START_i) | (1 << REPEAT_i))
//#define CMD_READ_REPEAT_START ((1 << I2C_EN_i) | (1 << RD_i) | (1 << START_i) | (1 << REPEAT_i))
#define CMD_STOP ((1 << I2C_EN_i) | (1 << STOP_i))
#define CMD_READ_REPEAT_STOP ((1 << I2C_EN_i) | (1 << RD_i) | (1 << START_i) | (1 << REPEAT_i) | (1 << STOP_i))

static enum err_code trans_write_devaddr(struct i2c_config *cfg, struct i2c_transaction *t)
{
	uint8_t offset_h = (t->offset & 0xff00) >> 8;
	uint8_t offset_l = t->offset & 0x00ff;

	iowrite8(t->devaddr, cfg->base + I2C_TRANSMIT);
	cfg->txfifo_count++;
	iowrite8(offset_h, cfg->base + I2C_TRANSMIT);
	cfg->txfifo_count++;
	iowrite8(offset_l, cfg->base + I2C_TRANSMIT);
	cfg->txfifo_count++;

	iowrite32(0x1, cfg->base + I2C_STAT_RST);
	iowrite32(CMD_WRITE_START, cfg->base + I2C_CTRL);

	if (i2c_wait_transaction_timeout(cfg, TX_EMPTY, I2C_TIMEOUT) < 0) {
		return -1;
	}

	// if (i2c_wait_transaction_timeout(cfg, DONE, I2C_TIMEOUT) < 0) {
	//      return -1;
	// }

	return 0;
}

static void set_number(uint32_t base, enum device_type type, size_t len)
{
	uint8_t number;

	if (type == EEPROM) {
		//number = 128;
		number = len;
	} else {
		number = 255;
	}

	iowrite8(number, base + I2C_NUMBER);
}

static void set_fifofil(uint32_t base, enum device_type type, size_t len)
{
	uint8_t fifofil;

	if (type == EEPROM) {
		fifofil = (len > 128) ? (len % 128) : (len);
	} else {
		fifofil = (len > 256) ? (len % 256) : (len);
	}

	iowrite32((fifofil << 16) | (fifofil), base + I2C_FIFOFIL);
}

static enum err_code write_data_chunk(uint32_t base, size_t *txfifo_count, void *buf, uint32_t len)
{
	if (buf == NULL || len == 0) {
		return -1;
	}

	while (len--) {
		if ((*txfifo_count) == 256) {
			rumboot_printf("TX buffer have overflowed!\n");
			*txfifo_count = 0;
			return len + 1;
		}

		iowrite8(*((uint8_t *)buf), base + I2C_TRANSMIT);
		(*txfifo_count)++;
		(uint8_t *)buf++;
	}

	return 0;
}

static enum err_code send_write_cmd(uint32_t base /*, uint32_t fifofil*/)
{
	//Before - reset old state!
	iowrite32(0x1, base + I2C_STAT_RST);
	iowrite32(CMD_WRITE, base + I2C_CTRL);

	return 0;
}

#define TXBUF_SIZE 256
static enum err_code trans_write_data(struct i2c_config *cfg, struct i2c_transaction *t)
{
	int ret;
	//usually equals to 256!
	uint32_t rem = 0;
	size_t n = 1;

	if (t->len > TXBUF_SIZE) {
		n = t->len / TXBUF_SIZE;
		rem = (ioread32(cfg->base + I2C_FIFOFIL) & 0xff);
		rumboot_printf("n: %d, fifofil: %d\n", n, rem);
	}

	set_number(cfg->base, cfg->device_type, t->len);
	set_fifofil(cfg->base, cfg->device_type, t->len);

	while (n--) {
		ret = (t->len > TXBUF_SIZE) ? write_data_chunk(cfg->base, &cfg->txfifo_count, t->buf, TXBUF_SIZE) :
		      write_data_chunk(cfg->base, &cfg->txfifo_count, t->buf, t->len);

		if (ret != 0) {
			if (ret < 0) {
				return -1;
			}

			if (ret > 0) {
				rem += ret;
				rumboot_printf("remainder: %d\n", rem);
			}
		}

		send_write_cmd(cfg->base);

		if (i2c_wait_transaction_timeout(cfg, TX_EMPTY, I2C_TIMEOUT_PER_WR_US * TXBUF_SIZE) < 0) {
			return -2;
		}
	}

	if (rem) {
		rumboot_printf("Write remainder.\n");
		set_fifofil(cfg->base, cfg->device_type, rem);

		ret = write_data_chunk(cfg->base, &cfg->txfifo_count, t->buf, rem);

		if (ret < 0) {
			return -3;
		}

		send_write_cmd(cfg->base);

		if (i2c_wait_transaction_timeout(cfg, TX_EMPTY, I2C_TIMEOUT_PER_WR_US * rem) < 0) {
			return -4;
		}
	}

	return 0;
}

static enum err_code read_data_chunk(uint32_t base, void *buf, size_t len)
{
	if (buf == NULL || len == 0) {
		return -1;
	}

	while (len--) {
		*((uint8_t *)buf) = ioread8(base + I2C_RECEIVE);
		(uint8_t *)buf++;
	}

	return 0;
}

static enum err_code send_read_cmd(struct i2c_config *cfg, uint8_t devaddr, bool do_stop)
{
	uint32_t cmd = (do_stop) ? CMD_READ_REPEAT_STOP : CMD_READ_REPEAT_START;

	iowrite8(devaddr + 1, cfg->base + I2C_TRANSMIT);
	iowrite32(0x1, cfg->base + I2C_STAT_RST);
	iowrite32(cmd, cfg->base + I2C_CTRL);

	return 0;
}

#define RXBUF_SIZE 256
static enum err_code trans_read_data(struct i2c_config *cfg, struct i2c_transaction *t)
{
	set_number(cfg->base, cfg->device_type, t->len);
	set_fifofil(cfg->base, cfg->device_type, t->len);

	size_t numb = ioread8(cfg->base + I2C_NUMBER);
	uint32_t rem = 0;
	size_t n = 1;
	enum i2c_waited_event e = (t->len > numb) ? RX_FULL : RX_FULL_ALMOST;
	bool do_stop = (t->len > numb) ? false : true;

	if (numb == 255) {
		numb += 1;
	}

	if (t->len > numb) {
		n = t->len / numb;
		rem = (ioread32(cfg->base + I2C_FIFOFIL) & 0xff0000) >> 16;
	}

	while (n--) {
		if (n == 0 && t->len > numb && rem == 0) {
			do_stop = true;
		}

		if (cfg->device_type == EEPROM) {
			do_stop = true;
		}

		send_read_cmd(cfg, t->devaddr, do_stop);

		if (i2c_wait_transaction(cfg, e) != 0) {
			return -1;
		}

		//udelay(I2C_TIMEOUT_PER_WR_US*numb);

		(t->len > 256) ? read_data_chunk(cfg->base, t->buf, TXBUF_SIZE) : read_data_chunk(cfg->base, t->buf, t->len);
	}

	if (rem) {
		set_fifofil(cfg->base, cfg->device_type, rem);

		send_read_cmd(cfg, t->devaddr, true);

		if (i2c_wait_transaction(cfg, RX_FULL_ALMOST) != 0) {
			return -2;
		}

		//udelay(I2C_TIMEOUT_PER_WR_US*rem*10);

		read_data_chunk(cfg->base, t->buf, rem);
	}

	return 0;
}

static bool is_tx_empty(uint32_t base, bool is_irq)
{
	bool ret = false;
	uint32_t stat = ioread32(base + I2C_STATUS);

	if (is_irq) {
		ret = stat & (1 << INT_TRN_EMPTY_i);
	} else {
		ret = stat & (1 << TX_EMPTY_i);
	}

	return ret;
}

static bool is_rx_full_almost(uint32_t base, bool is_irq)
{
	bool ret = false;
	uint32_t stat = ioread32(base + I2C_STATUS);

	if (is_irq) {
		ret = stat & (1 << INT_RCV_FULL_ALM_i);
	} else {
		ret = stat & (1 << RX_FULL_ALM_i);
	}


	return ret;
}

static bool is_tx_empty_almost(uint32_t base, bool is_irq)
{
	bool ret = false;
	uint32_t stat = ioread32(base + I2C_STATUS);

	if (is_irq) {
		ret = stat & (1 << INT_TRN_EMPTY_ALM_i);
	} else {
		//FIX IT!
		ret = stat & (1 << TX_EMPTY_ALMOST_i);
	}

	return ret;
}

static bool is_rx_full(uint32_t base, bool is_irq)
{
	bool ret = false;
	uint32_t stat = ioread32(base + I2C_STATUS);

	if (is_irq) {
		ret = stat & (1 << INT_RCV_FULL_i);
	} else {
		ret = stat & (1 << RX_FULL_i);
	}

	return ret;
}

static bool is_op_done(uint32_t base, bool is_irq)
{
	bool ret = false;
	uint32_t stat = ioread32(base + I2C_STATUS);

	if (is_irq) {
		ret = stat & (1 << INT_DONE_i);
	} else {
		ret = stat & (1 << DONE_i);
	}

	return ret;
}

static int i2c_check_transaction(struct i2c_config *cfg, enum i2c_waited_event e)
{
	switch (e) {
	case DONE:
		return is_op_done(cfg->base, cfg->irq_en) ? 0 : -1;
	case TX_EMPTY:
		return is_tx_empty(cfg->base, cfg->irq_en) ? 0 : -1;
	case RX_FULL_ALMOST:
		return is_rx_full_almost(cfg->base, cfg->irq_en) ? 0 : -1;
	case TX_EMPTY_ALMOST:
		return is_tx_empty_almost(cfg->base, cfg->irq_en) ? 0 : -1;
	case RX_FULL:
		return is_rx_full(cfg->base, cfg->irq_en) ? 0 : -1;
	default:
		break;
	}

	return 0;
}

#if 0
static void  i2c_enable(uint32_t base)
{
	uint8_t ctrl = ioread8(base + I2C_CTRL);

	iowrite8(ctrl | (1 << I2C_EN_i), base + I2C_CTRL);
}
#endif

static void i2c_irq_enable(uint32_t base)
{
	iowrite32((1 << EN_INT_TRN_EMPTY_i) | (1 << EN_INT_RX_AL_FULL_i), base + I2C_IER);
}

void i2c_init(struct i2c_config *cfg)
{
	//private data
	cfg->txfifo_count = 0;

	size_t number = (cfg->byte_numb < 256) ? cfg->byte_numb : 255;
	size_t fifofil = (cfg->byte_numb < 256) ? cfg->byte_numb : cfg->byte_numb % 256;

	iowrite32(cfg->scl_freq, cfg->base + I2C_PRESCALE);
	iowrite8(number, cfg->base + I2C_NUMBER);
	iowrite32((fifofil << 16) | (fifofil), cfg->base + I2C_FIFOFIL);

	//i2c_enable(cfg->base);

	if (cfg->irq_en == true) {
		i2c_irq_enable(cfg->base);
	}
}

void i2c_irq_handler(int irq, void *arg)
{
	struct i2c_config *cfg = (struct i2c_config *)arg;

	irqstat |= ioread32(cfg->base + I2C_ISR);

	rumboot_printf("IRQ arrived, irq status %x\n", irqstat);
}

int i2c_execute_transaction(struct i2c_config *cfg, struct i2c_transaction *t)
{
	if (t->type == WRITE_DEV) {
		return trans_write_devaddr(cfg, t);
	}

	if (t->type == WRITE_DATA) {
		return trans_write_data(cfg, t);
	}

	if (t->type == READ_DATA) {
		return trans_read_data(cfg, t);
	}

	return 0;
}

int i2c_stop_transaction(struct i2c_config *cfg)
{
	if (i2c_wait_transaction_timeout(cfg, TX_EMPTY, I2C_TIMEOUT * 5) < 0) {
		//return -1;
	}

	cfg->txfifo_count = 0;

	iowrite32(CMD_STOP, cfg->base + I2C_CTRL);

	return 0;
}

int i2c_wait_transaction(struct i2c_config *cfg, enum i2c_waited_event e)
{
	size_t n = 0;

	while (i2c_check_transaction(cfg, e) < 0) {
		if (!(n % 100)) {
			rumboot_printf("waiting event %d\n", e);
		}
	}

	return 0;
}

int i2c_wait_transaction_timeout(struct i2c_config *cfg, enum i2c_waited_event e, uint32_t us)
{
	uint32_t uptime = rumboot_platform_get_uptime();

	while (i2c_check_transaction(cfg, e) < 0) {
		if (rumboot_platform_get_uptime() - uptime > us) {
			rumboot_printf("Timeout. Was not waiting enent №%d, expired %d us.\n", e, us);
			return -1;
		}
	}

	return 0;
}
