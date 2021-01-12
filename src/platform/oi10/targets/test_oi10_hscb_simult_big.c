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
#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/regpoker.h>
#include <platform/devices.h>
#include <platform/trace.h>
#include <platform/test_assert.h>
#include <platform/interrupts.h>
#include <platform/regs/regs_hscb.h>
#include <rumboot/irq.h>
#include <rumboot/rumboot.h>
#include <rumboot/memfill.h>
#include <platform/devices/hscb.h>
#include <platform/devices/emi.h>
#include <platform/devices/nor_1636RR4.h>
#include <platform/arch/ppc/ppc_476fp_mmu.h>

#ifndef N_OF_PACKETS
#define N_OF_PACKETS 5
#endif

#ifndef SIZE_OF_PACKET
#define SIZE_OF_PACKET 1024
#endif

//------ HSCB0 
#ifndef HSCB0_TX_DSCTBL_BASE
#define HSCB0_TX_DSCTBL_BASE "SRAM0"
#endif
#ifndef HSCB0_TX_DATA_BASE
#define HSCB0_TX_DATA_BASE "SRAM0"
#endif
#ifndef HSCB0_RX_DSCTBL_BASE
#define HSCB0_RX_DSCTBL_BASE "SRAM0"
#endif
#ifndef HSCB0_RX_DATA_BASE
#define HSCB0_RX_DATA_BASE "SRAM0"
#endif
//------ HSCB1 
#ifndef HSCB1_TX_DSCTBL_BASE
#define HSCB1_TX_DSCTBL_BASE "SRAM0"
#endif
#ifndef HSCB1_TX_DATA_BASE
#define HSCB1_TX_DATA_BASE "SRAM0"
#endif
#ifndef HSCB1_RX_DSCTBL_BASE
#define HSCB1_RX_DSCTBL_BASE "SRAM0"
#endif
#ifndef HSCB1_RX_DATA_BASE
#define HSCB1_RX_DATA_BASE "SRAM0"
#endif
//------ HSCB2 
#ifndef HSCB2_TX_DSCTBL_BASE
#define HSCB2_TX_DSCTBL_BASE "SRAM0"
#endif
#ifndef HSCB2_TX_DATA_BASE
#define HSCB2_TX_DATA_BASE "SRAM0"
#endif
#ifndef HSCB2_RX_DSCTBL_BASE
#define HSCB2_RX_DSCTBL_BASE "SRAM0"
#endif
#ifndef HSCB2_RX_DATA_BASE
#define HSCB2_RX_DATA_BASE "SRAM0"
#endif
//------ HSCB3
#ifndef HSCB3_TX_DSCTBL_BASE
#define HSCB3_TX_DSCTBL_BASE "SRAM0"
#endif
#ifndef HSCB3_TX_DATA_BASE
#define HSCB3_TX_DATA_BASE "SRAM0"
#endif
#ifndef HSCB3_RX_DSCTBL_BASE
#define HSCB3_RX_DSCTBL_BASE "SRAM0"
#endif
#ifndef HSCB3_RX_DATA_BASE
#define HSCB3_RX_DATA_BASE "SRAM0"
#endif
#ifndef DEBUG_PRINT
#define DEBUG_PRINT 0
#endif
#ifndef CHANGE_ENDIANN
#define CHANGE_ENDIANN true
#endif

static volatile uint32_t hscb0_status;
static volatile uint32_t hscb1_status;
static volatile uint32_t hscb0_dma_status;
static volatile uint32_t hscb1_dma_status;
static volatile bool hscb0_link_established;
static volatile bool hscb1_link_established;
volatile uint32_t hscb0_rx_irq_cnt, hscb1_rx_irq_cnt, hscb2_rx_irq_cnt, hscb3_rx_irq_cnt;


