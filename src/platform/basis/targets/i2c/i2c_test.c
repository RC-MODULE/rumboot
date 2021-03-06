#include <stdio.h>
#include <string.h>

#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <rumboot/timer.h>
#include <rumboot/rumboot.h>

#include <devices/i2c.h>

#include <platform/interrupts.h>
#include <platform/devices.h>

int main()
{
	rumboot_printf("I2C0 test!\n");

	const size_t byte_number = 3;
	const uint8_t eeprom_dev = 0xA0;
	const uint16_t offset = 0x0;
	struct i2c_config cfg = {
		.base		= I2C0_BASE,
		.irq_en		= false,
		.scl_freq	= 0x13,
		.byte_numb	= byte_number,
		.device_type = EEPROM

	};
	char *in_buf = rumboot_malloc_from_heap_aligned(0, byte_number, 8);
	char *out_buf = rumboot_malloc_from_heap_aligned(0, byte_number, 8);
	memset(out_buf, 0x0, byte_number);
	size_t i;
	for (i = 0; i < byte_number; i++)
		in_buf[i] = (i <= 255) ? i : (i - 255);
	memset(out_buf, 0x0, byte_number);

	struct rumboot_irq_entry *tbl = rumboot_irq_create(NULL);
	rumboot_irq_set_handler(tbl, I2C0_IRQ, 0, i2c_irq_handler, (void *)&cfg);
	rumboot_irq_enable(I2C0_IRQ);
	rumboot_irq_table_activate(tbl);

	rumboot_printf("Init i2c.\n");
	i2c_init(&cfg);

	rumboot_printf("Write data throught i2c to eeprom.\n");
	int ret = eeprom_random_write(&cfg, eeprom_dev, offset, &in_buf[0], byte_number);

	if (ret < 0) {
		rumboot_printf("Write failed with error code %i\n", ret);
		goto test_failed;
	}

	rumboot_printf("Read eeprom data throught i2c.\n");
	ret = eeprom_random_read(&cfg, eeprom_dev, offset, &out_buf[0], byte_number);

	if (ret < 0) {
		rumboot_printf("Read failed with error code %i\n", ret);
		goto test_failed;
	}

	uint count = byte_number;
	while (count) {
		rumboot_printf("%x, %x  ", out_buf[byte_number - count], in_buf[byte_number - count]);
		count--;
	}

	rumboot_printf("\n");

	if (memcmp(&in_buf[0], &out_buf[0], byte_number) != 0) {
		goto test_failed;
	}

	rumboot_printf("Test OK.\n");
	rumboot_free(in_buf);
	rumboot_free(out_buf);
	return 0;

	test_failed:
		rumboot_free(in_buf);
		rumboot_free(out_buf);
		rumboot_printf("Test failed.\n");
		return -1;
}
