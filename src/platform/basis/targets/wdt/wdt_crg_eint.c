#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <devices/gpio.h>
#include <rumboot/io.h>
#include <rumboot/timer.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <devices/wdt.h>
#include <devices/crg.h>
#include <rumboot/platform.h>

#include <platform/devices.h>
#include <platform/interrupts.h>


struct wdt_crg_eint_var s = {
     .crg_sys_base        = CRG_SYS_BASE,
     .crg_rst_cfg2        = CRG_RST_GFG2,
     .wdt_int             = WDT_INT,
     .wdt_interval        = WDT_INTERVAL,
     .wdt_unlock_code     = WDT_UNLOCK_CODE,
     .wdt_base            = WDT_BASE,
     .wdt_lock            = WDT_LOCK,
     .wdt_ctrl            = WDT_CTRL,
     .wdt_load            = WDT_LOAD,
     .crg_unlock_code     = CRG_UNLOCK_CODE,
     .crg_wr_lock         = CRG_WR_LOCK,
     .gpio0_base          = GPIO0_BASE,
     .gpio_rd_data        = GPIO_RD_DATA
};


uint32_t main()
{
  int result = wdt_crg_eint(&s);
  
  if (result)
      rumboot_printf("Test OK!\n");
  else
      rumboot_printf("Test FAIL!\n");


  return result ? 0 : 1;

}