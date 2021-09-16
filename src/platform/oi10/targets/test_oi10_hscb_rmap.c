/*
 * test_oi10_hscb.c
 *
 *  Created on: Jul 23, 2018
 *      Author: r.galiulin
 */


#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <platform/devices.h>
#include <platform/trace.h>
#include <platform/test_assert.h>
#include <platform/interrupts.h>
#include <platform/regs/regs_hscb.h>
#include <rumboot/irq.h>
#include <rumboot/rumboot.h>
#include <rumboot/platform.h>
#include <devices/ugly/hscb.h>
#include <devices/ugly/emi.h>
#include <platform/devices/nor_1636RR4.h>
#include <arch/common_macros.h>
#include <platform/test_event_c.h>
#include <arch/ppc_476fp_mmu.h>
#include <algo/crc8.h>

                          //          MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )
#define TLB_ENTRY_EM_1stGB_NO_EXEC   {MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b0,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )}
#define TLB_ENTRY_EM_2ndGB_NO_EXEC   {MMU_TLB_ENTRY(  0x000,  0x40000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b0,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )}

static const tlb_entry em_anti_x_tlb_entries[] =   {TLB_ENTRY_EM_1stGB_NO_EXEC,
                                                    TLB_ENTRY_EM_2ndGB_NO_EXEC};

#define MAX_ATTEMPTS 10000


#ifndef TX_0_HEAP_NAME
#define TX_0_HEAP_NAME "IM1"
#endif

#ifndef RX_0_HEAP_NAME
#define RX_0_HEAP_NAME "SRAM1"
#endif

#ifndef TX_1_HEAP_NAME
#define TX_1_HEAP_NAME "NOR"
#endif

#ifndef RX_1_HEAP_NAME
#define RX_1_HEAP_NAME "IM1"
#endif

#ifndef TX_2_HEAP_NAME
#define TX_2_HEAP_NAME "IM2"
#endif

#ifndef RX_2_HEAP_NAME
#define RX_2_HEAP_NAME "IM1"
#endif

#ifndef TX_3_HEAP_NAME
#define TX_3_HEAP_NAME "IM2"
#endif
//
//#ifndef RX_3_HEAP_NAME
//#define RX_3_HEAP_NAME "SSRAM"
//#endif

#ifndef TX_4_HEAP_NAME
#define TX_4_HEAP_NAME "IM1"
#endif

#ifndef RX_4_HEAP_NAME
#define RX_4_HEAP_NAME "IM2"
#endif

//#ifndef TX_5_HEAP_NAME
//#define TX_5_HEAP_NAME "SSRAM"
//#endif
//
//#ifndef RX_5_HEAP_NAME
//#define RX_5_HEAP_NAME "IM2"
//#endif

#ifndef DATA_INITIAL_VALUE
#define DATA_INITIAL_VALUE 0x80
#endif

#ifndef INCREMENT_0
#define INCREMENT_0 1
#endif
#ifndef INCREMENT_1
#define INCREMENT_1 -1
#endif
#ifndef INCREMENT_2
#define INCREMENT_2 2
#endif
#ifndef INCREMENT_3
#define INCREMENT_3 -2
#endif
#ifndef INCREMENT_4
#define INCREMENT_4 3
#endif
//#ifndef INCREMENT_5
//#define INCREMENT_5 -3
//#endif
#ifndef INCREMENT_SRAM0
#define INCREMENT_SRAM0 5
#endif
#ifndef INCREMENT_SRAM0_REPLY
#define INCREMENT_SRAM0_REPLY -5
#endif
#ifndef INCREMENT_SRAM1
#define INCREMENT_SRAM1 6
#endif
#ifndef INCREMENT_SRAM1_REPLY
#define INCREMENT_SRAM1_REPLY -6
#endif

#ifndef DATA_SIZE_0
#define DATA_SIZE_0 0xF
#endif
#ifndef DATA_SIZE_1
#define DATA_SIZE_1 0x22
#endif
#ifndef DATA_SIZE_2
#define DATA_SIZE_2 0x19
#endif
#ifndef DATA_SIZE_3
#define DATA_SIZE_3 0x2C
#endif
#ifndef DATA_SIZE_4
#define DATA_SIZE_4 0x34
#endif
//#ifndef DATA_SIZE_5
//#define DATA_SIZE_5 0x33
//#endif
#ifndef DATA_SIZE_NOR
#define DATA_SIZE_NOR 0x30
#endif
#ifndef DATA_SIZE_SRAM0
#define DATA_SIZE_SRAM0 0x22
#endif
#ifndef DATA_SIZE_SRAM1
#define DATA_SIZE_SRAM1 0x19
#endif

#ifndef REPLY_ADDR_SIZE_0
#define REPLY_ADDR_SIZE_0 HSCB_RMAP_PACKET_REPLY_ADDR_12B
#endif


#define DEFAULT_HEAP_ID 1

#ifndef DEFAULT_HEAP_NAME_FOR_REPLY_ADDR
#define DEFAULT_HEAP_NAME_FOR_REPLY_ADDR "IM2"
#endif

#ifndef DEFAULT_HEAP_NAME_FOR_DATA
#define DEFAULT_HEAP_NAME_FOR_DATA "IM2"
#endif

#ifndef DEFAULT_HEAP_NAME_FOR_RMAP_HEADER
#define DEFAULT_HEAP_NAME_FOR_RMAP_HEADER "IM2"
#endif

#ifndef DEFAULT_HEAP_NAME_FOR_RECEIVING
#define DEFAULT_HEAP_NAME_FOR_RECEIVING "IM1"
#endif

#ifndef DEFAULT_INITIATOR_LOGICAL_ADDRESS
#define DEFAULT_INITIATOR_LOGICAL_ADDRESS HSCB_RMAP_DEFAULT_LOGICAL_ADDRESS
#endif

static volatile uint32_t hscb0_status;
static volatile uint32_t hscb1_status;
static volatile uint32_t hscb_under_test_dma_status;
static volatile uint32_t hscb_supplementary_dma_status;
uint32_t sram0_address = 0;
uint32_t sram1_address = 0;

static void handler( int irq, void *arg ) {
    //get interrupt source
    rumboot_printf( "HSCB IRQ arrived  \n" );
//    rumboot_puthex (irq);
    if (irq == HSCB_UNDER_TEST_INT) {
        rumboot_putstring("HSCB_0_IRQ\n");
        rumboot_puthex (hscb_get_status(HSCB_UNDER_TEST_BASE) );
    }
    if (irq == HSCB_UNDER_TEST_DMA_INT) {
        rumboot_putstring("HSCB_0_DMA_IRQ\n");
        hscb_under_test_dma_status = hscb_get_adma_ch_status(HSCB_UNDER_TEST_BASE );
        rumboot_puthex(hscb_under_test_dma_status);
        if (hscb_under_test_dma_status & HSCB_ADMA_CH_STATUS_RDMA_IRQ_mask)
            hscb_under_test_dma_status = hscb_get_rdma_status(HSCB_UNDER_TEST_BASE);
        else if (hscb_under_test_dma_status & HSCB_ADMA_CH_STATUS_WDMA_IRQ_mask)
            hscb_under_test_dma_status = hscb_get_wdma_status(HSCB_UNDER_TEST_BASE);
        rumboot_puthex(hscb_under_test_dma_status);
    }
    if (irq == HSCB_SUPPLEMENTARY_INT) {
        rumboot_putstring("HSCB_1_IRQ\n");
        rumboot_puthex (hscb_get_status(HSCB_SUPPLEMENTARY_BASE));
    }
    if (irq == HSCB_SUPPLEMENTARY_DMA_INT) {
        rumboot_putstring("HSCB_1_DMA_IRQ\n");
        hscb_supplementary_dma_status = hscb_get_adma_ch_status(HSCB_SUPPLEMENTARY_BASE );
        rumboot_puthex(hscb_supplementary_dma_status);
        if (hscb_supplementary_dma_status & HSCB_ADMA_CH_STATUS_RDMA_IRQ_mask)
            hscb_supplementary_dma_status = hscb_get_rdma_status(HSCB_SUPPLEMENTARY_BASE);
        else if (hscb_supplementary_dma_status & HSCB_ADMA_CH_STATUS_WDMA_IRQ_mask)
            hscb_supplementary_dma_status = hscb_get_wdma_status(HSCB_SUPPLEMENTARY_BASE);
        rumboot_puthex(hscb_supplementary_dma_status);
    }
}

struct rumboot_irq_entry * create_irq_handlers()
{
    rumboot_irq_cli();
    struct rumboot_irq_entry *tbl = rumboot_irq_create( NULL );

    rumboot_irq_set_handler( tbl, HSCB_UNDER_TEST_INT,          RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler, ( void* )0 );
    rumboot_irq_set_handler( tbl, HSCB_UNDER_TEST_DMA_INT,      RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler, ( void* )0 );
    rumboot_irq_set_handler( tbl, HSCB_SUPPLEMENTARY_INT,       RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler, ( void* )0 );
    rumboot_irq_set_handler( tbl, HSCB_SUPPLEMENTARY_DMA_INT,   RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, handler, ( void* )0 );

    /* Activate the table */
    rumboot_irq_table_activate( tbl );
    rumboot_irq_enable( HSCB_UNDER_TEST_INT );
    rumboot_irq_enable( HSCB_UNDER_TEST_DMA_INT );
    rumboot_irq_enable( HSCB_SUPPLEMENTARY_INT );
    rumboot_irq_enable( HSCB_SUPPLEMENTARY_DMA_INT );
    rumboot_irq_sei();

     return tbl;
}

void delete_irq_handlers(struct rumboot_irq_entry *tbl)
{
    rumboot_irq_table_activate(NULL);
    rumboot_irq_free(tbl);
}

static inline uint32_t min(uint32_t val1, uint32_t val2)
{
    return (val1 > val2) ? val2 : val1;
}


#ifdef TEST_OI10_HSCB_FULL_TRACING
inline static void print_hscb_descriptor(hscb_packed_descr_struct_t descriptor){
    rumboot_printf("descriptor address: \n%x\nstart_address == \n%x\nlength_attr == \n%x\n",
            &descriptor,hscb_change_endian(descriptor.start_address, true),hscb_change_endian(descriptor.length_attr, true));
}

void print_hscb_descriptors_in_cycle(   hscb_packed_descr_struct_t**    descriptors,
                                        uint32_t*                       descriptor_counts,
                                        uint32_t                        descriptor_tables_count){
    int i,j;
    rumboot_putstring("printing descriptors in cycle");

    for (i = 0; i < descriptor_tables_count; ++i){
        rumboot_puthex(i);
        rumboot_puthex(descriptor_counts[i]);
        rumboot_puthex((uint32_t)(descriptors + i));
        for(j = 0; j < descriptor_counts[i]; ++j){
            rumboot_printf("%x: MEM(0x%x) == 0x%x, MEM(0x%x) == 0x%x\n",
                    ((i << 16) | j),
                    (uint32_t)((*(descriptors + i))+j),
                    *((uint32_t*)((*(descriptors + i)) + j)),
                    (((uint32_t)((*(descriptors + i)) + j)) + 4),
                    *((uint32_t*)(((uint32_t)((*(descriptors + i)) + j)) + 4)));
        }
    }
    rumboot_putstring("end printing descriptors in cycle");

}

