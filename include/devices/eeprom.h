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
 * eeprom_init Init EEPROM
 * src: pointer to rumboot_bootsource structure
 * pdata: pointer to private eeprom configuration data
 * return: false or true
 */
bool eeprom_init(const struct rumboot_bootsource* src, struct pdata* pdata);

/**
 * eeprom_deinit Deinit EEPROM
 * pdata: pointer to private eeprom configuration data
 */
void eeprom_deinit(struct pdata* pdata);
/**
 * eeprom_read Read from EEPROM
 * pdata: pointer to private eeprom configuration data
 * @return id read ok - true, else - false
 */
bool eeprom_read(struct pdata* pdata);
/**
 * eeprom_load_failed_should_i_try_again: Find another source for load
 * pdata: pointer to private eeprom configuration data
 * return: true or false
 */
bool eeprom_load_failed_should_i_try_again(struct pdata* pdata) ;

#endif
