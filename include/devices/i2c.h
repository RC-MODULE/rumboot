#ifndef I2C__
#define I2C__

#include <stdbool.h>
#include <stdint.h>

enum device_type {
	EEPROM = 0,
	SENSOR = 1
};

struct i2c_config {

								volatile uint32_t base;
								bool irq_en;
								uint32_t scl_freq;
								size_t byte_numb;
								size_t txfifo_count;
								enum device_type device_type;

								/*PRIVATE DATA*/
								uint32_t irqstat;
};

enum i2c_transaction_type {
	WRITE_DEV,
	WRITE_DATA,
	READ_DATA
};

enum i2c_waited_event {
	TX_EMPTY = 0,
	TX_EMPTY_ALMOST = 1,
	RX_FULL = 2,
	RX_FULL_ALMOST = 3,
	DONE = 4
};

struct i2c_transaction {

	enum i2c_transaction_type type;
	uint8_t devaddr;
	uint16_t offset;
	void *buf;
	int len;
};

/*I2C API*/

/**
 * \brief Init i2c
 * @param [in] cfg configuration of i2c
 */
void i2c_init(struct i2c_config *cfg);

/**
 * \brief Execute i2c transaction
 * @param [in] cfg configuration of i2c
 * @param [in] t parameters of transaction, which include type of transaction!
 * return Return exit code, if value < 0 - error!
 */
int i2c_execute_transaction(struct i2c_config *cfg, struct i2c_transaction *t);

/**
 * \brief Wait i2c transaction
 * @param [in] cfg configuration of i2c
 * @param [in] e waited event, see enum waited_event!
 * return Return exit code, if value < 0 - error!
 */
int i2c_wait_transaction(struct i2c_config *cfg, enum i2c_waited_event e);

/**
* \brief Wait i2c transaction with timeout
* @param [in] cfg configuration of i2c
* @param [in] t parameters of transaction, which include type of transaction!
* @param [in] us  waited time in us
* return Return exit code, if value < 0 - error!
 */
int i2c_wait_transaction_timeout(struct i2c_config *cfg, enum i2c_waited_event e, uint32_t us);

/**
 * \brief Stop i2c transaction
 * @param [in] cfg configuration of i2c
 * return Return exit code, if value < 0 - error!
 */
int i2c_stop_transaction(struct i2c_config *cfg);

/**
 * \brief Handle interrupt request
 * @param [in] irq number of irq in irq table, see platform/interrupts.h!
 * @param [in] arg address of transmission argument
 * return Return nothing!
 */
void i2c_irq_handler(int irq, void *arg);

/* EEPROM FUNCTIONS*/
int eeprom_random_read(struct i2c_config *cfg, uint8_t slave_dev, uint16_t offset, void* buf, size_t number);
int eeprom_random_write(struct i2c_config *cfg, uint8_t slave_dev, uint16_t offset, void* buf, size_t number);


#endif
