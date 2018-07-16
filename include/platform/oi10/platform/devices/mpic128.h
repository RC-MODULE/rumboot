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
 * #include <platform/devices/mpic128.h>
 * \endcode
 *
 * \addtogroup devices_irq_mpic128_api
 * @{
 */

/**
 * Register the MPIC128 controller within the interrupt subsystem
 * This function should be called within platform initialization code
 */
void rumboot_irq_register_mpic128();

/**
* @}
*/


#endif /* PLATFORM_DEVICES_MPIC128_H */
