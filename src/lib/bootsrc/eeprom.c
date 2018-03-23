#include <rumboot/bootsrc/eeprom.h>
#include <rumboot/bootsource.h>
#include <rumboot/printf.h>

#include <devices/i2c.h>

struct eeprom_private_data {
	const struct rumboot_bootsource *src;
};

const uint32_t get_eeprom_numb()
{
	uint32_t eeprom_numb = 0;

	/*TO DO!*/

	return (const uint32_t)eeprom_numb;
}

bool eeprom_init(const struct rumboot_bootsource *src, void *pdata)
{
	/*TO DO!*/
	struct i2c_config *cfg = pdata;

	i2c_init(cfg);

	return true;
}

void eeprom_deinit(void *pdata)
{
	/*TO DO!*/
}

int eeprom_read(void *pdata, void *dst, void *src)
{
	/*TO DO!*/
	return true;
}

bool eeprom_load_again(void *pdata /*, uint32_t attempts_numb*/)
{
	//Error can be only in loading image step?
	const uint32_t eeprom_numb = get_eeprom_numb();

	//FIX IT. EEPROM DEVICES CAN BE > 2!
	return (eeprom_numb > 1) ? true : false;
}
