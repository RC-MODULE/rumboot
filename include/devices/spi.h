#ifndef SPI__
#define SPI__

/**
 * \defgroup devices_spi SPI
 * \ingroup devices
 *
 *  SPI function libriary.
 *  Contains functions such as:
 *  - Init SPI
 *  - Deinit SPI
 *  - Read data from SPI
 *
 * \code{.c}
 * #include <devices/spi.h>
 * \endcode
 *
 * TODO: Insert usage example here
 *
 * \addtogroup devices_spi
 *
 * @{
 */

#include <stdbool.h>

#include <rumboot/bootsource.h>

/**
 * spi_init: init spi
 * src: pointer to rumboot_bootsource object
 * pdata: pointer to configuration data of spi
 */
bool spi_init(const struct rumboot_bootsource* src, struct pdata* pdata);

/**
 * spi_deinit: deinit spi
 * pdata: pointer to configuration data of spi
 */
void spi_deinit(struct pdata* pdata);

/**
 * spi_read: read data from spi
 * pdata: pointer to configuration data of spi
 * return: true or false
 */
bool spi_read(struct pdata* pdata);

/**
 * spi_load_failed_should_i_try_agai: Find another source for load
 * pdata: pointer to configuration data of spi
 * return: true or false
 */
bool spi_load_failed_should_i_try_again(struct pdata* pdata);

#endif
