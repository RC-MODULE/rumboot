#ifndef SDIO__
#define SDIO__

/**
 * \defgroup devices_sdio SDIO
 * \ingroup devices
 *
 *  SDIO function libriary.
 *  Contains functions such as:
 *  - Init SDIO
 *  - Deinit SDIO
 *  - Read data from SDIO
 *  - Read data from SDIO n
 *
 * \code{.c}
 * #include <devices/sdio.h>
 * \endcode
 *
 * TODO: Insert usage example here
 *
 * \addtogroup devices_sdio
 *
 * @{
 */

#include <rumboot/boot.h>

/**
 * SD card initialization
 * src: pointer to rumboot_bootsource structure
 * pdata: pointer to private sd configuration data
 * @return true or false
 */
bool sd_init(const struct rumboot_bootsource* src, struct pdata* pdata);

/**
 * sd_read: read data from source
 * pdata: pointer to private sd configuration data
 * return: true if read ok
 */
bool sd_read(struct pdata* pdata);

/**
 * sd_try_read: read data from source n attempts
 * pdata: pointer to private sd configuration data
 * attempts: number of read attempts
 * return: true if read ok, else - false
 */
bool sd_try_read(struct pdata* pdata, const uint32_t attempts);

/**
 * sd_deinit: Deinit sd card
 * pdata: pointer to private sd configuration data
 */
void sd_deinit(struct pdata* pdata);

/**
 * sd_load_failed_should_i_try_again: Find another source for load
 * return: true or false
 */
bool sd_load_failed_should_i_try_again();

#endif
