/*
 * test_oi10_cpu_power.c
 *
 *  Created on: 
 *      Author: 
 */

//#include <arch/common_macros.h>
//#include <platform/test_event_codes.h>
//#include <platform/devices.h>
//#include <rumboot/platform.h>
//
//#include <rumboot/io.h>
//#include <rumboot/printf.h>
//#include <platform/trace.h>


#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <rumboot/regpoker.h>
#include <platform/devices.h>
#include <platform/interrupts.h>
#include <rumboot/irq.h>
#include <rumboot/rumboot.h>
#include <rumboot/memfill.h>
#include <devices/ugly/hscb.h>
#include <devices/ugly/emi.h>
#include <arch/ppc_476fp_mmu.h>
#include <devices/com_simple.h>
#include <devices/rcm_cp.h>
#include <devices/gpio_pl061.h>
#include <arch/dma2plb6.h>
#include <regs/regs_gpio_pl061.h>

#ifndef MATRIX_DIMENTION
#define MATRIX_DIMENTION 4
#endif

#ifndef N_OF_PACKETS
#define N_OF_PACKETS 2
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

#define CACHEABLE_IM0            0x80090000
#define CACHEABLE_IM0_CODE       (CACHEABLE_IM0)
#define CACHEABLE_IM0_CODE_SIZE  (16*1024)
#define CACHEABLE_IM0_DATA       (CACHEABLE_IM0_CODE + CACHEABLE_IM0_CODE_SIZE)
#define CACHEABLE_IM0_DATA_SIZE  (32*1024)

#define UNCACHEABLE_IM0             (IM0_BASE + 64*1024)
#define UNCACHEABLE_IM0_CODE        (UNCACHEABLE_IM0)
#define UNCACHEABLE_IM0_CODE_SIZE   (CACHEABLE_IM0_CODE_SIZE)
#define UNCACHEABLE_IM0_DATA        (UNCACHEABLE_IM0_CODE + CACHEABLE_IM0_CODE_SIZE)
#define UNCACHEABLE_IM0_DATA_SIZE   (CACHEABLE_IM0_DATA_SIZE)

#define UNCACHEABLE_SRAM0   0x00100000
#define PPC476FP_L2_CACHELINE_SIZE 128u 


#ifdef COMP_FOR_POWER
#define ARR_SIZE 128
#define COM0_BASE 0xC0304000
#define COM1_BASE 0xC0305000
#ifndef COM_SRC_HEAP
#define COM_SRC_HEAP "IM1"
#endif
#ifndef COM_DST_HEAP
#define COM_DST_HEAP "IM1"
#endif
#endif

#define BUF_SIZE 4096
//#define TIMEOUT (BUF_SIZE*50)
#define WORD_SIZE 8
#ifndef BASE_FREQ
    #define BASE_FREQ 50000
#endif

static volatile uint32_t hscb0_status;
static volatile uint32_t hscb1_status;
static volatile uint32_t hscb0_dma_status;
static volatile uint32_t hscb1_dma_status;
static volatile uint32_t cmc4p;
static volatile bool hscb0_link_established;
static volatile bool hscb1_link_established;
volatile uint32_t hscb0_rx_irq_cnt, hscb1_rx_irq_cnt, hscb2_rx_irq_cnt, hscb3_rx_irq_cnt;
uint32_t *int_A, *int_B, *int_res;
float *flt_A, *flt_B, *flt_res;


//extern void check_multiple_channels_4_power();
//extern void dma2plb6_4ch_init_pwr();
//extern void dma2plb6_4ch_start();
//extern void dma2plb6_4ch_finish();

struct dma2plb6_setup_info ch0_dma_info_p;
struct dma2plb6_setup_info ch1_dma_info_p;
struct dma2plb6_setup_info ch2_dma_info_p;
struct dma2plb6_setup_info ch3_dma_info_p;
struct channel_status ch0_status_p = {};
struct channel_status ch1_status_p = {};
struct channel_status ch2_status_p = {};
struct channel_status ch3_status_p = {};
uint32_t *ch0_src_p;
uint32_t *ch1_src_p;
uint32_t *ch2_src_p;
uint32_t *ch3_src_p;
uint32_t *ch0_dst_p;
uint32_t *ch1_dst_p;
uint32_t *ch2_dst_p;
uint32_t *ch3_dst_p;

