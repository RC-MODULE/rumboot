#include <stdint.h>
#include <stdlib.h>

#include <devices/i2c.h>

#include <regs/regs_i2c.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>

#define I2C_TIMEOUT 1000

void i2c_init(uint32_t base, struct i2c_config *cfg)
{
	//1)
	iowrite32(cfg->scl_freq, base + I2C_CLKPR);

	//Why 0x00070001 ???
	iowrite32(cfg->fifofil, (base + I2C_FIFOFIL));

	iowrite8(cfg->byte_numb, base + I2C_NUMB);
}

void  i2c_enable(uint32_t base)
{
	uint8_t ctrl = ioread8(base + I2C_CR);

	iowrite8(ctrl | (1 << I2C_EN_i), base + I2C_CR);
  iowrite8(0xff, base + I2C_IER);
}

enum i2c_state_cmd i2c_get_state(uint32_t base)
{
	return (enum i2c_state_cmd)((ioread32(base + I2C_SR) && 0x3c00) >> 10);
}

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

static bool wait_tx_fifo_empty(uint32_t base)
{
  int count = 0;
  while (!(ioread32(base + I2C_ISR) & (1 << INT_TRN_EMPTY))) {

  //  rumboot_printf("interrupt status register: %x\n", ioread32(base + I2C_ISR));
    if (count++ == I2C_TIMEOUT) {

      rumboot_printf("Timeout. Was not waiting TX fifo empty.\n");
      return false;
    }
  }

  return true;
}

static bool wait_rx_full_almost(uint32_t base)
{
  int count = 0;
  while ( !(ioread32(base + I2C_SR) & (1 << IBUSY_i)) ) {

  //  rumboot_printf("interrupt status register: %x\n", ioread32(base + I2C_ISR));
    if (count++ == I2C_TIMEOUT) {

      rumboot_printf("Timeout. Was not waiting RX fifo almost empty.\n");
      return false;
    }
  }

  return true;
}

enum err_code {
  OK = 0,
  NADDR_ERROR = -1,
  NACK_ERROR = -2,
  TX_FIFO_NOT_EMPTY_ERROR = -3,
  RX_FIFO_NOT_FULL = -4
};

static enum err_code send_write_cmd(uint32_t base, uint8_t slave_dev, uint16_t offset)
{
  uint8_t offset_h = (offset && 0xff00) >> 8;
  uint8_t offset_l = offset && 0x00ff;

  if (i2c_get_state(base) == ST_IDLE)
    iowrite8(slave_dev + offset, base + I2C_TXBUF);

  if (ioread32(base + I2C_ISR) & (1 << INT_NADDR_i))
    return NADDR_ERROR;

  iowrite8(offset_h, base + I2C_TXBUF);
  iowrite8(offset_l, base + I2C_TXBUF);

  if (ioread32(base + I2C_ISR) & (1 << INT_NACK_i))
    return NACK_ERROR;

  iowrite32(ioread32(base + I2C_CR) | (1 << WR_i) | (1 << START_i), base + I2C_CR);

  if(!wait_tx_fifo_empty(base))
    return TX_FIFO_NOT_EMPTY_ERROR;

  return 0;
}

#if 0
static bool wait_rx_fifo_full(uint32_t base)
{
  int count = I2C_TIMEOUT;

  while( ioread32(base + I2C_SR) & (1<<RX_FULL_i) ) {

    if(count--)
      return RX_FIFO_NOT_FULL;
  };
}
#endif

int i2c_write_byte(uint32_t base, uint8_t slave_dev, uint16_t offset, uint8_t byte)
{
  

  return 0;
}

int i2c_write_data(uint32_t base, uint8_t slave_dev, uint16_t offset, void *buf, size_t number)
{
  if(send_write_cmd(base, slave_dev, offset) < 0)
    return -1;

  while(number--) {
    iowrite8(*( (uint8_t*) buf), base + I2C_TXBUF);
    (uint8_t*) buf++;
  }

  iowrite32(ioread32(base + I2C_CR) | (1 << WR_i) | (1 << START_i) | (1 << REPEAT_i), (base + I2C_CR));
  if(!wait_tx_fifo_empty(base))
    return -2;

  iowrite32(ioread32(base + I2C_CR) | (1 << STOP_i), base + I2C_CR);

  return 0;
}

int i2c_random_read(uint32_t base, uint8_t slave_dev, uint16_t offset, void *buf)
{
  uint8_t byte_numb = ioread8(base + I2C_NUMB);
  rumboot_printf("Byte number: %d\n", byte_numb);

  if(send_write_cmd(base, slave_dev, offset) < 0)
    return -1;

	//LA said what need add 1! Ask her about this
	iowrite8(slave_dev + 1, base + I2C_TXBUF);

	iowrite32(ioread32(base + I2C_CR) | (1 << RD_i) | (1 << START_i) | (1 << REPEAT_i), base + I2C_CR);

	if(!(wait_rx_full_almost(base)))
    return -2;

	while (byte_numb--) {
    *((uint8_t *)buf) = ioread8(base + I2C_RXBUF);
    (uint8_t *) buf++;
    rumboot_printf("data:%x ", ioread8(base + I2C_RXBUF));
  }
  rumboot_printf("\n");

  return 0;
}
