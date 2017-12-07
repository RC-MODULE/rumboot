#include <rumboot/bootsrc/spiflash.h>
#include <platform/spiflash.h>

#include <rumboot/bootsource.h>

bool spiflash_init(const struct rumboot_bootsource* src, void* pdata) {

  /*TO DO!*/
  return true;
}

void spiflash_deinit(void* pdata) {

  /*TO DO!*/
}

bool spiflash_read(void* pdata) {

  /*TO DO!*/
  return true;
}

bool spiflash_load_failed_should_i_try_again(void* pdata) {

  return false;
}
