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


/*struct mystr { 
    int a,
    int b,
    int c
};

struct mystr s = {
    .a = 1,
    .b = 2
};*/




uint32_t main()
{
  //int result = wdt_crg_iint(&s);
  int result = wdt_crg_iint(WDT_MAGIC,
                 CRG_DDR_BASE,
                 CRG_FBDIV,
                 CRG_DDR_FBDIV_DEF,
                 WDT_INT,
                 WDT_INTERVAL,
                 WDT_UNLOCK_CODE,
                 WDT_BASE,
                 WDT_LOCK,
                 WDT_CTRL,
                 WDT_LOAD,
                 CRG_UNLOCK_CODE,
                 CRG_WR_LOCK) ;

  if (result)
      rumboot_printf("Test OK!\n");
  else
      rumboot_printf("Test FAIL!\n");


  return result ? 0 : 1;

}


/*int crg_init(struct mystr *conf)
{
    conf->a = 1;
}*/