void print_hscb_rmap_packets_via_putdump(hscb_rmap_packet_ready_for_transmit_t rmap_packets)
{
    rumboot_putstring("print_hscb_rmap_packets_via_putdump start");
    for(int i = 0; i < rmap_packets.count_areas; ++i)
    {
        rumboot_putdump((uint32_t)rmap_packets.data_areas[i], rmap_packets.data_area_sizes[i]);
        if(((rmap_packets.array_of_descriptors[i].length_attr & HSCB_RD_DESCR_ACT0_mask) >> HSCB_RD_DESCR_ACT0_i)
                == HSCB_ACT0_LAST)
            rumboot_putstring("end of descriptor");
    }
    rumboot_putstring("print_hscb_rmap_packets_via_putdump end");
}
#endif

void free_mem_from_raw_rmap_packets(hscb_rmap_packet_raw_configuration_t* raw_rmap_packets, const uint32_t length)
{
    for(int i = 0; i < length; ++i)
    {
        if(raw_rmap_packets[i].data_chain.array){
            rumboot_free(raw_rmap_packets[i].data_chain.array);
            raw_rmap_packets[i].data_chain.array = NULL;
            raw_rmap_packets[i].data_chain.length = 0;
        }
        if(raw_rmap_packets[i].reply_addr_chain.array)
        {
            rumboot_free(raw_rmap_packets[i].reply_addr_chain.array);
            raw_rmap_packets[i].reply_addr_chain.array = NULL;
            raw_rmap_packets[i].reply_addr_chain.length = 0;
        }
        if(raw_rmap_packets[i].target_addr_chain.array)
        {
            rumboot_free(raw_rmap_packets[i].target_addr_chain.array);
            raw_rmap_packets[i].target_addr_chain.array = NULL;
            raw_rmap_packets[i].target_addr_chain.length = 0;
        }
    }
    rumboot_free(raw_rmap_packets);
}

enum{
    OK = 0,
    UNSUPPORTED_COUNT_PACKETS = (1 << 0),
    UNABLE_TO_ALLOCATE_MEMORY = (1 << 1),
    NO_REPLIES_ARE_EXPECTED = 3,
    REPLY_ADDR_CHAIN_MISMATCH_IN_REPLY = (1 << 2),
    INITIATOR_LOGICAL_ADDRESS_MISMATCH = (1 << 3),
    PROTOCOL_ID_MISMATCH = (1 << 4),
    INSTRUCTION_MISMATCH = (1 << 5),
    STATUS_MISMATCH = (1 << 6),
    TARGET_LOGICAL_ADDRESS_MISMATCH = (1 << 7),
    TRANSACTION_ID_MISMATCH = (1 << 8),
    DATA_LENGTH_MISMATCH = (1 << 9),
    HEADER_CRC_MISMATCH = (1 << 10),
    DATA_CRC_MISMATCH = (1 << 11),
    UNSUPPORTED_INSTRUCITON = (1 << 12),
    SOURCE_AND_DEST_LENGTH_MISMATCH = (1 << 13),
    SOURCE_AND_DEST_DATA_MISMATCH = (1 << 14),
    REPLY_ARRAY_IS_NULL = (1 << 15),
    REPLY_CALCULATED_AND_ACTUAL_LENGTH_MISMATCH = (1 << 16),
    RESERVED_FIELD_MISMATCH = (1 << 17)
}generate_some_raw_rmap_packets_result;

uint32_t RMW(hscb_rmap_packet_raw_configuration_t* raw_rmap_packets, uint32_t index, bool change_endian,  const uint32_t length)
{
    uint32_t reply_addr_length = 0;
    char default_heap_name_for_reply_addr[] = DEFAULT_HEAP_NAME_FOR_REPLY_ADDR;
    char rx_2_heap_name[] = RX_2_HEAP_NAME;
    char tx_2_heap_name[] = TX_2_HEAP_NAME;

    raw_rmap_packets[index].data_chain.length = 6;
    raw_rmap_packets[index].data_chain.array
        = rumboot_malloc_from_named_heap_aligned(tx_2_heap_name,
                raw_rmap_packets[index].data_chain.length, 8);

    raw_rmap_packets[index].addr
        = rumboot_virt_to_dma( rumboot_malloc_from_named_heap_aligned(rx_2_heap_name,
                raw_rmap_packets[index].data_chain.length >> 1, 8));
    raw_rmap_packets[index].ext_addr = 0; //here we have 32bit AXI address space

    reply_addr_length = HSCB_RMAP_PACKET_REPLY_ADDR_12B;
    raw_rmap_packets[index].reply_addr_chain.length = reply_addr_length << 2;
    raw_rmap_packets[index].reply_addr_chain.array
        = rumboot_malloc_from_named_heap_aligned(default_heap_name_for_reply_addr,
                raw_rmap_packets[index].reply_addr_chain.length, 8);

    /*Careful with the following checks in a common case!*/
    if((!raw_rmap_packets[index].reply_addr_chain.array)
         || (!raw_rmap_packets[index].data_chain.array)
         || (!raw_rmap_packets[index].addr))
    {
        free_mem_from_raw_rmap_packets(raw_rmap_packets, length);
        return UNABLE_TO_ALLOCATE_MEMORY;
    }

    rumboot_memfill8(
            (void*)raw_rmap_packets[index].addr,
            raw_rmap_packets[index].data_chain.length >> 1,
            0x33,
            0);

    rumboot_memfill8(
            raw_rmap_packets[index].data_chain.array,
            raw_rmap_packets[index].data_chain.length >> 1,
            0xCC,
            0);

    rumboot_memfill8(
            raw_rmap_packets[index].data_chain.array + (raw_rmap_packets[index].data_chain.length >> 1),
            raw_rmap_packets[index].data_chain.length >> 1,
            0x55,
            0);

    rumboot_memfill8(
            raw_rmap_packets[index].reply_addr_chain.array,
            raw_rmap_packets[index].reply_addr_chain.length,
            0,
            0);

    raw_rmap_packets[index].target_addr_chain.array = NULL;
    raw_rmap_packets[index].target_addr_chain.length = 0;
    raw_rmap_packets[index].target_logical_addr = HSCB_RMAP_DEFAULT_TARGET_LOGICAL_ADDRESS;
    raw_rmap_packets[index].instruction =
            (HSCB_RMAP_COMMAND_RMW_INCREMENTING_ADDRESS     << HSCB_RMAP_PACKET_INSTRUCTION_RMAP_COMMAND_i)
          | (HSCB_RMAP_PACKET_TYPE_COMMAND                  << HSCB_RMAP_PACKET_INSTRUCTION_FIELD_PACKET_TYPE_i)
          | (reply_addr_length    << HSCB_RMAP_PACKET_INSTRUCTION_FIELD_REPLY_ADDR_LEN_i);
    raw_rmap_packets[index].key = HSCB_RMAP_DEFAULT_KEY;
    raw_rmap_packets[index].initiator_logical_addr = 00;
    raw_rmap_packets[index].transaction_id = index;
    raw_rmap_packets[index].change_endian = change_endian;
    raw_rmap_packets[index].expected_reply_status = HSCB_RMAP_REPLY_STATUS_OK;
    return OK;
}

uint32_t RRI(hscb_rmap_packet_raw_configuration_t* raw_rmap_packets, uint32_t index, bool change_endian,  const uint32_t length)
{
    uint32_t reply_addr_length = 0;
//    char default_heap_name_for_reply_addr[] = DEFAULT_HEAP_NAME_FOR_REPLY_ADDR;
//    char rx_1_heap_name[] = RX_1_HEAP_NAME;
    char tx_1_heap_name[] = TX_1_HEAP_NAME;

    raw_rmap_packets[index].data_chain.length = DATA_SIZE_1;
    raw_rmap_packets[index].data_chain.array = NULL;

    raw_rmap_packets[index].addr
        = rumboot_virt_to_dma( rumboot_malloc_from_named_heap_aligned(tx_1_heap_name,
                raw_rmap_packets[index].data_chain.length, 8));
    raw_rmap_packets[index].ext_addr = 0; //here we have 32bit AXI address space

    /*Careful with the following checks in a common case!*/
    if((!raw_rmap_packets[index].addr))
    {
        free_mem_from_raw_rmap_packets(raw_rmap_packets, length);
        return UNABLE_TO_ALLOCATE_MEMORY;
    }

    rumboot_memfill8(
            (void*)raw_rmap_packets[index].addr,
            raw_rmap_packets[index].data_chain.length,
            DATA_INITIAL_VALUE,
            INCREMENT_1);

    reply_addr_length = HSCB_RMAP_PACKET_REPLY_ADDR_0B;
    raw_rmap_packets[index].reply_addr_chain.length = reply_addr_length << 2;
    raw_rmap_packets[index].reply_addr_chain.array = NULL;

    raw_rmap_packets[index].target_addr_chain.array = NULL;
    raw_rmap_packets[index].target_addr_chain.length = 0;
    raw_rmap_packets[index].target_logical_addr = HSCB_RMAP_DEFAULT_TARGET_LOGICAL_ADDRESS;
    raw_rmap_packets[index].instruction =
            (HSCB_RMAP_COMMAND_READ_INCREMENTING_ADDRESS    << HSCB_RMAP_PACKET_INSTRUCTION_RMAP_COMMAND_i)
          | (HSCB_RMAP_PACKET_TYPE_COMMAND                  << HSCB_RMAP_PACKET_INSTRUCTION_FIELD_PACKET_TYPE_i)
          | (reply_addr_length    << HSCB_RMAP_PACKET_INSTRUCTION_FIELD_REPLY_ADDR_LEN_i);
    raw_rmap_packets[index].key = HSCB_RMAP_DEFAULT_KEY;
    raw_rmap_packets[index].initiator_logical_addr = DEFAULT_INITIATOR_LOGICAL_ADDRESS;
    raw_rmap_packets[index].transaction_id = index;
    raw_rmap_packets[index].change_endian = change_endian;
    raw_rmap_packets[index].expected_reply_status = HSCB_RMAP_REPLY_STATUS_OK;
    return OK;
}

