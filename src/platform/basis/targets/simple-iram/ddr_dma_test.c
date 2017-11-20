
//-----------------------------------------------------------------------------
//  This program is for checking DDR performance.
//  4 RMACE DMA are used.
//  
//  
//  Test includes:
//    - Change DDR CRG frequency to correspond ddr3 1600 data rate
//    - DDR turning On function
//    - create etalon arrays for 4 RMACE
//    - clear space in DDR and eSRAM destination
//    - create 4 RMACE descriptor sets
//    - send data {data_src_X -> ddr_data_X}
//    - wait fixed time
//    - create 4 RMACE descriptor sets
//    - send data {ddr_data_X -> data_dst_X}
//    - wait fixed time
//    - compare data_src_X and data_dst_X
//    - both DDR0 And DDR1 used sequentially
//    
//    Test duration (RTL): < 1500us
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/io.h>
#include <rumboot/platform.h>
#include <rumboot/bootheader.h>


#include <rumboot/ddr_test_lib.h>
#include <rumboot/dma_test_lib.h>

  // Single Array Data Size In Bytes
#define DATA_SIZE   1024

uint32_t data_src_0[DATA_SIZE>>2] __attribute__ ((section("data.esram1")));
uint32_t data_src_1[DATA_SIZE>>2] __attribute__ ((section("data.esram1")));
uint32_t data_src_2[DATA_SIZE>>2] __attribute__ ((section("data.esram1")));
uint32_t data_src_3[DATA_SIZE>>2] __attribute__ ((section("data.esram1")));

#define ddr_data_0_offset           0
#define ddr_data_1_offset           DATA_SIZE
#define ddr_data_2_offset          (DATA_SIZE*2)
#define ddr_data_3_offset          (DATA_SIZE*3)

#define DESCRIPTORS_NUM_NEEDED_FOR_SINGLE_RUN 4

uint64_t tx_descriptor_0[DESCRIPTORS_NUM_NEEDED_FOR_SINGLE_RUN] __attribute__ ((section("data.esram1")));
uint64_t tx_descriptor_1[DESCRIPTORS_NUM_NEEDED_FOR_SINGLE_RUN] __attribute__ ((section("data.esram1")));
uint64_t tx_descriptor_2[DESCRIPTORS_NUM_NEEDED_FOR_SINGLE_RUN] __attribute__ ((section("data.esram1")));
uint64_t tx_descriptor_3[DESCRIPTORS_NUM_NEEDED_FOR_SINGLE_RUN] __attribute__ ((section("data.esram1")));
uint64_t rx_descriptor_0[DESCRIPTORS_NUM_NEEDED_FOR_SINGLE_RUN] __attribute__ ((section("data.esram1")));
uint64_t rx_descriptor_1[DESCRIPTORS_NUM_NEEDED_FOR_SINGLE_RUN] __attribute__ ((section("data.esram1")));
uint64_t rx_descriptor_2[DESCRIPTORS_NUM_NEEDED_FOR_SINGLE_RUN] __attribute__ ((section("data.esram1")));
uint64_t rx_descriptor_3[DESCRIPTORS_NUM_NEEDED_FOR_SINGLE_RUN] __attribute__ ((section("data.esram1")));

uint32_t data_dst_0[DATA_SIZE>>2] __attribute__ ((section("data.esram1")));
uint32_t data_dst_1[DATA_SIZE>>2] __attribute__ ((section("data.esram1")));
uint32_t data_dst_2[DATA_SIZE>>2] __attribute__ ((section("data.esram1")));
uint32_t data_dst_3[DATA_SIZE>>2] __attribute__ ((section("data.esram1")));


 // 1200 ns
#define dma_ddr_write_duration 4
#define dma_ddr_read_duration  4

//------------------------------------------------------------------------
//  Create reference data array for next transactions
//  1K bytes
//------------------------------------------------------------------------
void create_etalon_array (uint32_t* address, uint32_t size)
{
    for (uint32_t i = 0; i < (size >> 2); i++)
    {
        *address = (i<<1) + (i<<17) + 0x00010000;
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
        *address = 0xDEADBEEF;
        address++;
    }
}

