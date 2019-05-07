#ifndef PRINTF_H
#define PRINTF_H
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

/**
 *
 * \defgroup stdio Standard input/output
 * \ingroup libraries
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

#if !defined(RUMBOOT_NEWLIB_PRINTF)
/**
 * Rumboot's printf implementation. The user-formatted text is sent to
 * the currently active stout device
 *
 * _WARNING:_ Floats and uint64_t are not supported. Open a JIRA issue if you
 * really need an unsupported type.
 *
 * If rumboot code is compiled with -DRUMBOOT_PRINTF_ACCEL the internal printf
 * implementation is disabled. Platform code should provide an accelerated printf
 * implementation instead (e.g. for simulation speedup).
 *
 * @param fmt     format
 * @param ... arguments
 */

void rumboot_printf(const char *fmt, ...);
#else

#include <stdio.h>
#define rumboot_printf(fmt, ...) printf(fmt, ##__VA_ARGS__)

#endif
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

/**
 * Return the character from stdint casted to int or
 * -1 if reception timed out. 
 * 
 * @param timeout_us 
 * @return int 
 */
int rumboot_getchar(uint32_t timeout_us);

/**
 * Send a character to stdout.
 * Automatically add \r for every \n
 * @param c character
 */
void rumboot_putchar(uint8_t c);


/** @}*/


#endif