void create_dsc_table(uint32_t quantity_of_descr, hscb_descr **dsc_table, char *hscb_dsctbl_heap_name){
    hscb_descr *dsc_table_loc;
    if(DEBUG_PRINT) rumboot_printf("Start of create_dsc_table\n");
    if(DEBUG_PRINT) {if (*dsc_table == NULL) rumboot_printf("dsc_table have NULL value\n");}
    dsc_table_loc = (hscb_descr*)rumboot_malloc_from_named_heap_aligned(hscb_dsctbl_heap_name, quantity_of_descr * sizeof(hscb_descr), 0x8);
    *dsc_table = dsc_table_loc;
    if(DEBUG_PRINT) {if (*dsc_table!= NULL) rumboot_printf("End of create_dsc_table: pointer = 0x%x\n", *dsc_table);
    else rumboot_printf("Malloc for dsc_table failed");}
}
void print_uint64(uint64_t val){
    uint32_t word1, word2;
    word1 = val >> 32;
    word2 = val;
    rumboot_printf("%x%x", word1, word2);    
}

void init_descr(hscb_descr *dsc_table, uint32_t num_of_descr, hscb_descr *dsc) {
    uint32_t word1, word2, word1_p, word2_p;
    hscb_descr *dsc_table_pt;
    uint64_t *dscr_p;
    if(DEBUG_PRINT)rumboot_printf("dsc_addr = %x, start_address = %x, length = %x, act = %x, act0 = %x, ie = %x, err = %x, valid = %x\n", &dsc_table[num_of_descr], dsc->start_address, dsc->length, dsc->act, dsc->act0, dsc->ie, dsc->err, dsc->valid);
    dsc_table[num_of_descr].start_address  =   dsc->start_address;
    dsc_table[num_of_descr].length         =   dsc->length       ;
    dsc_table[num_of_descr].act            =   dsc->act          ;
    dsc_table[num_of_descr].act0           =   dsc->act0         ;
    dsc_table[num_of_descr].ie             =   dsc->ie           ;
    dsc_table[num_of_descr].err            =   dsc->err          ;
    dsc_table[num_of_descr].valid          =   dsc->valid        ;
    if (CHANGE_ENDIANN){
        dsc_table_pt = &dsc_table[num_of_descr];
        dscr_p = (uint64_t*)(dsc_table_pt);
        word1 = (*dscr_p) >> 32;
        word2 = *dscr_p;
        if(DEBUG_PRINT){
            rumboot_printf("dscr_p = %x, word1 = %x, word2 = %x\n", dsc_table_pt, word1, word2);
        }
        word1 = hscb_change_endian(word1 ,1);
        word2 = hscb_change_endian(word2 ,1);
        *dscr_p = 0;
        if(DEBUG_PRINT){
            rumboot_printf("Step 1: *dscr_p = ");
            print_uint64(*dscr_p);
            rumboot_printf("\n");
        }
        *dscr_p = word2;
        if(DEBUG_PRINT){
            rumboot_printf("Step 2: *dscr_p = ");
            print_uint64(*dscr_p);
            rumboot_printf("\n");
        }
        *dscr_p = (*dscr_p << 32);
        if(DEBUG_PRINT){
            rumboot_printf("Step 3: *dscr_p = ");
            print_uint64(*dscr_p);
            rumboot_printf("\n");
        }
        *dscr_p = *dscr_p | word1;
        if(DEBUG_PRINT){
            rumboot_printf("Step 4: *dscr_p = ");
            print_uint64(*dscr_p);
            rumboot_printf("\n");
        }
    }
}

void print_dscr_table(hscb_descr *dsc_table, bool ch_end) {
    uint32_t i, word1, word2;
    uint64_t *dscr_p, dsc_loc;
    hscb_descr *dsc;
    rumboot_printf("Print content of DSC table at 0x%x\n", dsc_table);
    for (i=0; i < N_OF_PACKETS; i=i+1) {
        dsc = &dsc_table[i];
        //dscr_p = (uint64_t*)(dsc_table +i);
        dscr_p = (uint64_t*)(dsc);
        if(DEBUG_PRINT) {
            rumboot_printf("Read *dscr_p = ");
            print_uint64(*dscr_p);
            rumboot_printf("\n");
        }
        word1 = *dscr_p >> 32;
        word2 = *dscr_p;
        if (ch_end){
            word1 = hscb_change_endian(word1 ,1);
            word2 = hscb_change_endian(word2 ,1);
            dsc_loc = 0;
            dsc_loc = word2;
            dsc_loc = (dsc_loc << 32);
            dsc_loc = dsc_loc | word1;
            dscr_p = &dsc_loc;
            dsc = (hscb_descr*)dscr_p;
        }
        //rumboot_printf("N=%d, word1 = %x, word2 = %x, start_address = %x, length = %x, act = %x, act0 = %x, ie = %x, err = %x, valid = %x\n", i, word1, word2, dsc_table[i].start_address, dsc_table[i].length, dsc_table[i].act, dsc_table[i].act0, dsc_table[i].ie, dsc_table[i].err, dsc_table[i].valid);
        rumboot_printf("N=%d, word1 = %x, word2 = %x, start_address = %x, length = %x, act = %x, act0 = %x, ie = %x, err = %x, valid = %x\n", i, word1, word2, dsc->start_address, dsc->length, dsc->act, dsc->act0, dsc->ie, dsc->err, dsc->valid);
    }
    
}


