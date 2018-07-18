#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

//SPI ARM SSP registers

//************vars***************

//Interrupt flags
volatile uint32_t SPI_HANDLED;

//************functions*****************

int read_eeprom_status (unsigned spi_apb_base);

int base_test_spi (unsigned spi_apb_base,
                   unsigned gpio_base);