static uint32_t dma2plb6_get_bytesize(transfer_width transfer_width_code)
{
    switch(transfer_width_code){
    case tr_width_byte:
        return 1;
        break;
    case tr_width_halfword:
        return 2;
        break;
    case tr_width_word:
        return 4;
        break;
    case tr_width_doubleword:
        return 8;
        break;
    case tr_width_quadword:
        return 16;
        break;
    }
    return 0;
}

static struct dma2plb6_setup_info get_default_dma_info(DmaChannel channel, uint32_t* ch_src, uint32_t* ch_dst)
{
    struct dma2plb6_setup_info dma_info;
    uint64_t src1, src2, dst;
        
    rumboot_printf("DMA2PLB6-inf: channel = %d, ch_src = %x, ch_dst = %x\n", channel, ch_src, ch_dst);
    dma_info.base_addr = DCR_DMAPLB6_BASE;
    //src1 = rumboot_virt_to_dma64(ch_src);
    //src2 = get_physical_addr( (uint32_t) ch_src, 0);
    //rumboot_printf("DMA2PLB6-inf: channel = %d, src1 = %x%x, src2 = %x%x\n", channel, (uint32_t)(src1>>32), (uint32_t)src1, (uint32_t)(src2>>32), (uint32_t)src2);
    dma_info.source_adr = get_physical_addr( (uint32_t) ch_src, 0);
    //dst = rumboot_virt_to_dma64(ch_dst);
    dma_info.dest_adr = get_physical_addr( (uint32_t) ch_dst, 0);
    //rumboot_printf("DMA2PLB6-inf: channel = %d, dst = %x%x\n", channel, (uint32_t)(dst>>32), (uint32_t)dst);
    dma_info.priority = priority_medium_low;
    dma_info.striding_info.striding = striding_none;
    dma_info.tc_int_enable = false;
    dma_info.err_int_enable = false;
    dma_info.int_mode = int_mode_level_wait_clr;
    dma_info.channel = channel;
    dma_info.transfer_width = tr_width_quadword;
    dma_info.rw_transfer_size = rw_tr_size_8q;
    dma_info.count = TEST_DATA_SIZE/dma2plb6_get_bytesize(dma_info.transfer_width);
    rumboot_printf("DMA2PLB6-inf: dma_info.channel = %d, dma_info.source_adr = %x%x, dma_info.dest_adr = %x%x\n", dma_info.channel, (uint32_t)(dma_info.source_adr>>32), (uint32_t)dma_info.source_adr, (uint32_t)(dma_info.dest_adr>>32),(uint32_t)dma_info.dest_adr);
    return dma_info;
}


void plb6dma_init(uint32_t src_start_addr, uint32_t dst_start_addr){
//---    
    rumboot_printf("PLB6DMA: Start of initialization\n");
    ch0_src_p = src_start_addr;
    ch1_src_p = src_start_addr+0x2000;
    ch2_src_p = src_start_addr+0x4000;
    ch3_src_p = src_start_addr+0x6000;
    ch0_dst_p = dst_start_addr;
    ch1_dst_p = dst_start_addr+0x2000;
    ch2_dst_p = dst_start_addr+0x4000;
    ch3_dst_p = dst_start_addr+0x6000;
    rumboot_printf("PLB6DMA-SRC: ch0_src_p = %x, ch1_src_p = %x, ch2_src_p = %x, ch3_src_p = %x \n", ch0_src_p, ch1_src_p, ch2_src_p, ch3_src_p);
    rumboot_printf("PLB6DMA-DST: ch0_dst_p = %x, ch1_dst_p = %x, ch2_dst_p = %x, ch3_dst_p = %x \n", ch0_dst_p, ch1_dst_p, ch2_dst_p, ch3_dst_p);
    rumboot_memfill32(ch0_dst_p, 0x2000, 0x00000000, 0);
    rumboot_memfill32(ch1_dst_p, 0x2000, 0x00000000, 0);
    rumboot_memfill32(ch2_dst_p, 0x2000, 0x00000000, 0);
    rumboot_memfill32(ch3_dst_p, 0x2000, 0x00000000, 0);   
    ch0_dma_info_p = get_default_dma_info(channel0, ch0_src_p, ch0_dst_p);
    ch1_dma_info_p = get_default_dma_info(channel1, ch1_src_p, ch1_dst_p);
    ch2_dma_info_p = get_default_dma_info(channel2, ch2_src_p, ch2_dst_p);
    ch3_dma_info_p = get_default_dma_info(channel3, ch3_src_p, ch3_dst_p);
    dma2plb6_mcpy_init(&ch0_dma_info_p);
    dma2plb6_mcpy_init(&ch1_dma_info_p);
    dma2plb6_mcpy_init(&ch2_dma_info_p);
    dma2plb6_mcpy_init(&ch3_dma_info_p);
    rumboot_printf("PLB6DMA: End of initialization\n");   
}
void plb6dma_simult_start(){
    dma2plb6_enable_channel( ch0_dma_info_p.base_addr, ch0_dma_info_p.channel );
    dma2plb6_enable_channel( ch1_dma_info_p.base_addr, ch1_dma_info_p.channel );
    dma2plb6_enable_channel( ch2_dma_info_p.base_addr, ch2_dma_info_p.channel );
    dma2plb6_enable_channel( ch3_dma_info_p.base_addr, ch3_dma_info_p.channel );
}

