#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <rumboot/testsuite.h>

#include <rumboot/printf.h>
#include <rumboot/io.h>

#include <rumboot/irq.h>
#include <rumboot/timer.h>

#include <platform/devices.h>
#include <rumboot/platform.h>
#include <platform/interrupts.h>
#include <regs/regs_tsns.h>
#include <regs/regs_gpio.h>
volatile unsigned irq_done;
unsigned status;
uint32_t base;

static void handler(int irq, void *arg)
{
	irq_done = 0x1;
	status=ioread32(TSNS_BASE + TSNS_IR);
	if (status!=0x1)  
	rumboot_printf("wrong status!\n"); 
	iowrite32(0xC8, GPIO2_BASE + GPIO_WRITE_APBtoPAD);	//change data
	udelay(100);
	iowrite32(0x0, TSNS_BASE + TSNS_IR);				//inactive interrupt
}

void init_tsns()
{
  iowrite32(0x7CF, TSNS_BASE + TSNS_CLKDIV);
  iowrite32(0xD9F, TSNS_BASE + TSNS_LEVEL);
  iowrite32(0x1, TSNS_BASE + TSNS_IM);//mask
  iowrite32(0x0,   TSNS_BASE + TSNS_PWDN);
  iowrite32(0x1, TSNS_BASE + TSNS_START);
}

int TSNS_INT( uint32_t base_tsns_addr,  uint32_t TSNS_IN)
{
  
	unsigned send_data;
	unsigned count;
	//unsigned count2;
	count=0x0;
	irq_done=0x0;
	
	init_tsns();
	rumboot_irq_enable(TSNS_INTR);
  
	send_data = 42;			// for interrupt data
	
	iowrite32(0xFF, GPIO2_BASE + GPIO_SOURCE);
	iowrite32(0xFF, GPIO2_BASE + GPIO_DIRECTION);
	iowrite32(send_data, GPIO2_BASE + GPIO_WRITE_APBtoPAD);
	udelay(500);
 
  while(irq_done==0x0)
    {
      count= count+1;
      if (count>=1000)
      {
       rumboot_printf("TIMEOUT!\n");
       return 1;
      }
    }	
	
  rumboot_irq_disable(TSNS_INTR);
  return 0; 
}
  

int main()
{
  rumboot_irq_cli();
  struct rumboot_irq_entry *tbl = rumboot_irq_create(NULL);
  
  rumboot_irq_set_handler(tbl, TSNS_INTR, 0, handler, NULL );
  
  rumboot_irq_table_activate(tbl);
  rumboot_irq_sei();
  if (TSNS_INT(TSNS_BASE,TSNS_INTR))
    {
      rumboot_printf("TSNS_INT ERROR!\n");
      return 1;
    }
    rumboot_printf("TEST OK!\n");
  
  return 0;
}