void create_and_init_dsc_table(hscb_descr **hscb_dsc_table, char *hscb_dsctbl_heap_name, char *hscb_data_heap_name) {
    uint32_t n_of_dsc = N_OF_PACKETS, i, *frame_buf;
    hscb_descr descr, *dsc_table;
    //--------
    if(DEBUG_PRINT) rumboot_printf("Start of config_hscb\n");
    create_dsc_table(n_of_dsc, &dsc_table, hscb_dsctbl_heap_name);
    *hscb_dsc_table = dsc_table;
    if(DEBUG_PRINT) rumboot_printf("hscb_dsc_table = 0x%x\n", *hscb_dsc_table);
    for (i=0; i < n_of_dsc; i=i+1) {
        descr.length = SIZE_OF_PACKET; // size of packet in bytes
        frame_buf = (uint32_t*)rumboot_malloc_from_named_heap_aligned(hscb_data_heap_name, descr.length/4 * sizeof(uint32_t), 0x8);
        descr.start_address = rumboot_virt_to_dma(frame_buf);
        descr.act    =   2;      
        descr.act0   =   1;      
        if(i == n_of_dsc-1) descr.ie = 1; else descr.ie = 0;          
        descr.valid  =   1;
        init_descr(dsc_table, i, &descr);
        rumboot_memfill32(frame_buf, descr.length, descr.start_address, 0xF);
    }
}

void hscb_rdma_prepare(uint32_t base_addr, uint32_t *rdma_ctrl_word)
{
    hscb_rwdma_settings_t rdma_settings;
    hscb_get_rdma_settings(base_addr, &rdma_settings);
    rdma_settings.en_rwdma = true;
    rdma_settings.en_rwdma_desc_tbl = true;
    rdma_settings.rwdma_long_len = true;
    *rdma_ctrl_word =( rdma_settings.rw_desc_int       << HSCB_XDMA_SETTINGS_X_DESC_INT_i )          |
               ( rdma_settings.rw_desc_end       << HSCB_XDMA_SETTINGS_X_DESC_END_i )          |
               ( rdma_settings.rw_bad_desc       << HSCB_XDMA_SETTINGS_X_BAD_DESC_i )          |
               ( rdma_settings.rw_axi_error      << HSCB_XDMA_SETTINGS_X_AXI_ERROR_i )      |
               ( rdma_settings.rw_buf_full       << HSCB_XDMA_SETTINGS_X_BUF_FULL_i )       |
               ( rdma_settings.rw_buf_empty      << HSCB_XDMA_SETTINGS_X_BUF_EMPTY_i )      |
               ( rdma_settings.en_rwdma          << HSCB_XDMA_SETTINGS_EN_XDMA_i )          |
               ( rdma_settings.en_rwdma_desc_tbl << HSCB_XDMA_SETTINGS_EN_XDMA_DESC_TBL_i ) |
               ( rdma_settings.rwdma_long_len    << HSCB_XDMA_SETTINGS_XDMA_LONG_LEN_i )    |
               ( rdma_settings.cancel_rwdma      << HSCB_XDMA_SETTINGS_CANCEL_XDMA_i );
}

