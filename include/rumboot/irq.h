#ifndef RUMBOOT_IRQ_H
#define RUMBOOT_IRQ_H

#define RUMBOOT_IRQ_TYPE_NOTIRQ         0
#define RUMBOOT_IRQ_TYPE_NORMAL         1
#define RUMBOOT_IRQ_TYPE_EXCEPTION      2
#define RUMBOOT_IRQ_TYPE_SOFT           3
#define RUMBOOT_IRQ_TYPE_FIQ            4
#define RUMBOOT_IRQ_TYPE_CRITICAL       5
#define RUMBOOT_IRQ_TYPE_MACHINECHECK   6

/* ARM Exception names */
#define RUMBOOT_IRQ_PREFETCH_ABORT          0
#define RUMBOOT_IRQ_DATA_ABORT              1
#define RUMBOOT_IRQ_UNDEFINED_INSTRUCTION   2
#define RUMBOOT_IRQ_SOFTWARE_EXCEPTION      3
/* PPC Interrupt names */
#define RUMBOOT_IRQ_MACHINE_CHECK           4
#define RUMBOOT_IRQ_DATA_STORAGE            5
#define RUMBOOT_IRQ_INST_STORAGE            6
#define RUMBOOT_IRQ_ALIGNMENT               7
#define RUMBOOT_IRQ_PROGRAM                 8
#define RUMBOOT_IRQ_FP_UNAVAILABLE          9
#define RUMBOOT_IRQ_SYSTEM_CALL             10
#define RUMBOOT_IRQ_AP_UNAVAILABLE          11
#define RUMBOOT_IRQ_DECREMENTER             12
#define RUMBOOT_IRQ_FIXED_INTERVAL_TIMER    13
#define RUMBOOT_IRQ_WATCHDOG_TIMER          14
#define RUMBOOT_IRQ_DATA_TLB_ERROR          15
#define RUMBOOT_IRQ_INST_TLB_ERROR          16
#define RUMBOOT_IRQ_DEBUG                   17
/* NMC Interrupt names */
#define RUMBOOT_IRQ_OVERFLOW                18



#ifndef __ASSEMBLER__
#include <stdlib.h>

#if !defined(RUMBOOT_NATIVE) && (defined( __arm__) || defined(__riscv) || defined (__NM__))
#include <arch/irq_macros.h>
#elif !defined(RUMBOOT_NATIVE) && (defined( __PPC__ ) && !defined(RUMBOOT_NO_IRQ_MACROS))
#include <arch/irq_macros.h>
#else

#define RUMBOOT_IRQ_EDGE    (0)
#define RUMBOOT_IRQ_LEVEL   (0)
#define RUMBOOT_IRQ_POS     (0)
#define RUMBOOT_IRQ_NEG     (0)
#define RUMBOOT_IRQ_HIGH    (0)
#define RUMBOOT_IRQ_LOW     (0)

/* stubs */
static inline uint32_t rumboot_arch_irq_disable( )
{
    return 0;
}

static inline uint32_t rumboot_arch_irq_enable()
{
    return 0;
}

static inline uint32_t rumboot_arch_irq_setstate(uint32_t new_state)
{
    return 0;
}

