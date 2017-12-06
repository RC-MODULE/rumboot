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
bool eeprom_init(struct bdata* data);
/**
 * [eeprom_deinit description]
 * @param data [description]
 */
void eeprom_deinit(struct bdata* data);
/**
 * [eeprom_read description]
 * @param  data [description]
 * @return      [description]
 */
bool eeprom_read(struct bdata* data);
/**
 * [eeprom_load_failed_should_i_try_again description]
 * @param  data [description]
 * @return      [description]
 */
bool eeprom_load_failed_should_i_try_again(struct bdata* data) ;

#endif
