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


enum Data_size {
  _4_BIT = 0,
  _5_BIT,
  _6_BIT,
  _7_BIT,
  _8_BIT,
  _9_BIT,
  _10_BIT,
  _11_BIT,
  _12_BIT,
  _13_BIT,
  _14_BIT,
  _15_BIT,
  _16_BIT,
};

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
bool spiflash_read(uint32_t* to, uint32_t* from, void* pdata);

/**
 * [spi_load_failed_should_i_try_again description]
 * @param  data [description]
 * @return      [description]
 */
bool spiflash_load_again(void* pdata);

#endif
