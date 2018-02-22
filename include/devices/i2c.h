#ifndef I2C__
#define I2C__

#include <stdbool.h>
#include <stdint.h>



struct i2c_config {

								volatile uint32_t base;
								bool irq_en;
								uint32_t scl_freq;
								uint8_t byte_numb;

								/*PRIVATE DATA*/
								uint32_t irqstat;
};

/*I2C API*/
void i2c_init(struct i2c_config *cfg);
int i2c_start_transaction(struct i2c_config *cfg,
        uint8_t devaddr,
        void *txbuf, int txlen,
        void *rxbuf, int rxlen
);

enum waited_event {
	RX_FULL_ALM,
	TX_EMPTY
};

int i2c_check_transaction(struct i2c_config *cfg, enum waited_event e);
int i2c_wait_transaction(struct i2c_config *cfg, enum waited_event e);
int i2c_wait_transaction_timeout(struct i2c_config *cfg, enum waited_event e, uint32_t us);
void i2c_irq_handler(int irq, void *arg);

/* EEPROM FUNCTIONS*/
int eeprom_random_read(struct i2c_config *cfg, uint8_t slave_dev, uint16_t offset, void* buf);
int eeprom_write(struct i2c_config *cfg, uint8_t slave_dev, uint16_t offset, void* buf, size_t number);


#endif
