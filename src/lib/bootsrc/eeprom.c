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
	//struct eeprom_private_data pdata;
	struct transaction t = {
		.devaddr	= slave_dev,
		.offset		= offset,
		.buf		= buf,
		.len		= number,
		.type		= WRITE_DEV
	};

	if( i2c_execute_transaction(cfg, &t) < 0) {
    rumboot_printf("Failed to write device address.\n");
    return -1;
  }

	t.type = WRITE_DATA;
	if( i2c_execute_transaction(cfg, &t) < 0) {
    rumboot_printf("Failed to write data.\n");
    return -2;
  }

	// t.type = WRITE_DATA_CMD;
	// i2c_execute_transaction(cfg, &t);

	i2c_stop_transaction(cfg);

	return 0;
}

int eeprom_random_read(struct i2c_config *cfg, uint8_t slave_dev, uint16_t offset, void *buf, size_t number)
{
	struct transaction t = {
		.devaddr	= slave_dev,
		.offset		= offset,
		.buf		= buf,
		.len		= number,
		.type		= WRITE_DEV
	};

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

	return 0;
}
