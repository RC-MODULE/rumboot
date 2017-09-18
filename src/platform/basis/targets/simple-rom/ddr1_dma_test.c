
//-----------------------------------------------------------------------------
//  This program is for checking DDR1 performance.
//  2 RMACE DMA are used.
//  
//  
//  Test includes:
//    - Change DDR1 CRG frequency to correspond ddr3 1600 data rate
//    - DDR1 turning On function
//    - create etalon arrays for 2 RMACE
//    - clear space in DDR0 and eSRAM destination
//    - create 2 RMACE descriptor sets
//    - send data {data_src_X -> ddr_data_X}
//    - wait fixed time
//    - create 2 RMACE descriptor sets
//    - send data {ddr_data_X -> data_dst_X}
//    - wait fixed time
//    - compare data_src_X and data_dst_X
//    
//    
//    Test duration (RTL): < TODO
//-----------------------------------------------------------------------------

#include <rumboot/ddr_test_lib.h>
#include <rumboot/dma_test_lib.h>

#define data_src_0    ((uint32_t*) 0x00048000)
#define data_dst_0    ((uint32_t*) 0x00068000)
#define data_src_1    ((uint32_t*) 0x00058000)
#define data_dst_1    ((uint32_t*) 0x00078000)
#define ddr_data_0    ((uint32_t*) 0xC0000000)
#define ddr_data_1    ((uint32_t*) 0xC0000800)

#define data_size   1024

#define tx_descriptor_pointer_0  ((uint32_t*) 0x00050000)
#define rx_descriptor_pointer_0  ((uint32_t*) 0x00051000)
#define tx_descriptor_pointer_1  ((uint32_t*) 0x00052000)
#define rx_descriptor_pointer_1  ((uint32_t*) 0x00053000)

#define dma_ddr_write_duration 0x16
#define dma_ddr_read_duration 0x16

//------------------------------------------------------------------------
//  Create reference data array for next transactions
//  1K bytes
//------------------------------------------------------------------------
void create_etalon_array (uint32_t* address, uint32_t size)
{
    for (uint32_t i = 0; i < (size >> 2); i++)
    {
        *address = i + (i<<8) + (i<<16) + (i<<24);
        address++;
    }
}

//------------------------------------------------------------------------
//  Nullify destination space.
//  Useful for repeating writes.
//  1K bytes
//------------------------------------------------------------------------
void clear_destination_space (uint32_t* address, uint32_t size)
{
    for (uint32_t i = 0; i < (size >> 2); i++)
    {
        *address = 0x00000000;
        address++;
    }
}

