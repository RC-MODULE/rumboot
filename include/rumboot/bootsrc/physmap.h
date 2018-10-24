#ifndef BOOTSRC_PHYSMAP__
#define BOOTSRC_PHYSMAP__

/**
 * \defgroup bootsrc_physmap physmap boot source
 * \ingroup bootsrc
 *
 *
 * \code{.c}
 * #include <rumboot/bootsrc/physmap.h>
 * \endcode
 *
 * Sample boot source. Do not use for production
 *
 * \addtogroup bootsrc_physmap
 *
 * @{
 */

#include <stdbool.h>

#include <rumboot/boot.h>
struct physmap_private_data {
    int whatever;
};

extern const struct rumboot_bootmodule g_bootmodule_physmap;

/**
 * @}
 */

#endif