uint32_t RRS(hscb_rmap_packet_raw_configuration_t* raw_rmap_packets, uint32_t index, bool change_endian,  const uint32_t length)
{
    uint32_t reply_addr_length = 0;
//    char default_heap_name_for_reply_addr[] = DEFAULT_HEAP_NAME_FOR_REPLY_ADDR;
//    char rx_1_heap_name[] = RX_1_HEAP_NAME;
    char tx_1_heap_name[] = TX_3_HEAP_NAME;

    raw_rmap_packets[index].data_chain.length = DATA_SIZE_3;
    raw_rmap_packets[index].data_chain.array = NULL;

    raw_rmap_packets[index].addr
        = rumboot_virt_to_dma( rumboot_malloc_from_named_heap_aligned(tx_1_heap_name,
                raw_rmap_packets[index].data_chain.length, 8));
    raw_rmap_packets[index].ext_addr = 0; //here we have 32bit AXI address space

    /*Careful with the following checks in a common case!*/
    if((!raw_rmap_packets[index].addr))
    {
        free_mem_from_raw_rmap_packets(raw_rmap_packets, length);
        return UNABLE_TO_ALLOCATE_MEMORY;
    }

    rumboot_memfill8(
            (void*)raw_rmap_packets[index].addr,
            raw_rmap_packets[index].data_chain.length,
            0xda,
            INCREMENT_3);

    reply_addr_length = HSCB_RMAP_PACKET_REPLY_ADDR_0B;
    raw_rmap_packets[index].reply_addr_chain.length = reply_addr_length << 2;
    raw_rmap_packets[index].reply_addr_chain.array = NULL;

    raw_rmap_packets[index].target_addr_chain.array = NULL;
    raw_rmap_packets[index].target_addr_chain.length = 0;
    raw_rmap_packets[index].target_logical_addr = HSCB_RMAP_DEFAULT_TARGET_LOGICAL_ADDRESS;
    raw_rmap_packets[index].instruction =
            (HSCB_RMAP_COMMAND_READ_SINGLE_ADDRESS          << HSCB_RMAP_PACKET_INSTRUCTION_RMAP_COMMAND_i)
          | (HSCB_RMAP_PACKET_TYPE_COMMAND                  << HSCB_RMAP_PACKET_INSTRUCTION_FIELD_PACKET_TYPE_i)
          | (reply_addr_length    << HSCB_RMAP_PACKET_INSTRUCTION_FIELD_REPLY_ADDR_LEN_i);
    raw_rmap_packets[index].key = HSCB_RMAP_DEFAULT_KEY;
    raw_rmap_packets[index].initiator_logical_addr = DEFAULT_INITIATOR_LOGICAL_ADDRESS;
    raw_rmap_packets[index].transaction_id = index;
    raw_rmap_packets[index].change_endian = change_endian;
    raw_rmap_packets[index].expected_reply_status = HSCB_RMAP_REPLY_STATUS_OK;
    return OK;
}

uint32_t W_V_RI(hscb_rmap_packet_raw_configuration_t* raw_rmap_packets, uint32_t index, bool change_endian,  const uint32_t length)
{
    uint32_t reply_addr_length = 0;
    char default_heap_name_for_reply_addr[] = DEFAULT_HEAP_NAME_FOR_REPLY_ADDR;
    char rx_0_heap_name[] = RX_0_HEAP_NAME;
    char tx_0_heap_name[] = TX_0_HEAP_NAME;

    reply_addr_length = HSCB_RMAP_PACKET_REPLY_ADDR_12B;
    raw_rmap_packets[index].reply_addr_chain.length = reply_addr_length << 2;
    raw_rmap_packets[index].reply_addr_chain.array
        = rumboot_malloc_from_named_heap_aligned(default_heap_name_for_reply_addr,
                raw_rmap_packets[index].reply_addr_chain.length, 8);

    raw_rmap_packets[index].data_chain.length = DATA_SIZE_0;
    raw_rmap_packets[index].data_chain.array
        = rumboot_malloc_from_named_heap_aligned(tx_0_heap_name,
                raw_rmap_packets[index].data_chain.length, 8);


    raw_rmap_packets[index].addr
        = rumboot_virt_to_dma( rumboot_malloc_from_named_heap_misaligned(rx_0_heap_name,
                raw_rmap_packets[index].data_chain.length, 8, 5));
    raw_rmap_packets[index].ext_addr = 0; //here we have 32bit AXI address space

    rumboot_memfill8(
            raw_rmap_packets[index].data_chain.array,
            raw_rmap_packets[index].data_chain.length,
            DATA_INITIAL_VALUE,
            INCREMENT_0);

    rumboot_memfill8(
            raw_rmap_packets[index].reply_addr_chain.array,
            raw_rmap_packets[index].reply_addr_chain.length,
            DATA_INITIAL_VALUE,
            INCREMENT_2);

    raw_rmap_packets[index].target_addr_chain.array = NULL;
    raw_rmap_packets[index].target_addr_chain.length = 0;
    raw_rmap_packets[index].target_logical_addr = HSCB_RMAP_DEFAULT_TARGET_LOGICAL_ADDRESS;
    raw_rmap_packets[index].instruction =
            (HSCB_RMAP_COMMAND_WRITE_INCR_ADDR_VERIFY_REPLY          << HSCB_RMAP_PACKET_INSTRUCTION_RMAP_COMMAND_i)
          | (HSCB_RMAP_PACKET_TYPE_COMMAND      << HSCB_RMAP_PACKET_INSTRUCTION_FIELD_PACKET_TYPE_i)
          | (reply_addr_length    << HSCB_RMAP_PACKET_INSTRUCTION_FIELD_REPLY_ADDR_LEN_i);
    raw_rmap_packets[index].key = HSCB_RMAP_DEFAULT_KEY;
    raw_rmap_packets[index].initiator_logical_addr = DEFAULT_INITIATOR_LOGICAL_ADDRESS;
    raw_rmap_packets[index].transaction_id = index;
    raw_rmap_packets[index].change_endian = change_endian;
    raw_rmap_packets[index].expected_reply_status = (raw_rmap_packets[index].data_chain.length > 256)
            ? HSCB_RMAP_REPLY_STATUS_VERIFY_BUFFER_OVERRUN : HSCB_RMAP_REPLY_STATUS_OK;
    return OK;
}

uint32_t WnVnRS(hscb_rmap_packet_raw_configuration_t* raw_rmap_packets, uint32_t index, bool change_endian,  const uint32_t length)
{
    uint32_t reply_addr_length = 0;
    char default_heap_name_for_reply_addr[] = DEFAULT_HEAP_NAME_FOR_REPLY_ADDR;
    char rx_0_heap_name[] = RX_4_HEAP_NAME;
    char tx_0_heap_name[] = TX_4_HEAP_NAME;

    reply_addr_length = HSCB_RMAP_PACKET_REPLY_ADDR_12B;
    raw_rmap_packets[index].reply_addr_chain.length = reply_addr_length << 2;
    raw_rmap_packets[index].reply_addr_chain.array
        = rumboot_malloc_from_named_heap_aligned(default_heap_name_for_reply_addr,
                raw_rmap_packets[index].reply_addr_chain.length, 8);

    raw_rmap_packets[index].data_chain.length = DATA_SIZE_4;
    raw_rmap_packets[index].data_chain.array
        = rumboot_malloc_from_named_heap_aligned(tx_0_heap_name,
                raw_rmap_packets[index].data_chain.length, 8);


    raw_rmap_packets[index].addr
        = rumboot_virt_to_dma( rumboot_malloc_from_named_heap_aligned(rx_0_heap_name,
                raw_rmap_packets[index].data_chain.length, 8));
    raw_rmap_packets[index].ext_addr = 0; //here we have 32bit AXI address space

    rumboot_memfill8(
            raw_rmap_packets[index].data_chain.array,
            raw_rmap_packets[index].data_chain.length,
            DATA_INITIAL_VALUE,
            INCREMENT_3);

    rumboot_memfill8(
            raw_rmap_packets[index].reply_addr_chain.array,
            raw_rmap_packets[index].reply_addr_chain.length,
            DATA_INITIAL_VALUE,
            INCREMENT_4);

    raw_rmap_packets[index].target_addr_chain.array = NULL;
    raw_rmap_packets[index].target_addr_chain.length = 0;
    raw_rmap_packets[index].target_logical_addr = HSCB_RMAP_DEFAULT_TARGET_LOGICAL_ADDRESS;
    raw_rmap_packets[index].instruction =
            (HSCB_RMAP_COMMAND_WRITE_SINGLE_ADDR_DONT_VERIFY_NO_REPLY          << HSCB_RMAP_PACKET_INSTRUCTION_RMAP_COMMAND_i)
          | (HSCB_RMAP_PACKET_TYPE_COMMAND      << HSCB_RMAP_PACKET_INSTRUCTION_FIELD_PACKET_TYPE_i)
          | (reply_addr_length    << HSCB_RMAP_PACKET_INSTRUCTION_FIELD_REPLY_ADDR_LEN_i);
    raw_rmap_packets[index].key = HSCB_RMAP_DEFAULT_KEY;
    raw_rmap_packets[index].initiator_logical_addr = DEFAULT_INITIATOR_LOGICAL_ADDRESS;
    raw_rmap_packets[index].transaction_id = index;
    raw_rmap_packets[index].change_endian = change_endian;
    raw_rmap_packets[index].expected_reply_status = HSCB_RMAP_REPLY_STATUS_OK;
    return OK;
}

uint32_t RRS_nor(hscb_rmap_packet_raw_configuration_t* raw_rmap_packets, uint32_t index, bool change_endian,  const uint32_t length)
{
    uint32_t reply_addr_length = 0;
//    char default_heap_name_for_reply_addr[] = DEFAULT_HEAP_NAME_FOR_REPLY_ADDR;
//    char rx_1_heap_name[] = RX_1_HEAP_NAME;
    char tx_1_heap_name[] = "NOR";

    raw_rmap_packets[index].data_chain.length = DATA_SIZE_NOR;
    raw_rmap_packets[index].data_chain.array = NULL;

    raw_rmap_packets[index].addr
        = rumboot_virt_to_dma( rumboot_malloc_from_named_heap_aligned(tx_1_heap_name,
                raw_rmap_packets[index].data_chain.length, 8));
    raw_rmap_packets[index].ext_addr = 0; //here we have 32bit AXI address space

    /*Careful with the following checks in a common case!*/
    if((!raw_rmap_packets[index].addr))
    {
        free_mem_from_raw_rmap_packets(raw_rmap_packets, length);
        return UNABLE_TO_ALLOCATE_MEMORY;
    }

    rumboot_memfill8(
            (void*)raw_rmap_packets[index].addr,
            raw_rmap_packets[index].data_chain.length,
            0xda,
            INCREMENT_3);

    reply_addr_length = HSCB_RMAP_PACKET_REPLY_ADDR_0B;
    raw_rmap_packets[index].reply_addr_chain.length = reply_addr_length << 2;
    raw_rmap_packets[index].reply_addr_chain.array = NULL;

    raw_rmap_packets[index].target_addr_chain.array = NULL;
    raw_rmap_packets[index].target_addr_chain.length = 0;
    raw_rmap_packets[index].target_logical_addr = HSCB_RMAP_DEFAULT_TARGET_LOGICAL_ADDRESS;
    raw_rmap_packets[index].instruction =
            (HSCB_RMAP_COMMAND_READ_SINGLE_ADDRESS          << HSCB_RMAP_PACKET_INSTRUCTION_RMAP_COMMAND_i)
          | (HSCB_RMAP_PACKET_TYPE_COMMAND                  << HSCB_RMAP_PACKET_INSTRUCTION_FIELD_PACKET_TYPE_i)
          | (reply_addr_length    << HSCB_RMAP_PACKET_INSTRUCTION_FIELD_REPLY_ADDR_LEN_i);
    raw_rmap_packets[index].key = HSCB_RMAP_DEFAULT_KEY;
    raw_rmap_packets[index].initiator_logical_addr = DEFAULT_INITIATOR_LOGICAL_ADDRESS;
    raw_rmap_packets[index].transaction_id = index;
    raw_rmap_packets[index].change_endian = change_endian;
    raw_rmap_packets[index].expected_reply_status = HSCB_RMAP_REPLY_STATUS_OK;
    return OK;
}