void config_hscb(uint32_t hscb_base_addr, hscb_descr **hscb_tx_dsc_table, char *hscb_tx_dsctbl_heap_name, char *hscb_tx_data_heap_name, hscb_descr **hscb_rx_dsc_table, char *hscb_rx_dsctbl_heap_name, char *hscb_rx_data_heap_name, uint32_t *rdma_ctrl_word) {
    hscb_descr *dsc_table;
    uint32_t rdma_ctrl_word_loc;
    // --- create & init of descriptors tables
    create_and_init_dsc_table(&dsc_table, hscb_tx_dsctbl_heap_name, hscb_tx_data_heap_name); // create & init DSC table for TX (RDMA)
    *hscb_tx_dsc_table=dsc_table;
    create_and_init_dsc_table(&dsc_table, hscb_rx_dsctbl_heap_name, hscb_rx_data_heap_name); // create & init DSC table for TX (RDMA)
    *hscb_rx_dsc_table=dsc_table;
    // --- set of DMA regs
    hscb_set_rdma_tbl_size(hscb_base_addr, N_OF_PACKETS * sizeof(hscb_descr));
    hscb_set_rdma_sys_addr(hscb_base_addr, *hscb_tx_dsc_table);
    hscb_set_wdma_tbl_size(hscb_base_addr, N_OF_PACKETS * sizeof(hscb_descr));
    hscb_set_wdma_sys_addr(hscb_base_addr, *hscb_rx_dsc_table);
    // --- set of HSCB regs
    hscb_set_max_speed(hscb_base_addr);
    hscb_rdma_prepare(hscb_base_addr, &rdma_ctrl_word_loc);
    *rdma_ctrl_word = rdma_ctrl_word_loc;
}

uint32_t establish_link(uint32_t hscb_base_addr_0, uint32_t hscb_base_addr_1){
    iowrite32((1 << HSCB_IRQ_MASK_ACTIVE_LINK_i),    hscb_base_addr_0 + HSCB_IRQ_MASK);
    iowrite32((1 << HSCB_IRQ_MASK_ACTIVE_LINK_i),    hscb_base_addr_1 + HSCB_IRQ_MASK);
    iowrite32((1 << HSCB_SETTINGS_EN_HSCB_i),        hscb_base_addr_0 + HSCB_SETTINGS);
    iowrite32((1 << HSCB_SETTINGS_EN_HSCB_i),        hscb_base_addr_1 + HSCB_SETTINGS);
    // Wait connecting
    rumboot_printf( "Waiting for HSCB (0x%x) and HSCB (0x%x) link establishing\n", hscb_base_addr_0, hscb_base_addr_1);
    if(!(   hscb_wait_status(hscb_base_addr_0, (HSCB_STATE_RUN << HSCB_STATUS_STATE_i)) &
            hscb_wait_status(hscb_base_addr_1, (HSCB_STATE_RUN << HSCB_STATUS_STATE_i)))){
        rumboot_putstring( "Wait HSCB RUN state Time-out\n" );
        rumboot_printf("HSCB (0x%x) status: 0x%x\n", hscb_base_addr_0, hscb_get_status(hscb_base_addr_0));
        rumboot_printf("HSCB (0x%x) status: 0x%x\n", hscb_base_addr_1, hscb_get_status(hscb_base_addr_1));
        return 1;
    }
    rumboot_printf( "Link is established\n", hscb_base_addr_0, hscb_base_addr_1);
    return 0;   
}

void simult_run_of_hscbs_transfers(uint32_t *rdma_ctrl_word){
    hscb_run_wdma(HSCB0_BASE);
    hscb_run_wdma(HSCB1_BASE);
    hscb_run_wdma(HSCB2_BASE);
    hscb_run_wdma(HSCB3_BASE);
    //hscb_run_rdma(HSCB0_BASE);
    //hscb_run_rdma(HSCB1_BASE);
    //hscb_run_rdma(HSCB2_BASE);
    //hscb_run_rdma(HSCB3_BASE);
    if(DEBUG_PRINT) rumboot_printf("Run all RDMAs: %x, %x, %x, %x\n", rdma_ctrl_word[0], rdma_ctrl_word[1], rdma_ctrl_word[2], rdma_ctrl_word[3]);
    iowrite32(rdma_ctrl_word[0], HSCB0_BASE + HSCB_RDMA_SETTINGS);
    iowrite32(rdma_ctrl_word[1], HSCB1_BASE + HSCB_RDMA_SETTINGS);
    iowrite32(rdma_ctrl_word[2], HSCB2_BASE + HSCB_RDMA_SETTINGS);
    iowrite32(rdma_ctrl_word[3], HSCB3_BASE + HSCB_RDMA_SETTINGS);
}

