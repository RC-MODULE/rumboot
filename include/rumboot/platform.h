#ifndef RUMBOOT_PLATFORM_H
#define RUMBOOT_PLATFORM_H

#include <stdint.h>
/**
 *
 * \defgroup platform_glue Platform-specific glue functions
 * These functions should be implemented in the platform support code.
 * Rumboot core modules depend on these for proper operation
 * \code{.c}
 * #include <rumboot/testsuite.h>
 * \endcode
 *
 * \addtogroup platform_glue
 *  @{
 */


/**
 * This enumeration represents common events from rumboot to
 * simulation environment. Platfoms should implement their
 * specific ways of delivering these events.
 */
enum rumboot_simulation_event {
    EVENT_NOP, /** No-op event */
    EVENT_FINISH, /** Successfull completion */
    EVENT_CRASH, /** Fatal error */
    EVENT_WARN, /** Software-generated warning */
    EVENT_PERF, /** Performance checkpoint */
    EVENT_STDOUT, /** A character is ready for output */
    EVENT_STDIN, /** A character is requested */
};


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
 * Raise an event to notify the environment. The EVENT_FINISH and EVENT_CRASH
 * can be used to terminate the simulation
 *
 *
 * @param event
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
 * This global structure stores internal romboot state.
 * It stores selftest results for further inspection by secondary
 * loader.
 */
struct rumboot_runtime_info {
    uint32_t magic; /** Magic value. Indicates that this structure contains valid data */
    void *current_max_heap; /** Current heap end pointer, used by _sbrk */
};

/**
 * Global instance of struct rumboot_runtime_info.
 */
extern struct rumboot_runtime_info rumboot_platform_runtime_info;

/**
 * This global variable defined by the linker points to the start of SPL
 * area
 */
extern void *rumboot_platform_spl_start;

/**
 * This global variable defined by the linker points to the end of SPL
 * area
 */
extern void *rumboot_platform_spl_end;

/**
* This global variable defined by the linker points to the top of the stack
*/
extern void *rumboot_platform_stack_top;

/**
 * This global variable defined by the linker points to the beginning of the heap area
 */
extern void *rumboot_platform_heap_start;

/**
 * This global variable defined by the linker points to the end of the heap area
 */
extern void *rumboot_platform_heap_end;

/** @}*/

#endif /* end of include guard: RUMBOOT_PLATFORM_H */
