#ifndef WDT
#define WDT




#define WDT_UNLOCK_CODE     0x1ACCE551
#define WDT_INTERVAL        0x00000400
#define WDT_MAGIC           0x8A61C0DE

#define WDT_LOAD            0x000
#define WDT_VAL             0x004
#define WDT_CTRL            0x008
#define WDT_INT_CLR         0x00C
#define WDT_LOCK            0xC00

#define INT_EN              0
#define RST_EN              1

#include <stdbool.h>

struct wdt_crg_iint_var{
    int wdt_magic           ;
    int crg_ddr_base        ;
    int crg_fbdiv           ;
    int crg_ddr_fbdiv_def   ;
    int wdt_int             ;
    int wdt_interval        ;
    int wdt_unlock_code     ;
    int wdt_base            ;
    int wdt_lock            ;
    int wdt_ctrl            ;
    int wdt_load            ;
    int crg_unlock_code     ;
    int crg_wr_lock         ;
};

struct wdt_crg_eint_var{
    int crg_sys_base      ;
    int crg_rst_cfg2      ;
    int wdt_int           ;
    int wdt_interval      ;
    int wdt_unlock_code   ;
    int wdt_base          ;
    int wdt_lock          ;
    int wdt_ctrl          ;
    int wdt_load          ;
    int crg_unlock_code   ;
    int crg_wr_lock       ;
    int gpio0_base        ;
    int gpio_rd_data      ;
}; 


 int wdt_crg_iint(struct wdt_crg_iint_var *conf);
 int wdt_crg_eint(struct wdt_crg_eint_var *conf);


#endif
