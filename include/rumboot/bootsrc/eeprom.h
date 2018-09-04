#ifndef EEPROM__
#define EEPROM__

/**
 * \defgroup bootsrc_eeprom EEPROM
 * \ingroup bootsrc
 *
 *  EEPROM function libriary.
 *  Contains functions such as:
 *  - Init EEPROM
 *  - Deinit EEPROM
 *  - Read data from EEPROM
 *
 * \code{.c}
 * #include <rumboot/bootsrc/eeprom.h>
 * \endcode
 *
 * TODO: Insert usage example here
 *
 * \addtogroup bootsrc_eeprom
 *
 * @{
 */

#include <stdbool.h>

#include <rumboot/boot.h>


const struct rumboot_bootmodule g_bootmodule_eeprom;

#endif
