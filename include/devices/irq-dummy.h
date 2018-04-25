#ifndef IRQ_DUMMY
#define IRQ_DUMMY

/**
 * \defgroup devices_irq_dummy Dummy IRQ Controller
 * \ingroup devices_irq
 *
 * Dummy IRQ controller. Doesn't really do a thing.
 * Use as a boilerplate for new IRQ controllers.
 *
 * \code{.c}
 * #include <devices/irq-dummy.h>
 * \endcode
 *
 * \addtogroup devices_irq_dummy
 * @{
 */


/**
 * Register the dummy controller within the IRQ controller subsystem.
 * This function should be called from platform initialization code (e.g. glue.c)
 * AFTER GIC has already been registered.
 */
void rumboot_irq_register_dummy_controller();

/**
* @}
*/

#endif /* end of include guard: IRQ_DUMMY */
