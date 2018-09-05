#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#ifndef PL022_FLASH_H
#define PL022_FLASH_H

void pl022_flash_write_enable(uint32_t base_addr);
void pl022_flash_erase(uint32_t base_addr);
void pl022_flash_write_data(uint32_t base_addr, uint32_t mem_addr, uint32_t data);
uint32_t pl022_flash_read_data(uint32_t base_addr);

#endif /* end of include guard: PL022_FLASH_H */
