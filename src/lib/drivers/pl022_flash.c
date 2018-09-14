#include <rumboot/bootsrc/spiflash.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/macros.h>
#include <regs/regs_spi.h>
#include <devices/pl022_flash.h>
#include <platform/common_macros/common_macros.h>
#include <platform/trace.h>

void pl022_flash_write_enable(uint32_t base_addr)
{
    iowrite32(0x06, base_addr+GSPI_SSPDR); //write data to SPI - command write enable

    while((ioread32(base_addr+GSPI_SSPSR) & 0x1) == 0) //wait tx fifo empty
        ;

    (void)ioread32(base_addr+GSPI_SSPDR);  //read data from rx fifo
}

static uint32_t read_flash_status (uint32_t base_addr)
{
    uint32_t addr = base_addr+GSPI_SSPDR;
    iowrite32(0x05, addr); //write data to SPI - command read status
    iowrite32(0xff, addr); //write data to SPI - data staff

    while((ioread32(base_addr+GSPI_SSPSR) & 0x1) == 0) //wait tx fifo empty
        ;

    (void)ioread32(addr); //read data from rx fifo

    return (ioread32(addr)); //read data from rx fifo
}

void pl022_flash_erase(uint32_t base_addr)
{
    uint32_t addr = base_addr+GSPI_SSPDR;
    iowrite32(0xD8, addr); //sector erase command
    iowrite32(0x00, addr); //write data to SPI - write address
    iowrite32(0x00, addr); //write data to SPI - write address
    iowrite32(0x00, addr); //write data to SPI - write address

    while((ioread32(base_addr+GSPI_SSPSR) & 0x1) == 0) //wait tx fifo empty
        ;

    (void)ioread32(addr); //read data from rx fifo
    (void)ioread32(addr); //read data from rx fifo
    (void)ioread32(addr); //read data from rx fifo
    (void)ioread32(addr); //read data from rx fifo

    while ((read_flash_status(base_addr) & 0x1) == 0x1) //wait write complete
        ;
}

void pl022_flash_write_data(uint32_t const base_addr, uint8_t const * const data, size_t const size)
{
    uint32_t addr =  base_addr+GSPI_SSPDR;
    iowrite8(0x02, addr); //write data to SPI - command write
    iowrite8(0x00, addr); //write data to SPI - write address
    iowrite8(0x00, addr); //write data to SPI - write address
    iowrite8(0x00, addr); //write data to SPI - write address

    for(size_t i = 0; i < size; i++)
    {
        iowrite8(data[i], addr); //write data to SPI - write data
    }
    while((ioread8(base_addr+GSPI_SSPSR) & 0x1) == 0) //wait tx fifo empty
        ;

    for(size_t i =0; i<(size+4); i++)
    {
        (void)ioread8(addr); //read data from rx fifo
    }

    while ((read_flash_status(base_addr) & 0x1) == 0x1) //wait write complete
        ;
}

void __attribute__((optimize("-O3"))) pl022_flash_read_data(uint32_t const base_addr, uint8_t* const data, size_t const size)
{
    uint32_t addr =  base_addr+GSPI_SSPDR;
    iowrite8(0x03, addr); //write data to SPI - command read
    iowrite8(0x00, addr); //write data to SPI - read address
    iowrite8(0x00, addr); //write data to SPI - read address
    iowrite8(0x00, addr); //write data to SPI - read address

    for(size_t i =0; i < size; i++)
    {
        iowrite8(0xff, addr); //write data to SPI - staff
    }

    while((ioread8(base_addr+GSPI_SSPSR) & 0x1) == 0) //wait tx fifo empty
        ;

    (void)ioread8(addr); //read data from rx fifo
    (void)ioread8(addr); //read data from rx fifo
    (void)ioread8(addr); //read data from rx fifo
    (void)ioread8(addr); //read data from rx fifo

    for(size_t i =0; i < size; i++)
    {
        data[i] = ioread8(addr);
    }
}

