#include <devices/spi.h>
#include <platform/spi.h>

#include <rumboot/bootsource.h>

bool spi_init(const struct rumboot_bootsource* src, struct pdata* pdata) {

  /*TO DO!*/
  return true;
}

void spi_deinit(struct pdata* pdata) {

  /*TO DO!*/
}

bool spi_read(struct pdata* pdata) {

  /*TO DO!*/
  return true;
}

bool spi_load_failed_should_i_try_again(struct pdata* pdata) {

  return false;
}