uint32_t W_V_RI_sram0(hscb_rmap_packet_raw_configuration_t* raw_rmap_packets, uint32_t index, bool change_endian,  const uint32_t length)
{
    uint32_t reply_addr_length = 0;
    char default_heap_name_for_reply_addr[] = DEFAULT_HEAP_NAME_FOR_REPLY_ADDR;
    char rx_0_heap_name[] = "SRAM0";
    char tx_0_heap_name[] = "IM1";

    reply_addr_length = HSCB_RMAP_PACKET_REPLY_ADDR_12B;
    raw_rmap_packets[index].reply_addr_chain.length = reply_addr_length << 2;
    raw_rmap_packets[index].reply_addr_chain.array
        = rumboot_malloc_from_named_heap_aligned(default_heap_name_for_reply_addr,
                raw_rmap_packets[index].reply_addr_chain.length, 8);

    raw_rmap_packets[index].data_chain.length = DATA_SIZE_SRAM0;
    raw_rmap_packets[index].data_chain.array
        = rumboot_malloc_from_named_heap_aligned(tx_0_heap_name,
                raw_rmap_packets[index].data_chain.length, 8);


    sram0_address = rumboot_virt_to_dma( rumboot_malloc_from_named_heap_misaligned(rx_0_heap_name,
                raw_rmap_packets[index].data_chain.length, 8, 5));
    raw_rmap_packets[index].addr = sram0_address;
    raw_rmap_packets[index].ext_addr = 0; //here we have 32bit AXI address space

    rumboot_memfill8(
            raw_rmap_packets[index].data_chain.array,
            raw_rmap_packets[index].data_chain.length,
            DATA_INITIAL_VALUE,
            INCREMENT_SRAM0);

    rumboot_memfill8(
            raw_rmap_packets[index].reply_addr_chain.array,
            raw_rmap_packets[index].reply_addr_chain.length,
            DATA_INITIAL_VALUE,
            INCREMENT_SRAM0_REPLY);

    raw_rmap_packets[index].target_addr_chain.array = NULL;
    raw_rmap_packets[index].target_addr_chain.length = 0;
    raw_rmap_packets[index].target_logical_addr = HSCB_RMAP_DEFAULT_TARGET_LOGICAL_ADDRESS;
    raw_rmap_packets[index].instruction =
            (HSCB_RMAP_COMMAND_WRITE_INCR_ADDR_VERIFY_REPLY          << HSCB_RMAP_PACKET_INSTRUCTION_RMAP_COMMAND_i)
          | (HSCB_RMAP_PACKET_TYPE_COMMAND      << HSCB_RMAP_PACKET_INSTRUCTION_FIELD_PACKET_TYPE_i)
          | (reply_addr_length    << HSCB_RMAP_PACKET_INSTRUCTION_FIELD_REPLY_ADDR_LEN_i);
    raw_rmap_packets[index].key = HSCB_RMAP_DEFAULT_KEY;
    raw_rmap_packets[index].initiator_logical_addr = DEFAULT_INITIATOR_LOGICAL_ADDRESS;
    raw_rmap_packets[index].transaction_id = index;
    raw_rmap_packets[index].change_endian = change_endian;
    raw_rmap_packets[index].expected_reply_status = (raw_rmap_packets[index].data_chain.length > 256)
            ? HSCB_RMAP_REPLY_STATUS_VERIFY_BUFFER_OVERRUN : HSCB_RMAP_REPLY_STATUS_OK;
    return OK;
}

uint32_t RRI_sram0(hscb_rmap_packet_raw_configuration_t* raw_rmap_packets, uint32_t index, bool change_endian,  const uint32_t length)
{
    uint32_t reply_addr_length = 0;
//    char default_heap_name_for_reply_addr[] = DEFAULT_HEAP_NAME_FOR_REPLY_ADDR;
//    char rx_1_heap_name[] = RX_1_HEAP_NAME;
//    char tx_1_heap_name[] = "SRAM0";

    TEST_ASSERT((sram0_address != 0),"RRI_sram0 must be called after W_V_RI_sram0");

    raw_rmap_packets[index].data_chain.length = DATA_SIZE_SRAM0;
    raw_rmap_packets[index].data_chain.array = NULL;

    raw_rmap_packets[index].addr = sram0_address;
//        = rumboot_virt_to_dma( rumboot_malloc_from_named_heap_aligned(tx_1_heap_name,
//                raw_rmap_packets[index].data_chain.length, 8));
    raw_rmap_packets[index].ext_addr = 0; //here we have 32bit AXI address space

    /*Careful with the following checks in a common case!*/
    if((!raw_rmap_packets[index].addr))
    {
        free_mem_from_raw_rmap_packets(raw_rmap_packets, length);
        return UNABLE_TO_ALLOCATE_MEMORY;
    }
//
//    rumboot_memfill8(
//            (void*)raw_rmap_packets[index].addr,
//            raw_rmap_packets[index].data_chain.length,
//            DATA_INITIAL_VALUE,
//            INCREMENT_1);

    reply_addr_length = HSCB_RMAP_PACKET_REPLY_ADDR_0B;
    raw_rmap_packets[index].reply_addr_chain.length = reply_addr_length << 2;
    raw_rmap_packets[index].reply_addr_chain.array = NULL;

    raw_rmap_packets[index].target_addr_chain.array = NULL;
    raw_rmap_packets[index].target_addr_chain.length = 0;
    raw_rmap_packets[index].target_logical_addr = HSCB_RMAP_DEFAULT_TARGET_LOGICAL_ADDRESS;
    raw_rmap_packets[index].instruction =
            (HSCB_RMAP_COMMAND_READ_INCREMENTING_ADDRESS    << HSCB_RMAP_PACKET_INSTRUCTION_RMAP_COMMAND_i)
          | (HSCB_RMAP_PACKET_TYPE_COMMAND                  << HSCB_RMAP_PACKET_INSTRUCTION_FIELD_PACKET_TYPE_i)
          | (reply_addr_length    << HSCB_RMAP_PACKET_INSTRUCTION_FIELD_REPLY_ADDR_LEN_i);
    raw_rmap_packets[index].key = HSCB_RMAP_DEFAULT_KEY;
    raw_rmap_packets[index].initiator_logical_addr = DEFAULT_INITIATOR_LOGICAL_ADDRESS;
    raw_rmap_packets[index].transaction_id = index;
    raw_rmap_packets[index].change_endian = change_endian;
    raw_rmap_packets[index].expected_reply_status = HSCB_RMAP_REPLY_STATUS_OK;
    return OK;
}

uint32_t WnV_RI_sram1(hscb_rmap_packet_raw_configuration_t* raw_rmap_packets, uint32_t index, bool change_endian,  const uint32_t length)
{
    uint32_t reply_addr_length = 0;
    char default_heap_name_for_reply_addr[] = DEFAULT_HEAP_NAME_FOR_REPLY_ADDR;
    char rx_0_heap_name[] = "SRAM1";
    char tx_0_heap_name[] = "IM2";

    reply_addr_length = HSCB_RMAP_PACKET_REPLY_ADDR_12B;
    raw_rmap_packets[index].reply_addr_chain.length = reply_addr_length << 2;
    raw_rmap_packets[index].reply_addr_chain.array
        = rumboot_malloc_from_named_heap_aligned(default_heap_name_for_reply_addr,
                raw_rmap_packets[index].reply_addr_chain.length, 8);

    raw_rmap_packets[index].data_chain.length = DATA_SIZE_SRAM1;
    raw_rmap_packets[index].data_chain.array
        = rumboot_malloc_from_named_heap_aligned(tx_0_heap_name,
                raw_rmap_packets[index].data_chain.length, 8);



    sram1_address = rumboot_virt_to_dma( rumboot_malloc_from_named_heap_misaligned(rx_0_heap_name,
                raw_rmap_packets[index].data_chain.length, 8, 1));
    raw_rmap_packets[index].addr = sram1_address;
    raw_rmap_packets[index].ext_addr = 0; //here we have 32bit AXI address space

    rumboot_memfill8(
            raw_rmap_packets[index].data_chain.array,
            raw_rmap_packets[index].data_chain.length,
            DATA_INITIAL_VALUE,
            INCREMENT_SRAM1);

    rumboot_memfill8(
            raw_rmap_packets[index].reply_addr_chain.array,
            raw_rmap_packets[index].reply_addr_chain.length,
            DATA_INITIAL_VALUE,
            INCREMENT_SRAM1_REPLY);

    raw_rmap_packets[index].target_addr_chain.array = NULL;
    raw_rmap_packets[index].target_addr_chain.length = 0;
    raw_rmap_packets[index].target_logical_addr = HSCB_RMAP_DEFAULT_TARGET_LOGICAL_ADDRESS;
    raw_rmap_packets[index].instruction =
            (HSCB_RMAP_COMMAND_WRITE_INCR_ADDR_DONT_VERIFY_REPLY          << HSCB_RMAP_PACKET_INSTRUCTION_RMAP_COMMAND_i)
          | (HSCB_RMAP_PACKET_TYPE_COMMAND      << HSCB_RMAP_PACKET_INSTRUCTION_FIELD_PACKET_TYPE_i)
          | (reply_addr_length    << HSCB_RMAP_PACKET_INSTRUCTION_FIELD_REPLY_ADDR_LEN_i);
    raw_rmap_packets[index].key = HSCB_RMAP_DEFAULT_KEY;
    raw_rmap_packets[index].initiator_logical_addr = DEFAULT_INITIATOR_LOGICAL_ADDRESS;
    raw_rmap_packets[index].transaction_id = index;
    raw_rmap_packets[index].change_endian = change_endian;
    raw_rmap_packets[index].expected_reply_status = HSCB_RMAP_REPLY_STATUS_OK;
    return OK;
}