//------------------------------------------------------------------------
//  Create all necessary descriptors for RMACE write and read channels.
//  Configure RMACE control registers.
//  Start data transceive.
//------------------------------------------------------------------------
//  Configuration space. It shows that direct transactions must be.
uint8_t desc_in [8] __attribute__ ((aligned(8))) = {0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x01};

void rmace_create_descriptors
(
    uint32_t  MDMA_BASE                ,
    uint32_t* data_src                 ,
    uint32_t* data_dst                 ,
    uint32_t* tx_descriptor_pointer    ,
    uint32_t* rx_descriptor_pointer    ,
    uint32_t  data_size
)
{
    uint32_t cur_tbl_addr;

    
    //---------------------------------------------------------
    //  Transmit channel settings
    //---------------------------------------------------------
    //  Configuration space descriptor
    cur_tbl_addr = rmace_set_desc((uint32_t) tx_descriptor_pointer, sizeof(desc_in), (uint32_t) desc_in, 0, 0, 1, 0);
    //  Data space descriptor
    cur_tbl_addr = rmace_set_desc(cur_tbl_addr, data_size, (uint32_t) data_src, 1, 0, 0, 0);
    //  Stop descriptor
    cur_tbl_addr = rmace_set_desc(cur_tbl_addr, 0, 0, 0, 0, 0, 0);
    
    //---------------------------------------------------------
    //  Setting read AXI-DMA registers
    //---------------------------------------------------------
    iowrite32((uint32_t) tx_descriptor_pointer,MDMA_BASE + RMACE_RDMA_SYS_ADDR);
    iowrite32(0x80,                            MDMA_BASE + RMACE_RDMA_TBL_SIZE);
    iowrite32(0x60000004,                      MDMA_BASE + RMACE_RDMA_SETTINGS);
    iowrite32(0xF,                             MDMA_BASE + RMACE_ADMA_ARLEN);
    iowrite32((uint32_t) tx_descriptor_pointer,MDMA_BASE + RMACE_RDMA_DESC_ADDR);
    
    //---------------------------------------------------------
    //  Receive channel settings
    //---------------------------------------------------------
    //  Data space descriptor
    cur_tbl_addr = rmace_set_desc((uint32_t) rx_descriptor_pointer, data_size, (uint32_t) data_dst, 1, 0, 0, 0);
    //  Stop descriptor
    cur_tbl_addr = rmace_set_desc(cur_tbl_addr, 0, 0, 0, 0, 0, 0);

    //---------------------------------------------------------
    //  Setting write AXI-DMA registers
    //---------------------------------------------------------
    iowrite32((uint32_t) rx_descriptor_pointer,MDMA_BASE + RMACE_WDMA_SYS_ADDR);
    iowrite32(0x80,                            MDMA_BASE + RMACE_WDMA_TBL_SIZE);
    iowrite32(0x60000004,                      MDMA_BASE + RMACE_WDMA_SETTINGS);
    iowrite32(0xF,                             MDMA_BASE + RMACE_ADMA_AWLEN);
    iowrite32((uint32_t) rx_descriptor_pointer,MDMA_BASE + RMACE_WDMA_DESC_ADDR);
    
}

//---------------------------------------------------------------------
//  Start DMA transactions for 0 and 1 channels
//---------------------------------------------------------------------
void rmace_start ()
{
    rgRMACE0_WDMA_SETTINGS |= 0x10000000;
    rgRMACE1_WDMA_SETTINGS |= 0x10000000;
    rgRMACE2_WDMA_SETTINGS |= 0x10000000;
    rgRMACE3_WDMA_SETTINGS |= 0x10000000;
    rgRMACE0_RDMA_SETTINGS |= 0x10000000;
    rgRMACE1_RDMA_SETTINGS |= 0x10000000;
    rgRMACE2_RDMA_SETTINGS |= 0x10000000;
    rgRMACE3_RDMA_SETTINGS |= 0x10000000;
}

