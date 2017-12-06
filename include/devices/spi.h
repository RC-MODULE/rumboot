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
 * SPI initialization
 * @param  data [description]
 * @return      [description]
 */
bool spi_init(struct bdata* data);

/**
 * [spi_deinit description]
 * @param data [description]
 */
void spi_deinit(struct bdata* data);

/**
 * [spi_read description]
 * @param  data [description]
 * @return      [description]
 */
bool spi_read(struct bdata* data);

/**
 * [spi_load_failed_should_i_try_again description]
 * @param  data [description]
 * @return      [description]
 */
bool spi_load_failed_should_i_try_again(struct bdata* data);

#endif
