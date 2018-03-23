#include <stdio.h>

#include <rumboot/printf.h>

#include <devices/i2c.h>

#define EEPROM_TIMEOUT 500
#define EEPROM_PAGE_SIZE 128

int eeprom_chunk_write(struct i2c_config *cfg, uint8_t slave_dev, uint16_t offset, void *buf, size_t len)
{
	//struct eeprom_private_data pdata;
	struct i2c_transaction t = {
		.devaddr	= slave_dev,
		.offset		= offset,
		.buf		= buf,
		.len		= len,
	};

	t.type = WRITE_DEV;
	if (i2c_execute_transaction(cfg, &t) < 0) {
		rumboot_printf("Failed to write device address.\n");
		return -1;
	}

	t.type = WRITE_DATA;
	if (i2c_execute_transaction(cfg, &t) < 0) {
		rumboot_printf("Failed to read data.\n");
		return -2;
	}

	i2c_stop_transaction(cfg);

	return 0;
}

int eeprom_random_write(struct i2c_config *cfg, uint8_t slave_dev, uint16_t offset, void *buf, size_t len)
{
	size_t count = len / EEPROM_PAGE_SIZE;
	size_t rem = len % EEPROM_PAGE_SIZE;
	uint8_t * ptr = (uint8_t *) buf;

	while (count) {

		rumboot_printf("write to: %x device, offset: %x\n", slave_dev, offset);

		if (eeprom_chunk_write(cfg, slave_dev, offset, &ptr[EEPROM_PAGE_SIZE * (len / EEPROM_PAGE_SIZE - count)], EEPROM_PAGE_SIZE) < 0) {
			return -1;
		}

		offset += EEPROM_PAGE_SIZE;
		count--;
	}

	rumboot_printf("write to: %x device, offset: %x\n", slave_dev, offset);

	if (eeprom_chunk_write(cfg, slave_dev, offset, &ptr[EEPROM_PAGE_SIZE * count], rem) < 0) {
		return -2;
	}

	return 0;
}

int eeprom_chunk_read(struct i2c_config *cfg, uint8_t slave_dev, uint16_t offset, void *buf, size_t len)
{
	if (len > EEPROM_PAGE_SIZE) {
		return -3;
	}

	//struct eeprom_private_data pdata;
	struct i2c_transaction t = {
		.devaddr	= slave_dev,
		.offset		= offset,
		.buf		= buf,
		.len		= len,
	};

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

	//i2c_stop_transaction(cfg);

	return 0;
}

int eeprom_random_read(struct i2c_config *cfg, uint8_t slave_dev, uint16_t offset, void *buf, size_t len)
{
	size_t count = len / EEPROM_PAGE_SIZE;
	size_t rem = len % EEPROM_PAGE_SIZE;
	uint8_t * ptr = (uint8_t *) buf;

	while (count) {

		rumboot_printf("read from: %x device, offset: %x\n", slave_dev, offset);

		if (eeprom_chunk_read(cfg, slave_dev, offset, &ptr[EEPROM_PAGE_SIZE * (len / EEPROM_PAGE_SIZE - count)], EEPROM_PAGE_SIZE) < 0) {
			return -1;
		}

		offset += EEPROM_PAGE_SIZE;
		count --;
	}

	rumboot_printf("read from: %x device, offset: %x\n", slave_dev, offset);

	if (eeprom_chunk_read(cfg, slave_dev, offset, &ptr[EEPROM_PAGE_SIZE * count], rem) < 0) {
		return -2;
	}

	return 0;
}
