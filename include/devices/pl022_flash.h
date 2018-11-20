#ifndef PL022_FLASH_H
#define PL022_FLASH_H


#include <stdint.h>
#include <rumboot/macros.h>


enum spiflash_command {
    SPIFLASH_PAGEPROG       = 0x02,
    SPIFLASH_READDATA       = 0x03,
    SPIFLASH_READSTAT       = 0x05,
    SPIFLASH_WRITEENABLE    = 0x06,
    SPIFLASH_BLOCK64ERASE   = 0xD8
};


void pl022_flash_write_enable(uint32_t base_addr);
uint32_t pl022_flash_read_status(uint32_t base_addr);
void pl022_flash_erase(uint32_t base_addr);


#endif /* end of include guard: PL022_FLASH_H */
