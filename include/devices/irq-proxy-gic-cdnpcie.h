#ifndef IRQ_PROXY
#define IRQ_PROXY

/**
 * \defgroup devices_irq_gic_pcie_proxy GIC<-->PCIe Proxy controller
 * \ingroup devices_irq
 *
 * \code{.c}
 * #include <devices/irq-proxy-gic-cdnpcie.h>
 * \endcode
 *
 * \addtogroup devices_irq_gic_pcie_proxy
 * @{
 */


/**
 * Register the proxy controller within the IRQ controller subsystem.
 * This function should be called from platform initialization code (e.g. glue.c)
 * AFTER GIC has already been registered.
 */
void rumboot_irq_register_msix_proxy_controller();

/**
* @}
*/

#endif /* end of include guard: IRQ_PROXY */