void plb6dma_simult_finish()
{
    rumboot_putstring("DMA2PLB6: Waiting for DMA transfer complete ...\n");
    if (wait_dma2plb6_mcpy(&ch0_dma_info_p, &ch0_status_p))
    {
        rumboot_putstring("DMA2PLB6: Channel 0 transfer succeeded.\n");
        if (wait_dma2plb6_mcpy(&ch1_dma_info_p, &ch1_status_p))
        {
            rumboot_putstring("DMA2PLB6: Channel 1 transfer succeeded.\n");
            if (wait_dma2plb6_mcpy(&ch2_dma_info_p, &ch2_status_p))
            {
                rumboot_putstring("DMA2PLB6: Channel 2 transfer succeeded.\n");
                if (wait_dma2plb6_mcpy(&ch3_dma_info_p, &ch3_status_p))
                {
                    rumboot_putstring("DMA2PLB6: Channel 3 transfer succeeded.\n");
                }
                else
                {
                    rumboot_putstring("DMA2PLB6: Channel 3 transfer error.\n");
                    //return 1;
                }
            }
            else
            {
                rumboot_putstring("DMA2PLB6: Channel 2 transfer error.\n");
                //return 1;
            }
        }
        else
        {
            rumboot_putstring("DMA2PLB6: Channel 1 transfer error.\n");
            //return 1;
        }
    }
    else
    {
        rumboot_putstring("DMA2PLB6: Channel 0 transfer error.\n");
        //return 1;
    }

    /*
    rumboot_putstring("DMA2PLB6: Memory check ...\n");
    TEST_ASSERT(compare((uint32_t)ch0_src, (uint32_t)ch0_dst, TEST_DATA_SIZE) == true, "Channel0: memory comparison failed");
    TEST_ASSERT(compare((uint32_t)ch1_src, (uint32_t)ch1_dst, TEST_DATA_SIZE) == true, "Channel1: memory comparison failed");
    TEST_ASSERT(compare((uint32_t)ch2_src, (uint32_t)ch2_dst, TEST_DATA_SIZE) == true, "Channel2: memory comparison failed");
    TEST_ASSERT(compare((uint32_t)ch3_src, (uint32_t)ch3_dst, TEST_DATA_SIZE) == true, "Channel3: memory comparison failed");
    */

    //return 0;
}



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
    //---- hack
    dsc_table_pt = &dsc_table[num_of_descr];
    dscr_p = (uint64_t*)(dsc_table_pt);
    *dscr_p = 0;
    msync();
    // ---- 
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

