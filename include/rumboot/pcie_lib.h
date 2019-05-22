#ifndef PCIE_LIB_H
#define PCIE_LIB_H

#include <stdint.h>

/**
 *
 * \defgroup PCIe Functions
 * \ingroup devices_pcie
 *
 * \code{.c}
 * #include <rumboot/pcie_test_lib.h>
 * \endcode
 * Functions in this group are used for real work with PCIe subsystem
 *
 *  @{
 */

    /**
     * This function setup initial PCIe parameters and waits PLL lock
     *   Parameters description in .c file
     *
     * @param speed                     [description]
     * @param device_id                 [description]
     * @param revision_id               [description]
     * @param subclass_code             [description]
     * @param class_code                [description]
     * @param state_auto_ack            [description]
     * @return pass
     */
    uint32_t pcie_init
    (
        uint32_t speed              ,
        uint32_t device_id          ,
        uint32_t revision_id        ,
        uint32_t subclass_code      ,
        uint32_t class_code         ,
        uint32_t state_auto_ack
    );

    /**
     * This function setup initial BAR parameters
     *   Parameters description in .c file
     *
     * @param bar_number         [description]
     * @param bar_axi_start      [description]
     * @param bar_aperture       [description]
     * @param bar_type           [description]
     */
    void pcie_add_bar
    (
        uint32_t bar_number     ,
        uint32_t bar_axi_start  ,
        uint32_t bar_aperture   ,
        uint32_t bar_type
    );

    /**
     * This function waits, until PCIe link is up
     *
     * @return pass
     */
    uint32_t pcie_link_up ();
    
/** @}*/

#endif /* end of include guard: PCIE_LIB_H */