#define RUMBOOT_ATOMIC_BLOCK()
#endif


    /**
    *
    * \defgroup irq Interrupt subsystem
    * \ingroup libraries
    *
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
     * Internally it represents an allocated irq table with all configured interrupts
     * and their respective vectors
     */
    struct rumboot_irq_entry;

    /**
     * This struct represents an IRQ controller in the system
     */
    struct rumboot_irq_controller {
        /** Human-readable name for this controller */
        const char *name;
    	int			first; /** The first IRQ line handled by this controller */
    	int			last; /** The last IRQ line handled by this controller */

        /**
         * This function is called to initialize IRQ controller
         * @param dev irq_controller_device
         * @param arg user-defined argument (passed from rumboot_irq_register_controller())
         */
    	int		    (*init)(const struct rumboot_irq_controller *dev);

        /**
         * This function is called by the IRQ subsystem to reconfigure an interrupt
         * line.
         *
         * @param dev    The irq controller device
         * @param irq    The irq number
         * @param flags  A bitmask of RUMBOOT_IRQ_IRQ/RUMBOOT_IRQ_FIQ,
         *               RUMBOOT_IRQ_LEVEL/RUMBOOT_IRQ_EDGE. In most cases - just use 0.
         * @param enable Do we want it enabled
         */
    	void		(*configure)(const struct rumboot_irq_controller *dev, int irq, uint32_t flags, int enable);
        /**
         * This function is called by the IRQ subsystem when starting to
         * service the IRQ routine. The function should return the current pending
         * irq number. It can accept arbitrary id from underlying assembly code as
         * a parameter and either return it, or change in the driver code.
         *
         * @param dev      The irq controller device
         * @param id       The driver-specific id. Controller driver may override it
         * @return IRQ ID that is currently being serviced
         */
    	uint32_t	(*begin)(const struct rumboot_irq_controller *dev, int id);
        /**
         * This function is called by the IRQ subsystem when the subsystem is done
         * servicing the current interrupt.
         *
         * @param dev      The irq controller device
         * @param irq id of the irq that is done being serviced
         */
    	void		(*end)(const struct rumboot_irq_controller *dev, int irq);

        /**
         * Generate a software interrupt.
         *
         * @param generate_swint [description]
         */
    	void		(*generate_swint)(const struct rumboot_irq_controller *dev, int irq);

        /**
         * Deprecated. Do not use.
         */
	    void		(*slave_save)(void);
        /**
         * Deprecated. Do not use.
         */
	    void		(*slave_restore)(void);


	    void		(*adjust_priority)(const struct rumboot_irq_controller *dev, int irq, int priority);
        /**
         * Maximum possible interrupt priority supported by this controller
         */
        int         priority_max;

        /**
         * Minimum possible interrupt priority supported by this controller
         */
        int         priority_min;

        /** Default irq line priority (if not set) */
        int         priority_default;

        /** Base address for this controller */
        uintptr_t    base0;
        /** Optional secondary base address. Some interrupt controllers e.g. gic have 2 base addresses: (distributor & cpuif) */
        uintptr_t    base1;

    };

    /**
     * Create an IRQ handler table. The resulting table should be freed using
     * rumboot_irq_free().
     *
     * If copyfrom is NOT NULL, it will be copied to the newly allocated IRQ table
     *
     * If copyfrom is NULL and there is an active IRQ table, currently active table
     * will be used as a template when creating a new table
     *
     * @param  copyfrom
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
     * Sets IRQ line priority, greater numbers indentify interrupts to be
     * serviced first.
     *
     * The maximum and minimum priorities are specified in
     * struct rumboot_irq_controller and should be obtained by
     * rumboot_irq_prioity_get_max() and rumboot_irq_prioity_get_min() functions.
     * 
     * _WARNING_: 0 is always considered default priority and is overwritten by controller's
     * default priority when rumboot_irq_priority_adjust() is called. Either internally, or 
     * within the IRQ subsystem
     *
     * If the requested priority if greater than maximum possible or lesser
     * that minimum possible a warning will be generated and maximum and minimum
     * priorities will be used instead.
     *
     * For convenience, this function returns previous priority value.
     *
     * @param tbl      IRQ table to work with. Specify NULL for currently active table
     * @param irq      IRQ number
     * @param priority New priority
     * @return Previous priority
     */
     int rumboot_irq_priority_adjust(struct rumboot_irq_entry *tbl, int irq, int priority);

     /**
      * Returns current IRQ priority for an IRQ line
      *
      * @param  tbl IRQ table to work with. Specify NULL for currently active table
      * @param  irq IRQ number
      * @return IRQ priority
      */
     int rumboot_irq_priority_get(struct rumboot_irq_entry *tbl, int irq);

     /**
      * Returns the maximum possible priority for the line
      * @param  irq [description]
      * @return     [description]
      */
     int rumboot_irq_prioity_get_max(int irq);
     /**
      * Returns the minimum possible priority for the line
      * @param  irq [description]
      * @return     [description]
      */
     int rumboot_irq_prioity_get_min(int irq);

    /**
     * Request controller to generate an interrupt on line irq.
     * NOTE: Not all IRQ lines may be supported by the controller driver
     *
     * @param irq The IRQ line to trigger
     */
     void rumboot_irq_swint(int irq);

    /**
     * Set a default IRQ handler. This will be called when an interrupt
     * arrives in the absense of an active IRQ table or with no handler
     * configured.
     * @param handler
     * @param arg
     */
     void rumboot_irq_set_default_handler(void (*handler)(int irq, void *arg), void *arg);

     /**
      * Set user exception handler. This one will be called when an exception
      * happens. Default handler will only print exception name and panic.
      * Set this one to provide additional debug information
      *
      * @param handler Pointer to exception handler
      */
     void rumboot_irq_set_exception_handler(void (*handler)(int id, const char *name));

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
     * Enable all available IRQ lines.
     * Convenience function used to catch spurious interrupts.
     */
    void rumboot_irq_enable_all();

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
     * Fetches the number of times this interrupt has arrived.
     * All accounting is done in the active irq table and copied
     * when nesting
     * 
     * @param irq IRQ number
     * @return uint32_t 
     */
    uint32_t rumboot_irq_get_count(int irq);

    /** Resets irq count information in irq table.
     *  All accounting is done in the active irq table and copied
     *  when nesting
     *
     *  @param irq IRQ number
     */
    void rumboot_irq_reset_count(int irq);

    /**
     * Get current context level. 
     * 0 - Normal program execution
     * 1 - ISR context
     * 2 and more - nested ISR context
     * 
     * @return int 
     */
    int rumboot_irq_get_context();


    /**
     * @brief Get the type of interrupt we're in.
     * Returns one of these:
     *
     * RUMBOOT_IRQ_TYPE_NOTIRQ    
     * RUMBOOT_IRQ_TYPE_EXCEPTION 
     * RUMBOOT_IRQ_TYPE_NORMAL    
     * RUMBOOT_IRQ_TYPE_SOFT      
     * RUMBOOT_IRQ_TYPE_FIQ       
     * RUMBOOT_IRQ_TYPE_CRITICAL
     * RUMBOOT_IRQ_TYPE_CRITICAL  
     * 
     */
    int rumboot_irq_current_type();

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
    */

    /**
    *  \addtogroup platform_glue_irq
    *  @{
    */

    /**
     * This function should be called by the platform assembly code to route all
     * interrupts to controller driver or from C code implementing nested controller.
     *
     * For people writing tests: YOU NORMALLY DO NOT NEED THIS FUNCTION!
     *
     * @param ctrl interrupt controller id obtained from rumboot_irq_register_controller
     * @param type RUMBOOT_IRQ_TYPE_NORMAL or RUMBOOT_IRQ_TYPE_EXCEPTION
     * @param id   RUMBOOT_IRQ_IRQ, RUMBOOT_IRQ_FIQ, etc.
     */
    void rumboot_irq_core_dispatch(uint32_t ctrl, uint32_t type, int id);

    /**
     * ARCH-specific glue: Disable CPU IRQ handling.
     * DO NOT USE DIRECTLY - USE rumboot_irq_cli()
     */
    static inline uint32_t rumboot_arch_irq_disable();

    /**
     * ARCH-specific glue: Enable CPU IRQ handling.
     * DO NOT USE DIRECTLY - USE rumboot_irq_sei()
     */
    static inline uint32_t rumboot_arch_irq_enable();

    /**
     * This function should be called via platform glue code to register
     * and initilize a new interrupt controller.
     * @param ctl [description]
     */
    void rumboot_irq_register_controller(const struct rumboot_irq_controller *ctl);

    /**
    *  @}
    */

   const struct rumboot_irq_controller *rumboot_irq_controller_by_id(int id);

#endif


#endif /* end of include guard: RUMBOOT_IRQ_H */
