#include <rumboot/bootsrc/spiflash.h>
#include <platform/spiflash.h>

#include <rumboot/bootsource.h>
#include <rumboot/io.h>

struct spiflash_private_data {
    const struct rumboot_bootsource* src;

};

bool spiflash_init(const struct rumboot_bootsource* src, void* pdata) {

  struct spiflash_private_data* spi_flash = (struct spiflash_private_data*) pdata;

  /*FixMe: This should be Platform-specific ! */
  iowrite32(8, src->base + PL022_CPSR); //100MHz/8 = 12.5MHz
  iowrite32((0b0111 << PL022_CR0__DSS_i), src->base + PL022_CR0);
  iowrite32(0, src->base + PL022_IMSC);
  iowrite32((0b1 << PL022_ICR__RTIC_i) | (0b1 << PL022_ICR__RORIC_i), src->base + PL022_ICR);
  iowrite32(0, src->base + PL022_DMACR);
  iowrite32((1 << PL022_CR1__SOD_i) | (1 << PL022_CR1__SSE_i), src->base + PL022_CR1);//enable PL022 SSP

  spi_flash->src = src;
  /*TO DO!*/
  return true;
}

void spiflash_deinit(void* pdata) {

  /*TO DO!*/
  struct spiflash_private_data* spiflash_pdata = (struct spiflash_private_data*) pdata;
  const struct rumboot_bootsource* src = spiflash_pdata->src;

  iowrite32(ioread32(src->base + PL022_CR1) & ~(0b1 << PL022_CR1__SSE_i), src->base + PL022_CR1); //disable PL022 SSP
}

bool spiflash_read(void* pdata) {

  /*TO DO!*/
  return true;
}

bool spiflash_load_failed_should_i_try_again(void* pdata) {

  return false;
}
