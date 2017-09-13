#ifndef DMA_TEST_LIB_H
#define DMA_TEST_LIB_H

#include <stdint.h>

#include <basis/defs_c.h>

/**
 *
 * \defgroup DMA test library
 *
 * \code{.c}
 * #include <rumboot/dma_test_lib.h>
 * \endcode
 * Functions in this group are used for DMA subsystem testing
 *
 *  @{
 */
 
    /**
     * This function is for creating descriptor, used by RMACE DMA block.
     * More description in .c file.
     *
     * @param sys_addr           [description]
     * @param len                [description]
     * @param desc_addr          [description]
     * @param act0               [description]
     * @param interrupt          [description]
     * @param valid              [description]
     * @param endian             [description]
     * @return sys_addr_end
     */
    uint32_t rmace_set_desc
    (
        uint32_t sys_addr     ,
        uint32_t len          ,
        uint32_t desc_addr    ,
        uint8_t  act0         ,
        uint8_t  interrupt    ,
        uint8_t  valid        ,
        uint8_t  endian
    );
    
    /** @}*/


typedef volatile struct __attribute__ ((__packed__))
{
    //---------------------------------------------------------------
    //  This is Configuration descriptor.
    //---------------------------------------------------------------
    uint32_t   DescConf_Address             : 32;
    uint32_t   DescConf_Length_and_Ctrl     : 32;
    //---------------------------------------------------------------
    //  This is Data descriptor.
    //  There may be many of them. Add neccessary amount.
    //---------------------------------------------------------------
    //-----------------------------------------------------
    //      [31:0]    AXI Base Address
    //-----------------------------------------------------
    uint32_t   Desc0_Address                : 32;
    //-----------------------------------------------------
    //      [31:6]   Length of data in bytes
    //      [5:4]    {Act2, Act1}
    //        10       Descriptor execution in progress
    //        != 10    Descriptor execution complete
    //      [3]      Act0
    //        0        Not last descriptor
    //        1        This is last descriptor
    //      [2]      Int
    //      [1]      Reserved
    //      [0]      Valid
    //-----------------------------------------------------
    uint32_t   Desc0_Length_and_Ctrl        : 32;
    
    // uint32_t   Desc1_Address                : 32;
    // uint32_t   Desc1_Length_and_Ctrl        : 32;
    // uint32_t   Desc2_Address                : 32;
    // uint32_t   Desc2_Length_and_Ctrl        : 32;
    // uint32_t   Desc3_Address                : 32;
    // uint32_t   Desc3_Length_and_Ctrl        : 32;
    
    //---------------------------------------------------------------
    //  This is End descriptor.
    //---------------------------------------------------------------
    uint32_t   DescEnd_Address              : 32;
    uint32_t   DescEnd_Length_and_Ctrl      : 32;
} RMACE_DMA_descriptors;

#endif /* end of include guard: DMA_TEST_LIB_H */
