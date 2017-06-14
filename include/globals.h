#ifndef RUMBOOT_GLOBALS_H
#define RUMBOOT_GLOBALS_H

/**
 *
 * \defgroup globals Global variables
 * These global variables and structures are defined by the linker script
 * Among those the rumboot_runtime_info struct holds the internal rumboot state
 * information
 * \code{.c}
 * #include <rumboot/globals.h>
 * \endcode
 *
 * \addtogroup globals
 *  @{
 */


/**
 * This global structure stores internal romboot state.
 * It stores selftest results for further inspection by secondary
 * loader.
 */
struct rumboot_runtime_info {
    uint32_t magic; /** Magic value. Indicates that this structure contains valid data */
    void *current_heap_end; /** Current heap end pointer, used by _sbrk */
};

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


#endif /* end of include guard: RUMBOOT_GLOBALS_H */