// SW0 connected to SW1
// SW2 connected to SW3
// SW0_HSCB_INT
// SW0_AXI_INT
static void hscb_handler( int irq, void *arg ) {
// irq - number of interrupt
    // ---- interrupt handler for HSCB-core
    uint32_t irq_type, hscb_base_addr;
    irq_type = 0;
    switch(irq){
        case SW0_HSCB_INT   : irq_type = 1; hscb_base_addr = HSCB0_BASE; break;
        case SW0_AXI_INT    : irq_type = 2; hscb_base_addr = HSCB0_BASE; break;
        case SW1_HSCB_INT   : irq_type = 1; hscb_base_addr = HSCB1_BASE; break;
        case SW1_AXI_INT    : irq_type = 2; hscb_base_addr = HSCB1_BASE; break;
        case SW2_HSCB_INT   : irq_type = 1; hscb_base_addr = HSCB2_BASE; break;
        case SW2_AXI_INT    : irq_type = 2; hscb_base_addr = HSCB2_BASE; break;
        case SW3_HSCB_INT   : irq_type = 1; hscb_base_addr = HSCB3_BASE; break;
        case SW3_AXI_INT    : irq_type = 2; hscb_base_addr = HSCB3_BASE; break;
    }
    // ---- interrupt handler for HSCB-core
    if (irq_type == 1) {
        rumboot_printf("HSCB_IRQ (0x%x), status : 0x%x\n",
                hscb_base_addr,
                hscb_get_status(hscb_base_addr));
    }
    // ---- interrupt handler for DMA-core
    if (irq_type == 2) {
        hscb0_dma_status = hscb_get_adma_ch_status(hscb_base_addr );
        rumboot_printf("HSCB_DMA_IRQ (0x%x), AXI DMA channel status : 0x%x\n", hscb_base_addr, hscb0_dma_status);
        if (hscb0_dma_status & HSCB_ADMA_CH_STATUS_RDMA_IRQ_mask){
            hscb0_dma_status = hscb_get_rdma_status(hscb_base_addr);
            rumboot_printf("RDMA channel detailed status : 0x%x\n", hscb0_dma_status);
        }
        else if (hscb0_dma_status & HSCB_ADMA_CH_STATUS_WDMA_IRQ_mask) {
            hscb0_dma_status = hscb_get_wdma_status(hscb_base_addr);
            rumboot_printf("WDMA channel detailed status : 0x%x\n", hscb0_dma_status);
            if(hscb0_dma_status & 0x00000001) 
                switch(hscb_base_addr){
                    case HSCB0_BASE   : hscb0_rx_irq_cnt = hscb0_rx_irq_cnt + 1; break;
                    case HSCB1_BASE   : hscb1_rx_irq_cnt = hscb1_rx_irq_cnt + 1; break;
                    case HSCB2_BASE   : hscb2_rx_irq_cnt = hscb2_rx_irq_cnt + 1; break;
                    case HSCB3_BASE   : hscb3_rx_irq_cnt = hscb3_rx_irq_cnt + 1; break;
                }
            rumboot_printf("WDMA-RX interrupts counter:\n  hscb0_rx_irq_cnt = 0x%x\n  hscb1_rx_irq_cnt = 0x%x\n  hscb2_rx_irq_cnt = 0x%x\n  hscb3_rx_irq_cnt = 0x%x\n", hscb0_rx_irq_cnt, hscb1_rx_irq_cnt, hscb2_rx_irq_cnt, hscb3_rx_irq_cnt);
        }
        rumboot_printf("0x%x\n", hscb0_dma_status);
    }


}
//***********************************************************************************************************************************************************************************************************
struct rumboot_irq_entry * create_irq_handlers()
{
    rumboot_irq_cli();
    struct rumboot_irq_entry *tbl = rumboot_irq_create( NULL );