void change_descr_endianess (hscb_descr *src_dsc, hscb_descr *dst_dsc) {
    hscb_descr *result_dsc;
    uint64_t *dscr_p, dsc_loc;
    uint32_t word1, word2;
    dscr_p = (uint64_t*)(src_dsc);
    word1 = *dscr_p >> 32;
    word2 = *dscr_p;
    dscr_p = (uint64_t*)(dst_dsc);
    word1 = hscb_change_endian(word1, 1);
    word2 = hscb_change_endian(word2, 1);
    *dscr_p = 0;
    *dscr_p = word2;
    *dscr_p = (*dscr_p << 32);
    *dscr_p = *dscr_p | word1;
    //src_dsc = (hscb_descr*)dscr_p;
}
void print_dscr_table(hscb_descr *dsc_table, bool ch_end) {
    uint32_t i, word1, word2;
    uint64_t *dscr_p;
    hscb_descr *dsc, dsc_loc;
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
            //dsc_loc = 0;
            //dsc_loc = word2;
            //dsc_loc = (dsc_loc << 32);
            //dsc_loc = dsc_loc | word1;
            //dscr_p = &dsc_loc;
            //dsc = (hscb_descr*)dscr_p;
            change_descr_endianess(dsc, &dsc_loc);
        }
        rumboot_printf("N=%d, word1 = %x, word2 = %x, start_address = %x, length = %x, act = %x, act0 = %x, ie = %x, err = %x, valid = %x\n", i, word1, word2, dsc_loc.start_address, dsc_loc.length, dsc_loc.act, dsc_loc.act0, dsc_loc.ie, dsc_loc.err, dsc_loc.valid);
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
        frame_buf = (uint32_t*)rumboot_malloc_from_named_heap_aligned(hscb_data_heap_name, descr.length, 0x8);
        descr.start_address = rumboot_virt_to_dma(frame_buf);
        descr.act    =   2;      
        descr.act0   =   1;      
        if(i == n_of_dsc-1) descr.ie = 1; else descr.ie = 0;          
        descr.valid  =   1;
        init_descr(dsc_table, i, &descr);
        rumboot_memfill32(frame_buf, descr.length/4, descr.start_address, 0x1);
        if(DEBUG_PRINT) rumboot_printf("First word = %x, Last word = %x \n", *(frame_buf), *(frame_buf+descr.length/4-1));
        
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

// Add by M.Chelyshev as glitch-protect
uint32_t disable_hscb (uint32_t hscb_base){
    uint32_t tmp = 0;
    tmp = ioread32(hscb_base + HSCB_SETTINGS);
    tmp = tmp & 0xfffffffe;
    iowrite32(tmp, hscb_base + HSCB_SETTINGS);
}
// ------------------------------------

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
    hscb_set_rdma_sys_addr(hscb_base_addr, rumboot_virt_to_dma(*hscb_tx_dsc_table));
    hscb_set_wdma_tbl_size(hscb_base_addr, N_OF_PACKETS * sizeof(hscb_descr));
    hscb_set_wdma_sys_addr(hscb_base_addr, rumboot_virt_to_dma(*hscb_rx_dsc_table));
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
    iowrite32(15, HSCB0_BASE + HSCB_AWLEN);
    iowrite32(15, HSCB1_BASE + HSCB_AWLEN);
    iowrite32(15, HSCB2_BASE + HSCB_AWLEN);
    iowrite32(15, HSCB3_BASE + HSCB_AWLEN);
    iowrite32(15, HSCB0_BASE + HSCB_ARLEN);
    iowrite32(15, HSCB1_BASE + HSCB_ARLEN);
    iowrite32(15, HSCB2_BASE + HSCB_ARLEN);
    iowrite32(15, HSCB3_BASE + HSCB_ARLEN);
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


