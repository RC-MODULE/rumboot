#include <stdint.h>
#include <stdlib.h>

#include <platform/oi10/platform/devices/spi.h>
#include <platform/oi10/platform/regs/regs_spi.h>
#include <platform/oi10/platform/common_macros/common_macros.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/timer.h>

int read_eeprom_status (unsigned spi_apb_base)
{
  int tmp;
  iowrite32(0xd0, GPI0_BASE + GPI0_ADDR_MASK);    //start operation
  iowrite32(0x05, spi_apb_base + SSPDR); //write data to SPI - command read status
  iowrite32(0xff, spi_apb_base + SSPDR); //write data to SPI - data staff
  while((ioread32(spi_apb_base + SSPSR) & 0x1) == 0)    {;} //wait tx fifo empty
  iowrite32(0xf0, GPI0_BASE + GPI0_ADDR_MASK);    //stop operation
  tmp = ioread32(spi_apb_base + SSPDR); //read data from rx fifo
  return (ioread32(spi_apb_base + SSPDR)); //read data from rx fifo
}
