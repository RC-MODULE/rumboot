#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <rumboot/testsuite.h>

#include <rumboot/printf.h>
#include <rumboot/io.h>

#include <rumboot/irq.h>

#include <platform/devices.h>
#include <rumboot/platform.h>
#include <platform/interrupts.h>
#include <regs/regs_gpio.h>

#define LINES_AMOUNT 8
#define GPIO_AMOUNT  6
#define TIMEOUT 1000
#define DEBUG 1

void GPIO_config (uint32_t base_gpio_addr, uint8_t direction, uint8_t source) {


  iowrite32 (direction, base_gpio_addr + GPIO_DIRECTION); 
  iowrite32 (source, base_gpio_addr + GPIO_SOURCE);
  #ifdef DEBUG
  rumboot_printf ("Direction = %x\n", direction);
  #endif

}

int main()
{
    GPIO_config (GPIO_BASE, 0xff, 0xff);
    while (1) {
      iowrite32 (0xaa, GPIO_BASE + GPIO_WRITE_APBtoPAD);
      iowrite32 (0x55, GPIO_BASE + GPIO_WRITE_APBtoPAD);
    }
    return 0;

}

