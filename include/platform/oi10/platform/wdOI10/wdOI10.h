#ifndef __WD_H__
#define __WD_H__

#include <platform/oi10/platform/wdOI10/wdOI10_regs.h>

//uint32_t inline wd_get_value();
//uint32_t inline wd_get_int_status();

void wd_restart(uint32_t lval);
void wd_int_enable();
void wd_rst_enable();
void wd_clear_int();
//itc
void inline wd_itc_enable();
void inline wd_itc_disable();
void wd_itc_set_output(uint32_t test_reset, uint32_t test_int);

#endif //__WD_H__
