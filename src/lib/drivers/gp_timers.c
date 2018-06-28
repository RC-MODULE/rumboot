
//-----------------------------------------------------------------------------
//  This file contains GP_TIMERS related functions 
//  It includes next functions:
//    - gp_timer_present
//    - 
//    - 
//    - 
//    - 
//    - 
//    - 
//    - 
//    - 
//
//-----------------------------------------------------------------------------
#include <stdint.h>

#include <rumboot/io.h>
#include <rumboot/printf.h>

#include <platform/devices.h>

#include <regs/regs_gp_timers.h>

#include <devices/gp_timers.h>

uint32_t gp_timer_present ()
{
    if (ioread32 (GLOBAL_TIMERS + ID) != 0x54475355)
        return -1;
    return 0;
}

void gp_timer_turn_on ()
{
    iowrite32 (0x1, GLOBAL_TIMERS + ENABLE);
}

void gp_timer_fix_value ()
{
    iowrite32 (0x1, GLOBAL_TIMERS + FIX_CMD);
}

uint32_t gp_timer_get_free_run_l ()
{
    return ioread32 (GLOBAL_TIMERS + FREE_RUN_L);
}

uint32_t gp_timer_get_free_run_h ()
{
    return ioread32 (GLOBAL_TIMERS + FREE_RUN_H);
}

void gp_timer_show_free_run ()
{
    gp_timer_fix_value ();
    rumboot_printf("free run timer value: %x.%x\n", gp_timer_get_free_run_h (), gp_timer_get_free_run_l ());
}