int check_results(hscb_descr *tx_dsc_table, hscb_descr *rx_dsc_table) {
    uint32_t i, result, *tx_word, *rx_word;
    hscb_descr tx_dsc, rx_dsc;
    result = 0;
    rumboot_printf("Start checks of results\n");
    for (i=0; i < N_OF_PACKETS; i=i+1) {
        change_descr_endianess(&tx_dsc_table[i], &tx_dsc);
        change_descr_endianess(&rx_dsc_table[i], &rx_dsc);
        //tx_dsc = &tx_dsc_table[i];
        //rx_dsc = &rx_dsc_table[i];
        rumboot_printf("TX_DSC: address = %x, N=%d, start_address = %x, length = %x, act = %x, act0 = %x, ie = %x, err = %x, valid = %x\n", &tx_dsc_table[i], i, tx_dsc.start_address, tx_dsc.length, tx_dsc.act, tx_dsc.act0, tx_dsc.ie, tx_dsc.err, tx_dsc.valid);
        rumboot_printf("RX_DSC: address = %x, N=%d, start_address = %x, length = %x, act = %x, act0 = %x, ie = %x, err = %x, valid = %x\n", &rx_dsc_table[i], i, rx_dsc.start_address, rx_dsc.length, rx_dsc.act, rx_dsc.act0, rx_dsc.ie, rx_dsc.err, rx_dsc.valid);
        if(rumboot_memcheck32(tx_dsc.start_address, rx_dsc.start_address, tx_dsc.length/4)==1) return 1;
        //for(i=0; i < tx_dsc.length/4; i=i+4) {
        //    tx_word = (tx_dsc.start_address + i);
        //    rx_word = (rx_dsc.start_address + i);
        //   //rumboot_printf("CMP: TX - %x at addr %x, RX - %x at addr %x\n", *tx_word, tx_word, *rx_word, rx_word);
        //    if(*tx_word !=*rx_word) { 
        //        rumboot_printf("CMP_ERROR\n"); 
        //        return 1;
        //    }
        //}
    }
    return 0; 
}

typedef uint8_t func();

                        //MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,             IL1I,            IL1D,           W,         I,    M,    G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,           V   )
//#define TLB_ENTRY_WT    MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,  L2C_IL1I_BIT,   L2C_IL1D_BIT,  L2C_W_BIT,  0b0,  0b1,  0b1,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )
//#define TLB_ENTRY_WT    MMU_TLB_ENTRY(  0x000,  0x00000,    0x00000,    MMU_TLBE_DSIZ_1GB,  L2C_IL1I_BIT,   L2C_IL1D_BIT,  L2C_W_BIT,  0b0,  0b1,  0b0,    MMU_TLBE_E_BIG_END,     0b0,0b0,0b0,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_0,       MMU_TLBWE_WAY_3,    MMU_TLBWE_BE_UND,   0b1 )

void print_int_matrix(uint32_t *arr, uint32_t N) {
    uint32_t i, j;
    for(i=0; i<N; i++) 
        for(j=0; j<N; j++){
            rumboot_printf("%d ", *(arr+i*N+j));
            if(j==N-1) rumboot_printf("\n");
        }
}

void print_flt_matrix(float *arr, uint32_t N) {
    uint32_t i, j;
    for(i=0; i<N; i++) 
        for(j=0; j<N; j++){
            rumboot_printf("%x ", (uint32_t)(*(arr+i*N+j)));
            if(j==N-1) rumboot_printf("\n");
        }
}

void simult_matrix_multiply_prepare(uint32_t N){
    rumboot_printf("Start of simult_matrix_multiply_prepare");
    int_A = (uint32_t*)rumboot_malloc_from_named_heap_aligned("SRAM0", N*N, 0x4);
    int_B = (uint32_t*)rumboot_malloc_from_named_heap_aligned("SRAM0", N*N, 0x4);
    int_res = (uint32_t*)rumboot_malloc_from_named_heap_aligned("SRAM0", N*N, 0x4);
    flt_A = (float*)rumboot_malloc_from_named_heap_aligned("SRAM0", N*N, 0x4);
    flt_B = (float*)rumboot_malloc_from_named_heap_aligned("SRAM0", N*N, 0x4);
    flt_res = (float*)rumboot_malloc_from_named_heap_aligned("SRAM0", N*N, 0x4);
    rumboot_printf("int_A = %x, int_B = %x, int_res =%x, flt_A = %x, flt_B = %x, flt_res =%x, N = %x\n", int_A, int_B, int_res, flt_A, flt_B, flt_res, N);
}

