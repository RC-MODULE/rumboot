#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <devices/wdt.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <platform/devices.h> 
#include <platform/interrupts.h>




uint32_t magic_var = 0x00000000;
uint32_t irq_got   = 0x00000000;
uint32_t last_irq  = 0x00000000;

static void handler_wdt(int irq, void *arg)
{
    volatile uint32_t *done = arg;
    rumboot_printf("IRQ from WDT arrived, arg %x, count %d\n", arg, (*done));
    *done = *done + 1;  
}


int main()
{
    
  rumboot_printf("This is WDT test\n");
  
  //WDT_MAGIC check
  if (magic_var == WDT_MAGIC)
  {
      rumboot_printf("magic_var is equal to WDT_MAGIC\n");
      return 0;
  }
  else {
      if (magic_var != 0)
      {
          rumboot_printf("Test WDT FAIL! Init value of magic_var isn't 0\n");
          return 1;
      }
  }
  
  // Disable all interrupts
  rumboot_irq_cli(); 
  
  // Create an IRQ table
  struct rumboot_irq_entry *tbl = rumboot_irq_create(NULL);

  // Set handler
  rumboot_irq_set_handler(tbl, WDT_INT, 0, handler_wdt, (void*)&irq_got);
  
  //write irq_got
  rumboot_printf("irq_got(after Set handler) = %x\n", irq_got);
      
  // Activate the table
  rumboot_irq_table_activate(tbl);
  rumboot_irq_enable(WDT_INT);
  
  // Allow interrupt handling
  rumboot_irq_sei();
  
  //WDT init
  iowrite32(WDT_UNLOCK_CODE          , WDT_BASE + WDT_LOCK); // Unlock WDT regs
  iowrite32(1 << RST_EN | 1 << INT_EN, WDT_BASE + WDT_CTRL); // WDINT and WDRES enable
  iowrite32(WDT_INTERVAL             , WDT_BASE + WDT_LOAD); // Cnt interval
  
  //write irq_got
  rumboot_printf("irq_got(after WDT init) = %x\n", irq_got);
  
  while (irq_got != 1){}
  
  // Deinit
  rumboot_irq_table_activate(NULL);
  rumboot_irq_free(tbl);
  
  magic_var = WDT_MAGIC;
  rumboot_printf("WDT_MAGIC in memory\n");
  
  //Infinite loop
  while(1){}
  
  
   
  //rumboot_printf( "Test WDT OK!\n");

  //return 0;
}   





