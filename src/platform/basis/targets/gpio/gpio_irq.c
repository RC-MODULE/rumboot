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
volatile unsigned irq_done;
unsigned status;

static void handler1()
{
  irq_done = 0x1;
  status=ioread32(GPIO1_BASE+0x14);
  if ((status!=0x1)&&(status!=0x2))
  rumboot_printf("wrong status!\n");
}
static void handler2()
{
  irq_done = 0x1;
  status=ioread32(GPIO2_BASE+0x14);
  if ((status!=0x1)&&(status!=0x2))
  rumboot_printf("wrong status!\n");
}
static void handler3()
{
  irq_done = 0x1;
  status=ioread32(GPIO3_BASE+0x14);
  if ((status!=0x1)&&(status!=0x2))
  rumboot_printf("wrong status!\n");
}
static void handler4()
{
  irq_done = 0x1;
  status=ioread32(GPIO4_BASE+0x14);
  if ((status!=0x1)&&(status!=0x2))
  rumboot_printf("wrong status!\n");
}
static void handler5()
{
  irq_done = 0x1;
  status=ioread32(GPIO5_BASE+0x14);
  if ((status!=0x1)&&(status!=0x2))
  rumboot_printf("wrong status!\n");
}
static void handler6()
{
  irq_done = 0x1;
  status=ioread32(GPIO6_BASE+0x14);
  if ((status!=0x1)&&(status!=0x2))
  rumboot_printf("wrong status!\n");
}

int GPIO_INT( uint32_t base_gpio_addr,  uint32_t GPIO_INTR )
{

  unsigned apb_to_pad;
  unsigned count;
  count=0x0;
  irq_done=0x0;
  iowrite32(0xff, base_gpio_addr + GPIO_SOURCE);
  iowrite32(0xf0, base_gpio_addr + GPIO_DIRECTION);

  rumboot_irq_enable(GPIO_INTR);

  iowrite32(0x3, base_gpio_addr + 0x18);//mask

  apb_to_pad = ioread32(base_gpio_addr + GPIO_WRITE_APBtoPAD);
  apb_to_pad = apb_to_pad^0x10;
  iowrite32(apb_to_pad,base_gpio_addr + GPIO_WRITE_APBtoPAD);
  while(irq_done==0x0)
    {
      count= count+1;
      if (count>=1000)
      {
       rumboot_printf("TIMEOUT!\n");
       return 1;
      }
    }
  rumboot_irq_disable(GPIO_INTR);
  return 0;
}


int main()
{
  rumboot_irq_cli();
  struct rumboot_irq_entry *tbl = rumboot_irq_create(NULL);

  rumboot_irq_set_handler(tbl, GPIO1_INTR, 0, handler1, NULL );
  rumboot_irq_set_handler(tbl, GPIO2_INTR, 0, handler2, NULL );
  rumboot_irq_set_handler(tbl, GPIO3_INTR, 0, handler3, NULL );
  rumboot_irq_set_handler(tbl, GPIO4_INTR, 0, handler4, NULL );
  rumboot_irq_set_handler(tbl, MDIO_GPIO0_INT, 0, handler5, NULL );
  rumboot_irq_set_handler(tbl, MDIO_GPIO1_INT, 0, handler6, NULL );
  /* Activate the table */
  rumboot_irq_table_activate(tbl);
  rumboot_irq_sei();
  if (GPIO_INT(GPIO1_BASE,GPIO1_INTR))
    {
      rumboot_printf("GPIO1_INT ERROR!\n");
      return 1;
    }
  if (GPIO_INT(GPIO2_BASE,GPIO2_INTR))
    {
      rumboot_printf("GPIO2_INT ERROR!\n");
      return 1;
    }
  if (GPIO_INT(GPIO3_BASE,GPIO3_INTR))
    {
      rumboot_printf("GPIO3_INT ERROR!\n");
      return 1;
    }
  if (GPIO_INT(GPIO4_BASE,GPIO4_INTR))
    {
      rumboot_printf("GPIO4_INT ERROR!\n");
      return 1;
    }

   if (GPIO_INT(GPIO5_BASE,MDIO_GPIO0_INT))
    {
      rumboot_printf("GPIO5_INT ERROR!\n");
      return 1;
    }
   if (GPIO_INT(GPIO6_BASE,MDIO_GPIO1_INT))
    {
      rumboot_printf("GPIO6_INT ERROR!\n");
      return 1;
    }

    rumboot_printf("TEST OK!\n");

  return 0;
}