uint32_t RRI_sram1(hscb_rmap_packet_raw_configuration_t* raw_rmap_packets, uint32_t index, bool change_endian,  const uint32_t length)
{
    uint32_t reply_addr_length = 0;
//    char default_heap_name_for_reply_addr[] = DEFAULT_HEAP_NAME_FOR_REPLY_ADDR;
//    char rx_1_heap_name[] = RX_1_HEAP_NAME;
//    char tx_1_heap_name[] = "SRAM1";
    TEST_ASSERT((sram1_address != 0),"RRI_sram1 must be called after WnV_RI_sram1");


    raw_rmap_packets[index].data_chain.length = DATA_SIZE_SRAM1;
    raw_rmap_packets[index].data_chain.array = NULL;

    raw_rmap_packets[index].addr = sram1_address;
    raw_rmap_packets[index].ext_addr = 0; //here we have 32bit AXI address space

    /*Careful with the following checks in a common case!*/
    if((!raw_rmap_packets[index].addr))
    {
        free_mem_from_raw_rmap_packets(raw_rmap_packets, length);
        return UNABLE_TO_ALLOCATE_MEMORY;
    }
//
//    rumboot_memfill8(
//            (void*)raw_rmap_packets[index].addr,
//            raw_rmap_packets[index].data_chain.length,
//            DATA_INITIAL_VALUE,
//            INCREMENT_1);

    reply_addr_length = HSCB_RMAP_PACKET_REPLY_ADDR_0B;
    raw_rmap_packets[index].reply_addr_chain.length = reply_addr_length << 2;
    raw_rmap_packets[index].reply_addr_chain.array = NULL;

    raw_rmap_packets[index].target_addr_chain.array = NULL;
    raw_rmap_packets[index].target_addr_chain.length = 0;
    raw_rmap_packets[index].target_logical_addr = HSCB_RMAP_DEFAULT_TARGET_LOGICAL_ADDRESS;
    raw_rmap_packets[index].instruction =
            (HSCB_RMAP_COMMAND_READ_INCREMENTING_ADDRESS    << HSCB_RMAP_PACKET_INSTRUCTION_RMAP_COMMAND_i)
          | (HSCB_RMAP_PACKET_TYPE_COMMAND                  << HSCB_RMAP_PACKET_INSTRUCTION_FIELD_PACKET_TYPE_i)
          | (reply_addr_length    << HSCB_RMAP_PACKET_INSTRUCTION_FIELD_REPLY_ADDR_LEN_i);
    raw_rmap_packets[index].key = HSCB_RMAP_DEFAULT_KEY;
    raw_rmap_packets[index].initiator_logical_addr = DEFAULT_INITIATOR_LOGICAL_ADDRESS;
    raw_rmap_packets[index].transaction_id = index;
    raw_rmap_packets[index].change_endian = change_endian;
    raw_rmap_packets[index].expected_reply_status = HSCB_RMAP_REPLY_STATUS_OK;
    return OK;
}

uint32_t (*generate_RMAP_packet_functions[]) (hscb_rmap_packet_raw_configuration_t* raw_rmap_packets, uint32_t index, bool change_endian,  const uint32_t length)
        ={
#ifdef RMAP_PACKET_LIST
                RMAP_PACKET_LIST
#else
                W_V_RI_sram0,
                RRI_sram0,
                WnV_RI_sram1,
                RRI_sram1,
#ifdef RUMBOOT_PRINTF_ACCEL
                RRS_nor,
#endif
                RRS,
                WnVnRS,
                W_V_RI,
                RMW,
                RRI,
#endif
        };

#ifndef COUNT_PACKETS
#define COUNT_PACKETS sizeof(generate_RMAP_packet_functions)/sizeof(*generate_RMAP_packet_functions)
#endif

uint32_t generate_some_raw_rmap_packets(hscb_rmap_packet_raw_configuration_t* raw_rmap_packets, const uint32_t length)
{
    bool change_endian = true;
    uint32_t result = 0;
    for(uint32_t i = 0; i < sizeof(generate_RMAP_packet_functions)/sizeof(*generate_RMAP_packet_functions); ++i  )
        {
            result |= generate_RMAP_packet_functions[i](raw_rmap_packets, i, change_endian, length);
            if (result != OK)
                return result;

        }
    return OK;
}

uint32_t count_of_necessary_TX_descriptors(
        hscb_rmap_packet_raw_configuration_t* raw_rmap_packets,
        uint32_t count_packets)
{
    uint32_t counter = 0;
    for(int i = 0; i < count_packets; ++i)
    {
        counter += 2 //two mandatory chains of fields in RMAP packet header
                + ((raw_rmap_packets[i].target_addr_chain.length > 0) ? 1 : 0)//Target Addr chain is supplied
                + ((raw_rmap_packets[i].reply_addr_chain.length > 0) ? 1 : 0)//Reply Addr chain is supplied
                + (((raw_rmap_packets[i].data_chain.length > 0) &&
                        (raw_rmap_packets[i].instruction
                    & (HSCB_RMAP_PACKET_INSTRUCTION_FIELD_WRITE_mask
                     | HSCB_RMAP_PACKET_INSTRUCTION_FIELD_VERIFY_DATA_mask))) ? 2 : 0);// Data with CRC is supplied
    }
    return ++counter;//Reserve 1 place for terminating descriptor
}

void free_mem_from_ready_rmap_packets(hscb_rmap_packet_ready_for_transmit_t* ready_rmap_packets)
{
    rumboot_printf("free_mem_from_ready_rmap_packets start\n");
    for(int i = 0; i < ready_rmap_packets->count_areas; ++i)
    {
        if(ready_rmap_packets->data_areas[i])
            rumboot_free(ready_rmap_packets->data_areas[i]);
    }
    if(ready_rmap_packets->data_areas)
        rumboot_free(ready_rmap_packets->data_areas);
    if(ready_rmap_packets->array_of_descriptors)
        rumboot_free(ready_rmap_packets->array_of_descriptors);
    if(ready_rmap_packets->data_area_sizes)
        rumboot_free(ready_rmap_packets->data_area_sizes);
    rumboot_printf("free_mem_from_ready_rmap_packets end\n");
}

uint32_t packing_for_rmap(
        hscb_rmap_packet_ready_for_transmit_t* ready_rmap_packets,
        hscb_rmap_packet_raw_configuration_t* raw_rmap_packets,
        uint32_t count_packets)
{
    uint32_t result = OK;
    char default_heap_name_for_rmap_header[] = DEFAULT_HEAP_NAME_FOR_RMAP_HEADER;
    rumboot_printf("packing_for_rmap start\n");
    uint32_t count_descriptors = count_of_necessary_TX_descriptors(raw_rmap_packets, count_packets);

    ready_rmap_packets->array_of_descriptors = rumboot_malloc_from_heap_aligned(DEFAULT_HEAP_ID,
            sizeof(hscb_packed_descr_struct_t) * count_descriptors, 8);
    if(!ready_rmap_packets->array_of_descriptors)
        return UNABLE_TO_ALLOCATE_MEMORY;

    ready_rmap_packets->data_areas = rumboot_malloc_from_heap_aligned(DEFAULT_HEAP_ID,
            sizeof(ready_rmap_packets->data_areas) * count_descriptors,8);
    if(!ready_rmap_packets->data_areas)
    {
        rumboot_free(ready_rmap_packets->array_of_descriptors);
        ready_rmap_packets->array_of_descriptors = NULL;
        return UNABLE_TO_ALLOCATE_MEMORY;
    }

    ready_rmap_packets->data_area_sizes = rumboot_malloc_from_heap_aligned(DEFAULT_HEAP_ID,
            sizeof(*ready_rmap_packets->data_area_sizes) * count_descriptors, 8);

    if(!ready_rmap_packets->data_area_sizes)
    {
        rumboot_free(ready_rmap_packets->array_of_descriptors);
        ready_rmap_packets->array_of_descriptors = NULL;
        rumboot_free(ready_rmap_packets->data_areas);
        ready_rmap_packets->data_areas = NULL;
        return UNABLE_TO_ALLOCATE_MEMORY;
    }

    for(int i = 0, counter_of_fields = 0; i < count_packets; ++i)
    {
        /*Target SW Address*/
        if(raw_rmap_packets[i].target_addr_chain.length &&
                raw_rmap_packets[i].target_addr_chain.array)
            ++counter_of_fields;

        /*first mandatory part of RMAP packet header*/
        ready_rmap_packets->data_area_sizes[counter_of_fields] = 4;
        ready_rmap_packets->data_areas[counter_of_fields]
              = rumboot_malloc_from_named_heap_aligned(default_heap_name_for_rmap_header,
                      ready_rmap_packets->data_area_sizes[counter_of_fields], 8);
        if(!ready_rmap_packets->data_areas[counter_of_fields])
        {
            rumboot_free(ready_rmap_packets->data_areas[counter_of_fields]);
            ready_rmap_packets->data_areas[counter_of_fields] = NULL;
            free_mem_from_ready_rmap_packets(ready_rmap_packets);
            return UNABLE_TO_ALLOCATE_MEMORY;
        }
        ++counter_of_fields;

        /*reply address chain*/
        if(raw_rmap_packets[i].reply_addr_chain.length &&
                raw_rmap_packets[i].reply_addr_chain.array)
            ++counter_of_fields;

        /*second mandatory part of RMAP packet header*/
        ready_rmap_packets->data_area_sizes[counter_of_fields] = 12;
        ready_rmap_packets->data_areas[counter_of_fields]
              = rumboot_malloc_from_named_heap_aligned(default_heap_name_for_rmap_header,
                      ready_rmap_packets->data_area_sizes[counter_of_fields], 8);
        if(!ready_rmap_packets->data_areas[counter_of_fields])
        {
            rumboot_free(ready_rmap_packets->data_areas[counter_of_fields]);
            ready_rmap_packets->data_areas[counter_of_fields] = NULL;
            rumboot_free(ready_rmap_packets->data_areas[counter_of_fields - 2]);
            ready_rmap_packets->data_areas[counter_of_fields - 2] = NULL;
            free_mem_from_ready_rmap_packets(ready_rmap_packets);
            return UNABLE_TO_ALLOCATE_MEMORY;
        }
        ++counter_of_fields;

        /*Data CRC*/
        if(raw_rmap_packets[i].data_chain.length &&
                raw_rmap_packets[i].data_chain.array)
        {
            ++counter_of_fields;
            ready_rmap_packets->data_area_sizes[counter_of_fields] = 1;
            ready_rmap_packets->data_areas[counter_of_fields]
                  = rumboot_malloc_from_named_heap_aligned(default_heap_name_for_rmap_header,
                          ready_rmap_packets->data_area_sizes[counter_of_fields], 8);
            if(!ready_rmap_packets->data_areas[counter_of_fields])
            {
                rumboot_free(ready_rmap_packets->data_areas[counter_of_fields]);
                ready_rmap_packets->data_areas[counter_of_fields] = NULL;
                rumboot_free(ready_rmap_packets->data_areas[counter_of_fields - 1]);
                ready_rmap_packets->data_areas[counter_of_fields - 1] = NULL;
                rumboot_free(ready_rmap_packets->data_areas[counter_of_fields - 3]);
                ready_rmap_packets->data_areas[counter_of_fields - 3] = NULL;
                free_mem_from_ready_rmap_packets(ready_rmap_packets);
                return UNABLE_TO_ALLOCATE_MEMORY;
            }
            ++counter_of_fields;
        }
//TODO: TEST_ASSERT on non-incrementing command alignments
        result = hscb_prepare_rmap_packet(raw_rmap_packets[i], ready_rmap_packets);
        switch(result)
        {
            case PREPARE_RMAP_PACKET_OK: rumboot_printf("hscb_prepare_rmap_packet: OK\n"); break;
            case PREPARE_RMAP_PACKET_ADDR_CHAIN_IS_NULL:
            {
                free_mem_from_ready_rmap_packets(ready_rmap_packets);
                rumboot_printf("packing_for_rmap: target SpaceWire addresses array is NULL, but its length is not zero\n");
                return result;
            } break;
            case PREPARE_RMAP_PACKET_REPLY_ADDR_CHAIN_IS_NULL:
            {
                free_mem_from_ready_rmap_packets(ready_rmap_packets);
                rumboot_printf("packing_for_rmap: reply SpaceWire addresses array is NULL, but its length is not zero\n");
                return result;
            } break;
            case PREPARE_RMAP_PACKET_REPLY_ADDR_CHAIN_TOO_LONG:
            {
                free_mem_from_ready_rmap_packets(ready_rmap_packets);
                rumboot_printf("packing_for_rmap: reply SpaceWire addresses array is NULL, but its length is not zero\n");
                return result;
            } break;
            case PREPARE_RMAP_PACKET_REPLY_ADDR_CHAIN_WRONG_SIZE:
            {
                free_mem_from_ready_rmap_packets(ready_rmap_packets);
                rumboot_printf("packing_for_rmap: reply SpaceWire addresses array has unsupported length\n");
                return result;
            } break;
            case PREPARE_RMAP_PACKET_DATA_LEN_OVERFLOW:
            {
                free_mem_from_ready_rmap_packets(ready_rmap_packets);
                rumboot_printf("packing_for_rmap: SpaceWire RMAP data length is above upper limit of 16MB-1\n");
                return result;
            } break;
            case PREPARE_RMAP_PACKET_DATA_CHAIN_IS_NULL:
            {
                free_mem_from_ready_rmap_packets(ready_rmap_packets);
                rumboot_printf("packing_for_rmap: SpaceWire RMAP data field is NULL but its length is not zero for a Write or RMW command\n");
                return result;
            } break;
            default:
            {
                free_mem_from_ready_rmap_packets(ready_rmap_packets);
                rumboot_printf("packing_for_rmap: unexpected return value == %x\n", result);
                return result;
            }
        }
    }
    rumboot_printf("packing_for_rmap end\n");

    return result;
}

