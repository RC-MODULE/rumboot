#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#define SSP_SYS_FREQ_HZ 500000000
//clock divider
#define CPSDVR  2

typedef enum{
    SSP_INT = (0x1 << 0),
    GSPI_INT = (0x1 << 1),
    end_buf_write = (0x1 << 2),
    end_buf_read = (0x1 << 3),
    err_read = (0x1 << 4),
    err_write = (0x1 << 5),
    overflow_buf_write = (0x1 << 6)
}gspi_dma_interrupt;

typedef enum{
    disable = 0x0,
    transmit = 0x1,
    receive = 0x2,
    all = 0x3
}dma_enable;

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
    uint32_t cpsdvr;
    uint8_t cpol;//spol
    uint8_t cpha;//sph
    ssp_data_size data_size;
    ssp_mode mode;
    bool loopback;
    ssp_frame_format fr_format;
}ssp_params;


//global functions

//ssp functions
void gspi_disable(uint32_t base_address);
void gspi_init(uint32_t base_address, ssp_params params);
uint32_t gspi_get_ris(uint32_t base_address);
uint32_t gspi_get_mis(uint32_t base_address);
uint32_t gspi_get_ssp_status(uint32_t base_addr);
void gspi_set_int_mask(uint32_t base_address, uint16_t mask);
void gspi_dma_enable(uint32_t base_addr, dma_enable enabled);
void gspi_write_data(uint32_t base_addr, uint32_t data);
uint32_t gspi_read_data(uint32_t base_addr);
int gspi_send_word(uint32_t base_address, uint32_t word);
int gspi_get_word(uint32_t base_address, uint32_t * word);

//dma function
void gspi_dma_reset(uint32_t base_addr);
void gspi_dma_set_irq_mask(uint32_t base_addr, gspi_dma_interrupt interrupt);
uint32_t gspi_get_dma_status(uint32_t base_addr);

//eeprom function
void gspi_eeprom_write_enable(uint32_t base_addr);
void gspi_eeprom_erase_init(uint32_t base_addr);
void gspi_eeprom_write_data(uint32_t base_addr, uint32_t mem_addr, uint32_t data);
uint32_t gspi_eeprom_read_data(uint32_t base_addr);