    rumboot_irq_set_handler( tbl, SW0_HSCB_INT  ,   RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, hscb_handler, ( void* )0 );
    rumboot_irq_set_handler( tbl, SW0_AXI_INT   ,   RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, hscb_handler, ( void* )0 );
    rumboot_irq_set_handler( tbl, SW1_HSCB_INT  ,   RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, hscb_handler, ( void* )0 );
    rumboot_irq_set_handler( tbl, SW1_AXI_INT   ,   RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, hscb_handler, ( void* )0 );
    rumboot_irq_set_handler( tbl, SW2_HSCB_INT  ,   RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, hscb_handler, ( void* )0 );
    rumboot_irq_set_handler( tbl, SW2_AXI_INT   ,   RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, hscb_handler, ( void* )0 );
    rumboot_irq_set_handler( tbl, SW3_HSCB_INT  ,   RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, hscb_handler, ( void* )0 );
    rumboot_irq_set_handler( tbl, SW3_AXI_INT   ,   RUMBOOT_IRQ_LEVEL | RUMBOOT_IRQ_HIGH, hscb_handler, ( void* )0 );
    

    /* Activate the table */
    rumboot_irq_table_activate( tbl );
    rumboot_irq_enable( SW0_HSCB_INT );
    rumboot_irq_enable( SW0_AXI_INT  );
    rumboot_irq_enable( SW1_HSCB_INT );
    rumboot_irq_enable( SW1_AXI_INT  );
    rumboot_irq_enable( SW2_HSCB_INT );
    rumboot_irq_enable( SW2_AXI_INT  );
    rumboot_irq_enable( SW3_HSCB_INT );
    rumboot_irq_enable( SW3_AXI_INT  );
    
    
    rumboot_irq_sei();

     return tbl;
}
//***********************************************************************************************************************************************************************************************************
void delete_irq_handlers(struct rumboot_irq_entry *tbl)
{
    rumboot_irq_table_activate(NULL);
    rumboot_irq_free(tbl);
}
//***********************************************************************************************************************************************************************************************************
void wait_of_end(){
    uint32_t i=0;
    while(hscb0_rx_irq_cnt * hscb1_rx_irq_cnt * hscb2_rx_irq_cnt * hscb3_rx_irq_cnt == 0) {
        i = i+1;
        if(i%1000 == 0) rumboot_printf("Current status at %d iteration: hscb0_rx_irq_cnt = %d, hscb1_rx_irq_cnt = %d, hscb2_rx_irq_cnt = %d, hscb3_rx_irq_cnt = %d\n", i, hscb0_rx_irq_cnt, hscb1_rx_irq_cnt, hscb2_rx_irq_cnt, hscb3_rx_irq_cnt);}
    rumboot_printf("Reach end of test: hscb0_rx_irq_cnt = %d, hscb1_rx_irq_cnt = %d, hscb2_rx_irq_cnt = %d, hscb3_rx_irq_cnt = %d\n", hscb0_rx_irq_cnt, hscb1_rx_irq_cnt, hscb2_rx_irq_cnt, hscb3_rx_irq_cnt);
}

