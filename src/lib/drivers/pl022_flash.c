#include <rumboot/bootsrc/spiflash.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/macros.h>
#include <regs/regs_spi.h>
#include <devices/pl022_flash.h>


void pl022_flash_write_enable(uint32_t const base_addr)
{
    iowrite32(SPIFLASH_WRITEENABLE, base_addr+GSPI_SSPDR); //write data to SPI - command write enable

    while((ioread32(base_addr+GSPI_SSPSR) & 0x1) == 0) //wait tx fifo empty
        ;

    (void)ioread32(base_addr+GSPI_SSPDR);  //read data from rx fifo
}

uint32_t read_flash_status (uint32_t const base_addr)
{
    uint32_t addr = base_addr+GSPI_SSPDR;
    iowrite32(SPIFLASH_READSTAT, addr); //write data to SPI - command read status
    iowrite32(0xff, addr); //write data to SPI - data staff

    while((ioread32(base_addr+GSPI_SSPSR) & 0x1) == 0) //wait tx fifo empty
        ;

    (void)ioread32(addr); //read data from rx fifo

    return (ioread32(addr)); //read data from rx fifo
}

void pl022_flash_erase(uint32_t const base_addr)
{
    uint32_t addr = base_addr+GSPI_SSPDR;

    iowrite8(SPIFLASH_BLOCK64ERASE, addr); //sector erase command
    for (int i =0; i<3; i++)
        iowrite8(0x00, addr);//write data to SPI - write address
    while((ioread32(base_addr+GSPI_SSPSR) & 0x1) == 0) //wait tx fifo empty
        {}
    for (int i =0; i<4; i++)
        (void)ioread8(addr); //read data from SPI - staff
    while ((read_flash_status(base_addr) & 0x1) == 0x1) //wait write complete
        {}
}
