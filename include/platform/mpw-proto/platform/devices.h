#ifndef DEVICES_H
#define DEVICES_H


/**
 * \defgroup platform_native_register_map Register Map
 *
 * \ingroup platform_native
 *
 * \addtogroup platform_native_register_map
 *
 *
 * @{
 *
 * This file contains physical addresses of devices present in SoC.
 * For developers: Please, keep the list sorted by device address
 * and formatted properly.
 *
 * WARNING: DO NOT ADD INDIVIDUAL REGISTER OFFSETS HERE
 */

#define SDIO0_BASE                0x3C06A000
#define SDIO_SIZE             SDIO_PLB6_SIZE

#define SPI_CTRL0__         0x3C03E000
#define SPI_CTRL0_SIZE      SPI_CTRL0_PLB6_SIZE

#define LSIF1_GPIO7__       0x3C067000


#endif
