#include <devices/eeprom.h>
#include <platform/eeprom.h>

#include <rumboot/bootsource.h>

const uint32_t get_eeprom_numb() {

  uint32_t eeprom_numb = 0;

  /*TO DO!*/

  return (const uint32_t) eeprom_numb;
}

bool eeprom_init(struct bdata* data) {

  /*TO DO!*/

  return true;
}

void eeprom_deinit(struct bdata* data) {

  /*TO DO!*/
}

bool eeprom_read(struct bdata* data) {

  /*TO DO!*/
  return true;
}

bool eeprom_load_failed_should_i_try_again(struct bdata* data/*, uint32_t attempts_numb*/) {

  //Error can be only in loading image step?

  const uint32_t eeprom_numb = get_eeprom_numb();

  //FIX IT. EEPROM DEVICES CAN BE > 2!
  return (eeprom_numb > 1) ? true : false;
}
