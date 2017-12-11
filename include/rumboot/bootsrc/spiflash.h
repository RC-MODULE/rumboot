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

/**/
#define PL022_SR__RNE_i 2
#define PL022_SR__TNF_i 1
#define PL022_SR__TFE_i 0
#define PL022_CR1__SOD_i 3
#define PL022_CR1__SSE_i 1


/*Constants*/
#define SPI_READ_CMD_TIMEOUT 500
#define SPI_READ_TIMEOUT 500

/*OFFSETS*/
#define PL022_CPSR  0x10
#define PL022_CR0 0x0
#define PL022_IMSC 0x14
#define PL022_ICR 0x20
#define PL022_CR1 0x4
#define PL022_DMACR 0x24
#define PL022_SR 0xc
#define PL022_DR 0x8

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
bool spiflash_read(void* pdata);

/**
 * [spi_load_failed_should_i_try_again description]
 * @param  data [description]
 * @return      [description]
 */
bool spiflash_load_failed_should_i_try_again(void* pdata);

#endif
