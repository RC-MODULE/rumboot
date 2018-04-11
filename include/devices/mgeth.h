#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

enum mgeth_SPEED {
  SPEED_10 = 0b00,
  SPEED_100 = 0b01,
  SPEED_1000 = 0b10,
};

/**
 * mgeth_conf: : Structure contains configuration parameters
 * is_full_duplex whether duplex full or not
 * speed speed of MGETH
 */
struct mgeth_conf {

  bool is_full_duplex;
  enum mgeth_SPEED speed;
};

/**
 * brief Init MGETH
 * @param base base address of mgeth
 * @param conf
 */
void mgeth_init(const uint32_t base, const struct mgeth_conf *conf);

/**
* brief Transmit data throught MDMA
* base1: base address of MGETH
* base2: base address of MGETH
* src: poiniter to transmitted data
* dst: pointer to received data
* len: size of transmitted data
* return if data was transmitted OK - 0, else - value less than 0
 */
int mgeth_transmit_data_throught_mdma(uint32_t base1, uint32_t base2, volatile void *dest, volatile void *src, size_t len);

/**
 * return if data was transmitted OK - 0, else - value less than 0
 */
int mgeth_init_sgmii();

/**
 * @}
 */
