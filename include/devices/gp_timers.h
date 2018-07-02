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
     * This function is for checking, that GP_TIMER is present in current project.
     * It reads GP_TIMER ID and check its value
     * Logicaly, there only one GP_TIMER in project and its address is hardcoded
     *
     * @return pass
     */
    uint32_t gp_timer_present ();

    /**
     * This function turns On GP_TIMER.
     *  Free run counter starts to work.
     *  Global counter starts to work.
     *  Synchronisation signals that goes to periph blocks toggle.
     *  No events are generated by default.
     */
    void gp_timer_turn_on ();

    /**
     * Fix current free run counter and system counter values.
     */
    void gp_timer_fix_value ();

    /**
     * Return Return free run timer low part
     */
    uint32_t gp_timer_get_free_run_l ();

    /**
     * Return Return free run timer high part
     */
    uint32_t gp_timer_get_free_run_h ();
    
    /**
     * Fix and display free run timer value
     */
    void gp_timer_show_free_run ();

/** @}*/

#endif /* end of include guard: GP_TIMERS_H */