uint32_t count_of_necessary_RX_descriptors(
        hscb_rmap_packet_raw_configuration_t* raw_rmap_packets,
        uint32_t count_packets)
{
    uint32_t counter = 0;
    for(int i = 0; i < count_packets; ++i)
    {
        counter += (raw_rmap_packets[i].instruction
                    & HSCB_RMAP_PACKET_INSTRUCTION_FIELD_W_REPLY_mask) ? 1 : 0;// Data with CRC is supplied
    }
    return ((counter) ? (counter + 1) : (counter));//Reserve 1 place for terminating descriptor
}

uint32_t prepare_receiving_areas(
        hscb_rmap_packet_ready_for_transmit_t* receiving_rmap_packets,
        hscb_rmap_packet_raw_configuration_t* raw_rmap_packets,
        uint32_t count_packets)
{
    uint32_t result = OK;
    char default_heap_for_receiving[] = DEFAULT_HEAP_NAME_FOR_RECEIVING;
    hscb_descr_struct_t descr;
    rumboot_printf("prepare_receiving_areas start\n");
    uint32_t count_descriptors = count_of_necessary_RX_descriptors(raw_rmap_packets, count_packets);
#ifdef TEST_OI10_HSCB_FULL_TRACING
    rumboot_printf("expected count of descriptors == 0x%x\n", count_descriptors);
#endif
    if(count_descriptors == 0)
        return NO_REPLIES_ARE_EXPECTED;

    receiving_rmap_packets->array_of_descriptors = rumboot_malloc_from_heap_aligned(DEFAULT_HEAP_ID,
            sizeof(hscb_packed_descr_struct_t) * count_descriptors, 8);
    if(!receiving_rmap_packets->array_of_descriptors)
        return UNABLE_TO_ALLOCATE_MEMORY;

    receiving_rmap_packets->data_areas = rumboot_malloc_from_heap_aligned(DEFAULT_HEAP_ID,
            sizeof(receiving_rmap_packets->data_areas) * count_descriptors, 8);
    if(!receiving_rmap_packets->data_areas)
    {
        rumboot_free(receiving_rmap_packets->array_of_descriptors);
        return UNABLE_TO_ALLOCATE_MEMORY;
    }

    receiving_rmap_packets->data_area_sizes = rumboot_malloc_from_heap_aligned(DEFAULT_HEAP_ID,
            sizeof(*receiving_rmap_packets->data_area_sizes) * count_descriptors, 8);

    if(!receiving_rmap_packets->data_area_sizes)
    {
        rumboot_free(receiving_rmap_packets->array_of_descriptors);
        rumboot_free(receiving_rmap_packets->data_areas);
        return UNABLE_TO_ALLOCATE_MEMORY;
    }

    receiving_rmap_packets->count_areas = 0;

    for(uint32_t i = 0; i < count_packets; ++i)
    {
        if(!(raw_rmap_packets[i].instruction
                    & HSCB_RMAP_PACKET_INSTRUCTION_FIELD_W_REPLY_mask))
            continue;

        receiving_rmap_packets->data_area_sizes[receiving_rmap_packets->count_areas] =
                  ((raw_rmap_packets[i].instruction
                    & HSCB_RMAP_PACKET_INSTRUCTION_FIELD_WRITE_mask)
                    ? 8  //write reply has 8 bytes in mandatory header
                    : 12 //read and RMW replies have 12 bytes in mandatory header
                      + ((raw_rmap_packets[i].data_chain.length >>
                              ((((raw_rmap_packets[i].instruction
                              & HSCB_RMAP_PACKET_INSTRUCTION_RMAP_COMMAND_mask)
                                      >> HSCB_RMAP_PACKET_INSTRUCTION_RMAP_COMMAND_i)
                                      == HSCB_RMAP_COMMAND_RMW_INCREMENTING_ADDRESS)
                                      ? 1  //RMW command, we have to take away a half of Data Length (the length of mask)
                                      : 0)) //Read command, we leave data length unchanged
                            + 1//Data CRC
                        )
                  )
                + hscb_rmap_get_reply_addr_actual_length(raw_rmap_packets[i].reply_addr_chain);

        receiving_rmap_packets->data_areas[receiving_rmap_packets->count_areas]
              = rumboot_malloc_from_named_heap_aligned(default_heap_for_receiving,
                      receiving_rmap_packets->data_area_sizes[receiving_rmap_packets->count_areas], 8);
        if(!receiving_rmap_packets->data_areas[receiving_rmap_packets->count_areas])
        {
            rumboot_free(receiving_rmap_packets->data_areas[receiving_rmap_packets->count_areas]);
            receiving_rmap_packets->data_areas[receiving_rmap_packets->count_areas] = NULL;
            free_mem_from_ready_rmap_packets(receiving_rmap_packets);
            return UNABLE_TO_ALLOCATE_MEMORY;
        }
        descr.start_address = rumboot_virt_to_dma(receiving_rmap_packets->data_areas[receiving_rmap_packets->count_areas]);
        descr.length        = receiving_rmap_packets->data_area_sizes[receiving_rmap_packets->count_areas];
        descr.act           = HSCB_ACT_TRAN;
        descr.act0          = HSCB_ACT0_NOLAST;
        descr.ie            = HSCB_DESCR_ITRPT_OFF;
        descr.valid         = HSCB_DESCR_VALID;
        descr.change_endian = raw_rmap_packets[i].change_endian;
        hscb_set_descr_in_mem(descr, (uint32_t) (receiving_rmap_packets->array_of_descriptors + receiving_rmap_packets->count_areas));

        ++receiving_rmap_packets->count_areas;
    }
    hscb_set_empty_descr_in_mem(
            (uint32_t) (receiving_rmap_packets->array_of_descriptors + receiving_rmap_packets->count_areas),
            descr.change_endian);
#ifdef TEST_OI10_HSCB_FULL_TRACING
    rumboot_printf("obtained count of descriptors == 0x%x\n", receiving_rmap_packets->count_areas + 1);
#endif
    TEST_ASSERT(((receiving_rmap_packets->count_areas + 1) == count_descriptors), "prepare_receiving_areas: count of descriptors and areas mismatch");
    rumboot_printf("prepare_receiving_areas end\n");

    return result;
}

void print_error_status_on_rmap_reply_result(uint32_t result)
{
    if(result & REPLY_ADDR_CHAIN_MISMATCH_IN_REPLY)
        rumboot_printf("RMAP reply: mismatch in reply address chain\n");
    if(result & INITIATOR_LOGICAL_ADDRESS_MISMATCH)
        rumboot_printf("RMAP reply: mismatch in initiator logical address\n");
    if(result & PROTOCOL_ID_MISMATCH)
        rumboot_printf("RMAP reply: RMAP protocol ID (0x1) mismatch\n");
    if(result & INSTRUCTION_MISMATCH)
        rumboot_printf("RMAP reply: mismatch in transmitted and received instruction\n");
    if(result & STATUS_MISMATCH)
        rumboot_printf("RMAP reply: expected RMAP status mismatch\n");
    if(result & TARGET_LOGICAL_ADDRESS_MISMATCH)
        rumboot_printf("RMAP reply: target logical address mismatch\n");
    if(result & TRANSACTION_ID_MISMATCH)
        rumboot_printf("RMAP reply: transaction ID mismatch\n");
    if(result & DATA_LENGTH_MISMATCH)
        rumboot_printf("RMAP reply: mismatch of data length transmitted and received in reply\n");
    if(result & HEADER_CRC_MISMATCH)
        rumboot_printf("RMAP reply: header CRC mismatch\n");
    if(result & DATA_CRC_MISMATCH)
        rumboot_printf("RMAP reply: data CRC mismatch\n");
    if(result & UNSUPPORTED_INSTRUCITON)
        rumboot_printf("RMAP reply: unsupported RMAP instruction detected\n");
    if(result & SOURCE_AND_DEST_LENGTH_MISMATCH)
        rumboot_printf("check_results: memory action check: calculated source and destination length mismatch\n");
    if(result & SOURCE_AND_DEST_DATA_MISMATCH)
        rumboot_printf("check_results: memory action check: source and destination data mismatch\n");
    if(result & REPLY_ARRAY_IS_NULL)
        rumboot_printf("RMAP reply: current reply array is NULL\n");
    if(result & REPLY_CALCULATED_AND_ACTUAL_LENGTH_MISMATCH)
        rumboot_printf("RMAP reply: actually received and calculated length of RMAP reply packet mismatch\n");
    if(result & RESERVED_FIELD_MISMATCH)
        rumboot_printf("RMAP reply: Read reply reserved field is non-zero\n");
    if(!result)
        rumboot_printf("check_results: everything is OK in this packet\n");
}