// 1 duration -> 298 ns
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
int Check_last_cell (uint32_t* address_src, uint32_t* address_dst, uint32_t size)
{
    volatile uint32_t* address_src_end = address_src + (size >> 2);
    volatile uint32_t* address_dst_end = address_dst + (size >> 2);
  
    if (
            (*(address_src_end - 1) != *(address_dst_end - 1))
      )
      return -1;
    else
      return 0;
}

int Check_transaction_reverse (uint32_t* address_src, uint32_t* address_dst, uint32_t size)
{
    volatile uint32_t* address_src_end = address_src + (size >> 2);
    volatile uint32_t* address_dst_end = address_dst + (size >> 2);
    for (volatile uint32_t i = 0; i < (size >> 2); i += 8)
    {
        if (
            (*(address_src_end - 1) != *(address_dst_end - 1)) ||    //    mass
            (*(address_src_end - 2) != *(address_dst_end - 2)) ||    //    check
            (*(address_src_end - 3) != *(address_dst_end - 3)) ||    //    reduce
            (*(address_src_end - 4) != *(address_dst_end - 4)) ||    //    execution
            (*(address_src_end - 5) != *(address_dst_end - 5)) ||    //    time
            (*(address_src_end - 6) != *(address_dst_end - 6)) ||    //    
            (*(address_src_end - 7) != *(address_dst_end - 7)) ||    //    
            (*(address_src_end - 8) != *(address_dst_end - 8))
            )
            return -1;
        address_src_end -= 8;
        address_dst_end -= 8;
    }
    return 0;
}

