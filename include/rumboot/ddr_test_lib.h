#ifndef DDR_TEST_LIB_H
#define DDR_TEST_LIB_H

#include <stdint.h>

/**
 *
 * \defgroup example_functions Example Functions
 * \ingroup devices_ddr
 *
 * \code{.c}
 * #include <rumboot/ddr_test_lib.h>
 * \endcode
 * Functions in this group are used for DDR subsystem testing
 * Also they may be used as examples for further development
 *
 *  @{
 */

    /**
     * This function is for turning On one of DDR subsystem.
     * More description in .c file.
     *
     * @param DDRx_BASE           [description]
     * @return pass
     */
    uint32_t ddr_init (uint32_t DDRx_BASE);

    /**
     * This function is for turning On both DDR subsystems.
     * More description in .c file.
     *
     * @return pass
     */
    uint32_t ddr0_ddr1_init ();

    /**
     * This function is for changing DDR external CRG frequency.
     * More description in .c file.
     *
     * @param pll_fbdiv
     * @param pll_psdiv
     * @return pass
     */
    uint32_t crg_ddr_init
    (
        uint32_t pll_fbdiv ,
        uint32_t pll_psdiv
    );

    /**
     * This function checks status of DDR Core and Phy, and
     *   reports, if there are any errors.
     * More description in .c file.
     *
     *    !!!  Not all status is checked now  !!!
     *
     * @param DDRx_BASE
     * @return pass
     */
    uint32_t ddr_check_status
    (
        uint32_t DDRx_BASE
    );
    /** @}*/

    /**
     * This function must be called after ddr_init (or smth similar)
     * It prepares required value of SDRAM for work, if ECC is On
     *
     * @param DDRx_BASE
     * @param SDRAM_start_addr
     * @param SDRAM_size
     */
    void ddr_ecc_init
    (
        uint32_t DDRx_BASE,
        uint32_t SDRAM_start_addr,
        uint32_t SDRAM_size
    );
    /** @}*/


#endif /* end of include guard: DDR_TEST_LIB_H */
