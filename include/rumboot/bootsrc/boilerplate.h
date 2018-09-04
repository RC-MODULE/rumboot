#ifndef SPIFLASH__
#define SPIFLASH__

/**
 * \defgroup bootsrc_boilerplate Boilerplate boot source
 * \ingroup bootsrc
 *
 *
 * \code{.c}
 * #include <rumboot/bootsrc/boilerplate.h>
 * \endcode
 *
 * Sample boot source. Do not use for production
 *
 * \addtogroup bootsrc_boilerplate
 *
 * @{
 */

#include <stdbool.h>

#include <rumboot/boot.h>
struct boilerplate_private_data {
    int whatever;
};

extern const struct rumboot_bootmodule g_bootmodule_boilerplate;

/**
 * @}
 */

#endif