int ddr_dma_test(uint32_t emi_base, uint32_t ddrc_base)
{
    uint32_t* ddr_data_0;
    uint32_t* ddr_data_1;
    uint32_t* ddr_data_2;
    uint32_t* ddr_data_3;
    uint32_t* tx_descriptor_pointer_0;
    uint32_t* tx_descriptor_pointer_1;
    uint32_t* tx_descriptor_pointer_2;
    uint32_t* tx_descriptor_pointer_3;
    uint32_t* rx_descriptor_pointer_0;
    uint32_t* rx_descriptor_pointer_1;
    uint32_t* rx_descriptor_pointer_2;
    uint32_t* rx_descriptor_pointer_3;

    ddr_data_0 = (uint32_t*) (emi_base + ddr_data_0_offset);
    ddr_data_1 = (uint32_t*) (emi_base + ddr_data_1_offset);
    ddr_data_2 = (uint32_t*) (emi_base + ddr_data_2_offset);
    ddr_data_3 = (uint32_t*) (emi_base + ddr_data_3_offset);
    
    tx_descriptor_pointer_0 = (uint32_t*) tx_descriptor_0;
    tx_descriptor_pointer_1 = (uint32_t*) tx_descriptor_1;
    tx_descriptor_pointer_2 = (uint32_t*) tx_descriptor_2;
    tx_descriptor_pointer_3 = (uint32_t*) tx_descriptor_3;
    rx_descriptor_pointer_0 = (uint32_t*) rx_descriptor_0;
    rx_descriptor_pointer_1 = (uint32_t*) rx_descriptor_1;
    rx_descriptor_pointer_2 = (uint32_t*) rx_descriptor_2;
    rx_descriptor_pointer_3 = (uint32_t*) rx_descriptor_3;
    
    if (ddr_init (ddrc_base) != 0)
        return -1;
    create_etalon_array         (data_src_0, DATA_SIZE);
    create_etalon_array         (data_src_1, DATA_SIZE);
    create_etalon_array         (data_src_2, DATA_SIZE);
    create_etalon_array         (data_src_3, DATA_SIZE);
    
    // Dont Clear Destination Yet - It Is Boring
//     clear_destination_space     (data_dst_0, DATA_SIZE);
//     clear_destination_space     (data_dst_1, DATA_SIZE);
//     clear_destination_space     (data_dst_2, DATA_SIZE);
//     clear_destination_space     (data_dst_3, DATA_SIZE);
//     clear_destination_space     (ddr_data_0, DATA_SIZE);
//     clear_destination_space     (ddr_data_1, DATA_SIZE);
//     clear_destination_space     (ddr_data_2, DATA_SIZE);
//     clear_destination_space     (ddr_data_3, DATA_SIZE);
    
    rmace_create_descriptors
    (
        MDMA0_BASE                 ,
        data_src_0                 ,
        ddr_data_0                 ,
        tx_descriptor_pointer_0    ,
        rx_descriptor_pointer_0    ,
        DATA_SIZE
    );
    rmace_create_descriptors
    (
        MDMA1_BASE                 ,
        data_src_1                 ,
        ddr_data_1                 ,
        tx_descriptor_pointer_1    ,
        rx_descriptor_pointer_1    ,
        DATA_SIZE
    );
    rmace_create_descriptors
    (
        MDMA2_BASE                 ,
        data_src_2                 ,
        ddr_data_2                 ,
        tx_descriptor_pointer_2    ,
        rx_descriptor_pointer_2    ,
        DATA_SIZE
    );
    rmace_create_descriptors
    (
        MDMA3_BASE                 ,
        data_src_3                 ,
        ddr_data_3                 ,
        tx_descriptor_pointer_3    ,
        rx_descriptor_pointer_3    ,
        DATA_SIZE
    );
    rmace_start                 ();
    wait_cycle                  (dma_ddr_write_duration);
    
    if (Check_last_cell(data_src_3,ddr_data_3,DATA_SIZE) != 0) return -1;
    if (Check_last_cell(data_src_2,ddr_data_2,DATA_SIZE) != 0) return -1;
    if (Check_last_cell(data_src_1,ddr_data_1,DATA_SIZE) != 0) return -1;
    if (Check_last_cell(data_src_0,ddr_data_0,DATA_SIZE) != 0) return -1;
    
    if (Check_transaction_reverse (data_src_0, ddr_data_0, DATA_SIZE) != 0)
        return -1;
    if (Check_transaction_reverse (data_src_1, ddr_data_1, DATA_SIZE) != 0)
        return -1;
    if (Check_transaction_reverse (data_src_2, ddr_data_2, DATA_SIZE) != 0)
        return -1;
    if (Check_transaction_reverse (data_src_3, ddr_data_3, DATA_SIZE) != 0)
        return -1;
    
    rmace_create_descriptors
    (
        MDMA0_BASE                 ,
        ddr_data_0                 ,
        data_dst_0                 ,
        tx_descriptor_pointer_0    ,
        rx_descriptor_pointer_0    ,
        DATA_SIZE
    );
    rmace_create_descriptors
    (
        MDMA1_BASE                 ,
        ddr_data_1                 ,
        data_dst_1                 ,
        tx_descriptor_pointer_1    ,
        rx_descriptor_pointer_1    ,
        DATA_SIZE
    );
    rmace_create_descriptors
    (
        MDMA2_BASE                 ,
        ddr_data_2                 ,
        data_dst_2                 ,
        tx_descriptor_pointer_2    ,
        rx_descriptor_pointer_2    ,
        DATA_SIZE
    );
    rmace_create_descriptors
    (
        MDMA3_BASE                 ,
        ddr_data_3                 ,
        data_dst_3                 ,
        tx_descriptor_pointer_3    ,
        rx_descriptor_pointer_3    ,
        DATA_SIZE
    );
    rmace_start                 ();
    wait_cycle                  (dma_ddr_read_duration);
    
    if (Check_transaction_reverse (data_src_0, data_dst_0, DATA_SIZE) != 0)
        return -1;
    if (Check_transaction_reverse (data_src_1, data_dst_1, DATA_SIZE) != 0)
        return -1;
    if (Check_transaction_reverse (data_src_2, data_dst_2, DATA_SIZE) != 0)
        return -1;
    if (Check_transaction_reverse (data_src_3, data_dst_3, DATA_SIZE) != 0)
        return -1;
    
    return 0;
}

int main ()
{
   int ret;
   
   ret = ddr_dma_test(EMI0_BASE,DDR0_BASE); // FixME!
   if(ret == 0)
    ret = ddr_dma_test(EMI1_BASE,DDR1_BASE); // FixME!
   return ret;
}

