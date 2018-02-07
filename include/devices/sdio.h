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

#include <rumboot/bootsource.h>

/**
 * SD card initialization
 * @param  base address of SDIO
 * @param  freq_in_mhz sys clock frequency in MHz
 * @return pointer to configuration structure of sd card
 */
bool sd_init(const struct rumboot_bootsource* src, struct pdata* pdata);
/**
 * [sd_read description]
 * @param  base      base address of SDIO
 * @param  src_addr  [description]
 * @param  dest_addr [description]
 * @return           true if read ok
 */
bool sd_read(struct pdata* pdata);
/**
 * [sd_try_read description]
 * @param  base      base address of SDIO
 * @param  src_addr  [description]
 * @param  dest_addr [description]
 * @param  attempts  number of read attempts
 * @return           true if read ok
 */
bool sd_try_read(struct pdata* pdata, const uint32_t attempts);

/**
 * [sd_deinit description]
 * @param  base_addr [description]
 * @return           [description]
 */
void sd_deinit(struct pdata* pdata);

/**
 * Find another source for load
 * @return [description]
 */
bool sd_load_failed_should_i_try_again();

#endif
