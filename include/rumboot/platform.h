#ifndef RUMBOOT_PLATFORM_H
#define RUMBOOT_PLATFORM_H

#include <stdint.h>
/**
 *
 * \defgroup platform_glue Platform bindings
 *
 * These functions should be implemented in the platform support code.
 * Rumboot core modules depend on these for proper operation
 * \code{.c}
 * #include <rumboot/testsuite.h>
 * \endcode
 *
 * \defgroup platform_glue_base Basics
 * \ingroup platform_glue
 *
 * \defgroup platform_glue_irq IRQ subsystem glue functions
 * \ingroup platform_glue
 *
 * \defgroup platform_glue_lds Global variables defined by linker
 * \ingroup platform_glue
 *
 * \addtogroup platform_glue_base
 *  @{
 */


/**
 * This enumeration represents common events from rumboot to
 * simulation environment. Platfoms should implement their
 * specific ways of delivering these events.
 */
enum rumboot_simulation_event {
    EVENT_NOP, /** No-op event */
    EVENT_TERM, /** Termination request */
    EVENT_STDOUT, /** A character is ready for output */
    EVENT_STDIN, /** Request for next character input */
    EVENT_TRACE, /** A function call trace */
    EVENT_PRINTF, /** Simulator printf */
    EVENT_UPLOAD, /** Request file upload */
};


/**
 * Emergency exit, optionally printing out a reason why
 * This will terminate the simulation or cause an infinite loop
 * in production builds.
 *
 * WARNING! Use with extreme care for production code!
 *
 * @param fmt format for description message. Can be NULL
 */
void rumboot_platform_panic(const char *fmt, ...) __attribute__((noreturn));

/**
 * This function will be called before main(). This function should contains
 * platform-specific initialization. This should initilize at lest:
 * - A timer for rumboot_platform_get_uptime()
 * - The event system for rumboot_platform_raise_event()
 *
 */
void rumboot_platform_setup();

/**
 * Returns the current system uptime (useconds)
 * @return useconds
 */
uint32_t rumboot_platform_get_uptime();


/**
 * This function is called to trace a function call with a pc value
 * @param pc register value
 */
void rumboot_platform_trace(void *pc);

/**
 * Raise an event to notify the environment. The EVENT_FINISH and EVENT_CRASH
 * can be used to terminate the simulation
 *
 *
 * @param event Event code to send to modelling environment
 * @param arg Simulation event argument
 */
void rumboot_platform_raise_event(enum rumboot_simulation_event event, uint8_t arg);

/**
 * Send a character to stdout.
 * @param c [description]
 */
void rumboot_platform_putchar(uint8_t c);

/**
 * Return the character from stdint casted to int or
 * -1 if reception timed out.
 * Timeout may be 0
 *
 * @param  timeout_us [description]
 * @return            [description]
 */
int rumboot_platform_getchar(uint32_t timeout_us);

/**
 * Request simulation environment to upload a file from plusarg to address addr
 *
 * @param plusarg the name of plusarg that contains the filename or the filename
 * @param addr   address to load the file to
 */
void rumboot_platform_request_file(const char *plusarg, uint32_t addr);
/**
*
*  @}
*/

/**
*
* \addtogroup platform_glue_lds
*  @{
*/


/**
 * This global structure stores internal romboot state.
 * It stores selftest results for further inspection by secondary
 * loader. The instance is always available via rumboot_platform_runtime_info
 */
struct rumboot_runtime_info {
    /** Magic value. Indicates that this structure contains valid data */
    uint32_t magic;
    /** Current heap end pointer, used by _sbrk and rumboot_malloc() */
    char *current_heap_end;
    /** Pointer to current active irq table. Do not use directly, use rumboot_irq_table_get() */
    void *irq_handler_table;
};

/**
 * Global instance of struct rumboot_runtime_info.
 */
extern struct rumboot_runtime_info rumboot_platform_runtime_info;

/**
 * This global variable defined by the linker points to the start of SPL
 * area. Use &reference to this variable to get the desired address.
 */
extern char rumboot_platform_spl_start;

/**
 * This global variable defined by the linker points to the end of SPL
 * area. Use &reference to this variable to get the desired address.
 */
extern char rumboot_platform_spl_end;

/**
* This global variable defined by the linker points to the top of the stack.
* Use &reference to this variable to get the desired address.
*/
extern void *rumboot_platform_stack_top;

/**
 * This global variable defined by the linker points to the beginning of the heap area.
 * Use &reference to this variable to get the desired address.
 */
extern char rumboot_platform_heap_start;

/**
 * This global variable defined by the linker points to the end of the heap area.
 * Use &reference to this variable to get the desired address.
 */
extern char rumboot_platform_heap_end;

/**
 * This global variable defined by the linker points to the lowest address where
 * stack(s) are stored. Use &reference to this variable to get the desired address.
 */
extern char rumboot_platform_stack_area_start;

/**
 * This global variable defined by the linker points to the highest address where
 * stack(s) are stored. Use &reference to this variable to get the desired address.
 */
extern char rumboot_platform_stack_area_end;

/**
 * Start of the BSS section
 */
extern char rumboot_platform_bss_start;

/**
 * End of bss section
 */
extern char rumboot_platform_bss_end;

/** @}*/

#endif /* end of include guard: RUMBOOT_PLATFORM_H */
