#include <stdio.h>
#include <string.h>

#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <rumboot/bootheader.h>
#include <rumboot/timer.h>

#include <devices/i2c.h>

#include <platform/devices.h>

int main()
{
	rumboot_printf("I2C test!\n");

  const uint byte_number = 4;
  const uint32_t base = I2C0_BASE;
  const uint8_t slave_dev = 0xA0;
  const uint16_t offset = 0x0;
  struct i2c_config cfg = {
    .scl_freq = 0x13,
    .fifofil = 0x00070001,
    .byte_numb = byte_number
  };
  char in_buf[byte_number];
  char out_buf[byte_number];
  memset(in_buf, 0x55, byte_number);
  memset(out_buf, 0x0, byte_number);

  rumboot_printf("Init i2c.\n");
  i2c_init(base, &cfg);

  rumboot_printf("Enable i2c.\n");
  i2c_enable(base);

  rumboot_printf("Write data throught i2c.\n");
  int ret = i2c_write_data(base, slave_dev, offset, in_buf, byte_number);

  if(ret < 0)
    rumboot_printf("Write failed with error code %i\n", ret);

  udelay(10);

  rumboot_printf("Read data throught i2c.\n");
  ret = i2c_random_read(base, slave_dev, offset, out_buf);

  if(ret < 0)
    rumboot_printf("Read failed with error code %i\n", ret);

  uint count = byte_number;
  while(count--)
    rumboot_printf("%x ", out_buf[byte_number - count]);

  rumboot_printf("\n");

	return 0;
}
