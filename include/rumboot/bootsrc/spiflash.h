#ifndef SPIFLASH__
#define SPIFLASH__

/**
 * \defgroup bootsrc_spiflash SPIFLASH
 * \ingroup bootsrc
 *
 *  SPI function libriary.
 *  Contains functions such as:
 *  - Init SPIFLASH
 *  - Deinit SPIFLASH
 *  - Read data from SPIFLASH
 *
 * \code{.c}
 * #include <rumboot/bootsrc/spiflash.h>
 * \endcode
 *
 * TODO: Insert usage example here
 *
 * \addtogroup bootsrc_spiflash
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
bool spiflash_init(const struct rumboot_bootsource* src, void* pdata);

/**
 * [spi_deinit description]
 * @param data [description]
 */
void spiflash_deinit(void* pdata);

/**
 * [spi_read description]
 * @param  data [description]
 * @return      [description]
 */
bool spiflash_read(void* pdata);

/**
 * [spi_load_failed_should_i_try_again description]
 * @param  data [description]
 * @return      [description]
 */
bool spiflash_load_failed_should_i_try_again(void* pdata);

#endif