static uint32_t check_results(
        hscb_rmap_packet_ready_for_transmit_t received_rmap_packets,
        hscb_rmap_packet_raw_configuration_t* raw_rmap_packets,
        uint32_t count_packets
        )
{
    uint32_t result = OK;
    uint32_t common_result = OK;
    uint32_t reply_addr_actual_length;
    uint32_t start_addr;
    uint32_t reply_packet_number = 0;
    uint8_t  instruction;
    uint8_t  header_crc8;
    hscb_uint8_array_with_length_t current_reply;
    hscb_uint8_array_with_length_t source_data = {0};
    hscb_uint8_array_with_length_t destination_data = {0};

    for(uint32_t j = 0; j < count_packets; ++j)
    {
        rumboot_printf("check_results: current packet number == 0x%x, current reply number == 0x%x\n", j, reply_packet_number);
        /*Covering all write commands regarding source and destination areas*/
        if(raw_rmap_packets[j].instruction & HSCB_RMAP_PACKET_INSTRUCTION_FIELD_WRITE_mask)
        {
            if(raw_rmap_packets[j].instruction & HSCB_RMAP_PACKET_INSTRUCTION_FIELD_INCREMENT_mask)
            {
                source_data.array = raw_rmap_packets[j].data_chain.array;
                source_data.length = raw_rmap_packets[j].data_chain.length;
                destination_data.array = (void *)raw_rmap_packets[j].addr;
                destination_data.length = raw_rmap_packets[j].data_chain.length;
            }else
            {
                source_data.array = (raw_rmap_packets[j].data_chain.length) ? (raw_rmap_packets[j].data_chain.array + (raw_rmap_packets[j].data_chain.length - 4)) : NULL;
                source_data.length = (raw_rmap_packets[j].data_chain.length) ? 4 : 0;
                destination_data.array = (raw_rmap_packets[j].data_chain.length) ? (void *)raw_rmap_packets[j].addr : NULL;
                destination_data.length = (raw_rmap_packets[j].data_chain.length) ? 4 : 0;
            }
        }

        /*Let's skip packets without reply*/
        if((raw_rmap_packets[j].instruction
                    & HSCB_RMAP_PACKET_INSTRUCTION_FIELD_W_REPLY_mask))
        {

            current_reply.array = received_rmap_packets.data_areas[reply_packet_number];
            current_reply.length = received_rmap_packets.data_area_sizes[reply_packet_number];
            reply_addr_actual_length = hscb_rmap_get_reply_addr_actual_length(raw_rmap_packets[j].reply_addr_chain);
            start_addr = raw_rmap_packets[j].reply_addr_chain.length - reply_addr_actual_length;
            if((current_reply.array == NULL))
                return (result |  REPLY_ARRAY_IS_NULL);//, "hscb_rmap_get_reply_byte: empty RMAP reply array");

            if(received_rmap_packets.data_area_sizes[reply_packet_number]
                           != hscb_rmap_reply_calculate_length(current_reply,reply_addr_actual_length) )
                return (result |  REPLY_CALCULATED_AND_ACTUAL_LENGTH_MISMATCH);//, "hscb_rmap_get_reply_byte: empty RMAP reply array");
//                    "Calculated length does not match the actual");

            /*checking reply addr*/
            for(uint32_t i = start_addr; i < raw_rmap_packets[j].reply_addr_chain.length; ++i)
            {
                if(current_reply.array[i - start_addr] != raw_rmap_packets[j].reply_addr_chain.array[i])
                    result |= REPLY_ADDR_CHAIN_MISMATCH_IN_REPLY;
            }
            instruction = hscb_rmap_get_reply_byte(current_reply,reply_addr_actual_length, HSCB_RMAP_REPLY_INSTRUCTION_i);
#ifdef TEST_OI10_HSCB_FULL_TRACING
            rumboot_printf("instruction == 0x%x, raw packet instruction == 0x%x, reply-from-initial instruction == 0x%x\n",
                    instruction, raw_rmap_packets[j].instruction, hscb_rmap_make_reply_instruction(raw_rmap_packets[j].instruction));
#endif
            /*checking the rest*/
            result |= ((hscb_rmap_get_reply_byte(current_reply,reply_addr_actual_length, HSCB_RMAP_REPLY_INITIATOR_LOGICAL_ADDR_i)
                    == raw_rmap_packets[j].initiator_logical_addr)
                        ? OK : INITIATOR_LOGICAL_ADDRESS_MISMATCH)
                    | ((hscb_rmap_get_reply_byte(current_reply,reply_addr_actual_length, HSCB_RMAP_REPLY_PROTOCOL_ID_i) == HSCB_RMAP_PROTOCOL_ID)
                        ? OK : PROTOCOL_ID_MISMATCH)
                    | ((instruction == hscb_rmap_make_reply_instruction(raw_rmap_packets[j].instruction))
                        ? OK : INSTRUCTION_MISMATCH)
                    | ((hscb_rmap_get_reply_byte(current_reply,reply_addr_actual_length, HSCB_RMAP_REPLY_STATUS_i) == raw_rmap_packets[j].expected_reply_status)
                        ? OK : STATUS_MISMATCH)
                    | ((hscb_rmap_get_reply_byte(current_reply,reply_addr_actual_length, HSCB_RMAP_REPLY_TARGET_LOGICAL_ADDR_i) == raw_rmap_packets[j].target_logical_addr)
                        ? OK : TARGET_LOGICAL_ADDRESS_MISMATCH)
                    | ((((hscb_rmap_get_reply_byte(current_reply,reply_addr_actual_length, HSCB_RMAP_REPLY_TRANSACTION_ID_MS_i) << 8 )
                            |(hscb_rmap_get_reply_byte(current_reply,reply_addr_actual_length, HSCB_RMAP_REPLY_TRANSACTION_ID_LS_i)))
                            == raw_rmap_packets[j].transaction_id)
                        ? OK : TRANSACTION_ID_MISMATCH)
                        ;
            if(instruction & HSCB_RMAP_PACKET_INSTRUCTION_FIELD_WRITE_mask)
            {
                rumboot_printf("header crc8\n");
                header_crc8 = crc8(0, current_reply.array + reply_addr_actual_length, HSCB_RMAP_REPLY_RESERVED_R_HEADER_CRC8_W_i);
                result |= (hscb_rmap_get_reply_byte(current_reply,reply_addr_actual_length,HSCB_RMAP_REPLY_RESERVED_R_HEADER_CRC8_W_i) == header_crc8)
                        ? OK : HEADER_CRC_MISMATCH;
            }else{
                uint8_t data_crc8 = 0;
                rumboot_printf("header crc8\n");
                header_crc8 = crc8(0, current_reply.array + reply_addr_actual_length, HSCB_RMAP_REPLY_HEADER_CRC8_R_i);
                rumboot_printf("data crc8\n");
                data_crc8 = crc8(0, current_reply.array + reply_addr_actual_length + HSCB_RMAP_REPLY_DATA_START_i,
                        hscb_rmap_reply_get_data_len(current_reply,reply_addr_actual_length));
                result |=   ((hscb_rmap_get_reply_byte(current_reply,reply_addr_actual_length,HSCB_RMAP_REPLY_RESERVED_R_HEADER_CRC8_W_i) == 0)
                              ? OK : RESERVED_FIELD_MISMATCH)
                          | ((hscb_rmap_reply_get_data_len(current_reply,reply_addr_actual_length) ==
                                  ((((raw_rmap_packets[j].instruction & HSCB_RMAP_PACKET_INSTRUCTION_RMAP_COMMAND_mask)
                                          >> HSCB_RMAP_PACKET_INSTRUCTION_RMAP_COMMAND_i)
                                          == HSCB_RMAP_COMMAND_RMW_INCREMENTING_ADDRESS)
                                          ? (raw_rmap_packets[j].data_chain.length >> 1): raw_rmap_packets[j].data_chain.length))
                              ? OK : DATA_LENGTH_MISMATCH)
                          | ((hscb_rmap_get_reply_byte(current_reply,reply_addr_actual_length,HSCB_RMAP_REPLY_HEADER_CRC8_R_i) == header_crc8)
                              ? OK : HEADER_CRC_MISMATCH)
                          | ((hscb_rmap_get_reply_byte(current_reply,reply_addr_actual_length,HSCB_RMAP_REPLY_DATA_START_i
                                  + hscb_rmap_reply_get_data_len(current_reply,reply_addr_actual_length)) == data_crc8)
                              ? OK : DATA_CRC_MISMATCH)
                                ;
            }
#ifdef TEST_OI10_HSCB_FULL_TRACING
            rumboot_printf("check_results: before checking instruction\n");
#endif
            /*Here we skip all write commands and cover all read commands*/
            switch((raw_rmap_packets[j].instruction &
                    HSCB_RMAP_PACKET_INSTRUCTION_RMAP_COMMAND_mask) >> HSCB_RMAP_PACKET_INSTRUCTION_RMAP_COMMAND_i)
            {
            case HSCB_RMAP_COMMAND_READ_SINGLE_ADDRESS:
                {
                    source_data.array = (void *)raw_rmap_packets[j].addr;
                    source_data.length = raw_rmap_packets[j].data_chain.length;
                    destination_data.length = hscb_rmap_reply_get_data_len(current_reply, reply_addr_actual_length);
                    destination_data.array = (destination_data.length)
                            ? (received_rmap_packets.data_areas[reply_packet_number]
                               + reply_addr_actual_length
                               + HSCB_RMAP_REPLY_DATA_START_i)
                                    : NULL;
                }break;
            case HSCB_RMAP_COMMAND_READ_INCREMENTING_ADDRESS:
                {
                    source_data.array = (void *)raw_rmap_packets[j].addr;
                    source_data.length = raw_rmap_packets[j].data_chain.length;
                    destination_data.length = hscb_rmap_reply_get_data_len(current_reply, reply_addr_actual_length);
                    destination_data.array = (destination_data.length)
                            ? (received_rmap_packets.data_areas[reply_packet_number]
                               + reply_addr_actual_length
                               + HSCB_RMAP_REPLY_DATA_START_i)
                                    : NULL;
                }break;
            case HSCB_RMAP_COMMAND_RMW_INCREMENTING_ADDRESS:
                {
                    destination_data.array = (void *)raw_rmap_packets[j].addr;
                    destination_data.length = raw_rmap_packets[j].data_chain.length >> 1;
                    source_data.array = (received_rmap_packets.data_areas[reply_packet_number]
                                                                   + reply_addr_actual_length + HSCB_RMAP_REPLY_DATA_START_i);
                    source_data.length = hscb_rmap_reply_get_data_len(current_reply, reply_addr_actual_length);
                }break;
            default:
                {
                    if(!(raw_rmap_packets[j].instruction & HSCB_RMAP_PACKET_INSTRUCTION_FIELD_WRITE_mask))
                        return result | UNSUPPORTED_INSTRUCITON;
                }
            }

            ++reply_packet_number;
        }else if(!(raw_rmap_packets[j].instruction & HSCB_RMAP_PACKET_INSTRUCTION_FIELD_WRITE_mask)){
            return (result | UNSUPPORTED_INSTRUCITON);
        }

#ifdef TEST_OI10_HSCB_FULL_TRACING
        rumboot_printf("check_results: before checking memory\n");
#endif
        if(raw_rmap_packets[j].expected_reply_status == HSCB_RMAP_REPLY_STATUS_OK)
        {
            result |= (source_data.length == destination_data.length) ? OK : SOURCE_AND_DEST_LENGTH_MISMATCH;
            if(((raw_rmap_packets[j].instruction & HSCB_RMAP_PACKET_INSTRUCTION_RMAP_COMMAND_mask)
                    >> HSCB_RMAP_PACKET_INSTRUCTION_RMAP_COMMAND_i)
                    == HSCB_RMAP_COMMAND_RMW_INCREMENTING_ADDRESS)
            {
                for(uint32_t i = 0; (i < min(destination_data.length,source_data.length)); ++i)
                    if(((source_data.array[i] & (~raw_rmap_packets[j].data_chain.array[i + destination_data.length]))
                            | (raw_rmap_packets[j].data_chain.array[i + destination_data.length]
                                   & raw_rmap_packets[j].data_chain.array[i]))
                            != (destination_data.array[i]))
                        result |= SOURCE_AND_DEST_DATA_MISMATCH;
            }
            else
                if(((raw_rmap_packets[j].instruction & HSCB_RMAP_PACKET_INSTRUCTION_RMAP_COMMAND_mask)
                    >> HSCB_RMAP_PACKET_INSTRUCTION_RMAP_COMMAND_i)
                    == HSCB_RMAP_COMMAND_READ_SINGLE_ADDRESS)
                    for(uint32_t i = 0; (i < (min(destination_data.length,source_data.length) >> 2)); ++i)
                        result |= (*((uint32_t*)(source_data.array)) == *((uint32_t*)(destination_data.array + 4)))
                            ? OK : SOURCE_AND_DEST_DATA_MISMATCH ;
                else
                    result |= (memcmp(source_data.array, destination_data.array,
                            min(destination_data.length,source_data.length)) ? SOURCE_AND_DEST_DATA_MISMATCH : OK);
        }
        print_error_status_on_rmap_reply_result(result);
        common_result |= result;
        result = OK;
    }
    return common_result;
}

