#include <rumboot/bootsrc/eeprom.h>
#include <rumboot/bootsource.h>

struct eeprom_private_data {
    const struct rumboot_bootsource* src;

};

const uint32_t get_eeprom_numb() {

  uint32_t eeprom_numb = 0;

  /*TO DO!*/

  return (const uint32_t) eeprom_numb;
}

bool eeprom_init(const struct rumboot_bootsource* src, void* pdata) {

  /*TO DO!*/

  return true;
}

void eeprom_deinit(void* pdata) {

  /*TO DO!*/
}

bool eeprom_read(void* pdata) {

  /*TO DO!*/
  return true;
}

bool eeprom_are_load_attempts(void* pdata/*, uint32_t attempts_numb*/) {

  //Error can be only in loading image step?

  const uint32_t eeprom_numb = get_eeprom_numb();

  //FIX IT. EEPROM DEVICES CAN BE > 2!
  return (eeprom_numb > 1) ? true : false;
}
