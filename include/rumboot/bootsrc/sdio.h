#ifndef SDIO__
#define SDIO__

/**
 * \defgroup bootsrc_sdio SDIO
 * \ingroup bootsrc
 *
 *  SDIO function libriary.
 *  Contains functions such as:
 *  - Init SDIO
 *  - Deinit SDIO
 *  - Read data from SDIO
 *
 * \code{.c}
 * #include <rumboot/bootsrc/sdio.h>
 * \endcode
 *
 * TODO: Insert usage example here
 *
 * \addtogroup bootsrc_sdio
 *
 * @{
 */

#include <rumboot/bootsource.h>

/**
 * Card_type Type card enumeration
 */
enum Card_type {

        SDIO_CARD_UNKNOW = 0,
        SDIO_CARD_OLD,
        SDIO_CARD_SDHC,
        SDIO_CARD_SDSC,

};

/**
 * SD card initialization
 * @param  base address of SDIO
 * @param  freq_khz sys clock frequency in KHz
 * @return pointer to configuration structure of sd card
 */
bool sd_init(const struct rumboot_bootsource* src, void* pdata);
/**
 * Read SD card
 * @param  pdata     pointer to private data
 * @return           true if read ok
 */
bool sd_read(void* pdata);

/**
 * [sd_deinit description]
 * @param  base_addr [description]
 * @return           [description]
 */
void sd_deinit(void* pdata);

/**
 * Find another source for load
 * @return [description]
 */
bool sd_are_load_attempts();


#endif
