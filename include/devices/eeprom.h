#ifndef EEPROM__
#define EEPROM__

/**
 * \defgroup devices_eeprom EEPROM
 * \ingroup devices
 *
 *  EEPROM function libriary.
 *  Contains functions such as:
 *  - Init EEPROM
 *  - Deinit EEPROM
 *  - Read data from EEPROM
 *
 * \code{.c}
 * #include <devices/eeprom.h>
 * \endcode
 *
 * TODO: Insert usage example here
 *
 * \addtogroup devices_eeprom
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
bool eeprom_init(const struct rumboot_bootsource* src, struct pdata* pdata);
/**
 * [eeprom_deinit description]
 * @param data [description]
 */
void eeprom_deinit(struct pdata* pdata);
/**
 * [eeprom_read description]
 * @param  data [description]
 * @return      [description]
 */
bool eeprom_read(struct pdata* pdata);
/**
 * [eeprom_load_failed_should_i_try_again description]
 * @param  data [description]
 * @return      [description]
 */
bool eeprom_load_failed_should_i_try_again(struct pdata* pdata) ;

#endif
