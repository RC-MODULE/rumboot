#ifndef PRINTF_H
#define PRINTF_H
#include <stdint.h>
#include <stdarg.h>

/**
 *
 * \defgroup stdio Standard input/output
 *
 * Functions and macros in this group implement reading and writing
 *
 * \code{.c}
 * #include <rumboot/printf.h>
 * \endcode
 *
 * \addtogroup stdio
 *  @{
 */


/**
 * Rumboot's printf implementation. The user-formatted text is sent to
 * the currently active stout device
 * _WARNING:_ Floats are not supported. Open a JIRA issue if you really need them
 *
 * @param fmt     format
 * @param ... arguments
 */

void rumboot_printf(const char *fmt, ...);

/**
 * Rumboot's vprintf implementation. The same as printf, but with va_list
 * as the argument
 *
 * @param  fmt  [description]
 * @param  args [description]
 * @return      [description]
 */
int rumboot_vprintf(const char *fmt, va_list args);


/**
 * Outputs a string to the currently active stdout device
 *
 * @param string [description]
 */
void rumboot_putstring(const char *string);

/**
 * Prints The Rumboot logo, current git branch and commit version
 */
void rumboot_print_logo();


/** @}*/

#endif
