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
#define PL022_CR0__SCR_i  0
#define PL022_CR0__SPH_i  0
#define PL022_CR0__SPO_i  0
#define PL022_CR0__FRF_i  0
#define PL022_CR0__DSS_i  0
#define PL022_IMSC__TXIM_i  0
#define PL022_IMSC__RXIM_i  0
#define PL022_IMSC__RTIM_i  0
#define PL022_IMSC__RORIM_i 0
#define PL022_ICR__RTIC_i   0
#define PL022_ICR__RORIC_i  0
#define PL022_DMACR__TXDMAE_i   0
#define PL022_DMACR__RXDMAE_i   0
#define PL022_CR1__SOD_i  0
#define PL022_CR1__MS_i   0
#define PL022_CR1__SSE_i  0
#define PL022_CR1__LBM_i  0

/*OFFSETS*/
#define PL022_CPSR        0
#define PL022_CR0__DSS_i 0
#define PL022_CR0 0
#define PL022_IMSC 0x014
#define PL022_ICR 0
#define PL022_CR1 0
#define PL022_DMACR 0



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
