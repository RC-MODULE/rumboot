#include <stdint.h>
#include <stdlib.h>

#include <platform/oi10/platform/devices/spi.h>
#include <platform/oi10/platform/regs/regs_spi.h>
#include <platform/oi10/platform/common_macros/common_macros.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/timer.h>

void gspi_dma_reset(uint32_t base_addr)
{
    iowrite32(0x01, base_addr + GSPI_SOFTRST); //reset DMA

    while (ioread32(base_addr + GSPI_SOFTRST))
        ;
}

void gspi_dma_enable(uint32_t base_addr, dma_enable enabled)
{
    iowrite32(enabled, base_addr + GSPI_SSPDMACR);
}

void gspi_dma_set_irq_mask(uint32_t base_addr, gspi_dma_interrupt interrupt)
{
    iowrite32(interrupt, base_addr + GSPI_IRQMASKS); //set irq masks
}

void gspi_write_data(uint32_t base_addr, uint32_t data)
{
    iowrite32(data, base_addr+GSPI_SSPDR);
}

uint32_t gspi_read_data(uint32_t base_addr)
{
    return (ioread32(base_addr + GSPI_SSPDR));
}
uint32_t gspi_get_ris(uint32_t base_address)
{
    return ioread32(base_address + GSPI_SSPRIS);
}

uint32_t gspi_get_mis(uint32_t base_address)
{
    return ioread32(base_address + GSPI_SSPMIS);
}

uint32_t gspi_get_ssp_status(uint32_t base_addr)
{
    return (ioread32(base_addr + GSPI_SSPSR));
}

uint32_t gspi_get_dma_status(uint32_t base_addr)
{
    return (ioread32(base_addr + GSPI_STATUS));
}

void gspi_set_int_mask(uint32_t base_address, uint16_t mask)
{
    iowrite32(mask, base_address + GSPI_SSPIMSC);
}

int gspi_send_word(uint32_t base_address, uint32_t word)
{
    //check transmit FIFO full
    if(ioread32(base_address + GSPI_SSPSR) & TNF) // transmit FIFO is not full
    {
        iowrite32(word, base_address + GSPI_SSPDR);
        return 1;
    } else
    {
        rumboot_printf("Warning: writing to SSP TFIFO when TFIFO is full\n");
        return 0;
    }
}

int gspi_get_word(uint32_t base_address, uint32_t * word)
{
    //check receive FIFO empty
    if(ioread32(base_address + GSPI_SSPSR) & RNE) //receive FIFO is not empty
    {
        *word = ioread32(base_address + GSPI_SSPDR);
        return 1;
    } else
    {
        rumboot_printf("Warning: reading from SSP RFIFO when RFIFO is empty\n");
        return 0;
    }
}


void gspi_set_clock_rate(uint32_t base_address, uint32_t cpsdvr)
{
    //NOTE: CR0 should be zero
    iowrite32(cpsdvr, base_address + GSPI_SSPCPSR); //set divider
//    iowrite32(CPSDVR, base_address + GSPI_SSPCPSR); //set divider
//    int scr = (SSP_SYS_FREQ_HZ / (clk_rate * 2)) - 1;
//    iowrite32(scr << SCR_SHIFT, base_address + GSPI_SSPCR0); //set SCR
}

void gspi_disable(uint32_t base_address)
{
    iowrite32(SSE, base_address + GSPI_SSPCR1); //clear enable bit
//    SSP_clear_SSP_CR1(base_address + GSPI_SSPCR1, SSE);//clear enable bit
}



void gspi_init(uint32_t base_address, ssp_params params)
{
    //disable ssp
    iowrite32(0x00, base_address + GSPI_SSPCR1);
    iowrite32(0x0000, base_address + GSPI_SSPCR0);
    //setup the parameters
    //clock rate
    gspi_set_clock_rate(base_address,params.cpsdvr);
    //cpol, cpha, format
    iowrite32((((params.cpha & 0x1) << SPH_SHIFT) |
                ((params.cpol & 0x1) << SPO_SHIFT) |
                (params.fr_format << FRF_SHIFT) |
                (params.data_size << DSS_SHIFT)),
                base_address + GSPI_SSPCR0);
    //mode, loopback, enable core
    iowrite32( ( ((params.loopback & 0x1) << LBM_SHIFT) | (params.mode << MS_SHIFT) | SSE ), base_address + GSPI_SSPCR1);
}


void gspi_eeprom_write_enable(uint32_t base_addr)
{
    iowrite32(0x06, base_addr+GSPI_SSPDR); //write data to SPI - command write enable

    while((ioread32(base_addr+GSPI_SSPSR) & 0x1) == 0) //wait tx fifo empty
        ;

    (void)ioread32(base_addr+GSPI_SSPDR);  //read data from rx fifo
}

