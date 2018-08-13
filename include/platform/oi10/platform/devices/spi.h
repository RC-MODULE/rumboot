#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

uint32_t read_eeprom_status (uint32_t base_addr);
void spi_eeprom_write_enable(uint32_t base_addr);
void spi_eeprom_erase_init(uint32_t base_addr);

#define SSP_SYS_FREQ_HZ 500000000
//clock divider
#define CPSDVR  2

typedef enum{
    ssp_data_size_4 = 0x3,
    ssp_data_size_5 = 0x4,
    ssp_data_size_6 = 0x5,
    ssp_data_size_7 = 0x6,
    ssp_data_size_8 = 0x7,
    ssp_data_size_9 = 0x8,
    ssp_data_size_10 = 0x9,
    ssp_data_size_11 = 0xA,
    ssp_data_size_12 = 0xB,
    ssp_data_size_13 = 0xC,
    ssp_data_size_14 = 0xD,
    ssp_data_size_15 = 0xE,
    ssp_data_size_16 = 0xF
}ssp_data_size;

typedef enum{
    master_mode = 0,
    slave_mode = 1
}ssp_mode;

typedef enum{
    ssp_motorola_fr_form = 0x00,
    ssp_ti_fr_form = 0x01,
    ssp_nmw_fr_form = 0x02
}ssp_frame_format;

typedef struct{
    uint32_t clock_rate;
    uint8_t cpol;//spol
    uint8_t cpha;//sph
    ssp_data_size data_size;
    ssp_mode mode;
    bool loopback;
    ssp_frame_format fr_format;
}ssp_params;


//global functions
void ssp_set_int_mask(uint32_t base_address, uint16_t mask);
uint16_t ssp_get_ris(uint32_t base_address);
uint16_t ssp_get_mis(uint32_t base_address);
uint16_t ssp_get_status(uint32_t base_address);
void ssp_dma_enable(uint32_t base_address, bool enabled);
void ssp_disable(uint32_t base_address);
void ssp_init(uint32_t base_address, ssp_params params);
int ssp_send_word(uint32_t base_address, uint16_t word);
int ssp_get_word(uint32_t base_address, uint16_t * word);
