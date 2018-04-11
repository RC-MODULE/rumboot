#ifndef I2C__
#define I2C__

#include <stdbool.h>
#include <stdint.h>

/**
 * \defgroup devices_i2c I2C
 * \ingroup devices
 *
 *  I2C function libriary.
 *  Contains functions such as:
 *  - Init i2c
 *  - Execute i2c transaction
 *  - Wait i2c transaction
 *  - Wait i2c transaction with timeout
 *  - Stop i2c transaction
 *  - IRQ handler
 *

 *
 *  Usage example:
 *
 *
 * \addtogroup devices_i2c
 *
 * @{
 */

enum device_type {
	EEPROM = 0,
	SENSOR = 1
};

/**
 * i2c_config: Structure contains i2c configuration parameters
 * base: base address of I2C
 * irq_en: enable IRQ or not
 * scl_freq: frequency of scl
 * byte_numb: number of transmitted byte
 * txfifo_count: count of byte in TX FIFO. (For avoid event when we have overflow buffer!)
 * device_type: type of device
 * irq_stat: private data, which contains state of IRQ
 */
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

/**
 * i2c_transaction: Structure contains i2c transaction parameters
 * type: type of transaction
 * devaddr: address of device
 * offset: address of offset in certain device
 * buf: pointer to buffer, which we can use for read/write data
 * len: length of data
 */
struct i2c_transaction {

	enum i2c_transaction_type type;
	uint8_t devaddr;
	uint16_t offset;
	void *buf;
	int len;
};

/*I2C API*/

/**
 * brief: Init i2c
 * cfg: configuration of i2c
 */
void i2c_init(struct i2c_config *cfg);

/**
 * brief: Execute i2c transaction
 * cfg: configuration of i2c
 * t: parameters of transaction, which include type of transaction!
 * return Return exit code, if value < 0 - error!
 */
int i2c_execute_transaction(struct i2c_config *cfg, struct i2c_transaction *t);

/**
 * brief Wait i2c transaction
 * cfg: configuration of i2c
 * e: waited event, see enum waited_event!
 * return Return exit code, if value < 0 - error!
 */
int i2c_wait_transaction(struct i2c_config *cfg, enum i2c_waited_event e);

/**
* brief: Wait i2c transaction with timeout
* cfg: configuration of i2c
* t: parameters of transaction, which include type of transaction!
* us:  waited time in us
* return Return exit code, if value < 0 - error!
 */
int i2c_wait_transaction_timeout(struct i2c_config *cfg, enum i2c_waited_event e, uint32_t us);

/**
 * brief: Stop i2c transaction
 * cfg: configuration of i2c
 * return Return exit code, if value < 0 - error!
 */
int i2c_stop_transaction(struct i2c_config *cfg);

/**
 * brief: Handle interrupt request
 * irq: number of irq in irq table, see platform/interrupts.h!
 * arg: address of transmission argument
 * return Return nothing!
 */
void i2c_irq_handler(int irq, void *arg);

/* EEPROM FUNCTIONS*/
/**
 * brief: Read data from EEPROM
 * cfg: configuration of i2c
 * slave_dev: address of slave device
 * offset: address of offset in slave device
 * buf: pointer to buffer where we must store read data
 * number: length of data
 * @return Return exit code, if value < 0 - error!
 */
int eeprom_random_read(struct i2c_config *cfg, uint8_t slave_dev, uint16_t offset, void* buf, size_t number);

/**
 * brief: Write data to EEPROM
 * cfg: configuration of i2c
 * slave_dev: address of slave device
 * offset: address of offset in slave device
 * buf: pointer to buffer where we must load written data
 * number: length of data
 * @return Return exit code, if value < 0 - error!
 */
int eeprom_random_write(struct i2c_config *cfg, uint8_t slave_dev, uint16_t offset, void* buf, size_t number);


/**
 * @}
 */

#endif
