/**
 *
 * \defgroup platform_glue Platform-specific glue functions
 * These functions should be implemented in the platform support code.
 * Rumboot core modules depend on these for proper operation
 *
 *
 * \addtogroup platform_glue
 *  @{
 */


enum rumboot_simulation_event {
    EVENT_NOP = 0, /** No-op event */
    EVENT_FINISH, /** Successfull completion */
    EVENT_CRASH, /** Fatal error */
    EVENT_WARN, /** Software-generated warning */
    EVENT_PERF, /** Performance checkpoint */
    EVENT_STDOUT, /** A character is ready for output */
    EVENT_STDIN, /** A character is requested */
};

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
void rumboot_platform_raise_event(enum rumboot_simulation_event event);

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


/** @}*/
