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

#include <rumboot/boot.h>


extern const struct rumboot_bootmodule g_bootmodule_spiflash;

/**
 * @}
 */

#endif