void simult_matrix_multiply_start(uint32_t N){
    uint32_t i, j, k;
    //gpio_set_value_by_mask( GPIO_1_BASE, GPIO_REG_MASK, 0x80);
    rumboot_printf("Start of simult_matrix_multiply_start\n");
    for(i = 0; i < N; i++){
        //rumboot_printf("Outer loop, itreration %x\n", i);
        for(j = 0; j < N; j++){
            //rumboot_printf("Inner loop, itreration %x\n", j);
            for(k = 0; k < N; k++) {
                //int_res[i][j] += int_A[i][k] * int_B[k][j];
                //flt_res[i][j] += flt_A[i][k] * flt_B[k][j];
                *(int_res+i*N+j) += *(int_A+i*N+k) * (*(int_B+k*N+j));
                *(flt_res+i*N+j) += *(flt_A+i*N+k) * (*(flt_B+k*N+j));
            }
            //rumboot_printf("int_res[%d][%d] = %x\n", i, j, *(int_res+i*N+j));
            //rumboot_printf("flt_res[%d][%d] = %f\n", i, j, *(flt_res+i*N+j));
        }
    }
    rumboot_printf("End of simult_matrix_multiply_start\n");
    //gpio_set_value_by_mask( GPIO_1_BASE, GPIO_REG_MASK, 0x00);
}

