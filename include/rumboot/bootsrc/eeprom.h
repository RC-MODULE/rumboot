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

#include <rumboot/bootsource.h>

/**
 * [eeprom_init description]
 * @param  data [description]
 * @return      [description]
 */
bool eeprom_init(const struct rumboot_bootsource* src, void* pdata);
/**
 * [eeprom_deinit description]
 * @param data [description]
 */
void eeprom_deinit(void* pdata);
/**
 * [eeprom_read description]
 * @param  data [description]
 * @return      [description]
 */
bool eeprom_read(void* pdata);
/**
 * [eeprom_load_failed_should_i_try_again description]
 * @param  data [description]
 * @return      [description]
 */
bool eeprom_load_failed_should_i_try_again(void* pdata) ;

#endif
