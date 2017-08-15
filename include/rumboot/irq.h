#ifndef RUMBOOT_IRQ_H
#define RUMBOOT_IRQ_H


#define RUMBOOT_IRQ_TYPE_NORMAL       0
#define RUMBOOT_IRQ_TYPE_EXCEPTION    1


/* ARM Exception names */
#define RUMBOOT_IRQ_PREFETCH_ABORT          0
#define RUMBOOT_IRQ_DATA_ABORT              1
#define RUMBOOT_IRQ_UNDEFINED_INSTRUCTION   2
#define RUMBOOT_IRQ_SOFTWARE_EXCEPTION      3

/* IRQ & FIQ */
#define RUMBOOT_IRQ_IRQ  0
#define RUMBOOT_IRQ_FIQ  1

#ifndef __ASSEMBLER__

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
    * struct rumboot_irq_entry *tbl = rumboot_irq_create(NULL);
    * rumboot_irq_set_handler(tbl, 3, handler, 123);
    * rumboot_irq_activate_table(tbl);
    *
    * // at this point irqs are active
    * // Call the following to disable interrupts
    *
    * rumboot_irq_activate_table(NULL);
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
     *
     * @param tbl     [description]
     * @param irq     [description]
     * @param handler [description]
     */
    void rumboot_irq_set_handler(struct rumboot_irq_entry *tbl, int irq, void (*handler)(), void *arg);

    /**
     * Make the specified irq table active. NULL to deactivate any existing table
     *
     * @param tbl [description]
     */
    void rumboot_irq_activate_table(struct rumboot_irq_entry *tbl);

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
    *  @}
    */

    /**
    *
    * \defgroup irq_platform Platfrom bindings: Interrupt subsystem
    * These functions should be implemented by platform code.
    *
    * For people writing tests: YOU DO NOT NEED THESE FUNCTIONS!
    *
    * \code{.c}
    * #include <rumboot/irq.h>
    * \endcode
    *
    * \addtogroup irq_platform
    *  @{
    */

    /**
     * This function is called by the platform assembly code to route all
     * interrupts to controller driver.
     *
     * For people writing tests: YOU DO NOT NEED THIS FUNCTION!
     *
     * @param type RUMBOOT_IRQ_TYPE_NORMAL or RUMBOOT_IRQ_TYPE_EXCEPTION
     * @param id   RUMBOOT_IRQ_IRQ, RUMBOOT_IRQ_FIQ, etc.
     */
    void rumboot_irq_core_dispatch(uint32_t type, uint32_t id);



    /**
    *  @}
    */

#endif


#endif /* end of include guard: RUMBOOT_IRQ_H */