static uint32_t check_rmap_func(
        uint32_t base_addr,
        uint32_t supplementary_base_addr){
    uint32_t cnt = 0;
    uint32_t result = 0;
    uint32_t rmap_status = 0;
    uint32_t rmap_c_status = 0;
    hscb_rmap_packet_raw_configuration_t* raw_rmap_packets = NULL;
    hscb_rmap_packet_ready_for_transmit_t ready_rmap_packets = {0};
    hscb_rmap_packet_ready_for_transmit_t receiving_rmap_packets = {0};
    hscb0_status = 0;
    hscb1_status = 0;
    hscb_under_test_dma_status = 0;
    hscb_supplementary_dma_status = 0;
    bool do_we_receive_anything = true;

    rumboot_putstring( "------- HSCB RMAP test -------\n" );

    raw_rmap_packets = rumboot_malloc_from_heap_aligned(
            DEFAULT_HEAP_ID,
            sizeof(hscb_rmap_packet_raw_configuration_t) * COUNT_PACKETS, 0);
    if(!raw_rmap_packets)
    {
        rumboot_putstring("Cannot allocate memory from default heap for raw RMAP packets, exiting.");
        return 1;
    }
    memset(raw_rmap_packets, 0, sizeof(hscb_rmap_packet_raw_configuration_t) * COUNT_PACKETS);

    switch (generate_some_raw_rmap_packets(raw_rmap_packets, COUNT_PACKETS))
    {
        case OK:rumboot_printf("generate_some_raw_rmap_packets: OK\n"); break;
        case UNSUPPORTED_COUNT_PACKETS:
        {
            rumboot_printf("generate_some_raw_rmap_packets: unsupported count of predefined packets\n");
            return 1;
        } break;
        case UNABLE_TO_ALLOCATE_MEMORY:
        {
            rumboot_printf("generate_some_raw_rmap_packets: unable to allocate memory for target address chain, reply address chain or data (either Rx or TX area).\n");
            return 1;
        } break;
        default:
        {
            rumboot_printf("generate_some_raw_rmap_packets: unexpected return value\n");
            return 1;
        }
    }

    switch(packing_for_rmap(&ready_rmap_packets, raw_rmap_packets, COUNT_PACKETS))
    {
        case OK:rumboot_printf("packing_for_rmap: OK\n"); break;
        case UNABLE_TO_ALLOCATE_MEMORY:
        {
            rumboot_printf("packing_for_rmap: unable to allocate memory for target address chain, reply address chain or data (either Rx or TX area).\n");
            return 1;
        } break;
        default:
        {
            rumboot_printf("packing_for_rmap: unexpected return value\n");
            return 1;
        }
    }
#ifdef TEST_OI10_HSCB_FULL_TRACING
    print_hscb_rmap_packets_via_putdump(ready_rmap_packets);
#endif

    switch(prepare_receiving_areas(&receiving_rmap_packets, raw_rmap_packets, COUNT_PACKETS))
    {
        case OK: rumboot_printf("prepare_receiving_areas: OK\n"); break;
        case NO_REPLIES_ARE_EXPECTED:
        {
            rumboot_printf("prepare_receiving_areas: no replies are expected.\n");
            do_we_receive_anything = false;
        }break;
        case UNABLE_TO_ALLOCATE_MEMORY:
        {
            free_mem_from_ready_rmap_packets(&ready_rmap_packets);
            rumboot_printf("prepare_receiving_areas: unable to allocate memory for target address chain, reply address chain or data (either Rx or TX area).\n");
            return 1;
        } break;
        default:
        {
            rumboot_printf("prepare_receiving_areas: unexpected return value\n");
            return 1;
        }
    }

#ifdef TEST_OI10_HSCB_FULL_TRACING
    print_hscb_descriptors_in_cycle(&(ready_rmap_packets.array_of_descriptors), &(ready_rmap_packets.count_areas), 1);
    print_hscb_descriptors_in_cycle(&(receiving_rmap_packets.array_of_descriptors), &(receiving_rmap_packets.count_areas), 1);
#endif

    hscb_set_rdma_tbl_size(supplementary_base_addr, hscb_get_tbl_len_by_count(ready_rmap_packets.count_areas - 1));
    hscb_set_rdma_sys_addr(supplementary_base_addr, rumboot_virt_to_dma((uint32_t *) ready_rmap_packets.array_of_descriptors));
    if(do_we_receive_anything)
    {
        hscb_set_wdma_tbl_size(supplementary_base_addr, hscb_get_tbl_len_by_count(receiving_rmap_packets.count_areas));
        hscb_set_wdma_sys_addr(supplementary_base_addr, rumboot_virt_to_dma((uint32_t *) receiving_rmap_packets.array_of_descriptors));
    }

    // Enable supplementary HSCB
    iowrite32((1 << HSCB_IRQ_MASK_ACTIVE_LINK_i),    supplementary_base_addr + HSCB_IRQ_MASK);
    iowrite32((1 << HSCB_SETTINGS_EN_HSCB_i),        supplementary_base_addr + HSCB_SETTINGS);
    rumboot_printf( "Waiting for supplementary HSCB(0x%x) link establishing\n",
            supplementary_base_addr);
    if(!(   hscb_wait_status(supplementary_base_addr,   (HSCB_STATE_RUN << HSCB_STATUS_STATE_i)))){
        rumboot_putstring( "Wait HSCB RUN state Time-out\n" );
        rumboot_printf("HSCB(0x%x) status: 0x%x\n", supplementary_base_addr, hscb_get_status(supplementary_base_addr));
        return 1;
    }

    rumboot_putstring( "HSCB link has enabled\n" );
    // Enable DMA for supplementary HSCB
    rumboot_putstring( "Start work!\n" );
    hscb_run_rdma(supplementary_base_addr);
    if(do_we_receive_anything)
    {
        hscb_run_wdma(supplementary_base_addr);
        rumboot_putstring( "Wait supplementary HSCB finish work\n" );
        while (!(hscb_supplementary_dma_status)){
            if (cnt == MAX_ATTEMPTS) {
                rumboot_putstring( "Wait interrupt Time-out\n" );
                return 1;
            }
            else
                cnt += 1;
        }
        rumboot_putstring( "Finish work!\n" );
    }
    cnt = 0;
    hscb_under_test_dma_status = 0;
    hscb_supplementary_dma_status = 0;
    rmap_status = ioread32(base_addr + HSCB_RMAP_STATUS);
    rmap_c_status = ioread32(base_addr + HSCB_RMAP_C_STATUS);
    rumboot_printf("rmap_status == 0x%x, rmap_c_status == 0x%x\n", rmap_status, rmap_c_status);

#ifdef TEST_OI10_HSCB_FULL_TRACING
    print_hscb_rmap_packets_via_putdump(receiving_rmap_packets);
#endif
    emi_init(DCR_EM2_EMI_BASE);

    result |= check_results(receiving_rmap_packets,raw_rmap_packets,COUNT_PACKETS);

    return result;
}
// Add by M.Chelyshev as glitch-protect
uint32_t disable_hscb (uint32_t hscb_base){
    uint32_t tmp = 0;
    tmp = ioread32(hscb_base + HSCB_SETTINGS);
    tmp = tmp & 0xfffffffe;
    iowrite32(tmp, hscb_base + HSCB_SETTINGS);
}
// ------------------------------------

int main() {
    uint32_t result = 0x0;
    struct rumboot_irq_entry *tbl;

    rumboot_printf( "Check HSCB RMAP (0x%x) \n", HSCB_UNDER_TEST_BASE );

    //crc8 calculation speedup
    test_event_send_test_id("test_oi10_hscb_rmap");

    write_tlb_entries(em_anti_x_tlb_entries,2);

    tbl = create_irq_handlers();
  
    disable_hscb(HSCB_SUPPLEMENTARY_BASE);    

    result += check_rmap_func(HSCB_UNDER_TEST_BASE, HSCB_SUPPLEMENTARY_BASE);
    delete_irq_handlers(tbl);


    return (result) ? 1 : 0;
}
