#include <stdint.h>
#include <stdlib.h>

#include <devices/i2c.h>

#include <regs/regs_i2c.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/timer.h>

static volatile uint32_t irqstat = 0;


enum i2c_state_cmd {
	ST_IDLE = 0b0000,
	ST_START = 0b0001,
	ST_READ = 0b0010,
	ST_WRITE = 0b0011,
	ST_ACK = 0b0100,
	ST_STOP = 0b0101,
};

static void  i2c_enable(uint32_t base)
{
	uint8_t ctrl = ioread8(base + I2C_CTRL);

	iowrite8(ctrl | (1 << I2C_EN_i), base + I2C_CTRL);
}

static void i2c_irq_enable(uint32_t base)
{
	iowrite32((1 << EN_INT_TRN_EMPTY) | (1 << EN_INT_RX_AL_FULL), base + I2C_IER);
}

void i2c_init(struct i2c_config *cfg)
{
	iowrite32(cfg->scl_freq, cfg->base + I2C_PRESCALE);
	iowrite8(cfg->byte_numb, cfg->base + I2C_NUMB);
	iowrite32((cfg->byte_numb << 16) | (cfg->byte_numb), cfg->base + I2C_FIFOFIL);

	i2c_enable(cfg->base);

	if (cfg->irq_en == true)
		i2c_irq_enable(cfg->base);
}

void i2c_irq_handler(int irq, void *arg)
{
	struct i2c_config *cfg = (struct i2c_config *)arg;

	irqstat |= ioread32(cfg->base + I2C_ISR);

	rumboot_printf("IRQ arrived, irq status %x\n", irqstat);
}

#if 0
static enum i2c_state_cmd i2c_get_state(uint32_t base)
{
	return (enum i2c_state_cmd)((ioread32(base + I2C_STATUS) && 0x3c00) >> 10);
}
#endif

#if 0
static bool wait_state(uint32_t base, enum i2c_state_cmd state)
{
	int count = 0;

	while (i2c_get_state(base) != state)
		if (count++ == I2C_TIMEOUT)
			return false;

	return true;
}
#endif

int i2c_wait_transaction(struct i2c_config *cfg, enum waited_event e)
{
	while (i2c_check_transaction(cfg, e) < 0) ; ;

	return 0;
}

int i2c_wait_transaction_timeout(struct i2c_config *cfg, enum waited_event e, uint32_t us)
{
	uint32_t uptime = rumboot_platform_get_uptime();

	while (i2c_check_transaction(cfg, e) < 0) {
		if (rumboot_platform_get_uptime() - uptime > us) {
			rumboot_printf("Timeout. Was not waiting TX fifo empty.\n");
			return -1;
		}
	}

	return 0;
}

int i2c_check_transaction(struct i2c_config *cfg, enum waited_event e)
{
	if(cfg->irq_en) rumboot_printf("irq status %x\n", irqstat);

	if (cfg->irq_en) {
		if ((e == TX_EMPTY) && (irqstat & (1 << INT_TRN_EMPTY_i))) {
			rumboot_printf("res: %d\n", irqstat & (1 << INT_TRN_EMPTY_i));
			return 0;
		}

		if ((e == RX_FULL_ALM) && (irqstat & (1 << INT_RCV_FULL_ALM_i)))
			return 0;
	} else {
		if ((e == TX_EMPTY) && (ioread32(cfg->base + I2C_STATUS) & (1 << TX_EMPTY_i)))
			return 0;
		if ((e == RX_FULL_ALM) && (ioread32(cfg->base + I2C_STATUS) & (1 << RX_FULL_ALM_i)))
			return 0;
	}

	return -1;
}

enum err_code {
	OK = 0,
	NADDR_ERROR = -1,
	NACK_ERROR = -2,
	TX_FIFO_NOT_EMPTY_ERROR = -3,
	RX_FIFO_NOT_FULL = -4
};

static enum err_code send_write_cmd(struct i2c_config *cfg, uint8_t slave_dev, uint16_t offset)
{
	uint8_t offset_h = (offset && 0xff00) >> 8;
	uint8_t offset_l = offset && 0x00ff;

	//if (i2c_get_state(cfg->base) == ST_IDLE)
	iowrite8(slave_dev, cfg->base + I2C_TRANSMIT);

	iowrite8(offset_h, cfg->base + I2C_TRANSMIT);
	iowrite8(offset_l, cfg->base + I2C_TRANSMIT);

	iowrite32((1 << I2C_EN_i) | (1 << WR_i) | (1 << START_i), cfg->base + I2C_CTRL);

	if (i2c_wait_transaction_timeout(cfg, TX_EMPTY, 500))
		return TX_FIFO_NOT_EMPTY_ERROR;

	// if (ioread32(cfg->base + I2C_ISR) & (1 << NADDR_i))
	//      return NADDR_ERROR;
	//
	// if (ioread32(cfg->base + I2C_ISR) & (1 << NACK_i))
	//      return NACK_ERROR;

	return 0;
}

int eeprom_write(struct i2c_config *cfg, uint8_t slave_dev, uint16_t offset, void *buf, size_t number)
{
	if (send_write_cmd(cfg, slave_dev, offset) < 0)
		return -1;

	while (number--) {
		iowrite8(*((uint8_t *)buf), cfg->base + I2C_TRANSMIT);
		(uint8_t *)buf++;
	}

	iowrite32((1 << I2C_EN_i) | (1 << WR_i), (cfg->base + I2C_CTRL));

	if (i2c_wait_transaction_timeout(cfg, TX_EMPTY, 500) < 0)
		return -2;

	iowrite32((1 << STOP_i), cfg->base + I2C_CTRL);

	return 0;
}

int eeprom_random_read(struct i2c_config *cfg, uint8_t slave_dev, uint16_t offset, void *buf)
{
	uint8_t byte_numb = ioread8(cfg->base + I2C_NUMB);

	rumboot_printf("Byte number: %d\n", byte_numb);
	rumboot_printf("fifofil: %x\n", ioread32(cfg->base + I2C_FIFOFIL));

	if (send_write_cmd(cfg, slave_dev, offset) < 0)
		return -1;

	//LA said what need add 1! Ask her about this
	iowrite8(slave_dev + 1, cfg->base + I2C_TRANSMIT);

	//iowrite32(0x1, cfg->base + I2C_STAT_RST);
	iowrite32((1 << I2C_EN_i) | (1 << RD_i) | (1 << START_i) | (1 << REPEAT_i) | (1 << STOP_i)
	, cfg->base + I2C_CTRL);


	if (i2c_wait_transaction_timeout(cfg, RX_FULL_ALM, 500) < 0)
		return -2;

	udelay(80);

	while (byte_numb--) {
		//rumboot_printf("XXX\n");
		*((uint8_t *)buf) = ioread8(cfg->base + I2C_RECEAVE);
		rumboot_printf("%x ", ioread8(cfg->base + I2C_RECEAVE));
		(uint8_t *)buf++;
	}
	rumboot_printf("\n");

	return 0;
}