//------------------------------------------------------------------------
//  Create all necessary descriptors for RMACE write and read channels.
//  Configure RMACE control registers.
//  Start data transceive.
//------------------------------------------------------------------------
void rmace_create_descriptors
(
    uint32_t  MDMA_BASE                ,
    uint32_t* data_src                 ,
    uint32_t* data_dst                 ,
    uint32_t* tx_descriptor_pointer    ,
    uint32_t* rx_descriptor_pointer
)
{
    uint32_t cur_tbl_addr;

    //  Configuration space. It shows that direct transactions must be.
    uint8_t desc_in [8] = {0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x01};
    
    //---------------------------------------------------------
    //  Transmit channel settings
    //---------------------------------------------------------
    //  Configuration space descriptor
    cur_tbl_addr = rmace_set_desc((uint32_t) tx_descriptor_pointer, sizeof(desc_in), (uint32_t) desc_in, 0, 0, 1, 0);
    //  Data space descriptor
    cur_tbl_addr = rmace_set_desc(cur_tbl_addr, data_size, (uint32_t) data_src, 0, 0, 0, 0);
    cur_tbl_addr = rmace_set_desc(cur_tbl_addr, data_size, (uint32_t) data_src, 0, 0, 0, 0);
    cur_tbl_addr = rmace_set_desc(cur_tbl_addr, data_size, (uint32_t) data_src, 0, 0, 0, 0);
    cur_tbl_addr = rmace_set_desc(cur_tbl_addr, data_size, (uint32_t) data_src, 0, 0, 0, 0);
    cur_tbl_addr = rmace_set_desc(cur_tbl_addr, data_size, (uint32_t) data_src, 0, 0, 0, 0);
    cur_tbl_addr = rmace_set_desc(cur_tbl_addr, data_size, (uint32_t) data_src, 0, 0, 0, 0);
    cur_tbl_addr = rmace_set_desc(cur_tbl_addr, data_size, (uint32_t) data_src, 0, 0, 0, 0);
    cur_tbl_addr = rmace_set_desc(cur_tbl_addr, data_size, (uint32_t) data_src, 1, 0, 0, 0);
    //  Stop descriptor
    cur_tbl_addr = rmace_set_desc(cur_tbl_addr, 0, 0, 0, 0, 0, 0);
    
    //---------------------------------------------------------
    //  Setting read AXI-DMA registers
    //---------------------------------------------------------
    (*(volatile uint32_t*)(MDMA_BASE + RMACE_RDMA_SYS_ADDR )) = (uint32_t) tx_descriptor_pointer;
    (*(volatile uint32_t*)(MDMA_BASE + RMACE_RDMA_TBL_SIZE )) = 0x80;
    (*(volatile uint32_t*)(MDMA_BASE + RMACE_RDMA_SETTINGS )) = 0x60000004;
    (*(volatile uint32_t*)(MDMA_BASE + RMACE_ADMA_ARLEN    )) = 0xF;
    (*(volatile uint32_t*)(MDMA_BASE + RMACE_RDMA_DESC_ADDR)) = (uint32_t) tx_descriptor_pointer;
    
    //---------------------------------------------------------
    //  Receive channel settings
    //---------------------------------------------------------
    //  Data space descriptor
    cur_tbl_addr = rmace_set_desc((uint32_t) rx_descriptor_pointer, data_size, (uint32_t) data_dst, 0, 0, 0, 0);
    cur_tbl_addr = rmace_set_desc(cur_tbl_addr, data_size, (uint32_t) data_dst, 0, 0, 0, 0);
    cur_tbl_addr = rmace_set_desc(cur_tbl_addr, data_size, (uint32_t) data_dst, 0, 0, 0, 0);
    cur_tbl_addr = rmace_set_desc(cur_tbl_addr, data_size, (uint32_t) data_dst, 0, 0, 0, 0);
    cur_tbl_addr = rmace_set_desc(cur_tbl_addr, data_size, (uint32_t) data_dst, 0, 0, 0, 0);
    cur_tbl_addr = rmace_set_desc(cur_tbl_addr, data_size, (uint32_t) data_dst, 0, 0, 0, 0);
    cur_tbl_addr = rmace_set_desc(cur_tbl_addr, data_size, (uint32_t) data_dst, 0, 0, 0, 0);
    cur_tbl_addr = rmace_set_desc(cur_tbl_addr, data_size, (uint32_t) data_dst, 1, 0, 0, 0);
    //  Stop descriptor
    cur_tbl_addr = rmace_set_desc(cur_tbl_addr, 0, 0, 0, 0, 0, 0);

    //---------------------------------------------------------
    //  Setting write AXI-DMA registers
    //---------------------------------------------------------
    (*(volatile uint32_t*)(MDMA_BASE + RMACE_WDMA_SYS_ADDR )) = (uint32_t) rx_descriptor_pointer;
    (*(volatile uint32_t*)(MDMA_BASE + RMACE_WDMA_TBL_SIZE )) = 0x80;
    (*(volatile uint32_t*)(MDMA_BASE + RMACE_WDMA_SETTINGS )) = 0x60000004;
    (*(volatile uint32_t*)(MDMA_BASE + RMACE_ADMA_AWLEN    )) = 0xF;
    (*(volatile uint32_t*)(MDMA_BASE + RMACE_WDMA_DESC_ADDR)) = (uint32_t) rx_descriptor_pointer;
    
}