int main() {
// -----------------------------------------------------------
    uint32_t result = 0;
    uint32_t rdma_ctrl_word[4];
    struct rumboot_irq_entry *tbl;
    hscb_descr *hscb0_tx_dsc_table, *hscb0_rx_dsc_table; 
    hscb_descr *hscb1_tx_dsc_table, *hscb1_rx_dsc_table; 
    hscb_descr *hscb2_tx_dsc_table, *hscb2_rx_dsc_table; 
    hscb_descr *hscb3_tx_dsc_table, *hscb3_rx_dsc_table;
    hscb_descr **tx_dsc_tables, **rx_dsc_tables;
// -----------------------------------------------------------    
    if(DEBUG_PRINT) rumboot_printf("Start of test\n");
    // ---- Config of MMU
    static struct tlb_entry const em2_nospeculative_tlb_entries[] =
    {
    /*   MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,   IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )*/
        {MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b0,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )},
        {MMU_TLB_ENTRY(  0x000,  0x40000,    0x40000,    MMU_TLBE_DSIZ_1GB,      0b1,    0b1,    0b0,    0b1,    0b0,    0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b0,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )}
    };
    write_tlb_entries(em2_nospeculative_tlb_entries, ARRAY_SIZE(em2_nospeculative_tlb_entries));
    // --- init EMI
    emi_init(DCR_EM2_EMI_BASE);
    tbl = create_irq_handlers();
    // ---- Config of HSCB     
    if(DEBUG_PRINT) rumboot_printf("----Config of HSCB0----\n");
    config_hscb(HSCB0_BASE, &hscb0_tx_dsc_table, HSCB0_TX_DSCTBL_BASE, HSCB0_TX_DATA_BASE, &hscb0_rx_dsc_table, HSCB0_RX_DSCTBL_BASE, HSCB0_RX_DATA_BASE, &rdma_ctrl_word[0]);
    if(DEBUG_PRINT) rumboot_printf("----Config of HSCB1----\n");
    config_hscb(HSCB1_BASE, &hscb1_tx_dsc_table, HSCB1_TX_DSCTBL_BASE, HSCB1_TX_DATA_BASE, &hscb1_rx_dsc_table, HSCB1_RX_DSCTBL_BASE, HSCB1_RX_DATA_BASE, &rdma_ctrl_word[1]);
    if(DEBUG_PRINT) rumboot_printf("----Config of HSCB2----\n");
    config_hscb(HSCB2_BASE, &hscb2_tx_dsc_table, HSCB2_TX_DSCTBL_BASE, HSCB2_TX_DATA_BASE, &hscb2_rx_dsc_table, HSCB2_RX_DSCTBL_BASE, HSCB2_RX_DATA_BASE, &rdma_ctrl_word[2]);
    if(DEBUG_PRINT) rumboot_printf("----Config of HSCB3----\n");
    config_hscb(HSCB3_BASE, &hscb3_tx_dsc_table, HSCB3_TX_DSCTBL_BASE, HSCB3_TX_DATA_BASE, &hscb3_rx_dsc_table, HSCB3_RX_DSCTBL_BASE, HSCB3_RX_DATA_BASE, &rdma_ctrl_word[3]);
    //---------------- 
    rumboot_printf("HSCB0 descriptor tables\n");
    rumboot_printf("--- TX descriptors table\n");
    if(DEBUG_PRINT) print_dscr_table(hscb0_tx_dsc_table, false);
    print_dscr_table(hscb0_tx_dsc_table, CHANGE_ENDIANN);
    rumboot_printf("--- RX descriptors table\n");
    if(DEBUG_PRINT) print_dscr_table(hscb0_rx_dsc_table, false);
    print_dscr_table(hscb0_rx_dsc_table, CHANGE_ENDIANN);
    rumboot_printf("HSCB1 descriptor tables\n");
    rumboot_printf("--- TX descriptors table\n");
    if(DEBUG_PRINT) print_dscr_table(hscb1_tx_dsc_table, false);
    print_dscr_table(hscb1_tx_dsc_table, CHANGE_ENDIANN);
    rumboot_printf("--- RX descriptors table\n");
    if(DEBUG_PRINT) print_dscr_table(hscb1_rx_dsc_table, false);
    print_dscr_table(hscb1_rx_dsc_table, CHANGE_ENDIANN);
    rumboot_printf("HSCB2 descriptor tables\n");
    rumboot_printf("--- TX descriptors table\n");
    if(DEBUG_PRINT) print_dscr_table(hscb2_tx_dsc_table, false);
    print_dscr_table(hscb2_tx_dsc_table, CHANGE_ENDIANN);
    rumboot_printf("--- RX descriptors table\n");
    if(DEBUG_PRINT) print_dscr_table(hscb2_rx_dsc_table, false);
    print_dscr_table(hscb2_rx_dsc_table, CHANGE_ENDIANN);
    rumboot_printf("HSCB3 descriptor tables\n");
    rumboot_printf("--- TX descriptors table\n");
    if(DEBUG_PRINT) print_dscr_table(hscb3_tx_dsc_table, false);
    print_dscr_table(hscb3_tx_dsc_table, CHANGE_ENDIANN);
    rumboot_printf("--- RX descriptors table\n");
    if(DEBUG_PRINT) print_dscr_table(hscb3_rx_dsc_table, false);
    print_dscr_table(hscb3_rx_dsc_table, CHANGE_ENDIANN);
    //---------------- 
    establish_link(HSCB0_BASE, HSCB1_BASE);
    establish_link(HSCB2_BASE, HSCB3_BASE);
    simult_run_of_hscbs_transfers(rdma_ctrl_word);
    wait_of_end();
    //---------------- 
    delete_irq_handlers(tbl);
    return result;
}


