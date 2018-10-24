#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <regs/regs_gpio_rcm.h>
#include <rumboot/io.h>
#include <rumboot/timer.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <devices/wdt.h>
#include <devices/crg.h>
#include <rumboot/platform.h>
#include <platform/devices.h>
#include <platform/interrupts.h>

/*void handler_wdt(int irq, void *arg)
{
    iowrite32(1 << RST_EN | 0 << INT_EN,         WDT_BASE + WDT_CTRL); // WDINT and WDRES enable
    volatile uint32_t *done = arg;
    rumboot_printf("IRQ from WDT arrived, arg %x, count %d\n", arg, (*done));
    *done = *done + 1;
    rumboot_printf("after *done = *done + 1; done = %x\n", *done);
}*/

void handler_wdt(int irq, void *arg)
{
    iowrite32(0x00000002,         0x0108F000 + 0x8); // WDINT and WDRES enable
    volatile uint32_t *done = arg;
    rumboot_printf("IRQ from WDT arrived, arg %x, count %d\n", arg, (*done));
    *done = *done + 1;
    rumboot_printf("after *done = *done + 1; done = %x\n", *done);
}

void write_to_reg(int value, int addr_base, int addr_shift)
{
    iowrite32(value, addr_base + addr_shift);
}

void wdt_init(int wdt_interval, int wdt_unlock_code, int wdt_base, int wdt_lock, int wdt_ctrl, int wdt_load)
{
  write_to_reg(wdt_unlock_code, wdt_base, wdt_lock); // Unlock WDT regs for write
  write_to_reg(0x3            , wdt_base, wdt_ctrl); // WDINT and WDRES enable
  write_to_reg(wdt_interval   , wdt_base, wdt_load); // Cnt interval
}

bool check_crg_ddr_fbdiv_default(int crg_ddr_base, int crg_fbdiv, int crg_ddr_fbdiv_def)
{
    if(ioread32(crg_ddr_base + crg_fbdiv) == crg_ddr_fbdiv_def)
       return true;
    else    {
        rumboot_printf("CRG_DDR[FBDIV] has wrong default value!");
        rumboot_printf("Test WDT FAIL!\n");
        return false;
    }
}


//Test   WDT CRG internal interrupt
int wdt_crg_iint(struct wdt_crg_iint_var *conf)

{
  uint32_t *magic_var = &rumboot_platform_runtime_info->persistent[0];

  //WDT_MAGIC check
  if (*magic_var == conf->wdt_magic)
  {
      rumboot_printf("magic_var is equal to WDT_MAGIC\n");
      bool flag = check_crg_ddr_fbdiv_default(conf->crg_ddr_base, conf->crg_fbdiv, conf->crg_ddr_fbdiv_def);

      if (flag)
      {
          rumboot_printf("CRG_DDR_FBDIV has correct default value");
          rumboot_printf("Test WDT OK!\n");
          return 1;
      }
      else
      {
          rumboot_printf("CRG_DDR_FBDIV has wrong default value!");
          rumboot_printf("Test WDT FAIL!\n");
          return 0;
      }
  }
  else {
      if (*magic_var != 0)
      {
          rumboot_printf("Test WDT FAIL! Init value of magic_var isn't 0\n");
          rumboot_printf("Test WDT FAIL!\n");
          return 0;
      }
  }

  rumboot_printf("This is WDT test\n");

  volatile uint32_t done = 0;

  // Disable all interrupts
  rumboot_irq_cli();

  // Create an IRQ table
  struct rumboot_irq_entry *tbl = rumboot_irq_create(NULL);

  // Set handler
  rumboot_irq_set_handler(tbl, conf->wdt_int, 0, handler_wdt, (void*)&done);

  //write done
  rumboot_printf("done (after Set handler) = %x\n", done);

  // Activate the table
  rumboot_irq_table_activate(tbl);
  rumboot_irq_enable(conf->wdt_int);

  // Allow interrupt handling
  rumboot_irq_sei();

  //WDT init
  wdt_init(conf->wdt_interval, conf->wdt_unlock_code, conf->wdt_base, conf->wdt_lock, conf->wdt_ctrl, conf->wdt_load);

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
  *magic_var = conf->wdt_magic;
  rumboot_printf("WDT_MAGIC in memory\n");


  rumboot_platform_perf("WDT Reset");
  rumboot_printf("WDT init again\n");
  //Unlock CRG_DDR
  write_to_reg(conf->crg_unlock_code, conf->crg_ddr_base, conf->crg_wr_lock);
  //Write to CRG_DDR[FBDIV] wrong value
  write_to_reg(0xFFFFFFFF, conf->crg_ddr_base, conf->crg_fbdiv);

  //WDT init
  wdt_init(conf->wdt_interval, conf->wdt_unlock_code, conf->wdt_base, conf->wdt_lock, conf->wdt_ctrl, conf->wdt_load);


  //Infinite loop
  while(1){}

}

//Test   WDT CRG external interrupt
int wdt_crg_eint(struct wdt_crg_eint_var *conf)

{
    //Unlock CRG_SYS
    write_to_reg(conf->crg_unlock_code, conf->crg_sys_base, conf->crg_wr_lock);
    //Mask rst_req from WDT
    write_to_reg(0x00000000, conf->crg_sys_base, conf->crg_rst_cfg2);

    //Check low state for GPIO0
    int rd_data = ioread32(conf->gpio0_base+conf->gpio_rd_data);
    if ((rd_data & 0x00000040) != 0x00000000)
    {
        rumboot_printf("GPIO0_6 is n't 0! It's wrong value!");
        rumboot_printf("Test WDT FAIL!\n");
        return 0;
    }

    //WDT start
    wdt_init(conf->wdt_interval, conf->wdt_unlock_code, conf->wdt_base, conf->wdt_lock, conf->wdt_ctrl, conf->wdt_load);

    //Wait high state in GPIO0
    rd_data = ioread32(conf->gpio0_base+conf->gpio_rd_data);
    while (!(rd_data & 0x00000040))
     {rd_data = ioread32(conf->gpio0_base+conf->gpio_rd_data);}
    rumboot_printf("GPIO0_6 is 1! It indicates external output WDT_INT!\n");
    return 1;


}
