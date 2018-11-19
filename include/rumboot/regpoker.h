#ifndef REGPOKER_H
#define REGPOKER_H

/**
 *
 * \defgroup regpoker Register Checker Library
 * \ingroup libraries
 *
 * This library implements a simple way to read/write check an array of registers
 *
 * \code{.c}
 * #include <rumboot/regpoker.h>
 * \endcode
 *
 * \addtogroup regpoker
 *  @{
 */

enum checker_type {
    REGPOKER_EOL = 0,
    REGPOKER_READ8,
    REGPOKER_WRITE8,
    REGPOKER_READ16,
    REGPOKER_WRITE16,
    REGPOKER_READ32,
    REGPOKER_WRITE32,
    REGPOKER_READ64,
    REGPOKER_WRITE64,
#ifdef __PPC__
    REGPOKER_READ_DCR,
    REGPOKER_WRITE_DCR,
#endif
};


struct regpoker_checker {
    const char *        name;
    enum checker_type   tp;
    uintptr_t           base;
    uint64_t            expected;
    uint64_t            mask;
    uint64_t (*readfunc)(uintptr_t base);
    void (*writefunc)(uint64_t value, uintptr_t addr);
};


/**
 * Checks an array of registers for read and write modes.
 *
 * @param  array  Array of struct regpoker_cheker elements to check
 * @param  offset An offset to apply to all base addresses in array
 * @return        The number of registers that failed the check
 */
int rumboot_regpoker_check_array(const struct regpoker_checker *array, uint32_t offset);

/**
*  @}
*/
#endif /* end of include guard: REGPOKER_H */
