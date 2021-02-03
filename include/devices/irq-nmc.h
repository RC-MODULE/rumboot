#ifndef IRQ_NMC
#define IRQ_NMC

/**
 * \defgroup devices_irq_nmc NeuroMatrix IRQ Controller
 * \ingroup devices_irq
 *
 * NeuroMatrix built-in IRQ controller. 
 *
 * \code{.c}
 * #include <devices/irq-nmc.h>
 * \endcode
 *
 * \addtogroup devices_irq_nmc
 * @{
 */


/**
 * Register the dummy controller within the IRQ controller subsystem.
 * This function should be called from platform initialization code (e.g. glue.c)
 * AFTER GIC has already been registered.
 */
void rumboot_irq_register_nmc_intc_controller();

/**
* @}
*/

#endif /* end of include guard: IRQ_NMC */
