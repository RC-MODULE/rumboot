#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <platform/common_macros/common_macros.h>
#include <platform/test_event_codes.h>
#include <platform/devices.h>
#include <platform/oi10/platform/regs/regs_spi.h>
#include <platform/oi10/platform/devices/spi.h>

#include <rumboot/irq.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <platform/interrupts.h>
#include <platform/test_assert.h>
#include <platform/regs/fields/mpic128.h>

#define CHECK_REGS

//Interrupt flags

int base_test_spi (unsigned spi_apb_base,
                   unsigned gpio_base)
{
//  //Congigure gpio
//  MEM(gpio_base+0x420) = 0xff; //Switch GPIO to functional mode
//
//  MEM(S_GPIO_BASE+0x420) = 0x00; //Switch GPIO1 to gpio mode
//  MEM(S_GPIO_BASE+0x400) = 0xff; //set GPIO1[7:4] direction output
//  MEM(S_GPIO_BASE+0x3fc) = 0xf0; //write data to output
//
//  //Init mpic and set interrupts handler
//  mpic_reset();
//  mpic_pass_through_disable();
//  mpic_setup_ext_interrupt(100,MPIC_PRIOR_1,int_sense_level,int_pol_pos,Processor0);
//  mpic_setup_ext_interrupt(73,MPIC_PRIOR_2,int_sense_level,int_pol_pos,Processor0);
//  SET_NONCR_INT_HANDLER(non_cr_interrupt_handler);
//  ppc_noncr_int_enable();
//  mpic_set_interrupt_borders(10, 5);
//  mpic_set_minimal_interrupt_priority_mask(Processor0, MPIC_PRIOR_0);
//  //Reset interrupt flags
//  SPI_HANDLED = false;
//
//  //Read and check ID
//  unsigned ssp_id;
//  ssp_id =  (MEM(spi_apb_base+0xffc)&0x000000ff)      | ((MEM(spi_apb_base+0xff8)<<8)&0x0000ff00) |
//           ((MEM(spi_apb_base+0xff4)<<16)&0x00ff0000) | ((MEM(spi_apb_base+0xff0)<<24)&0xff000000);
//  if (ssp_id != 0x0df005b1) {
//    trace_msg("ID read error! ID from SSP: ");
//    trace_hex(ssp_id);
//    return 1;
//  }
//
//  //Check SPI interrupt with loop operation
//  MEM(spi_apb_base+0x000) = 0xc7; //set 8-bit data
//  MEM(spi_apb_base+0x004) = 0x03; //turn on SSP controller, loop operation
//  MEM(spi_apb_base+0x010) = 0x02; //clock prescale
//  MEM(spi_apb_base+0x014) = 0x02; //interrupt masks - unmask rx_fifo not empty
//  MEM(spi_apb_base+0x008) = 0x5a; //write data to SPI with IRQ
//  if (!wait_spi_int_handled(1000)) {trace_msg("SPI interrupt timeout\n"); return 1;}
//  if (MEM(spi_apb_base+0x8) != 0x5a) {trace_msg("SPI loop read error\n"); return 1;} //Read and check data
//
//  //Write data to spi eeprom
//  int tmp;
//  MEM(spi_apb_base+0x004) = 0x2; //SSP controller from loop to normal operation
//
//  MEM(S_GPIO_BASE+0x3fc) = 0xd0;    //start operation
//  MEM(spi_apb_base+0x008) = 0x06; //write data to SPI - command write enable
//  while((MEM(spi_apb_base+0x00c) & 0x1) == 0){;} //wait tx fifo empty
//  MEM(S_GPIO_BASE+0x3fc) = 0xf0;    //stop operation
//  tmp = MEM(spi_apb_base+0x008); //read data from rx fifo
//
//  MEM(S_GPIO_BASE+0x3fc) = 0xd0;    //start operation
//  MEM(spi_apb_base+0x008) = 0x02; //write data to SPI - command write
//  MEM(spi_apb_base+0x008) = 0x00; //write data to SPI - write address
//  MEM(spi_apb_base+0x008) = 0x12; //write data to SPI - write data 1
//  MEM(spi_apb_base+0x008) = 0x34; //write data to SPI - write data 2
//  MEM(spi_apb_base+0x008) = 0xfe; //write data to SPI - write data 3
//  MEM(spi_apb_base+0x008) = 0xdc; //write data to SPI - write data 4
//  while((MEM(spi_apb_base+0x00c) & 0x1) == 0){;} //wait tx fifo empty
//  MEM(S_GPIO_BASE+0x3fc) = 0xf0;    //stop operation
//  tmp = MEM(spi_apb_base+0x008); //read data from rx fifo
//  tmp = MEM(spi_apb_base+0x008); //read data from rx fifo
//  tmp = MEM(spi_apb_base+0x008); //read data from rx fifo
//  tmp = MEM(spi_apb_base+0x008); //read data from rx fifo
//  tmp = MEM(spi_apb_base+0x008); //read data from rx fifo
//  tmp = MEM(spi_apb_base+0x008); //read data from rx fifo
//
//  while ((read_eeprom_status(spi_apb_base) & 0x1) == 0x1) {;} //wait write complete
//
//  //Read and check data from spi slash
//  MEM(S_GPIO_BASE+0x3fc) = 0xd0;    //start operation
//  MEM(spi_apb_base+0x008) = 0x03; //write data to SPI - command read
//  MEM(spi_apb_base+0x008) = 0x00; //write data to SPI - read address
//  MEM(spi_apb_base+0x008) = 0xff; //write data to SPI - staff
//  MEM(spi_apb_base+0x008) = 0xff; //write data to SPI - staff
//  MEM(spi_apb_base+0x008) = 0xff; //write data to SPI - staff
//  MEM(spi_apb_base+0x008) = 0xff; //write data to SPI - staff
//  while((MEM(spi_apb_base+0x00c) & 0x1) == 0){;} //wait tx fifo empty
//  MEM(S_GPIO_BASE+0x3fc) = 0xf0;    //stop operation
//  tmp = MEM(spi_apb_base+0x008); //read data from rx fifo
//  tmp = MEM(spi_apb_base+0x008); //read data from rx fifo
//  if (MEM(spi_apb_base+0x008) != 0x12){trace_msg("SPI data read fail\n"); return 1;}
//  if (MEM(spi_apb_base+0x008) != 0x34){trace_msg("SPI data read fail\n"); return 1;}
//  if (MEM(spi_apb_base+0x008) != 0xfe){trace_msg("SPI data read fail\n"); return 1;}
//  if (MEM(spi_apb_base+0x008) != 0xdc){trace_msg("SPI data read fail\n"); return 1;}

  return 0;
}

int main(void)
{
    uint32_t result = 0;

    rumboot_printf("Check GSPI0\n");
    #ifdef
    if( base_test_spi(GSPI0_BASE))
    {
        rumboot_printf("Check GSPI0: OK\n");
        return 0;
    } else
    {
        rumboot_printf("Check GSPI0: ERROR\n");
        return result << 1;
    }

    rumboot_printf("Check GSPI1\n");
    if( base_test_spi(GSPI1_BASE))
    {
        rumboot_printf("Check GSPI1: OK\n");
        return 0;
    } else
    {
        rumboot_printf("Check GSPI1: ERROR\n");
        return result << 1;
    }
}