static uint32_t read_eeprom_status (uint32_t base_addr)
{
    iowrite32(0x05, base_addr+GSPI_SSPDR); //write data to SPI - command read status
    iowrite32(0xff, base_addr+GSPI_SSPDR); //write data to SPI - data staff

    while((ioread32(base_addr+GSPI_SSPSR) & 0x1) == 0) //wait tx fifo empty
        ;

    (void)ioread32(base_addr+GSPI_SSPDR); //read data from rx fifo

    return (ioread32(base_addr+GSPI_SSPDR)); //read data from rx fifo
}

void gspi_eeprom_erase_init(uint32_t base_addr)
{
    iowrite32(0xD8, base_addr+GSPI_SSPDR); //sector erase command
    iowrite32(0x00, base_addr+GSPI_SSPDR); //write data to SPI - write address
    iowrite32(0x00, base_addr+GSPI_SSPDR); //write data to SPI - write address
    iowrite32(0x00, base_addr+GSPI_SSPDR); //write data to SPI - write address

    while((ioread32(base_addr+GSPI_SSPSR) & 0x1) == 0) //wait tx fifo empty
        ;

    (void)ioread32(base_addr+GSPI_SSPDR); //read data from rx fifo
    (void)ioread32(base_addr+GSPI_SSPDR); //read data from rx fifo
    (void)ioread32(base_addr+GSPI_SSPDR); //read data from rx fifo
    (void)ioread32(base_addr+GSPI_SSPDR); //read data from rx fifo

    while ((read_eeprom_status(base_addr) & 0x1) == 0x1) //wait write complete
        ;
}

void gspi_eeprom_write_data(uint32_t base_addr, uint32_t mem_addr, uint32_t data)
{
    iowrite32(0x02, base_addr+GSPI_SSPDR); //write data to SPI - command write
    iowrite32((mem_addr & 0x000000FF), base_addr+GSPI_SSPDR); //write data to SPI - write address
    iowrite32((mem_addr & 0x0000FF00) >> 8, base_addr+GSPI_SSPDR); //write data to SPI - write address
    iowrite32((mem_addr & 0x00FF0000) >> 16, base_addr+GSPI_SSPDR); //write data to SPI - write address
    iowrite32((data & 0x000000FF), base_addr+GSPI_SSPDR); //write data to SPI - write data 1
    iowrite32((data & 0x0000FF00) >> 8, base_addr+GSPI_SSPDR); //write data to SPI - write data 2
    iowrite32((data & 0x00FF0000) >> 16, base_addr+GSPI_SSPDR); //write data to SPI - write data 3
    iowrite32((data & 0xFF000000) >> 24, base_addr+GSPI_SSPDR); //write data to SPI - write data 4

    while((ioread32(base_addr+GSPI_SSPSR) & 0x1) == 0) //wait tx fifo empty
        ;

    (void)ioread32(base_addr+GSPI_SSPDR); //read data from rx fifo
    (void)ioread32(base_addr+GSPI_SSPDR); //read data from rx fifo
    (void)ioread32(base_addr+GSPI_SSPDR); //read data from rx fifo
    (void)ioread32(base_addr+GSPI_SSPDR); //read data from rx fifo
    (void)ioread32(base_addr+GSPI_SSPDR); //read data from rx fifo
    (void)ioread32(base_addr+GSPI_SSPDR); //read data from rx fifo
    (void)ioread32(base_addr+GSPI_SSPDR); //read data from rx fifo
    (void)ioread32(base_addr+GSPI_SSPDR); //read data from rx fifo

    while ((read_eeprom_status(base_addr) & 0x1) == 0x1) //wait write complete
        ;
}

uint32_t gspi_eeprom_read_data(uint32_t base_addr)
{
        uint32_t data;
        iowrite32(0x03, base_addr+GSPI_SSPDR); //write data to SPI - command read
        iowrite32(0x00, base_addr+GSPI_SSPDR); //write data to SPI - read address
        iowrite32(0x00, base_addr+GSPI_SSPDR); //write data to SPI - read address
        iowrite32(0x00, base_addr+GSPI_SSPDR); //write data to SPI - read address
        iowrite32(0xff, base_addr+GSPI_SSPDR); //write data to SPI - staff
        iowrite32(0xff, base_addr+GSPI_SSPDR); //write data to SPI - staff
        iowrite32(0xff, base_addr+GSPI_SSPDR); //write data to SPI - staff
        iowrite32(0xff, base_addr+GSPI_SSPDR); //write data to SPI - staff

        while((ioread32(base_addr+GSPI_SSPSR) & 0x1) == 0) //wait tx fifo empty
            ;

        (void)ioread32(base_addr+GSPI_SSPDR); //read data from rx fifo
        (void)ioread32(base_addr+GSPI_SSPDR); //read data from rx fifo
        (void)ioread32(base_addr+GSPI_SSPDR); //read data from rx fifo
        (void)ioread32(base_addr+GSPI_SSPDR); //read data from rx fifo

        data = ioread32(base_addr+GSPI_SSPDR);
        data = data | (ioread32(base_addr+GSPI_SSPDR) << 8);
        data = data | (ioread32(base_addr+GSPI_SSPDR) << 16);
        data = data | (ioread32(base_addr+GSPI_SSPDR) << 24);
    return data;
}


