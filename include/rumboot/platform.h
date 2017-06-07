/**
 *
 * \defgroup platform_glue Platform-specific glue functions
 * \addtogroup platform_glue
 *  @{
 */

enum rumboot_simulation_event {
    EVENT_FINISH = 0,
    EVENT_CRASH,
    EVENT_WARN
};

/**
 * [rumboot_platform_get_uptime description]
 * @return [description]
 */
uint32_t rumboot_platform_get_uptime();

/**
 * Raise a specific event to notify the environment
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
