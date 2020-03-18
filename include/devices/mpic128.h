/*
 * mpic128.h
 *
 *  Created on: Jul 9, 2018
 *      Author: r.galiulin
 */

#ifndef PLATFORM_DEVICES_MPIC128_H
#define PLATFORM_DEVICES_MPIC128_H


#include <stdint.h>
#include <stdbool.h>


/**
 * \defgroup devices_irq_mpic128_api MPIC128 Registration API
 * \ingroup devices_irq_mpic128
 *
 * \code{.c}
 * #include <devices/mpic128.h>
 * \endcode
 *
 * \addtogroup devices_irq_mpic128_api
 * @{
 */

typedef enum{
    Mpic128Timer0 = 0,
    Mpic128Timer1 = 1,
    Mpic128Timer2 = 2,
    Mpic128Timer3 = 3
}mpic128_tim_num;

typedef enum
{
    mpic128_timer_freq_SYS_CLK = 0,
    mpic128_timer_freq_SYS_TMRCLK = 1,
} mpic128_timer_freq;

/**
 * Register the MPIC128 controller within the interrupt subsystem
 * This function should be called within platform initialization code
 */
void rumboot_irq_register_mpic128();
void mpic128_set_interrupt_borders( uint32_t base_address, uint32_t mc_border, uint32_t cr_border );

void mpic128_timer_init(uint32_t base_address, mpic128_timer_freq frequency);
void mpic128_start_timer(uint32_t const base_address, mpic128_tim_num const timer_num, uint32_t const base_count);
void mpic128_stop_timer(uint32_t const base_address, mpic128_tim_num const timer_num);
/**
* @}
*/


#endif /* PLATFORM_DEVICES_MPIC128_H */
