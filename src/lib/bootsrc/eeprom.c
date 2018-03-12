#include <rumboot/bootsrc/eeprom.h>
#include <rumboot/bootsource.h>
#include <rumboot/printf.h>

#include <devices/i2c.h>

#define EEPROM_TIMEOUT 500

struct eeprom_private_data {
	const struct rumboot_bootsource *src;
};

const uint32_t get_eeprom_numb()
{
	uint32_t eeprom_numb = 0;

	/*TO DO!*/

	return (const uint32_t)eeprom_numb;
}

bool eeprom_init(const struct rumboot_bootsource *src, void *pdata)
{
	/*TO DO!*/
	struct i2c_config *cfg = pdata;

	i2c_init(cfg);

	return true;
}

void eeprom_deinit(void *pdata)
{
	/*TO DO!*/
}

int eeprom_read(void *pdata, void *dst, void *src)
{
	/*TO DO!*/
	return true;
}

bool eeprom_load_again(void *pdata /*, uint32_t attempts_numb*/)
{
	//Error can be only in loading image step?
	const uint32_t eeprom_numb = get_eeprom_numb();

	//FIX IT. EEPROM DEVICES CAN BE > 2!
	return (eeprom_numb > 1) ? true : false;
}

int eeprom_write(struct i2c_config *cfg, uint8_t slave_dev, uint16_t offset, void *buf, size_t number)
{
	size_t count = 1;
	size_t rem = 0;

	//struct eeprom_private_data pdata;

	struct transaction t = {
		.devaddr	= slave_dev,
		.offset		= offset,
		.buf		= buf
	};

	if (number > 128) {
		count = number / 128;
		rem = number % 128;
		t.len = 128;//Write a semi buffer, bacause full buffer is bugged!
	} else {
		t.len = number;
	}

	while (count--) {

    rumboot_printf("count: %d, rem %d\n", (count + 1), rem);

		if (rem != 0 && count == 0 && number > 128) {
			t.len = rem;
		}

		t.type = WRITE_DEV;
		if (i2c_execute_transaction(cfg, &t) < 0) {
			rumboot_printf("Failed to write device address.\n");
			return -1;
		}

    rumboot_printf("buf address: %x, length: %x\n", t.buf, t.len);
		t.type = WRITE_DATA;

		if (i2c_execute_transaction(cfg, &t) < 0) {
			rumboot_printf("Failed to write data.\n");
			return -2;
		}

		if( i2c_stop_transaction(cfg) < 0 ) {
      rumboot_printf("Error! Didn't stop after write transaction.\n");
      return -3;
    }

    t.buf += 128;
	}


	return 0;
}

int eeprom_random_read(struct i2c_config *cfg, uint8_t slave_dev, uint16_t offset, void *buf, size_t number)
{
	size_t count = 1;
	size_t rem = 0;
	bool have_rem = false;
	bool is_end = false;

	//struct eeprom_private_data pdata;
	struct transaction t = {
		.devaddr	= slave_dev,
		.offset		= offset,
		.buf		= buf,
		.len		= number,
	};

	if (number > 128) {
		count = number / 128;
		rem = number % 128;
		t.len = 128;

		if (rem != 0) {
			have_rem = true;
		}
	}

	while (count--) {
		if (count == 0) {
			is_end = true;
		}

		if (have_rem && is_end) {
			t.len = rem;
		}

    rumboot_printf("count: %d, rem: %d, len: %d\n", (count + 1), rem, t.len);

		t.type = WRITE_DEV;
		if (i2c_execute_transaction(cfg, &t) < 0) {
			rumboot_printf("Failed to write device address.\n");
			return -1;
		}

		t.type = READ_DATA;
		if (i2c_execute_transaction(cfg, &t) < 0) {
			rumboot_printf("Failed to read data.\n");
			return -2;
		}

		i2c_stop_transaction(cfg);

    t.buf += 128;
	}

	return 0;
}
