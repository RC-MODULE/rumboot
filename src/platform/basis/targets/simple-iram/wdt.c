#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <devices/wdt.h>
#include <devices/crg.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <platform/devices.h>
#include <platform/interrupts.h>




static void handler_wdt(int irq, void *arg)
{
    iowrite32(1 << RST_EN | 0 << INT_EN,         WDT_BASE + WDT_CTRL); // WDINT and WDRES enable
    volatile uint32_t *done = arg;
    rumboot_printf("IRQ from WDT arrived, arg %x, count %d\n", arg, (*done));
    *done = *done + 1;
    rumboot_printf("after *done = *done + 1; done = %x\n", *done);
}

static void wdt_init(int to_ctrl, int interval)
{
  iowrite32(WDT_UNLOCK_CODE, WDT_BASE + WDT_LOCK); // Unlock WDT regs for write
  iowrite32(to_ctrl        , WDT_BASE + WDT_CTRL); // WDINT and WDRES enable
  iowrite32(interval       , WDT_BASE + WDT_LOAD); // Cnt interval
}

static void unlock_crg_ddr()
{
    iowrite32(CRG_UNLOCK_CODE, CRG_DDR_BASE + CRG_WR_LOCK); // // Unlock CRG_DDR regs for write
    //iowrite32(0x1ACCE551, CRG_DDR_BASE + CRG_WR_LOCK); // // Unlock CRG_DDR regs for write
}



static void write_to_crg_ddr_fbdiv(int value)
{
    iowrite32(value, CRG_DDR_BASE + CRG_FBDIV);
}

static bool check_crg_ddr_fbdiv_default()
{
    if(ioread32(CRG_DDR_BASE + CRG_FBDIV) == CRG_DDR_FBDIV_DEF)
       return true;
    else    {
        rumboot_printf("CRG_DDR[FBDIV] has wrong default value!");
        rumboot_printf("Test WDT FAIL!\n");
        return false;
    }
}
    



int main()
{
  uint32_t *magic_var = &rumboot_platform_runtime_info.persistent[0];
  
  //WDT_MAGIC check
  if (*magic_var == WDT_MAGIC)
  {
      rumboot_printf("magic_var is equal to WDT_MAGIC\n");
      bool flag = check_crg_ddr_fbdiv_default();
      
      if (flag)
      {
          rumboot_printf("CRG_DDR_FBDIV has correct default value");
          rumboot_printf("Test WDT OK!\n");
          return 0;
      }
      else
      {
          rumboot_printf("CRG_DDR_FBDIV has wrong default value!");
          rumboot_printf("Test WDT FAIL!\n");
          return 1;
      }          
  }
  else {
      if (*magic_var != 0)
      {
          rumboot_printf("Test WDT FAIL! Init value of magic_var isn't 0\n");
          rumboot_printf("Test WDT FAIL!\n");
          return 1;
      }
  }

  rumboot_printf("This is WDT test\n");

  volatile uint32_t done = 0;

  // Disable all interrupts
  rumboot_irq_cli();

  // Create an IRQ table
  struct rumboot_irq_entry *tbl = rumboot_irq_create(NULL);

  // Set handler
  rumboot_irq_set_handler(tbl, WDT_INT, 0, handler_wdt, (void*)&done);

  //write done
  rumboot_printf("done (after Set handler) = %x\n", done);

  // Activate the table
  rumboot_irq_table_activate(tbl);
  rumboot_irq_enable(WDT_INT);

  // Allow interrupt handling
  rumboot_irq_sei();

  //WDT init
  wdt_init(1 << RST_EN | 1 << INT_EN, WDT_INTERVAL);

  //write done
  rumboot_printf("done (after WDT init) = %x\n", done);

  while (done != 1)
  {
      //rumboot_printf("Wait done = 1. Now done is %x\n", done);
  }

  //write done
  rumboot_printf("done (after waiting) = %x\n", done);

  // Deinit interrupt handling
  rumboot_irq_table_activate(NULL);
  rumboot_printf("after activate(NULL)\n");

  // Disable all interrupts
  rumboot_irq_cli();
  rumboot_printf("after rumboot_irq_cli\n");

  rumboot_irq_free(tbl);


  rumboot_printf("Prepare for WDT_MAGIC writing in memory\n");
  *magic_var = WDT_MAGIC;
  rumboot_printf("WDT_MAGIC in memory\n");

  /* Place a perf checkpoint to make perf subsystem happy */
  rumboot_platform_perf("WDT Reset");
  rumboot_printf("WDT init again\n");
  //Unlock CRG_DDR
  unlock_crg_ddr();
  //Write to CRG_DDR[FBDIV] wrong value
  write_to_crg_ddr_fbdiv(0xFFFFFFFF);
  
  //WDT init
  wdt_init(1 << RST_EN | 1 << INT_EN, WDT_INTERVAL);


  //Infinite loop
  while(1){}

}
