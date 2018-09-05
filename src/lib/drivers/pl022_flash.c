#include <rumboot/bootsrc/spiflash.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/macros.h>
#include <regs/regs_spi.h>
#include <devices/pl022_flash.h>


void pl022_flash_write_enable(uint32_t base_addr)
{
    iowrite32(0x06, base_addr+GSPI_SSPDR); //write data to SPI - command write enable

    while((ioread32(base_addr+GSPI_SSPSR) & 0x1) == 0) //wait tx fifo empty
        ;

    (void)ioread32(base_addr+GSPI_SSPDR);  //read data from rx fifo
}

static uint32_t read_flash_status (uint32_t base_addr)
{
    iowrite32(0x05, base_addr+GSPI_SSPDR); //write data to SPI - command read status
    iowrite32(0xff, base_addr+GSPI_SSPDR); //write data to SPI - data staff

    while((ioread32(base_addr+GSPI_SSPSR) & 0x1) == 0) //wait tx fifo empty
        ;

    (void)ioread32(base_addr+GSPI_SSPDR); //read data from rx fifo

    return (ioread32(base_addr+GSPI_SSPDR)); //read data from rx fifo
}

void pl022_flash_erase(uint32_t base_addr)
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

    while ((read_flash_status(base_addr) & 0x1) == 0x1) //wait write complete
        ;
}

void pl022_flash_write_data(uint32_t base_addr, uint32_t mem_addr, uint32_t data)
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

    while ((read_flash_status(base_addr) & 0x1) == 0x1) //wait write complete
        ;
}

uint32_t pl022_flash_read_data(uint32_t base_addr)
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