//---------------------------------------------------------------------
//  Start DMA transactions for 0 and 1 channels
//---------------------------------------------------------------------
void rmace_start ()
{
    rgRMACE0_WDMA_SETTINGS |= 0x10000000;
    rgRMACE1_WDMA_SETTINGS |= 0x10000000;
    rgRMACE0_RDMA_SETTINGS |= 0x10000000;
    rgRMACE1_RDMA_SETTINGS |= 0x10000000;
}

void wait_cycle (uint32_t duration)
{
    for (volatile uint32_t i = 0; i < duration; i++)
        ;
}

//------------------------------------------------------------------------
//  Check data transceive results.
//  Reversed order - to check last data first. Otherwise they can come
//    while checking, so, performance will be checked incorrectly.
//------------------------------------------------------------------------
uint32_t Check_transaction_reverse (uint32_t* address_src, uint32_t* address_dst, uint32_t size)
{
    volatile uint32_t* address_src_end = address_src + (size >> 2);
    volatile uint32_t* address_dst_end = address_dst + (size >> 2);
    for (volatile uint32_t i = 0; i < (size >> 2); i += 8)
    {
        if (
            (*(address_src_end - 0) != *(address_dst_end - 0)) ||    //    mass
            (*(address_src_end - 1) != *(address_dst_end - 1)) ||    //    check
            (*(address_src_end - 2) != *(address_dst_end - 2)) ||    //    reduce
            (*(address_src_end - 3) != *(address_dst_end - 3)) ||    //    execution
            (*(address_src_end - 4) != *(address_dst_end - 4)) ||    //    time
            (*(address_src_end - 5) != *(address_dst_end - 5)) ||    //    
            (*(address_src_end - 6) != *(address_dst_end - 6)) ||    //    
            (*(address_src_end - 7) != *(address_dst_end - 7))
            )
            return -1;
        address_src_end -= 8;
        address_dst_end -= 8;
    }
    return 0;
}

int main ()
{
    crg_ddr_init (0x63 ,0x0);
    if (ddr_init (DDR1_BASE) != 0)
        return -1;
    create_etalon_array         (data_src_0, data_size);
    create_etalon_array         (data_src_1, data_size);
    
    clear_destination_space     (data_dst_0, data_size);
    clear_destination_space     (data_dst_1, data_size);
    clear_destination_space     (ddr_data_0, data_size);
    clear_destination_space     (ddr_data_1, data_size);
    
    rmace_create_descriptors
    (
        MDMA0_BASE                 ,
        data_src_0                 ,
        ddr_data_0                 ,
        tx_descriptor_pointer_0    ,
        rx_descriptor_pointer_0
    );
    rmace_create_descriptors
    (
        MDMA1_BASE                 ,
        data_src_1                 ,
        ddr_data_1                 ,
        tx_descriptor_pointer_1    ,
        rx_descriptor_pointer_1
    );
    rmace_start                 ();
    wait_cycle                  (dma_ddr_write_duration);
    
    rmace_create_descriptors
    (
        MDMA0_BASE                 ,
        ddr_data_0                 ,
        data_dst_0                 ,
        tx_descriptor_pointer_0    ,
        rx_descriptor_pointer_0
    );
    rmace_create_descriptors
    (
        MDMA1_BASE                 ,
        ddr_data_1                 ,
        data_dst_1                 ,
        tx_descriptor_pointer_1    ,
        rx_descriptor_pointer_1
    );
    rmace_start                 ();
    wait_cycle                  (dma_ddr_read_duration);
    
    if (Check_transaction_reverse (data_src_0, data_dst_0, data_size) != 0)
        return -1;
    if (Check_transaction_reverse (data_src_1, data_dst_1, data_size) != 0)
        return -1;
    
    return 0;
}

