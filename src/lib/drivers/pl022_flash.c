#include <rumboot/bootsrc/spiflash.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/macros.h>
#include <regs/regs_spi.h>
#include <devices/pl022_flash.h>
#include <platform/common_macros/common_macros.h>
#include <platform/trace.h>

#define SPIFLASH_PAGEPROG      0x02
#define SPIFLASH_READDATA      0x03
#define SPIFLASH_READSTAT      0x05
#define SPIFLASH_WRITEENABLE   0x06
#define SPIFLASH_BLOCK64ERASE  0xD8

void pl022_flash_write_enable(uint32_t base_addr)
{
    iowrite32(SPIFLASH_WRITEENABLE, base_addr+GSPI_SSPDR); //write data to SPI - command write enable

    while((ioread32(base_addr+GSPI_SSPSR) & 0x1) == 0) //wait tx fifo empty
        ;

    (void)ioread32(base_addr+GSPI_SSPDR);  //read data from rx fifo
}

static uint32_t read_flash_status (uint32_t base_addr)
{
    uint32_t addr = base_addr+GSPI_SSPDR;
    iowrite32(SPIFLASH_READSTAT, addr); //write data to SPI - command read status
    iowrite32(0xff, addr); //write data to SPI - data staff

    while((ioread32(base_addr+GSPI_SSPSR) & 0x1) == 0) //wait tx fifo empty
        ;

    (void)ioread32(addr); //read data from rx fifo

    return (ioread32(addr)); //read data from rx fifo
}
#define WRITE_ADDR_SPI(i, dummy)  iowrite8(0x00, addr);//write data to SPI - write address
#define READ_DATA_STAFF(i, dummy)  (void)ioread8(addr); //read data from SPI - staff
#define WRITE_ADDR_SPI(i, dummy)  iowrite8(0x00, addr);//write data to SPI - write address
#define WRITE_DATA_STAFF(i, dummy)  iowrite8(0xFF, addr);//write data to SPI - write address

void pl022_flash_erase(uint32_t base_addr)
{
    uint32_t addr = base_addr+GSPI_SSPDR;

    iowrite8(SPIFLASH_BLOCK64ERASE, addr); //sector erase command
    REPEAT(3, WRITE_ADDR_SPI, dummy)
    while((ioread32(base_addr+GSPI_SSPSR) & 0x1) == 0) //wait tx fifo empty
        {}
    REPEAT(4, READ_DATA_STAFF, dummy)
    while ((read_flash_status(base_addr) & 0x1) == 0x1) //wait write complete
        {}
}

void pl022_flash_write_data(uint32_t const base_addr, uint32_t data)
{
    uint32_t addr =  base_addr+GSPI_SSPDR;
    iowrite8(SPIFLASH_PAGEPROG, addr); //write data to SPI - command write
    REPEAT(3, WRITE_ADDR_SPI, dummy)

    iowrite8((data & 0x000000FF), base_addr+GSPI_SSPDR); //write data to SPI - write data 1
    iowrite8((data & 0x0000FF00) >> 8, base_addr+GSPI_SSPDR); //write data to SPI - write data 2
    iowrite8((data & 0x00FF0000) >> 16, base_addr+GSPI_SSPDR); //write data to SPI - write data 3
    iowrite8((data & 0xFF000000) >> 24, base_addr+GSPI_SSPDR); //write data to SPI - write data 4

    while((ioread8(base_addr+GSPI_SSPSR) & 0x1) == 0) //wait tx fifo empty
        {}
    REPEAT(8, READ_DATA_STAFF, dummy)
    while ((read_flash_status(base_addr) & 0x1) == 0x1) //wait write complete
        {}
}

uint32_t pl022_flash_read_data(uint32_t const base_addr)
{
    uint32_t data = 0x00000000;
    uint32_t addr =  base_addr+GSPI_SSPDR;

    iowrite8(SPIFLASH_READDATA, addr); //write data to SPI - command read
    REPEAT(3, WRITE_ADDR_SPI, dummy)

    REPEAT(4, WRITE_DATA_STAFF, dummy)
    while((ioread8(base_addr+GSPI_SSPSR) & 0x1) == 0) //wait tx fifo empty
        {}
    REPEAT(4, READ_DATA_STAFF, dummy)

    data = ioread8(addr);
    data = data | (ioread8(addr) << 8);
    data = data | (ioread8(addr) << 16);
    data = data | (ioread8(addr) << 24);
    return data;
}

