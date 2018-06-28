#ifndef GP_TIMERS_H
#define GP_TIMERS_H

#include <stdint.h>


/**
 *
 * \defgroup GP Timers functions
 * \ingroup devices_gp_timers
 *
 * \code{.c}
 * #include <rumboot/gp_timers.h>
 * \endcode
 *
 *  @{
 */


    /**
     * This function is for checking, that GP_TIMER is present in current project
     * Logicaly, there only one GP_TIMER in project and its address is hardcoded
     *
     * @return pass
     */
    uint32_t gp_timer_present ();

    void gp_timer_turn_on ();

    void gp_timer_fix_value ();

    uint32_t gp_timer_get_free_run_l ();

    uint32_t gp_timer_get_free_run_h ();
    
    void gp_timer_show_free_run ();

/** @}*/

#endif /* end of include guard: GP_TIMERS_H */
