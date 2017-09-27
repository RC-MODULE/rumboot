#ifndef RUMBOOT_IRQ_H
#define RUMBOOT_IRQ_H


#define RUMBOOT_IRQ_TYPE_NORMAL       0
#define RUMBOOT_IRQ_TYPE_EXCEPTION    1
#define RUMBOOT_IRQ_TYPE_SOFT        2

/* ARM Exception names */
#define RUMBOOT_IRQ_PREFETCH_ABORT          0
#define RUMBOOT_IRQ_DATA_ABORT              1
#define RUMBOOT_IRQ_UNDEFINED_INSTRUCTION   2
#define RUMBOOT_IRQ_SOFTWARE_EXCEPTION      3

/* IRQ Types */
#define RUMBOOT_IRQ_IRQ  0
#define RUMBOOT_IRQ_FIQ  1

#define RUMBOOT_IRQ_EDGE  (1 << 1)
#define RUMBOOT_IRQ_LEVEL (0 << 1)

#ifndef __ASSEMBLER__

#ifdef __arm__
#include <arch/arm/irq_macros.h>
#else
    /* stubs */
static inline int rumboot_arch_irq_disable(void)
{
    return 0;
}

static inline void rumboot_arch_irq_enable()
{

}

#define RUMBOOT_ATOMIC_BLOCK()
#endif


    /**
    *
    * \defgroup irq Interrupt subsystem
    * These functions implement interrupt handling in rumboot code.
    * \code{.c}
    * #include <rumboot/irq.h>
    * \endcode
    *
    * A typical usage example is provided for you below.
    *
    * \code{.c}
    * #include <rumboot/irq.h>
    * int main()
    * {
    * // Create an IRQ table
    * struct rumboot_irq_entry *tbl = rumboot_irq_create(NULL);
    * // Set a handler
    * rumboot_irq_set_handler(tbl, 3, 0 handler, (void*) arg);
    * //Activate the table, it will enable configured IRQ lines
    * rumboot_irq_table_activate(tbl);
    *
    * // at this point irqs are active
    *
    * //Proper deinitialization
    * rumboot_irq_table_activate(NULL);
    * // Free the table
    * rumboot_irq_free(tbl);
    * \endcode
    *
    * \addtogroup irq
    *  @{
    */

    /** This struct should be used as opaque object. All irq functions use it.
     */
    struct rumboot_irq_entry;


    /**
     * Create an IRQ handler table. The resulting table should be freed using
     * rumboot_irq_free().
     *
     * @param  copyfrom [description]
     * @return          [description]
     */
    struct rumboot_irq_entry *rumboot_irq_create(struct rumboot_irq_entry *copyfrom);

    /**
     * Release an irq table. The table should NOT be active.
     * @param tbl [description]
     */
    void rumboot_irq_free(struct rumboot_irq_entry *tbl);
    /**
     * Set an irq handler in the user-supplied irq table. An optional arg will
     * be passed by the subsystem code to the handler function
     * If tbl is NULL, currently active IRQ table is used
     *
     * @param tbl     IRQ table instance
     * @param irq     IRQ Line number
     * @param flags   Bitmask of RUMBOOT_IRQ_FIQ, RUMBOOT_IRQ_EDGE...
     * @param handler Pointer to handler function
     * @param arg     Optional arg to be passed to handler function
     */
     void rumboot_irq_set_handler(struct rumboot_irq_entry *tbl, int irq, uint32_t flags,
        void (*handler)(int irq, void *args), void *arg);

    /**
     * Make the specified irq table active. NULL to deactivate any existing table
     * and disable interrupts.
     *
     * WARNING: Activating a table will also make any interrupts with configured handlers
     * enabled.  If this is NOT what you want - activate a table before configuring any of
     * the interrupt handlers.
     *
     *
     * @param tbl [description]
     */
     void rumboot_irq_table_activate(struct rumboot_irq_entry *tbl);

    /**
     * Enable IRQ line
     *
     * @param irq [description]
     */
    void rumboot_irq_enable(int irq);

    /**
     * Disable IRQ line
     *
     * @param irq [description]
     */
    void rumboot_irq_disable(int irq);

    /**
     * Get current active irq table
     * @return [description]
     */
    void *rumboot_irq_table_get();

    /**
     * Disable global interrupt handling by current CPU core
     * Returns previous interrupt status that can be passed to
     * rumboot_irq_setstate()
     */
    static inline int rumboot_irq_cli()
    {
        return rumboot_arch_irq_disable();
    }

    /**
     * Enable interrupt handling by current CPU core
     */
    static inline void rumboot_irq_sei()
    {
        rumboot_arch_irq_enable();
    }

    /**
     * Restore a previous IRQ handling state
     *
     * @param state IRQ handling state from rumboot_irq_cli()
     */
    static inline void rumboot_irq_setstate(int state)
    {
        rumboot_arch_irq_setstate(state);
    }

    /**
    *  @}
    */

    /**
    *
    * \defgroup platform_glue_irq Platfrom bindings: Interrupt subsystem
    * These functions should be implemented by platform code.
    *
    * For people writing tests: YOU DO NOT NEED THESE FUNCTIONS!
    *
    * \code{.c}
    * #include <rumboot/irq.h>
    * \endcode
    *

    /**
    *  \addtogroup platform_glue_irq
    *  @{
    */

    /**
     * This function should be called by the platform assembly code to route all
     * interrupts to controller driver.
     *
     * For people writing tests: YOU DO NOT NEED THIS FUNCTION!
     *
     * @param type RUMBOOT_IRQ_TYPE_NORMAL or RUMBOOT_IRQ_TYPE_EXCEPTION
     * @param id   RUMBOOT_IRQ_IRQ, RUMBOOT_IRQ_FIQ, etc.
     */
    void rumboot_irq_core_dispatch(uint32_t type, uint32_t id);

    /**
     * ARCH-specific glue: Disable CPU IRQ handling.
     * DO NOT USE DIRECTLY - USE rumboot_irq_cli()
     */
    static inline int rumboot_arch_irq_disable();

    /**
     * ARCH-specific glue: Enable CPU IRQ handling.
     * DO NOT USE DIRECTLY - USE rumboot_irq_sei()
     */
    static inline void rumboot_arch_irq_enable();

    /**
     * This function is called by the IRQ subsystem when beginning to
     * service the IRQ routine. The function should return the next pending
     * irq number
     *
     * @return IRQ ID that is currently being serviced
     */
    uint32_t rumboot_platform_irq_begin();

    /**
     * This function is called by the IRQ subsystem when the subsystem is done
     * servicing the current interrupt.
     * @param irq id of the irq that is done being serviced
     */
    void rumboot_platform_irq_end(uint32_t irq);

    /**
     * This function is called by the IRQ subsystem to reconfigure an interrupt
     * line.
     * @param irq    The irq number
     * @param flags  A bitmask of RUMBOOT_IRQ_IRQ/RUMBOOT_IRQ_FIQ,
     *               RUMBOOT_IRQ_LEVEL/RUMBOOT_IRQ_EDGE. In most cases - just use 0.
     * @param enable Do we want it enabled
     */
    void rumboot_platform_irq_configure(int irq, uint32_t flags, int enable);
    /**
    *  @}
    */

#endif


#endif /* end of include guard: RUMBOOT_IRQ_H */
