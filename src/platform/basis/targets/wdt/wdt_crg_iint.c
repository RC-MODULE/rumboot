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
#include <rumboot/bootheader.h>
#include <platform/devices.h>
#include <platform/interrupts.h>


struct wdt_crg_iint_var s = {
     .wdt_magic           = WDT_MAGIC,
     .crg_ddr_base        = CRG_DDR_BASE,
     .crg_fbdiv           = CRG_FBDIV,
     .crg_ddr_fbdiv_def   = CRG_DDR_FBDIV_DEF,
     .wdt_int             = WDT_INT,
     .wdt_interval        = WDT_INTERVAL,
     .wdt_unlock_code     = WDT_UNLOCK_CODE,
     .wdt_base            = WDT_BASE,
     .wdt_lock            = WDT_LOCK,
     .wdt_ctrl            = WDT_CTRL,
     .wdt_load            = WDT_LOAD,
     .crg_unlock_code     = CRG_UNLOCK_CODE,
     .crg_wr_lock         = CRG_WR_LOCK
};


uint32_t main()
{
  int result = wdt_crg_iint(&s);
  
  if (result)
      rumboot_printf("Test OK!\n");
  else
      rumboot_printf("Test FAIL!\n");


  return result ? 0 : 1;

}