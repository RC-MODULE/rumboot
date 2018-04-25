#ifndef DEVICES_IRQ_GIC_H
#define DEVICES_IRQ_GIC_H

/**
 * \defgroup devices_irq_gic_api GIC Registration API
 * \ingroup devices_irq_gic
 *
 * \code{.c}
 * #include <devices/gic.h>
 * \endcode
 *
 * \addtogroup devices_irq_gic_api
 * @{
 */


/**
 * Register the GIC constroller within the interrupt subsystem
 * This function should be called within platform initialization code
 */
void rumboot_irq_register_gic();

/**
* @}
*/


#endif /* end of include guard: DEVICES_IRQ_GIC_H */