int main(void)
{
      // -----------------------------------------------------------
      uint32_t rdma_ctrl_word[4], result;
      uint32_t *int_A, *int_B, *int_res;
      float *flt_A, *flt_B, *flt_res;      
      uint64_t paddr;
      uint32_t *plb6dma_dst, *test_addr;
      struct rumboot_irq_entry *tbl;
      hscb_descr *hscb0_tx_dsc_table, *hscb0_rx_dsc_table; 
      hscb_descr *hscb1_tx_dsc_table, *hscb1_rx_dsc_table; 
      hscb_descr *hscb2_tx_dsc_table, *hscb2_rx_dsc_table; 
      hscb_descr *hscb3_tx_dsc_table, *hscb3_rx_dsc_table;
      hscb_descr **tx_dsc_tables, **rx_dsc_tables;
  // -----------------------------------------------------------   
  
  #ifdef COMP_FOR_POWER 
  // for com port (from test_com_es_elaborate)
      uint32_t* tx_buf;
      uint32_t* rx_buf;
      struct rcm_cp_instance c0; 
      struct rcm_cp_instance c1; 
      int r;
      int size;
      //int timeout_us;  
    
      size = BUF_SIZE;
      //timeout_us = TIMEOUT;
      ////
  #endif
  
      #ifndef EMI_INIT_FOR_POWER
      //emi_init(DCR_EM2_EMI_BASE);
      #else
      emi_init(DCR_EM2_EMI_BASE);
      #endif
      
      tbl = create_irq_handlers();  
  
      #ifdef COMP_FOR_POWER
      rumboot_printf("Running power test for O32T project\n");
// #########################################################
// ### Run scrubbers for IM1-IM3
// #########################################################
      iowrite32(0x8, 0xC0307008); // run scrubber in IM1-IM3 in ALL-IM-VOLUME mode
      iowrite32(0xF, 0xC0307000); // run scrubber in IM1-IM3 in ALL-IM-VOLUME mode
      #else
      rumboot_printf("Power test for OI10 project\n");
      #endif
  
      rumboot_printf("Init mem... ");
      memset((void*)(SRAM0_BASE), 0x00, 0x28000);
      rumboot_printf("done\n");
      // --- 
      rumboot_printf( "Initialize cacheable IM0...");
      rumboot_memfill32(UNCACHEABLE_IM0_DATA, UNCACHEABLE_IM0_DATA_SIZE/4, 0xA5A5A5A5, 0x01010101);
      rumboot_printf("done\n");
  
      rumboot_printf("Load bin from 0x%x\n", UNCACHEABLE_IM0_CODE);
      rumboot_platform_request_file("MBIN", UNCACHEABLE_IM0_CODE);
      asm("isync");
      asm("sync");
// #########################################################
// ### Prepare of HSCB0-HSCB3
// #########################################################     
  // Add by M.Chelyshev as glitch-protect
      if(DEBUG_PRINT) rumboot_printf( "Disable all HSCB controllers\n");
      disable_hscb(HSCB0_BASE);    
      disable_hscb(HSCB1_BASE);    
      disable_hscb(HSCB2_BASE);    
      disable_hscb(HSCB3_BASE);
  // ------------------------------------
      if(DEBUG_PRINT) rumboot_printf("----Config of HSCB0----\n");
      config_hscb(HSCB0_BASE, &hscb0_tx_dsc_table, HSCB0_TX_DSCTBL_BASE, HSCB0_TX_DATA_BASE, &hscb0_rx_dsc_table, HSCB0_RX_DSCTBL_BASE, HSCB0_RX_DATA_BASE, &rdma_ctrl_word[0]);
      if(DEBUG_PRINT) rumboot_printf("----Config of HSCB1----\n");
      config_hscb(HSCB1_BASE, &hscb1_tx_dsc_table, HSCB1_TX_DSCTBL_BASE, HSCB1_TX_DATA_BASE, &hscb1_rx_dsc_table, HSCB1_RX_DSCTBL_BASE, HSCB1_RX_DATA_BASE, &rdma_ctrl_word[1]);
      if(DEBUG_PRINT) rumboot_printf("----Config of HSCB2----\n");
      config_hscb(HSCB2_BASE, &hscb2_tx_dsc_table, HSCB2_TX_DSCTBL_BASE, HSCB2_TX_DATA_BASE, &hscb2_rx_dsc_table, HSCB2_RX_DSCTBL_BASE, HSCB2_RX_DATA_BASE, &rdma_ctrl_word[2]);
      if(DEBUG_PRINT) rumboot_printf("----Config of HSCB3----\n");
      config_hscb(HSCB3_BASE, &hscb3_tx_dsc_table, HSCB3_TX_DSCTBL_BASE, HSCB3_TX_DATA_BASE, &hscb3_rx_dsc_table, HSCB3_RX_DSCTBL_BASE, HSCB3_RX_DATA_BASE, &rdma_ctrl_word[3]);
      
      establish_link(HSCB0_BASE, HSCB1_BASE);
      establish_link(HSCB2_BASE, HSCB3_BASE);
      
      #ifdef COMP_FOR_POWER
// #########################################################
// ### Init of COM0-COM1 (1st part)
// #########################################################   
      cp_instance_init(&c0, COM0_BASE, BASE_FREQ);
      cp_instance_init(&c1, COM1_BASE, BASE_FREQ);
      rumboot_printf("----\n");
      r = cp_set_speed(&c0,BASE_FREQ); // WE GOT AN EXCEPTION: 15: Data TLB Error
      r = cp_set_speed(&c1,BASE_FREQ);    
      rumboot_printf("Initialized instances 0x%x 0x%x\n",c0.base,c1.base);
      // Test 1 - Simple Transmit And Receive
      tx_buf = rumboot_malloc_from_named_heap_aligned(COM_SRC_HEAP, size, 8);
      rx_buf = rumboot_malloc_from_named_heap_aligned(COM_DST_HEAP, size, 8);      
      if(tx_buf==NULL || rx_buf==NULL) return -1;
      rumboot_printf("Allocated tx_buf=0x%x, rx_buf=0x%x\n",(uint32_t) tx_buf,(uint32_t) rx_buf);
      memset(rx_buf,0xA5,size);
      rumboot_memfill32(tx_buf,size/4,0x00010203,0x04040404);
      #endif    
// #########################################################
// ### Init of PLB6DMA
// #########################################################        
      plb6dma_dst = (uint32_t*)rumboot_malloc_from_named_heap_aligned("SSRAM", 16*1024, 0x8);
      //dma2plb6_4ch_init_pwr(UNCACHEABLE_IM0_DATA, plb6dma_dst);
      plb6dma_init(UNCACHEABLE_IM0_DATA, plb6dma_dst);
  
  //----- Active part 
      test_addr = 0x80014000;
      paddr = get_physical_addr( (uint32_t) test_addr, 0);
      rumboot_printf("Virtual address = %x, physical_address = %x%x\n", test_addr, (uint32_t)(paddr>>32), (uint32_t)paddr);
// #########################################################
// ### Init of TLB - make IM0 cacheable
// #########################################################
    rumboot_printf("Init tlb...");
    static tlb_entry const wb_tlb_entries[] = {
//*******Make IM0 cacheble *****************************************************************************************************************************************************************************************
//       MMU_TLB_ENTRY(  ERPN,   RPN,        EPN,        DSIZ,                   IL1I,  IL1D,   W,      I,      M,      G,      E,                      UX, UW, UR,     SX, SW, SR      DULXE,  IULXE,      TS,     TID,                WAY,                BID,                V   )
        {MMU_TLB_ENTRY(  0x010,  0x80010,    0x80090,    MMU_TLBE_DSIZ_16KB,     0b0,   0b0,    0b0,    0b0,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b1,0b1,0b1,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_1,    MMU_TLBWE_BE_2,     0b1 )},
        {MMU_TLB_ENTRY(  0x010,  0x80014,    0x80094,    MMU_TLBE_DSIZ_16KB,     0b0,   0b0,    0b0,    0b0,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b1,0b1,0b1,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_1,    MMU_TLBWE_BE_2,     0b1 )},
        {MMU_TLB_ENTRY(  0x010,  0x80018,    0x80098,    MMU_TLBE_DSIZ_16KB,     0b0,   0b0,    0b0,    0b0,    0b0,    0b0,    MMU_TLBE_E_BIG_END,     0b1,0b1,0b1,    0b1,0b1,0b1,    0b0,    0b0,        0b0,    MEM_WINDOW_SHARED,  MMU_TLBWE_WAY_1,    MMU_TLBWE_BE_2,     0b1 )}
        };
        
    write_tlb_entries( wb_tlb_entries, ARRAY_SIZE(wb_tlb_entries) );
    rumboot_printf("done (%d enties)\n", ARRAY_SIZE(wb_tlb_entries));
    
    rumboot_printf("UNCACHEABLE_IM0_DATA init ... ");
    rumboot_memfill32(UNCACHEABLE_IM0_DATA, MATRIX_DIMENTION*MATRIX_DIMENTION, 0xA5, 0x0); // init int by 165
    rumboot_memfill32(UNCACHEABLE_IM0_DATA + MATRIX_DIMENTION*MATRIX_DIMENTION*4, MATRIX_DIMENTION*MATRIX_DIMENTION, 0xA6, 0x0); // init int by 166
    rumboot_memfill32(UNCACHEABLE_IM0_DATA + 2*MATRIX_DIMENTION*MATRIX_DIMENTION*4, 2*MATRIX_DIMENTION*MATRIX_DIMENTION, 0x401c0000, 0x0);// init float array by 2.9375
    //-----
    rumboot_printf("Done\n");
    

    #ifdef COMP_FOR_POWER

// #########################################################
// ### Init of COM0-COM1 (2nd part)
// #########################################################
    cp_prep_tx(&c0, tx_buf, size);
    cp_prep_rx(&c1, rx_buf, size);
    #endif

// #########################################################
// ### Start active part of test - run all DMA-based units + FPU
// #########################################################    
// Run HSCB0-HSCB3
    simult_run_of_hscbs_transfers(rdma_ctrl_word);
// Run PLB6DMA
    plb6dma_simult_start();
// Prepare FPU
  //rumboot_printf("Starting test code for FPU from %x\n", CACHEABLE_IM0_CODE);
  gpio_set_direction_by_mask( GPIO_1_BASE, 0x80, gpio_pin_direction_out );
  func* f = (func*)(CACHEABLE_IM0_CODE);
// Run COM0-COM1
    #ifdef COMP_FOR_POWER
    cp_go_tx(&c0, tx_buf, size);
    cp_go_rx(&c1, rx_buf, size);
    #endif  
// Run FPU calculations    
  result = f();
// #########################################################
// ### End active part of test - finish FPU calculations
// #########################################################  
  
  //rumboot_printf("Result of FPU = %d\n", result);
  //rumboot_printf("---- Matrix int_A\n");
  //print_int_matrix(int_A, MATRIX_DIMENTION);
  //rumboot_printf("---- Matrix int_B\n");
  //print_int_matrix(int_B, MATRIX_DIMENTION);
  //rumboot_printf("---- Matrix int_res\n");
  //print_int_matrix(int_res, MATRIX_DIMENTION);
  //rumboot_printf("---- Matrix flt_A\n");
  //print_flt_matrix(flt_A, MATRIX_DIMENTION);
  //rumboot_printf("---- Matrix flt_B\n");
  //print_flt_matrix(flt_B, MATRIX_DIMENTION);
  //rumboot_printf("---- Matrix flt_res\n");
  //print_flt_matrix(flt_res, MATRIX_DIMENTION);
      
    //-----------------
    plb6dma_simult_finish();    
    wait_of_end();
    delete_irq_handlers(tbl);
    return result;
}
