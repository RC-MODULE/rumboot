#ifndef I2C__
#define I2C__

#include <stdbool.h>
#include <stdint.h>


struct i2c_config {

								volatile uint32_t base;
								bool irq_en;
								uint32_t scl_freq;
								size_t byte_numb;

								/*PRIVATE DATA*/
								uint32_t irqstat;
};

enum transaction_type {
	WRITE_DEV,
	WRITE_DATA,
	READ_DATA
};

enum waited_event {
	TX_EMPTY = 0,
	TX_EMPTY_ALMOST = 1,
	RX_FULL = 2,
	RX_FULL_ALMOST = 3
};

struct transaction {

	enum transaction_type type;
	uint8_t devaddr;
	uint16_t offset;
	void *buf;
	int len;
};

/*I2C API*/
void i2c_init(struct i2c_config *cfg);
int i2c_execute_transaction(struct i2c_config *cfg, struct transaction *t);
int i2c_wait_transaction(struct i2c_config *cfg, enum waited_event e);
int i2c_wait_transaction_timeout(struct i2c_config *cfg, enum waited_event e, uint32_t us);
int i2c_stop_transaction(struct i2c_config *cfg);
void i2c_irq_handler(int irq, void *arg);

/* EEPROM FUNCTIONS*/
int eeprom_random_read(struct i2c_config *cfg, uint8_t slave_dev, uint16_t offset, void* buf, size_t number);
int eeprom_write(struct i2c_config *cfg, uint8_t slave_dev, uint16_t offset, void* buf, size_t number);


#endif
