#ifndef I2C__
#define I2C__

#include <stdbool.h>
#include <stdint.h>

enum i2c_state_cmd {

								ST_IDLE = 0b0000,
								ST_START = 0b0001,
								ST_READ = 0b0010,
								ST_WRITE = 0b0011,
								ST_ACK = 0b0100,
								ST_STOP = 0b0101,
};

struct i2c_config {

								uint32_t scl_freq;
								uint8_t byte_numb;
};

void i2c_init(uint32_t base, struct i2c_config *cfg);
void  i2c_enable(uint32_t base);
enum i2c_state_cmd i2c_get_state(uint32_t base);
int i2c_random_read(uint32_t base, uint8_t slave_dev, uint16_t offset, void* buf);
int i2c_write_data(uint32_t base, uint8_t slave_dev, uint16_t offset, void* buf, size_t number);
int i2c_write_byte(uint32_t base, uint8_t slave_dev, uint16_t offset, uint8_t byte);

#endif
